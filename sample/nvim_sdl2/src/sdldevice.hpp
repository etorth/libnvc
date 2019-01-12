/*
 * =====================================================================================
 *
 *       Filename: sdldevice.hpp
 *        Created: 01/10/2019 05:41:00
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
#include <cstring>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class sdl_device
{
    public:
        SDL_Window *m_window;

    public:
        SDL_Renderer *m_renderer;

    private:
        TTF_Font *m_ttf;

    private:
        std::unordered_map<uint32_t, SDL_Texture *> m_fontcache;

    private:
        size_t m_font_width;
        size_t m_font_height;

    public:
        sdl_device(size_t, size_t, const char *, uint8_t);

    public:
        ~sdl_device();

    public:
        SDL_Texture *load_font_image(const char *, uint32_t, bool, bool);

    public:
        size_t font_width() const
        {
            return m_font_width;
        }

        size_t font_height() const
        {
            return m_font_height;
        }
};
