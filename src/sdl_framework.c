/**
 * @file sdl_framework.c
 * @brief SDL2 game framework implementation
 * 
 * Requirements: 1.1, 1.2, 1.3, 1.4, 1.5, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6
 */

#include "sdl_framework.h"
#include <stdio.h>

/**
 * Initialize the SDL framework
 * Req 1.1: Initialize SDL2 with video subsystem
 * Req 1.2: Create window with configurable dimensions
 * Req 1.3: Create hardware-accelerated renderer
 * Req 1.4: Return error code and log on failure
 */
int sdl_framework_init(SDLFramework* fw, const SDLFrameworkConfig* config) {
    if (!fw || !config) {
        fprintf(stderr, "Error: NULL pointer passed to sdl_framework_init\n");
        return -1;
    }

    /* Initialize framework state */
    fw->window = NULL;
    fw->renderer = NULL;
    fw->running = 0;
    fw->frame_delay = 0;

    /* Req 1.1: Initialize SDL2 with video subsystem */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    /* Req 1.2: Create window with configurable width and height */
    fw->window = SDL_CreateWindow(
        config->window_title ? config->window_title : "SDL Framework",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config->window_width > 0 ? config->window_width : 800,
        config->window_height > 0 ? config->window_height : 600,
        SDL_WINDOW_SHOWN
    );

    if (!fw->window) {
        fprintf(stderr, "Error: SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -2;
    }

    /* Req 1.3: Create hardware-accelerated renderer */
    fw->renderer = SDL_CreateRenderer(
        fw->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!fw->renderer) {
        fprintf(stderr, "Error: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(fw->window);
        fw->window = NULL;
        SDL_Quit();
        return -3;
    }

    /* Req 2.6: Support configurable target frame rate */
    if (config->target_fps > 0) {
        fw->frame_delay = 1000 / config->target_fps;
    } else {
        fw->frame_delay = 16; /* Default ~60 FPS */
    }

    /* Req 2.1: Set running flag for main loop */
    fw->running = 1;

    return 0;
}

/**
 * Release all SDL resources
 * Req 1.5: Properly release all SDL resources on exit
 */
void sdl_framework_cleanup(SDLFramework* fw) {
    if (!fw) {
        return;
    }

    if (fw->renderer) {
        SDL_DestroyRenderer(fw->renderer);
        fw->renderer = NULL;
    }

    if (fw->window) {
        SDL_DestroyWindow(fw->window);
        fw->window = NULL;
    }

    fw->running = 0;

    SDL_Quit();
}

/**
 * Process pending SDL events
 * Req 2.2: Process all pending SDL events
 * Req 2.3: Set flag to exit on quit event
 * 
 * @return 1 if there's an event to process, 0 if no more events
 */
int sdl_framework_poll_events(SDLFramework* fw, SDL_Event* event) {
    if (!fw || !event) {
        return 0;
    }

    /* Req 2.2: Process pending SDL events */
    if (SDL_PollEvent(event)) {
        /* Req 2.3: Set flag to exit on quit event */
        if (event->type == SDL_QUIT) {
            fw->running = 0;
        }
        return 1;
    }

    return 0;
}

/**
 * Begin frame rendering
 * Req 2.4: Clear the render target at frame start
 */
void sdl_framework_begin_frame(SDLFramework* fw) {
    if (!fw || !fw->renderer) {
        return;
    }

    /* Set clear color to black */
    SDL_SetRenderDrawColor(fw->renderer, 0, 0, 0, 255);
    
    /* Req 2.4: Clear the render target */
    SDL_RenderClear(fw->renderer);
}

/**
 * End frame rendering
 * Req 2.5: Present rendered content to screen
 */
void sdl_framework_end_frame(SDLFramework* fw) {
    if (!fw || !fw->renderer) {
        return;
    }

    /* Req 2.5: Present the rendered content */
    SDL_RenderPresent(fw->renderer);
}

/**
 * Frame rate control delay
 * Req 2.6: Support configurable target frame rate
 */
void sdl_framework_delay(SDLFramework* fw, uint32_t frame_start) {
    if (!fw) {
        return;
    }

    uint32_t frame_time = SDL_GetTicks() - frame_start;
    
    if (frame_time < fw->frame_delay) {
        SDL_Delay(fw->frame_delay - frame_time);
    }
}
