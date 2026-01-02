/**
 * @file ui_login.h
 * @brief Login Window Component
 */

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback events */
#define UI_LOGIN_EVENT_OK 1
#define UI_LOGIN_EVENT_CANCEL 2
#define UI_LOGIN_EVENT_NEW 3

typedef struct UI_Login UI_Login;

typedef void (*UI_LoginCallback)(UI_Window* window, int event_type, void* data);

UI_Window* ui_login_create(UI_GlobalResource* global_resource, UI_LoginCallback callback, void* callback_data);

#ifdef __cplusplus
}
#endif

#endif /* UI_LOGIN_H */
