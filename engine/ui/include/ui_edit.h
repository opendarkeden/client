/**
 * @file ui_edit.h
 * @brief Text input editor component
 * 
 * Provides single-line text editing with cursor control, password mode,
 * and input validation. Pure C implementation for maximum portability.
 * 
 * Requirements: 3.10, 3.11, 3.12, 3.13, 3.14, 3.15
 */

#ifndef UI_EDIT_H
#define UI_EDIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Constants
 * ============================================================================ */

#define UI_EDIT_MAX_LENGTH 256  /* Maximum text buffer size */

/* ============================================================================
 * Types
 * ============================================================================ */

typedef struct UI_Edit UI_Edit;

/**
 * Edit mode flags
 */
typedef enum {
    UI_EDIT_MODE_NORMAL = 0x00,
    UI_EDIT_MODE_PASSWORD = 0x01,   /* Display '*' for each character */
    UI_EDIT_MODE_DIGIT_ONLY = 0x02, /* Only accept digits 0-9 */
    UI_EDIT_MODE_READONLY = 0x04    /* Read-only, no editing allowed */
} UI_EditMode;

/**
 * Text editor structure
 */
struct UI_Edit {
    char buffer[UI_EDIT_MAX_LENGTH];  /* Text buffer */
    int length;                        /* Current text length */
    int cursor;                        /* Cursor position (0 to length) */
    int max_length;                    /* Maximum allowed length */
    int scroll;                        /* Horizontal scroll offset */
    int focused;                       /* 1 if focused (receiving input) */
    uint32_t mode_flags;               /* UI_EditMode flags */
    
    /* Display properties (for visual wrapper) */
    int x, y;                          /* Position */
    int width;                         /* Display width in pixels */
};

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize an edit control
 * @param edit Pointer to edit structure
 * @param max_length Maximum text length (< UI_EDIT_MAX_LENGTH)
 */
void ui_edit_init(UI_Edit* edit, int max_length);

/**
 * Free edit resources
 * @param edit Pointer to edit structure
 */
void ui_edit_free(UI_Edit* edit);

/**
 * Clear all text
 * @param edit Pointer to edit structure
 */
void ui_edit_clear(UI_Edit* edit);

/* ============================================================================
 * Focus management
 * ============================================================================ */

/**
 * Acquire focus (start receiving input)
 * @param edit Pointer to edit structure
 */
void ui_edit_acquire_focus(UI_Edit* edit);

/**
 * Release focus (stop receiving input)
 * @param edit Pointer to edit structure
 */
void ui_edit_release_focus(UI_Edit* edit);

/**
 * Check if edit has focus
 * @param edit Pointer to edit structure
 * @return 1 if focused, 0 otherwise
 */
int ui_edit_is_focused(UI_Edit* edit);

/* ============================================================================
 * Text manipulation
 * ============================================================================ */

/**
 * Insert character at cursor position
 * @param edit Pointer to edit structure
 * @param c Character to insert
 * @return 1 if inserted, 0 if failed (length limit reached)
 */
int ui_edit_insert_char(UI_Edit* edit, char c);

/**
 * Delete character before cursor (backspace)
 * @param edit Pointer to edit structure
 * @return 1 if deleted, 0 if at beginning
 */
int ui_edit_delete_char(UI_Edit* edit);

/**
 * Delete character at cursor (delete key)
 * @param edit Pointer to edit structure
 * @return 1 if deleted, 0 if at end
 */
int ui_edit_delete_char_forward(UI_Edit* edit);

/**
 * Set text content
 * @param edit Pointer to edit structure
 * @param text Text to set (will be truncated if too long)
 */
void ui_edit_set_text(UI_Edit* edit, const char* text);

/**
 * Append text to end
 * @param edit Pointer to edit structure
 * @param text Text to append
 */
void ui_edit_append_text(UI_Edit* edit, const char* text);

/**
 * Get text content
 * @param edit Pointer to edit structure
 * @return Pointer to internal buffer (valid until next modification)
 */
const char* ui_edit_get_text(UI_Edit* edit);

/**
 * Get text length
 * @param edit Pointer to edit structure
 * @return Current text length
 */
int ui_edit_get_length(UI_Edit* edit);

/* ============================================================================
 * Cursor control
 * ============================================================================ */

/**
 * Move cursor to home (beginning)
 * @param edit Pointer to edit structure
 */
void ui_edit_cursor_home(UI_Edit* edit);

/**
 * Move cursor to end
 * @param edit Pointer to edit structure
 */
void ui_edit_cursor_end(UI_Edit* edit);

/**
 * Move cursor left
 * @param edit Pointer to edit structure
 * @return 1 if moved, 0 if at beginning
 */
int ui_edit_cursor_left(UI_Edit* edit);

/**
 * Move cursor right
 * @param edit Pointer to edit structure
 * @return 1 if moved, 0 if at end
 */
int ui_edit_cursor_right(UI_Edit* edit);

/**
 * Set cursor position
 * @param edit Pointer to edit structure
 * @param pos New cursor position (clamped to valid range)
 */
void ui_edit_set_cursor(UI_Edit* edit, int pos);

/**
 * Get cursor position
 * @param edit Pointer to edit structure
 * @return Current cursor position
 */
int ui_edit_get_cursor(UI_Edit* edit);

/* ============================================================================
 * Mode control
 * ============================================================================ */

/**
 * Set password mode
 * @param edit Pointer to edit structure
 * @param enabled 1 to enable, 0 to disable
 */
void ui_edit_set_password_mode(UI_Edit* edit, int enabled);

/**
 * Check if password mode is enabled
 * @param edit Pointer to edit structure
 * @return 1 if password mode, 0 otherwise
 */
int ui_edit_is_password_mode(UI_Edit* edit);

/**
 * Set digit-only mode
 * @param edit Pointer to edit structure
 * @param enabled 1 to enable, 0 to disable
 */
void ui_edit_set_digit_only_mode(UI_Edit* edit, int enabled);

/**
 * Set read-only mode
 * @param edit Pointer to edit structure
 * @param enabled 1 to enable, 0 to disable
 */
void ui_edit_set_readonly_mode(UI_Edit* edit, int enabled);

/* ============================================================================
 * Keyboard input
 * ============================================================================ */

/**
 * Handle keyboard event
 * @param edit Pointer to edit structure
 * @param message Keyboard message type (WM_KEYDOWN, WM_CHAR, etc.)
 * @param key Virtual key code or character
 * @param extra Additional data (scan code, etc.)
 */
void ui_edit_keyboard_control(UI_Edit* edit, uint32_t message, uint32_t key, long extra);

/* ============================================================================
 * Display properties
 * ============================================================================ */

/**
 * Set display position
 * @param edit Pointer to edit structure
 * @param x X position
 * @param y Y position
 */
void ui_edit_set_position(UI_Edit* edit, int x, int y);

/**
 * Set display width
 * @param edit Pointer to edit structure
 * @param width Width in pixels
 */
void ui_edit_set_width(UI_Edit* edit, int width);

/**
 * Get visible text (accounting for scroll)
 * @param edit Pointer to edit structure
 * @return Pointer to visible portion of text
 */
const char* ui_edit_get_visible_text(UI_Edit* edit);

/* ============================================================================
 * Validation
 * ============================================================================ */

/**
 * Check if character is allowed
 * @param edit Pointer to edit structure
 * @param c Character to check
 * @return 1 if allowed, 0 otherwise
 */
int ui_edit_is_char_allowed(UI_Edit* edit, char c);

#ifdef __cplusplus
}
#endif

#endif /* UI_EDIT_H */
