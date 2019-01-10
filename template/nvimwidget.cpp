/*
 * =====================================================================================
 *
 *       Filename: nvimwidget.cpp
 *        Created: 01/07/2019 00:01:24
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
void libnvc::nvim_widget::draw()
{
    int64_t hl_id = 0;
    const auto *hl_def0 = &(m_client->get_hl_attrdef(0));
    const auto *hl_def  = hl_def0;

    for(size_t y = 0; y < m_client->height(); ++y){
        for(size_t x = 0; x < m_client->width(); ++x){
            auto &cell = m_client->get_backcell(x, y);
            if(hl_id != cell.hl_id()){
                hl_id = cell.hl_id();
                hl_def = &(m_client->get_hl_attrdef(hl_id));
            }

            uint32_t color_fg = hl_def->color_fg_defined ? hl_def->color_fg : hl_def0->color_fg;
            uint32_t color_bg = hl_def->color_bg_defined ? hl_def->color_bg : hl_def0->color_bg;
            uint32_t color_sp = hl_def->color_sp_defined ? hl_def->color_sp : hl_def0->color_sp;

            if(hl_def->reverse){
                std::swap(color_fg, color_bg);
            }
            draw_text(x, y, color_fg, color_bg, color_sp, hl_def->underline, hl_def->undercurl, cell.len4_cstr());
        }
    }
}
