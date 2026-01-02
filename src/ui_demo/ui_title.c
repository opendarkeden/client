/**
 * @file ui_title.c
 * @brief Title Screen Implementation with Flow Control
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui_title.h"
#include "ui_button_group.h"
#include "ui_button.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"

#include "ui_login.h"
#include "ui_server_select.h"
#include "ui_char_manager.h"
#include "ui_newchar.h"

/* Flow State */
typedef enum {
    TITLE_STATE_MAIN,
    TITLE_STATE_LOGIN,
    TITLE_STATE_SERVER_SELECT,
    TITLE_STATE_CHAR_MANAGER,
    TITLE_STATE_NEW_CHAR
} TitleState;

/* Structure definition */
struct UI_Title {
    UI_Window base;
    
    UI_GlobalResource* gr;
    UI_ButtonGroup buttons;     /* Main Title Buttons */
    
    /* Sub-windows */
    UI_Window* win_login;
    UI_Window* win_server_select;
    UI_Window* win_char_manager;
    UI_Window* win_newchar;
    
    TitleState state;

    /* Sprite Packs for Main Title */
    SpritePack* spk_title;
    SpritePack* spk_menu_default;
    SpritePack* spk_menu_select;
};

/* Forward declarations */
static void ui_title_show(UI_Window* window);
static void ui_title_process(UI_Window* window);
static int ui_title_mouse_control(UI_Window* window, uint32_t message, int x, int y);

/* Button Constants */
#define TITLE_BTN_X 673
#define TITLE_BTN_Y_START 371
#define TITLE_BTN_Y_GAP 48

/* Callback Implementations */
static void on_login_event(UI_Window* w, int event, void* data) {
    struct UI_Title* title = (struct UI_Title*)data;
    if (event == UI_LOGIN_EVENT_OK) {
        printf("Login Successful. Going to Server Select.\n");
        title->state = TITLE_STATE_SERVER_SELECT;
    } else if (event == UI_LOGIN_EVENT_CANCEL) {
        printf("Login Cancelled.\n");
        title->state = TITLE_STATE_MAIN;
    }
}

static void on_server_select_event(UI_Window* w, int event, void* data) {
    struct UI_Title* title = (struct UI_Title*)data;
    if (event == UI_SERVER_SELECT_EVENT_CONNECT) {
        printf("Server Selected. Going to Char Manager.\n");
        title->state = TITLE_STATE_CHAR_MANAGER;
    } else if (event == UI_SERVER_SELECT_EVENT_BACK) {
        title->state = TITLE_STATE_LOGIN;
    }
}

static void on_char_manager_event(UI_Window* w, int event, void* data) {
    struct UI_Title* title = (struct UI_Title*)data;
    if (event == UI_CHAR_MANAGER_EVENT_CREATE) {
        printf("Create New Character Request.\n");
        title->state = TITLE_STATE_NEW_CHAR;
    } else if (event == UI_CHAR_MANAGER_EVENT_BACK) {
        title->state = TITLE_STATE_SERVER_SELECT;
    }
}

static void on_newchar_event(UI_Window* w, int event, void* data) {
    struct UI_Title* title = (struct UI_Title*)data;
    if (event == UI_NEWCHAR_EVENT_CREATE_OK) {
        printf("Character Created. Back to Manager.\n");
        title->state = TITLE_STATE_CHAR_MANAGER;
    } else if (event == UI_NEWCHAR_EVENT_CANCEL) {
        title->state = TITLE_STATE_CHAR_MANAGER;
    }
}


/* Helper to draw a sprite from pack at position */
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

static void on_title_button_click(UI_Button* button, int button_id) {
    struct UI_Title* title = (struct UI_Title*)button->user_data;
    /* This assumes user_data is set to title. We need to ensure that in add_title_button */
    
    switch (button_id) {
        case UI_TITLE_ID_CONNECT:
            printf("Action: Connect -> Login Screen\n");
            title->state = TITLE_STATE_LOGIN;
            break;
        case UI_TITLE_ID_EXIT:
            printf("Action: Exit Application\n");
            SDL_Event quit_event;
            quit_event.type = SDL_QUIT;
            SDL_PushEvent(&quit_event);
            break;
        default:
            printf("Action: %d (Not Implemented)\n", button_id);
            break;
    }
}

static void add_title_button(struct UI_Title* title, int x, int y, int w, int h, int id) {
    UI_Button* btn = (UI_Button*)malloc(sizeof(UI_Button));
    if (!btn) return;
    
    ui_button_init(btn, x, y, w, h, id);
    ui_button_set_callback(btn, on_title_button_click);
    ui_button_set_user_data(btn, title); /* Important for callback context */
    ui_button_group_add(&title->buttons, btn);
}

