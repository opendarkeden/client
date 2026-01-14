/**
 * @file types.h
 * @brief Shared type definitions for Dark Eden SDL library
 * 
 * Requirements: 2.6, 3.3, 6.1
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Sprite and Frame IDs
 * ============================================================================ */

/**
 * Sprite ID type - identifies a sprite within a SpritePack
 */
typedef uint16_t SpriteID;

/**
 * Frame ID type - identifies an animation frame group
 */
typedef uint16_t FrameID;

/**
 * Null/invalid sprite ID marker
 */
#define SPRITEID_NULL   0xFFFF

/**
 * Null/invalid frame ID marker
 */
#define FRAMEID_NULL    0xFFFF

/* ============================================================================
 * Direction Enumeration
 * Requirement 6.1: Support 8 directions (0-7)
 * ============================================================================ */

/**
 * Direction enumeration for 8-directional animations
 * Values correspond to the original Dark Eden client direction system
 */
typedef enum Direction {
    DIR_LEFT = 0,       /**< Left direction */
    DIR_LEFTDOWN,       /**< Left-down diagonal */
    DIR_DOWN,           /**< Down direction */
    DIR_RIGHTDOWN,      /**< Right-down diagonal */
    DIR_RIGHT,          /**< Right direction */
    DIR_RIGHTUP,        /**< Right-up diagonal */
    DIR_UP,             /**< Up direction */
    DIR_LEFTUP,         /**< Left-up diagonal */
    DIR_MAX             /**< Number of directions (8) */
} Direction;

/* ============================================================================
 * BltType Enumeration
 * Requirement 2.6: Store BltType for rendering mode selection
 * ============================================================================ */

/**
 * Blit type enumeration for different rendering modes
 * 
 * BLT_NORMAL: Standard alpha blending (SDL_BLENDMODE_BLEND)
 * BLT_EFFECT: Additive blending for glow effects (SDL_BLENDMODE_ADD)
 * BLT_SHADOW: Shadow effect with darkened, semi-transparent rendering
 * BLT_SCREEN: Screen blend effect (approximated with SDL_BLENDMODE_ADD)
 */
typedef enum BltType {
    BLT_NORMAL = 0,     /**< Standard alpha blending */
    BLT_EFFECT,         /**< Additive blending for effects */
    BLT_SHADOW,         /**< Shadow rendering mode */
    BLT_SCREEN          /**< Screen blend mode */
} BltType;

/* ============================================================================
 * Default Values
 * ============================================================================ */

/**
 * Default colorkey value for transparency (RGB565 value 0)
 */
#define DEFAULT_COLORKEY 0

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */
