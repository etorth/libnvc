/*
 * =====================================================================================
 *
 *       Filename: libnc.h
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

#ifndef __NVIMCLIENT_H_R93470121277972910__
#define __NVIMCLIENT_H_R93470121277972910__

#ifdef __cplusplus
extern "C" {
#endif

struct libnc_client;

struct libnc_client *libnc_new_client();
void                 libnc_delete_client(struct libnc_client *);

int libnc_draw  (struct libnc_client *);
int libnc_update(struct libnc_client *);

typedef void (*libnc_api_draw_line)(int, int, int, int);
typedef void (*libnc_api_draw_utf8)(int, int, const char *);

int libnc_register_api();

#ifdef __cplusplus
}
#endif

#endif
