/**
 * @file viewer.h
 * @brief Item viewer application
 */

#ifndef VIEWER_H
#define VIEWER_H

#include "engine/sprite/include/sdl_framework.h"
#include "engine/sprite/include/index_spritepack.h"
#include "engine/sprite/include/sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Item viewer state
 */
typedef struct {
    SDLFramework framework;
    IndexSpritePack pack;
    DecodedSprite* decoded_sprites;
    int current_index;
    float zoom;
    int show_info;
} ItemViewer;

/**
 * Initialize item viewer
 * @param viewer Pointer to viewer state
 * @param pack_file Path to indexed sprite pack file (.ispk)
 * @param width Window width
 * @param height Window height
 * @return 0 on success, negative error code on failure
 */
int viewer_init(ItemViewer* viewer, const char* pack_file,
                int width, int height);

/**
 * Handle input events
 * @param viewer Pointer to viewer state
 * @param event SDL event to process
 */
void viewer_handle_input(ItemViewer* viewer, SDL_Event* event);

/**
 * Render current frame
 * @param viewer Pointer to viewer state
 */
void viewer_render(ItemViewer* viewer);

/**
 * Run main loop
 * @param viewer Pointer to viewer state
 */
void viewer_run(ItemViewer* viewer);

/**
 * Cleanup viewer resources
 * @param viewer Pointer to viewer state
 */
void viewer_cleanup(ItemViewer* viewer);

#ifdef __cplusplus
}
#endif

#endif /* VIEWER_H */
