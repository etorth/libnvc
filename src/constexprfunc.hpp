/*
 * =====================================================================================
 *
 *       Filename: constexprfunc.hpp
 *        Created: 12/28/2018 08:49:12
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

namespace libnvc::constexpr_func
{
    void memcpy(void *dst, void *src, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            ((uint8_t *)(dst))[index] = ((uint8_t *)(src))[index];
        }
    }

    size_t strlen()
    {

    }

    int strcmp()
    {

    }

    int memcmp()
    {

    }

    void strcat()
    {
    }

    void memset()
    {

    }
}
