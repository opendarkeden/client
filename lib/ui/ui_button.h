/**
 * @file ui_button.h
 * @brief Interactive button component
 * 
 * Provides a clickable button with hover/pressed states and callback support.
 * 
 * Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9
 */

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <stdint.h>
#include "ui_surface.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_Button UI_Button;

/**
 * Button callback function type
 * @param button The button that was clicked
 * @param id Button identifier
 */
typedef void (*UI_ButtonCallback)(UI_Button* button, int id);

/**
 * Button render callback function type
 * Allows parent to customize button rendering based on state
 * @param button The button to render
 * @param surface Target surface
 * @param pack Sprite pack (may be NULL)
 * @param parent_x Parent window X position
 * @param parent_y Parent window Y position
 */
typedef void (*UI_ButtonRenderCallback)(UI_Button* button, UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y);

struct UI_Button {
    int x, y;           /* Position relative to parent */
    int w, h;           /* Dimensions */
    int id;             /* Button identifier */
    int focus;          /* Mouse hovering (1=hover, 0=normal) */
    int pressed;        /* Mouse button down (1=pressed, 0=released) */
    int sprite_index;   /* Base sprite index in pack */
    UI_ButtonCallback callback;
    UI_ButtonRenderCallback render_callback;  /* Custom render function */
    void* user_data;
};

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize a button
 * @param button Pointer to button structure
 * @param x X position relative to parent
 * @param y Y position relative to parent
 * @param w Width
 * @param h Height
 * @param id Button identifier
 */
void ui_button_init(UI_Button* button, int x, int y, int w, int h, int id);

/**
 * Free button resources
 * @param button Pointer to button structure
 */
void ui_button_free(UI_Button* button);

/* ============================================================================
 * State queries
 * ============================================================================ */

/**
 * Get button focus state
 * @param button Pointer to button structure
 * @return 1 if focused (hovering), 0 otherwise
 */
int ui_button_get_focus(UI_Button* button);

/**
 * Get button pressed state
 * @param button Pointer to button structure
 * @return 1 if pressed, 0 otherwise
 */
int ui_button_get_pressed(UI_Button* button);

/* ============================================================================
 * Configuration
 * ============================================================================ */

/**
 * Set button callback
 * @param button Pointer to button structure
 * @param callback Callback function
 */
void ui_button_set_callback(UI_Button* button, UI_ButtonCallback callback);

/**
 * Set button sprite index
 * @param button Pointer to button structure
 * @param sprite_index Base sprite index in pack
 */
void ui_button_set_sprite_index(UI_Button* button, int sprite_index);

/**
 * Set button user data
 * @param button Pointer to button structure
 * @param user_data User data pointer
 */
void ui_button_set_user_data(UI_Button* button, void* user_data);

/**
 * Set custom render callback
 * If set, this function will be called instead of default rendering
 * @param button Pointer to button structure
 * @param render_callback Custom render function (NULL for default)
 */
void ui_button_set_render_callback(UI_Button* button, UI_ButtonRenderCallback render_callback);

/* ============================================================================
 * Event handling
 * ============================================================================ */

/**
 * Handle mouse event
 * @param button Pointer to button structure
 * @param message Mouse message type (M_MOVING, M_LEFTBUTTON_DOWN, etc.)
 * @param x X coordinate relative to parent
 * @param y Y coordinate relative to parent
 * @return 1 if event was consumed, 0 otherwise
 */
int ui_button_mouse_control(UI_Button* button, uint32_t message, int x, int y);

/**
 * Reset button state (clear focus and pressed)
 * @param button Pointer to button structure
 */
void ui_button_reset(UI_Button* button);

/* ============================================================================
 * Rendering
 * ============================================================================ */

/**
 * Render button using sprite pack
 * @param button Pointer to button structure
 * @param surface Target surface
 * @param pack Sprite pack containing button sprites
 * @param parent_x Parent window X position
 * @param parent_y Parent window Y position
 */
void ui_button_render(UI_Button* button, UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y);

/**
 * Render button with fallback (simple rectangles)
 * @param button Pointer to button structure
 * @param surface Target surface
 * @param parent_x Parent window X position
 * @param parent_y Parent window Y position
 */
void ui_button_render_fallback(UI_Button* button, UI_Surface* surface, int parent_x, int parent_y);

/* ============================================================================
 * Utility
 * ============================================================================ */

/**
 * Check if point is inside button bounds
 * @param button Pointer to button structure
 * @param x X coordinate relative to parent
 * @param y Y coordinate relative to parent
 * @return 1 if inside, 0 otherwise
 */
int ui_button_contains_point(UI_Button* button, int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* UI_BUTTON_H */
