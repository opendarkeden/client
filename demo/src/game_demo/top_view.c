#include "top_view.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sdl_framework.h"
#include "zone.h"
#include "spritepack.h"
#include "sprite.h"
#include "index_spritepack.h"
#include "framepack.h"
#include "colorset.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct Player {
    double x, y;        /* world pixel position (anchor for drawing) */
    double tx, ty;      /* target world pixel */
    double vx, vy;      /* current velocity (pixels/sec) */
    int moving;         /* 1 if moving toward target */

    /* Animation state */
    int action;         /* 0=STAND, 1=WALK */
    int direction;      /* 0..7 per original */
    int frame;          /* current frame index */
    int max_frames;     /* frames for current action+direction */
    uint32_t last_frame_time; /* SDL_GetTicks() */
    uint32_t frame_interval_ms; /* ms per frame */
} Player;

struct TopView {
    SDLFramework fw;

    /* Resources */
    Zone zone;
    SpritePack tile_pack;
    IndexSpritePack creature_ispk;
    CreatureFramePack creature_cfpk;

    /* Decoded cache for tiles */
    DecodedSprite* tile_cache;
    uint8_t* tile_have;

    /* Creature decode cache (indexed) not needed; index sprites cache internally */

    /* Config */
    int cell_pixels;  /* width in pixels; height = cell_pixels/2 */
    float move_speed; /* pixels/sec */

    /* Camera */
    double cam_x, cam_y; /* top-left in world pixels */

    /* Player */
    Player player;

    /* Creature selection */
    int creature_type;      /* index in cfpk */
    int creature_action_walk; /* usually 1 */
    int creature_action_stand; /* usually 0 */
};

