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
#include "strfunc.hpp"

libnvc::nvim_widget::nvim_widget(libnvc::io_device * pdev, size_t width, size_t height, size_t cell_width, size_t cell_height)
    : m_client(std::make_unique<libnvc::nvim_client>(pdev, width, height))
    , m_cell_width(cell_width)
    , m_cell_height(cell_height)
{
    if(m_cell_width == 0 || m_cell_height == 0){
        throw std::invalid_argument(str_fflprintf(": Invalid argument: nvim_widget(%p, %zu, %zu, %zu, %zu)", pdev, width, height, cell_width, cell_height));
    }
}

static bool inn_overlap_region(const struct libnvc_rect &r1, struct libnvc_rect *r2)
{
    if(false
            || (r1.x >= r2->x + r2->w) || (r1.x + r1.w < r2->x)
            || (r1.y >= r2->y + r2->h) || (r1.y + r1.h < r2->y)){
        return false;
    }

    int x = std::max<int>(r1.x, r2->x);
    int y = std::max<int>(r1.y, r2->y);
    int w = std::min<int>(r1.x + r1.w, r2->x + r2->w) - x;
    int h = std::min<int>(r1.y + r1.h, r2->y + r2->h) - y;

    r2->x = x;
    r2->y = y;
    r2->w = w;
    r2->h = h;

    return true;
}

void libnvc::nvim_widget::draw_ex(int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h)
{
    const int w = width();
    const int h = height();

    const int cell_w = cell_width();
    const int cell_h = cell_height();

    const int dst_cell0_x = dst_x - src_x;
    const int dst_cell0_y = dst_y - src_y;

    libnvc_rect src;
    libnvc_rect all;

    src.x = src_x;
    src.y = src_y;
    src.w = src_w;
    src.h = src_h;

    all.x = 0;
    all.y = 0;
    all.w = w * cell_w;
    all.h = h * cell_h;

    if(!inn_overlap_region(all, &src)){
        return;
    }

    dst_x += (src.x - src_x);
    dst_y += (src.y - src_y);

    const int x0 = src.x / cell_w;
    const int y0 = src.y / cell_h; 

    const int x1 = (src.x + src.w - 1) / cell_w;
    const int y1 = (src.y + src.h - 1) / cell_h;

    int64_t hl_id = 0;
    const auto *hl_def0 = &(m_client->get_hl_attrdef(0));
    const auto *hl_def  = hl_def0;

    for(int y = y0; y <= y1; ++y){
        for(int x = x0; x <= x1; ++x){

            libnvc_rect curr_src;
            libnvc_rect curr_dst;

            curr_src.x = cell_w * x;
            curr_src.y = cell_h * y;
            curr_src.w = cell_w;
            curr_src.h = cell_h;

            curr_dst.x = curr_src.x + dst_cell0_x;
            curr_dst.y = curr_src.y + dst_cell0_y;
            curr_dst.w = curr_src.w;
            curr_dst.h = curr_src.h;

            libnvc_rect  rect_show;
            libnvc_rect *rect_show_ptr = nullptr;

            if(x == x0 || x == x1 || y == y0 || y == y1){

                // need to clip the cell
                // we are crossing the boundary

                if(!inn_overlap_region(src, &curr_src)){
                    throw std::runtime_error(str_fflprintf(": Can't reach here, boundary calculation failed"));
                }

                rect_show.x = curr_src.x % cell_w;
                rect_show.y = curr_src.y % cell_h;
                rect_show.w = curr_src.w;
                rect_show.h = curr_src.h;

                rect_show_ptr = &rect_show;
            }

            auto &curr_cell = m_client->get_backcell(x, y);
            if(hl_id != curr_cell.hl_id()){
                hl_id = curr_cell.hl_id();
                hl_def = &(m_client->get_hl_attrdef(hl_id));
            }

            uint32_t color_fg = hl_def->color_fg_defined ? hl_def->color_fg : hl_def0->color_fg;
            uint32_t color_bg = hl_def->color_bg_defined ? hl_def->color_bg : hl_def0->color_bg;
            uint32_t color_sp = hl_def->color_sp_defined ? hl_def->color_sp : hl_def0->color_sp;

            if(hl_def->reverse){
                std::swap(color_fg, color_bg);
            }
            draw_char(&curr_dst, rect_show_ptr, curr_cell.len4_cstr(), color_fg, color_bg, color_sp, hl_def->italic, hl_def->bold, hl_def->underline, hl_def->undercurl);
        }
    }
}
