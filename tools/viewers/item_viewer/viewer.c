/**
 * @file viewer.c
 * @brief Item viewer application implementation
 */

#include "viewer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Default colorset (basic palette) */
#define DEFAULT_COLORSET 0

/* Zoom limits */
#define MIN_ZOOM 0.25f
#define MAX_ZOOM 8.0f
#define ZOOM_STEP 0.25f

/**
 * Convert RGB565 to RGBA32
 */
static void rgb565_to_rgba32(const uint16_t* rgb565, uint32_t* rgba32,
                             int width, int height) {
    for (int i = 0; i < width * height; i++) {
        uint16_t pixel = rgb565[i];

        /* Extract RGB565 components */
        uint8_t r = ((pixel >> 11) & 0x1F) << 3;
        uint8_t g = ((pixel >> 5) & 0x3F) << 2;
        uint8_t b = (pixel & 0x1F) << 3;

        /* Set RGBA32 (fully opaque) */
        rgba32[i] = (255 << 24) | (r << 16) | (g << 8) | b;
    }
}

/**
 * Initialize item viewer
 */
int viewer_init(ItemViewer* viewer, const char* pack_file,
                int width, int height) {
    if (!viewer || !pack_file) {
        return -1;
    }

    /* Initialize viewer state */
    memset(viewer, 0, sizeof(ItemViewer));
    viewer->current_index = 0;
    viewer->zoom = 1.0f;
    viewer->show_info = 1;
    viewer->decoded_sprites = NULL;

    /* Initialize SDL framework */
    SDLFrameworkConfig config = {
        .window_width = width,
        .window_height = height,
        .window_title = "Item Viewer",
        .target_fps = 60
    };

    if (sdl_framework_init(&viewer->framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        return -1;
    }

    /* Initialize and load indexed sprite pack */
    index_spritepack_init(&viewer->pack);

    if (!index_spritepack_load(&viewer->pack, pack_file)) {
        fprintf(stderr, "Error: Failed to load sprite pack: %s\n", pack_file);
        sdl_framework_cleanup(&viewer->framework);
        return -2;
    }

    uint16_t count = index_spritepack_get_size(&viewer->pack);
    printf("Loaded %d items from pack\n", count);

    /* Handle empty pack */
    if (count == 0) {
        fprintf(stderr, "Warning: Sprite pack is empty\n");
        return 0;
    }

    /* Allocate decoded sprites array */
    viewer->decoded_sprites = (DecodedSprite*)calloc(count, sizeof(DecodedSprite));
    if (!viewer->decoded_sprites) {
        fprintf(stderr, "Error: Failed to allocate decoded sprites\n");
        index_spritepack_release(&viewer->pack);
        sdl_framework_cleanup(&viewer->framework);
        return -3;
    }

    /* Decode all indexed sprites and create textures */
    for (uint16_t i = 0; i < count; i++) {
        IndexSprite* isprite = index_spritepack_get(&viewer->pack, i);
        if (isprite && index_sprite_is_init(isprite)) {
            uint16_t w = index_sprite_get_width(isprite);
            uint16_t h = index_sprite_get_height(isprite);

            viewer->decoded_sprites[i].width = w;
            viewer->decoded_sprites[i].height = h;

            /* Allocate RGB565 buffer */
            uint16_t* rgb565_buffer = (uint16_t*)calloc(w * h, sizeof(uint16_t));
            if (!rgb565_buffer) {
                continue;
            }

            /* Render indexed sprite to RGB565 buffer */
            index_sprite_blt_colorset(isprite, rgb565_buffer, w * 2, DEFAULT_COLORSET);

            /* Convert to RGBA32 */
            viewer->decoded_sprites[i].pixels = (uint32_t*)calloc(w * h, sizeof(uint32_t));
            if (viewer->decoded_sprites[i].pixels) {
                rgb565_to_rgba32(rgb565_buffer, viewer->decoded_sprites[i].pixels, w, h);

                /* Create SDL texture */
                decoded_sprite_create_texture(&viewer->decoded_sprites[i],
                                             viewer->framework.renderer);
            }

            free(rgb565_buffer);
        }
    }

    return 0;
}

/**
 * Handle input events
 */
void viewer_handle_input(ItemViewer* viewer, SDL_Event* event) {
    if (!viewer || !event) {
        return;
    }

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_LEFT:
                /* Navigate to previous item */
                if (viewer->current_index > 0) {
                    viewer->current_index--;
                }
                break;

            case SDLK_RIGHT:
                /* Navigate to next item */
                if (viewer->current_index < index_spritepack_get_size(&viewer->pack) - 1) {
                    viewer->current_index++;
                }
                break;

            case SDLK_UP:
                /* Zoom in */
                viewer->zoom += ZOOM_STEP;
                if (viewer->zoom > MAX_ZOOM) {
                    viewer->zoom = MAX_ZOOM;
                }
                break;

            case SDLK_DOWN:
                /* Zoom out */
                viewer->zoom -= ZOOM_STEP;
                if (viewer->zoom < MIN_ZOOM) {
                    viewer->zoom = MIN_ZOOM;
                }
                break;

            case SDLK_i:
                /* Toggle info display */
                viewer->show_info = !viewer->show_info;
                break;

            case SDLK_ESCAPE:
                /* Exit */
                viewer->framework.running = 0;
                break;
        }
    }
}

