/**
 * @file ui_game_update.h
 * @brief Game Mode Update Loop
 *
 * Manages the game loop when in active game mode, including world rendering (placeholder)
 * and UI overlay rendering (delegated to ui_game_main).
 */

#ifndef UI_GAME_UPDATE_H
#define UI_GAME_UPDATE_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_GameUpdate UI_GameUpdate;

/* Event Callback */
typedef void (*UI_GameUpdateCallback)(void* data, int event_type);

/**
 * Creates the Game Update manager
 */
UI_GameUpdate* ui_game_update_create(UI_GlobalResource* global_resource, UI_GameUpdateCallback callback, void* callback_data);

/**
 * Destroys the Game Update manager
 */
void ui_game_update_destroy(UI_GameUpdate* update);

/**
 * Main Game Loop Process (Input/Logic)
 */
void ui_game_update_process(UI_GameUpdate* update);

/**
 * Main Game Loop Render (World + UI)
 */
void ui_game_update_render(UI_GameUpdate* update);

/**
 * Pass mouse input to Game Update
 */
int ui_game_update_mouse_control(UI_GameUpdate* update, uint32_t message, int x, int y);

/**
 * Pass keyboard input to Game Update
 */
void ui_game_update_keyboard_control(UI_GameUpdate* update, uint32_t message, uint32_t key, long extra);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_UPDATE_H */
