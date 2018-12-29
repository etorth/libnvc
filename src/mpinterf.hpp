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
#include <cstdint>
#include <variant>
#include "mpack.h"
#include "ctf.hpp"
#include "valdef.hpp"
#include "strfunc.hpp"

namespace libnvc
{
    using object = std::variant<std::string, int64_t, bool>;
}

namespace libnvc
{
    template<typename T> T mp_read(mpack_node_t);

    template<typename T> void check_node_type(mpack_node_t node)
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

    template<> bool mp_read<bool>(mpack_node_t node)
    {
        return mpack_node_bool(node);
    }

    template<> int64_t mp_read<int64_t>(mpack_node_t node)
    {
        return mpack_node_i64(node);
    }

    template<> double mp_read<double>(mpack_node_t node)
    {
        return mpack_node_double(node);
    }

    template<> std::string mp_read<std::string>(mpack_node_t node)
    {
        const char *str = mpack_node_str(node);
        size_t      len = mpack_node_strlen(node);
        return std::string(str, str + len);
    }

    template<> libnvc::object mp_read<libnvc::object>(mpack_node_t node)
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

    template<> std::array<int64_t, 2> mp_read<std::array<int64_t, 2>>(mpack_node_t)
    {
        return {0, 0};
    }

    template<> std::map<std::string, libnvc::object> mp_read<std::map<std::string, libnvc::object>>(mpack_node_t node)
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

namespace libnvc
{
    // don't modify
    // entries generated by jinja2

    constexpr static const char * g_nvim_api_string_tbl[]
    {
        // put the zero index as reserved
        nullptr,

        // requests
        "req::nvim_buf_line_count",
        "req::nvim_input",
        "req::buffer_get_line",

        // notifications
        "notif::test",
    };

    inline constexpr const char *idstr(size_t id)
    {
        if(id == 0 || id >= std::extent<decltype(g_nvim_api_string_tbl)>::value){
            return nullptr;
        }
        return g_nvim_api_string_tbl[id];
    }

    inline constexpr size_t strid(const char *str)
    {
        if(str == nullptr){
            return 0;
        }

        for(size_t index = 1; index < std::extent<decltype(g_nvim_api_string_tbl)>::value; ++index){
            if(libnvc::ctf::strcmp(str, g_nvim_api_string_tbl[index]) == 0){
                return index;
            }
        }
        return 0;
    }
}

namespace libnvc
{
    constexpr size_t strid(const char *prefix, const char *name)
    {
        char buf[512] = {'\0'};
        libnvc::ctf::strcat(buf, prefix);
        libnvc::ctf::strcat(buf, "::"  );
        libnvc::ctf::strcat(buf, name  );
        return libnvc::strid(buf);
    }

    constexpr int reqid(const char *req_name)
    {
        return strid("req", req_name);
    }

    constexpr int respid(const char *resp_name)
    {
        return strid("resp", resp_name);
    }

    constexpr int notifid(const char *notif_name)
    {
        return strid("notif", notif_name);
    }
}

namespace libnvc
{
    // we only allow explicit declared req<i>
    // for undefined i just trigger a compilation error
    template<size_t> struct req;

    template<> struct req<libnvc::reqid("nvim_buf_line_count")>
    {
        using parms_t = struct
        {
            int64_t buffer;

            std::string pack(int64_t msgid)
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
        };

        using res_t = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr int deprecated() const
        {
            return 0;
        }

        constexpr auto id() const
        {
            return libnvc::strid("req::nvim_buf_line_count");
        }

        constexpr auto name() const
        {
            return libnvc::idstr(id());
        }
    };

    template<> struct req<libnvc::reqid("nvim_input")>
    {
        using parms_t = struct
        {
            std::string keys;

            std::string pack(int64_t msgid)
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
        };

        using res_t = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr int deprecated() const
        {
            return 0;
        }

        constexpr auto id() const
        {
            return libnvc::strid("req::nvim_input");
        }

        constexpr auto name() const
        {
            return libnvc::idstr(id());
        }
    };
}