static int clamp_int(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

static void player_set_target(Player* p, double tx, double ty) {
    p->tx = tx; p->ty = ty; p->moving = 1;
}

static void update_direction_from_velocity(Player* p) {
    /* Map velocity vector to 8 directions used by assets */
    if (fabs(p->vx) < 1e-4 && fabs(p->vy) < 1e-4) return;
    double ang = atan2(p->vy, p->vx); /* -pi..pi, 0 is +X (RIGHT) */
    /* Define octants centered at RIGHT (dir=4) and proceed CCW */
    /* Directions: 0 LEFT,1 LD,2 DOWN,3 RD,4 RIGHT,5 RU,6 UP,7 LU */
    double step = M_PI / 4.0;
    int dir = 4; /* default RIGHT */
    if (ang >= -step/2 && ang < step/2) dir = 4;                 /* RIGHT */
    else if (ang >= step/2 && ang < 3*step/2) {                  /* lower half? Actually positive y is down in screen; atan2(vy,vx) uses math y+ up, but our vy>0 means down */
        /* We'll remap using explicit ranges to match screen coords (down is +y) */
    }
    /* Build mapping considering SDL screen: y down, but atan2 uses math y up. To align, invert vy */
}

/* Convert screen pixel to world pixel */
static void screen_to_world(TopView* tv, int sx, int sy, double* wx, double* wy) {
    *wx = tv->cam_x + sx;
    *wy = tv->cam_y + sy;
}

/* Determine frames for current action/direction */
static int compute_max_frames(CreatureFramePack* fpk, int ctype, int action, int dir) {
    ActionArray* aa = creature_framepack_get(fpk, ctype);
    if (!aa) return 0;
    DirectionArray* da = action_array_get(aa, action);
    if (!da) return 0;
    FrameArray* fa = direction_array_get(da, dir);
    if (!fa) return 0;
    return frame_array_size(fa);
}

static TYPE_SPRITEID get_frame_sprite(CreatureFramePack* fpk, int ctype, int action, int dir, int frame_idx, int16_t* cx, int16_t* cy) {
    *cx = 0; *cy = 0;
    Frame* fr = creature_framepack_get_frame(fpk, ctype, action, dir, frame_idx);
    if (!fr) return SPRITEID_NULL;
    *cx = fr->cx; *cy = fr->cy;
    return fr->sprite_id;
}

static void ensure_tile_cache(TopView* tv) {
    if (!tv->tile_cache) {
        uint32_t n = tv->tile_pack.count;
        tv->tile_cache = (DecodedSprite*)calloc(n, sizeof(DecodedSprite));
        tv->tile_have = (uint8_t*)calloc(n, 1);
    }
}

static void render_tiles(TopView* tv) {
    int win_w = 0, win_h = 0;
    SDL_GetRendererOutputSize(tv->fw.renderer, &win_w, &win_h);

    double tile_w = (double)tv->cell_pixels;
    double tile_h = (double)(tv->cell_pixels / 2);
    if (tile_h < 1.0) tile_h = 1.0;

    double world_w = tv->zone.width * tile_w;
    double world_h = tv->zone.height * tile_h;

    if (tv->cam_x < 0.0) tv->cam_x = 0.0; if (tv->cam_y < 0.0) tv->cam_y = 0.0;
    if (tv->cam_x > world_w - win_w) tv->cam_x = world_w - win_w;
    if (tv->cam_y > world_h - win_h) tv->cam_y = world_h - win_h;
    if (world_w < win_w) tv->cam_x = 0.0; if (world_h < win_h) tv->cam_y = 0.0;

    int x0 = (int)(tv->cam_x / tile_w);
    int y0 = (int)(tv->cam_y / tile_h);
    int x1 = (int)((tv->cam_x + win_w) / tile_w) + 1;
    int y1 = (int)((tv->cam_y + win_h) / tile_h) + 1;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > tv->zone.width) x1 = tv->zone.width;
    if (y1 > tv->zone.height) y1 = tv->zone.height;

    ensure_tile_cache(tv);

    for (int gy = y0; gy < y1; ++gy) {
        for (int gx = x0; gx < x1; ++gx) {
            const Sector* sec = zone_get_sector(&tv->zone, (uint16_t)gx, (uint16_t)gy);
            uint16_t sid = (sec ? sec->sprite_id : 0xFFFF);
            int dst_x = (int)(gx * tile_w - tv->cam_x);
            int dst_y = (int)(gy * tile_h - tv->cam_y);
            int cell_w = (int)tile_w;
            int cell_h = (int)tile_h;
            SDL_Rect dst = { dst_x, dst_y, cell_w, cell_h };

            if (sid == 0xFFFF || sid >= spritepack_size(&tv->tile_pack)) {
                SDL_SetRenderDrawColor(tv->fw.renderer, 16, 16, 16, 255);
                SDL_RenderFillRect(tv->fw.renderer, &dst);
                continue;
            }

            if (tv->tile_have[sid] == 1 && tv->tile_cache[sid].texture) {
                int tw = tv->tile_cache[sid].width;
                int th = tv->tile_cache[sid].height;
                float sx = (float)cell_w / (float)tw;
                float sy = (float)cell_h / (float)th;
                float smin = sx < sy ? sx : sy;
                int dw = (int)(tw * smin);
                int dh = (int)(th * smin);
                SDL_Rect dd = { dst_x + (cell_w - dw) / 2, dst_y + (cell_h - dh) / 2, dw, dh };
                SDL_RenderCopy(tv->fw.renderer, tv->tile_cache[sid].texture, NULL, &dd);
            } else {
                Sprite* sp = spritepack_get(&tv->tile_pack, sid);
                if (sp && sp->is_valid) {
                    if (sprite_decode(sp, &tv->tile_cache[sid], 0) == 0) {
                        if (decoded_sprite_create_texture(&tv->tile_cache[sid], tv->fw.renderer) == 0) {
                            tv->tile_have[sid] = 1;
                            int tw = tv->tile_cache[sid].width;
                            int th = tv->tile_cache[sid].height;
                            float sx = (float)cell_w / (float)tw;
                            float sy = (float)cell_h / (float)th;
                            float smin = sx < sy ? sx : sy;
                            int dw = (int)(tw * smin);
                            int dh = (int)(th * smin);
                            SDL_Rect dd = { dst_x + (cell_w - dw) / 2, dst_y + (cell_h - dh) / 2, dw, dh };
                            SDL_RenderCopy(tv->fw.renderer, tv->tile_cache[sid].texture, NULL, &dd);
                        } else {
                            decoded_sprite_free(&tv->tile_cache[sid]);
                            tv->tile_have[sid] = 0xFF;
                        }
                    } else {
                        tv->tile_have[sid] = 0xFF;
                    }
                }
            }
        }
    }
}

