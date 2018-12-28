/*
 * =====================================================================================
 *
 *       Filename: socket.hpp
 *        Created: 12/26/2018 05:12:41
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
#include <vector>
#include <cstddef>
#include "strfunc.hpp"

namespace libnvc
{
    class socket
    {
        public:
            size_t m_start;
            size_t m_capacity;

        public:
            std::vector<uint8_t> m_buf;

        public:
            socket(const char *, int, size_t);

        public:
            ~sockect();

        public:
            void read();
            void write(const uint8_t *, size_t);

        public:
            const uint8_t *buf_data() const
            {
                return m_buf.data() + m_start;
            }

            size_t buf_size() const
            {
                return m_buf.size() - m_start;
            }

            void buf_consume(size_t size)
            {
                if(m_start + size >= m_buf.size()){
                    throw std::invalid_argument(str_fflprintf(": consume more than buffered data: %zu", size));
                }
                m_start += size;

                if(buf_size() == 0){
                    buf_clear();
                }
            }

            void buf_clear()
            {
                m_start = 0;
                m_buf.clear();

                // notice: non-binding request
                // shrink_to_fit mat have no effect, depends on implementation

                if(m_capacity > 0){
                    m_buf.shrink_to_fit(m_capacity);
                }
            }
    };
}
