/**
 * @file ui_game_main.h
 * @brief Main Game Interface (HUD Manager)
 */

#ifndef UI_GAME_MAIN_H
#define UI_GAME_MAIN_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Event types */
#define UI_GAME_MAIN_EVENT_EXIT 1

typedef void (*UI_GameMainCallback)(void* data, int event_type);

/**
 * Creates the Game Main UI window
 * @param global_resource Pointer to shared global resources
 * @param callback Event callback
 * @param callback_data User data for callback
 * @return New UI_Window instance
 */
UI_Window* ui_game_main_create(UI_GlobalResource* global_resource, UI_GameMainCallback callback, void* callback_data);

/**
 * Renders the Game HUD
 */
void ui_game_main_render(UI_Window* window);

/**
 * Processes the Game HUD logic
 */
void ui_game_main_process(UI_Window* window);

/**
 * Get System Menu window for registration with window manager
 */
UI_Window* ui_game_main_get_sysmenu(UI_Window* window);

/**
 * Toggle System Menu visibility
 */
void ui_game_main_toggle_sysmenu(UI_Window* window);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_MAIN_H */
