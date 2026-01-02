/**
 * @file ui_game_gear.h
 * @brief Gear (Equipment/Inventory) Window Interface
 */

#ifndef UI_GAME_GEAR_H
#define UI_GAME_GEAR_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_Gear UI_Gear;

UI_Window* ui_game_gear_create(UI_GlobalResource* global_resource);
void ui_game_gear_destroy(UI_Window* window);

void ui_game_gear_start(UI_Window* window);
void ui_game_gear_finish(UI_Window* window);
int ui_game_gear_is_visible(UI_Window* window);
void ui_game_gear_toggle(UI_Window* window);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_GEAR_H */
