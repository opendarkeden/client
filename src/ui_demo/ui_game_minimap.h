/**
 * @file ui_game_minimap.h
 * @brief Game Minimap Interface
 */

#ifndef UI_GAME_MINIMAP_H
#define UI_GAME_MINIMAP_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_GameMinimap UI_GameMinimap;

UI_GameMinimap* ui_game_minimap_create(UI_GlobalResource* global_resource);
void ui_game_minimap_destroy(UI_GameMinimap* minimap);

void ui_game_minimap_render(UI_GameMinimap* minimap, int x, int y);
void ui_game_minimap_process(UI_GameMinimap* minimap);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_MINIMAP_H */
