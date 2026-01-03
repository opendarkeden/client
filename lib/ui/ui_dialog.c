/**
 * @file ui_dialog.c
 * @brief Dialog component implementation
 * 
 * Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
 */

#include "ui_dialog.h"
#include <stdlib.h>
#include <string.h>

/* Button dimensions and positioning */
#define BUTTON_WIDTH  60
#define BUTTON_HEIGHT 24
#define BUTTON_MARGIN 10

/* ============================================================================
 * Forward declarations for virtual methods
 * ============================================================================ */

static void dialog_show(UI_Window* window);
static void dialog_process(UI_Window* window);
static int dialog_mouse_control(UI_Window* window, uint32_t message, int x, int y);
static void dialog_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra);

/* ============================================================================
 * Button callback
 * ============================================================================ */

static void dialog_button_callback(UI_Button* button, int id) {
    if (!button || !button->on_press.fn) {
        return;
    }

    UI_Dialog* dialog = (UI_Dialog*)button->on_press.data;
    
    if (dialog->callback) {
        dialog->callback(dialog, id);
    }
}

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

UI_Dialog* ui_dialog_create(int x, int y, int w, int h, int button_flags, UI_DialogCallback callback) {
    UI_Dialog* dialog = (UI_Dialog*)malloc(sizeof(UI_Dialog));
    if (!dialog) {
        return NULL;
    }

    memset(dialog, 0, sizeof(UI_Dialog));

    /* Initialize base window */
    ui_window_init(&dialog->window, x, y, w, h);
    
    /* Set virtual methods */
    dialog->window.show = dialog_show;
    dialog->window.process = dialog_process;
    dialog->window.mouse_control = dialog_mouse_control;
    dialog->window.keyboard_control = dialog_keyboard_control;
    dialog->window.user_data = dialog;

    dialog->callback = callback;
    dialog->button_flags = button_flags;
    dialog->message = NULL;
    dialog->resources = NULL;

    /* Initialize button group */
    ui_button_group_init(&dialog->buttons, &dialog->window);

    /* Calculate button positions (centered at bottom) */
    int num_buttons = 0;
    if (button_flags & DIALOG_OK) num_buttons++;
    if (button_flags & DIALOG_CANCEL) num_buttons++;

    int total_button_width = num_buttons * BUTTON_WIDTH + (num_buttons - 1) * BUTTON_MARGIN;
    int button_start_x = (w - total_button_width) / 2;
    int button_y = h - BUTTON_HEIGHT - BUTTON_MARGIN;
    int current_x = button_start_x;

    /* Create OK button */
    if (button_flags & DIALOG_OK) {
        ui_button_init(&dialog->ok_button, current_x, button_y, BUTTON_WIDTH, BUTTON_HEIGHT, DIALOG_EXECID_OK);
  //      ui_button_set_sprite_index(&dialog->ok_button, AB_BUTTON_OK);
        ui_button_set_on_press(&dialog->ok_button, dialog_button_callback, dialog);
        ui_button_group_add(&dialog->buttons, &dialog->ok_button);
        current_x += BUTTON_WIDTH + BUTTON_MARGIN;
    }

    /* Create Cancel button */
    if (button_flags & DIALOG_CANCEL) {
        ui_button_init(&dialog->cancel_button, current_x, button_y, BUTTON_WIDTH, BUTTON_HEIGHT, DIALOG_EXECID_CANCEL);
//        ui_button_set_sprite_index(&dialog->cancel_button, AB_BUTTON_CANCEL);
        ui_button_set_on_press(&dialog->cancel_button, dialog_button_callback, dialog);
        ui_button_group_add(&dialog->buttons, &dialog->cancel_button);
    }

    return dialog;
}

void ui_dialog_destroy(UI_Dialog* dialog) {
    if (!dialog) {
        return;
    }

    if (dialog->message) {
        free(dialog->message);
        dialog->message = NULL;
    }

    ui_button_group_free(&dialog->buttons);
    free(dialog);
}

/* ============================================================================
 * Configuration
 * ============================================================================ */

void ui_dialog_set_message(UI_Dialog* dialog, const char* message) {
    if (!dialog) {
        return;
    }

    if (dialog->message) {
        free(dialog->message);
        dialog->message = NULL;
    }

    if (message) {
        size_t len = strlen(message);
        dialog->message = (char*)malloc(len + 1);
        if (dialog->message) {
            strcpy(dialog->message, message);
        }
    }
}

void ui_dialog_set_resources(UI_Dialog* dialog, UI_GlobalResource* resources) {
    if (dialog) {
        dialog->resources = resources;
    }
}

/* ============================================================================
 * Control
 * ============================================================================ */

void ui_dialog_start(UI_Dialog* dialog) {
    if (!dialog) {
        return;
    }

    ui_window_set_visible(&dialog->window, 1);
    ui_button_group_reset(&dialog->buttons);
}

void ui_dialog_finish(UI_Dialog* dialog) {
    if (!dialog) {
        return;
    }

    ui_window_set_visible(&dialog->window, 0);
}

UI_Window* ui_dialog_get_window(UI_Dialog* dialog) {
    return dialog ? &dialog->window : NULL;
}

/* ============================================================================
 * Virtual method implementations
 * ============================================================================ */

static void dialog_show(UI_Window* window) {
    if (!window || !window->user_data) {
        return;
    }

    UI_Dialog* dialog = (UI_Dialog*)window->user_data;
    UI_Surface* surface = NULL;
    SpritePack* button_pack = NULL;

    if (dialog->resources) {
        surface = ui_global_resource_get_surface(dialog->resources);
        button_pack = ui_global_resource_get_button_pack(dialog->resources);

        /* Draw dialog background */
        ui_global_resource_draw_dialog(dialog->resources, 
                                       window->x, window->y, 
                                       window->w, window->h, 0);
    }

    /* Render buttons */
    if (surface) {
        ui_button_group_render(&dialog->buttons, surface, button_pack);
    }

    /* TODO: Render message text (requires text rendering support) */
}

static void dialog_process(UI_Window* window) {
    (void)window;
    /* No per-frame processing needed currently */
}

static int dialog_mouse_control(UI_Window* window, uint32_t message, int x, int y) {
    if (!window || !window->user_data) {
        return 0;
    }

    UI_Dialog* dialog = (UI_Dialog*)window->user_data;

    /* Convert to dialog-relative coordinates */
    int rel_x = x - window->x;
    int rel_y = y - window->y;

    /* Route to button group */
    return ui_button_group_mouse_control(&dialog->buttons, message, rel_x, rel_y);
}

static void dialog_keyboard_control(UI_Window* window, uint32_t message, uint32_t key, long extra) {
    if (!window || !window->user_data) {
        return;
    }

    (void)extra;

    UI_Dialog* dialog = (UI_Dialog*)window->user_data;

    if (message != WM_KEYDOWN) {
        return;
    }

    /* Handle keyboard shortcuts */
    switch (key) {
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            /* Enter -> OK */
            if ((dialog->button_flags & DIALOG_OK) && dialog->callback) {
                dialog->callback(dialog, DIALOG_EXECID_OK);
            }
            break;

        case SDLK_ESCAPE:
            /* Escape -> Cancel (or OK if no cancel button) */
            if (dialog->callback) {
                if (dialog->button_flags & DIALOG_CANCEL) {
                    dialog->callback(dialog, DIALOG_EXECID_CANCEL);
                } else if (dialog->button_flags & DIALOG_OK) {
                    dialog->callback(dialog, DIALOG_EXECID_OK);
                }
            }
            break;

        default:
            break;
    }
}
