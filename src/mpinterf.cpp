/*
 * =====================================================================================
 *
 *       Filename: mpinterf.cpp
 *        Created: 12/28/2018 03:25:41
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
#include <new>
#include <map>
#include <array>
#include <cstdint>
#include <variant>
#include <stdexcept>
#include <type_traits>
#include "mpack.h"
#include "ctf.hpp"
#include "valdef.hpp"
#include "objdef.hpp"
#include "strfunc.hpp"

libnvc::mpinterf::writer::writer()
    : m_storage()
    , m_data(nullptr)
    , m_size(0)
{
    // m_writer_t from mpack package is pure c struct
    // so it's a pod type, should be safe ...

    static_assert( sizeof(m_storage) >= sizeof(mpack_writer_t)     , "aligned memory for mpack_writer_t too small, define bigger LIBNVC_MPACK_WRITER_SIZE");
    static_assert(alignof(m_storage) % alignof(mpack_writer_t) == 0, "aligned memory misaligned to mpack_writer_t, define bigger LIBNVC_MPACK_WRITER_ALIGN");

    new (storage()) mpack_writer_t();
    mpack_writer_init_growable((mpack_writer_t *)(storage()), &m_data, &m_size);
    m_writer_alive = true;
}

libnvc::mpinterf::writer::~writer()
{
    clear();
    reinterpret_cast<mpack_writer_t *>(storage())->~mpack_writer_t();
}

void libnvc::mpinterf::flush()
{
    if(m_writer_alive){
        if(mpack_writer_destroy(reinterpret_cast<mpack_writer_t *>(storage())) != mpack_ok){
            throw std::runtime_error(str_fflprintf("failed to call mpack_writer_destroy() to flush data"));
        }
        m_writer_alive = false;
    }
}

std::string_view libnvc::mpinterf::writer::pack() const
{
    this->flush();
    if(m_data == nullptr){
        throw std::runtime_error(str_fflprintf(": try to pack an empty writer"));
    }
    return {m_data, m_data + m_size};
}

void libnvc::mpinterf::writer::clear()
{
    flush();
    if(m_data){
        MPACK_FREE(m_data);
    }

    m_data = nullptr;
    m_size = 0;
}

void libnvc::mpinterf::write()
{
    mpack_write_nil(reinterpret_cast<mpack_writer_t *>(storage());
}

void libnvc::mpinterf::write(const std::array<int64_t, 2> &vec)
{
    write(vec[0]);
    write(vec[1]);
}

void libnvc::mpinterf::write(int64_t val)
{
    mpack_write_i64(reinterpret_cast<mpack_writer_t *>(storage()), val);
}

void libnvc::mpinterf::write(double val)
{
    mpack_write_double(reinterpret_cast<mpack_writer_t *>(storage()), val);
}

void libnvc::mpinterf::writer::write(const std::string& val)
{
    mpack_write_cstr(reinterpret_cast<mpack_writer_t *>(storage()), val.c_str());
}

void libnvc::mpinterf::writer::write(const char *val)
{
    mpack_write_cstr(reinterpret_cast<mpack_writer_t *>(storage()), val);
}

void libnvc::mpinterf::writer::write(const libnvc::object& obj)
{
    if(std::holds_alternative<std::string>(obj)){
        write(std::get<std::string>(obj));
    }

    else if(std::holds_alternative<bool>(obj)){
        write(std::get<bool>(obj));
    }

    else if(std::holds_alternative<int64_t>(obj)){
        write(std::get<int64_t>(obj));
    }

    else{
        throw std::runtime_error(str_fflprintf(": libnvc::object holds unknown type"));
    }
}

void libnvc::mpinterf::writer::write(const std::map<std::string, libnvc::object>& options)
{
    mpack_start_map(reinterpret_cast<mpack_writer_t *>(storage()), options.size());
    for(const auto& e : options){
        write(e.first);
        write(e.second);
    }
    mpack_finish_map(reinterpret_cast<mpack_writer_t *>(storage()));
}
