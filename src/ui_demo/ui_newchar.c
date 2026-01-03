/**
 * @file ui_newchar.c
 * @brief New Character Window Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_newchar.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

#define UI_NC_ID_OK 1
#define UI_NC_ID_CANCEL 2

struct UI_NewChar {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;
    UI_NewCharCallback callback;
    void* callback_data;
    
    SpritePack* spk_char_create;
};

/* Forward declarations */
static void ui_nc_show(UI_Window* window);
static void ui_nc_process(UI_Window* window);
static int ui_nc_mouse_control(UI_Window* window, uint32_t message, int x, int y);

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
    struct UI_NewChar* nc = (struct UI_NewChar*)button->on_press.data;
    if (nc && nc->callback) {
        int event = 0;
        if (id == UI_NC_ID_OK) event = UI_NEWCHAR_EVENT_CREATE_OK;
        else if (id == UI_NC_ID_CANCEL) event = UI_NEWCHAR_EVENT_CANCEL;
        
        if (event) {
            nc->callback(&nc->base, event, nc->callback_data);
        }
    }
}

static void add_button(struct UI_NewChar* nc, int x, int y, int w, int h, int id) {
    UI_Button* btn = (UI_Button*)malloc(sizeof(UI_Button));
    if (!btn) return;
    ui_button_init(btn, x, y, w, h, id);
    ui_button_set_on_press(btn, internal_button_callback, nc);
    ui_button_group_add(&nc->buttons, btn);
}

UI_Window* ui_newchar_create(UI_GlobalResource* global_resource, UI_NewCharCallback callback, void* callback_data) {
    struct UI_NewChar* nc = (struct UI_NewChar*)malloc(sizeof(struct UI_NewChar));
    if (!nc) return NULL;
    
    nc->spk_char_create = (SpritePack*)malloc(sizeof(SpritePack));
    memset(nc->spk_char_create, 0, sizeof(SpritePack));
    if (spritepack_load(nc->spk_char_create, "DarkEden/Data/Ui/spk/CharCreate.spk") != 0) {
        printf("Failed to load CharCreate.spk\n");
    }
    
    int w = 800, h = 600;
    if (nc->spk_char_create && nc->spk_char_create->count > 0) {
         Sprite* s = spritepack_get(nc->spk_char_create, 0);
         if (s) { w = s->width; h = s->height; }
    }
    
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;

    ui_window_init(&nc->base, x, y, w, h);
    nc->base.user_data = nc;
    nc->base.show = ui_nc_show;
    nc->base.process = ui_nc_process;
    nc->base.mouse_control = ui_nc_mouse_control;
    nc->base.is_pixel = NULL;
    
    nc->gr = global_resource;
    nc->callback = callback;
    nc->callback_data = callback_data;
    
    ui_button_group_init(&nc->buttons, &nc->base);
    
    add_button(nc, 100, h - 60, 100, 30, UI_NC_ID_OK);
    add_button(nc, 220, h - 60, 100, 30, UI_NC_ID_CANCEL);
    
    return &nc->base;
}

static void ui_nc_show(UI_Window* window) {
    struct UI_NewChar* nc = (struct UI_NewChar*)window->user_data;
    
    if (nc->spk_char_create) {
        draw_sprite(nc->gr, nc->spk_char_create, 0, window->x, window->y);
    } else {
        SDL_Rect r = { window->x, window->y, window->w, window->h };
        ui_surface_fill_rect(nc->gr->surface, &r, 0x222222FF);
    }
    
    ui_button_group_render(&nc->buttons, nc->gr->surface, NULL);
}

static void ui_nc_process(UI_Window* window) {
    (void)window;
}

static int ui_nc_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_NewChar* nc = (struct UI_NewChar*)window->user_data;
    return ui_button_group_mouse_control(&nc->buttons, message, x - window->x, y - window->y);
}
