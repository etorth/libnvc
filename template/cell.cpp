/*
 * =====================================================================================
 *
 *       Filename: cell.cpp
 *        Created: 01/08/2019 00:24:42
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

#include "libnvc.hpp"
libnvc::nvim_box::CELL::CELL(uint32_t bg)
    : utf8code(0)
    , color_fg(0)
    , color_bg(bg)
    , color_sp(0)
{}
