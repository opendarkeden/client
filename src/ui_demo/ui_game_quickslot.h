/**
 * @file ui_game_quickslot.h
 * @brief Quick Item Slot Interface (Slayer)
 */

#ifndef UI_GAME_QUICKSLOT_H
#define UI_GAME_QUICKSLOT_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_QuickSlot UI_QuickSlot;

UI_QuickSlot* ui_game_quickslot_create(UI_GlobalResource* global_resource);
void ui_game_quickslot_destroy(UI_QuickSlot* quickslot);

void ui_game_quickslot_render(UI_QuickSlot* quickslot, int x, int y);
void ui_game_quickslot_process(UI_QuickSlot* quickslot);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_QUICKSLOT_H */
