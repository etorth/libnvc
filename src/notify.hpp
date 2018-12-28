/*
 * =====================================================================================
 *
 *       Filename: notify.hpp
 *        Created: 12/27/2018 05:16:53
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
#include "mpack.h"

namespace libnvc::type
{
    class notify
    {
        private:
            mpack_tree_t m_tree;
            mpack_node_t m_root;

        private:
            // mpack_tree_t doesn't own the data
            // but it requires the data to be valid during the whole tree life
            std::vector<uint8_t> m_data;

        public:
            notify(const uint8_t *data, size_t size)
                : m_data(data, data + size)
            {
                parse();
            }

            notify(std::vector<uint8_t> data)
                : m_data(data)
            {
                parse();
            }

            notify(const notify &other)
                : m_data(other.m_data)
            {
                parse();
            }

            notify(notify &&other)
                : m_data(std::move(other.m_data))
            {
                m_tree = other.m_tree;
                m_root = other.m_root;

                // bind a tree life to it's data buf
                // a tree with empty data buf should be moved already
                other.m_data.clear();
            }

        public:
            ~notify()
            {
                if(m_data.empty()){
                    return;
                }

                if(mpack_tree_destroy(&m_tree) != mpack_ok){
                    libnvc::log(LIBNVC_LOG_FATAL, str_fflprintf(": Tree pack failed").c_str());
                }
            }

        public:
            void parse()
            {
                mpack_tree_init (&m_tree, m_data.data(), m_data.size());
                mpack_tree_parse(&m_tree);

                m_root = mpack_tree_root(&m_tree);
            }
    };
}
