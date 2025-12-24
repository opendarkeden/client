/**
 * @file main.c
 * @brief SDL Animation Viewer - Tool for viewing and testing animations
 * 
 * Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 6.3, 6.4
 */

#include "sdl_framework.h"
#include "spritepack.h"
#include "animation.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Window defaults */
#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

/* Animation defaults */
#define DEFAULT_FRAME_RATE 10  /* Frames per second for animation playback */
#define MIN_ZOOM 0.25f
#define MAX_ZOOM 8.0f
#define ZOOM_STEP 0.25f

/* BltType names for display */
static const char* BLT_TYPE_NAMES[] = {
    "NORMAL",
    "EFFECT",
    "SHADOW",
    "SCREEN"
};

/* Direction names for display */
static const char* DIR_NAMES[] = {
    "LEFT",
    "LEFT-DOWN",
    "DOWN",
    "RIGHT-DOWN",
    "RIGHT",
    "RIGHT-UP",
    "UP",
    "LEFT-UP"
};

/**
 * Animation viewer state
 */
typedef struct {
    SDLFramework framework;
    SpritePack pack;
    DecodedSprite* decoded_sprites;
    AnimObject anim;
    
    int current_sprite;     /* Current base sprite index */
    int max_frames;         /* Number of frames in current animation */
    float zoom;
    int show_info;
    int playing;            /* Animation playback state */
    uint32_t last_frame_time;
    uint32_t frame_interval; /* Milliseconds between frames */
    BltType current_blt;
} AnimationViewer;

/**
 * Print usage information
 * Requirement 5.1: Accept sprite pack file path as command line argument
 */
static void print_usage(const char* program) {
    printf("SDL Animation Viewer\n");
    printf("====================\n\n");
    printf("Usage: %s <sprite_pack_file.spk> [max_frames]\n", program);
    printf("\nArguments:\n");
    printf("  sprite_pack_file.spk  Path to sprite pack file\n");
    printf("  max_frames            Number of frames per animation (default: 8)\n");
    printf("\nControls:\n");
    printf("  LEFT/RIGHT  - Navigate to previous/next animation\n");
    printf("  UP/DOWN     - Change direction (8 directions)\n");
    printf("  SPACE       - Toggle animation playback (play/pause)\n");
    printf("  1-4         - Switch BltType (1=Normal, 2=Effect, 3=Shadow, 4=Screen)\n");
    printf("  +/-         - Zoom in/out\n");
    printf("  I           - Toggle info display\n");
    printf("  ESC         - Exit\n");
}

/**
 * Initialize animation viewer
 * Requirement 5.1: Accept sprite pack file path
 * Requirement 5.2: Display first animation centered on screen
 */
