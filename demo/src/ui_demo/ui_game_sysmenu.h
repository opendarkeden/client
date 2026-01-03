/**
 * @file ui_game_sysmenu.h
 * @brief Game System Menu Window (Interactive)
 */

#ifndef UI_GAME_SYSMENU_H
#define UI_GAME_SYSMENU_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* System Menu Events */
#define UI_SYSMENU_EVENT_OPTION   1
#define UI_SYSMENU_EVENT_LOGOUT   2
#define UI_SYSMENU_EVENT_CONTINUE 3

typedef struct UI_GameSysMenu C_VS_UI_GAMEMENU;
typedef void (*UI_SysMenuCallback)(UI_Window* window, int event, void* data);

/**
 * Creates the System Menu window
 */
UI_Window* ui_game_sysmenu_create(UI_GlobalResource* global_resource, UI_SysMenuCallback callback, void* callback_data);

/**
 * Show/activate the system menu (centered on screen)
 */
void ui_game_sysmenu_start(UI_Window* window);

/**
 * Hide/deactivate the system menu
 */
void ui_game_sysmenu_finish(UI_Window* window);

/**
 * Check if system menu is currently visible
 */
int ui_game_sysmenu_is_visible(UI_Window* window);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_SYSMENU_H */
