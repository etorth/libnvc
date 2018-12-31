/*
 * =====================================================================================
 *
 *       Filename: log.hpp
 *        Created: 12/31/2018 04:44:43
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
#include <functional>
#include "log.hpp"

namespace libnvc
{
    constexpr int LOG_INFO    = 0;
    constexpr int LOG_WARNING = 1;
    constexpr int LOG_FATAL   = 2;
    constexpr int LOG_DEBUG   = 3;

    void log(int, const char *);
    void set_log(std::function<void(int, const char *)>);
}
