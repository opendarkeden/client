/*
 * client/resurrected/tools/map_viewer/main.c
 *
 * Map Viewer (Zone + SDL)
 *
 * Purpose:
 *  - 使用 resurrected 的 zone 解析与 spritepack 解码，在 SDL 中浏览整个地图。
 *  - 默认相机模式（平移/缩放/拖拽），网格比例 2:1（宽=cell，高=cell/2）。
 */

#include "sdl_framework.h"
#include "spritepack.h"
#include "sprite.h"
#include "zone.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define DEFAULT_CELL_PIXELS 32
#define DEFAULT_RECT_W 40
#define DEFAULT_RECT_H 30
#define MAX_DETECT_SCAN (64 * 1024) /* scan up to 64KB for candidate sector table */
#define SECTOR_RECORD_SIZE 4        /* bytes per sector in .map: spriteID(2) + prop(1) + light(1) */

static void print_usage(const char* prog) {
    printf("MapViewer (Zone + SDL)\n");
    printf("用途: 以相机浏览整个 zone 地图，并渲染地表 tile。\n");
    printf("\n用法:\n  %s <zone.map> <tile.spk> [--cell PIXELS]\n", prog);
    printf("\n示例:\n  %s DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk --cell 32\n", prog);
    printf("\n参数:\n");
    printf("  --cell N   格子宽度像素，默认 32。格子高度为宽度的一半 (2:1)。\n");
    printf("\n默认行为:\n");
    printf("  - 启动即为全图相机模式（无需 --full）。\n");
    printf("  - 网格比例为 2:1：宽=cell，高=cell/2（例如 cell=32 → 32x16）。\n");
    printf("\n交互:\n");
    printf("  - 平移：W/A/S/D 或方向键\n");
    printf("  - 缩放：+ / - 或鼠标滚轮（围绕屏幕中心缩放）\n");
    printf("  - 拖拽平移：右键拖动\n");
    printf("  - 重置视角：1 或 Home\n");
    printf("  - 网格开关：G\n");
}

/* detect_sector_table_offset no longer used by MapViewer.
 * map_loader handles header parsing and heuristic detection.
 * Keep a minimal stub for compatibility should any other code reference it.
 */
/* 仅保留 zone 加载与 SDL 渲染。 */

