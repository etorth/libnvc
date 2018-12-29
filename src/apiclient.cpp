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
                if(!mpack_tree_try_parse(&m_tree)){
                    return {};
                }

                switch(auto ec = mpack_tree_error(&m_tree)){
                    case mpack_ok:
                        {
                            return mpack_tree_root(&m_tree);
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
                    return psocket->recv((uint8_t *)(buf), count);
                }
                throw std::runtime_error(str_fflprintf(": Empty context in mpack_tree_t: %p", ptree));
            }
    };
}

libnvc::api_client::api_client(libnvc::socket *psocket)
    : m_decoder(std::make_unique<stream_decoder>(psocket))
    , m_socket(psocket)
    , m_onresp()
{}

void libnvc::api_client::poll()
{
    for(auto rootopt = m_decoder->poll(); rootopt.has_value(); rootopt = m_decoder->poll()){
        // seems no official way to check if the root is valid
        // internally we can check if it equals to mpack_tree_nil_node(&m_tree)

        auto root = rootopt.value();
        switch(mpack_node_type(root)){
            case mpack_type_int:
                {
                    switch(mpack_node_int(root)){
                        case libnvc::REQ:
                            {
                                break;
                            }
                        case libnvc::RESP:
                            {
                                auto [msg_id, req_id] = msgid_decomp(123);
                                auto resp_handler     = m_onresp.find(msg_id);

                                if( resp_handler == m_onresp.end()){
                                    throw std::runtime_error("can't find handler...");
                                }

                                switch(req_id){
                                    case libnvc::reqid("nvim_input"):
                                        {
                                            auto res = libnvc::mp_read<typename libnvc::req<libnvc::reqid("nvim_input")>::res_t>(root);
                                            (resp_handler->second)(libnvc::object(res));
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
                                break;
                            }
                    }
                }
            default:
                {
                    break;
                }
        }
    }
}
