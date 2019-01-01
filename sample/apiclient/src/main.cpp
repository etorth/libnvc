/*
 * =====================================================================================
 *
 *       Filename: main.cpp
 *        Created: 12/29/2018 06:56:01
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
#include <cstdio>
#include "libnvc.hpp"

int main()
{
    libnvc::asio_socket socket;
    if(!socket.connect("localhost", 6666)){
        throw std::runtime_error("failed to connect to localhost:6666");
    }

    libnvc::api_client client(&socket);
    client.forward<libnvc::reqid("nvim_input")>({"$i123<CR>123<ESC>"}, [](int64_t len_done)
    {
        char buf[128];
        std::sprintf(buf, "nvim_input returns: %" PRIi64, len_done);
        libnvc::log(libnvc::LOG_INFO, buf);
    });

    client.forward<libnvc::reqid("nvim_buf_set_name")>({1, "1234"}, []()
    {
        libnvc::log(libnvc::LOG_INFO, "nvim_buf_set_name done");
    }, [](int64_t ec, std::string emsg)
    {
        std::printf("nvim reports error: [%d, %s]", (int)(ec), emsg.c_str());
    });

    while(true){
        client.poll();
    }
}
