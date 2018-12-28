/*
 * =====================================================================================
 *
 *       Filename: socket.cpp
 *        Created: 12/26/2018 08:35:30
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
#include "socket.hpp"

libnvc::socket::socket(const char *host, int port, size_t buf_capacity)
    : 
{}

libnvc::socket::~socket()
{}

size_t libnvc::socket::read(int32_t timeout_ms)
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(m_socket, &sockets);

    struct timeval timeout;
    std::memset(&timeout, 0, sizeof(timeout));

    if(timeout_ms >= 0){
        timeout.tv_sec  = timeout_ms / 1000;
        timeout.tv_usec = 1000 * (timeout_ms % 1000);
    }

    switch(int rc = select(0, &sockets, nullptr, nullptr, (timeout_ms < 0) ? nullptr : &timeout)){
        case 0:
            {
                return 0;
            }
        default:
            {
                if(rc < 0){
                    throw std::runtime_error(str_fflprintf(": select() failed: %s", std::strerror(errno)));
                }
                break;
            }
    }

    // we sure there is data
    // read data to the output buffer

    constexpr size_t  recv_buf = 4096;
    const     size_t start_off = m_buf.data() + m_start;

    m_buf.resize(m_buf.size() + recv_buf);
    if(int rc = ::recv(m_socket, m_buf.data() + start_off, recv_buf, 0); rc < 0){
        throw std::runtime_error(str_fflprintf(": recv failed: %s", std::strerror(errno)));
    }else{
        if(rc > (int)(recv_buf)){
            throw std::runtime_error(str_fflprintf(": recv data bytes %d, more than given buffer size: %zu", rc, recv_buf));
        }

        // output buffer may have pending data
        // this function returns data size read in this function, not pending length
        m_buf.resize(recv_buf - (size_t)(rc));
        return rc;
    }
}

void libnvc::socket::write(const uint8_t *data, size_t size)
{
    if(int rc = ::send(m_socket, data, size, 0); rc != (int)(size)){
        throw std::invalid_argument(str_fflprintf(": send(%p, %zu) returns %d%s%s", data, size, rc, ((rc < 0) ? ": " : ""), ((rc < 0) ? std::strerror(errno) : "")));
    }
}
