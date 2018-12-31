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
{}

libnvc::api_client::~api_client()
{}

static void log_node(mpack_node_t node)
{
    std::string log_buf;
    auto callback = [](void *context, const char *data, size_t data_len)
    {
        auto pstring = (std::string *)(context);
        pstring->insert(pstring->end(), data, data + data_len);
    };
    mpack_node_print_to_callback(node, callback, &log_buf);
    libnvc::log(libnvc::LOG_INFO, str_printf("%s\n", log_buf.c_str()).c_str());
}

void libnvc::api_client::poll()
{
    for(auto rootopt = m_decoder->poll(); rootopt.has_value(); rootopt = m_decoder->poll()){
        // seems no official way to check if the root is valid
        // internally we can check if it equals to mpack_tree_nil_node(&m_tree)

        log_node(rootopt.value());
        if(auto root_type = mpack_node_type(rootopt.value()); root_type != mpack_type_array){
            throw std::runtime_error(str_printf("message is not an array: %d", (int)(root_type)));
        }

        auto first_node = mpack_node_array_at(rootopt.value(), 0);
        if(auto first_node_type = mpack_node_type(first_node); first_node_type != mpack_type_uint){
            throw std::runtime_error(str_printf("first node type is not mpack_type_uint: %s", mpack_type_to_string(first_node_type)));
        }

        switch(auto msg_type = (int64_t)(mpack_node_uint(first_node))){
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
                    auto [req_id, msg_id] = msgid_decomp(msgid);
                    libnvc::log(libnvc::LOG_INFO, str_printf("req_id = %s, msg_id = %" PRIi64, libnvc::idstr(req_id), msg_id).c_str());

                    if(!mpack_node_is_nil(mpack_node_array_at(rootopt.value(), 2))){
                        // should we call the failure handler?
                        m_onresp.erase(msg_id);
                        return;
                    }

                    auto resp_handler = m_onresp.find(msgid);
                    if(resp_handler == m_onresp.end()){
                        throw std::runtime_error(str_printf("can't find handler: req_id = %s, msg_id = %" PRIi64, libnvc::idstr(req_id), msg_id));
                    }

                    switch(req_id){
                        case libnvc::reqid("nvim_input"):
                            {
                                (resp_handler->second)(libnvc::object(mpack_node_i64(mpack_node_array_at(rootopt.value(), 3))));
                                m_onresp.erase(resp_handler);
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
                    throw std::runtime_error(str_printf("nvim message is not of [REQ, RESP, NOTIF]: %d", (int)(msg_type)));
                }
        }
    }
}
