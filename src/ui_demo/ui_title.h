
/**
 * @file ui_title.h
 * @brief Title Screen Window
 *
 * Handles the main title screen with buttons for Connect, Option, Credit, and Exit.
 */

#ifndef UI_TITLE_H
#define UI_TITLE_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Button IDs matching legacy VS_UI_Title.cpp logic */
#define UI_TITLE_ID_CONNECT 1
#define UI_TITLE_ID_OPTION  2
#define UI_TITLE_ID_CREDIT  3
#define UI_TITLE_ID_EXIT    4

/* For public access if needed, otherwise keeping it opaque is better */
struct UI_Title;

/**
 * Create the Title Screen window
 * @param global_resource Pointer to initialized global resources
 * @return Pointer to new UI_Title window (cast to UI_Window* for manager), or NULL on failure
 */
UI_Window* ui_title_create(UI_GlobalResource* global_resource);

#ifdef __cplusplus
}
#endif

#endif /* UI_TITLE_H */
