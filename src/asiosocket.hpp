/*
 * =====================================================================================
 *
 *       Filename: asiosocket.hpp
 *        Created: 12/28/2018 22:14:03
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

#pragma once
#include <memory>
#include "socket.hpp"

namespace libnvc
{
    class asio_socket: public libnvc::socket
    {
        private:
            class asio_socket_impl;

        private:
            std::unique_ptr<asio_socket_impl> m_impl;

        public:
            asio_socket();

        public:
            virtual ~asio_socket();

        public:
            virtual bool connect(const char *, int);
            virtual void disconnect();

        public:
            virtual size_t send(const char *, size_t);
            virtual size_t recv(      char *, size_t);
    };
}
