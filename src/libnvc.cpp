/*
 * =====================================================================================
 *
 *       Filename: libnvc.cpp
 *        Created: 06/09/2018 07:04:48
 *    Description: mxtk C style api export to user
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

#include <stdexcept>
#include "libnvc.h"
#include "client.hpp"

static void handle_exception(const nvc_inn_client &client)
{
    try{
        throw;
    }catch(const std::exception &e){
        client.log(LIBNVC_LOG_ERROR, e.what().c_str());
    }
}

struct libnvc_client *libnvc_client_new(const char *address, struct libnvc_api api)
{
    try{
        return (struct libnvc_client *)(new nvc_inn_client(address, api));
    }catch(...){
        handle_exception();
    }
}

void libnvc_client_delete(struct libnvc_client *client)
{
    if(client == nullptr){
        return;
    }
    delete (nvc_inn_client *)(client);
}

void libnvc_update(struct libnvc_client *client, float ms)
{
    try{
        return ((nvc_inn_client *)(client))->update(ms);
    }catch(...){
        handle_exception(client);
    }
}

void libnvc_draw(struct libnvc_client *client)
{
    try{
        return ((nvc_inn_client *)(client))->draw();
    }catch(...){
        handle_exception(client);
    }
}
