/**
 * @file main.c
 * @brief Map Viewer - Zone map browser with pan/zoom support
 *
 * Features:
 * - Load .map zone file with tile and ImageObject sprite packs
 * - Camera-based viewing with pan, zoom, and drag controls
 * - 2:1 aspect ratio grid rendering (isometric-style)
 * - Lazy sprite loading for performance
 * - Toggle grid and ImageObject display
 */

#include "engine/sprite/include/sdl_framework.h"
#include "engine/sprite/include/spritepack.h"
#include "engine/sprite/include/sprite.h"
#include "engine/sprite/include/zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CELL_PIXELS 32
#define DEFAULT_WINDOW_TILES_W 40
#define DEFAULT_WINDOW_TILES_H 30

/**
 * Print usage information
 */
static void print_usage(const char* prog) {
    printf("Map Viewer - Zone Map Browser\n");
    printf("==============================\n\n");
    printf("Usage: %s <zone.map> <tile.spk> [imageobject.spk] [--cell N]\n", prog);
    printf("\nArguments:\n");
    printf("  zone.map         - Zone map file (.map)\n");
    printf("  tile.spk         - Tile sprite pack (.spk)\n");
    printf("  imageobject.spk  - Optional: ImageObject sprite pack\n");
    printf("  --cell N         - Cell pixel width (default: 32)\n");
    printf("                    Cell height = width/2 for 2:1 ratio\n");
    printf("\nControls:\n");
    printf("  WASD/Arrows  - Pan camera\n");
    printf("  +/- or Wheel - Zoom in/out (around screen center)\n");
    printf("  Right-drag   - Drag to pan\n");
    printf("  1/Home       - Reset camera\n");
    printf("  G            - Toggle grid display\n");
    printf("  I            - Toggle ImageObject display\n");
    printf("  ESC          - Exit\n");
    printf("\nExamples:\n");
    printf("  %s DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk\n", prog);
    printf("  %s adam_c.map tile.spk obj.spk --cell 64\n", prog);
}

/**
 * Main entry point
 */
