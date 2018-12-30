/*
 * =====================================================================================
 *
 *       Filename: asiosocket.cpp
 *        Created: 12/28/2018 22:16:00
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

#include "asio.hpp"
#include "asiosocket.hpp"

namespace libnvc
{
    class asio_socket::asio_socket_impl
    {
        private:
            asio::io_context        m_context;
            asio::ip::tcp::resolver m_resolver;
            asio::ip::tcp::socket   m_socket;

        public:
            asio_socket_impl()
                : m_context(1)
                , m_resolver(m_context)
                , m_socket(m_context)
            {}

        public:
            bool connect(const char *address, int port)
            {
                asio::error_code ec;
                auto endpoints = m_resolver.resolve(asio::ip::tcp::v4(), address, std::to_string(port).c_str());

                asio::connect(m_socket, endpoints, ec);
                if(ec){
                    return false;
                }
                return true;
            }

            void disconnect()
            {
                m_socket.shutdown(asio::ip::tcp::socket::shutdown_send);
                m_socket.close();
            }

        public:
            size_t send(const char *buf, size_t size)
            {
                std::error_code ec;
                return asio::write(m_socket, asio::buffer(buf, size), ec);
            }

            size_t recv(char *buf, size_t size)
            {
                std::error_code ec;
                return asio::read(m_socket, asio::buffer(buf, size), [](std::error_code &, size_t) -> size_t
                {
                    // always return zero
                    // to support the non-blocking read for mpack_try_parse_tree()
                    return 0;
                }, ec);
            }
    };
}

libnvc::asio_socket::asio_socket()
    : m_impl(std::make_unique<asio_socket_impl>())
{}

libnvc::asio_socket::~asio_socket()
{}

bool libnvc::asio_socket::connect(const char *address, int port)
{
    return m_impl->connect(address, port);
}

void libnvc::asio_socket::disconnect()
{
    m_impl->disconnect();
}

size_t libnvc::asio_socket::send(const char *buf, size_t size)
{
    return m_impl->send(buf, size);
}

size_t libnvc::asio_socket::recv(char *buf, size_t size)
{
    return m_impl->recv(buf, size);
}
