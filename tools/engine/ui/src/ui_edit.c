/**
 * @file ui_edit.c
 * @brief Text input editor implementation
 */

#include "ui_edit.h"
#include <string.h>
#include <ctype.h>

/* Windows keyboard message constants (for compatibility) */
#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0x0100
#endif

#ifndef WM_CHAR
#define WM_CHAR 0x0102
#endif

/* Virtual key codes */
#ifndef VK_BACK
#define VK_BACK    0x08
#define VK_RETURN  0x0D
#define VK_ESCAPE  0x1B
#define VK_DELETE  0x2E
#define VK_LEFT    0x25
#define VK_RIGHT   0x27
#define VK_HOME    0x24
#define VK_END     0x23
#endif

/* ============================================================================
 * Helper functions
 * ============================================================================ */

static void clamp_cursor(UI_Edit* edit) {
    if (edit->cursor < 0) {
        edit->cursor = 0;
    }
    if (edit->cursor > edit->length) {
        edit->cursor = edit->length;
    }
}

static void update_scroll(UI_Edit* edit) {
    /* Keep cursor visible by adjusting scroll */
    if (edit->cursor < edit->scroll) {
        edit->scroll = edit->cursor;
    }
    /* Note: Width-based scrolling needs font metrics, handled by visual wrapper */
}

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

void ui_edit_init(UI_Edit* edit, int max_length) {
    if (!edit) return;
    
    memset(edit, 0, sizeof(UI_Edit));
    edit->max_length = max_length;
    if (edit->max_length >= UI_EDIT_MAX_LENGTH) {
        edit->max_length = UI_EDIT_MAX_LENGTH - 1;
    }
    edit->length = 0;
    edit->cursor = 0;
    edit->scroll = 0;
    edit->focused = 0;
    edit->mode_flags = UI_EDIT_MODE_NORMAL;
    edit->buffer[0] = '\0';
}

void ui_edit_free(UI_Edit* edit) {
    /* No dynamic allocation, nothing to free */
    (void)edit;
}

void ui_edit_clear(UI_Edit* edit) {
    if (!edit) return;
    
    edit->buffer[0] = '\0';
    edit->length = 0;
    edit->cursor = 0;
    edit->scroll = 0;
}

/* ============================================================================
 * Focus management
 * ============================================================================ */

void ui_edit_acquire_focus(UI_Edit* edit) {
    if (edit) {
        edit->focused = 1;
    }
}

void ui_edit_release_focus(UI_Edit* edit) {
    if (edit) {
        edit->focused = 0;
    }
}

int ui_edit_is_focused(UI_Edit* edit) {
    return edit ? edit->focused : 0;
}

/* ============================================================================
 * Text manipulation
 * ============================================================================ */

int ui_edit_insert_char(UI_Edit* edit, char c) {
    if (!edit) return 0;
    
    /* Check if read-only */
    if (edit->mode_flags & UI_EDIT_MODE_READONLY) {
        return 0;
    }
    
    /* Check if character is allowed */
    if (!ui_edit_is_char_allowed(edit, c)) {
        return 0;
    }
    
    /* Check length limit */
    if (edit->length >= edit->max_length) {
        return 0;
    }
    
    /* Shift characters to the right */
    if (edit->cursor < edit->length) {
        memmove(&edit->buffer[edit->cursor + 1],
                &edit->buffer[edit->cursor],
                edit->length - edit->cursor);
    }
    
    /* Insert character */
    edit->buffer[edit->cursor] = c;
    edit->length++;
    edit->buffer[edit->length] = '\0';
    edit->cursor++;
    
    update_scroll(edit);
    return 1;
}

int ui_edit_delete_char(UI_Edit* edit) {
    if (!edit) return 0;
    
    /* Check if read-only */
    if (edit->mode_flags & UI_EDIT_MODE_READONLY) {
        return 0;
    }
    
    /* Check if at beginning */
    if (edit->cursor <= 0) {
        return 0;
    }
    
    /* Shift characters to the left */
    if (edit->cursor < edit->length) {
        memmove(&edit->buffer[edit->cursor - 1],
                &edit->buffer[edit->cursor],
                edit->length - edit->cursor);
    }
    
    edit->cursor--;
    edit->length--;
    edit->buffer[edit->length] = '\0';
    
    update_scroll(edit);
    return 1;
}

int ui_edit_delete_char_forward(UI_Edit* edit) {
    if (!edit) return 0;
    
    /* Check if read-only */
    if (edit->mode_flags & UI_EDIT_MODE_READONLY) {
        return 0;
    }
    
    /* Check if at end */
    if (edit->cursor >= edit->length) {
        return 0;
    }
    
    /* Shift characters to the left */
    memmove(&edit->buffer[edit->cursor],
            &edit->buffer[edit->cursor + 1],
            edit->length - edit->cursor - 1);
    
    edit->length--;
    edit->buffer[edit->length] = '\0';
    
    return 1;
}

void ui_edit_set_text(UI_Edit* edit, const char* text) {
    if (!edit || !text) return;
    
    int len = strlen(text);
    if (len > edit->max_length) {
        len = edit->max_length;
    }
    
    strncpy(edit->buffer, text, len);
    edit->buffer[len] = '\0';
    edit->length = len;
    edit->cursor = len;
    edit->scroll = 0;
}

