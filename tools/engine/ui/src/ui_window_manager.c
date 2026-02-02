/**
 * @file ui_window_manager.c
 * @brief Window management and z-order control implementation
 * 
 * Requirements: 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8
 */

#include "ui_window_manager.h"
#include <string.h>

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

void ui_window_manager_init(UI_WindowManager* wm, UI_Surface* surface) {
    if (!wm) {
        return;
    }

    memset(wm, 0, sizeof(UI_WindowManager));
    wm->surface = surface;
    wm->count = 0;
}

void ui_window_manager_free(UI_WindowManager* wm) {
    if (!wm) {
        return;
    }

    /* Note: We don't free individual windows - caller owns them */
    memset(wm, 0, sizeof(UI_WindowManager));
}

/* ============================================================================
 * Window registration
 * ============================================================================ */

int ui_window_manager_register(UI_WindowManager* wm, UI_Window* window) {
    if (!wm || !window) {
        return -1;
    }

    if (wm->count >= MAX_WINDOWS) {
        return -1; /* Manager is full */
    }

    /* Check if already registered */
    for (int i = 0; i < wm->count; i++) {
        if (wm->windows[i] == window) {
            return 0; /* Already registered, success */
        }
    }

    wm->windows[wm->count] = window;
    wm->count++;
    return 0;
}

int ui_window_manager_unregister(UI_WindowManager* wm, UI_Window* window) {
    if (!wm || !window) {
        return -1;
    }

    /* Find window */
    int found_idx = -1;
    for (int i = 0; i < wm->count; i++) {
        if (wm->windows[i] == window) {
            found_idx = i;
            break;
        }
    }

    if (found_idx < 0) {
        return -1; /* Not found */
    }

    /* Shift remaining windows down */
    for (int i = found_idx; i < wm->count - 1; i++) {
        wm->windows[i] = wm->windows[i + 1];
    }

    wm->windows[wm->count - 1] = NULL;
    wm->count--;
    return 0;
}

int ui_window_manager_count(UI_WindowManager* wm) {
    return wm ? wm->count : 0;
}

/* ============================================================================
 * Z-order management
 * ============================================================================ */

void ui_window_manager_bring_to_front(UI_WindowManager* wm, UI_Window* window) {
    if (!wm || !window || wm->count <= 1) {
        return;
    }

    /* Find window */
    int found_idx = -1;
    for (int i = 0; i < wm->count; i++) {
        if (wm->windows[i] == window) {
            found_idx = i;
            break;
        }
    }

    if (found_idx < 0 || found_idx == wm->count - 1) {
        return; /* Not found or already at front */
    }

    /* Shift windows down and move target to end */
    for (int i = found_idx; i < wm->count - 1; i++) {
        wm->windows[i] = wm->windows[i + 1];
    }
    wm->windows[wm->count - 1] = window;
}

void ui_window_manager_appear(UI_WindowManager* wm, UI_Window* window) {
    if (!wm || !window) {
        return;
    }

    ui_window_set_visible(window, 1);
    ui_window_manager_bring_to_front(wm, window);
}

void ui_window_manager_disappear(UI_WindowManager* wm, UI_Window* window) {
    (void)wm; /* Not used, but kept for API consistency */
    
    if (!window) {
        return;
    }

    ui_window_set_visible(window, 0);
}

/* ============================================================================
 * Frame processing
 * ============================================================================ */

void ui_window_manager_process(UI_WindowManager* wm) {
    if (!wm) {
        return;
    }

    /* Process all visible windows */
    for (int i = 0; i < wm->count; i++) {
        UI_Window* window = wm->windows[i];
        if (window && window->attr.visible && window->process) {
            window->process(window);
        }
    }
}

void ui_window_manager_render(UI_WindowManager* wm) {
    if (!wm) {
        return;
    }

    /* Render visible windows in z-order (0 = bottom, count-1 = top) */
    for (int i = 0; i < wm->count; i++) {
        UI_Window* window = wm->windows[i];
        if (window && window->attr.visible && window->show) {
            window->show(window);
        }
    }
}

/* ============================================================================
 * Event dispatch
 * ============================================================================ */

int ui_window_manager_dispatch_event(UI_WindowManager* wm, UI_Event* event) {
    if (!wm || !event) {
        return 0;
    }

    /* Dispatch in reverse z-order (topmost first) */
    for (int i = wm->count - 1; i >= 0; i--) {
        UI_Window* window = wm->windows[i];
        if (window && window->attr.visible) {
            if (ui_event_dispatch_to_window(event, window)) {
                return 1; /* Event consumed */
            }
        }
    }

    return 0;
}
