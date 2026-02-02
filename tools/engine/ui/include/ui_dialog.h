/**
 * @file ui_dialog.h
 * @brief Dialog component with buttons
 *
 * Provides a modal dialog with OK/Cancel buttons and keyboard shortcuts.
 *
 * Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
 */

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include "ui_window.h"
#include "ui_button_group.h"
// #include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Dialog button flags
 * ============================================================================ */

#define DIALOG_OK     0x01
#define DIALOG_CANCEL 0x02

/* ============================================================================
 * Dialog execution IDs (passed to callback)
 * ============================================================================ */

#define DIALOG_EXECID_OK     1
#define DIALOG_EXECID_CANCEL 2

/* ============================================================================
 * Dialog structure
 * ============================================================================ */

typedef struct UI_Dialog UI_Dialog;
typedef void (*UI_DialogCallback)(UI_Dialog* dialog, int exec_id);

struct UI_Dialog {
    UI_Window window;           /* Base window (must be first for casting) */
    UI_ButtonGroup buttons;
    UI_Button ok_button;
    UI_Button cancel_button;
    UI_DialogCallback callback;
    int button_flags;
    char* message;
    UI_GlobalResource* resources;
};

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Create a dialog
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 * @param button_flags DIALOG_OK, DIALOG_CANCEL, or both OR'd together
 * @param callback Callback function for button clicks
 * @return Pointer to dialog, or NULL on failure
 */
UI_Dialog* ui_dialog_create(int x, int y, int w, int h, int button_flags, UI_DialogCallback callback);

/**
 * Destroy a dialog
 * @param dialog Pointer to dialog
 */
void ui_dialog_destroy(UI_Dialog* dialog);

/* ============================================================================
 * Configuration
 * ============================================================================ */

/**
 * Set dialog message text
 * @param dialog Pointer to dialog
 * @param message Message text (copied internally)
 */
void ui_dialog_set_message(UI_Dialog* dialog, const char* message);

/**
 * Set dialog resources (for sprite rendering)
 * @param dialog Pointer to dialog
 * @param resources Global resource pointer
 */
void ui_dialog_set_resources(UI_Dialog* dialog, UI_GlobalResource* resources);

/* ============================================================================
 * Control
 * ============================================================================ */

/**
 * Start (show) the dialog
 * @param dialog Pointer to dialog
 */
void ui_dialog_start(UI_Dialog* dialog);

/**
 * Finish (hide) the dialog
 * @param dialog Pointer to dialog
 */
void ui_dialog_finish(UI_Dialog* dialog);

/**
 * Get the base window pointer (for window manager registration)
 * @param dialog Pointer to dialog
 * @return Pointer to base UI_Window
 */
UI_Window* ui_dialog_get_window(UI_Dialog* dialog);

#ifdef __cplusplus
}
#endif

#endif /* UI_DIALOG_H */
