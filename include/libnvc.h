/*
 * =====================================================================================
 *
 *       Filename: libnvc.h
 *        Created: 06/09/2018 07:04:48
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

#ifndef __LIBNVC_H_18630956136784961__
#define __LIBNVC_H_18630956136784961__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBNVC_LOG_INFO    0
#define LIBNVC_LOG_WARNING 1
#define LIBNVC_LOG_ERROR   2
#define LIBNVC_LOG_DEBUG   3

struct libnvc_client;
struct libnvc_server;

typedef int (*libnvc_api_draw_utf8)(
        uint32_t,       // color, RGBA
        const char *,   // utf-8 string
        int,            // top-left x
        int,            // top-left y
        int *,          // done width
        int *);         // done height

typedef int (*libnvc_api_draw_line)(
        uint32_t,       // color, RGBA
        int,            // x0
        int,            // y0
        int,            // x1
        int);           // y1

typedef int (*libnvc_api_draw_rect)(
        uint32_t,       // color, RGBA
        int,            // x0
        int,            // y0
        int,            // w
        int);           // h

typedef int (*libnvc_api_fill_rect)(
        uint32_t,       // color, RGBA
        int,            // x0
        int,            // y0
        int,            // w
        int);           // h

typedef int (*libnvc_api_draw_hook_before)();
typedef int (*libnvc_api_draw_hook_after )();

struct libnvc_api
{
    libnvc_api_draw_utf8            draw_utf8;
    libnvc_api_draw_line            draw_line;
    libnvc_api_draw_rect            draw_rect;
    libnvc_api_fill_rect            fill_rect;
    libnvc_api_draw_hook_before     draw_hook_before;
    libnvc_api_draw_hook_after      draw_hook_after;
};

struct libnvc_client *libnvc_client_new   (const char *, struct libnvc_api);
void                  libnvc_client_delete(struct libnvc_client *);

int libnvc_draw  (struct libnvc_client *);
int libnvc_update(struct libnvc_client *);

#ifdef __cplusplus
}
#endif

#endif
