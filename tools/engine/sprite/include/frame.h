/**
 * @file frame.h
 * @brief Frame structures for animation data (Pure C implementation)
 *
 * Requirements: 3.4, 3.5
 *
 * Frame stores a single animation frame's data:
 * - sprite_id: Index into SpritePack
 * - cx, cy: Coordinate offsets for rendering
 */

#ifndef FRAME_H
#define FRAME_H

#include <stdio.h>
#include <stdint.h>
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

/**
 * Sprite ID type - identifies a sprite within a SpritePack
 */
typedef uint16_t TYPE_SPRITEID;

/**
 * Frame ID type - identifies a frame within a FramePack
 */
typedef uint16_t TYPE_FRAMEID;

/**
 * Null/invalid sprite ID marker
 */
#define SPRITEID_NULL   0xFFFF

/**
 * Null/invalid frame ID marker
 */
#define FRAMEID_NULL    0xFFFF

/* ============================================================================
 * Frame Structure
 * ============================================================================ */

/**
 * Frame structure - stores single frame data
 *
 * Each frame contains:
 * - sprite_id: Index into SpritePack (0-65535)
 * - cx, cy: Coordinate offsets for rendering position
 */
typedef struct {
    TYPE_SPRITEID sprite_id;    /**< Sprite index (0-65535) */
    int16_t cx;                 /**< X coordinate offset */
    int16_t cy;                 /**< Y coordinate offset */
} Frame;

/* ============================================================================
 * Frame Array Types using vector.h
 * ============================================================================ */

/**
 * FrameArray: Array of frames for a single direction
 * Indexed by frame number
 */
typedef vector(Frame) FrameArray;

/**
 * DirectionArray: Array of frame arrays for all directions
 * Indexed by direction (0-7)
 */
typedef vector(FrameArray) DirectionArray;

/**
 * ActionArray: Array of direction arrays for all actions
 * Indexed by action type (STAND, WALK, RUN, etc.)
 */
typedef vector(DirectionArray) ActionArray;

/* ============================================================================
 * Frame Functions
 * ============================================================================ */

/**
 * Initialize a frame with values
 * @param frame Pointer to frame
 * @param sprite_id Sprite index
 * @param cx X coordinate offset
 * @param cy Y coordinate offset
 */
void frame_init(Frame* frame, TYPE_SPRITEID sprite_id, int16_t cx, int16_t cy);

/**
 * Save frame to file
 * @param frame Pointer to frame
 * @param file FILE pointer to write to
 * @return 1 on success, 0 on failure
 */
int frame_save(const Frame* frame, FILE* file);

/**
 * Load frame from file
 * @param frame Pointer to frame
 * @param file FILE pointer to read from
 * @return 1 on success, 0 on failure
 */
int frame_load(Frame* frame, FILE* file);

/* ============================================================================
 * FrameArray Functions
 * ============================================================================ */

/**
 * Initialize a FrameArray
 * @param arr Pointer to FrameArray
 */
void frame_array_init(FrameArray* arr);

/**
 * Release a FrameArray
 * @param arr Pointer to FrameArray
 */
void frame_array_free(FrameArray* arr);

/**
 * Get size of FrameArray
 * @param arr Pointer to FrameArray
 * @return Number of frames
 */
int frame_array_size(const FrameArray* arr);

/**
 * Get frame at index
 * @param arr Pointer to FrameArray
 * @param index Frame index
 * @return Pointer to frame, or NULL if invalid index
 */
Frame* frame_array_get(const FrameArray* arr, int index);

/**
 * Save FrameArray to file
 * @param arr Pointer to FrameArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int frame_array_save(const FrameArray* arr, FILE* file);

/**
 * Load FrameArray from file
 * @param arr Pointer to FrameArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int frame_array_load(FrameArray* arr, FILE* file);

/* ============================================================================
 * DirectionArray Functions
 * ============================================================================ */

/**
 * Initialize a DirectionArray
 * @param arr Pointer to DirectionArray
 */
void direction_array_init(DirectionArray* arr);

/**
 * Release a DirectionArray (and all nested FrameArrays)
 * @param arr Pointer to DirectionArray
 */
void direction_array_free(DirectionArray* arr);

/**
 * Get size of DirectionArray
 * @param arr Pointer to DirectionArray
 * @return Number of directions
 */
int direction_array_size(const DirectionArray* arr);

/**
 * Get FrameArray at direction index
 * @param arr Pointer to DirectionArray
 * @param index Direction index (0-7)
 * @return Pointer to FrameArray, or NULL if invalid index
 */
FrameArray* direction_array_get(DirectionArray* arr, int index);

/**
 * Save DirectionArray to file
 * @param arr Pointer to DirectionArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int direction_array_save(const DirectionArray* arr, FILE* file);

/**
 * Load DirectionArray from file
 * @param arr Pointer to DirectionArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int direction_array_load(DirectionArray* arr, FILE* file);

/* ============================================================================
 * ActionArray Functions
 * ============================================================================ */

/**
 * Initialize an ActionArray
 * @param arr Pointer to ActionArray
 */
void action_array_init(ActionArray* arr);

/**
 * Release an ActionArray (and all nested arrays)
 * @param arr Pointer to ActionArray
 */
void action_array_free(ActionArray* arr);

/**
 * Get size of ActionArray
 * @param arr Pointer to ActionArray
 * @return Number of actions
 */
int action_array_size(const ActionArray* arr);

/**
 * Get DirectionArray at action index
 * @param arr Pointer to ActionArray
 * @param index Action index
 * @return Pointer to DirectionArray, or NULL if invalid index
 */
DirectionArray* action_array_get(ActionArray* arr, int index);

/**
 * Save ActionArray to file
 * @param arr Pointer to ActionArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int action_array_save(const ActionArray* arr, FILE* file);

/**
 * Load ActionArray from file
 * @param arr Pointer to ActionArray
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int action_array_load(ActionArray* arr, FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* FRAME_H */
