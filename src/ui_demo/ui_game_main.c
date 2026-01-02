/**
 * @file ui_game_main.c
 * @brief Main Game Interface Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_game_main.h"
#include "ui_game_quickslot.h"
#include "ui_game_gear.h"
#include "ui_game_chat.h"
#include "ui_game_minimap.h"
#include "ui_game_sysmenu.h"
#include "ui_window.h" // For WM_* constants
#include <SDL2/SDL.h> // For Keycodes
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

/* Structure to hold Game UI state and specific resources */
struct UI_GameMain {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_GameMainCallback callback;
    void* callback_data;

    /* Components */
    UI_QuickSlot* quickslot;
    UI_Window* gear;            /* Gear window */
    UI_GameChat* chat;          /* Chat component */
    UI_GameMinimap* minimap;    /* Minimap component */
    UI_Window* sysmenu_window;  /* System Menu as window, not component */

    /* Slayer UI Resources */
    SpritePack* spk_hp_bar;
    /* spk_chat moved to ui_game_chat */
    /* spk_minimap moved to ui_game_minimap */
    /* spk_sys_button moved to ui_game_sysmenu */
    SpritePack* spk_skill_etc;
};

/* Forward declarations */

static int ui_game_main_mouse_control(UI_Window* window, uint32_t message, int x, int y);
static void ui_game_main_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra);

/* System Menu callback */
static void on_sysmenu_event(UI_Window* window, int event, void* data) {
    struct UI_GameMain* game = (struct UI_GameMain*)data;
    (void)window;

    switch(event) {
        case UI_SYSMENU_EVENT_OPTION:
            printf("System Menu: Option clicked\n");
            /* TODO: Open options dialog */
            break;
        case UI_SYSMENU_EVENT_LOGOUT:
            printf("System Menu: Logout clicked\n");
            /* TODO: Confirm and logout */
            if (game->callback) {
                game->callback(game->callback_data, UI_GAME_MAIN_EVENT_EXIT);
            }
            break;
        case UI_SYSMENU_EVENT_CONTINUE:
            printf("System Menu: Continue (close menu)\n");
            ui_game_sysmenu_finish(game->sysmenu_window);
            break;
    }
}

/* Helper to load packs safely */
static SpritePack* load_pack(const char* path) {
    SpritePack* pack = (SpritePack*)malloc(sizeof(SpritePack));
    if (!pack) return NULL;
    memset(pack, 0, sizeof(SpritePack));
    if (spritepack_load(pack, path) != 0) {
        printf("Failed to load pack: %s\n", path);
        free(pack);
        return NULL;
    }
    return pack;
}

static void draw_sprite_at(UI_GlobalResource* gr, SpritePack* pack, int index, int x, int y) {
    if (!pack || !gr) return;
    Sprite* s = spritepack_get(pack, (uint16_t)index);
    if (!s) return;

    DecodedSprite decoded = {0};
    if (sprite_decode(s, &decoded, 0) == 0) {
        if (decoded_sprite_create_texture(&decoded, gr->renderer) == 0) {
            ui_surface_blit_sprite(gr->surface, x, y, &decoded);
        }
        decoded_sprite_free(&decoded);
    }
}

/* Main Render Function */
void ui_game_main_render(UI_Window* window) {
    if (!window) return;
    struct UI_GameMain* game = (struct UI_GameMain*)window->user_data;

    /* 1. HP Bar: Bottom Center or Left?
       Legacy Slayer Health bar is usually bottom left/center structure.
    */
    if (game->spk_hp_bar) {
        /* Index 0 is usually the background frame */
        draw_sprite_at(game->gr, game->spk_hp_bar, 0, 0, 600 - 150); /* Guestimate Y */
    }

    /* 2. Chat Box: Bottom Left */
    if (game->chat) {
        ui_game_chat_render(game->chat, 0, 600 - 150);
    }

    /* 3. Minimap: Top Right */
    if (game->minimap) {
        ui_game_minimap_render(game->minimap, 800 - 150, 0);
    }

    /* SysMenu is a separate window managed by window manager, not rendered here */

    /* 4. Quick Slot: Bottom Right */
    if (game->quickslot) {
        ui_game_quickslot_render(game->quickslot, 800 - (30 * 8) - 10, 600 - 40);
    }

    /* 5. Gear Window (Render on top) */
    if (game->gear) {
        /* Position: Right side aligned? */
        /* Legacy: Gear is roughly 520, ... */
        // ui_game_gear_render(game->gear, 800 - 250, 100);
    }
}

