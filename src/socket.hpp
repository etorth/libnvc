/*
 * =====================================================================================
 *
 *       Filename: socket.hpp
 *        Created: 12/26/2018 05:12:41
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
#include <vector>
#include <cstddef>
#include "strfunc.hpp"

namespace libnvc
{
    class socket
    {
        public:
            socket() = default;

        public:
            virtual ~socket() = default;

        public:
            virtual bool connect(const char *, int) = 0;
            virtual void disconnect()               = 0;

        public:
            virtual size_t send(const uint8_t *, size_t) = 0; // blocking send, return after done all bytes
            virtual size_t recv(      uint8_t *, size_t) = 0;
    };
}
