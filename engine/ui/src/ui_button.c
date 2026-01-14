/**
 * @file ui_button.c
 * @brief Interactive button component implementation
 * 
 * Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9
 */

#include "ui_button.h"
#include "ui_window.h"
#include "sprite.h"
#include <string.h>

/* Button sprite offsets */
#define BUTTON_SPRITE_NORMAL     0
#define BUTTON_SPRITE_HOVER      3
#define BUTTON_SPRITE_PRESSED    6

/* Fallback colors (RGBA format) */
#define COLOR_NORMAL   0x404040FF
#define COLOR_HOVER    0x606060FF
#define COLOR_PRESSED  0x303030FF
#define COLOR_BORDER   0xFFFFFFFF

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

void ui_button_init(UI_Button* button, int x, int y, int w, int h, int id) {
    if (!button) {
        return;
    }

    memset(button, 0, sizeof(UI_Button));
    button->x = x;
    button->y = y;
    button->w = w;
    button->h = h;
    button->id = id;
    button->focus = 0;
    button->pressed = 0;
    button->on_press.fn = NULL;
    button->on_press.data = NULL;
    button->show_widget.fn = NULL;
    button->show_widget.data = NULL;
}

void ui_button_free(UI_Button* button) {
    if (!button) {
        return;
    }
    /* No dynamic resources to free currently */
    memset(button, 0, sizeof(UI_Button));
}

/* ============================================================================
 * State queries
 * ============================================================================ */

int ui_button_get_focus(UI_Button* button) {
    return button ? button->focus : 0;
}

int ui_button_get_pressed(UI_Button* button) {
    return button ? button->pressed : 0;
}

/* ============================================================================
 * Configuration
 * ============================================================================ */

void ui_button_set_on_press(UI_Button* button, UI_ButtonOnPressCallback callback, void* data) {
    if (button) {
        button->on_press.fn = callback;
        button->on_press.data = data;
    }
}

void ui_button_set_show_widget(UI_Button* button, UI_ButtonRenderCallback callback, void* data) {
    if (button) {
        button->show_widget.fn = callback;
        button->show_widget.data = data;
    }
}

/* ============================================================================
 * Utility
 * ============================================================================ */

int ui_button_contains_point(UI_Button* button, int x, int y) {
    if (!button) {
        return 0;
    }
    return (x >= button->x && x < button->x + button->w &&
            y >= button->y && y < button->y + button->h);
}

/* ============================================================================
 * Event handling
 * ============================================================================ */

int ui_button_mouse_control(UI_Button* button, uint32_t message, int x, int y) {
    if (!button) {
        return 0;
    }

    int inside = ui_button_contains_point(button, x, y);

    switch (message) {
        case M_MOVING:
            button->focus = inside;
            /* If mouse moves outside while pressed, keep pressed but lose focus */
            if (!inside && button->pressed) {
                /* Still pressed, but not focused */
            }
            return inside; /* Consume if inside */

        case M_LEFTBUTTON_DOWN:
            if (inside) {
                button->pressed = 1;
                button->focus = 1;
                return 1; /* Consumed */
            }
            break;

        case M_LEFTBUTTON_UP:
            if (button->pressed) {
                int was_pressed = button->pressed;
                button->pressed = 0;
                
                /* Trigger callback only if released inside */
                if (inside && was_pressed && button->on_press.fn) {
                    button->on_press.fn(button, button->id);
                }
                return 1; /* Consumed */
            }
            break;

        default:
            break;
    }

    return 0;
}

void ui_button_reset(UI_Button* button) {
    if (button) {
        button->focus = 0;
        button->pressed = 0;
    }
}

/* ============================================================================
 * Rendering
 * ============================================================================ */

void ui_button_render(UI_Button* button, UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y) {
    if (!button || !surface) {
        return;
    }

    /* Fall back to simple rendering if no sprite pack */
    if (!pack) {
        goto fallback;
    }

    /* Use custom render callback if provided */
    if (!button->show_widget.fn) {
        goto fallback;
    }

    button->show_widget.fn(button, surface, pack, parent_x, parent_y);
    return;

fallback:
    ui_button_render_fallback(button, surface, parent_x, parent_y);
    return;
}

void ui_button_render_fallback(UI_Button* button, UI_Surface* surface, int parent_x, int parent_y) {
    if (!button || !surface) {
        return;
    }

    int abs_x = parent_x + button->x;
    int abs_y = parent_y + button->y;

    /* Choose color based on state */
    uint32_t fill_color = COLOR_NORMAL;
    if (button->pressed) {
        fill_color = COLOR_PRESSED;
    } else if (button->focus) {
        fill_color = COLOR_HOVER;
    }

    /* Draw filled rectangle */
    UI_Rect rect = { abs_x, abs_y, button->w, button->h };
    ui_surface_fill_rect(surface, &rect, fill_color);

    /* Draw border */
    ui_surface_draw_rect(surface, &rect, COLOR_BORDER);
}