static int map_direction_from_velocity(double vx, double vy) {
    /* SDL screen coords: +x right, +y down. atan2 expects y up, so invert vy */
    double ang = atan2(-vy, vx); /* -pi..pi, 0: RIGHT */
    double step = M_PI / 4.0;
    double a = ang;
    if (a < -M_PI) a += 2*M_PI; if (a > M_PI) a -= 2*M_PI;
    int oct = (int)floor((a + M_PI) / step); /* 0..7 starting from -pi */
    /* Map octants to our dir indices */
    /* Octant centers (starting -pi): -157.5,-112.5,-67.5,-22.5,22.5,67.5,112.5,157.5 */
    static const int map[8] = { 0, 7, 6, 5, 4, 3, 2, 1 };
    if (oct < 0) oct = 0; if (oct > 7) oct = 7;
    return map[oct];
}

static void update_player(TopView* tv, uint32_t dt_ms) {
    Player* p = &tv->player;
    double dt = dt_ms / 1000.0;
    if (p->moving) {
        double dx = p->tx - p->x;
        double dy = p->ty - p->y;
        double dist = sqrt(dx*dx + dy*dy);
        double step = tv->move_speed * dt;
        if (dist <= step || dist < 0.5) {
            p->x = p->tx; p->y = p->ty; p->vx = p->vy = 0.0; p->moving = 0;
            p->action = tv->creature_action_stand;
        } else {
            double nx = dx / dist, ny = dy / dist;
            p->vx = nx * tv->move_speed;
            p->vy = ny * tv->move_speed;
            p->x += nx * step;
            p->y += ny * step;
            p->action = tv->creature_action_walk;
            p->direction = map_direction_from_velocity(p->vx, p->vy);
        }
    } else {
        p->vx = p->vy = 0.0;
        p->action = tv->creature_action_stand;
    }

    /* Update animation frames */
    int maxf = compute_max_frames(&tv->creature_cfpk, tv->creature_type, p->action, p->direction);
    p->max_frames = maxf;
    if (maxf <= 0) { p->frame = 0; return; }

    uint32_t now = SDL_GetTicks();
    if (now - p->last_frame_time >= p->frame_interval_ms) {
        p->frame = (p->frame + 1) % maxf;
        p->last_frame_time = now;
    }
}

static void render_player(TopView* tv) {
    int win_w = 0, win_h = 0;
    SDL_GetRendererOutputSize(tv->fw.renderer, &win_w, &win_h);

    /* We draw player at screen center (camera follows player) */
    int center_x = win_w / 2;
    int center_y = win_h / 2;

    /* Fetch current frame sprite */
    int16_t cx = 0, cy = 0;
    TYPE_SPRITEID sid = get_frame_sprite(&tv->creature_cfpk, tv->creature_type, tv->player.action, tv->player.direction, tv->player.frame, &cx, &cy);
    if (sid == SPRITEID_NULL || sid >= index_spritepack_get_size(&tv->creature_ispk)) return;

    IndexSprite* sprite = index_spritepack_get(&tv->creature_ispk, sid);
    if (!sprite || !index_sprite_is_init(sprite)) return;

    /* Apply colorset 0 for now */
    colorset_set_using(0, 0);

    int draw_x = center_x + cx;
    int draw_y = center_y + cy;
    index_sprite_render(sprite, tv->fw.renderer, draw_x, draw_y);
}

static void center_camera_on_player(TopView* tv) {
    int win_w = 0, win_h = 0;
    SDL_GetRendererOutputSize(tv->fw.renderer, &win_w, &win_h);
    tv->cam_x = tv->player.x - win_w * 0.5;
    tv->cam_y = tv->player.y - win_h * 0.5;
}

TopView* top_view_create(void) {
    TopView* tv = (TopView*)calloc(1, sizeof(TopView));
    return tv;
}

