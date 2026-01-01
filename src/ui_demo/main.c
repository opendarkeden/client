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

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    printf("SDL UI System Demo\n");
    printf("==================\n");
    printf("Click OK or Cancel, or press Enter/Escape to exit.\n\n");

    /* Initialize SDL framework */
    SDLFramework fw;
    SDLFrameworkConfig config = {
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .window_title = "SDL UI Demo",
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

    /* Initialize global resources (no sprite packs for this demo) */
    UI_GlobalResource resources;
    ui_global_resource_init(&resources, fw.renderer, &surface, NULL, NULL);

    /* Create dialog centered on screen */
    int dialog_x = (WINDOW_WIDTH - DIALOG_WIDTH) / 2;
    int dialog_y = (WINDOW_HEIGHT - DIALOG_HEIGHT) / 2;
    
    UI_Dialog* dialog = ui_dialog_create(
        dialog_x, dialog_y,
        DIALOG_WIDTH, DIALOG_HEIGHT,
        DIALOG_OK | DIALOG_CANCEL,
        on_dialog_action
    );

    if (!dialog) {
        fprintf(stderr, "Failed to create dialog\n");
        ui_global_resource_free(&resources);
        ui_surface_free(&surface);
        sdl_framework_cleanup(&fw);
        return 1;
    }

    ui_dialog_set_resources(dialog, &resources);
    ui_dialog_set_message(dialog, "Hello from SDL UI!");

    /* Initialize window manager */
    UI_WindowManager wm;
    ui_window_manager_init(&wm, &surface);

    /* Register dialog with window manager */
    ui_window_manager_register(&wm, ui_dialog_get_window(dialog));

    /* Show the dialog */
    ui_dialog_start(dialog);

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
        ui_surface_clear(&surface, 0x1a1a2eFF);

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
    ui_dialog_destroy(dialog);
    ui_window_manager_free(&wm);
    ui_global_resource_free(&resources);
    ui_surface_free(&surface);
    sdl_framework_cleanup(&fw);

    printf("Demo finished.\n");
    return 0;
}
