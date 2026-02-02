/**
 * @file animation.h
 * @brief Animation frame and object management
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 3.1, 3.2, 3.3, 3.5, 3.6, 6.1, 6.2
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <stdint.h>
#include "types.h"
#include "sprite.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Animation Frame Structure
 * Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6
 * ============================================================================ */

/**
 * Animation frame state
 * 
 * Manages frame progression for animations, supporting both manual
 * frame advancement and loop-based frame calculation.
 */
typedef struct AnimFrame {
    FrameID frame_id;       /**< Frame group identifier (Req 2.2) */
    uint8_t current_frame;  /**< Current frame index (Req 2.1) */
    uint8_t max_frame;      /**< Maximum frame count (Req 2.1) */
    uint8_t blt_type;       /**< Rendering type (BltType) (Req 2.6) */
    uint8_t loop;           /**< Loop mode flag (Req 2.5) */
} AnimFrame;

/**
 * Initialize animation frame with default values
 * 
 * @param frame Pointer to AnimFrame structure
 * @param blt_type Initial rendering type (BltType)
 * 
 * Requirements: 2.1, 2.6
 */
void anim_frame_init(AnimFrame* frame, uint8_t blt_type);

/**
 * Set frame sequence parameters
 * 
 * @param frame Pointer to AnimFrame structure
 * @param id Frame group identifier
 * @param max_frame Maximum number of frames in sequence
 * 
 * Requirements: 2.1, 2.2
 */
void anim_frame_set(AnimFrame* frame, FrameID id, uint8_t max_frame);

/**
 * Advance to next frame
 * 
 * Increments current_frame by 1. When current_frame reaches max_frame,
 * it wraps back to 0.
 * 
 * @param frame Pointer to AnimFrame structure
 * 
 * Requirements: 2.3, 2.4
 */
void anim_frame_next(AnimFrame* frame);

/**
 * Advance frame in loop mode
 * 
 * In loop mode, the current frame is calculated as (loop_counter % max_frame).
 * This allows multiple animations to stay synchronized.
 * 
 * @param frame Pointer to AnimFrame structure
 * @param loop_counter Global loop counter value
 * 
 * Requirements: 2.5
 */
void anim_frame_next_loop(AnimFrame* frame, uint32_t loop_counter);

/**
 * Get current frame index
 * 
 * @param frame Pointer to AnimFrame structure
 * @return Current frame index
 * 
 * Requirements: 2.1
 */
uint8_t anim_frame_get(const AnimFrame* frame);

/* ============================================================================
 * Animation Object Structure
 * Requirements: 3.1, 3.2, 3.3, 3.5, 3.6, 6.1, 6.2
 * ============================================================================ */

/**
 * Animation object
 * 
 * Combines sprite data with frame management for rendering animated
 * game objects. Supports 8-directional animations.
 */
typedef struct AnimObject {
    AnimFrame frame;        /**< Frame management data (Req 3.4) */
    SpriteID sprite_id;     /**< Base sprite ID (Req 3.1) */
    int pixel_x;            /**< X position in pixels (Req 3.2) */
    int pixel_y;            /**< Y position in pixels (Req 3.2) */
    uint8_t direction;      /**< Direction (0-7) (Req 3.3, 6.1) */
    uint8_t transparent;    /**< Transparency flag (Req 3.6) */
} AnimObject;

/**
 * Initialize animation object with default values
 * 
 * @param obj Pointer to AnimObject structure
 * 
 * Requirements: 3.1, 3.2, 3.3, 3.6
 */
void anim_object_init(AnimObject* obj);

/**
 * Set animation object properties
 * 
 * @param obj Pointer to AnimObject structure
 * @param sprite_id Base sprite ID
 * @param px X pixel position
 * @param py Y pixel position
 * @param direction Direction (0-7, automatically wrapped)
 * @param trans Transparency flag
 * 
 * Requirements: 3.1, 3.2, 3.3, 3.6, 6.1
 */
void anim_object_set(AnimObject* obj, SpriteID sprite_id, 
                     int px, int py, uint8_t direction, uint8_t trans);

/**
 * Set frame sequence for animation
 * 
 * @param obj Pointer to AnimObject structure
 * @param frame_id Frame group identifier
 * @param max_frame Maximum number of frames
 * 
 * Requirements: 3.4
 */
void anim_object_set_frame(AnimObject* obj, FrameID frame_id, uint8_t max_frame);

/**
 * Advance animation frame
 * 
 * @param obj Pointer to AnimObject structure
 * 
 * Requirements: 3.5
 */
void anim_object_next_frame(AnimObject* obj);

/**
 * Get current sprite ID
 * 
 * Calculates the actual sprite index from base sprite_id + current frame.
 * 
 * @param obj Pointer to AnimObject structure
 * @return Current sprite ID (base + frame offset)
 * 
 * Requirements: 3.5
 */
SpriteID anim_object_get_sprite(const AnimObject* obj);

/**
 * Set direction
 * 
 * @param obj Pointer to AnimObject structure
 * @param dir Direction value (automatically wrapped to 0-7)
 * 
 * Requirements: 6.1, 6.2
 */
void anim_object_set_direction(AnimObject* obj, uint8_t dir);

/* ============================================================================
 * Animation Rendering Functions
 * Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 4.6
 * ============================================================================ */

/**
 * Set SDL blend mode based on BltType
 * 
 * @param texture SDL texture to set blend mode on
 * @param blt_type Rendering type
 * 
 * Requirements: 4.2, 4.3, 4.4, 4.5
 */
void anim_set_blend_mode(SDL_Texture* texture, BltType blt_type);

/**
 * Render animation object to screen
 * 
 * @param renderer SDL renderer
 * @param obj Animation object to render
 * @param pack SpritePack containing sprites
 * @param cache Optional sprite cache (can be NULL)
 * @param colorkey Transparent color key
 * @param zoom Zoom factor (1.0 = normal size)
 * @return 0 on success, negative on error
 * 
 * Requirements: 4.1, 4.6
 */
int anim_render(SDL_Renderer* renderer, 
                const AnimObject* obj,
                SpritePack* pack,
                DecodedSprite* cache,
                uint16_t colorkey,
                float zoom);

/**
 * Render with specific blend mode
 * 
 * @param renderer SDL renderer
 * @param obj Animation object to render
 * @param pack SpritePack containing sprites
 * @param cache Optional sprite cache (can be NULL)
 * @param colorkey Transparent color key
 * @param zoom Zoom factor
 * @param blt_type Rendering type to use
 * @return 0 on success, negative on error
 * 
 * Requirements: 4.2, 4.3, 4.4, 4.5
 */
int anim_render_blt(SDL_Renderer* renderer,
                    const AnimObject* obj,
                    SpritePack* pack,
                    DecodedSprite* cache,
                    uint16_t colorkey,
                    float zoom,
                    BltType blt_type);

#ifdef __cplusplus
}
#endif

#endif /* ANIMATION_H */
