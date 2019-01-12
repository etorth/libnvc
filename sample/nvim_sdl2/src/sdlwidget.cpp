/*
 * =====================================================================================
 *
 *       Filename: sdlwidget.cpp
 *        Created: 01/11/2019 08:55:02
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
#include "sdldevice.hpp"
#include "sdlwidget.hpp"

nvim_sdlwidget::nvim_sdlwidget(libnvc::io_device *io_dev, sdl_device *sdl_dev, size_t pixel_width, size_t pixel_height)
    : libnvc::nvim_widget
      {
          io_dev,
          pixel_width / sdl_dev->font_width(),
          pixel_height / sdl_dev->font_height(),

          sdl_dev->font_width(),
          sdl_dev->font_height(),
      }
    , m_sdldev(sdl_dev)
    , m_client(std::make_unique<libnvc::nvim_client>(io_dev, width(), height()))
    , m_pixel_width(pixel_width)
    , m_pixel_height(pixel_height)
{
    // attach the ui
    // this function responds slow
    m_client->nvim_ui_attach(width(), height(), {{"rgb", true}, {"ext_linegrid", true}});
}

void nvim_sdlwidget::draw_char(const struct libnvc_rect *rect_draw, const struct libnvc_rect *rect_show, const char *cstr, uint32_t color_fg, uint32_t color_bg, uint32_t, bool, bool, bool, bool)
{
    auto ptex = m_sdldev->load_font_image(cstr, 0x0f000000 | color_fg, false, false);
    if(ptex == nullptr){
        return;
    }

    int tex_width = 0;
    int tex_height = 0;

    if(SDL_QueryTexture(ptex, 0, 0, &tex_width, &tex_height)){
        throw std::runtime_error(std::string("SDL_QueryTexture() failed: ") + SDL_GetError());
    }

    const struct libnvc_rect *rect_show_ptr;
    struct       libnvc_rect  rect_show_local;

    if(rect_show == nullptr){
        rect_show_local.x = 0;
        rect_show_local.y = 0;
        rect_show_local.w = rect_draw->w;
        rect_show_local.h = rect_draw->h;

        rect_show_ptr = &rect_show_local;
    }else{
        rect_show_ptr = rect_show;
    }

    SDL_Rect dst_rect;
    SDL_Rect src_rect;

    dst_rect.x = rect_draw->x + rect_show_ptr->x;
    dst_rect.y = rect_draw->y + rect_show_ptr->y;

    const int rect_dw = (rect_draw->w - tex_width) / 2;
    const int rect_dh = (rect_draw->h - tex_height) / 2;

    src_rect.x = rect_draw->x - rect_dw;
    src_rect.y = rect_draw->y - rect_dh;
    src_rect.w = std::min<int>(rect_draw->w, tex_width  - src_rect.x);
    src_rect.h = std::min<int>(rect_draw->h, tex_height - src_rect.y);

    dst_rect.w = src_rect.w;
    dst_rect.h = src_rect.h;

    uint8_t a = (color_bg & 0xff000000) >> 24;
    uint8_t r = (color_bg & 0x00ff0000) >> 16;
    uint8_t g = (color_bg & 0x0000ff00) >>  8;
    uint8_t b = (color_bg & 0x000000ff) >>  0;

    SDL_SetRenderDrawColor(m_sdldev->m_renderer, r, g, b, a);
    SDL_RenderClear(m_sdldev->m_renderer);

    SDL_RenderFillRect(m_sdldev->m_renderer, &dst_rect);
    SDL_RenderCopy(m_sdldev->m_renderer, ptex, &src_rect, &dst_rect);
}
