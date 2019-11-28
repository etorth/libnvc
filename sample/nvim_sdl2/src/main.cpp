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
#include "sdlutil.hpp"
#include "sdldevice.hpp"
#include "sdlwidget.hpp"

bool process_event(nvim_sdlwidget *pwidget)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT:
                {
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_CLOSE:
                            {
                                return true;
                            }
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            {
                                pwidget->request_resize(event.window.data1, event.window.data2);
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym){
                        case SDLK_BACKSPACE:
                            {
                                pwidget->input_keystr("<BS>");
                                break;
                            }
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
                        case SDLK_TAB:
                            {
                                pwidget->input_keystr("<Tab>");
                                break;
                            }
                        default:
                            {
                                const char ch = sdl_key_event_char(event);
                                if(ch != '\0'){
                                    if(event.key.keysym.mod & KMOD_CTRL){
                                        char buf[128];
                                        std::sprintf(buf, "<C-%c>", ch);
                                        pwidget->input_keystr(buf);
                                    }else if(SDL_IsTextInputActive() == SDL_FALSE){
                                        char buf[128];
                                        std::sprintf(buf, "%c", ch);
                                        pwidget->input_keystr(buf);
                                    }
                                }
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

std::vector<SDL_Texture *> load_bg_imgs(SDL_Renderer *renderer)
{
    std::vector<SDL_Texture *> imgs;
    for(int i = 0;; ++i){
        char namebuf[128];
        std::sprintf(namebuf, "./img/bg%d.jpg", i);

        auto surf = IMG_Load(namebuf);
        if(surf == nullptr){
            break;
        }

        auto tex = SDL_CreateTextureFromSurface(renderer, surf);
        if(tex == nullptr){
            throw std::runtime_error(std::string("SDL_CreateTextureFromSurface() failed: ") + SDL_GetError());
        }

        SDL_FreeSurface(surf);
        imgs.push_back(tex);
    }
    return imgs;
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
    const size_t widget_width  = window_width;
    const size_t widget_height = window_height;

    sdl_device sdl_dev(window_width, window_height, "./font/font.ttf", 16);
    nvim_sdlwidget widget(&socket, &sdl_dev, widget_width, widget_height);
    auto bg_imgs =  load_bg_imgs(sdl_dev.m_renderer);

    while(!process_event(&widget)){
        SDL_SetRenderDrawColor(sdl_dev.m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl_dev.m_renderer);

        if(!bg_imgs.empty()){
            auto tex = bg_imgs.at((SDL_GetTicks() / 100) % bg_imgs.size());
            SDL_SetTextureAlphaMod(tex, 100);
            SDL_RenderCopy(sdl_dev.m_renderer, tex, nullptr, nullptr);
        }

        widget.update();
        widget.draw_ex(0, 0, 0, 0, window_width, window_height);

        SDL_RenderPresent(sdl_dev.m_renderer);
        SDL_Delay(10);
    }

    for(auto p: bg_imgs){
        SDL_DestroyTexture(p);
    }
    return 0;
}
