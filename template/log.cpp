/*
 * =====================================================================================
 *
 *       Filename: log.cpp
 *        Created: 12/31/2018 04:49:42
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

#include <mutex>
#include <cstdio>
#include "libnvc.hpp"
#include "strfunc.hpp"

// give a basic implementation
// later to switch to better log functionality

static void default_log_func(int log_type, const char *log_str)
{
    switch(log_type){
        case libnvc::LOG_INFO    : std::printf(   "INFO: %s\n", log_str); return;
        case libnvc::LOG_WARNING : std::printf("WARNING: %s\n", log_str); return;
        case libnvc::LOG_FATAL   : std::printf(  "FATAL: %s\n", log_str); return;
        case libnvc::LOG_DEBUG   : std::printf(  "DEBUG: %s\n", log_str); return;
        default                  :                                        return;
    }
}

static std::mutex g_log_mutex;
static std::function<void(int, const char *)> g_log_func = default_log_func;

void libnvc::set_log(std::function<void(int, const char *)> log_func)
{
    std::lock_guard<std::mutex> guard(g_log_mutex);
    if(!log_func){
        g_log_func = default_log_func;
    }else{
        g_log_func = log_func;
    }
}

void libnvc::log(int log_type, const char *log_str)
{
    if(log_str == nullptr){
        log_type = libnvc::LOG_WARNING;
        log_str  = "(nullptr)";
    }

    std::lock_guard<std::mutex> guard(g_log_mutex);
    switch(log_type){
        case libnvc::LOG_INFO:
        case libnvc::LOG_WARNING:
        case libnvc::LOG_FATAL:
        case libnvc::LOG_DEBUG:
            {
                g_log_func(log_type, log_str);
                return;
            }
        default:
            {
                g_log_func(libnvc::LOG_WARNING, str_fflprintf("unknown log type (%d) with message: %s", log_type, log_str).c_str());
                return;
            }
    }
}
