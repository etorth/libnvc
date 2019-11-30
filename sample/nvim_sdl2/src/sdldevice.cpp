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
#include "sdldevice.hpp"

sdl_device::sdl_device(size_t win_width, size_t win_height, const char *path_ttf, uint8_t ptsize)
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_ttf(nullptr)
    , m_fontcache()
    , m_font_width(0)
    , m_font_height(0)
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
        throw std::runtime_error(std::string(": SDL error: ") + SDL_GetError());
    }

    if(TTF_Init()){
        throw std::runtime_error(std::string(": TTF error: ") + TTF_GetError());
    }

    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        throw std::runtime_error(std::string(": IMG error: ") + IMG_GetError());
    }

    m_window = SDL_CreateWindow("SDLWIDGET", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_RESIZABLE);
    if(m_window == nullptr){
        throw std::runtime_error(std::string(": SDL error: ") + SDL_GetError());
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if(m_renderer == nullptr){
        throw std::runtime_error(std::string(": SDL error: ") + SDL_GetError());
    }

    m_ttf = TTF_OpenFont(path_ttf, ptsize);
    if(m_ttf == nullptr){
        throw std::runtime_error(std::string(": TTF error: ") + TTF_GetError());
    }

    // done initialization
    // check font, calculate window size

    auto psurf1 = TTF_RenderUTF8_Blended(m_ttf, "AMWKOMMMMXXX", {0xff, 0xff, 0xff, 0xff});
    auto psurf2 = TTF_RenderUTF8_Blended(m_ttf, "iiii1111||||", {0xff, 0xff, 0xff, 0xff});

    if(psurf1 == nullptr || psurf2 == nullptr){
        throw std::runtime_error(std::string(": SDL error: ") + SDL_GetError());
    }

    if(psurf1->w != psurf2->w || psurf1->h != psurf2->h){
        // not monospace font
        // 
    }

    m_font_width  = psurf1->w / 12;
    m_font_height = psurf1->h;

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

SDL_Texture *sdl_device::load_font_image(const char *cstr, uint32_t u32argb, bool /* italic */, bool /* bold */)
{
    auto fn_set_image_color = [](SDL_Texture *ptex, uint32_t u32argb) -> SDL_Texture *
    {
        uint8_t a = (u32argb & 0XFF000000) >> 24;
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

    uint32_t utf8;
    std::memcpy(&utf8, cstr, 4);

    if(auto pimg = m_fontcache.find(utf8); pimg != m_fontcache.end()){
        return fn_set_image_color(pimg->second, u32argb);
    }

    char utf8buf[8];
    std::memset(utf8buf,    0, 8);
    std::memcpy(utf8buf, cstr, 4);

    SDL_Color color {0XFF, 0XFF, 0XFF, 0XFF};
    if(auto psurf = TTF_RenderUTF8_Blended(m_ttf, utf8buf, color)){
        if(auto ptex = SDL_CreateTextureFromSurface(m_renderer, psurf)){
            SDL_FreeSurface(psurf);
            m_fontcache[utf8] = ptex;
            return fn_set_image_color(ptex, u32argb);
        }
    }

    m_fontcache[utf8] = nullptr;
    return nullptr;
}
