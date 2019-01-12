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
#include "sdldevice.hpp"
#include "sdlwidget.hpp"

bool process_event(nvim_sdlwidget *pwidget)
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
                        case SDLK_RETURN:
                            {
                                pwidget->input_keystr("<CR>");
                                break;
                            }
                        case SDLK_ESCAPE:
                            {
                                pwidget->input_keystr("<ESC>");
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
                    pwidget->input_keystr(event.text.text);
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

int main()
{
    // start nvim quickly:
    // $ nvim --headless --listen "127.0.0.1:6666"
    libnvc::asio_socket socket;
    if(!socket.connect("localhost", 6666)){
        throw std::runtime_error("Failed to connect to localhost:6666");
    }

    const size_t window_width  = 800;
    const size_t window_height = 600;
    const size_t widget_width  = 800;
    const size_t widget_height = 600;

    sdl_device sdl_dev(window_width, window_height, "./font.ttf", 15);
    nvim_sdlwidget widget(&socket, &sdl_dev, widget_width, widget_height);

    while(!process_event(&widget)){
        SDL_SetRenderDrawColor(sdl_dev.m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl_dev.m_renderer);
        widget.draw_ex(0, 0, 0, 0, window_width, window_height);
        SDL_RenderPresent(sdl_dev.m_renderer);
        SDL_Delay(10);
    }
    return 0;
}
