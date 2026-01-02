/**
 * @file ui_game_gear.c
 * @brief Gear Window Implementation
 */

#include "ui_game_gear.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct UI_Gear {
    UI_Window base;
    UI_GlobalResource* gr;
    SpritePack* spk_gear;
    SpritePack* spk_inventory;
    
    int visible;
};

/* Forward declarations */
static void ui_game_gear_show(UI_Window* window);
static void ui_game_gear_process(UI_Window* window);
static int ui_game_gear_mouse_control(UI_Window* window, uint32_t message, int x, int y);

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

UI_Window* ui_game_gear_create(UI_GlobalResource* global_resource) {
    struct UI_Gear* gear = (struct UI_Gear*)malloc(sizeof(struct UI_Gear));
    if (!gear) return NULL;
    
    memset(gear, 0, sizeof(struct UI_Gear));
    
    gear->spk_gear = load_pack("DarkEden/Data/Ui/spk/GearSlayer.spk");
    gear->spk_inventory = load_pack("DarkEden/Data/Ui/spk/InventorySlayer.spk");
    
    /* Determine window dimensions from gear sprite */
    int w = 200, h = 300; /* Fallback */
    if (gear->spk_gear && gear->spk_gear->count > 0) {
        Sprite* s = spritepack_get(gear->spk_gear, 0);
        if (s) { w = s->width; h = s->height; }
    }
    
    /* Position: Right side aligned? */
    int x = 800 - w - 20;
    int y = 50;
    
    ui_window_init(&gear->base, x, y, w, h);
    gear->base.user_data = gear;
    gear->base.show = ui_game_gear_show;
    gear->base.process = ui_game_gear_process;
    gear->base.mouse_control = ui_game_gear_mouse_control;
    
    gear->gr = global_resource;
    gear->visible = 0; /* Hidden by default */
    
    return &gear->base;
}

void ui_game_gear_destroy(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (gear) {
        if (gear->spk_gear) {
            spritepack_free(gear->spk_gear);
            free(gear->spk_gear);
        }
        if (gear->spk_inventory) {
            spritepack_free(gear->spk_inventory);
            free(gear->spk_inventory);
        }
        free(gear);
    }
}

void ui_game_gear_start(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (gear) {
        gear->visible = 1;
        ui_window_set_visible(window, 1);
    }
}

void ui_game_gear_finish(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (gear) {
        gear->visible = 0;
        ui_window_set_visible(window, 0);
    }
}

void ui_game_gear_toggle(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (gear) {
        if (gear->visible) ui_game_gear_finish(window);
        else ui_game_gear_start(window);
    }
}

int ui_game_gear_is_visible(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    return gear ? gear->visible : 0;
}

static void ui_game_gear_show(UI_Window* window) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (!gear || !gear->visible) return;

    /* 1. Draw Gear Window background */
    if (gear->spk_gear) {
        draw_sprite(gear->gr, gear->spk_gear, 0, window->x, window->y);
    }
    
    /* 2. Draw Inventory (attached for now) */
    if (gear->spk_inventory) {
        /* Inventory is usually to the left or right? 
           In Slayer legacy, usually Gear is on top and Inventory below, or side-by-side. 
           Let's draw it to the left of Gear for now.
        */
        int inv_w = 0;
        Sprite* inv_s = spritepack_get(gear->spk_inventory, 0);
        if (inv_s) inv_w = inv_s->width;
        
        draw_sprite(gear->gr, gear->spk_inventory, 0, window->x - inv_w, window->y);
    }
}

static void ui_game_gear_process(UI_Window* window) {
    (void)window;
}

static int ui_game_gear_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    struct UI_Gear* gear = (struct UI_Gear*)window->user_data;
    if (!gear || !gear->visible) return 0;
    
    /* Check if click is within Gear window or Inventory attached area */
    /* For now just simple bounds check on Gear window */
    if (x >= window->x && x < window->x + window->w &&
        y >= window->y && y < window->y + window->h) {
        /* Consume event */
        return 1;
    }
    
    return 0;
}
