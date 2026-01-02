/**
 * @file ui_newchar.h
 * @brief New Character Window Component
 */

#ifndef UI_NEWCHAR_H
#define UI_NEWCHAR_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback events */
#define UI_NEWCHAR_EVENT_CREATE_OK 1
#define UI_NEWCHAR_EVENT_CANCEL 2

typedef void (*UI_NewCharCallback)(UI_Window* window, int event_type, void* data);

UI_Window* ui_newchar_create(UI_GlobalResource* global_resource, UI_NewCharCallback callback, void* callback_data);

#ifdef __cplusplus
}
#endif

#endif /* UI_NEWCHAR_H */
