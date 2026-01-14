/**
 * @file ui_window.h
 * @brief Base UI window component
 * 
 * Provides the base window structure with virtual methods for derived types.
 * Compatible with legacy DarkEden UI message types.
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9
 */

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Legacy message types (from i_signal.h)
 * ============================================================================ */

typedef enum {
    M_MOVING = 1,
    M_LEFTBUTTON_DOWN,
    M_LEFTBUTTON_UP,
    M_RIGHTBUTTON_DOWN,
    M_RIGHTBUTTON_UP,
    M_LB_DOUBLECLICK,
    M_RB_DOUBLECLICK,
    M_WHEEL_UP,
    M_WHEEL_DOWN
} UI_MouseMessage;

/* Windows-compatible keyboard message constants */
#define WM_KEYDOWN 0x0100
#define WM_KEYUP   0x0101
#define WM_CHAR    0x0102

/* ============================================================================
 * Window attributes
 * ============================================================================ */

typedef struct {
    int visible;
    int topmost;
    int pinned;
    int alpha;          /* 0-255 transparency */
} UI_WindowAttr;

/* ============================================================================
 * Window structure and function pointers
 * ============================================================================ */

typedef struct UI_Window UI_Window;

/* Virtual method types */
typedef void (*UI_ShowFunc)(UI_Window* window);
typedef void (*UI_ProcessFunc)(UI_Window* window);
typedef int (*UI_MouseControlFunc)(UI_Window* window, uint32_t message, int x, int y);
typedef void (*UI_KeyboardControlFunc)(UI_Window* window, uint32_t message, uint32_t key, long extra);
typedef int (*UI_IsPixelFunc)(UI_Window* window, int x, int y);

struct UI_Window {
    int x, y;           /* Position */
    int w, h;           /* Dimensions */
    UI_WindowAttr attr;
    void* user_data;    /* For derived types */
    
    /* Virtual methods */
    UI_ShowFunc show;
    UI_ProcessFunc process;
    UI_MouseControlFunc mouse_control;
    UI_KeyboardControlFunc keyboard_control;
    UI_IsPixelFunc is_pixel;
};

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize a window with default values
 * @param window Pointer to window structure
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 */
void ui_window_init(UI_Window* window, int x, int y, int w, int h);

/**
 * Set window rectangle
 * @param window Pointer to window structure
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 */
void ui_window_set_rect(UI_Window* window, int x, int y, int w, int h);

/* ============================================================================
 * Attribute helpers
 * ============================================================================ */

/**
 * Set window visibility
 * @param window Pointer to window structure
 * @param visible 1 for visible, 0 for hidden
 */
void ui_window_set_visible(UI_Window* window, int visible);

/**
 * Get window visibility
 * @param window Pointer to window structure
 * @return 1 if visible, 0 if hidden
 */
int ui_window_get_visible(UI_Window* window);

/**
 * Set window topmost flag
 * @param window Pointer to window structure
 * @param topmost 1 for topmost, 0 for normal
 */
void ui_window_set_topmost(UI_Window* window, int topmost);

/**
 * Set window pinned flag (cannot be moved)
 * @param window Pointer to window structure
 * @param pinned 1 for pinned, 0 for movable
 */
void ui_window_set_pinned(UI_Window* window, int pinned);

/**
 * Set window alpha
 * @param window Pointer to window structure
 * @param alpha Alpha value (0-255)
 */
void ui_window_set_alpha(UI_Window* window, int alpha);

/* ============================================================================
 * Default implementations
 * ============================================================================ */

/**
 * Default is_pixel implementation (simple rectangle check)
 * @param window Pointer to window structure
 * @param x X coordinate to test
 * @param y Y coordinate to test
 * @return 1 if point is inside window, 0 otherwise
 */
int ui_window_default_is_pixel(UI_Window* window, int x, int y);

/**
 * Default mouse_control implementation (no-op)
 * @param window Pointer to window structure
 * @param message Mouse message type
 * @param x X coordinate
 * @param y Y coordinate
 * @return 0 (event not consumed)
 */
int ui_window_default_mouse_control(UI_Window* window, uint32_t message, int x, int y);

/**
 * Default keyboard_control implementation (no-op)
 * @param window Pointer to window structure
 * @param message Keyboard message type
 * @param key Key code
 * @param extra Additional data
 */
void ui_window_default_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra);

/**
 * Default show implementation (no-op)
 * @param window Pointer to window structure
 */
void ui_window_default_show(UI_Window* window);

/**
 * Default process implementation (no-op)
 * @param window Pointer to window structure
 */
void ui_window_default_process(UI_Window* window);

/* ============================================================================
 * Utility
 * ============================================================================ */

/**
 * Check if a point is inside the window bounds
 * @param window Pointer to window structure
 * @param x X coordinate
 * @param y Y coordinate
 * @return 1 if inside, 0 otherwise
 */
int ui_window_contains_point(UI_Window* window, int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* UI_WINDOW_H */
