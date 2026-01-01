/**
 * @file ui_window.c
 * @brief Base UI window component implementation
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9
 */

#include "ui_window.h"
#include <string.h>

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

void ui_window_init(UI_Window* window, int x, int y, int w, int h) {
    if (!window) {
        return;
    }

    memset(window, 0, sizeof(UI_Window));
    window->x = x;
    window->y = y;
    window->w = w;
    window->h = h;

    /* Default attributes */
    window->attr.visible = 0;
    window->attr.topmost = 0;
    window->attr.pinned = 0;
    window->attr.alpha = 255;

    /* Set default implementations */
    window->show = ui_window_default_show;
    window->process = ui_window_default_process;
    window->mouse_control = ui_window_default_mouse_control;
    window->keyboard_control = ui_window_default_keyboard_control;
    window->is_pixel = ui_window_default_is_pixel;
}

void ui_window_set_rect(UI_Window* window, int x, int y, int w, int h) {
    if (!window) {
        return;
    }
    window->x = x;
    window->y = y;
    window->w = w;
    window->h = h;
}

/* ============================================================================
 * Attribute helpers
 * ============================================================================ */

void ui_window_set_visible(UI_Window* window, int visible) {
    if (window) {
        window->attr.visible = visible ? 1 : 0;
    }
}

int ui_window_get_visible(UI_Window* window) {
    return window ? window->attr.visible : 0;
}

void ui_window_set_topmost(UI_Window* window, int topmost) {
    if (window) {
        window->attr.topmost = topmost ? 1 : 0;
    }
}

void ui_window_set_pinned(UI_Window* window, int pinned) {
    if (window) {
        window->attr.pinned = pinned ? 1 : 0;
    }
}

void ui_window_set_alpha(UI_Window* window, int alpha) {
    if (window) {
        if (alpha < 0) alpha = 0;
        if (alpha > 255) alpha = 255;
        window->attr.alpha = alpha;
    }
}

/* ============================================================================
 * Default implementations
 * ============================================================================ */

int ui_window_default_is_pixel(UI_Window* window, int x, int y) {
    return ui_window_contains_point(window, x, y);
}

int ui_window_default_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    (void)window;
    (void)message;
    (void)x;
    (void)y;
    return 0; /* Event not consumed */
}

void ui_window_default_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra) {
    (void)window;
    (void)message;
    (void)key;
    (void)extra;
}

void ui_window_default_show(UI_Window* window) {
    (void)window;
}

void ui_window_default_process(UI_Window* window) {
    (void)window;
}

/* ============================================================================
 * Utility
 * ============================================================================ */

int ui_window_contains_point(UI_Window* window, int x, int y) {
    if (!window) {
        return 0;
    }
    return (x >= window->x && x < window->x + window->w &&
            y >= window->y && y < window->y + window->h);
}
