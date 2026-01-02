/**
 * @file ui_server_select.c
 * @brief Server Select Window Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_server_select.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

#define UI_SS_ID_CONNECT 1
#define UI_SS_ID_BACK 2

struct UI_ServerSelect {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;
    UI_ServerSelectCallback callback;
    void* callback_data;
    
    SpritePack* spk_server_select;
};

/* Forward declarations */
static void ui_ss_show(UI_Window* window);
static void ui_ss_process(UI_Window* window);
static int ui_ss_mouse_control(UI_Window* window, uint32_t message, int x, int y);

static void draw_sprite(UI_GlobalResource* gr, SpritePack* pack, int index, int x, int y) {
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

static void internal_button_callback(UI_Button* button, int id) {
    struct UI_ServerSelect* ss = (struct UI_ServerSelect*)button->user_data;
    if (ss && ss->callback) {
        int event = 0;
        if (id == UI_SS_ID_CONNECT) event = UI_SERVER_SELECT_EVENT_CONNECT;
        else if (id == UI_SS_ID_BACK) event = UI_SERVER_SELECT_EVENT_BACK;
        
        if (event) {
            ss->callback(&ss->base, event, ss->callback_data);
        }
    }
}

static void add_button(struct UI_ServerSelect* ss, int x, int y, int w, int h, int id) {
    UI_Button* btn = (UI_Button*)malloc(sizeof(UI_Button));
    if (!btn) return;
    ui_button_init(btn, x, y, w, h, id);
    ui_button_set_callback(btn, internal_button_callback);
    ui_button_set_user_data(btn, ss);
    ui_button_group_add(&ss->buttons, btn);
}

UI_Window* ui_server_select_create(UI_GlobalResource* global_resource, UI_ServerSelectCallback callback, void* callback_data) {
    struct UI_ServerSelect* ss = (struct UI_ServerSelect*)malloc(sizeof(struct UI_ServerSelect));
    if (!ss) return NULL;
    
    /* Load sprites */
    ss->spk_server_select = (SpritePack*)malloc(sizeof(SpritePack));
    memset(ss->spk_server_select, 0, sizeof(SpritePack));
    if (spritepack_load(ss->spk_server_select, "DarkEden/Data/Ui/spk/ServerSelect.spk") != 0) {
        printf("Failed to load ServerSelect.spk\n");
    }
    
    int w = 800, h = 600; /* Default to full screen or center */
    if (ss->spk_server_select && ss->spk_server_select->count > 0) {
         Sprite* s = spritepack_get(ss->spk_server_select, 0);
         if (s) { w = s->width; h = s->height; }
    }
    
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;

    ui_window_init(&ss->base, x, y, w, h);
    ss->base.user_data = ss;
    ss->base.show = ui_ss_show;
    ss->base.process = ui_ss_process;
    ss->base.mouse_control = ui_ss_mouse_control;
    ss->base.is_pixel = NULL;
    
    ss->gr = global_resource;
    ss->callback = callback;
    ss->callback_data = callback_data;
    
    ui_button_group_init(&ss->buttons, &ss->base);
    
    /* Mockup/Placeholder buttons positions */
    add_button(ss, w/2 - 50, h - 50, 100, 30, UI_SS_ID_CONNECT);
    /* Maybe a back button somewhere top right? */
    add_button(ss, w - 40, 10, 30, 30, UI_SS_ID_BACK);
    
    return &ss->base;
}

static void ui_ss_show(UI_Window* window) {
    struct UI_ServerSelect* ss = (struct UI_ServerSelect*)window->user_data;
    
    if (ss->spk_server_select) {
        draw_sprite(ss->gr, ss->spk_server_select, 0, window->x, window->y);
    } else {
        SDL_Rect r = { window->x, window->y, window->w, window->h };
        ui_surface_fill_rect(ss->gr->surface, &r, 0x00AA00FF);
    }
    
    ui_button_group_render(&ss->buttons, ss->gr->surface, NULL);
    
    /* Draw placeholder text or list? */
}

static void ui_ss_process(UI_Window* window) {
    (void)window;
}

static int ui_ss_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_ServerSelect* ss = (struct UI_ServerSelect*)window->user_data;
    return ui_button_group_mouse_control(&ss->buttons, message, x - window->x, y - window->y);
}
