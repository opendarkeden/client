/**
 * @file ui_game_sysmenu.c
 * @brief Game System Menu Window Implementation (Interactive)
 */

#include "ui_game_sysmenu.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Button IDs */
#define BTN_ID_OPTION   1
#define BTN_ID_LOGOUT   2
#define BTN_ID_CONTINUE 3

struct UI_GameSysMenu {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;
    UI_SysMenuCallback callback;
    void* callback_data;
    
    SpritePack* spk_sysmenu;
    int visible;
};

/* Forward declarations */
static void ui_game_sysmenu_show(UI_Window* window);
static void ui_game_sysmenu_process(UI_Window* window);
static int ui_game_sysmenu_mouse_control(UI_Window* window, uint32_t message, int x, int y);
static void ui_game_sysmenu_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra);

/* Helper to draw sprite */
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

/* Button callback */
static void on_button_click(UI_Button* button, int button_id) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)button->on_press.data;
    if (menu && menu->callback) {
        int event = 0;
        if (button_id == BTN_ID_OPTION) event = UI_SYSMENU_EVENT_OPTION;
        else if (button_id == BTN_ID_LOGOUT) event = UI_SYSMENU_EVENT_LOGOUT;
        else if (button_id == BTN_ID_CONTINUE) event = UI_SYSMENU_EVENT_CONTINUE;
        
        if (event) {
            menu->callback(&menu->base, event, menu->callback_data);
        }
    }
}

typedef struct {
    UI_Button base;
    int16_t sprite_index;
} UI_GameSysMenuButton;

/* Custom button render - mimics legacy ShowButtonWidget */
static void render_sysmenu_button(void* data, UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y) {
    UI_GameSysMenuButton* button = (UI_GameSysMenuButton*)data;
    if (!button || !pack) return;
    
    /* Only render sprite when button has focus (hover) */
    if (button->base.focus) {
        Sprite* s = spritepack_get(pack, (uint16_t)button->sprite_index);
        if (s && s->is_valid) {
            DecodedSprite decoded = {0};
            if (sprite_decode(s, &decoded, 0) == 0) {
                if (decoded_sprite_create_texture(&decoded, surface->renderer) == 0) {
                    int abs_x = parent_x + button->base.x;
                    int abs_y = parent_y + button->base.y;
                    ui_surface_blit_sprite(surface, abs_x, abs_y, &decoded);
                }
                decoded_sprite_free(&decoded);
            }
        }
    }
}

static void add_button(struct UI_GameSysMenu* menu, int x, int y, int w, int h, int id) {
    UI_GameSysMenuButton* btn = (UI_GameSysMenuButton*)malloc(sizeof(UI_GameSysMenuButton));
    if (!btn) return;
    ui_button_init(&btn->base, x, y, w, h, id);
    ui_button_set_on_press(&btn->base, on_button_click, menu);
    btn->sprite_index = id;
    ui_button_set_show_widget(&btn->base, render_sysmenu_button, btn);
    
    ui_button_group_add(&menu->buttons, btn);
}

UI_Window* ui_game_sysmenu_create(UI_GlobalResource* global_resource, UI_SysMenuCallback callback, void* callback_data) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)malloc(sizeof(struct UI_GameSysMenu));
    if (!menu) return NULL;
    
    memset(menu, 0, sizeof(struct UI_GameSysMenu));
    
    /* Load sprite pack */
    menu->spk_sysmenu = (SpritePack*)malloc(sizeof(SpritePack));
    memset(menu->spk_sysmenu, 0, sizeof(SpritePack));
    if (spritepack_load(menu->spk_sysmenu, "DarkEden/Data/Ui/spk/GameMenuSlayer.spk") != 0) {
        printf("Failed to load GameMenuSlayer.spk\n");
    }
    
    /* Get window dimensions from sprite */
    int w = 200, h = 150; /* Fallback */
    if (menu->spk_sysmenu && menu->spk_sysmenu->count > 0) {
        Sprite* s = spritepack_get(menu->spk_sysmenu, 0);
        if (s) { w = s->width; h = s->height; }
    }
    
    /* Center on screen (800x600) */
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;
    
    ui_window_init(&menu->base, x, y, w, h);
    menu->base.user_data = menu;
    menu->base.show = ui_game_sysmenu_show;
    menu->base.process = ui_game_sysmenu_process;
    menu->base.mouse_control = ui_game_sysmenu_mouse_control;
    menu->base.keyboard_control = ui_game_sysmenu_keyboard_control;
    menu->base.attr.topmost = 1; /* Always on top */
    
    menu->gr = global_resource;
    menu->callback = callback;
    menu->callback_data = callback_data;
    menu->visible = 0; /* Hidden by default */
    
    ui_button_group_init(&menu->buttons, &menu->base);
    
    /* Add buttons (positions from legacy code for Slayer)
       Based on C_VS_UI_GAMEMENU constructor:
       - OPTION: y2_offset
       - LOGOUT: y3_offset
       - CONTINUE: y4_offset
       Using estimated positions (need sprite info for exact placement)
    */
    int x_offset = 30;  /* Approximate button x offset */
    int y_offset = 10;
    int btn_w = 60, btn_h = 20;
    
    add_button(menu, x_offset, y_offset+52, btn_w, btn_h, BTN_ID_OPTION);
    add_button(menu, x_offset, y_offset+80, btn_w, btn_h, BTN_ID_LOGOUT);
    add_button(menu, x_offset, y_offset+100, btn_w, btn_h, BTN_ID_CONTINUE);
    
    return &menu->base;
}

void ui_game_sysmenu_start(UI_Window* window) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    if (!menu) return;
    
    menu->visible = 1;
    ui_window_set_visible(window, 1);
    printf("System Menu: Opened\n");
}

void ui_game_sysmenu_finish(UI_Window* window) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    if (!menu) return;
    
    menu->visible = 0;
    ui_window_set_visible(window, 0);
    printf("System Menu: Closed\n");
}

int ui_game_sysmenu_is_visible(UI_Window* window) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    return menu ? menu->visible : 0;
}

static void ui_game_sysmenu_show(UI_Window* window) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    
    if (!menu->visible) return;
    
    /* Draw background window */
    if (menu->spk_sysmenu) {
        draw_sprite(menu->gr, menu->spk_sysmenu, 0, window->x, window->y);
    }
    
    /* Draw buttons */
    ui_button_group_render(&menu->buttons, menu->gr->surface, menu->spk_sysmenu);
}

static void ui_game_sysmenu_process(UI_Window* window) {
    (void)window;
}

static int ui_game_sysmenu_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    
    if (!menu->visible) return 0;
    
    return ui_button_group_mouse_control(&menu->buttons, message, x - window->x, y - window->y);
}

static void ui_game_sysmenu_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra) {
    struct UI_GameSysMenu* menu = (struct UI_GameSysMenu*)window->user_data;
    (void)extra;
    
    if (!menu->visible) return;
    
    /* ESC key closes the menu */
    if (message == WM_KEYDOWN && key == SDLK_ESCAPE) {
        ui_game_sysmenu_finish(window);
    }
}
