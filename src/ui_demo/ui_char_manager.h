/**
 * @file ui_char_manager.h
 * @brief Character Manager Window Component
 */

#ifndef UI_CHAR_MANAGER_H
#define UI_CHAR_MANAGER_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback events */
#define UI_CHAR_MANAGER_EVENT_CREATE 1
#define UI_CHAR_MANAGER_EVENT_BACK 2
#define UI_CHAR_MANAGER_EVENT_ENTER_GAME 3

typedef void (*UI_CharManagerCallback)(UI_Window* window, int event_type, void* data);

UI_Window* ui_char_manager_create(UI_GlobalResource* global_resource, UI_CharManagerCallback callback, void* callback_data);

#ifdef __cplusplus
}
#endif

#endif /* UI_CHAR_MANAGER_H */