void ui_edit_append_text(UI_Edit* edit, const char* text) {
    if (!edit || !text) return;
    
    int available = edit->max_length - edit->length;
    if (available <= 0) return;
    
    int len = strlen(text);
    if (len > available) {
        len = available;
    }
    
    strncat(edit->buffer, text, len);
    edit->length += len;
    edit->cursor = edit->length;
}

const char* ui_edit_get_text(UI_Edit* edit) {
    return edit ? edit->buffer : "";
}

int ui_edit_get_length(UI_Edit* edit) {
    return edit ? edit->length : 0;
}

/* ============================================================================
 * Cursor control
 * ============================================================================ */

void ui_edit_cursor_home(UI_Edit* edit) {
    if (!edit) return;
    
    edit->cursor = 0;
    edit->scroll = 0;
}

void ui_edit_cursor_end(UI_Edit* edit) {
    if (!edit) return;
    
    edit->cursor = edit->length;
    update_scroll(edit);
}

int ui_edit_cursor_left(UI_Edit* edit) {
    if (!edit) return 0;
    
    if (edit->cursor > 0) {
        edit->cursor--;
        update_scroll(edit);
        return 1;
    }
    return 0;
}

int ui_edit_cursor_right(UI_Edit* edit) {
    if (!edit) return 0;
    
    if (edit->cursor < edit->length) {
        edit->cursor++;
        update_scroll(edit);
        return 1;
    }
    return 0;
}

void ui_edit_set_cursor(UI_Edit* edit, int pos) {
    if (!edit) return;
    
    edit->cursor = pos;
    clamp_cursor(edit);
    update_scroll(edit);
}

int ui_edit_get_cursor(UI_Edit* edit) {
    return edit ? edit->cursor : 0;
}

/* ============================================================================
 * Mode control
 * ============================================================================ */

void ui_edit_set_password_mode(UI_Edit* edit, int enabled) {
    if (!edit) return;
    
    if (enabled) {
        edit->mode_flags |= UI_EDIT_MODE_PASSWORD;
    } else {
        edit->mode_flags &= ~UI_EDIT_MODE_PASSWORD;
    }
}

int ui_edit_is_password_mode(UI_Edit* edit) {
    return edit ? (edit->mode_flags & UI_EDIT_MODE_PASSWORD) != 0 : 0;
}

void ui_edit_set_digit_only_mode(UI_Edit* edit, int enabled) {
    if (!edit) return;
    
    if (enabled) {
        edit->mode_flags |= UI_EDIT_MODE_DIGIT_ONLY;
    } else {
        edit->mode_flags &= ~UI_EDIT_MODE_DIGIT_ONLY;
    }
}

void ui_edit_set_readonly_mode(UI_Edit* edit, int enabled) {
    if (!edit) return;
    
    if (enabled) {
        edit->mode_flags |= UI_EDIT_MODE_READONLY;
    } else {
        edit->mode_flags &= ~UI_EDIT_MODE_READONLY;
    }
}

/* ============================================================================
 * Keyboard input
 * ============================================================================ */

void ui_edit_keyboard_control(UI_Edit* edit, uint32_t message, uint32_t key, long extra) {
    if (!edit) return;
    (void)extra;  /* Unused parameter */
    
    /* Ignore input if not focused */
    if (!edit->focused) {
        return;
    }
    
    /* Ignore input if read-only */
    if (edit->mode_flags & UI_EDIT_MODE_READONLY) {
        return;
    }
    
    if (message == WM_KEYDOWN) {
        switch (key) {
            case VK_BACK:
                ui_edit_delete_char(edit);
                break;
                
            case VK_DELETE:
                ui_edit_delete_char_forward(edit);
                break;
                
            case VK_LEFT:
                ui_edit_cursor_left(edit);
                break;
                
            case VK_RIGHT:
                ui_edit_cursor_right(edit);
                break;
                
            case VK_HOME:
                ui_edit_cursor_home(edit);
                break;
                
            case VK_END:
                ui_edit_cursor_end(edit);
                break;
                
            case VK_RETURN:
            case VK_ESCAPE:
                /* These are typically handled by parent window */
                break;
        }
    }
    else if (message == WM_CHAR) {
        /* Printable character */
        char c = (char)key;
        
        /* Ignore control characters (except backspace handled above) */
        if (c >= 32 && c < 127) {
            ui_edit_insert_char(edit, c);
        }
    }
}

/* ============================================================================
 * Display properties
 * ============================================================================ */

void ui_edit_set_position(UI_Edit* edit, int x, int y) {
    if (!edit) return;
    
    edit->x = x;
    edit->y = y;
}

void ui_edit_set_width(UI_Edit* edit, int width) {
    if (!edit) return;
    
    edit->width = width;
}

const char* ui_edit_get_visible_text(UI_Edit* edit) {
    if (!edit) return "";
    
    /* Return text starting from scroll position */
    if (edit->scroll < edit->length) {
        return &edit->buffer[edit->scroll];
    }
    return "";
}

/* ============================================================================
 * Validation
 * ============================================================================ */

int ui_edit_is_char_allowed(UI_Edit* edit, char c) {
    if (!edit) return 0;
    
    /* Check digit-only mode */
    if (edit->mode_flags & UI_EDIT_MODE_DIGIT_ONLY) {
        return isdigit((unsigned char)c) != 0;
    }
    
    /* Allow most printable ASCII characters */
    return (c >= 32 && c < 127);
}
