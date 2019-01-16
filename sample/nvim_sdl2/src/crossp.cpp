/*
 * =====================================================================================
 *
 *       Filename: crossp.cpp
 *        Created: 01/15/2019 23:45:13
 *    Description: for cross-platform
 *                 put all dirty function implementations here
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

#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include <cstring>
#include <stdexcept>
#include <filesystem>
#include "crossp.hpp"

const std::string &crossp::get_exe_dir()
{
    const static std::string s_exe_dir = []()
    {
        char buf[PATH_MAX];
        std::memset(buf, 0, sizeof(buf));

        if(readlink("/proc/self/exe", buf, PATH_MAX) == -1){
            throw std::runtime_error(std::strerror(errno));
        }
        return std::filesystem::path(buf).parent_path();
    }();
    return s_exe_dir;
}
