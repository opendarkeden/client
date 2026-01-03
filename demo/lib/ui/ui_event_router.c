/**
 * @file ui_event_router.c
 * @brief SDL event to legacy UI message translation implementation
 * 
 * Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9
 */

#include "ui_event_router.h"
#include <string.h>

/* ============================================================================
 * Event translation
 * ============================================================================ */

int ui_event_translate(SDL_Event* sdl_event, UI_Event* ui_event) {
    if (!sdl_event || !ui_event) {
        return 0;
    }

    memset(ui_event, 0, sizeof(UI_Event));

    switch (sdl_event->type) {
        case SDL_MOUSEMOTION:
            ui_event->message = M_MOVING;
            ui_event->x = sdl_event->motion.x;
            ui_event->y = sdl_event->motion.y;
            return 1;

        case SDL_MOUSEBUTTONDOWN:
            ui_event->x = sdl_event->button.x;
            ui_event->y = sdl_event->button.y;
            
            if (sdl_event->button.button == SDL_BUTTON_LEFT) {
                if (sdl_event->button.clicks == 2) {
                    ui_event->message = M_LB_DOUBLECLICK;
                } else {
                    ui_event->message = M_LEFTBUTTON_DOWN;
                }
                return 1;
            } else if (sdl_event->button.button == SDL_BUTTON_RIGHT) {
                if (sdl_event->button.clicks == 2) {
                    ui_event->message = M_RB_DOUBLECLICK;
                } else {
                    ui_event->message = M_RIGHTBUTTON_DOWN;
                }
                return 1;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            ui_event->x = sdl_event->button.x;
            ui_event->y = sdl_event->button.y;
            
            if (sdl_event->button.button == SDL_BUTTON_LEFT) {
                ui_event->message = M_LEFTBUTTON_UP;
                return 1;
            } else if (sdl_event->button.button == SDL_BUTTON_RIGHT) {
                ui_event->message = M_RIGHTBUTTON_UP;
                return 1;
            }
            break;

        case SDL_MOUSEWHEEL:
            /* Get current mouse position for wheel events */
            SDL_GetMouseState(&ui_event->x, &ui_event->y);
            
            if (sdl_event->wheel.y > 0) {
                ui_event->message = M_WHEEL_UP;
                return 1;
            } else if (sdl_event->wheel.y < 0) {
                ui_event->message = M_WHEEL_DOWN;
                return 1;
            }
            break;

        case SDL_KEYDOWN:
            ui_event->message = WM_KEYDOWN;
            ui_event->key = sdl_event->key.keysym.sym;
            ui_event->extra = sdl_event->key.repeat;
            return 1;

        case SDL_KEYUP:
            ui_event->message = WM_KEYUP;
            ui_event->key = sdl_event->key.keysym.sym;
            ui_event->extra = 0;
            return 1;

        case SDL_TEXTINPUT:
            /* Map text input to WM_CHAR for character input */
            ui_event->message = WM_CHAR;
            /* Take first character (UTF-8 handling would be more complex) */
            ui_event->key = (uint32_t)(unsigned char)sdl_event->text.text[0];
            ui_event->extra = 0;
            return 1;

        default:
            break;
    }

    return 0;
}

/* ============================================================================
 * Event dispatch
 * ============================================================================ */

int ui_event_dispatch_to_window(UI_Event* event, UI_Window* window) {
    if (!event || !window) {
        return 0;
    }

    /* Check if window is visible */
    if (!window->attr.visible) {
        return 0;
    }

    if (ui_event_is_mouse(event)) {
        /* Check if point is inside window */
        if (window->is_pixel) {
            if (!window->is_pixel(window, event->x, event->y)) {
                /* For M_MOVING, still update but don't consume */
                if (event->message == M_MOVING && window->mouse_control) {
                    window->mouse_control(window, event->message, event->x, event->y);
                }
                return 0;
            }
        }

        if (window->mouse_control) {
            return window->mouse_control(window, event->message, event->x, event->y);
        }
    } else if (ui_event_is_keyboard(event)) {
        if (window->keyboard_control) {
            window->keyboard_control(window, event->message, event->key, event->extra);
            return 1; /* Keyboard events are always consumed by focused window */
        }
    }

    return 0;
}

int ui_event_is_mouse(UI_Event* event) {
    if (!event) {
        return 0;
    }

    switch (event->message) {
        case M_MOVING:
        case M_LEFTBUTTON_DOWN:
        case M_LEFTBUTTON_UP:
        case M_RIGHTBUTTON_DOWN:
        case M_RIGHTBUTTON_UP:
        case M_LB_DOUBLECLICK:
        case M_RB_DOUBLECLICK:
        case M_WHEEL_UP:
        case M_WHEEL_DOWN:
            return 1;
        default:
            return 0;
    }
}

int ui_event_is_keyboard(UI_Event* event) {
    if (!event) {
        return 0;
    }

    switch (event->message) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
            return 1;
        default:
            return 0;
    }
}