/**
 * Render current frame
 */
void viewer_render(ItemViewer* viewer) {
    if (!viewer) {
        return;
    }

    /* Clear screen */
    sdl_framework_begin_frame(&viewer->framework);

    /* Get current item */
    if (viewer->current_index >= 0 && viewer->current_index < index_spritepack_get_size(&viewer->pack)) {
        DecodedSprite* sprite = &viewer->decoded_sprites[viewer->current_index];
        if (sprite && sprite->texture) {
            /* Calculate centered position with zoom */
            int window_width, window_height;
            SDL_GetWindowSize(viewer->framework.window, &window_width, &window_height);

            int sprite_width = (int)(sprite->width * viewer->zoom);
            int sprite_height = (int)(sprite->height * viewer->zoom);

            int x = (window_width - sprite_width) / 2;
            int y = (window_height - sprite_height) / 2;

            /* Render sprite */
            SDL_Rect dest_rect = { x, y, sprite_width, sprite_height };
            SDL_RenderCopy(viewer->framework.renderer, sprite->texture, NULL, &dest_rect);
        }
    }

    sdl_framework_end_frame(&viewer->framework);
}

/**
 * Run main loop
 */
void viewer_run(ItemViewer* viewer) {
    if (!viewer) {
        return;
    }

    SDL_Event event;
    while (viewer->framework.running) {
        /* Process events */
        while (sdl_framework_poll_events(&viewer->framework, &event)) {
            viewer_handle_input(viewer, &event);
        }

        /* Render */
        viewer_render(viewer);

        /* Display info in terminal */
        if (viewer->show_info) {
            printf("\rItem %d/%d | Zoom: %.2fx                        ",
                   viewer->current_index + 1,
                   index_spritepack_get_size(&viewer->pack),
                   viewer->zoom);
            fflush(stdout);
        }

        /* Frame rate control */
        uint32_t frame_start = SDL_GetTicks();
        sdl_framework_delay(&viewer->framework, frame_start);
    }

    if (viewer->show_info) {
        printf("\n");
    }
}

/**
 * Cleanup viewer resources
 */
void viewer_cleanup(ItemViewer* viewer) {
    if (!viewer) {
        return;
    }

    /* Free decoded sprites */
    if (viewer->decoded_sprites) {
        uint16_t count = index_spritepack_get_size(&viewer->pack);
        for (uint16_t i = 0; i < count; i++) {
            decoded_sprite_free(&viewer->decoded_sprites[i]);
        }
        free(viewer->decoded_sprites);
        viewer->decoded_sprites = NULL;
    }

    /* Free indexed sprite pack */
    index_spritepack_release(&viewer->pack);

    /* Cleanup SDL framework */
    sdl_framework_cleanup(&viewer->framework);
}
