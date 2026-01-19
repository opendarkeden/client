/**
 * @file main.c
 * @brief Creature Animation Viewer - Tool for viewing creature animations
 *
 * This tool loads {base}.ispk and {base}.cfk to display creature animations
 *
 * File Naming Convention:
 * - {BaseName}.ispk      - Indexed sprite pack (creature images)
 * - {BaseName}.cfpk      - Creature frame pack (animation frames)
 */

#include "engine/sprite/include/sdl_framework.h"
#include "engine/sprite/include/index_spritepack.h"
#include "engine/sprite/include/framepack.h"
#include "engine/sprite/include/colorset.h"
#include "engine/sprite/include/sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600
#define DEFAULT_FRAME_RATE 10
#define MAX_PATH_LEN 512

/* Action names for display */
static const char* ACTION_NAMES[] = {
    "STAND", "WALK", "RUN", "ATTACK", "DAMAGED", "DIE",
    "SKILL1", "SKILL2", "SKILL3", "SKILL4", "SKILL5", "SKILL6", "SKILL7", "SKILL8"
};
#define MAX_ACTION_NAMES 14

/* Direction names for display */
static const char* DIR_NAMES[] = {
    "S", "SW", "W", "NW", "N", "NE", "E", "SE"
};

/**
 * Creature viewer state
 */
typedef struct {
    SDLFramework framework;

    /* Resource packs */
    IndexSpritePack ispk;
    CreatureFramePack cfpk;

    /* State */
    int creature_type;      /* Current creature type index */
    int max_creature_types; /* Total number of creature types */
    int current_action;     /* Current action index */
    int max_actions;        /* Number of actions for current creature */
    int current_direction;  /* Current direction (0-7) */
    int current_frame;      /* Current frame index */
    int max_frames;         /* Number of frames for current action/direction */

    /* Decoded sprite cache */
    DecodedSprite* current_sprite;

    /* Display options */
    int show_info;
    int playing;            /* Animation playback state */
    int current_colorset;   /* Current ColorSet index */

    /* Timing */
    uint32_t last_frame_time;
    uint32_t frame_interval;
} CreatureViewer;

/* Action names for display */
static const char* get_action_name(int action) {
    if (action >= 0 && action < MAX_ACTION_NAMES) {
        return ACTION_NAMES[action];
    }
    return "UNKNOWN";
}

/**
 * Print usage information
 */
static void print_usage(const char* program) {
    printf("Creature Animation Viewer\n");
    printf("=========================\n\n");
    printf("Usage: %s <base_path>\n", program);
    printf("\nArguments:\n");
    printf("  base_path   Base path for resource files (without extension)\n");
    printf("              Example: DarkEden/Data/Image/Creature\n");
    printf("\nThe viewer will automatically load:\n");
    printf("  {base_path}.ispk  - Indexed sprite pack\n");
    printf("  {base_path}.cfpk  - Creature frame pack\n");
    printf("\nControls:\n");
    printf("  LEFT/RIGHT  - Navigate to previous/next Creature type\n");
    printf("  UP/DOWN     - Change Action (STAND, WALK, ATTACK, etc.)\n");
    printf("  1-8         - Change Direction\n");
    printf("  SPACE       - Toggle animation playback\n");
    printf("  C           - Cycle through ColorSets\n");
    printf("  I           - Toggle info display\n");
    printf("  ESC         - Exit\n");
}

/**
 * Update frame data for current creature/action/direction
 */
static void viewer_update_frame_info(CreatureViewer* viewer) {
    if (!viewer) return;

    /* Get creature type data */
    ActionArray* actions = creature_framepack_get(&viewer->cfpk, viewer->creature_type);
    if (!actions) {
        viewer->max_actions = 0;
        viewer->max_frames = 0;
        return;
    }

    viewer->max_actions = action_array_size(actions);

    if (viewer->current_action >= viewer->max_actions) {
        viewer->current_action = 0;
    }

    DirectionArray* dirs = action_array_get(actions, viewer->current_action);
    if (!dirs) {
        viewer->max_frames = 0;
        return;
    }

    if (viewer->current_direction >= 8) {
        viewer->current_direction = 0;
    }

    FrameArray* frames = direction_array_get(dirs, viewer->current_direction);
    if (!frames) {
        viewer->max_frames = 0;
        return;
    }

    viewer->max_frames = frame_array_size(frames);

    /* Clamp current frame */
    if (viewer->current_frame >= viewer->max_frames) {
        viewer->current_frame = 0;
    }
}

/**
 * Decode and cache current sprite
 */
