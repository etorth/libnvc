/*
 * =====================================================================================
 *
 *       Filename: client.hpp
 *        Created: 12/26/2018 01:57:33
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
#include <cinttypes>
#include "libnvc.h"

class nvc_inn_client
{
    public:
        nvc_inn_client(const char *, struct libnvc_api);

    public:
        ~nvc_inn_client() = default;
};
