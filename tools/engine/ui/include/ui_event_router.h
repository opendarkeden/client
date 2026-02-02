/**
 * @file ui_event_router.h
 * @brief SDL event to legacy UI message translation
 * 
 * Translates SDL events to legacy DarkEden UI message format.
 * 
 * Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9
 */

#ifndef UI_EVENT_ROUTER_H
#define UI_EVENT_ROUTER_H

#include <SDL.h>
#include <stdint.h>
#include "ui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UI Event structure - translated from SDL events
 */
typedef struct {
    uint32_t message;   /* UI_MouseMessage or WM_KEY* constant */
    int x, y;           /* Mouse position */
    uint32_t key;       /* Key code for keyboard events */
    long extra;         /* Additional data (e.g., repeat count) */
} UI_Event;

/* ============================================================================
 * Event translation
 * ============================================================================ */

/**
 * Translate SDL event to UI event
 * @param sdl_event Source SDL event
 * @param ui_event Destination UI event
 * @return 1 if event was translated, 0 if event type not supported
 */
int ui_event_translate(SDL_Event* sdl_event, UI_Event* ui_event);

/* ============================================================================
 * Event dispatch
 * ============================================================================ */

/**
 * Dispatch event to a window
 * @param event UI event to dispatch
 * @param window Target window
 * @return 1 if event was consumed, 0 otherwise
 */
int ui_event_dispatch_to_window(UI_Event* event, UI_Window* window);

/**
 * Check if event is a mouse event
 * @param event UI event to check
 * @return 1 if mouse event, 0 otherwise
 */
int ui_event_is_mouse(UI_Event* event);

/**
 * Check if event is a keyboard event
 * @param event UI event to check
 * @return 1 if keyboard event, 0 otherwise
 */
int ui_event_is_keyboard(UI_Event* event);

#ifdef __cplusplus
}
#endif

#endif /* UI_EVENT_ROUTER_H */
