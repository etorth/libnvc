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

#include <cstring>
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
            stream_decoder(libnvc::io_device *piodev)
                : m_tree()
            {
                if(!piodev){
                    throw std::invalid_argument(str_fflprintf(": Invalid io device: (nullptr)"));
                }
                // checked the code
                // seems this function always succeeds
                mpack_tree_init_stream(&m_tree, &read_iodev, piodev, 1024 * 1024, 1024);
            }

        public:
            ~stream_decoder()
            {
                // don't free the device
                // decoder doesn't own the lifecycle of io device
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
            static size_t read_iodev(mpack_tree_t *ptree, char *buf, size_t count)
            {
                if(auto piodev = (libnvc::io_device *)(mpack_tree_context(ptree))){
                    return piodev->recv(buf, count);
                }
                throw std::runtime_error(str_fflprintf(": Empty context in mpack_tree_t: %p", ptree));
            }
    };
}

namespace
{
    void inline check_node_type(mpack_node_t node, mpack_type_t expected_type)
    {
        auto tag  = mpack_node_tag(node);
        auto type = mpack_tag_type(&tag);

        // FIXME: aliasing i64/u64
        // nvim returns i64/u64 although it claims always i64
        if(true 
                && (         type == mpack_type_int ||          type == mpack_type_uint)
                && (expected_type == mpack_type_int || expected_type == mpack_type_uint)){
            return;
        }

        if(type != expected_type){
            throw std::runtime_error(str_fflprintf(": Node type doesn't match: %s, expecting: %s", mpack_type_to_string(type), mpack_type_to_string(expected_type)));
        }
    }

    template<typename T> T mp_read(mpack_node_t);

    template<> inline bool mp_read<bool>(mpack_node_t node)
    {
        check_node_type(node, mpack_type_bool);
        return mpack_node_bool(node);
    }

    template<> inline int64_t mp_read<int64_t>(mpack_node_t node)
    {
        check_node_type(node, mpack_type_int);
        return mpack_node_i64(node);
    }

    template<> inline double mp_read<double>(mpack_node_t node)
    {
        check_node_type(node, mpack_type_double);
        return mpack_node_double(node);
    }

    template<> inline std::string mp_read<std::string>(mpack_node_t node)
    {
        check_node_type(node, mpack_type_str);
        const char *str = mpack_node_str(node);
        size_t      len = mpack_node_strlen(node);
        return std::string(str, str + len);
    }

    template<> inline std::array<int64_t, 2> mp_read<std::array<int64_t, 2>>(mpack_node_t node)
    {
        check_node_type(node, mpack_type_array);
        size_t length = mpack_node_array_length(node);
        if(length != 2){
            throw std::runtime_error(str_fflprintf(": Try to read std::array<int64_t, 2> while mpack array size is %zu", length));
        }

        std::array<int64_t, 2> res;
        res[0] = mp_read<int64_t>(mpack_node_array_at(node, 0));
        res[1] = mp_read<int64_t>(mpack_node_array_at(node, 1));
        return res;
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
                    throw std::runtime_error(str_fflprintf(": Unsupport type: %s", mpack_type_to_string(type)));
                }
        }
    }

    template<typename T> std::vector<T> mp_read_array(mpack_node_t node)
    {
        check_node_type(node, mpack_type_array);

        std::vector<T> res;
        size_t length = mpack_node_array_length(node);

        for(size_t index = 0; index < length; ++index){
            auto curr_node = mpack_node_array_at(node, index);
            res.push_back(mp_read<T>(curr_node));
        }
        return res;
    }

    template<typename K, typename V> inline std::map<K, V> mp_read_map(mpack_node_t node)
    {
        check_node_type(node, mpack_type_map);

        std::map<K, V> res;
        const size_t size = mpack_node_map_count(node);

        for(size_t index = 0; index < size; ++index){
            auto curr_key = mpack_node_map_key_at(node, index);
            auto curr_val = mpack_node_map_value_at(node, index);
            res[mp_read<K>(curr_key)] = mp_read<V>(curr_val);
        }
        return res;
    }

    template<> inline std::vector<std::string> mp_read<std::vector<std::string>>(mpack_node_t node)
    {
        return mp_read_array<std::string>(node);
    }

    template<> inline std::vector<libnvc::object> mp_read<std::vector<libnvc::object>>(mpack_node_t node)
    {
        return mp_read_array<libnvc::object>(node);
    }

    template<> inline std::vector<int64_t> mp_read<std::vector<int64_t>>(mpack_node_t node)
    {
        return mp_read_array<int64_t>(node);
    }

    template<> inline std::vector<std::map<std::string, libnvc::object>> mp_read<std::vector<std::map<std::string, libnvc::object>>>(mpack_node_t node)
    {
        return mp_read_array<std::map<std::string, libnvc::object>>(node);
    }

    template<> inline std::map<std::string, libnvc::object> mp_read<std::map<std::string, libnvc::object>>(mpack_node_t node)
    {
        return mp_read_map<std::string, libnvc::object>(node);
    }
}

libnvc::api_client::api_client(libnvc::io_device *piodev)
    : m_decoder(std::make_unique<stream_decoder>(piodev))
    , m_seqid(1)
    , m_iodev(piodev)
    , m_onresp()
    , m_onresperr()
{
    const char *lib_signature = "{{build_signature}}";
    if(std::strcmp(lib_signature, build_signature()) != 0){
        throw std::runtime_error(str_fflprintf(": Using incompatible header and lib: header = %s, lib = %s", build_signature(), lib_signature));
    }
}

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

template<size_t reqid> libnvc::resp_variant inn_make_resp_variant([[maybe_unused]] mpack_node_t node)
{
    if constexpr (std::is_void_v<typename libnvc::req<reqid>::resp_type>){
        return libnvc::resp_variant(libnvc::void_type());
    }else{
        // explicitly use resp_type to initialize resp_variant
        // initialization of std::variant is error-prone, as an example:
        //
        //      std::variant<std::string, bool> x("abc");
        //      std::cout << std::get<std::string>(x) << std::endl;
        //
        // here initialization of x will choose bool, and the std::get<> triggers exception...
        return libnvc::resp_variant(mp_read<typename libnvc::req<reqid>::resp_type>(node));
    }
}

static void inn_dispatch(size_t reqid, int64_t msgid, mpack_node_t node, std::map<int64_t, std::function<void(libnvc::resp_variant)>> &resp_pool)
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
                throw std::invalid_argument(str_fflprintf(": Invalid reqid: %zu", reqid));
            }
    }
}

int64_t libnvc::api_client::poll_one()
{
    auto rootopt = m_decoder->poll();
    if(!rootopt.has_value()){
        return 0;
    }

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
                return 0;
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
                    return msgid;
                }

                // we get valid RESP, for non-return REQ nvim just returns nil
                // need inn_dispatch to different response handler

                if(m_onresp.find(msgid) == m_onresp.end()){
                    if(m_onresperr.find(msgid) != m_onresperr.end()){
                        throw std::runtime_error(str_fflprintf(": Found error handler but no resp handler: %zu", req_id));
                    }
                    m_onresperr.erase(msgid);
                    return 0;
                }

                inn_dispatch(req_id, msgid, mpack_node_array_at(rootopt.value(), 3), m_onresp);
                return msgid;
            }
        case libnvc::NOTIF:
            {
                return 0;
            }
        default:
            {
                throw std::runtime_error(str_fflprintf("mpack from nvim server has message type: %d, expect [REQ, RESP, NOTIF]: %d", (int)(msg_type)));
            }
    }
}
