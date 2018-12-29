/*
 * =====================================================================================
 *
 *       Filename: main.cpp
 *        Created: 12/28/2018 07:44:36
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

void init_sdl()
{

}

std::string append_input(std::string old, const SDL_Event &e)
{
    return old + 'A';
}

int main()
{
    init_sdl();
    libnvc::default_client client;

    if(!client.connect("localhost", 6666, {})){
        throw std::runtime_error("connect failed");
    }

    while(true){
        SDL_Event e;
        std::string input;
        while(SDL_PollEvent(&e)){
            append_event(&input, e);
        }

        client.forward<libnvc::reqid("nvim_input")>({input}, [input](int64_t done_size)
        {
            if(done_size != input.size()){
                printf("nvim buffer full...");
            }
        });

        client.poll();
    }
}
