/*
 * =====================================================================================
 *
 *       Filename: sdldevice.cpp
 *        Created: 01/10/2019 05:45:48
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

#include <cstdint>
#include <cinttypes>

sdl_device::sdl_device(const char *path_ttf, uint8_t ptsize)
    : m_renderer(nullptr)
    , m_ttf(nullptr)
    , m_fontcache()
    , m_cell_width(0)
    , m_cell_height(0)
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
        throw std::runtime_error(str_fflprintf(": SDL error: %s", SDL_GetError()));
    }

    if(TTF_Init()){
        throw std::runtime_error(str_fflprintf(": TTF error: %s", TTF_GetError()));
    }

    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        throw std::runtime_error(str_fflprintf(": IMG error: %s", IMG_GetError()));
    }

    m_renderer = SDL_CreateRenderer(mxtkDriver_getSDLWindow(), -1, SDL_RENDERER_ACCELERATED);
    if(m_renderer == nullptr){
        throw std::runtime_error(str_fflprintf(": SDL error: %s", SDL_GetError()));
    }

    m_ttf = TTF_OpenFont(path_ttf, ptsize);
    if(m_ttf == nullptr){
        throw std::runtime_error(str_fflprintf(": TTF_Open(%s) failed: %s", path_ttf, TTF_GetError()));
    }

    // done initialization
    // check font, calculate window size

    auto psurf1 = TTF_RenderUTF8_Blended(m_ttf, "AMWKOMMMMXXX", {0xff, 0xff, 0xff, 0xff});
    auto psurf2 = TTF_RenderUTF8_Blended(m_ttf, "iiiiii111111", {0xff, 0xff, 0xff, 0xff});

    if(psurf1 == nullptr || psurf2 == nullptr){
        throw std::runtime_error(str_fflprintf(": Failed to create SDL_Surface: %s", SDL_GetError()));
    }

    if(psurf1->w != psurf2->w || psurf1->h != psurf2->h){
        // not monospace font
        // 
    }

    m_gridw = psurf1->w / 12;
    m_gridh = psurf1->h;

    SDL_FreeSurface(psurf1);
    SDL_FreeSurface(psurf2);
}

sdl_device::~sdl_device()
{
    for(auto elem: m_fontcache){
        SDL_DestroyTexture(elem.second);
    }
    m_fontcache.clear();

    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);

    m_window   = nullptr;
    m_renderer = nullptr;
}

SDL_Texture *sdl_device::load_font_image(uint32_t utf8, uint32_t u32argb, bool /* italic */, bool /* bold */)
{
    auto fn_set_image_color = [](SDL_Texture *ptex, uint32_t u32argb) -> SDL_Texture *
    {
        uint8_t a = (u32argb & 0X00FF0000) >> 24;
        uint8_t r = (u32argb & 0X00FF0000) >> 16;
        uint8_t g = (u32argb & 0X0000FF00) >>  8;
        uint8_t b = (u32argb & 0X000000FF) >>  0;

        if(ptex){
            SDL_SetTextureColorMod(ptex, r, g, b);
            SDL_SetTextureAlphaMod(ptex, a);
            return ptex;
        }
        return nullptr;
    };

    if(auto pimg = m_fontcache.find(utf8); pimg != m_fontcache.end()){
        return fn_set_image_color(pimg->second, u32argb);
    }

    char utf8buf[8];
    std::memset(utf8buf, 0, sizeof(utf8buf));
    std::memcpy(utf8buf, &utf8, 4);

    SDL_Color color {0XFF, 0XFF, 0XFF, 0XFF};
    if(auto psurf = TTF_RenderUTF8_Blended(m_ttf, utf8buf, color)){
        if(auto ptex = SDL_CreateTextureFromSurface(m_renderer, psurf)){
            SDL_FreeSurface(psurf);
            m_fontcache[utf8] = ptex;
            return fn_set_image_color(ptex);
        }
    }

    m_fontcache[utf8] = nullptr;
    return nullptr;
}

void sdl_device::draw_char(size_t cell_x, size_t cell_y, uint32_t utf8, uint32_t color_fg, uint32_t color_bg, uint32_t color_sp, bool italic, bool bold, bool underline, bool undercurl)
{
    size_t pixel_x = cell_x * m_cell_width;
    size_t pixel_y = cell_y * m_cell_height;

    const uint32_t a_mask = 0xf0000000;
    fill_rectangle(a_mask | bg_u24rgb, pixel_x, pixel_y, m_cell_width, m_cell_height);

    auto ptex = load_font_image(utf8, 0xf0000000 | u24rgb, italic, bold);
    if(ptex == nullptr){
        return;
    }

    SDL_Rect dst_rect;
    dst_rect.x = pixel_x;
    dst_rect.y = pixel_y;
    dst_rect.w = m_cell_width;
    dst_rect.h = m_cell_height;

    SDL_RenderCopy(m_renderer, ptex, nullptr, &dst_rect);
}
