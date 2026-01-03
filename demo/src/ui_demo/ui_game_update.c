/**
 * @file ui_game_update.c
 * @brief Game Mode Update Loop Implementation
 */

#include "ui_game_update.h"
#include "ui_game_main.h"
#include "ui_game_sysmenu.h"
#include "ui_other_info.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

struct UI_GameUpdate {
    UI_GlobalResource* gr;
    UI_GameUpdateCallback callback;
    void* callback_data;

    /* The Main Game HUD */
    UI_Window* hud_window;
};

static void on_hud_event(void* data, int event_type) {
    UI_GameUpdate* update = (UI_GameUpdate*)data;
    if (update && update->callback) {
        // Forward HUD events (like Exit) to the Game Update callback
        update->callback(update->callback_data, event_type);
    }
}

UI_GameUpdate* ui_game_update_create(UI_GlobalResource* global_resource, UI_GameUpdateCallback callback, void* callback_data) {
    UI_GameUpdate* update = (UI_GameUpdate*)malloc(sizeof(UI_GameUpdate));
    if (!update) return NULL;

    update->gr = global_resource;
    update->callback = callback;
    update->callback_data = callback_data;

    /* Create the HUD Window */
    update->hud_window = ui_game_main_create(global_resource, on_hud_event, update);

    return update;
}

void ui_game_update_destroy(UI_GameUpdate* update) {
    if (!update) return;
    
    // In a full implementation, we would destroy hud_window here if it's not managed by a parent window container.
    // For now, assuming ui_game_main_create returns a window that we are responsible for.
    // However, the current ui_window system doesn't have a standard destroy? 
    // We'll leave it for now or assume standard free if needed, but UI_Window is usually just a struct pointer if malloc'd.
    // TODO: Add proper destroy for sub-windows
    
    free(update);
}

void ui_game_update_process(UI_GameUpdate* update) {
    if (!update) return;

    /* 1. Update Game World Logic (Placeholders) */
    // e.g., UpdateCreature(), UpdateEffect(), etc.

    /* 2. Update HUD Logic */
    if (update->hud_window && update->hud_window->process) {
        update->hud_window->process(update->hud_window);
    }
}

void ui_game_update_render(UI_GameUpdate* update) {
    if (!update) return;

    /* 1. Render Game World */
    // Placeholder: Clear background to black or draw a placeholder world
    // Since we are in the main render loop, the screen is likely already cleared or we can clear it.
    // For "Game Mode", we might want a specific background color representing the world.
    
    // Draw a dark gray rect to represent the game world viewport
    SDL_SetRenderDrawColor(update->gr->renderer, 20, 20, 20, 255);
    SDL_Rect world_rect = {0, 0, 800, 600};
    SDL_RenderFillRect(update->gr->renderer, &world_rect);

    /* 2. Render HUD Overlay */
    if (update->hud_window && update->hud_window->show) {
        update->hud_window->show(update->hud_window);
    }
    
    /* 3. Render System Menu (if visible) */
    UI_Window* sysmenu = ui_game_main_get_sysmenu(update->hud_window);
    if (sysmenu && ui_game_sysmenu_is_visible(sysmenu) && sysmenu->show) {
        sysmenu->show(sysmenu);
    }

    UI_Window* otherinfo = ui_game_main_get_other_info(update->hud_window);
    if (otherinfo && ui_other_info_is_visible(otherinfo) && otherinfo->show) {
        otherinfo->show(otherinfo);
    }
}

int ui_game_update_mouse_control(UI_GameUpdate* update, uint32_t message, int x, int y) {
    if (!update) return 0;

    /* Check System Menu first (topmost) */
    UI_Window* sysmenu = ui_game_main_get_sysmenu(update->hud_window);
    if (sysmenu && ui_game_sysmenu_is_visible(sysmenu) && sysmenu->mouse_control) {
        int handled = sysmenu->mouse_control(sysmenu, message, x, y);
        if (handled) return 1;
    }

    /* Pass input to HUD */
    if (update->hud_window && update->hud_window->mouse_control) {
        int handled = update->hud_window->mouse_control(update->hud_window, message, x, y);
        if (handled) return 1;
    }

    /* Pass input to Game World */
    
    return 0;
}

void ui_game_update_keyboard_control(UI_GameUpdate* update, uint32_t message, uint32_t key, long extra) {
    if (!update) return;

    /* ESC key toggles System Menu */
    if (message == WM_KEYDOWN && key == SDLK_ESCAPE) {
        ui_game_main_toggle_sysmenu(update->hud_window);
        return;
    }

    /* Pass input to HUD first */
    if (update->hud_window && update->hud_window->keyboard_control) {
        update->hud_window->keyboard_control(update->hud_window, message, key, extra);
    }
}
