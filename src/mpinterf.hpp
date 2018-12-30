/*
 * =====================================================================================
 *
 *       Filename: mpinterf.hpp
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
#include <map>
#include <array>
#include <cstdint>
#include <variant>
#include <stdexcept>
#include "mpack.h"
#include "ctf.hpp"
#include "valdef.hpp"
#include "objdef.hpp"
#include "strfunc.hpp"

namespace libnvc
{
    template<typename T> T mp_read(mpack_node_t);

    template<typename T> inline void check_node_type(mpack_node_t node)
    {
        auto tag = mpack_node_tag(node);
        switch(auto type = mpack_tag_type(&tag)){
            case mpack_type_str:
                {
                    if(!std::is_same_v<T, std::string>){
                        throw std::runtime_error("node is not a std::string");
                    }
                    break;
                }
            case mpack_type_int:
                {
                    if(!std::is_same_v<T, int64_t>){
                        throw std::runtime_error("node is not a int64_t");
                    }
                    break;
                }
            case mpack_type_bool:
                {
                    if(!std::is_same_v<T, bool>){
                        throw std::runtime_error("node is not a bool");
                    }
                }
            default:
                {
                    throw std::runtime_error(std::string("unsupport type: ") + std::to_string(type));
                }
        }
    }

    template<> inline bool mp_read<bool>(mpack_node_t node)
    {
        return mpack_node_bool(node);
    }

    template<> inline int64_t mp_read<int64_t>(mpack_node_t node)
    {
        return mpack_node_i64(node);
    }

    template<> inline double mp_read<double>(mpack_node_t node)
    {
        return mpack_node_double(node);
    }

    template<> inline std::string mp_read<std::string>(mpack_node_t node)
    {
        const char *str = mpack_node_str(node);
        size_t      len = mpack_node_strlen(node);
        return std::string(str, str + len);
    }

    template<> inline libnvc::object mp_read<libnvc::object>(mpack_node_t node)
    {
        libnvc::object obj{};
        auto tag = mpack_node_tag(node);
        switch(auto type = mpack_tag_type(&tag)){
            case mpack_type_str:
                {
                    return mp_read<std::string>(node);
                }
            case mpack_type_int:
                {
                    return mp_read<int64_t>(node);
                }
            case mpack_type_bool:
                {
                    return mp_read<bool>(node);
                }
            default:
                {
                    throw std::runtime_error(std::string("unsupport type: ") + std::to_string(type));
                }
        }
    }

    template<> inline std::array<int64_t, 2> mp_read<std::array<int64_t, 2>>(mpack_node_t)
    {
        return {0, 0};
    }

    template<> inline std::map<std::string, libnvc::object> mp_read<std::map<std::string, libnvc::object>>(mpack_node_t node)
    {
        check_node_type<std::map<std::string, libnvc::object>>(node);
        const size_t size = mpack_node_map_count(node);

        std::map<std::string, libnvc::object> options;
        for(size_t index = 0; index < size; ++index){
            auto curr_key = mpack_node_map_key_at(node, index);
            auto curr_val = mpack_node_map_value_at(node, index);
            options[mp_read<std::string>(curr_key)] = mp_read<libnvc::object>(curr_val);
        }
        return options;
    }
}

namespace libnvc
{
    inline void mp_write(mpack_writer_t *)
    {}

    template<typename T, typename... U> inline void mp_write(mpack_writer_t *writer, T t, U... u)
    {
        mp_write(writer, t   );
        mp_write(writer, u...);
    }

    inline void mp_write(mpack_writer_t *writer, const std::array<int64_t, 2> &vec)
    {
        mpack_write_i64(writer, vec[0]);
        mpack_write_i64(writer, vec[1]);
    }

    inline void mp_write(mpack_writer_t *writer, double val)
    {
        mpack_write_double(writer, val);
    }

    inline void mp_write(mpack_writer_t* writer, const std::string& val)
    {
        mpack_write_cstr(writer, val.c_str());
    }

    inline void mp_write(mpack_writer_t* writer, const libnvc::object& obj)
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

    inline void mp_write(mpack_writer_t* writer, const std::map<std::string, libnvc::object>& options)
    {
        mpack_start_map(writer, options.size());
        for(const auto& e : options){
            mp_write(writer, e.first);
            mp_write(writer, e.second);
        }
        mpack_finish_map(writer);
    }
}
