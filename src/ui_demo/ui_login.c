/**
 * @file ui_login.c
 * @brief Login Window Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_login.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

/* Button constants */
#define UI_LOGIN_ID_OK 1
#define UI_LOGIN_ID_CANCEL 2
#define UI_LOGIN_ID_NEW 3

/* Coordinates for Login Menu Buttons (Approximate based on observation, need to be precise?) 
   VS_UI_title.cpp: 
   Set(400 - m_pC_login_spk->GetWidth()/2, 300 - m_pC_login_spk->GetHeight()/2-57, ...)
   Add(new C_VS_UI_EVENT_BUTTON(118, 123, ... NEW_ID ...))
   Add(new C_VS_UI_EVENT_BUTTON(52, 123, ... OK ...))
   Add(new C_VS_UI_EVENT_BUTTON(156, 28, ... CANCEL ...))
*/

struct UI_Login {
    UI_Window base;
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;
    UI_LoginCallback callback;
    void* callback_data;
    
    SpritePack* spk_login;
    SpritePack* spk_login_menu;
    
    /* Input fields would be here */
};

/* Forward declarations */
static void ui_login_show(UI_Window* window);
static void ui_login_process(UI_Window* window);
static int ui_login_mouse_control(UI_Window* window, uint32_t message, int x, int y);
static void ui_login_destroy(UI_Window* window);

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

static void on_button_click(UI_Button* button, int button_id) {
    (void)button;
    /* Find parent window */
    /* UI_Login* login is accessible via button->user_data if we set it, or button group parent */
}

/* We need a way to pass callback up. 
   Simplest way: store UI_Login pointer in button's user_data */
static void internal_button_callback(UI_Button* button, int id) {
    struct UI_Login* login = (struct UI_Login*)(button->on_press.data);
    if (login && login->callback) {
        /* Map internal ID to public event */
        int event = 0;
        if (id == UI_LOGIN_ID_OK) event = UI_LOGIN_EVENT_OK;
        else if (id == UI_LOGIN_ID_CANCEL) event = UI_LOGIN_EVENT_CANCEL;
        else if (id == UI_LOGIN_ID_NEW) event = UI_LOGIN_EVENT_NEW;
        
        if (event) {
            login->callback(&login->base, event, login->callback_data);
        }
    }
}

static void add_button(struct UI_Login* login, int x, int y, int w, int h, int id) {
    UI_Button* btn = (UI_Button*)malloc(sizeof(UI_Button));
    if (!btn) return;
    ui_button_init(btn, x, y, w, h, id);
    ui_button_set_on_press(btn, internal_button_callback, login);    
    ui_button_group_add(&login->buttons, btn);
}

static int ui_login_is_pixel(UI_Window* window, int x, int y) {
    /* Optional: precise hit detection on background sprite */
    return 1;
}

UI_Window* ui_login_create(UI_GlobalResource* global_resource, UI_LoginCallback callback, void* callback_data) {
    struct UI_Login* login = (struct UI_Login*)malloc(sizeof(struct UI_Login));
    if (!login) return NULL;
    
    /* Load sprites first to get dimensions */
    login->spk_login = (SpritePack*)malloc(sizeof(SpritePack));
    memset(login->spk_login, 0, sizeof(SpritePack));
    if (spritepack_load(login->spk_login, "DarkEden/Data/Ui/spk/Login.spk") != 0) {
        printf("Failed to load Login.spk\n");
    }
    
    login->spk_login_menu = (SpritePack*)malloc(sizeof(SpritePack));
    memset(login->spk_login_menu, 0, sizeof(SpritePack));
    if (spritepack_load(login->spk_login_menu, "DarkEden/Data/Ui/spk/LoginMenu.spk") != 0) {
        printf("Failed to load LoginMenu.spk\n");
    }
    
    int w = 400, h = 300; /* Fallback */
    if (login->spk_login && login->spk_login->count > 0) {
         Sprite* s = spritepack_get(login->spk_login, 0);
         if (s) { w = s->width; h = s->height; }
    }
    
    /* Center on screen (800x600 context assumed) */
    int x = (800 - w) / 2;
    int y = (600 - h) / 2;

    ui_window_init(&login->base, x, y, w, h);
    login->base.user_data = login;
    login->base.show = ui_login_show;
    login->base.process = ui_login_process;
    login->base.mouse_control = ui_login_mouse_control;
    login->base.is_pixel = ui_login_is_pixel;
    
    login->gr = global_resource;
    login->callback = callback;
    login->callback_data = callback_data;
    
    ui_button_group_init(&login->buttons, &login->base);
    
    /* Add Buttons - sizes based on LoginMenu.spk indices? 
       Assuming indices: 0=NEW, 1=OK, 2=CANCEL (Need verification, using placeholders) 
       Legacy code:
       NEW_ID: GetWidth(NEW_ID) -> LoginMenu
       OK: GetWidth(OK)
       CANCEL: GetWidth(CANCEL)
       
       We'll approximate sizes for now or try to read from spk if possible.
       Let's assume standard button sizes 50x20 if spk fails.
    */
    int btn_w = 50, btn_h = 20;
    
    /* Coordinates from legacy: 118,123 (New), 52,123 (Ok), 156,28 (Cancel) */
    add_button(login, 52, 123, btn_w, btn_h, UI_LOGIN_ID_OK);
    add_button(login, 118, 123, btn_w, btn_h, UI_LOGIN_ID_NEW);
    add_button(login, 156, 28, btn_w, btn_h, UI_LOGIN_ID_CANCEL);
    
    return &login->base;
}

static void ui_login_show(UI_Window* window) {
    struct UI_Login* login = (struct UI_Login*)window->user_data;
    
    /* Draw Background */
    if (login->spk_login) {
        draw_sprite(login->gr, login->spk_login, 0, window->x, window->y);
    } else {
        /* Fallback box */
        SDL_Rect r = { window->x, window->y, window->w, window->h };
        ui_surface_fill_rect(login->gr->surface, &r, 0x0000AAFF);
    }
    
    /* Draw Buttons */
    /* If proper sprites are mapped, we use them. For now, simple fallback or mapped if spk exists */
    ui_button_group_render(&login->buttons, login->gr->surface, login->spk_login_menu);
}

static void ui_login_process(UI_Window* window) {
    (void)window;
}

static int ui_login_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_Login* login = (struct UI_Login*)window->user_data;
    return ui_button_group_mouse_control(&login->buttons, message, x - window->x, y - window->y);
}
