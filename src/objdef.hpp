/*
 * =====================================================================================
 *
 *       Filename: typedef.hpp
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
    using object = std::variant<std::string, int64_t, bool>;
}
