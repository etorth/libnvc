/*
 * =====================================================================================
 *
 *       Filename: sdlwidget.hpp
 *        Created: 01/11/2019 08:54:40
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

#include <cstddef>
#include "libnvc.hpp"
#include "sdldevice.hpp"

class nvim_sdlwidget: public libnvc::nvim_widget
{
    private:
        sdl_device *m_sdldev;

    private:
        size_t m_pixel_width;
        size_t m_pixel_height;

    public:
        nvim_sdlwidget(libnvc::io_device *, sdl_device *, size_t, size_t);

    public:
        size_t pixel_width() const
        {
            return m_pixel_width;
        }

        size_t pixel_height() const
        {
            return m_pixel_height;
        }

    public:
        virtual void draw_char(const struct libnvc_rect *, const struct libnvc_rect *, const char *, uint32_t, uint32_t, uint32_t, bool, bool, bool, bool);

    public:
        virtual void draw_ex(int, int, int, int, int, int);

    public:
        void input_keystr(const char *keystr)
        {
            m_client->nvim_input(keystr);
        }

        void request_resize(size_t w, size_t h)
        {
            // resize the pixel width/height immediately here
            // the grid_resize notif handler can't access this widget class
            m_pixel_width = w;
            m_pixel_height = h;
            m_client->nvim_ui_try_resize(w / m_sdldev->font_width(), h / m_sdldev->font_height());
        }

    public:
        void update()
        {
            m_client->poll();
        }
};
