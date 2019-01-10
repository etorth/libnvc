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

#include <map>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class sdl_device
{
    private:
        SDL_Renderer *m_renderer;

    private:
        TTF_Font *m_ttf;

    private:
        std::unordered_map<uint32_t, SDL_Texture *> m_fontcache;

    private:
        size_t m_cell_width;
        size_t m_cell_height;

    public:
        sdl_device(size_t, size_t, uint8_t);

    public:
        void draw_char(
                const char *,   // cstr
                size_t,         // cell_x
                size_t,         // cell_y
                uint32_t,       // color_fg
                uint32_t);      // color_bg

    private:
        SDL_Texture *load_font_image(uint32_t, bool, bool, uint32_t);
};