UI_Window* ui_title_create(UI_GlobalResource* global_resource) {
    struct UI_Title* title = (struct UI_Title*)malloc(sizeof(struct UI_Title));
    if (!title) return NULL;
    
    ui_window_init(&title->base, 0, 0, 800, 600);
    title->base.user_data = title;
    title->base.show = ui_title_show;
    title->base.process = ui_title_process;
    title->base.mouse_control = ui_title_mouse_control;
    title->base.is_pixel = NULL;
    
    title->gr = global_resource;
    title->state = TITLE_STATE_MAIN;
    
    /* Load Main Title Assets */
    title->spk_title = (SpritePack*)malloc(sizeof(SpritePack));
    memset(title->spk_title, 0, sizeof(SpritePack));
    spritepack_load(title->spk_title, "DarkEden/Data/Ui/spk/Title.spk");

    title->spk_menu_default = (SpritePack*)malloc(sizeof(SpritePack));
    memset(title->spk_menu_default, 0, sizeof(SpritePack));
    spritepack_load(title->spk_menu_default, "DarkEden/Data/Ui/spk/TitleMenuDefault.spk");

 //   title->spk_menu_select = (SpritePack*)malloc(sizeof(SpritePack));
 //   memset(title->spk_menu_select, 0, sizeof(SpritePack));
 //   spritepack_load(title->spk_menu_select, "DarkEden/Data/Ui/spk/TitleMenuSelect.spk");

    /* Init Buttons */
    ui_button_group_init(&title->buttons, &title->base);
    int w = 100, h = 30;
    if (title->spk_menu_default && title->spk_menu_default->count > 0) {
        Sprite* s = spritepack_get(title->spk_menu_default, 0);
        if (s) { w = s->width; h = s->height; }
    }
    add_title_button(title, TITLE_BTN_X, TITLE_BTN_Y_START + (TITLE_BTN_Y_GAP * 0), w, h, UI_TITLE_ID_CONNECT);
    add_title_button(title, TITLE_BTN_X, TITLE_BTN_Y_START + (TITLE_BTN_Y_GAP * 1), w, h, UI_TITLE_ID_OPTION);
    add_title_button(title, TITLE_BTN_X, TITLE_BTN_Y_START + (TITLE_BTN_Y_GAP * 2), w, h, UI_TITLE_ID_CREDIT);
    add_title_button(title, TITLE_BTN_X, TITLE_BTN_Y_START + (TITLE_BTN_Y_GAP * 3), w, h, UI_TITLE_ID_EXIT);
    
    /* Init Sub-Windows */
    title->win_login = ui_login_create(global_resource, on_login_event, title);
    title->win_server_select = ui_server_select_create(global_resource, on_server_select_event, title);
    title->win_char_manager = ui_char_manager_create(global_resource, on_char_manager_event, title);
    title->win_newchar = ui_newchar_create(global_resource, on_newchar_event, title);
    
    return &title->base;
}

static void ui_title_show(UI_Window* window) {
    struct UI_Title* title = (struct UI_Title*)window->user_data;
    
    /* Always draw Title Background for now (unless fully covered) */
    if (title->spk_title) {
        draw_sprite(title->gr, title->spk_title, 0, window->x, window->y);
    }
    
    switch (title->state) {
        case TITLE_STATE_MAIN:
            /* Draw Title Buttons */
            {
                int count = title->buttons.count;
                for (int i = 0; i < count; i++) {
                    UI_Button* btn = title->buttons.buttons[i];
                    if (!btn) continue;
                    
                    int frame_idx = i * 2;
                    SpritePack* pack_to_use = title->spk_menu_default;
                    if (ui_button_get_focus(btn) || ui_button_get_pressed(btn)) {
                            frame_idx++;
                    }
                    if (pack_to_use) {
                         draw_sprite(title->gr, pack_to_use, frame_idx, window->x + btn->x, window->y + btn->y);
                    }
                }
            }
            break;
            
        case TITLE_STATE_LOGIN:
            if (title->win_login && title->win_login->show) 
                title->win_login->show(title->win_login);
            break;
            
        case TITLE_STATE_SERVER_SELECT:
            if (title->win_server_select && title->win_server_select->show)
                title->win_server_select->show(title->win_server_select);
            break;
            
        case TITLE_STATE_CHAR_MANAGER:
             if (title->win_char_manager && title->win_char_manager->show)
                title->win_char_manager->show(title->win_char_manager);
            break;
            
        case TITLE_STATE_NEW_CHAR:
             if (title->win_newchar && title->win_newchar->show)
                title->win_newchar->show(title->win_newchar);
            break;
    }
}

static void ui_title_process(UI_Window* window) {
    (void)window;
}

static int ui_title_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_Title* title = (struct UI_Title*)window->user_data;
    
    switch (title->state) {
        case TITLE_STATE_MAIN:
            return ui_button_group_mouse_control(&title->buttons, message, x - window->x, y - window->y);
            
        case TITLE_STATE_LOGIN:
            if (title->win_login && title->win_login->mouse_control)
                return title->win_login->mouse_control(title->win_login, message, x, y);
            break;
            
        case TITLE_STATE_SERVER_SELECT:
            if (title->win_server_select && title->win_server_select->mouse_control)
                return title->win_server_select->mouse_control(title->win_server_select, message, x, y);
             break;
             
        case TITLE_STATE_CHAR_MANAGER:
            if (title->win_char_manager && title->win_char_manager->mouse_control)
                return title->win_char_manager->mouse_control(title->win_char_manager, message, x, y);
             break;

        case TITLE_STATE_NEW_CHAR:
            if (title->win_newchar && title->win_newchar->mouse_control)
                return title->win_newchar->mouse_control(title->win_newchar, message, x, y);
             break;
    }
    return 0;
}
