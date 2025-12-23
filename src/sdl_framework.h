/**
 * @file sdl_framework.h
 * @brief SDL2 game framework for window management and main loop
 * 
 * Requirements: 1.1, 1.2, 1.3, 1.4, 1.5, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6
 */

#ifndef SDL_FRAMEWORK_H
#define SDL_FRAMEWORK_H

#include <SDL.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Framework configuration structure
 */
typedef struct {
    int window_width;
    int window_height;
    const char* window_title;
    int target_fps;
} SDLFrameworkConfig;

/**
 * Framework state structure
 */
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int running;
    uint32_t frame_delay;
} SDLFramework;

/**
 * Initialize the SDL framework
 * @param fw Pointer to framework state
 * @param config Pointer to configuration
 * @return 0 on success, negative error code on failure
 */
int sdl_framework_init(SDLFramework* fw, const SDLFrameworkConfig* config);

/**
 * Release all SDL resources
 * @param fw Pointer to framework state
 */
void sdl_framework_cleanup(SDLFramework* fw);

/**
 * Process pending SDL events
 * @param fw Pointer to framework state
 * @param event Pointer to SDL_Event to store event data
 * @return 0 to continue running, non-zero to exit
 */
int sdl_framework_poll_events(SDLFramework* fw, SDL_Event* event);

/**
 * Begin frame rendering (clear screen)
 * @param fw Pointer to framework state
 */
void sdl_framework_begin_frame(SDLFramework* fw);

/**
 * End frame rendering (present to screen)
 * @param fw Pointer to framework state
 */
void sdl_framework_end_frame(SDLFramework* fw);

/**
 * Frame rate control delay
 * @param fw Pointer to framework state
 * @param frame_start Tick count at frame start
 */
void sdl_framework_delay(SDLFramework* fw, uint32_t frame_start);

#ifdef __cplusplus
}
#endif

#endif /* SDL_FRAMEWORK_H */
