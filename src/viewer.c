/**
 * @file viewer.c
 * @brief Sprite viewer application implementation
 * 
 * Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6
 */

#include "viewer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Default colorkey for transparency (RGB565 value 0) */
#define DEFAULT_COLORKEY 0

/* Zoom limits */
#define MIN_ZOOM 0.25f
#define MAX_ZOOM 8.0f
#define ZOOM_STEP 0.25f

/**
 * Initialize sprite viewer
 * Requirement 7.1: Accept sprite pack file path
 * Requirement 7.2: Display first sprite centered on screen
 */
int viewer_init(SpriteViewer* viewer, const char* pack_file, 
                int width, int height) {
    if (!viewer || !pack_file) {
        return -1;
    }
    
    /* Initialize viewer state */
    memset(viewer, 0, sizeof(SpriteViewer));
    viewer->current_index = 0;
    viewer->zoom = 1.0f;
    viewer->show_info = 1;
    viewer->decoded_sprites = NULL;
    
    /* Initialize SDL framework */
    SDLFrameworkConfig config = {
        .window_width = width,
        .window_height = height,
        .window_title = "SDL Sprite Viewer",
        .target_fps = 60
    };
    
    if (sdl_framework_init(&viewer->framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        return -1;
    }
    
    /* Load sprite pack - Requirement 7.1 */
    if (spritepack_load(&viewer->pack, pack_file) != 0) {
        fprintf(stderr, "Error: Failed to load sprite pack: %s\n", pack_file);
        sdl_framework_cleanup(&viewer->framework);
        return -2;
    }
    
    printf("Loaded %d sprites from pack\n", viewer->pack.count);
    
    /* Handle empty pack */
    if (viewer->pack.count == 0) {
        fprintf(stderr, "Warning: Sprite pack is empty\n");
        return 0;
    }
    
    /* Allocate decoded sprites array */
    viewer->decoded_sprites = (DecodedSprite*)calloc(viewer->pack.count, 
                                                      sizeof(DecodedSprite));
    if (!viewer->decoded_sprites) {
        fprintf(stderr, "Error: Failed to allocate decoded sprites\n");
        spritepack_free(&viewer->pack);
        sdl_framework_cleanup(&viewer->framework);
        return -3;
    }
    
    /* Decode all sprites and create textures */
    for (uint16_t i = 0; i < viewer->pack.count; i++) {
        Sprite* sprite = spritepack_get(&viewer->pack, i);
        if (sprite && sprite->is_valid) {
            if (sprite_decode(sprite, &viewer->decoded_sprites[i], 
                             DEFAULT_COLORKEY) == 0) {
                decoded_sprite_create_texture(&viewer->decoded_sprites[i],
                                             viewer->framework.renderer);
            }
        }
    }
    
    return 0;
}

/**
 * Handle input events
 * Requirement 7.3: LEFT/RIGHT arrow keys navigate sprites
 * Requirement 7.4: UP/DOWN arrow keys zoom in/out
 * Requirement 7.6: ESC or window close exits
 */
void viewer_handle_input(SpriteViewer* viewer, SDL_Event* event) {
    if (!viewer || !event) {
        return;
    }
    
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            /* Requirement 7.3: Navigate sprites with LEFT/RIGHT */
            case SDLK_LEFT:
                if (viewer->pack.count > 0) {
                    viewer->current_index--;
                    if (viewer->current_index < 0) {
                        viewer->current_index = viewer->pack.count - 1;
                    }
                }
                break;
                
            case SDLK_RIGHT:
                if (viewer->pack.count > 0) {
                    viewer->current_index++;
                    if (viewer->current_index >= viewer->pack.count) {
                        viewer->current_index = 0;
                    }
                }
                break;
                
            /* Requirement 7.4: Zoom with UP/DOWN */
            case SDLK_UP:
                viewer->zoom += ZOOM_STEP;
                if (viewer->zoom > MAX_ZOOM) {
                    viewer->zoom = MAX_ZOOM;
                }
                break;
                
            case SDLK_DOWN:
                viewer->zoom -= ZOOM_STEP;
                if (viewer->zoom < MIN_ZOOM) {
                    viewer->zoom = MIN_ZOOM;
                }
                break;
                
            /* Toggle info display */
            case SDLK_i:
                viewer->show_info = !viewer->show_info;
                break;
                
            /* Requirement 7.6: ESC exits */
            case SDLK_ESCAPE:
                viewer->framework.running = 0;
                break;
                
            default:
                break;
        }
    }
}