int main(int argc, char** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char* map_path = argv[1];
    const char* tile_spk_path = argv[2];
    const char* obj_spk_path = NULL;

    int cell_pixels = DEFAULT_CELL_PIXELS;
    int window_tiles_w = DEFAULT_WINDOW_TILES_W;
    int window_tiles_h = DEFAULT_WINDOW_TILES_H;

    /* Parse optional arguments */
    for (int i = 3; i < argc; i++) {
        if (argv[i][0] != '-' && obj_spk_path == NULL) {
            /* Positional argument for ImageObject spk */
            obj_spk_path = argv[i];
            continue;
        }
        if (strcmp(argv[i], "--cell") == 0 && (i + 1) < argc) {
            cell_pixels = atoi(argv[i + 1]);
            if (cell_pixels <= 0) {
                cell_pixels = DEFAULT_CELL_PIXELS;
            }
            i++;
            continue;
        }
    }

    printf("Map Viewer\n");
    printf("==========\n\n");
    printf("Loading zone: %s\n", map_path);
    printf("Loading tiles: %s\n", tile_spk_path);
    if (obj_spk_path) {
        printf("Loading objects: %s\n", obj_spk_path);
    }

    /* Load zone */
    Zone zone;
    Error* zerr = zone_load(map_path, &zone);
    if (zerr != NULL) {
        fprintf(stderr, "Error: Failed to load zone: %s\n", zone_strerror((ZoneError)zerr->code));
        err_print(zerr, stderr);
        return 1;
    }

    printf("Loaded zone: %dx%d tiles\n", zone.width, zone.height);
    printf("ImageObjects: %u\n", (unsigned)zone.image_object_count);

    /* Initialize SDL framework */
    int tile_w_px = cell_pixels;
    int tile_h_px = cell_pixels / 2;
    if (tile_h_px <= 0) {
        tile_h_px = 1;
    }

    SDLFrameworkConfig config = {
        .window_width = window_tiles_w * tile_w_px,
        .window_height = window_tiles_h * tile_h_px,
        .window_title = "Map Viewer",
        .target_fps = 30
    };

    SDLFramework framework;
    if (sdl_framework_init(&framework, &config) != 0) {
        fprintf(stderr, "Error: Failed to initialize SDL framework\n");
        zone_free(&zone);
        return 1;
    }

    /* Load tile sprite pack (lazy loading) */
    SpritePack tile_pack;
    if (spritepack_load_lazy(&tile_pack, tile_spk_path) != 0) {
        fprintf(stderr, "Error: Failed to load tile sprite pack: %s\n", tile_spk_path);
        sdl_framework_cleanup(&framework);
        zone_free(&zone);
        return 1;
    }
    printf("Loaded %u tile sprites\n", (unsigned)tile_pack.count);

    /* Load ImageObject sprite pack (optional) */
    SpritePack obj_pack;
    int has_obj_pack = 0;
    if (obj_spk_path) {
        if (spritepack_load_lazy(&obj_pack, obj_spk_path) != 0) {
            fprintf(stderr, "Warning: Failed to load ImageObject pack: %s (objects disabled)\n", obj_spk_path);
        } else {
            has_obj_pack = 1;
            printf("Loaded %u object sprites\n", (unsigned)obj_pack.count);
        }
    }

    /* Allocate tile sprite cache */
    DecodedSprite* tile_cache = (DecodedSprite*)calloc(tile_pack.count, sizeof(DecodedSprite));
    uint8_t* tile_have = (uint8_t*)calloc(tile_pack.count, 1);
    if (!tile_cache || !tile_have) {
        fprintf(stderr, "Error: Out of memory for tile cache\n");
        free(tile_cache);
        free(tile_have);
        spritepack_free(&tile_pack);
        sdl_framework_cleanup(&framework);
        zone_free(&zone);
        return 1;
    }

    /* Allocate object sprite cache */
    DecodedSprite* obj_cache = NULL;
    uint8_t* obj_have = NULL;
    if (has_obj_pack) {
        obj_cache = (DecodedSprite*)calloc(obj_pack.count, sizeof(DecodedSprite));
        obj_have = (uint8_t*)calloc(obj_pack.count, 1);
        if (!obj_cache || !obj_have) {
            fprintf(stderr, "Error: Out of memory for object cache\n");
            free(obj_cache);
            free(obj_have);
            free(tile_cache);
            free(tile_have);
            spritepack_free(&tile_pack);
            if (has_obj_pack) {
                spritepack_free(&obj_pack);
            }
            sdl_framework_cleanup(&framework);
            zone_free(&zone);
            return 1;
        }
    }

    /* Camera state */
    double zoom = 1.0;
    double cam_x = 0.0;
    double cam_y = 0.0;
    int show_grid = 1;
    int show_objects = 1;
    int dragging = 0;
    int drag_last_x = 0;
    int drag_last_y = 0;

    printf("\nControls:\n");
    printf("  WASD/Arrows - Pan camera\n");
    printf("  +/-/Wheel   - Zoom\n");
    printf("  Right-drag  - Drag pan\n");
    printf("  1/Home      - Reset camera\n");
    printf("  G           - Toggle grid\n");
    printf("  I           - Toggle objects\n");
    printf("  ESC         - Exit\n\n");

    /* Main loop */
    SDL_Event event;
    while (framework.running) {
        int win_w = 0, win_h = 0;
        SDL_GetRendererOutputSize(framework.renderer, &win_w, &win_h);

        /* Process events */
        while (sdl_framework_poll_events(&framework, &event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        framework.running = 0;
                        break;

                    /* Pan controls */
                    case SDLK_LEFT:
                    case SDLK_a:
                        cam_x -= (int)(32 * zoom);
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        cam_x += (int)(32 * zoom);
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        cam_y -= (int)(32 * zoom);
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        cam_y += (int)(32 * zoom);
                        break;

                    /* Zoom controls */
                    case SDLK_EQUALS:
                    case SDLK_PLUS: {
                        double old_zoom = zoom;
                        zoom *= 1.25;
                        if (zoom > 6.0) {
                            zoom = 6.0;
                        }
                        /* Zoom around screen center */
                        double center_unscaled_x = (cam_x + win_w * 0.5) / old_zoom;
                        double center_unscaled_y = (cam_y + win_h * 0.5) / old_zoom;
                        cam_x = center_unscaled_x * zoom - win_w * 0.5;
                        cam_y = center_unscaled_y * zoom - win_h * 0.5;
                        break;
                    }
                    case SDLK_MINUS: {
                        double old_zoom = zoom;
                        zoom /= 1.25;
                        if (zoom < 0.25) {
                            zoom = 0.25;
                        }
                        /* Zoom around screen center */
                        double center_unscaled_x = (cam_x + win_w * 0.5) / old_zoom;
                        double center_unscaled_y = (cam_y + win_h * 0.5) / old_zoom;
                        cam_x = center_unscaled_x * zoom - win_w * 0.5;
                        cam_y = center_unscaled_y * zoom - win_h * 0.5;
                        break;
                    }

                    /* Reset camera */
                    case SDLK_1:
                    case SDLK_HOME:
                        zoom = 1.0;
                        cam_x = 0.0;
                        cam_y = 0.0;
                        break;

                    /* Toggle grid */
                    case SDLK_g:
                        show_grid = !show_grid;
                        break;

                    /* Toggle objects */
                    case SDLK_i:
                        show_objects = !show_objects;
                        break;

                    default:
                        break;
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                /* Mouse wheel zoom */
                double old_zoom = zoom;
                if (event.wheel.y > 0) {
                    zoom *= 1.25;
                } else if (event.wheel.y < 0) {
                    zoom /= 1.25;
                }
                if (zoom < 0.25) {
                    zoom = 0.25;
                }
                if (zoom > 6.0) {
                    zoom = 6.0;
                }
                /* Zoom around screen center */
                double center_unscaled_x = (cam_x + win_w * 0.5) / old_zoom;
                double center_unscaled_y = (cam_y + win_h * 0.5) / old_zoom;
                cam_x = center_unscaled_x * zoom - win_w * 0.5;
                cam_y = center_unscaled_y * zoom - win_h * 0.5;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    dragging = 1;
                    drag_last_x = event.button.x;
                    drag_last_y = event.button.y;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    dragging = 0;
                }
            } else if (event.type == SDL_MOUSEMOTION && dragging) {
                int dx = event.motion.x - drag_last_x;
                int dy = event.motion.y - drag_last_y;
                cam_x -= dx;
                cam_y -= dy;
                drag_last_x = event.motion.x;
                drag_last_y = event.motion.y;
            }
        }

        /* Begin frame */
        sdl_framework_begin_frame(&framework);

        /* Calculate scaled tile size */
        double tile_w_px_scaled = (double)cell_pixels * zoom;
        double tile_h_px_scaled = (double)(cell_pixels / 2) * zoom;
        if (tile_w_px_scaled < 2.0) {
            tile_w_px_scaled = 2.0;
        }
        if (tile_h_px_scaled < 1.0) {
            tile_h_px_scaled = 1.0;
        }

        /* Clamp camera to world bounds */
        double world_w = zone.width * tile_w_px_scaled;
        double world_h = zone.height * tile_h_px_scaled;
        if (cam_x < 0.0) {
            cam_x = 0.0;
        }
        if (cam_y < 0.0) {
            cam_y = 0.0;
        }
        if (cam_x > world_w - win_w) {
            cam_x = world_w - win_w;
        }
        if (cam_y > world_h - win_h) {
            cam_y = world_h - win_h;
        }
        if (world_w < win_w) {
            cam_x = 0.0;
        }
        if (world_h < win_h) {
            cam_y = 0.0;
        }

        /* Calculate visible tile range */
        int x0 = (int)(cam_x / tile_w_px_scaled);
        int y0 = (int)(cam_y / tile_h_px_scaled);
        int x1 = (int)((cam_x + win_w) / tile_w_px_scaled) + 1;
        int y1 = (int)((cam_y + win_h) / tile_h_px_scaled) + 1;
        if (x0 < 0) {
            x0 = 0;
        }
        if (y0 < 0) {
            y0 = 0;
        }
        if (x1 > zone.width) {
            x1 = zone.width;
        }
        if (y1 > zone.height) {
            y1 = zone.height;
        }

        /* Render tiles */
        for (int gy = y0; gy < y1; ++gy) {
            for (int gx = x0; gx < x1; ++gx) {
                const Sector* sec = zone_get_sector(&zone, (uint16_t)gx, (uint16_t)gy);
                uint16_t sid = (sec ? sec->sprite_id : 0xFFFF);

                int dst_x = (int)(gx * tile_w_px_scaled - cam_x);
                int dst_y = (int)(gy * tile_h_px_scaled - cam_y);
                int cell_w = (int)tile_w_px_scaled;
                int cell_h = (int)tile_h_px_scaled;
                SDL_Rect dst_rect = { dst_x, dst_y, cell_w, cell_h };

                if (sid == 0xFFFF || sid >= tile_pack.count) {
                    /* Missing sprite - draw dark gray */
                    SDL_SetRenderDrawColor(framework.renderer, 16, 16, 16, 255);
                    SDL_RenderFillRect(framework.renderer, &dst_rect);
                } else if (tile_have[sid] == 1 && tile_cache[sid].texture) {
                    /* Cached texture - render */
                    int tw = tile_cache[sid].width;
                    int th = tile_cache[sid].height;
                    if (tw > 0 && th > 0) {
                        /* Scale maintaining aspect ratio */
                        float scale_x = (float)cell_w / (float)tw;
                        float scale_y = (float)cell_h / (float)th;
                        float scale = (scale_x < scale_y) ? scale_x : scale_y;
                        int draw_w = (int)(tw * scale);
                        int draw_h = (int)(th * scale);
                        SDL_Rect draw_rect = {
                            dst_x + (cell_w - draw_w) / 2,
                            dst_y + (cell_h - draw_h) / 2,
                            draw_w,
                            draw_h
                        };
                        SDL_RenderCopy(framework.renderer, tile_cache[sid].texture, NULL, &draw_rect);
                    } else {
                        SDL_SetRenderDrawColor(framework.renderer, 48, 48, 48, 255);
                        SDL_RenderFillRect(framework.renderer, &dst_rect);
                    }
                } else {
                    /* Decode sprite on-demand */
                    Sprite* sp = spritepack_get(&tile_pack, sid);
                    if (sp && sp->is_valid) {
                        if (sprite_decode(sp, &tile_cache[sid], 0) == 0) {
                            if (decoded_sprite_create_texture(&tile_cache[sid], framework.renderer) == 0) {
                                tile_have[sid] = 1;
                                if (tile_cache[sid].texture) {
                                    int tw = tile_cache[sid].width;
                                    int th = tile_cache[sid].height;
                                    if (tw > 0 && th > 0) {
                                        float scale_x = (float)cell_w / (float)tw;
                                        float scale_y = (float)cell_h / (float)th;
                                        float scale = (scale_x < scale_y) ? scale_x : scale_y;
                                        int draw_w = (int)(tw * scale);
                                        int draw_h = (int)(th * scale);
                                        SDL_Rect draw_rect = {
                                            dst_x + (cell_w - draw_w) / 2,
                                            dst_y + (cell_h - draw_h) / 2,
                                            draw_w,
                                            draw_h
                                        };
                                        SDL_RenderCopy(framework.renderer, tile_cache[sid].texture, NULL, &draw_rect);
                                    }
                                }
                            } else {
                                /* Texture creation failed */
                                decoded_sprite_free(&tile_cache[sid]);
                                tile_have[sid] = 0xFF;
                                SDL_SetRenderDrawColor(framework.renderer, 255, 0, 255, 255);
                                SDL_RenderFillRect(framework.renderer, &dst_rect);
                            }
                        } else {
                            /* Decode failed */
                            tile_have[sid] = 0xFF;
                            SDL_SetRenderDrawColor(framework.renderer, 255, 0, 255, 255);
                            SDL_RenderFillRect(framework.renderer, &dst_rect);
                        }
                    } else {
                        /* Invalid sprite */
                        SDL_SetRenderDrawColor(framework.renderer, 32, 32, 32, 255);
                        SDL_RenderFillRect(framework.renderer, &dst_rect);
                    }
                }

                /* Draw grid */
                if (show_grid) {
                    SDL_SetRenderDrawColor(framework.renderer, 64, 64, 64, 255);
                    SDL_RenderDrawRect(framework.renderer, &dst_rect);
                }
            }
        }

        /* Render ImageObjects (sorted by Y for proper depth) */
        if (has_obj_pack && show_objects && zone.image_objects && zone.image_object_count > 0) {
            double scale_x = tile_w_px_scaled / (double)DEFAULT_CELL_PIXELS;
            double scale_y = tile_h_px_scaled / (double)(DEFAULT_CELL_PIXELS / 2);
            if (scale_y <= 0.0) {
                scale_y = 1.0;
            }

            /* Collect visible objects */
            typedef struct {
                uint32_t idx;
                int sort_y;
                int sort_x;
            } VisibleObject;
            VisibleObject* visible = (VisibleObject*)malloc(sizeof(VisibleObject) * zone.image_object_count);
            uint32_t visible_count = 0;

            for (uint32_t i = 0; i < zone.image_object_count; ++i) {
                const ImageObject* obj = &zone.image_objects[i];

                /* Skip creatures and effects */
                if (obj->type == OBJECT_TYPE_CREATURE || obj->type == OBJECT_TYPE_EFFECT) {
                    continue;
                }

                /* Check sprite validity */
                if (obj->sprite_id == 0xFFFF || obj->sprite_id >= obj_pack.count) {
                    continue;
                }

                /* Calculate screen position */
                int screen_x = (int)(obj->pixel_x * scale_x - cam_x);
                int screen_y = (int)(obj->pixel_y * scale_y - cam_y);

                /* Rough culling */
                const int margin = 256;
                if (screen_x < -margin || screen_y < -margin ||
                    screen_x > win_w + margin || screen_y > win_h + margin) {
                    continue;
                }

                visible[visible_count].idx = i;
                visible[visible_count].sort_y = (int)(obj->pixel_y * scale_y);
                visible[visible_count].sort_x = (int)(obj->pixel_x * scale_x);
                visible_count++;
            }

            /* Sort by Y (descending), then X for proper depth */
            if (visible_count > 1) {
                for (uint32_t i = 1; i < visible_count; ++i) {
                    VisibleObject key = visible[i];
                    int j = (int)i - 1;
                    while (j >= 0) {
                        if (visible[j].sort_y < key.sort_y) {
                            break;
                        }
                        if (visible[j].sort_y == key.sort_y && visible[j].sort_x <= key.sort_x) {
                            break;
                        }
                        visible[j + 1] = visible[j];
                        j--;
                    }
                    visible[j + 1] = key;
                }
            }

            /* Render sorted objects */
            for (uint32_t i = 0; i < visible_count; ++i) {
                const ImageObject* obj = &zone.image_objects[visible[i].idx];

                /* Ensure texture is loaded */
                if (obj_have[obj->sprite_id] != 1 || !obj_cache[obj->sprite_id].texture) {
                    Sprite* sp = spritepack_get(&obj_pack, obj->sprite_id);
                    if (sp && sp->is_valid) {
                        if (sprite_decode(sp, &obj_cache[obj->sprite_id], 0) == 0) {
                            if (decoded_sprite_create_texture(&obj_cache[obj->sprite_id], framework.renderer) == 0) {
                                obj_have[obj->sprite_id] = 1;
                            } else {
                                decoded_sprite_free(&obj_cache[obj->sprite_id]);
                                obj_have[obj->sprite_id] = 0xFF;
                            }
                        } else {
                            obj_have[obj->sprite_id] = 0xFF;
                        }
                    } else {
                        obj_have[obj->sprite_id] = 0xFF;
                    }
                }

                if (obj_have[obj->sprite_id] != 1 || !obj_cache[obj->sprite_id].texture) {
                    continue;
                }

                /* Calculate scaled size */
                int draw_w = (int)(obj_cache[obj->sprite_id].width * scale_x);
                int draw_h = (int)(obj_cache[obj->sprite_id].height * scale_y);
                if (draw_w <= 0 || draw_h <= 0) {
                    continue;
                }

                /* Calculate destination rect */
                int dst_x = (int)(obj->pixel_x * scale_x - cam_x);
                int dst_y = (int)(obj->pixel_y * scale_y - cam_y);
                SDL_Rect dst_rect = { dst_x, dst_y, draw_w, draw_h };

                /* Cull off-screen objects */
                if (dst_rect.x > win_w || dst_rect.y > win_h ||
                    dst_rect.x + dst_rect.w < 0 || dst_rect.y + dst_rect.h < 0) {
                    continue;
                }

                /* Handle HALF transparency */
                if (obj->trans_flags & 2) {
                    SDL_SetTextureAlphaMod(obj_cache[obj->sprite_id].texture, 128);
                } else {
                    SDL_SetTextureAlphaMod(obj_cache[obj->sprite_id].texture, 255);
                }

                SDL_RenderCopy(framework.renderer, obj_cache[obj->sprite_id].texture, NULL, &dst_rect);
            }

            free(visible);
        }

        /* End frame */
        sdl_framework_end_frame(&framework);
    }

    /* Cleanup */
    printf("\nCleaning up...\n");

    for (uint32_t i = 0; i < tile_pack.count; i++) {
        if (tile_have[i] == 1) {
            decoded_sprite_free(&tile_cache[i]);
        }
    }
    free(tile_have);
    free(tile_cache);

    if (has_obj_pack) {
        for (uint32_t i = 0; i < obj_pack.count; i++) {
            if (obj_have[i] == 1) {
                decoded_sprite_free(&obj_cache[i]);
            }
        }
        free(obj_have);
        free(obj_cache);
        spritepack_free(&obj_pack);
    }

    spritepack_free(&tile_pack);
    zone_free(&zone);
    sdl_framework_cleanup(&framework);

    printf("Viewer closed\n");
    return 0;
}