static int viewer_init(AnimationViewer* viewer, const char* pack_file, 
                       int width, int height, int max_frames) {
    if (!viewer || !pack_file) {
        return -1;
    }
    
    /* Initialize viewer state */
    memset(viewer, 0, sizeof(AnimationViewer));
    viewer->current_sprite = 0;
    viewer->max_frames = max_frames > 0 ? max_frames : 8;
    viewer->zoom = 1.0f;
    viewer->show_info = 1;
    viewer->playing = 1;  /* Start playing by default */
    viewer->decoded_sprites = NULL;
    viewer->last_frame_time = 0;
    viewer->frame_interval = 1000 / DEFAULT_FRAME_RATE;
    viewer->current_blt = BLT_NORMAL;
    
    /* Initialize SDL framework */
    SDLFrameworkConfig config = {
        .window_width = width,
        .window_height = height,
        .window_title = "SDL Animation Viewer",
        .target_fps = 60
    };
    
    if (sdl_framework_init(&viewer->framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        return -1;
    }
    
    /* Load sprite pack - Requirement 5.1 */
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
    
    /* Initialize animation object - Requirement 5.2 */
    anim_object_init(&viewer->anim);
    anim_object_set(&viewer->anim, 0, 0, 0, DIR_DOWN, 0);
    anim_object_set_frame(&viewer->anim, 0, (uint8_t)viewer->max_frames);
    viewer->anim.frame.blt_type = BLT_NORMAL;
    
    return 0;
}

/**
 * Handle input events
 * Requirement 5.3: LEFT/RIGHT navigate animations
 * Requirement 5.4: SPACE toggles playback
 * Requirement 5.6: 1-4 keys switch BltTypes
 * Requirement 5.8: ESC exits
 * Requirement 6.3: UP/DOWN change direction
 */
static void viewer_handle_input(AnimationViewer* viewer, SDL_Event* event) {
    if (!viewer || !event) {
        return;
    }
    
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            /* Requirement 5.3: Navigate animations with LEFT/RIGHT */
            case SDLK_LEFT:
                if (viewer->pack.count > 0) {
                    viewer->current_sprite -= viewer->max_frames;
                    if (viewer->current_sprite < 0) {
                        /* Wrap to last animation */
                        int total_anims = viewer->pack.count / viewer->max_frames;
                        viewer->current_sprite = (total_anims - 1) * viewer->max_frames;
                        if (viewer->current_sprite < 0) {
                            viewer->current_sprite = 0;
                        }
                    }
                    anim_object_set(&viewer->anim, (SpriteID)viewer->current_sprite,
                                   0, 0, viewer->anim.direction, 0);
                    anim_object_set_frame(&viewer->anim, 0, (uint8_t)viewer->max_frames);
                }
                break;
                
            case SDLK_RIGHT:
                if (viewer->pack.count > 0) {
                    viewer->current_sprite += viewer->max_frames;
                    if (viewer->current_sprite >= viewer->pack.count) {
                        viewer->current_sprite = 0;
                    }
                    anim_object_set(&viewer->anim, (SpriteID)viewer->current_sprite,
                                   0, 0, viewer->anim.direction, 0);
                    anim_object_set_frame(&viewer->anim, 0, (uint8_t)viewer->max_frames);
                }
                break;
                
            /* Requirement 6.3: Change direction with UP/DOWN */
            case SDLK_UP:
                {
                    uint8_t new_dir = (viewer->anim.direction + 1) % DIR_MAX;
                    anim_object_set_direction(&viewer->anim, new_dir);
                }
                break;
                
            case SDLK_DOWN:
                {
                    uint8_t new_dir = (viewer->anim.direction + DIR_MAX - 1) % DIR_MAX;
                    anim_object_set_direction(&viewer->anim, new_dir);
                }
                break;
                
            /* Requirement 5.4: Toggle playback with SPACE */
            case SDLK_SPACE:
                viewer->playing = !viewer->playing;
                break;
                
            /* Requirement 5.6: Switch BltType with 1-4 keys */
            case SDLK_1:
                viewer->current_blt = BLT_NORMAL;
                viewer->anim.frame.blt_type = BLT_NORMAL;
                break;
                
            case SDLK_2:
                viewer->current_blt = BLT_EFFECT;
                viewer->anim.frame.blt_type = BLT_EFFECT;
                break;
                
            case SDLK_3:
                viewer->current_blt = BLT_SHADOW;
                viewer->anim.frame.blt_type = BLT_SHADOW;
                break;
                
            case SDLK_4:
                viewer->current_blt = BLT_SCREEN;
                viewer->anim.frame.blt_type = BLT_SCREEN;
                break;
                
            /* Zoom controls */
            case SDLK_EQUALS:
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                viewer->zoom += ZOOM_STEP;
                if (viewer->zoom > MAX_ZOOM) {
                    viewer->zoom = MAX_ZOOM;
                }
                break;
                
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                viewer->zoom -= ZOOM_STEP;
                if (viewer->zoom < MIN_ZOOM) {
                    viewer->zoom = MIN_ZOOM;
                }
                break;
                
            /* Toggle info display */
            case SDLK_i:
                viewer->show_info = !viewer->show_info;
                break;
                
            /* Requirement 5.8: ESC exits */
            case SDLK_ESCAPE:
                viewer->framework.running = 0;
                break;
                
            default:
                break;
        }
    }
}

/**
 * Update animation state
 * Requirement 5.5: Advance frames at configurable frame rate
 */
static void viewer_update(AnimationViewer* viewer) {
    if (!viewer || !viewer->playing) {
        return;
    }
    
    uint32_t current_time = SDL_GetTicks();
    
    /* Requirement 5.5: Advance frames at frame rate */
    if (current_time - viewer->last_frame_time >= viewer->frame_interval) {
        anim_object_next_frame(&viewer->anim);
        viewer->last_frame_time = current_time;
    }
}

/**
 * Render current frame
 * Requirement 5.2: Display animation centered on screen
 * Requirement 5.7: Show frame index, max frames, BltType, direction
 * Requirement 6.4: Show current direction
 */
