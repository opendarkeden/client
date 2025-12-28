#pragma once

#include <stdint.h>
#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations from lib */
typedef struct SDLFramework SDLFramework;
typedef struct Zone Zone;
typedef struct SpritePack SpritePack;
typedef struct IndexSpritePack IndexSpritePack;
typedef struct CreatureFramePack CreatureFramePack;

typedef struct {
    const char* map_path;            // DarkEden/Data/Map/*.map
    const char* tile_spk_path;       /* DarkEden/Data/Image/tile.spk */
    const char* creature_base_path;  /* DarkEden/Data/Image/Creature (without ext) */
    int window_width;                /* pixels */
    int window_height;               /* pixels */
    int cell_pixels;                 /* tile width in pixels (height is half) */
    float move_speed;                /* pixels per second in world space */
} TopViewConfig;

typedef struct TopView TopView;

/* Lifecycle */
TopView* top_view_create(void);
int top_view_init(TopView* tv, const TopViewConfig* cfg);
void top_view_run(TopView* tv);
void top_view_destroy(TopView* tv);

#ifdef __cplusplus
}
#endif
