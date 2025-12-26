/**
 * @file main.c
 * @brief Creature Animation Viewer - Tool for viewing creature animations
 * 
 * Requirements: 5.1-5.10
 * 
 * This tool loads Creature.ispk, Creature.cfpk, and optionally Creature.sspk
 * to display and navigate creature animations with ColorSet support.
 */

#include "sdl_framework.h"
#include "index_spritepack.h"
#include "shadow_spritepack.h"
#include "framepack.h"
#include "colorset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Window defaults */
#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

/* Animation defaults */
#define DEFAULT_FRAME_RATE 10  /* Frames per second for animation playback */
#define MAX_DIRECTIONS 8

/* Action names for display */
static const char* ACTION_NAMES[] = {
    "STAND",
    "WALK",
    "RUN",
    "ATTACK",
    "DAMAGED",
    "DIE",
    "SKILL1",
    "SKILL2",
    "SKILL3",
    "SKILL4",
    "SKILL5",
    "SKILL6",
    "SKILL7",
    "SKILL8"
};
#define MAX_ACTION_NAMES 14

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
 * Creature viewer state
 */
typedef struct {
    SDLFramework framework;
    
    /* Resource packs */
    IndexSpritePack ispk;
    CreatureFramePack cfpk;
    ShadowSpritePack sspk;
    
    /* State */
    int creature_type;      /* Current creature type index */
    int max_creature_types; /* Total number of creature types */
    int current_action;     /* Current action index */
    int max_actions;        /* Number of actions for current creature */
    int current_direction;  /* Current direction (0-7) */
    int current_frame;      /* Current frame index */
    int max_frames;         /* Number of frames for current action/direction */
    
    /* Display options */
    int show_shadow;        /* Whether to show shadow */
    int show_info;          /* Whether to show info */
    int playing;            /* Animation playback state */
    int current_colorset;   /* Current ColorSet index */
    
    /* Timing */
    uint32_t last_frame_time;
    uint32_t frame_interval;
    
    /* Flags */
    int has_shadow;         /* Whether shadow pack was loaded */
} CreatureViewer;

/**
 * Print usage information
 * Requirement 5.1: Accept file paths as command line arguments
 */
static void print_usage(const char* program) {
    printf("Creature Animation Viewer\n");
    printf("=========================\n\n");
    printf("Usage: %s <ispk_file> <cfpk_file> [sspk_file]\n", program);
    printf("\nArguments:\n");
    printf("  ispk_file   Path to Creature.ispk (IndexedSpritePack)\n");
    printf("  cfpk_file   Path to Creature.cfpk (CreatureFramePack)\n");
    printf("  sspk_file   Path to Creature.sspk (ShadowSpritePack, optional)\n");
    printf("\nControls:\n");
    printf("  LEFT/RIGHT  - Navigate to previous/next Creature type\n");
    printf("  UP/DOWN     - Change Action (STAND, WALK, ATTACK, etc.)\n");
    printf("  1-8         - Change Direction (0-7)\n");
    printf("  SPACE       - Toggle animation playback (play/pause)\n");
    printf("  S           - Toggle shadow display\n");
    printf("  C           - Cycle through ColorSets\n");
    printf("  I           - Toggle info display\n");
    printf("  ESC         - Exit\n");
}

/**
 * Update frame data for current creature/action/direction
 */
static void viewer_update_frame_info(CreatureViewer* viewer) {
    if (viewer == NULL) {
        return;
    }
    
    /* Get creature type data */
    if (viewer->creature_type < 0 || viewer->creature_type >= viewer->max_creature_types) {
        viewer->max_actions = 0;
        viewer->max_frames = 0;
        return;
    }
    
    ActionArray* actions = creature_framepack_get(&viewer->cfpk, viewer->creature_type);
    if (actions == NULL) {
        viewer->max_actions = 0;
        viewer->max_frames = 0;
        return;
    }
    
    viewer->max_actions = action_array_size(actions);
    
    if (viewer->max_actions == 0) {
        viewer->max_frames = 0;
        return;
    }
    
    /* Clamp action */
    if (viewer->current_action >= viewer->max_actions) {
        viewer->current_action = 0;
    }
    
    DirectionArray* directions = action_array_get(actions, viewer->current_action);
    if (directions == NULL) {
        viewer->max_frames = 0;
        return;
    }
    
    int num_directions = direction_array_size(directions);
    
    if (num_directions == 0) {
        viewer->max_frames = 0;
        return;
    }
    
    /* Clamp direction */
    if (viewer->current_direction >= num_directions) {
        viewer->current_direction = 0;
    }
    
    FrameArray* frames = direction_array_get(directions, viewer->current_direction);
    if (frames == NULL) {
        viewer->max_frames = 0;
        return;
    }
    
    viewer->max_frames = frame_array_size(frames);
    
    /* Clamp frame */
    if (viewer->current_frame >= viewer->max_frames) {
        viewer->current_frame = 0;
    }
}

