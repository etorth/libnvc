/*
 * =====================================================================================
 *
 *       Filename: typedef.cpp
 *        Created: 12/30/2018 04:48:23
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

#include <string>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "mpack.h"
#include "typedef.hpp"
#include "strfunc.hpp"
#include "mpinterf.hpp"

std::string libnvc::req<libnvc::reqid("nvim_buf_line_count")>::parms_t::pack(int64_t msgid) const
{
    char  *data = nullptr;
    size_t size = 0;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    mpack_start_array(&writer, 4);
    {
        mp_write(&writer, libnvc::REQ);
        mp_write(&writer, msgid);
        mp_write(&writer, "nvim_buf_line_count");
        mp_write(&writer, buffer);
    }
    mpack_finish_array(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok){
        throw std::runtime_error("failed to pack req::nvim_buf_line_count");
    }

    std::string buf{data, data + size};
    MPACK_FREE(data);

    return buf;
}

std::string libnvc::req<libnvc::reqid("nvim_input")>::parms_t::pack(int64_t msgid) const
{
    char  *data = nullptr;
    size_t size = 0;

    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    mpack_start_array(&writer, 4);
    {
        mp_write(&writer, libnvc::REQ);
        mp_write(&writer, msgid);
        mp_write(&writer, "nvim_buf_line_count");
        mp_write(&writer, keys);
    }
    mpack_finish_array(&writer);

    if(mpack_writer_destroy(&writer) != mpack_ok){
        throw std::runtime_error("failed to pack req::nvim_input");
    }

    std::string buf{data, data + size};
    MPACK_FREE(data);

    return buf;
}