int main(int argc, char** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char* map_path = argv[1];
    const char* spk_path = argv[2];

    int view_tiles_w = DEFAULT_RECT_W;
    int view_tiles_h = DEFAULT_RECT_H;
    int cell_pixels = DEFAULT_CELL_PIXELS;

    /* parse optional args */
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--cell") == 0 && (i + 1) < argc) {
            cell_pixels = atoi(argv[i + 1]);
            if (cell_pixels <= 0) cell_pixels = DEFAULT_CELL_PIXELS;
            i++;
            continue;
        }
        /* 仅支持 --cell；其余参数忽略 */
    }

    if (view_tiles_w <= 0 || view_tiles_h <= 0) {
        fprintf(stderr, "Invalid rectangle dimensions\n");
        return 1;
    }

    /* 加载 Zone 数据 */
    Zone zone;
    Error* zerr = zone_load(map_path, &zone);
    if (zerr != NULL) {
        fprintf(stderr, "Failed to load zone: %s\n", zone_strerror((ZoneError)zerr->code));
        err_print(zerr, stderr);
        return 1;
    }

    /* 窗口初始大小基于视口 tiles（仅用于初始显示，实际靠相机缩放浏览全图） */

    /* init SDL framework: 默认 tile 宽=cell，高=cell/2 实现 2:1 比例 */
    int tile_w_px_init = cell_pixels;
    int tile_h_px_init = cell_pixels / 2;
    if (tile_h_px_init <= 0) tile_h_px_init = 1;

    SDLFrameworkConfig cfg = {
        .window_width = view_tiles_w * tile_w_px_init,
        .window_height = view_tiles_h * tile_h_px_init,
        .window_title = "Zone Viewer (SDL)",
        .target_fps = 30
    };

    SDLFramework fw;
    if (sdl_framework_init(&fw, &cfg) != 0) {
        fprintf(stderr, "Failed to initialize SDL framework\n");
        return 1;
    }

    /* load sprite pack lazily */
    SpritePack pack;
    if (spritepack_load_lazy(&pack, spk_path) != 0) {
        fprintf(stderr, "Failed to load sprite pack: %s\n", spk_path);
        sdl_framework_cleanup(&fw);
        return 1;
    }
    printf("Loaded sprite pack (count=%u)\n", (unsigned)pack.count);

    /* 全图模式：贴图懒加载，无需预读 sids */

    /* prepare decode cache */
    DecodedSprite* cache = (DecodedSprite*)calloc((size_t)pack.count, sizeof(DecodedSprite));
    if (!cache) {
        fprintf(stderr, "Out of memory (cache)\n");
        spritepack_free(&pack);
        sdl_framework_cleanup(&fw);
        zone_free(&zone);
        return 1;
    }
    uint8_t* have = (uint8_t*)calloc((size_t)pack.count, 1);
    if (!have) {
        fprintf(stderr, "Out of memory (have)\n");
        free(cache);
        spritepack_free(&pack);
        sdl_framework_cleanup(&fw);
        zone_free(&zone);
        return 1;
    }

    /* 贴图完全懒加载，首次进入视口时解码并创建纹理 */

    /* main loop: render grid */
    /* 相机与交互（full 模式）*/
    double zoom = 1.0;
    int show_grid = 1;
    double cam_x = 0.0, cam_y = 0.0; /* 以屏幕像素为单位（缩放后世界像素）*/
    int dragging = 0; int drag_last_x = 0, drag_last_y = 0;

    SDL_Event ev;
    while (fw.running) {
        int win_w = 0, win_h = 0;
        SDL_GetRendererOutputSize(fw.renderer, &win_w, &win_h);
        while (sdl_framework_poll_events(&fw, &ev)) {
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_ESCAPE) {
                    fw.running = 0;
                } else {
                    /* 键盘相机控制 */
                    int step = (int)(32 * zoom);
                    if (step < 8) step = 8;
                    switch (ev.key.keysym.sym) {
                        case SDLK_LEFT:
                        case SDLK_a: cam_x -= step; break;
                        case SDLK_RIGHT:
                        case SDLK_d: cam_x += step; break;
                        case SDLK_UP:
                        case SDLK_w: cam_y -= step; break;
                        case SDLK_DOWN:
                        case SDLK_s: cam_y += step; break;
                        case SDLK_EQUALS: /* '=' '+' */
                        case SDLK_PLUS: {
                            double old = zoom; zoom *= 1.25; if (zoom > 6.0) zoom = 6.0;
                            double u_cx = (cam_x + win_w * 0.5) / old;
                            double u_cy = (cam_y + win_h * 0.5) / old;
                            cam_x = u_cx * zoom - win_w * 0.5;
                            cam_y = u_cy * zoom - win_h * 0.5;
                            break;
                        }
                        case SDLK_MINUS: {
                            double old = zoom; zoom /= 1.25; if (zoom < 0.25) zoom = 0.25;
                            double u_cx = (cam_x + win_w * 0.5) / old;
                            double u_cy = (cam_y + win_h * 0.5) / old;
                            cam_x = u_cx * zoom - win_w * 0.5;
                            cam_y = u_cy * zoom - win_h * 0.5;
                            break;
                        }
                        case SDLK_1: /* 重置 */
                        case SDLK_HOME: zoom = 1.0; cam_x = cam_y = 0.0; break;
                        case SDLK_g: show_grid = !show_grid; break;
                        default: break;
                    }
                }
            } else if (ev.type == SDL_MOUSEWHEEL) {
                double old = zoom;
                if (ev.wheel.y > 0) zoom *= 1.25; else if (ev.wheel.y < 0) zoom /= 1.25;
                if (zoom < 0.25) zoom = 0.25; if (zoom > 6.0) zoom = 6.0;
                /* 围绕屏幕中心缩放 */
                double u_cx = (cam_x + win_w * 0.5) / old;
                double u_cy = (cam_y + win_h * 0.5) / old;
                cam_x = u_cx * zoom - win_w * 0.5;
                cam_y = u_cy * zoom - win_h * 0.5;
            } else if ((ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP || ev.type == SDL_MOUSEMOTION)) {
                if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_RIGHT) {
                    dragging = 1; drag_last_x = ev.button.x; drag_last_y = ev.button.y;
                } else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_RIGHT) {
                    dragging = 0;
                } else if (ev.type == SDL_MOUSEMOTION && dragging) {
                    int dx = ev.motion.x - drag_last_x;
                    int dy = ev.motion.y - drag_last_y;
                    cam_x -= dx; cam_y -= dy;
                    drag_last_x = ev.motion.x; drag_last_y = ev.motion.y;
                }
            }
        }

        sdl_framework_begin_frame(&fw);

        {
            /* 相机模式：根据缩放裁剪可见 tiles 并渲染 */
            double tile_w_px = (double)cell_pixels * zoom;
            double tile_h_px = (double)(cell_pixels / 2) * zoom;
            if (tile_h_px < 1.0) tile_h_px = 1.0;
            if (tile_w_px < 2.0) tile_w_px = 2.0;

            /* 使用上面读取的 win_w/win_h */

            /* 世界尺寸（缩放后像素）用于相机约束 */
            double world_w = zone.width * tile_w_px;
            double world_h = zone.height * tile_h_px;

            /* 夹紧相机到世界 */
            if (cam_x < 0.0) cam_x = 0.0; if (cam_y < 0.0) cam_y = 0.0;
            if (cam_x > world_w - win_w) cam_x = world_w - win_w;
            if (cam_y > world_h - win_h) cam_y = world_h - win_h;
            if (world_w < win_w) cam_x = 0.0; if (world_h < win_h) cam_y = 0.0;

            int x0 = (int)(cam_x / tile_w_px);
            int y0 = (int)(cam_y / tile_h_px);
            int x1 = (int)((cam_x + win_w) / tile_w_px) + 1;
            int y1 = (int)((cam_y + win_h) / tile_h_px) + 1;
            if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
            if (x1 > zone.width) x1 = zone.width;
            if (y1 > zone.height) y1 = zone.height;

            for (int gy = y0; gy < y1; ++gy) {
                for (int gx = x0; gx < x1; ++gx) {
                    const Sector* sec = zone_get_sector(&zone, (uint16_t)gx, (uint16_t)gy);
                    uint16_t sid = (sec ? sec->sprite_id : 0xFFFF);
                    int dst_x = (int)(gx * tile_w_px - cam_x);
                    int dst_y = (int)(gy * tile_h_px - cam_y);
                    int cell_w = (int)tile_w_px;
                    int cell_h = (int)tile_h_px;
                    SDL_Rect dst = { dst_x, dst_y, cell_w, cell_h };

                    if (sid == 0xFFFF || sid >= pack.count) {
                        SDL_SetRenderDrawColor(fw.renderer, 16, 16, 16, 255);
                        SDL_RenderFillRect(fw.renderer, &dst);
                    } else if (have[sid] == 1 && cache[sid].texture) {
                        int tw = cache[sid].width;
                        int th = cache[sid].height;
                        if (tw > 0 && th > 0) {
                            float sx = (float)cell_w / (float)tw;
                            float sy = (float)cell_h / (float)th;
                            float smin = sx < sy ? sx : sy;
                            int dw = (int)(tw * smin);
                            int dh = (int)(th * smin);
                            SDL_Rect dd = { dst_x + (cell_w - dw) / 2, dst_y + (cell_h - dh) / 2, dw, dh };
                            SDL_RenderCopy(fw.renderer, cache[sid].texture, NULL, &dd);
                        } else {
                            SDL_SetRenderDrawColor(fw.renderer, 48, 48, 48, 255);
                            SDL_RenderFillRect(fw.renderer, &dst);
                        }
                    } else {
                        Sprite* sp = spritepack_get(&pack, sid);
                        if (sp && sp->is_valid) {
                            if (sprite_decode(sp, &cache[sid], 0) == 0) {
                                if (decoded_sprite_create_texture(&cache[sid], fw.renderer) == 0) {
                                    have[sid] = 1;
                                    if (cache[sid].texture) {
                                        int tw = cache[sid].width; int th = cache[sid].height;
                                        if (tw > 0 && th > 0) {
                                            float sx = (float)cell_w / (float)tw;
                                            float sy = (float)cell_h / (float)th;
                                            float smin = sx < sy ? sx : sy;
                                            int dw = (int)(tw * smin);
                                            int dh = (int)(th * smin);
                                            SDL_Rect dd = { dst_x + (cell_w - dw) / 2, dst_y + (cell_h - dh) / 2, dw, dh };
                                            SDL_RenderCopy(fw.renderer, cache[sid].texture, NULL, &dd);
                                        }
                                    }
                                } else {
                                    decoded_sprite_free(&cache[sid]);
                                    have[sid] = 0xFF;
                                    SDL_SetRenderDrawColor(fw.renderer, 255, 0, 255, 255);
                                    SDL_RenderFillRect(fw.renderer, &dst);
                                }
                            } else {
                                have[sid] = 0xFF;
                                SDL_SetRenderDrawColor(fw.renderer, 255, 0, 255, 255);
                                SDL_RenderFillRect(fw.renderer, &dst);
                            }
                        } else {
                            SDL_SetRenderDrawColor(fw.renderer, 32, 32, 32, 255);
                            SDL_RenderFillRect(fw.renderer, &dst);
                        }
                    }

                    if (show_grid) {
                        SDL_SetRenderDrawColor(fw.renderer, 64, 64, 64, 255);
                        SDL_RenderDrawRect(fw.renderer, &dst);
                    }
                }
            }
        }

        sdl_framework_end_frame(&fw);
    }

    /* cleanup */
    for (uint32_t i = 0; i < pack.count; i++) {
        if (have[i] == 1) decoded_sprite_free(&cache[i]);
    }
    free(have);
    free(cache);

    /* 释放 Zone 资源 */
    zone_free(&zone);

    spritepack_free(&pack);
    sdl_framework_cleanup(&fw);

    return 0;

    return 0;
}
