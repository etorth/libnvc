/*
 * =====================================================================================
 *
 *       Filename: typedef.hpp
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

namespace libnvc
{
    template<typename T> struct inn_req_t; // leave it unimplemented

    template<> struct inn_req_t<typestring_is("nvim_buf_line_count")>
    {
        using parms_t = struct
        {
            int64_t buffer;
        };

        using result_t = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr int deprecated() const
        {
            return 0;
        }

        constexpr auto type() const
        {
            return typestring_is("nvim_buf_line_count"){};
        }
    };

    template<> struct inn_req_t<typestring_is("buffer_get_line")>
    {
        using parms_t = struct
        {
            int64_t buffer;
            int64_t index;
        };

        using result_t = std::string;

        constexpr int since() const
        {
            return 0;
        }

        constexpr int deprecated() const
        {
            return 0;
        }
    };

    template<> struct inn_req_t<typestring_is("libnvc_api")>
    {
        using parms_t = std::variant
        {
            libncv_req("nvim_buf_line_count")::parms_t,
            libncv_req("buffer_get_line"    )::parms_t,
        };

        using result_t = std::variant
        {
            libncv_req("nvim_buf_line_count")::result_t,
            libncv_req("buffer_get_line"    )::result_t,
        };
    };
}

namespace libnvc
{
    constexpr char * g_reqname[]
    {
        "nvim_buf_line_count",
        "buffer_get_line",
    };

    inline constexpr size_t cstrseq(const char *name)
    {
        constexpr char * g_neovim_apistring[]
        {
            "",
            "req:nvim_buf_line_count",
            "req:buffer_get_line",
        };

        constexpr size_t size = std::extent<decltype(g_neovim_apistring)>::value;

        if(name == nullptr){
            return size;
        }

        for(auto p = g_neovim_apistring; p != g_neovim_apistring + size; ++p){
            for(size_t index = 0; ++index){
                if((*p)[index] != name[index]){
                    return false;
                }

                if((*p)[index] == '\0'){
                    return true;
                }
            }
        }
    }
}

namespace libnvc
{
    template<size_t> req;

    template<> req<libncv::cstr2id("req:nvim_buf_line_count")>
    {
        using parms_t = struct
        {
            int64_t buffer;
        };

        using result_t = int64_t;

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
            return libnvc::cstr2id("req:nvim_buf_line_count");
        }

        constexpr auto name() const
        {
            return libnvc::id2cstr(id());
        }
    }
}