static void viewer_render(AnimationViewer* viewer) {
    if (!viewer) {
        return;
    }
    
    /* Begin frame */
    sdl_framework_begin_frame(&viewer->framework);
    
    /* Render current animation frame if available */
    if (viewer->pack.count > 0 && viewer->decoded_sprites) {
        SpriteID sprite_id = anim_object_get_sprite(&viewer->anim);
        
        if (sprite_id < viewer->pack.count) {
            DecodedSprite* current = &viewer->decoded_sprites[sprite_id];
            
            if (current->texture && current->width > 0 && current->height > 0) {
                /* Get window size */
                int win_w, win_h;
                SDL_GetWindowSize(viewer->framework.window, &win_w, &win_h);
                
                /* Calculate scaled dimensions */
                int scaled_w = (int)(current->width * viewer->zoom);
                int scaled_h = (int)(current->height * viewer->zoom);
                
                /* Center sprite on screen - Requirement 5.2 */
                SDL_Rect dst_rect = {
                    .x = (win_w - scaled_w) / 2,
                    .y = (win_h - scaled_h) / 2,
                    .w = scaled_w,
                    .h = scaled_h
                };
                
                /* Apply blend mode based on BltType */
                anim_set_blend_mode(current->texture, viewer->current_blt);
                
                /* Render sprite */
                SDL_RenderCopy(viewer->framework.renderer, 
                              current->texture, NULL, &dst_rect);
            }
        }
    }
    
    /* Requirement 5.7, 6.4: Show frame info and direction in window title */
    if (viewer->show_info && viewer->pack.count > 0) {
        char title[512];
        uint8_t current_frame = anim_frame_get(&viewer->anim.frame);
        const char* blt_name = (viewer->current_blt < 4) ? 
                               BLT_TYPE_NAMES[viewer->current_blt] : "UNKNOWN";
        const char* dir_name = (viewer->anim.direction < DIR_MAX) ?
                               DIR_NAMES[viewer->anim.direction] : "UNKNOWN";
        const char* play_state = viewer->playing ? "PLAYING" : "PAUSED";
        
        snprintf(title, sizeof(title), 
                "Animation Viewer - Sprite %d | Frame %d/%d | BltType: %s | Dir: %s | Zoom: %.0f%% | %s",
                viewer->current_sprite,
                current_frame + 1, viewer->max_frames,
                blt_name,
                dir_name,
                viewer->zoom * 100,
                play_state);
        SDL_SetWindowTitle(viewer->framework.window, title);
    }
    
    /* End frame */
    sdl_framework_end_frame(&viewer->framework);
}

/**
 * Run main loop
 */
static void viewer_run(AnimationViewer* viewer) {
    if (!viewer) {
        return;
    }
    
    SDL_Event event;
    
    /* Initialize frame timing */
    viewer->last_frame_time = SDL_GetTicks();
    
    /* Main loop */
    while (viewer->framework.running) {
        uint32_t frame_start = SDL_GetTicks();
        
        /* Process events */
        while (sdl_framework_poll_events(&viewer->framework, &event)) {
            viewer_handle_input(viewer, &event);
        }
        
        /* Update animation */
        viewer_update(viewer);
        
        /* Render */
        viewer_render(viewer);
        
        /* Frame rate control */
        sdl_framework_delay(&viewer->framework, frame_start);
    }
}

/**
 * Cleanup viewer resources
 * Requirement 5.8: Exit cleanly
 */
static void viewer_cleanup(AnimationViewer* viewer) {
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
}

/**
 * Main entry point
 * Requirement 5.1: Accept sprite pack file path
 * Requirement 5.8: Exit cleanly
 */
int main(int argc, char* argv[]) {
    /* Requirement 5.1: Accept sprite pack file path as command line argument */
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* pack_file = argv[1];
    int max_frames = 8;  /* Default frames per animation */
    
    /* Optional: parse max_frames argument */
    if (argc >= 3) {
        max_frames = atoi(argv[2]);
        if (max_frames <= 0) {
            max_frames = 8;
        }
    }
    
    AnimationViewer viewer;

    printf("Loading sprite pack: %s\n", pack_file);
    printf("Frames per animation: %d\n", max_frames);

    /* Initialize viewer with sprite pack */
    int result = viewer_init(&viewer, pack_file, DEFAULT_WIDTH, DEFAULT_HEIGHT, max_frames);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize viewer (error: %d)\n", result);
        return 1;
    }

    printf("Viewer initialized successfully\n");
    printf("Controls:\n");
    printf("  LEFT/RIGHT  - Navigate animations\n");
    printf("  UP/DOWN     - Change direction\n");
    printf("  SPACE       - Play/Pause\n");
    printf("  1-4         - Switch BltType\n");
    printf("  +/-         - Zoom\n");
    printf("  ESC         - Exit\n");

    /* Run main loop */
    viewer_run(&viewer);

    /* Requirement 5.8: Exit cleanly */
    viewer_cleanup(&viewer);

    printf("Viewer closed\n");
    return 0;
}
