/**
 * @file ui_server_select.h
 * @brief Server Select Window Component
 */

#ifndef UI_SERVER_SELECT_H
#define UI_SERVER_SELECT_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback events */
#define UI_SERVER_SELECT_EVENT_CONNECT 1
#define UI_SERVER_SELECT_EVENT_BACK 2

typedef void (*UI_ServerSelectCallback)(UI_Window* window, int event_type, void* data);

UI_Window* ui_server_select_create(UI_GlobalResource* global_resource, UI_ServerSelectCallback callback, void* callback_data);

#ifdef __cplusplus
}
#endif

#endif /* UI_SERVER_SELECT_H */
