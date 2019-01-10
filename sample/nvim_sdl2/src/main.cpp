/*
 * =====================================================================================
 *
 *       Filename: main.cpp
 *        Created: 12/29/2018 06:56:01
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
#include <cstddef>
#include <cinttypes>
#include <stdexcept>
#include "libnvc.hpp"
#include "nvimproc.hpp"
#include "sdldevice.hpp"

bool process_event(libnvc::nvim_client *pclient)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT:
                {
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE){
                        return true;
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            {
                                pclient->nvim_input("<ESC>");
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }
                    break;
                }
            case SDL_TEXTINPUT:
                {
                    pclient->nvim_input(event.text.text);
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
    return false;
}

class nvim_sdlwidget: public libnvc::nvim_widget
{
    private:
        nvim_proc  *m_proc;
        sdl_device *m_device;

    public:
        nvim_sdlwidget(nvim_proc *piodev, sdl_device *pgfxdev)
            : m_proc(piodev)
            , m_device(pgfxdev)
        {}

    public:
        void draw_char(const char *cstr, size_t cell_x, size_t cell_y, uint32_t color_fg, uint32_t color_bg, uint32_t, bool, bool, bool, bool)
        {
            m_device->draw_char(cstr, cell_x, cell_y, color_fg, color_bg);
        }
}

int main()
{
    const size_t w = 100;
    const size_t h =  40;

    sdl_device gfxdev(w, h, 10);

    nvim_proc proc;
    libnvc::nvim_client client(&proc, w, h);

    nvim_sdlwidget widget(&proc, &gfxdev);

    // attach the ui
    // this function responds slow
    client.nvim_ui_attach(client.width(), client.height(), {{"rgb", true}, {"ext_linegrid", true}});

    while(process_event(&client)){
        widget.draw();
        SDL_Delay(10);
    }
    return 0;
}
