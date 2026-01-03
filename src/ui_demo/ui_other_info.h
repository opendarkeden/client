/**
 * @file ui_other_info.h
 * @brief Other Info panel (ported from C_VS_UI_OTHER_INFO)
 */

#ifndef UI_OTHER_INFO_H
#define UI_OTHER_INFO_H

#include "ui_window.h"
#include "ui_global_resource.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_OtherInfo UI_OtherInfo; /* Alias C_VS_UI_OTHER_INFO */

typedef struct UI_OtherInfoPlayer {
    char  name[64];
    int   domain_level[5];
    int   grade;
    int   level;
    int   fame;
    int   alignment;
    int   str_cur, dex_cur, int_cur;
    int   str_pure, dex_pure, int_pure;
    int   guild_id;
    int   face_type;
    int   female; /* bool-like */
} UI_OtherInfoPlayer;

struct UI_OtherInfo {
    UI_Window base; /* Inherit window */
    UI_GlobalResource* gr;
    int visible;
    UI_OtherInfoPlayer player;
    SpritePack info_pack;   /* InfoSlayer.spk (or race variant) */
    int info_loaded;
    int race;               /* 0=Slayer, 1=Vampire, 2=Ousters */
    SDL_Rect close_rect;    /* Click target for close */
};

UI_Window* ui_other_info_create(UI_GlobalResource* gr);
void ui_other_info_start(UI_Window* window);
void ui_other_info_finish(UI_Window* window);
int ui_other_info_is_visible(UI_Window* window);
void ui_other_info_set_player(UI_Window* window, const UI_OtherInfoPlayer* player);
void ui_other_info_set_race(UI_Window* window, int race); /* 0=Slayer,1=Vampire,2=Ousters */

#ifdef __cplusplus
}
#endif

#endif /* UI_OTHER_INFO_H */