static void viewer_decode_current_sprite(CreatureViewer* viewer) {
    if (!viewer) return;

    /* Free previous sprite */
    if (viewer->current_sprite) {
        decoded_sprite_free(viewer->current_sprite);
        free(viewer->current_sprite);
        viewer->current_sprite = NULL;
    }

    /* Get current frame */
    Frame* frame = creature_framepack_get_frame(&viewer->cfpk,
                                                  viewer->creature_type,
                                                  viewer->current_action,
                                                  viewer->current_direction,
                                                  viewer->current_frame);
    if (!frame) return;

    /* Get indexed sprite */
    IndexSprite* isprite = index_spritepack_get(&viewer->ispk, frame->sprite_id);
    if (!isprite || !index_sprite_is_init(isprite)) return;

    uint16_t w = index_sprite_get_width(isprite);
    uint16_t h = index_sprite_get_height(isprite);

    /* Allocate decoded sprite */
    viewer->current_sprite = (DecodedSprite*)calloc(1, sizeof(DecodedSprite));
    if (!viewer->current_sprite) return;

    viewer->current_sprite->width = w;
    viewer->current_sprite->height = h;

    /* Allocate RGB565 buffer */
    uint16_t* rgb565_buffer = (uint16_t*)calloc(w * h, sizeof(uint16_t));
    if (!rgb565_buffer) {
        free(viewer->current_sprite);
        viewer->current_sprite = NULL;
        return;
    }

    /* Render indexed sprite to RGB565 buffer */
    index_sprite_blt_colorset(isprite, rgb565_buffer, w * 2, viewer->current_colorset);

    /* Convert to RGBA32 */
    viewer->current_sprite->pixels = (uint32_t*)calloc(w * h, sizeof(uint32_t));
    if (viewer->current_sprite->pixels) {
        for (int i = 0; i < w * h; i++) {
            uint16_t pixel = rgb565_buffer[i];
            uint8_t r = ((pixel >> 11) & 0x1F) << 3;
            uint8_t g = ((pixel >> 5) & 0x3F) << 2;
            uint8_t b = (pixel & 0x1F) << 3;
            viewer->current_sprite->pixels[i] = (255 << 24) | (r << 16) | (g << 8) | b;
        }

        /* Create SDL texture */
        decoded_sprite_create_texture(viewer->current_sprite, viewer->framework.renderer);
    }

    free(rgb565_buffer);
}

/**
 * Render current frame
 */
static void viewer_render(CreatureViewer* viewer) {
    if (!viewer) return;

    /* Clear screen */
    sdl_framework_begin_frame(&viewer->framework);

    /* Render current sprite */
    if (viewer->current_sprite && viewer->current_sprite->texture) {
        int window_width, window_height;
        SDL_GetWindowSize(viewer->framework.window, &window_width, &window_height);

        SDL_Rect dest_rect = {
            (window_width - viewer->current_sprite->width) / 2,
            (window_height - viewer->current_sprite->height) / 2,
            viewer->current_sprite->width,
            viewer->current_sprite->height
        };

        SDL_RenderCopy(viewer->framework.renderer, viewer->current_sprite->texture, NULL, &dest_rect);
    }

    sdl_framework_end_frame(&viewer->framework);
}

/**
 * Handle input events
 */
static void viewer_handle_input(CreatureViewer* viewer, SDL_Event* event) {
    if (!viewer || !event) return;

    if (event->type == SDL_KEYDOWN) {
        int need_update = 0;

        switch (event->key.keysym.sym) {
            case SDLK_LEFT:
                viewer->creature_type--;
                if (viewer->creature_type < 0) {
                    viewer->creature_type = viewer->max_creature_types - 1;
                }
                viewer->current_action = 0;
                viewer->current_direction = 0;
                viewer->current_frame = 0;
                need_update = 1;
                break;

            case SDLK_RIGHT:
                viewer->creature_type++;
                if (viewer->creature_type >= viewer->max_creature_types) {
                    viewer->creature_type = 0;
                }
                viewer->current_action = 0;
                viewer->current_direction = 0;
                viewer->current_frame = 0;
                need_update = 1;
                break;

            case SDLK_UP:
                viewer->current_action++;
                need_update = 1;
                break;

            case SDLK_DOWN:
                viewer->current_action--;
                if (viewer->current_action < 0) {
                    viewer->current_action = 7; /* Wrap around */
                }
                need_update = 1;
                break;

            case SDLK_1: case SDLK_2: case SDLK_3: case SDLK_4:
            case SDLK_5: case SDLK_6: case SDLK_7: case SDLK_8:
                viewer->current_direction = event->key.keysym.sym - SDLK_1;
                viewer->current_frame = 0;
                need_update = 1;
                break;

            case SDLK_SPACE:
                viewer->playing = !viewer->playing;
                break;

            case SDLK_c:
                viewer->current_colorset++;
                if (viewer->current_colorset >= 16) {
                    viewer->current_colorset = 0;
                }
                need_update = 1;
                break;

            case SDLK_i:
                viewer->show_info = !viewer->show_info;
                break;

            case SDLK_ESCAPE:
                viewer->framework.running = 0;
                break;
        }

        if (need_update) {
            viewer_update_frame_info(viewer);
            viewer_decode_current_sprite(viewer);
        }
    }
}

