/*
 * =====================================================================================
 *
 *       Filename: nvimclient.cpp
 *        Created: 01/08/2019 05:33:48
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

#include <utf8.h>
#include "libnvc.hpp"
#include "strfunc.hpp"

static size_t peek_utf8_code(const char *str, size_t length, uint32_t *pcode)
{
    if(str == nullptr){
        throw std::invalid_argument(str_fflprintf(": Invalid argument: (nullptr)"));
    }

    if(length == 0){
        length = std::strlen(str);
    }

    if(length == 0){
        throw std::invalid_argument(str_fflprintf(": Invalid argument: empty string"));
    }

    auto pbegin = str;
    auto pend   = pbegin;

    try{
        utf8::advance(pend, 1, str + length);
    }catch(...){
        throw std::invalid_argument(str_fflprintf(": Invalid argument: failed to peek the first utf8 code"));
    }

    size_t advanced = pend - pbegin;
    if(advanced > 4){
        throw std::runtime_error(str_fflprintf(": First utf8 code from \"%s\" is longer than 4 bytes: %zu", str, advanced));
    }

    if(pcode){
        *pcode = (uint32_t)(0);
        std::memcpy(pcode, pbegin, pend - pbegin);
    }
    return advanced;
}

libnvc::nvim_client::nvim_client(libnvc::io_device *pdevice, size_t width, size_t height)
    : libnvc::api_client(pdevice)
    , m_currboard(std::make_unique<libnvc::board>(width, height))
    , m_backboard()
{}

void libnvc::nvim_client::on_put(const std::string &str)
{
    size_t done_length = 0;
    while(done_length < str.length()){
        uint32_t utf8_code = 0;
        auto code_length = peek_utf8_code(str.data() + done_length, str.length(), &utf8_code);

        m_currboard->get_cell().utf8_code = utf8_code;
        done_length += code_length;
    }

    if(done_length != str.length()){
        throw std::runtime_error(str_fflprintf(": Parse utf8 string failed: \"%s\"", str.c_str()));
    }
}