int top_view_init(TopView* tv, const TopViewConfig* cfg) {
    if (!tv || !cfg) return -1;

    /* SDL init */
    SDLFrameworkConfig sdl_cfg = {
        .window_width = cfg->window_width > 0 ? cfg->window_width : 960,
        .window_height = cfg->window_height > 0 ? cfg->window_height : 540,
        .window_title = "TopView (Game MVP)",
        .target_fps = 60
    };
    if (sdl_framework_init(&tv->fw, &sdl_cfg) != 0) return -2;

    /* Load zone */
    Error* zerr = zone_load(cfg->map_path, &tv->zone);
    if (zerr) {
        sdl_framework_cleanup(&tv->fw);
        return -3;
    }

    /* Load tile sprite pack */
    /* Load tile pack lazily: returns 0 on success */
    if (spritepack_load_lazy(&tv->tile_pack, cfg->tile_spk_path) != 0) {
        zone_free(&tv->zone);
        sdl_framework_cleanup(&tv->fw);
        return -4;
    }

    /* Load creature packs */
    char ispk[1024]; char cfpk[1024];
    snprintf(ispk, sizeof(ispk), "%s.ispk", cfg->creature_base_path);
    snprintf(cfpk, sizeof(cfpk), "%s.cfpk", cfg->creature_base_path);

    index_spritepack_init(&tv->creature_ispk);
    if (!index_spritepack_load_lazy(&tv->creature_ispk, ispk)) {
        spritepack_free(&tv->tile_pack);
        zone_free(&tv->zone);
        sdl_framework_cleanup(&tv->fw);
        return -5;
    }

    creature_framepack_init(&tv->creature_cfpk);
    if (!creature_framepack_load(&tv->creature_cfpk, cfpk)) {
        index_spritepack_release(&tv->creature_ispk);
        spritepack_free(&tv->tile_pack);
        zone_free(&tv->zone);
        sdl_framework_cleanup(&tv->fw);
        return -6;
    }

    /* Config */
    tv->cell_pixels = cfg->cell_pixels > 0 ? cfg->cell_pixels : 32;
    tv->move_speed = cfg->move_speed > 0 ? cfg->move_speed : 120.0f;

    /* Player init: spawn near center of zone */
    double tile_w = (double)tv->cell_pixels;
    double tile_h = (double)(tv->cell_pixels / 2);
    double spawn_x = (tv->zone.width * tile_w) * 0.5;
    double spawn_y = (tv->zone.height * tile_h) * 0.5;

    tv->player.x = spawn_x;
    tv->player.y = spawn_y;
    tv->player.tx = spawn_x;
    tv->player.ty = spawn_y;
    tv->player.vx = tv->player.vy = 0.0;
    tv->player.moving = 0;
    tv->player.action = 0; /* stand */
    tv->player.direction = 4; /* RIGHT default */
    tv->player.frame = 0;
    tv->player.frame_interval_ms = 100; /* 10 fps walk */
    tv->player.last_frame_time = SDL_GetTicks();

    /* Choose available actions */
    tv->creature_type = 0;
    tv->creature_action_stand = 0;
    tv->creature_action_walk = 1;
    tv->player.max_frames = compute_max_frames(&tv->creature_cfpk, tv->creature_type, tv->player.action, tv->player.direction);

    /* Camera init */
    center_camera_on_player(tv);

    /* Colorset init */
    colorset_init();
    colorset_set_using(0, 0);

    return 0;
}

void top_view_run(TopView* tv) {
    if (!tv) return;

    SDL_Event ev;
    uint32_t last = SDL_GetTicks();

    while (tv->fw.running) {
        uint32_t now = SDL_GetTicks();
        uint32_t dt = now - last; last = now;

        while (sdl_framework_poll_events(&tv->fw, &ev)) {
            if (ev.type == SDL_QUIT) tv->fw.running = 0;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) tv->fw.running = 0;
            if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
                double wx, wy; screen_to_world(tv, ev.button.x, ev.button.y, &wx, &wy);
                player_set_target(&tv->player, wx, wy);
            }
        }

        update_player(tv, dt);
        center_camera_on_player(tv);

        sdl_framework_begin_frame(&tv->fw);
        render_tiles(tv);
        render_player(tv);
        sdl_framework_end_frame(&tv->fw);

        sdl_framework_delay(&tv->fw, now);
    }
}

void top_view_destroy(TopView* tv) {
    if (!tv) return;

    /* Free tile cache */
    if (tv->tile_cache) {
        uint32_t n = tv->tile_pack.count;
        for (uint32_t i = 0; i < n; ++i) {
            if (tv->tile_have && tv->tile_have[i] == 1) decoded_sprite_free(&tv->tile_cache[i]);
        }
        free(tv->tile_cache);
        free(tv->tile_have);
    }

    creature_framepack_free(&tv->creature_cfpk);
    index_spritepack_release(&tv->creature_ispk);
    spritepack_free(&tv->tile_pack);
    zone_free(&tv->zone);
    sdl_framework_cleanup(&tv->fw);

    free(tv);
}
