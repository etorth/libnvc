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
    , m_pixel_width(pixel_width)
    , m_pixel_height(pixel_height)
{
    // attach the ui
    // this function responds slow
    m_client->nvim_ui_attach(width(), height(), {{"rgb", true}, {"ext_linegrid", true}});
}

void nvim_sdlwidget::draw_ex(int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h)
{
    const int xoff = dst_x + (pixel_width() - width() * m_sdldev->font_width()) / 2;
    const int yoff = dst_y + (pixel_height() - height() * m_sdldev->font_height()) / 2;
    libnvc::nvim_widget::draw_ex(dst_x + xoff, dst_y + yoff, src_x, src_y, src_w, src_h);
}

void nvim_sdlwidget::draw_char(const struct libnvc_rect *rect_full, const struct libnvc_rect *rect_show, const char *cstr, uint32_t color_fg, uint32_t color_bg, uint32_t, bool, bool, bool, bool)
{
    if(rect_full == nullptr){
        return;
    }

    SDL_Rect bg_dst;
    if(rect_show == nullptr){
        bg_dst.x = rect_full->x;
        bg_dst.y = rect_full->y;
        bg_dst.w = rect_full->w;
        bg_dst.h = rect_full->h;
    }else{
        if(rect_show->x < 0){
            bg_dst.x = rect_full->x;
            bg_dst.w = rect_show->w + rect_show->x;
        }else{
            bg_dst.x = rect_full->x + rect_show->x;
            bg_dst.w = rect_show->w;
        }

        if(rect_show->y < 0){
            bg_dst.y = rect_full->y;
            bg_dst.h = rect_show->h + rect_show->y;
        }else{
            bg_dst.y = rect_full->y + rect_show->y;
            bg_dst.h = rect_show->h;
        }
    }

    if(bg_dst.w <= 0 || bg_dst.h <= 0){
        return;
    }

    uint8_t r = (color_bg & 0x00ff0000) >> 16;
    uint8_t g = (color_bg & 0x0000ff00) >>  8;
    uint8_t b = (color_bg & 0x000000ff) >>  0;

    SDL_SetRenderDrawBlendMode(m_sdldev->m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_sdldev->m_renderer, r, g, b, 128);
    SDL_RenderFillRect(m_sdldev->m_renderer, &bg_dst);

    if(std::strcmp(cstr, " ") == 0){
        return;
    }

    auto ptex = m_sdldev->load_font_image(cstr, 0xf0000000 | color_fg, false, false);
    if(ptex == nullptr){
        return;
    }

    int tex_width  = -1;
    int tex_height = -1;

    if(SDL_QueryTexture(ptex, 0, 0, &tex_width, &tex_height)){
        throw std::runtime_error(std::string("SDL_QueryTexture() failed: ") + SDL_GetError());
    }

    if(tex_width <= 0 || tex_height <= 0){
        return;
    }

    const int rect_dw = (rect_full->w - tex_width ) / 2;
    const int rect_dh = (rect_full->h - tex_height) / 2;

    const int font_full_x = rect_full->x + rect_dw;
    const int font_full_y = rect_full->y + rect_dh;
    const int font_full_w = tex_width;
    const int font_full_h = tex_height;

    SDL_Rect font_dst;
    font_dst.x = std::max<int>(bg_dst.x, font_full_x);
    font_dst.y = std::max<int>(bg_dst.y, font_full_y);
    font_dst.w = std::min<int>(bg_dst.x + bg_dst.w, font_full_x + font_full_w) - font_dst.x;
    font_dst.h = std::min<int>(bg_dst.y + bg_dst.h, font_full_y + font_full_h) - font_dst.y;

    SDL_Rect font_src;
    font_src.x = font_dst.x - font_full_x;
    font_src.y = font_dst.y - font_full_y;
    font_src.w = font_dst.w;
    font_src.h = font_dst.h;

    SDL_RenderCopy(m_sdldev->m_renderer, ptex, &font_src, &font_dst);
}
