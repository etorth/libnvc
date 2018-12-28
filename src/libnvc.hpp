/*
 * =====================================================================================
 *
 *       Filename: libnvc.hpp
 *        Created: 12/27/2018 04:52:17
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
#include <variant>

namespace
{
    using object = std::variant<std::string, std::int64_t, bool>;
}
