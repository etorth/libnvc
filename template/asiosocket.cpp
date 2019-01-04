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
#include "libnvc.hpp"
#include "strfunc.hpp"

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
                if(auto size_done = asio::write(m_socket, asio::buffer(buf, size), ec); !ec){
                    return size_done;
                }
                throw std::runtime_error(str_printf("asio::write(%p, %zu) failed: %s", buf, size, ec.message().c_str()));
            }

            size_t recv(char *buf, size_t size)
            {
                std::error_code ec;
                if(auto size_recv = asio::read(m_socket, asio::buffer(buf, size), [this](std::error_code &, size_t done) -> size_t
                {
                    // asio::read() is error-prone, it works as:
                    //
                    //     buffer buf(ptr, capacity);
                    //     while(!buf.full()){
                    //         if(auto count = call_this_func(ec, buf.done_size())){
                    //             read_some(count, buf);
                    //         }else{
                    //             break;
                    //         }
                    //     }
                    //     return buf.done_size();
                    //
                    // so if the first call returns 0, this will never read any data
                    // but if first time doesn't return 0, then read_some() will block to read at least ONE byte
                    //
                    // what we need is:
                    //     1. read all data valid now, if no data just skip read
                    //     2. return immediately
                    // to get this we have to check if there is data

                    asio::error_code ec_ctrl;
                    asio::ip::tcp::socket::bytes_readable ctrl(true);

                    m_socket.io_control(ctrl, ec_ctrl);
                    if(ec_ctrl){
                        throw std::runtime_error(str_printf("socket::io_control(byte_readable) failed: %s", ec_ctrl.message().c_str()));
                    }

                    size_t has_data = ctrl.get();

                    // if there is data
                    // we can try it now or delay to next time

                    // this function is blocking
                    // if we alreayd done some data, return and try next time
                    if(has_data == 0 || done > 1024){
                        return 0;
                    }
                    return has_data;

                }, ec); !ec){
                    return size_recv;
                }
                throw std::runtime_error(str_printf("asio::read(%p, %zu) failed: %s", buf, size, ec.message().c_str()));
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
