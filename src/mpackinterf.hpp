/*
 * =====================================================================================
 *
 *       Filename: mpackinterf.hpp
 *        Created: 12/28/2018 09:16:40
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
#include "mpack.h"

namespace libnvc
{
    void mp_write(mpack_writer_t *)
    {}

    template<typename T, typename... U> void mp_write(mpack_writer_t *writer, T t, U... u)
    {
        mp_write(writer, t   );
        mp_write(writer, u...);
    }

    void mp_write(mpack_writer_t *writer, const std::array<int64_t, 2> &vec)
    {
        mp_write_i64(writer, vec[0]);
        mp_write_i64(writer, vec[1]);
    }

    void mp_write(mpack_writer_t *writer, double val)
    {
        mpack_write_double(writer, val);
    }

    void mp_write(mpack_writer_t* writer, const std::string& val)
    {
        mpack_write_cstr(writer, val.c_str());
    }

    void mp_write(mpack_writer_t* writer, const libnvc::object& obj)
    {
        if(std::holds_alternative<std::string>(obj)){
            mp_write(writer, std::get<std::string>(obj));
        }

        else if(std::holds_alternative<bool>(obj)){
            mpack_write_bool(writer, std::get<bool>(obj));
        }

        else if(std::holds_alternative<int64_t>(obj)){
            mpack_write_bool(writer, std::get<int64_t>(obj));
        }

        else{
            throw std::runtime_error(str_fflprintf(": libnvc::object holds unknown type"));
        }
    }

    void mp_write(mpack_writer_t* writer, const std::unordered_map<std::string, libncv::object>& options)
    {
        mpack_start_map(writer, options.size());
        for(const auto& e : options){
            mpack_write(writer, e.first);
            mpack_write(writer, e.second);
        }
        mpack_finish_map(writer);
    }
}
