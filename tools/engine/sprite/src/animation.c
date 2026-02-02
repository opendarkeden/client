/**
 * @file animation.c
 * @brief Animation frame and object management implementation
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 3.1, 3.2, 3.3, 3.5, 3.6, 6.1, 6.2
 */

#include "animation.h"
#include "color.h"
#include <string.h>

/* ============================================================================
 * Animation Frame Implementation
 * ============================================================================ */

void anim_frame_init(AnimFrame* frame, uint8_t blt_type) {
    if (frame == NULL) {
        return;
    }
    
    frame->frame_id = FRAMEID_NULL;
    frame->current_frame = 0;
    frame->max_frame = 1;
    frame->blt_type = blt_type;
    frame->loop = 0;
}

void anim_frame_set(AnimFrame* frame, FrameID id, uint8_t max_frame) {
    if (frame == NULL) {
        return;
    }
    
    frame->frame_id = id;
    frame->max_frame = (max_frame > 0) ? max_frame : 1;
    frame->current_frame = 0;
}

void anim_frame_next(AnimFrame* frame) {
    if (frame == NULL || frame->max_frame == 0) {
        return;
    }
    
    frame->current_frame++;
    if (frame->current_frame >= frame->max_frame) {
        frame->current_frame = 0;
    }
}

void anim_frame_next_loop(AnimFrame* frame, uint32_t loop_counter) {
    if (frame == NULL || frame->max_frame == 0) {
        return;
    }
    
    frame->current_frame = (uint8_t)(loop_counter % frame->max_frame);
}

uint8_t anim_frame_get(const AnimFrame* frame) {
    if (frame == NULL) {
        return 0;
    }
    
    return frame->current_frame;
}

/* ============================================================================
 * Animation Object Implementation
 * ============================================================================ */

void anim_object_init(AnimObject* obj) {
    if (obj == NULL) {
        return;
    }
    
    anim_frame_init(&obj->frame, BLT_NORMAL);
    obj->sprite_id = SPRITEID_NULL;
    obj->pixel_x = 0;
    obj->pixel_y = 0;
    obj->direction = 0;
    obj->transparent = 0;
}

void anim_object_set(AnimObject* obj, SpriteID sprite_id, 
                     int px, int py, uint8_t direction, uint8_t trans) {
    if (obj == NULL) {
        return;
    }
    
    obj->sprite_id = sprite_id;
    obj->pixel_x = px;
    obj->pixel_y = py;
    obj->direction = direction % DIR_MAX;
    obj->transparent = trans;
}

void anim_object_set_frame(AnimObject* obj, FrameID frame_id, uint8_t max_frame) {
    if (obj == NULL) {
        return;
    }
    
    anim_frame_set(&obj->frame, frame_id, max_frame);
}

void anim_object_next_frame(AnimObject* obj) {
    if (obj == NULL) {
        return;
    }
    
    anim_frame_next(&obj->frame);
}

SpriteID anim_object_get_sprite(const AnimObject* obj) {
    if (obj == NULL || obj->sprite_id == SPRITEID_NULL) {
        return SPRITEID_NULL;
    }
    
    return obj->sprite_id + anim_frame_get(&obj->frame);
}

void anim_object_set_direction(AnimObject* obj, uint8_t dir) {
    if (obj == NULL) {
        return;
    }
    
    obj->direction = dir % DIR_MAX;
}

/* ============================================================================
 * Animation Rendering Implementation
 * ============================================================================ */

void anim_set_blend_mode(SDL_Texture* texture, BltType blt_type) {
    if (texture == NULL) {
        return;
    }
    
    switch (blt_type) {
        case BLT_NORMAL:
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_SetTextureAlphaMod(texture, 255);
            break;
            
        case BLT_EFFECT:
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_SetTextureAlphaMod(texture, 255);
            break;
            
        case BLT_SHADOW:
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            /* Darken the texture for shadow effect */
            SDL_SetTextureColorMod(texture, 0, 0, 0);
            SDL_SetTextureAlphaMod(texture, 128);
            break;
            
        case BLT_SCREEN:
            /* Screen blend approximated with additive blending */
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_SetTextureAlphaMod(texture, 255);
            break;
            
        default:
            /* Default to normal blending */
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_SetTextureAlphaMod(texture, 255);
            break;
    }
}

int anim_render(SDL_Renderer* renderer, 
                const AnimObject* obj,
                SpritePack* pack,
                DecodedSprite* cache,
                uint16_t colorkey,
                float zoom) {
    if (obj == NULL) {
        return -1;
    }
    
    return anim_render_blt(renderer, obj, pack, cache, colorkey, zoom, 
                           (BltType)obj->frame.blt_type);
}

int anim_render_blt(SDL_Renderer* renderer,
                    const AnimObject* obj,
                    SpritePack* pack,
                    DecodedSprite* cache,
                    uint16_t colorkey,
                    float zoom,
                    BltType blt_type) {
    SpriteID sprite_id;
    Sprite* sprite;
    DecodedSprite local_decoded;
    DecodedSprite* decoded;
    SDL_Rect dst_rect;
    int result = 0;
    
    if (renderer == NULL || obj == NULL || pack == NULL) {
        return -1;
    }
    
    /* Get current sprite ID */
    sprite_id = anim_object_get_sprite(obj);
    if (sprite_id == SPRITEID_NULL) {
        return -1;
    }
    
    /* Check if sprite exists in pack */
    if (sprite_id >= pack->count) {
        return -1;
    }
    
    /* Use cache if provided, otherwise use local decoded sprite */
    decoded = cache ? cache : &local_decoded;
    
    /* Load and decode sprite if not using cache or cache is empty */
    if (!cache || cache->pixels == NULL) {
        sprite = spritepack_get(pack, sprite_id);
        if (sprite == NULL || !sprite->is_valid) {
            return -1;
        }
        
        if (sprite_decode(sprite, decoded, colorkey) != 0) {
            return -1;
        }
        
        /* Create texture */
        if (decoded_sprite_create_texture(decoded, renderer) != 0) {
            if (!cache) {
                decoded_sprite_free(decoded);
            }
            return -1;
        }
    }
    
    /* Set blend mode */
    anim_set_blend_mode(decoded->texture, blt_type);
    
    /* Calculate destination rectangle */
    dst_rect.x = obj->pixel_x;
    dst_rect.y = obj->pixel_y;
    dst_rect.w = (int)(decoded->width * zoom);
    dst_rect.h = (int)(decoded->height * zoom);
    
    /* Render */
    if (SDL_RenderCopy(renderer, decoded->texture, NULL, &dst_rect) != 0) {
        result = -1;
    }
    
    /* Clean up local decoded sprite if not using cache */
    if (!cache) {
        decoded_sprite_free(decoded);
    }
    
    return result;
}
