/**
 * @file ui_button_group.h
 * @brief Button container component
 * 
 * Manages a collection of buttons with event routing and rendering.
 * 
 * Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7
 */

#ifndef UI_BUTTON_GROUP_H
#define UI_BUTTON_GROUP_H

#include "ui_button.h"
#include "ui_surface.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BUTTONS_PER_GROUP 16

/* Forward declaration */
struct UI_Window;

typedef struct {
    UI_Button* buttons[MAX_BUTTONS_PER_GROUP];
    int count;
    struct UI_Window* parent;
} UI_ButtonGroup;

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize a button group
 * @param group Pointer to button group structure
 * @param parent Parent window (can be NULL)
 */
void ui_button_group_init(UI_ButtonGroup* group, struct UI_Window* parent);

/**
 * Free button group resources (does NOT free individual buttons)
 * @param group Pointer to button group structure
 */
void ui_button_group_free(UI_ButtonGroup* group);

/* ============================================================================
 * Button management
 * ============================================================================ */

/**
 * Add a button to the group
 * @param group Pointer to button group structure
 * @param button Button to add
 * @return 0 on success, -1 if group is full
 */
int ui_button_group_add(UI_ButtonGroup* group, UI_Button* button);

/**
 * Remove a button from the group by ID
 * @param group Pointer to button group structure
 * @param id Button ID to remove
 * @return 0 on success, -1 if not found
 */
int ui_button_group_remove(UI_ButtonGroup* group, int id);

/**
 * Get a button by ID
 * @param group Pointer to button group structure
 * @param id Button ID to find
 * @return Pointer to button, or NULL if not found
 */
UI_Button* ui_button_group_get(UI_ButtonGroup* group, int id);

/**
 * Get button count
 * @param group Pointer to button group structure
 * @return Number of buttons in group
 */
int ui_button_group_count(UI_ButtonGroup* group);

/* ============================================================================
 * State management
 * ============================================================================ */

/**
 * Reset all button states (clear focus and pressed)
 * @param group Pointer to button group structure
 */
void ui_button_group_reset(UI_ButtonGroup* group);

/* ============================================================================
 * Event routing
 * ============================================================================ */

/**
 * Route mouse event to all buttons
 * @param group Pointer to button group structure
 * @param message Mouse message type
 * @param x X coordinate relative to parent
 * @param y Y coordinate relative to parent
 * @return 1 if any button consumed the event, 0 otherwise
 */
int ui_button_group_mouse_control(UI_ButtonGroup* group, uint32_t message, int x, int y);

/* ============================================================================
 * Rendering
 * ============================================================================ */

/**
 * Render all buttons in the group
 * @param group Pointer to button group structure
 * @param surface Target surface
 * @param pack Sprite pack for button sprites (can be NULL for fallback)
 */
void ui_button_group_render(UI_ButtonGroup* group, UI_Surface* surface, SpritePack* pack);

#ifdef __cplusplus
}
#endif

#endif /* UI_BUTTON_GROUP_H */
