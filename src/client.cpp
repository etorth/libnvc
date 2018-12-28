/*
 * =====================================================================================
 *
 *       Filename: client.cpp
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

#include "libnvc.h"
#include "client.hpp"

void libncv::client::poll()
{
    while(mpack_try_tree_parse(&m_tree)){
        if(mpack_tree_error(&m_tree) != mpack_error_ok){
            // need check the error
            // most likely neovim server exists
            return;
        }
        process_new_root(mpack_tree_root(&m_tree));
    }
}

void libnvc::client::process_new_root(mpack_node_t root)
{
    // seems no official way to check if the root is valid
    // internally we can check if it equals to mpack_tree_nil_node(&m_tree)

    switch(mpack_node_type(root)){
        case mpack_type_int:
            {
                switch(mpack_node_int(root)){
                    case libnvc::client::REQ:
                        {
                            break;
                        }
                    case libncv::client::RESP:
                        {
                            break;
                        }
                    case libnvc::type::NOTIF:
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

#include "asio.h"
#include "client.hpp"

namespace libnvc
{
    class socket_impl
    {
        private:
            asio::io_context        m_context;
            asio::ip::tcp::acceptor m_acceptor;
            asio::ip::tcp::socket   m_socket;

        public:
            socket_impl(const char *address, int port, libncv::log log)
                : m_context()
                , m_acceptor()
                , m_socket()
            {}

        public:
            size_t send(const uint8_t *buf, size_t size)
            {
                std::error_code ec;
                return asio::write(*m_socket, asio::buffer(buf, size), ec);
            }

            size_t recv(uint8_t *buf, size_t size)
            {
                std::error_code ec;
                return asio::read(*m_socket, asio::buffer(buf, size), [](std::error_code &, size_t) -> size_t
                {
                    // always return zero
                    // to support the non-blocking read for mpack_try_parse_tree()
                    return 0;
                }, ec);
            }
    }
}
