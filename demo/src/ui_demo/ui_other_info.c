/**
 * @file ui_other_info.c
 * @brief Other Info panel (simplified port of C_VS_UI_OTHER_INFO)
 */

#include "ui_other_info.h"
#include "ui_surface.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sprite.h"

static void ui_game_otherinfo_show(UI_Window* window);
static void ui_game_otherinfo_process(UI_Window* window);
static int ui_game_otherinfo_mouse_control(UI_Window* window, uint32_t message, int x, int y);
static void ui_game_otherinfo_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra);

/* Race-specific info packs. */
#define INFO_SPK_SLAYER_PATH   "DarkEden/Data/Ui/spk/InfoSlayer.spk"
#define INFO_SPK_VAMPIRE_PATH  "DarkEden/Data/Ui/spk/InfoVampire.spk"
#define INFO_SPK_OUSTERS_PATH  "DarkEden/Data/Ui/spk/InfoOusters.spk"

/* Minimal subset of indices used by C_VS_UI_OTHER_INFO for Slayer layout. */
enum InfoSpriteIndex {
    INFO_TAB_CHAR = 0,
    INFO_TAB_SKILL,
    INFO_TAB_GRADE1,
    INFO_TAB_GRADE2,
    INFO_TAB_GRADE3,
    INFO_BUTTON_CLOSE_BACK,
    INFO_BUTTON_CLOSE,
    INFO_BUTTON_CLOSE_PUSHED,
    INFO_BUTTON_CLOSE_HILIGHTED,
    INFO_BUTTON_CLOSE_HILIGHTED_PUSHED,
    INFO_BUTTON_HELP_BACK,
    INFO_BUTTON_HELP,
    INFO_BUTTON_HELP_PUSHED,
    INFO_BUTTON_HELP_HILIGHTED,
    INFO_BUTTON_HELP_HILIGHTED_PUSHED,
    INFO_CHAR_BOX,
    INFO_DESC_BOX,
    INFO_LARGE_BAR,
    INFO_LARGE_BAR_RIGHT,
    INFO_SMALL_BAR,
    INFO_SMALL_BAR2,
    INFO_EXP_BAR,
    INFO_GUILD_BACK,
    INFO_SKILL_TITLE,
    INFO_SKILL_ICON_BACK,
    INFO_SKILL_BACK,
    INFO_SKILL_BACK_HILIGHTED,
    INFO_SKILL_BACK_ALPHA,
    INFO_SKILL_BOX,
    INFO_TITLE_NAME,
    INFO_TITLE_ALIGN,
    INFO_TITLE_GUILD,
    INFO_TITLE_STR,
    INFO_TITLE_DEX,
    INFO_TITLE_INT,
    INFO_TITLE_HP,
    INFO_TITLE_TOHIT,
    INFO_TITLE_DAMAGE,
    INFO_TITLE_DEFENCE,
    INFO_TITLE_PROTECTION,
    INFO_GRADE_LEVEL,
    INFO_GRADE_LEVEL_HILIGHTED,
    INFO_GRADE_EXP,
    INFO_GRADE_EXP_HILIGHTED,
    INFO_GRADE_RED_BUTTON,
    INFO_GRADE_RED_BUTTON_DISABLE,
    INFO_TITLE_FAME,
    INFO_TITLE_MP,
    INFO_TITLE_DOMAINLEVEL,
    INFO_TITLE_DOMAINEXP,
    INFO_PLUS_BUTTON,
    INFO_PLUS_BUTTON_HILIGHTED,
    INFO_PLUS_BUTTON_HILIGHTED_PUSHED,
    INFO_MINUS_BUTTON,
    INFO_MINUS_BUTTON_HILIGHTED,
    INFO_MINUS_BUTTON_HILIGHTED_PUSHED,
    INFO_SUM_BUTTON,
    INFO_SUM_BUTTON_HILIGHTED,
    INFO_SUM_BUTTON_HILIGHTED_PUSHED,
    INFO_DOMAIN_BACK,
    INFO_DOMAIN_BAR,
    INFO_DM_BLADE,
    INFO_DM_BLADE_HILIGHTED,
    INFO_DM_SWORD,
    INFO_DM_SWORD_HILIGHTED,
    INFO_DM_GUN,
    INFO_DM_GUN_HILIGHTED,
    INFO_DM_HEAL,
    INFO_DM_HEAL_HILIGHTED,
    INFO_DM_ENCHANT,
    INFO_DM_ENCHANT_HILIGHTED,
    INFO_DM_TOTAL,
    INFO_DM_TOTAL_HILIGHTED,
    INFO_TITLE_GRADE,
    INFO_TITLE_TEAM,
    INFO_GRADE_PIVATE,
    INFO_GRADE_PIVATE_HILIGHTED,
    INFO_GRADE_SERENT,
    INFO_GRADE_SERENT_HILIGHTED,
    INFO_GRADE_FEACEL,
    INFO_GRADE_FEACEL_HILIGHTED,
    INFO_GRADE_LITENA,
    INFO_GRADE_LITENA_HILIGHTED,
    INFO_GRADE_KAINEL,
    INFO_GRADE_KAINEL_HILIGHTED,
    INFO_SLAYER_GRADE_BAR,
    INFO_SLAYER_GRADE_BAR_HILIGHTED,
    INFO_SLAYER_GRADE_BAR_PUSHED,
    INFO_SLAYER_GRADE_BAR_HILIGHTED_PUSHED,
    INFO_SLAYER_GRADE_MARK_EDGE,
    INFO_SLAYER_CHANGE_IMAGE,
    INFO_SLAYER_CHANGE_IMAGE_HILIGHTED,
    INFO_SLAYER_CHANGE_IMAGE_PUSHED,
    INFO_GRADE_GENEAL,
    INFO_GRADE_GENEAL_HILIGHTED,
    INFO_GRADE_FORE_GENEAL,
    INFO_GRADE_FORE_GENEAL_HILIGHTED,
    INFO_GRADE_MAJORIS_GENEAL,
    INFO_GRADE_MAJORIS_GENEAL_HILIGHTED,
    INFO_GRADE_CLOEL_GENEAL,
    INFO_GRADE_CLOEL_GENEAL_HILIGHTED,
    INFO_GRADE_MARSHAL,
    INFO_GRADE_MARSHAL_HILIGHTED
};

