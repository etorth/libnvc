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
#include "mpinterf.hpp"
#include "apiclient.hpp"

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
                    throw std::invalid_argument(str_fflprintf(": Empty socket"));
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

                // no message valid:
                //   1. no enough data received
                //   2. error occurred

                switch(auto ec = mpack_tree_error(&m_tree)){
                    case mpack_ok:
                        {
                            return {};
                        }
                    default:
                        {
                            throw std::runtime_error(str_fflprintf(": mpack_tree_error(): %d", (int)(ec)));
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
    libnvc::log(libnvc::LOG_INFO, str_printf("%s", log_str.c_str()).c_str());
}

static int64_t server_pack_type(mpack_node_t root)
{
    return (int64_t)(mpack_node_uint(mpack_node_array_at(root, 0)));
}

void libnvc::api_client::poll()
{
    for(auto rootopt = m_decoder->poll(); rootopt.has_value(); rootopt = m_decoder->poll()){

        // seems no official way to check if the root is valid
        // internally we can check if it equals to mpack_tree_nil_node(&m_tree)
        log_server_pack_node(rootopt.value());

        if(auto root_type = mpack_node_type(rootopt.value()); root_type != mpack_type_array){
            throw std::runtime_error(str_printf("msgpack from nvim server is not an array: %s", mpack_type_to_string(root_type)));
        }

        if(auto root_size = mpack_node_array_length(rootopt.value()); root_size < 1){
            throw std::runtime_error("msgpack from nvim server is an empty array");
        }

        if(auto first_node_type = mpack_node_type(mpack_node_array_at(rootopt.value(), 0)); first_node_type != mpack_type_uint){
            throw std::runtime_error(str_printf("mpack from nvim server has first node type %s, expect mpack_type_uint", mpack_type_to_string(first_node_type)));
        }

        switch(auto msg_type = server_pack_type(rootopt.value())){
            case libnvc::REQ:
                {
                    break;
                }
            case libnvc::RESP:
                {
                    if(size_t array_size = mpack_node_array_length(rootopt.value()); array_size != 4){
                        throw std::runtime_error(str_printf("RESP array size is not 4: %zu", array_size));
                    }

                    int64_t msgid = mpack_node_i64(mpack_node_array_at(rootopt.value(), 1));
                    auto [req_id, seq_id] = msgid_decomp(msgid);
                    libnvc::log(libnvc::LOG_INFO, str_printf("req_id = %s, seq_id = %" PRIi64, libnvc::idstr(req_id), seq_id).c_str());

                    if(libnvc::idstr(req_id) == nullptr){
                        throw std::runtime_error(str_printf("RESP to invalid REQ: %" PRIi64, req_id));
                    }

                    if(!libnvc::has_return(req_id)){
                        throw std::runtime_error(str_printf("RESP to an non-return REQ: %zu", req_id));
                    }

                    if(seq_id <= 0){
                        throw std::runtime_error(str_printf("RESP with invalid seq_id: %" PRIi64, seq_id));
                    }

                    if(auto errnode = mpack_node_array_at(rootopt.value(), 2); !mpack_node_is_nil(errnode)){
                        if(size_t errnode_size = mpack_node_array_length(errnode); errnode_size != 2){
                            throw std::runtime_error(str_printf("RESP error array [type, message] has invalid size: %zu", errnode_size));
                        }

                        int64_t ec = mpack_node_i64(mpack_node_array_at(errnode, 0));
                        const char *msg_ptr = mpack_node_str(mpack_node_array_at(errnode, 1));
                        size_t      msg_len = mpack_node_strlen(mpack_node_array_at(errnode, 1));

                        if(auto p = m_onresperr.find(msgid); p != m_onresperr.end()){
                            (p->second)(ec, std::string(msg_ptr, msg_len));
                            m_onresperr.erase(p);
                        }
                        m_onresp.erase(msgid);
                        return;
                    }


                    // we get RESP with valid return value
                    // need dispatch to different response handler

                    if(m_onresp.find(msgid) == m_onresp.end()){
                        return;
                    }

                    switch(req_id){
                        case libnvc::reqid("nvim_input"):
                            {
                                using result_t = libnvc::req<libnvc::reqid("nvim_input")>::res_t;
                                auto res = libnvc::mp_read<result_t>(mpack_node_array_at(rootopt.value(), 3));

                                m_onresp[msgid](libnvc::object(res));
                                m_onresp.erase(msgid);
                                break;
                            }
                        case libnvc::reqid("nvim_buf_set_name"):
                            {
                                m_onresp[msgid](libnvc::object(libnvc::void_t()));
                                m_onresp.erase(msgid);
                                break;
                            }
                    }
                    break;
                }
            case libnvc::NOTIF:
                {
                    break;
                }
            default:
                {
                    throw std::runtime_error(str_printf("mpack from nvim server has message type: %d, expect [REQ, RESP, NOTIF]: %d", (int)(msg_type)));
                }
        }
    }
}
