/**
 * @file ui_window_manager.h
 * @brief Window management and z-order control
 * 
 * Manages a collection of windows with z-order, event dispatch, and rendering.
 * 
 * Requirements: 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8
 */

#ifndef UI_WINDOW_MANAGER_H
#define UI_WINDOW_MANAGER_H

#include "ui_window.h"
#include "ui_surface.h"
#include "ui_event_router.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_WINDOWS 32

typedef struct {
    UI_Window* windows[MAX_WINDOWS];
    int count;
    UI_Surface* surface;
} UI_WindowManager;

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize window manager
 * @param wm Pointer to window manager structure
 * @param surface Target surface for rendering
 */
void ui_window_manager_init(UI_WindowManager* wm, UI_Surface* surface);

/**
 * Free window manager resources (does NOT free individual windows)
 * @param wm Pointer to window manager structure
 */
void ui_window_manager_free(UI_WindowManager* wm);

/* ============================================================================
 * Window registration
 * ============================================================================ */

/**
 * Register a window with the manager
 * @param wm Pointer to window manager structure
 * @param window Window to register
 * @return 0 on success, -1 if manager is full
 */
int ui_window_manager_register(UI_WindowManager* wm, UI_Window* window);

/**
 * Unregister a window from the manager
 * @param wm Pointer to window manager structure
 * @param window Window to unregister
 * @return 0 on success, -1 if not found
 */
int ui_window_manager_unregister(UI_WindowManager* wm, UI_Window* window);

/**
 * Get window count
 * @param wm Pointer to window manager structure
 * @return Number of registered windows
 */
int ui_window_manager_count(UI_WindowManager* wm);

/* ============================================================================
 * Z-order management
 * ============================================================================ */

/**
 * Bring a window to the front (highest z-order)
 * @param wm Pointer to window manager structure
 * @param window Window to bring to front
 */
void ui_window_manager_bring_to_front(UI_WindowManager* wm, UI_Window* window);

/**
 * Make a window appear (visible and brought to front)
 * @param wm Pointer to window manager structure
 * @param window Window to show
 */
void ui_window_manager_appear(UI_WindowManager* wm, UI_Window* window);

/**
 * Make a window disappear (hidden)
 * @param wm Pointer to window manager structure
 * @param window Window to hide
 */
void ui_window_manager_disappear(UI_WindowManager* wm, UI_Window* window);

/* ============================================================================
 * Frame processing
 * ============================================================================ */

/**
 * Process all visible windows (call their process functions)
 * @param wm Pointer to window manager structure
 */
void ui_window_manager_process(UI_WindowManager* wm);

/**
 * Render all visible windows in z-order
 * @param wm Pointer to window manager structure
 */
void ui_window_manager_render(UI_WindowManager* wm);

/* ============================================================================
 * Event dispatch
 * ============================================================================ */

/**
 * Dispatch event to windows in reverse z-order (topmost first)
 * @param wm Pointer to window manager structure
 * @param event Event to dispatch
 * @return 1 if any window consumed the event, 0 otherwise
 */
int ui_window_manager_dispatch_event(UI_WindowManager* wm, UI_Event* event);

#ifdef __cplusplus
}
#endif

#endif /* UI_WINDOW_MANAGER_H */