static int ui_otherinfo_load_pack(UI_OtherInfo* info);
static void ui_otherinfo_update_close_rect(UI_OtherInfo* info, int x, int y);
static Sprite* ui_otherinfo_sprite(UI_OtherInfo* info, int idx);
static int ui_otherinfo_sprite_w(UI_OtherInfo* info, int idx);
static int ui_otherinfo_sprite_h(UI_OtherInfo* info, int idx);
static void ui_otherinfo_blit(UI_OtherInfo* info, int idx, int x, int y);
static void ui_otherinfo_show_slayer(UI_Window* window, UI_OtherInfo* info);

UI_Window* ui_other_info_create(UI_GlobalResource* gr) {
    UI_OtherInfo* info = (UI_OtherInfo*)malloc(sizeof(UI_OtherInfo));
    if (!info) return NULL;
    memset(info, 0, sizeof(UI_OtherInfo));

    int w = 220;
    int h = 140;
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;

    ui_window_init(&info->base, x, y, w, h);
    info->base.user_data = info;
    info->base.show = ui_game_otherinfo_show;
    info->base.process = ui_game_otherinfo_process;
    info->base.mouse_control = ui_game_otherinfo_mouse_control;
    info->base.keyboard_control = ui_game_otherinfo_keyboard_control;
    info->gr = gr;
    info->visible = 0;
    memset(&info->player, 0, sizeof(info->player));
    info->info_loaded = 0;
    info->race = 0; /* Default Slayer */
    info->close_rect.x = x + w - 20;
    info->close_rect.y = y + 4;
    info->close_rect.w = 16;
    info->close_rect.h = 16;

    ui_otherinfo_load_pack(info);
    ui_otherinfo_update_close_rect(info, x, y);

    return &info->base;
}

void ui_other_info_start(UI_Window* window) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info) return;
    info->visible = 1;
    ui_window_set_visible(window, 1);
}

void ui_other_info_finish(UI_Window* window) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info) return;
    info->visible = 0;
    ui_window_set_visible(window, 0);
}

int ui_other_info_is_visible(UI_Window* window) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    return info ? info->visible : 0;
}

void ui_other_info_set_player(UI_Window* window, const UI_OtherInfoPlayer* player) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info || !player) return;
    memcpy(&info->player, player, sizeof(UI_OtherInfoPlayer));
}

void ui_other_info_set_race(UI_Window* window, int race) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info) return;
    if (race < 0) race = 0;
    if (race > 2) race = 2;
    if (info->race != race) {
        /* Free previous pack if loaded */
        if (info->info_loaded) {
            spritepack_free(&info->info_pack);
            memset(&info->info_pack, 0, sizeof(info->info_pack));
            info->info_loaded = 0;
        }
        info->race = race;
        ui_otherinfo_load_pack(info);
    }
}