/**
 * Get current frame's sprite ID
 */
static TYPE_SPRITEID viewer_get_current_sprite_id(CreatureViewer* viewer) {
    if (viewer == NULL) {
        return SPRITEID_NULL;
    }
    
    Frame* frame = creature_framepack_get_frame(&viewer->cfpk,
                                                viewer->creature_type,
                                                viewer->current_action,
                                                viewer->current_direction,
                                                viewer->current_frame);
    if (frame == NULL) {
        return SPRITEID_NULL;
    }
    
    return frame->sprite_id;
}

/**
 * Get current frame's offset
 */
static void viewer_get_current_offset(CreatureViewer* viewer, int16_t* cx, int16_t* cy) {
    *cx = 0;
    *cy = 0;
    
    if (viewer == NULL) {
        return;
    }
    
    Frame* frame = creature_framepack_get_frame(&viewer->cfpk,
                                                viewer->creature_type,
                                                viewer->current_action,
                                                viewer->current_direction,
                                                viewer->current_frame);
    if (frame == NULL) {
        return;
    }
    
    *cx = frame->cx;
    *cy = frame->cy;
}

/**
 * Initialize creature viewer
 * Requirement 5.1: Load Creature.ispk, Creature.cfpk, and optionally Creature.sspk
 * Requirement 5.2: Display the first Creature type centered on screen
 */
