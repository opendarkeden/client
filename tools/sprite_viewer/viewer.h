/**
 * @file viewer.h
 * @brief Sprite viewer application
 * 
 * Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6
 */

#ifndef VIEWER_H
#define VIEWER_H

#include "sdl_framework.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sprite viewer state
 */
typedef struct {
    SDLFramework framework;
    SpritePack pack;
    DecodedSprite* decoded_sprites;
    int current_index;
    float zoom;
    int show_info;
} SpriteViewer;

/**
 * Initialize sprite viewer
 * @param viewer Pointer to viewer state
 * @param pack_file Path to sprite pack file
 * @param width Window width
 * @param height Window height
 * @return 0 on success, negative error code on failure
 */
int viewer_init(SpriteViewer* viewer, const char* pack_file, 
                int width, int height);

/**
 * Handle input events
 * @param viewer Pointer to viewer state
 * @param event SDL event to process
 */
void viewer_handle_input(SpriteViewer* viewer, SDL_Event* event);

/**
 * Render current frame
 * @param viewer Pointer to viewer state
 */
void viewer_render(SpriteViewer* viewer);

/**
 * Run main loop
 * @param viewer Pointer to viewer state
 */
void viewer_run(SpriteViewer* viewer);

/**
 * Cleanup viewer resources
 * @param viewer Pointer to viewer state
 */
void viewer_cleanup(SpriteViewer* viewer);

#ifdef __cplusplus
}
#endif

#endif /* VIEWER_H */
