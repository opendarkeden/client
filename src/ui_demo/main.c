/**
 * @file main.c
 * @brief SDL UI System Demo Application
 * 
 * Demonstrates the pure C UI component system with a simple dialog.
 * 
 * Requirements: 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7
 */

#include <stdio.h>
#include <stdlib.h>

#include "sdl_framework.h"
#include "ui_surface.h"
#include "ui_window.h"
#include "ui_window_manager.h"
#include "ui_event_router.h"
#include "ui_dialog.h"
#include "ui_global_resource.h"
#include "ui_title.h"

/* Window dimensions */
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

/* Dialog dimensions */
#define DIALOG_WIDTH  300
#define DIALOG_HEIGHT 150

/* Application state */
static int g_running = 1;

/* ============================================================================
 * Dialog callback
 * ============================================================================ */

static void on_dialog_action(UI_Dialog* dialog, int exec_id) {
    (void)dialog;

    switch (exec_id) {
        case DIALOG_EXECID_OK:
            printf("OK clicked!\n");
            g_running = 0;
            break;
        case DIALOG_EXECID_CANCEL:
            printf("Cancel clicked!\n");
            g_running = 0;
            break;
        default:
            break;
    }
}

/* ============================================================================
 * Main
 * ============================================================================ */

/* Main */
/* ============================================================================
 * Main
 * ============================================================================ */

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    printf("SDL UI System Demo - Title Screen\n");
    printf("=================================\n");

    /* Initialize SDL framework */
    SDLFramework fw;
    SDLFrameworkConfig config = {
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .window_title = "DarkEden Title Demo",
        .target_fps = 60
    };

    if (sdl_framework_init(&fw, &config) != 0) {
        fprintf(stderr, "Failed to initialize SDL framework\n");
        return 1;
    }

    /* Create UI surface (backbuffer) */
    UI_Surface surface;
    if (ui_surface_init(&surface, fw.renderer, WINDOW_WIDTH, WINDOW_HEIGHT) != 0) {
        fprintf(stderr, "Failed to create UI surface\n");
        sdl_framework_cleanup(&fw);
        return 1;
    }

    /* Initialize global resources */
    UI_GlobalResource resources;
    /* Pass NULL so it doesn't try to load fallback dialog/button packs we don't need for Title yet, 
       or we could pass generic ones if we had them. Title manages its own packs. */
    ui_global_resource_init(&resources, fw.renderer, &surface, NULL, NULL);

    /* Create Title Window */
    UI_Window* title_window = ui_title_create(&resources);

    if (!title_window) {
        fprintf(stderr, "Failed to create title window. Check if data/spk/Title.spk exists.\n");
        ui_global_resource_free(&resources);
        ui_surface_free(&surface);
        sdl_framework_cleanup(&fw);
        return 1;
    }

    /* Initialize window manager */
    UI_WindowManager wm;
    ui_window_manager_init(&wm, &surface);

    /* Register title with window manager */
    ui_window_manager_register(&wm, title_window);
    ui_window_set_visible(title_window, 1);
    
    // ui_dialog_start is not needed for generic window, just visibility

    /* Main loop */
    while (g_running) {
        uint32_t frame_start = SDL_GetTicks();

        /* Process SDL events */
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                g_running = 0;
                break;
            }

            /* Translate and dispatch UI events */
            UI_Event ui_event;
            if (ui_event_translate(&sdl_event, &ui_event)) {
                ui_window_manager_dispatch_event(&wm, &ui_event);
            }
        }

        /* Clear surface */
        ui_surface_clear(&surface, 0x000000FF);

        /* Process and render windows */
        ui_window_manager_process(&wm);
        ui_window_manager_render(&wm);

        /* Present to screen */
        sdl_framework_begin_frame(&fw);
        SDL_RenderCopy(fw.renderer, ui_surface_get_texture(&surface), NULL, NULL);
        sdl_framework_end_frame(&fw);

        /* Frame rate control */
        sdl_framework_delay(&fw, frame_start);
    }

    /* Cleanup */
    // free(title_window); // Should be freed by specific destroy or cleanup
    // ideally title_window has a destroy function or we cast to title to free
    // For now simple leak is fine in demo main, or we rely on OS cleanup
    
    ui_window_manager_free(&wm);
    ui_global_resource_free(&resources);
    ui_surface_free(&surface);
    sdl_framework_cleanup(&fw);

    printf("Demo finished.\n");
    return 0;
}
