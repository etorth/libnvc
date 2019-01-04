/*
 * =====================================================================================
 *
 *       Filename: apiclient.cpp
 *        Created: 12/26/2018 01:59:10
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include <optional>
#include <stdexcept>
#include "mpack.h"
#include "libnvc.hpp"
#include "strfunc.hpp"

namespace libnvc
{
    class api_client::stream_decoder final
    {
        private:
            mpack_tree_t m_tree;

        public:
            stream_decoder(libnvc::socket *psocket)
                : m_tree()
            {
                if(!psocket){
                    throw std::invalid_argument(str_fflprintf(": Invalid socket: (nullptr)"));
                }
                // checked the code
                // seems this function always succeeds
                mpack_tree_init_stream(&m_tree, &read_socket, psocket, 1024 * 1024, 1024);
            }

        public:
            ~stream_decoder()
            {
                // don't free the socket
                // decoder doesn't own the lifecycle of socket
                mpack_tree_destroy(&m_tree);
            }

        public:
            std::optional<mpack_node_t> poll()
            {
                if(mpack_tree_try_parse(&m_tree)){
                    return mpack_tree_root(&m_tree);
                }

                // no message valid, two possible reasons:
                //   1. error occurred
                //   2. no sufficent data received

                switch(auto ec = mpack_tree_error(&m_tree)){
                    case mpack_ok:
                        {
                            return {};
                        }
                    default:
                        {
                            throw std::runtime_error(str_fflprintf(": Get mpack_tree_error(): %s", mpack_error_to_string(ec)));
                        }
                }
            }

        private:
            static size_t read_socket(mpack_tree_t *ptree, char *buf, size_t count)
            {
                if(auto psocket = (libnvc::socket *)(mpack_tree_context(ptree))){
                    return psocket->recv(buf, count);
                }
                throw std::runtime_error(str_fflprintf(": Empty context in mpack_tree_t: %p", ptree));
            }
    };
}

namespace
{
    template<typename T> T mp_read(mpack_node_t);

    template<typename T> inline void check_node_type(mpack_node_t node)
    {
        auto tag = mpack_node_tag(node);
        switch(auto type = mpack_tag_type(&tag)){
            case mpack_type_str:
                {
                    if(!std::is_same_v<T, std::string>){
                        throw std::runtime_error(str_fflprintf(": Invalid request for node std::string -> %s", typeid(T).name()));
                    }
                    break;
                }
            case mpack_type_int:
                {
                    if(!std::is_same_v<T, int64_t>){
                        throw std::runtime_error(str_fflprintf(": Invalid request for node int64_t -> %s", typeid(T).name()));
                    }
                    break;
                }
            case mpack_type_bool:
                {
                    if(!std::is_same_v<T, bool>){
                        throw std::runtime_error(str_fflprintf(": Invalid request for node bool -> %s", typeid(T).name()));
                    }
                }
            default:
                {
                    throw std::runtime_error(str_fflprintf(": Unsupport type: %s", mpack_type_to_string(type)));
                }
        }
    }

    template<> inline bool mp_read<bool>(mpack_node_t node)
    {
        return mpack_node_bool(node);
    }

    template<> inline int64_t mp_read<int64_t>(mpack_node_t node)
    {
        return mpack_node_i64(node);
    }

    template<> inline double mp_read<double>(mpack_node_t node)
    {
        return mpack_node_double(node);
    }

    template<> inline std::string mp_read<std::string>(mpack_node_t node)
    {
        const char *str = mpack_node_str(node);
        size_t      len = mpack_node_strlen(node);
        return std::string(str, str + len);
    }

    template<> inline libnvc::object mp_read<libnvc::object>(mpack_node_t node)
    {
        libnvc::object obj{};
        auto tag = mpack_node_tag(node);
        switch(auto type = mpack_tag_type(&tag)){
            case mpack_type_str:
                {
                    return mp_read<std::string>(node);
                }
            case mpack_type_int:
                {
                    return mp_read<int64_t>(node);
                }
            case mpack_type_bool:
                {
                    return mp_read<bool>(node);
                }
            default:
                {
                    throw std::runtime_error(std::string("unsupport type: ") + std::to_string(type));
                }
        }
    }

    template<> inline std::array<int64_t, 2> mp_read<std::array<int64_t, 2>>(mpack_node_t)
    {
        return {0, 0};
    }

    template<> inline std::map<std::string, libnvc::object> mp_read<std::map<std::string, libnvc::object>>(mpack_node_t node)
    {
        check_node_type<std::map<std::string, libnvc::object>>(node);
        const size_t size = mpack_node_map_count(node);

        std::map<std::string, libnvc::object> options;
        for(size_t index = 0; index < size; ++index){
            auto curr_key = mpack_node_map_key_at(node, index);
            auto curr_val = mpack_node_map_value_at(node, index);
            options[mp_read<std::string>(curr_key)] = mp_read<libnvc::object>(curr_val);
        }
        return options;
    }
}

libnvc::api_client::api_client(libnvc::socket *psocket)
    : m_decoder(std::make_unique<stream_decoder>(psocket))
    , m_seqid(1)
    , m_socket(psocket)
    , m_onresp()
    , m_onresperr()
{}

libnvc::api_client::~api_client()
{}

static void log_server_pack_node(mpack_node_t node)
{
    std::string log_str;
    auto callback = [](void *context, const char *data, size_t data_len)
    {
        auto pstring = (std::string *)(context);
        pstring->insert(pstring->end(), data, data + data_len);
    };
    mpack_node_print_to_callback(node, callback, &log_str);

    while(true){
        auto index = log_str.find("\n", 0);
        if(index == std::string::npos){
            break;
        }
        log_str.replace(index, 1, "\\n");
    }
    libnvc::log(libnvc::LOG_INFO, str_fflprintf("%s", log_str.c_str()).c_str());
}

static int64_t server_pack_type(mpack_node_t root)
{
    return (int64_t)(mpack_node_uint(mpack_node_array_at(root, 0)));
}

template<size_t reqid> libnvc::resp_variant inn_make_resp_variant(mpack_node_t node)
{
    if constexpr (std::is_void_v<typename libnvc::req<reqid>::resp_type>){
        return libnvc::resp_variant(libnvc::void_type());
    }else{
        return libnvc::resp_variant(mp_read<typename libnvc::req<reqid>::resp_type>(node));
    }
}

static void inn_dispatch(size_t reqid, int64_t msgid, mpack_node_t node, std::map<int64_t, libnvc::object> &resp_pool)
{
    // there is no argument checking
    // only call this function in api_client::poll

    switch(reqid){
{% for req in nvim_reqs %}
        case libnvc::reqid("{{req.name}}"):
            {
                resp_pool[msgid](inn_make_resp_variant<libnvc::reqid("{{req.name}}")>(node));
                resp_pool.erase(msgid);
                break;
            }
{% endfor %}
        default:
            {
                throw invalid_argument(str_fflprintf(": Invalid reqid: %zu", reqid));
            }
    }
}

void libnvc::api_client::poll()
{
    for(auto rootopt = m_decoder->poll(); rootopt.has_value(); rootopt = m_decoder->poll()){

        // seems no official way to check if the root is valid
        // internally we can check if it equals to mpack_tree_nil_node(&m_tree)
        log_server_pack_node(rootopt.value());

        if(auto root_type = mpack_node_type(rootopt.value()); root_type != mpack_type_array){
            throw std::runtime_error(str_fflprintf(": The msgpack from nvim server is not an array: %s", mpack_type_to_string(root_type)));
        }

        if(auto root_size = mpack_node_array_length(rootopt.value()); root_size < 1){
            throw std::runtime_error("The msgpack from nvim server is an empty array");
        }

        if(auto first_node_type = mpack_node_type(mpack_node_array_at(rootopt.value(), 0)); first_node_type != mpack_type_uint){
            throw std::runtime_error(str_fflprintf("mpack from nvim server has first node type %s, expect mpack_type_uint", mpack_type_to_string(first_node_type)));
        }

        switch(auto msg_type = server_pack_type(rootopt.value())){
            case libnvc::REQ:
                {
                    break;
                }
            case libnvc::RESP:
                {
                    if(size_t array_size = mpack_node_array_length(rootopt.value()); array_size != 4){
                        throw std::runtime_error(str_fflprintf("RESP array size is not 4: %zu", array_size));
                    }

                    int64_t msgid = mpack_node_i64(mpack_node_array_at(rootopt.value(), 1));
                    auto [req_id, seq_id] = msgid_decomp(msgid);
                    libnvc::log(libnvc::LOG_INFO, str_fflprintf("req_id = %s, seq_id = %" PRIi64, libnvc::idstr(req_id), seq_id).c_str());

                    if(libnvc::idstr(req_id) == nullptr){
                        throw std::runtime_error(str_fflprintf("RESP to invalid REQ: %" PRIi64, req_id));
                    }

                    if(seq_id <= 0){
                        throw std::runtime_error(str_fflprintf("RESP with invalid seq_id: %" PRIi64, seq_id));
                    }

                    if(auto errnode = mpack_node_array_at(rootopt.value(), 2); !mpack_node_is_nil(errnode)){
                        if(size_t errnode_size = mpack_node_array_length(errnode); errnode_size != 2){
                            throw std::runtime_error(str_fflprintf("RESP error array [type, message] has invalid size: %zu", errnode_size));
                        }

                        int64_t          ec = mpack_node_i64(mpack_node_array_at(errnode, 0));
                        const char *msg_ptr = mpack_node_str(mpack_node_array_at(errnode, 1));
                        size_t      msg_len = mpack_node_strlen(mpack_node_array_at(errnode, 1));

                        if(auto p = m_onresperr.find(msgid); p != m_onresperr.end()){
                            (p->second)(ec, std::string(msg_ptr, msg_len));
                            m_onresperr.erase(p);
                        }
                        m_onresp.erase(msgid);
                        return;
                    }

                    // we get valid RESP, for non-return REQ nvim just returns nil
                    // need inn_dispatch to different response handler

                    if(m_onresp.find(msgid) == m_onresp.end()){
                        return;
                    }

                    inn_dispatch(req_id, msgid, mpack_node_array_at(rootopt.value(), 3), m_onresp);
                    break;
                }
            case libnvc::NOTIF:
                {
                    break;
                }
            default:
                {
                    throw std::runtime_error(str_fflprintf("mpack from nvim server has message type: %d, expect [REQ, RESP, NOTIF]: %d", (int)(msg_type)));
                }
        }
    }
}