/**
 * Main loop
 */
static void viewer_run(CreatureViewer* viewer) {
    if (!viewer) return;

    SDL_Event event;
    while (viewer->framework.running) {
        /* Process events */
        while (sdl_framework_poll_events(&viewer->framework, &event)) {
            viewer_handle_input(viewer, &event);
        }

        /* Animation playback */
        if (viewer->playing && viewer->max_frames > 0) {
            uint32_t current_time = SDL_GetTicks();
            if (current_time - viewer->last_frame_time >= viewer->frame_interval) {
                viewer->current_frame++;
                if (viewer->current_frame >= viewer->max_frames) {
                    viewer->current_frame = 0; /* Loop animation */
                }
                viewer_decode_current_sprite(viewer);
                viewer->last_frame_time = current_time;
            }
        }

        /* Render */
        viewer_render(viewer);

        /* Display info */
        if (viewer->show_info) {
            printf("\rCreature %d/%d | %s | Dir:%s | Frame %d/%d | ColorSet %d%s ",
                   viewer->creature_type + 1, viewer->max_creature_types,
                   get_action_name(viewer->current_action),
                   DIR_NAMES[viewer->current_direction],
                   viewer->current_frame + 1, viewer->max_frames,
                   viewer->current_colorset,
                   viewer->playing ? " [PLAY]" : " [PAUSE]");
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
 * Main entry point
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* base_path = argv[1];
    char ispk_path[MAX_PATH_LEN];
    char cfpk_path[MAX_PATH_LEN];

    CreatureViewer viewer;

    /* Build paths */
    snprintf(ispk_path, MAX_PATH_LEN, "%s.ispk", base_path);
    snprintf(cfpk_path, MAX_PATH_LEN, "%s.cfpk", base_path);

    printf("Creature Animation Viewer\n");
    printf("=======================\n\n");
    printf("Loading resources from: %s\n", base_path);
    printf("  ISPK: %s\n", ispk_path);
    printf("  CFPK: %s\n", cfpk_path);

    /* Initialize viewer state */
    memset(&viewer, 0, sizeof(CreatureViewer));
    viewer.creature_type = 0;
    viewer.current_action = 0;
    viewer.current_direction = 0;
    viewer.current_frame = 0;
    viewer.show_info = 1;
    viewer.playing = 0;
    viewer.current_colorset = 0;
    viewer.frame_interval = 1000 / DEFAULT_FRAME_RATE;

    /* Initialize SDL framework */
    SDLFrameworkConfig config = {
        .window_width = DEFAULT_WIDTH,
        .window_height = DEFAULT_HEIGHT,
        .window_title = "Creature Viewer",
        .target_fps = 60
    };

    if (sdl_framework_init(&viewer.framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        return 1;
    }

    /* Load indexed sprite pack */
    index_spritepack_init(&viewer.ispk);
    if (!index_spritepack_load(&viewer.ispk, ispk_path)) {
        fprintf(stderr, "Error: Failed to load indexed sprite pack: %s\n", ispk_path);
        sdl_framework_cleanup(&viewer.framework);
        return 1;
    }
    printf("Loaded %d sprites from ISPK\n", index_spritepack_get_size(&viewer.ispk));

    /* Load creature frame pack */
    creature_framepack_init(&viewer.cfpk);
    if (!creature_framepack_load(&viewer.cfpk, cfpk_path)) {
        fprintf(stderr, "Error: Failed to load creature frame pack: %s\n", cfpk_path);
        index_spritepack_release(&viewer.ispk);
        sdl_framework_cleanup(&viewer.framework);
        return 1;
    }

    viewer.max_creature_types = creature_framepack_size(&viewer.cfpk);
    printf("Loaded %d creature types from CFPK\n", viewer.max_creature_types);

    /* Update frame info */
    viewer_update_frame_info(&viewer);
    viewer_decode_current_sprite(&viewer);

    printf("\nControls:\n");
    printf("  LEFT/RIGHT  - Switch creature type\n");
    printf("  UP/DOWN     - Switch action\n");
    printf("  1-8         - Switch direction\n");
    printf("  SPACE       - Play/Pause animation\n");
    printf("  C           - Cycle ColorSet\n");
    printf("  I           - Toggle info\n");
    printf("  ESC         - Exit\n\n");

    /* Run main loop */
    viewer_run(&viewer);

    /* Cleanup */
    if (viewer.current_sprite) {
        decoded_sprite_free(viewer.current_sprite);
        free(viewer.current_sprite);
    }

    index_spritepack_release(&viewer.ispk);
    creature_framepack_free(&viewer.cfpk);
    sdl_framework_cleanup(&viewer.framework);

    printf("\nViewer closed\n");
    return 0;
}
