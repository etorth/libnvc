/*
 * =====================================================================================
 *
 *       Filename: objdef.hpp
 *        Created: 12/30/2018 02:51:09
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
#include <string>
#include <variant>
#include <cstdint>

namespace libnvc
{
    struct nil_type
    {
        int unused = 0;
    };

    using nil_t = struct nil_type;
    using object = std::variant<libnvc::nil_t, std::string, int64_t, bool>;
}
