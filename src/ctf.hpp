/*
 * =====================================================================================
 *
 *       Filename: ctf.hpp
 *        Created: 12/28/2018 08:49:12
 *    Description: compile-time-functions
 *                 1. constexpr
 *                 2. simple
 *                 3. nothrow
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
#include <cstddef>

namespace libnvc::ctf
{
    constexpr void memcpy(void *dst, const void *src, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            ((char *)(dst))[index] = ((char *)(src))[index];
        }
    }

    constexpr size_t strlen(const char *str)
    {
        size_t index = 0;
        while(str[index] != '\0'){
            ++index;
        }
        return index;
    }

    constexpr int strcmp(const char *str1, const char *str2)
    {
        for(size_t index = 0;; ++index){
            if(str1[index] < str2[index]){
                return -1;
            }

            if(str1[index] > str2[index]){
                return 1;
            }

            if(str1[index] == '\0'){
                return 0;
            }
        }
    }

    constexpr int memcmp(const void *mem1, const void *mem2, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            if(((uint8_t *)(mem1))[index] < ((uint8_t *)(mem2))[index]){
                return -1;
            }

            if(((uint8_t *)(mem1))[index] > ((uint8_t *)(mem2))[index]){
                return 1;
            }
        }
        return 0;
    }

    constexpr char *strcat(char *dst, const char *src)
    {
        size_t index  = 0;
        size_t length = libnvc::ctf::strlen(dst);

        for(index = 0; src[index] != '\0'; ++index){
            dst[length + index] = src[index];
        }

        dst[length + index] = '\0';
        return dst;
    }

    constexpr void chmemset(char *dst, char value, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            dst[index] = value;
        }
    }
}
