/*
 * =====================================================================================
 *
 *       Filename: nvimprocess.hpp
 *        Created: 01/06/2019 04:35:01
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

#include "libnvc.hpp"
#include <reproc++/sink.hpp>
#include <reproc++/reproc.hpp>

class nvim_process: public libnvc::io_device
{
    private:
        reproc::process m_reproc;

    private:
        std::string m_output;

    public:
        nvim_process()
            : m_reproc(reproc::wait, reproc::infinite)
            , m_output()
        {
            if(auto ec = m_reproc.start({"nvim", "-u", "NONE", "--embed"}); ec){
                throw std::runtime_error(std::string("Error when starting nvim: ") + ec.message());
            }
        }

    public:
        size_t send(const char *buf, size_t length)
        {
            unsigned int done_length = 0;
            m_reproc.write(buf, length, &done_length);
            return (size_t)(done_length);
        }

    public:
        size_t recv(char *buf, size_t length)
        {
            unsigned int done_length = 0;
            if(auto ec = m_reproc.read(reproc::stream::out, buf, length, &done_length); ec){
                throw std::runtime_error(std::string("Failed to recv data: ") + ec.message());
            }
            return done_length;
        }
};