/* Main Process Function */
void ui_game_main_process(UI_Window* window) {
    if (!window) return;
    struct UI_GameMain* game = (struct UI_GameMain*)window->user_data;

    /* Process components */
    if (game->quickslot) ui_game_quickslot_process(game->quickslot);
    // if (game->gear) ui_game_gear_process(game->gear);
    // if (game->chat) ui_game_chat_process(game->chat);
    if (game->minimap) ui_game_minimap_process(game->minimap);
    /* sysmenu_window processes itself via window manager */
}

/* Wrap for UI_Window vtable if needed, but ui_game_update calls these directly now */
static void ui_game_main_show_wrapper(UI_Window* window) {
    ui_game_main_render(window);
}

static void ui_game_main_process_wrapper(UI_Window* window) {
    ui_game_main_process(window);
}

UI_Window* ui_game_main_create(UI_GlobalResource* global_resource, UI_GameMainCallback callback, void* callback_data) {
    struct UI_GameMain* game = (struct UI_GameMain*)malloc(sizeof(struct UI_GameMain));
    if (!game) return NULL;

    memset(game, 0, sizeof(struct UI_GameMain));

    /* Initialize Window Base */
    ui_window_init(&game->base, 0, 0, 800, 600);
    game->base.user_data = game;
    game->base.show = ui_game_main_show_wrapper;       /* Keep vtable for generic usages if any */
    game->base.process = ui_game_main_process_wrapper;
    game->base.mouse_control = ui_game_main_mouse_control;
    game->base.keyboard_control = ui_game_main_keyboard_control;
    game->base.is_pixel = NULL; // Full screen input intercept

    game->gr = global_resource;
    game->callback = callback;
    game->callback_data = callback_data;

    /* Initialize Components */
    game->quickslot = ui_game_quickslot_create(global_resource);
    game->gear = ui_game_gear_create(global_resource);
    game->chat = ui_game_chat_create(global_resource);
    game->minimap = ui_game_minimap_create(global_resource);
    game->sysmenu_window = ui_game_sysmenu_create(global_resource, on_sysmenu_event, game);

    /* Load Resources (Slayer Default) */
    game->spk_hp_bar = load_pack("DarkEden/Data/Ui/spk/HPBarSlayer.spk");
    /* game->spk_chat moved */
    /* game->spk_minimap moved */
    game->spk_skill_etc = load_pack("DarkEden/Data/Ui/spk/SkillEtcSlayer.spk");
    /* game->spk_sys_button moved */

    return &game->base;
}

static int ui_game_main_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    /* Handle clicks on Quit button etc. */
    return 0;
}

static void ui_game_main_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra) {
    struct UI_GameMain* game = (struct UI_GameMain*)window->user_data;
    (void)extra;

    if (message == WM_KEYDOWN) {
        if (key == SDLK_i || key == SDLK_TAB) {
            if (game->gear) {
                ui_game_gear_toggle(game->gear);
            }
        }
    }
}

UI_Window* ui_game_main_get_sysmenu(UI_Window* window) {
    struct UI_GameMain* game = (struct UI_GameMain*)window->user_data;
    return game ? game->sysmenu_window : NULL;
}

void ui_game_main_toggle_sysmenu(UI_Window* window) {
    struct UI_GameMain* game = (struct UI_GameMain*)window->user_data;
    if (!game || !game->sysmenu_window) return;

    if (ui_game_sysmenu_is_visible(game->sysmenu_window)) {
        ui_game_sysmenu_finish(game->sysmenu_window);
    } else {
        ui_game_sysmenu_start(game->sysmenu_window);
    }
}