static int ui_otherinfo_load_pack(UI_OtherInfo* info) {
    if (!info) return -1;
    if (info->info_loaded) return 0;

    const char* path = INFO_SPK_SLAYER_PATH;
    if (info->race == 1) path = INFO_SPK_VAMPIRE_PATH;
    else if (info->race == 2) path = INFO_SPK_OUSTERS_PATH;

    int rc = spritepack_load(&info->info_pack, path);
    if (rc == 0) {
        info->info_loaded = 1;
    } else {
        fprintf(stderr, "[ui_other_info] Failed to load %s (rc=%d)\n", path, rc);
    }
    return rc;
}

static Sprite* ui_otherinfo_sprite(UI_OtherInfo* info, int idx) {
    if (!info || !info->info_loaded) return NULL;
    return spritepack_get(&info->info_pack, (uint16_t)idx);
}

static int ui_otherinfo_sprite_w(UI_OtherInfo* info, int idx) {
    Sprite* s = ui_otherinfo_sprite(info, idx);
    return (s && s->is_valid) ? s->width : 0;
}

static int ui_otherinfo_sprite_h(UI_OtherInfo* info, int idx) {
    Sprite* s = ui_otherinfo_sprite(info, idx);
    return (s && s->is_valid) ? s->height : 0;
}

static void ui_otherinfo_blit(UI_OtherInfo* info, int idx, int x, int y) {
    if (!info || !info->gr) return;
    Sprite* s = ui_otherinfo_sprite(info, idx);
    if (!s || !s->is_valid) return;

    DecodedSprite decoded;
    memset(&decoded, 0, sizeof(decoded));
    if (sprite_decode(s, &decoded, 0) != 0) return;
    decoded_sprite_create_texture(&decoded, info->gr->renderer);
    ui_surface_blit_sprite(info->gr->surface, x, y, &decoded);
    decoded_sprite_free(&decoded);
}

static void ui_otherinfo_update_close_rect(UI_OtherInfo* info, int x, int y) {
    int w = ui_otherinfo_sprite_w(info, INFO_BUTTON_CLOSE);
    int h = ui_otherinfo_sprite_h(info, INFO_BUTTON_CLOSE);
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    info->close_rect.x = x + (info->base.w - w - 8);
    info->close_rect.y = y + 6;
    info->close_rect.w = w;
    info->close_rect.h = h;
}

static void ui_otherinfo_show_slayer(UI_Window* window, UI_OtherInfo* info) {
    const int field_x = 22;
    const int field_gap = 20;
    const int x_gap_from_image = 30;
    const int base_y = window->y;
    const int base_x = window->x;

    int bar_h = ui_otherinfo_sprite_h(info, INFO_LARGE_BAR);
    int bar_w = ui_otherinfo_sprite_w(info, INFO_LARGE_BAR);
    int small_h = ui_otherinfo_sprite_h(info, INFO_SMALL_BAR2);
    int small_w = ui_otherinfo_sprite_w(info, INFO_SMALL_BAR2);

    /* Close button */
    ui_otherinfo_blit(info, INFO_BUTTON_CLOSE, info->close_rect.x, info->close_rect.y);

    /* Face box */
    ui_otherinfo_blit(info, INFO_CHAR_BOX, base_x + field_x, base_y + 25);

    int field_y = 100;

    /* Name bar */
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + 85, base_y + field_y - field_gap - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + 85 + bar_w, base_y + field_y - field_gap - 3);

    /* Grade */
    ui_otherinfo_blit(info, INFO_TITLE_GRADE, base_x + field_x, base_y + field_y + 2);
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + field_x + x_gap_from_image + bar_w, base_y + field_y - 3);
    field_y += field_gap;

    /* Team */
    ui_otherinfo_blit(info, INFO_TITLE_TEAM, base_x + field_x + 6, base_y + field_y + 2);
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + field_x + x_gap_from_image + bar_w, base_y + field_y - 3);
    field_y += field_gap;

    /* Domain level */
    ui_otherinfo_blit(info, INFO_TITLE_DOMAINLEVEL, base_x + field_x - 2, base_y + field_y + 2);
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + field_x + x_gap_from_image + bar_w, base_y + field_y - 3);
    field_y += field_gap;

    /* Fame */
    ui_otherinfo_blit(info, INFO_TITLE_FAME, base_x + field_x, base_y + field_y + 2);
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + field_x + x_gap_from_image + bar_w, base_y + field_y - 3);
    field_y += field_gap;

    /* STR / DEX / INT */
    for (int i = 0; i < 3; i++) {
        int title_idx = INFO_TITLE_STR + i;
        ui_otherinfo_blit(info, title_idx, base_x + field_x, base_y + field_y + 2);
        if (small_h > 0 && small_w > 0) {
            ui_otherinfo_blit(info, INFO_SMALL_BAR2, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
        } else {
            ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + field_x + x_gap_from_image, base_y + field_y - 3);
        }
        ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + field_x + x_gap_from_image + bar_w, base_y + field_y - 3);
        field_y += field_gap;
    }

    /* Alignment bar */
    ui_otherinfo_blit(info, INFO_TITLE_ALIGN, base_x + 92 + x_gap_from_image + 3, base_y + field_y - field_gap + 2);
    ui_otherinfo_blit(info, INFO_LARGE_BAR, base_x + 92 + x_gap_from_image, base_y + field_y - field_gap - 3);
    ui_otherinfo_blit(info, INFO_LARGE_BAR_RIGHT, base_x + 92 + x_gap_from_image + bar_w, base_y + field_y - field_gap - 3);
}

