/**
 * @file ui_char_manager.c
 * @brief Character Manager Window Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_char_manager.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

#define UI_CM_ID_CREATE 1
#define UI_CM_ID_BACK 2
#define UI_CM_ID_ENTER 3

struct UI_CharManager {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;
    UI_CharManagerCallback callback;
    void* callback_data;
    
    SpritePack* spk_char_manager;
};

/* Forward declarations */
static void ui_cm_show(UI_Window* window);
static void ui_cm_process(UI_Window* window);
static int ui_cm_mouse_control(UI_Window* window, uint32_t message, int x, int y);

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
    struct UI_CharManager* cm = (struct UI_CharManager*)button->user_data;
    if (cm && cm->callback) {
        int event = 0;
        if (id == UI_CM_ID_CREATE) event = UI_CHAR_MANAGER_EVENT_CREATE;
        else if (id == UI_CM_ID_BACK) event = UI_CHAR_MANAGER_EVENT_BACK;
        else if (id == UI_CM_ID_ENTER) event = UI_CHAR_MANAGER_EVENT_ENTER_GAME;
        
        if (event) {
            cm->callback(&cm->base, event, cm->callback_data);
        }
    }
}

static void add_button(struct UI_CharManager* cm, int x, int y, int w, int h, int id) {
    UI_Button* btn = (UI_Button*)malloc(sizeof(UI_Button));
    if (!btn) return;
    ui_button_init(btn, x, y, w, h, id);
    ui_button_set_callback(btn, internal_button_callback);
    ui_button_set_user_data(btn, cm);
    ui_button_group_add(&cm->buttons, btn);
}

UI_Window* ui_char_manager_create(UI_GlobalResource* global_resource, UI_CharManagerCallback callback, void* callback_data) {
    struct UI_CharManager* cm = (struct UI_CharManager*)malloc(sizeof(struct UI_CharManager));
    if (!cm) return NULL;
    
    cm->spk_char_manager = (SpritePack*)malloc(sizeof(SpritePack));
    memset(cm->spk_char_manager, 0, sizeof(SpritePack));
    if (spritepack_load(cm->spk_char_manager, "DarkEden/Data/Ui/spk/CharManager.spk") != 0) {
        printf("Failed to load CharManager.spk\n");
    }
    
    int w = 800, h = 600;
    if (cm->spk_char_manager && cm->spk_char_manager->count > 0) {
         Sprite* s = spritepack_get(cm->spk_char_manager, 0);
         if (s) { w = s->width; h = s->height; }
    }
    
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;

    ui_window_init(&cm->base, x, y, w, h);
    cm->base.user_data = cm;
    cm->base.show = ui_cm_show;
    cm->base.process = ui_cm_process;
    cm->base.mouse_control = ui_cm_mouse_control;
    cm->base.is_pixel = NULL;
    
    cm->gr = global_resource;
    cm->callback = callback;
    cm->callback_data = callback_data;
    
    ui_button_group_init(&cm->buttons, &cm->base);
    
    /* "Create New Char" button - random usage position */
    add_button(cm, w/2 - 75, h - 80, 150, 40, UI_CM_ID_CREATE);
    add_button(cm, w - 50, 10, 40, 30, UI_CM_ID_BACK);
    
    /* "Start Game" button */
    add_button(cm, w/2 - 75, h - 130, 150, 40, UI_CM_ID_ENTER);
    
    return &cm->base;
}

static void ui_cm_show(UI_Window* window) {
    struct UI_CharManager* cm = (struct UI_CharManager*)window->user_data;
    
    if (cm->spk_char_manager) {
        draw_sprite(cm->gr, cm->spk_char_manager, 0, window->x, window->y);
    } else {
        SDL_Rect r = { window->x, window->y, window->w, window->h };
        ui_surface_fill_rect(cm->gr->surface, &r, 0x550055FF);
    }
    
    ui_button_group_render(&cm->buttons, cm->gr->surface, NULL);
}

static void ui_cm_process(UI_Window* window) {
    (void)window;
}

static int ui_cm_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_CharManager* cm = (struct UI_CharManager*)window->user_data;
    return ui_button_group_mouse_control(&cm->buttons, message, x - window->x, y - window->y);
}