static int viewer_init(CreatureViewer* viewer, const char* ispk_file, 
                       const char* cfpk_file, const char* sspk_file,
                       int width, int height) {
    if (viewer == NULL || ispk_file == NULL || cfpk_file == NULL) {
        return -1;
    }
    
    /* Initialize viewer state */
    memset(viewer, 0, sizeof(CreatureViewer));
    viewer->creature_type = 0;
    viewer->current_action = 0;
    viewer->current_direction = 0;
    viewer->current_frame = 0;
    viewer->show_shadow = 1;
    viewer->show_info = 1;
    viewer->playing = 1;
    viewer->current_colorset = 0;
    viewer->last_frame_time = 0;
    viewer->frame_interval = 1000 / DEFAULT_FRAME_RATE;
    viewer->has_shadow = 0;
    
    /* Initialize ColorSet system */
    colorset_init();
    colorset_set_using(0, 0);
    
    /* Initialize SDL framework */
    SDLFrameworkConfig config = {
        .window_width = width,
        .window_height = height,
        .window_title = "Creature Animation Viewer",
        .target_fps = 60
    };
    
    if (sdl_framework_init(&viewer->framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        return -1;
    }
    
    /* Initialize packs */
    index_spritepack_init(&viewer->ispk);
    creature_framepack_init(&viewer->cfpk);
    shadow_spritepack_init(&viewer->sspk);
    
    /* Load IndexedSpritePack - Requirement 5.1 */
    printf("Loading IndexedSpritePack: %s\n", ispk_file);
    if (!index_spritepack_load_lazy(&viewer->ispk, ispk_file)) {
        fprintf(stderr, "Error: Failed to load IndexedSpritePack: %s\n", ispk_file);
        sdl_framework_cleanup(&viewer->framework);
        return -2;
    }
    printf("Loaded %d sprites from ISPK\n", index_spritepack_get_size(&viewer->ispk));
    
    /* Load CreatureFramePack - Requirement 5.1 */
    printf("Loading CreatureFramePack: %s\n", cfpk_file);
    if (!creature_framepack_load(&viewer->cfpk, cfpk_file)) {
        fprintf(stderr, "Error: Failed to load CreatureFramePack: %s\n", cfpk_file);
        index_spritepack_release(&viewer->ispk);
        sdl_framework_cleanup(&viewer->framework);
        return -3;
    }
    viewer->max_creature_types = creature_framepack_size(&viewer->cfpk);
    printf("Loaded %d creature types from CFPK\n", viewer->max_creature_types);
    
    /* Load ShadowSpritePack (optional) - Requirement 5.1 */
    if (sspk_file != NULL) {
        printf("Loading ShadowSpritePack: %s\n", sspk_file);
        if (shadow_spritepack_load_lazy(&viewer->sspk, sspk_file)) {
            viewer->has_shadow = 1;
            printf("Loaded %d shadow sprites from SSPK\n", shadow_spritepack_get_size(&viewer->sspk));
        } else {
            fprintf(stderr, "Warning: Failed to load ShadowSpritePack: %s\n", sspk_file);
        }
    }
    
    /* Initialize frame info - Requirement 5.2 */
    viewer_update_frame_info(viewer);
    
    return 0;
}

/**
 * Handle input events
 * Requirement 5.3: LEFT/RIGHT navigate creature types
 * Requirement 5.4: UP/DOWN change action
 * Requirement 5.5: 1-8 keys change direction
 * Requirement 5.6: SPACE toggles playback
 * Requirement 5.7: S toggles shadow display
 * Requirement 5.8: C cycles through ColorSets
 * Requirement 5.10: ESC exits
 */
static void viewer_handle_input(CreatureViewer* viewer, SDL_Event* event) {
    if (viewer == NULL || event == NULL) {
        return;
    }
    
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            /* Requirement 5.3: Navigate creature types with LEFT/RIGHT */
            case SDLK_LEFT:
                viewer->creature_type--;
                if (viewer->creature_type < 0) {
                    viewer->creature_type = viewer->max_creature_types - 1;
                }
                viewer->current_action = 0;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
                
            case SDLK_RIGHT:
                viewer->creature_type++;
                if (viewer->creature_type >= viewer->max_creature_types) {
                    viewer->creature_type = 0;
                }
                viewer->current_action = 0;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
                
            /* Requirement 5.4: Change action with UP/DOWN */
            case SDLK_UP:
                viewer->current_action++;
                if (viewer->current_action >= viewer->max_actions) {
                    viewer->current_action = 0;
                }
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
                
            case SDLK_DOWN:
                viewer->current_action--;
                if (viewer->current_action < 0) {
                    viewer->current_action = viewer->max_actions - 1;
                    if (viewer->current_action < 0) {
                        viewer->current_action = 0;
                    }
                }
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
                
            /* Requirement 5.5: Change direction with 1-8 keys */
            case SDLK_1:
                viewer->current_direction = 0;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_2:
                viewer->current_direction = 1;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_3:
                viewer->current_direction = 2;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_4:
                viewer->current_direction = 3;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_5:
                viewer->current_direction = 4;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_6:
                viewer->current_direction = 5;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_7:
                viewer->current_direction = 6;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
            case SDLK_8:
                viewer->current_direction = 7;
                viewer->current_frame = 0;
                viewer_update_frame_info(viewer);
                break;
                
            /* Requirement 5.6: Toggle playback with SPACE */
            case SDLK_SPACE:
                viewer->playing = !viewer->playing;
                break;
                
            /* Requirement 5.7: Toggle shadow with S */
            case SDLK_s:
                viewer->show_shadow = !viewer->show_shadow;
                break;
                
            /* Requirement 5.8: Cycle ColorSet with C */
            case SDLK_c:
                viewer->current_colorset = (viewer->current_colorset + 1) % MAX_COLORSET;
                colorset_set_using(viewer->current_colorset, viewer->current_colorset);
                break;
                
            /* Toggle info display */
            case SDLK_i:
                viewer->show_info = !viewer->show_info;
                break;
                
            /* Requirement 5.10: ESC exits */
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
 */
static void viewer_update(CreatureViewer* viewer) {
    if (viewer == NULL || !viewer->playing) {
        return;
    }
    
    uint32_t current_time = SDL_GetTicks();
    
    /* Advance frames at frame rate */
    if (current_time - viewer->last_frame_time >= viewer->frame_interval) {
        viewer->current_frame++;
        if (viewer->current_frame >= viewer->max_frames) {
            viewer->current_frame = 0;
        }
        viewer->last_frame_time = current_time;
    }
}

/**
 * Render current frame
 * Requirement 5.2: Display creature centered on screen
 * Requirement 5.9: Show CreatureType, Action, Direction, Frame, ColorSet info
 */
static void viewer_render(CreatureViewer* viewer) {
    if (viewer == NULL) {
        return;
    }
    
    /* Begin frame */
    sdl_framework_begin_frame(&viewer->framework);
    
    /* Get window size */
    int win_w, win_h;
    SDL_GetWindowSize(viewer->framework.window, &win_w, &win_h);
    int center_x = win_w / 2;
    int center_y = win_h / 2;
    
    /* Get current sprite ID and offset */
    TYPE_SPRITEID sprite_id = viewer_get_current_sprite_id(viewer);
    int16_t cx, cy;
    viewer_get_current_offset(viewer, &cx, &cy);
    
    if (sprite_id != SPRITEID_NULL && sprite_id < index_spritepack_get_size(&viewer->ispk)) {
        /* Render shadow first (if enabled and available) */
        if (viewer->show_shadow && viewer->has_shadow) {
            ShadowSprite* shadow = shadow_spritepack_get(&viewer->sspk, sprite_id);
            if (shadow != NULL && shadow_sprite_is_init(shadow)) {
                int shadow_x = center_x - cx;
                int shadow_y = center_y - cy;
                shadow_sprite_render_alpha(shadow, viewer->framework.renderer, 
                                          shadow_x, shadow_y, 128);
            }
        }
        
        /* Render creature sprite */
        IndexSprite* sprite = index_spritepack_get(&viewer->ispk, sprite_id);
        if (sprite != NULL && index_sprite_is_init(sprite)) {
            int sprite_x = center_x - cx;
            int sprite_y = center_y - cy;
            index_sprite_render_colorset(sprite, viewer->framework.renderer,
                                        sprite_x, sprite_y, 
                                        (uint16_t)viewer->current_colorset);
        }
    }
    
    /* Requirement 5.9: Show info in window title */
    if (viewer->show_info) {
        char title[512];
        const char* action_name = (viewer->current_action < MAX_ACTION_NAMES) ?
                                  ACTION_NAMES[viewer->current_action] : "UNKNOWN";
        const char* dir_name = (viewer->current_direction < MAX_DIRECTIONS) ?
                               DIR_NAMES[viewer->current_direction] : "UNKNOWN";
        const char* play_state = viewer->playing ? "PLAYING" : "PAUSED";
        const char* shadow_state = viewer->show_shadow ? "ON" : "OFF";
        
        snprintf(title, sizeof(title),
                "Creature Viewer - Type %d/%d | Action: %s (%d/%d) | Dir: %s | "
                "Frame: %d/%d | ColorSet: %d | Shadow: %s | %s",
                viewer->creature_type + 1, viewer->max_creature_types,
                action_name, viewer->current_action + 1, viewer->max_actions,
                dir_name,
                viewer->current_frame + 1, viewer->max_frames,
                viewer->current_colorset,
                shadow_state,
                play_state);
        SDL_SetWindowTitle(viewer->framework.window, title);
    }
    
    /* End frame */
    sdl_framework_end_frame(&viewer->framework);
}

/**
 * Run main loop
 */
static void viewer_run(CreatureViewer* viewer) {
    if (viewer == NULL) {
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
 * Requirement 5.10: Exit cleanly
 */
static void viewer_cleanup(CreatureViewer* viewer) {
    if (viewer == NULL) {
        return;
    }
    
    /* Release packs */
    index_spritepack_release(&viewer->ispk);
    creature_framepack_free(&viewer->cfpk);
    shadow_spritepack_release(&viewer->sspk);
    
    /* Cleanup SDL framework */
    sdl_framework_cleanup(&viewer->framework);
}

/**
 * Main entry point
 * Requirement 5.1: Accept file paths as command line arguments
 * Requirement 5.10: Exit cleanly
 */
int main(int argc, char* argv[]) {
    /* Requirement 5.1: Accept file paths as command line arguments */
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* ispk_file = argv[1];
    const char* cfpk_file = argv[2];
    const char* sspk_file = (argc >= 4) ? argv[3] : NULL;
    
    CreatureViewer viewer;
    
    printf("Creature Animation Viewer\n");
    printf("=========================\n");
    printf("ISPK: %s\n", ispk_file);
    printf("CFPK: %s\n", cfpk_file);
    if (sspk_file != NULL) {
        printf("SSPK: %s\n", sspk_file);
    }
    printf("\n");
    
    /* Initialize viewer */
    int result = viewer_init(&viewer, ispk_file, cfpk_file, sspk_file,
                            DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize viewer (error: %d)\n", result);
        return 1;
    }
    
    printf("\nViewer initialized successfully\n");
    printf("Controls:\n");
    printf("  LEFT/RIGHT  - Navigate creature types\n");
    printf("  UP/DOWN     - Change action\n");
    printf("  1-8         - Change direction\n");
    printf("  SPACE       - Play/Pause\n");
    printf("  S           - Toggle shadow\n");
    printf("  C           - Cycle ColorSet\n");
    printf("  I           - Toggle info\n");
    printf("  ESC         - Exit\n\n");
    
    /* Run main loop */
    viewer_run(&viewer);
    
    /* Requirement 5.10: Exit cleanly */
    viewer_cleanup(&viewer);
    
    printf("Viewer closed\n");
    return 0;
}