static void ui_game_otherinfo_show(UI_Window* window) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info || !info->visible) return;

    if (!info->info_loaded) {
        ui_otherinfo_load_pack(info);
    }

    if (info->gr) {
        ui_global_resource_draw_dialog(info->gr, window->x, window->y, window->w, window->h, 0);
        ui_global_resource_draw_outbox(info->gr, window->x, window->y, window->w, window->h);
    } else {
        /* Fallback: filled rect if we have a surface */
        SDL_Rect rect = { window->x, window->y, window->w, window->h };
        UI_Surface* surface = info->gr ? info->gr->surface : NULL;
        if (surface) ui_surface_fill_rect(surface, &rect, 0x202020FF);
    }

    UI_Surface* surface = info->gr ? info->gr->surface : NULL;
    if (!surface) return;

    ui_otherinfo_update_close_rect(info, window->x, window->y);

    if (info->info_loaded) {
        switch (info->race) {
        case 0:
            ui_otherinfo_show_slayer(window, info);
            break;
        case 1:
        case 2:
            /* TODO: implement race-specific layouts. Use Slayer layout as fallback for now. */
            ui_otherinfo_show_slayer(window, info);
            break;
        }
    } else {
        /* Fallback: simple rectangles if pack failed to load. */
        SDL_Rect close_rect = info->close_rect;
        ui_surface_draw_rect(surface, &close_rect, 0xFFFFFFFF);

        const int left = window->x + 16;
        int top = window->y + 32;
        const int bar_w = window->w - 32;
        const int bar_h = 8;

        int values[5] = {
            info->player.level,
            info->player.fame,
            info->player.str_cur,
            info->player.dex_cur,
            info->player.int_cur
        };
        int maxes[5] = { 200, 1000, 300, 300, 300 };
        uint32_t colors[5] = { 0x3377FFFF, 0x33FF33FF, 0xFF3333FF, 0xFFFF33FF, 0x3333FFFF };

        for (int i = 0; i < 5; i++) {
            int value = values[i];
            int max = maxes[i];
            if (max <= 0) max = 1;
            if (value < 0) value = 0;
            if (value > max) value = max;
            int w = (bar_w * value) / max;
            SDL_Rect r = { left, top, w, bar_h };
            ui_surface_fill_rect(surface, &r, colors[i]);
            SDL_Rect outline = { left, top, bar_w, bar_h };
            ui_surface_draw_rect(surface, &outline, 0xFFFFFFFF);
            top += bar_h + 6;
        }
    }
}

static void ui_game_otherinfo_process(UI_Window* window) {
    (void)window;
}

static int ui_game_otherinfo_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    if (!info || !info->visible) return 0;

    switch (message) {
    case M_LEFTBUTTON_DOWN:
        if (x >= info->close_rect.x && x <= info->close_rect.x + info->close_rect.w &&
            y >= info->close_rect.y && y <= info->close_rect.y + info->close_rect.h) {
            ui_other_info_finish(window);
            return 1;
        }
        break;
    default:
        break;
    }
    return 0;
}

static void ui_game_otherinfo_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra) {
    UI_OtherInfo* info = (UI_OtherInfo*)window->user_data;
    (void)extra;
    if (!info || !info->visible) return;

    /* ESC closes */
    if (message == WM_KEYDOWN && key == SDLK_ESCAPE) {
        ui_other_info_finish(window);
    }
}