/**
 * Render current frame
 * Requirement 7.2: Display sprite centered on screen
 * Requirement 7.5: Show current sprite index and total count
 */
void viewer_render(SpriteViewer* viewer) {
    if (!viewer) {
        return;
    }
    
    /* Begin frame */
    sdl_framework_begin_frame(&viewer->framework);
    
    /* Render current sprite if available */
    if (viewer->pack.count > 0 && viewer->decoded_sprites) {
        DecodedSprite* current = &viewer->decoded_sprites[viewer->current_index];
        
        if (current->texture && current->width > 0 && current->height > 0) {
            /* Get window size */
            int win_w, win_h;
            SDL_GetWindowSize(viewer->framework.window, &win_w, &win_h);
            
            /* Calculate scaled dimensions */
            int scaled_w = (int)(current->width * viewer->zoom);
            int scaled_h = (int)(current->height * viewer->zoom);
            
            /* Center sprite on screen - Requirement 7.2 */
            SDL_Rect dst_rect = {
                .x = (win_w - scaled_w) / 2,
                .y = (win_h - scaled_h) / 2,
                .w = scaled_w,
                .h = scaled_h
            };
            
            /* Render sprite */
            SDL_RenderCopy(viewer->framework.renderer, 
                          current->texture, NULL, &dst_rect);
        }
    }
    
    /* Requirement 7.5: Show sprite index and total count */
    /* Note: SDL2 doesn't have built-in text rendering, so we'll use
     * the window title to display info instead */
    if (viewer->show_info && viewer->pack.count > 0) {
        char title[256];
        DecodedSprite* current = &viewer->decoded_sprites[viewer->current_index];
        snprintf(title, sizeof(title), 
                "SDL Sprite Viewer - Sprite %d/%d - Size: %dx%d - Zoom: %.0f%%",
                viewer->current_index + 1, viewer->pack.count,
                current->width, current->height,
                viewer->zoom * 100);
        SDL_SetWindowTitle(viewer->framework.window, title);
    }
    
    /* End frame */
    sdl_framework_end_frame(&viewer->framework);
}

/**
 * Run main loop
 * Requirement 2.1: Main loop runs until quit requested
 */
void viewer_run(SpriteViewer* viewer) {
    if (!viewer) {
        return;
    }
    
    SDL_Event event;
    
    /* Main loop */
    while (viewer->framework.running) {
        uint32_t frame_start = SDL_GetTicks();
        
        /* Process events */
        while (sdl_framework_poll_events(&viewer->framework, &event)) {
            viewer_handle_input(viewer, &event);
        }
        
        /* Render */
        viewer_render(viewer);
        
        /* Frame rate control */
        sdl_framework_delay(&viewer->framework, frame_start);
    }
}

/**
 * Cleanup viewer resources
 * Requirement 7.6: Exit cleanly
 */
void viewer_cleanup(SpriteViewer* viewer) {
    if (!viewer) {
        return;
    }
    
    /* Free decoded sprites */
    if (viewer->decoded_sprites) {
        for (uint16_t i = 0; i < viewer->pack.count; i++) {
            decoded_sprite_free(&viewer->decoded_sprites[i]);
        }
        free(viewer->decoded_sprites);
        viewer->decoded_sprites = NULL;
    }
    
    /* Free sprite pack */
    spritepack_free(&viewer->pack);
    
    /* Cleanup SDL framework */
    sdl_framework_cleanup(&viewer->framework);
    
    /* Reset state */
    viewer->current_index = 0;
    viewer->zoom = 1.0f;
    viewer->show_info = 0;
}
