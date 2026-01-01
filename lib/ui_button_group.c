/**
 * @file ui_button_group.c
 * @brief Button container component implementation
 * 
 * Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7
 */

#include "ui_button_group.h"
#include "ui_window.h"
#include <string.h>

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

void ui_button_group_init(UI_ButtonGroup* group, struct UI_Window* parent) {
    if (!group) {
        return;
    }

    memset(group, 0, sizeof(UI_ButtonGroup));
    group->parent = parent;
    group->count = 0;
}

void ui_button_group_free(UI_ButtonGroup* group) {
    if (!group) {
        return;
    }

    /* Note: We don't free individual buttons - caller owns them */
    memset(group, 0, sizeof(UI_ButtonGroup));
}

/* ============================================================================
 * Button management
 * ============================================================================ */

int ui_button_group_add(UI_ButtonGroup* group, UI_Button* button) {
    if (!group || !button) {
        return -1;
    }

    if (group->count >= MAX_BUTTONS_PER_GROUP) {
        return -1; /* Group is full */
    }

    group->buttons[group->count] = button;
    group->count++;
    return 0;
}

int ui_button_group_remove(UI_ButtonGroup* group, int id) {
    if (!group) {
        return -1;
    }

    /* Find button by ID */
    int found_idx = -1;
    for (int i = 0; i < group->count; i++) {
        if (group->buttons[i] && group->buttons[i]->id == id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx < 0) {
        return -1; /* Not found */
    }

    /* Shift remaining buttons down */
    for (int i = found_idx; i < group->count - 1; i++) {
        group->buttons[i] = group->buttons[i + 1];
    }

    group->buttons[group->count - 1] = NULL;
    group->count--;
    return 0;
}

UI_Button* ui_button_group_get(UI_ButtonGroup* group, int id) {
    if (!group) {
        return NULL;
    }

    for (int i = 0; i < group->count; i++) {
        if (group->buttons[i] && group->buttons[i]->id == id) {
            return group->buttons[i];
        }
    }

    return NULL;
}

int ui_button_group_count(UI_ButtonGroup* group) {
    return group ? group->count : 0;
}

/* ============================================================================
 * State management
 * ============================================================================ */

void ui_button_group_reset(UI_ButtonGroup* group) {
    if (!group) {
        return;
    }

    for (int i = 0; i < group->count; i++) {
        if (group->buttons[i]) {
            ui_button_reset(group->buttons[i]);
        }
    }
}

/* ============================================================================
 * Event routing
 * ============================================================================ */

int ui_button_group_mouse_control(UI_ButtonGroup* group, uint32_t message, int x, int y) {
    if (!group) {
        return 0;
    }

    int consumed = 0;

    /* Route event to all buttons */
    for (int i = 0; i < group->count; i++) {
        if (group->buttons[i]) {
            if (ui_button_mouse_control(group->buttons[i], message, x, y)) {
                consumed = 1;
                /* Don't break - let all buttons update their state */
            }
        }
    }

    return consumed;
}

/* ============================================================================
 * Rendering
 * ============================================================================ */

void ui_button_group_render(UI_ButtonGroup* group, UI_Surface* surface, SpritePack* pack) {
    if (!group || !surface) {
        return;
    }

    /* Get parent position for absolute coordinates */
    int parent_x = 0;
    int parent_y = 0;
    if (group->parent) {
        parent_x = group->parent->x;
        parent_y = group->parent->y;
    }

    /* Render all buttons */
    for (int i = 0; i < group->count; i++) {
        if (group->buttons[i]) {
            ui_button_render(group->buttons[i], surface, pack, parent_x, parent_y);
        }
    }
}
