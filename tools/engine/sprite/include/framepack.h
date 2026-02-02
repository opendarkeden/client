/**
 * @file framepack.h
 * @brief FramePack structures for animation data (Pure C implementation)
 * 
 * Requirements: 3.1, 3.2, 3.3
 * 
 * FramePack stores animation frame data for different entity types:
 * - CreatureFramePack: [CreatureType][Action][Direction][Frame] -> Frame
 * - DirectionFramePack: [Type][Direction][Frame] -> Frame (for effects)
 * - AnimationFramePack: [Type][Frame] -> Frame (for simple animations)
 * - ImageFramePack: [Type] -> Frame (for static images)
 */

#ifndef FRAMEPACK_H
#define FRAMEPACK_H

#include <stdio.h>
#include <stdint.h>
#include "frame.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * CreatureFramePack - [CreatureType][Action][Direction][Frame]
 * ============================================================================ */

typedef struct CreatureFramePack {
/**
 * CreatureFramePack structure
 * 
 * Stores animation frame data for all creature types.
 * Structure: [CreatureType][Action][Direction][Frame] -> Frame
 */
    vector(ActionArray) creatures;  /**< Array of ActionArrays, one per creature type */
} CreatureFramePack;

/**
 * Initialize a CreatureFramePack
 * @param pack Pointer to CreatureFramePack
 */
void creature_framepack_init(CreatureFramePack* pack);

/**
 * Release a CreatureFramePack (and all nested arrays)
 * @param pack Pointer to CreatureFramePack
 */
void creature_framepack_free(CreatureFramePack* pack);

/**
 * Get number of creature types
 * @param pack Pointer to CreatureFramePack
 * @return Number of creature types
 */
int creature_framepack_size(const CreatureFramePack* pack);

/**
 * Get ActionArray for a creature type
 * @param pack Pointer to CreatureFramePack
 * @param creature_type Creature type index
 * @return Pointer to ActionArray, or NULL if invalid index
 */
ActionArray* creature_framepack_get(CreatureFramePack* pack, int creature_type);

/**
 * Get a specific frame
 * @param pack Pointer to CreatureFramePack
 * @param creature_type Creature type index
 * @param action Action index
 * @param direction Direction index (0-7)
 * @param frame_idx Frame index
 * @return Pointer to Frame, or NULL if invalid indices
 */
Frame* creature_framepack_get_frame(CreatureFramePack* pack, 
                                    int creature_type, int action, 
                                    int direction, int frame_idx);

/**
 * Load CreatureFramePack from file
 * @param pack Pointer to CreatureFramePack
 * @param filename Path to .cfpk file
 * @return 1 on success, 0 on failure
 */
int creature_framepack_load(CreatureFramePack* pack, const char* filename);

/**
 * Load CreatureFramePack from file stream
 * @param pack Pointer to CreatureFramePack
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int creature_framepack_load_from_file(CreatureFramePack* pack, FILE* file);

/**
 * Save CreatureFramePack to file
 * @param pack Pointer to CreatureFramePack
 * @param filename Path to output file
 * @return 1 on success, 0 on failure
 */
int creature_framepack_save(const CreatureFramePack* pack, const char* filename);

/**
 * Save CreatureFramePack to file stream
 * @param pack Pointer to CreatureFramePack
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int creature_framepack_save_to_file(const CreatureFramePack* pack, FILE* file);

/**
 * Output frame pack information to a text file (for debugging)
 * @param pack Pointer to CreatureFramePack
 * @param filename Path to output file
 */
void creature_framepack_info_to_file(const CreatureFramePack* pack, const char* filename);

/* ============================================================================
 * DirectionFramePack - [Type][Direction][Frame]
 * ============================================================================ */

/**
 * DirectionFramePack structure
 * 
 * Stores animation frame data with direction support.
 * Structure: [Type][Direction][Frame] -> Frame
 * Used for effects and other directional animations.
 */
typedef struct {
    vector(DirectionArray) types;  /**< Array of DirectionArrays */
} DirectionFramePack;

/**
 * Initialize a DirectionFramePack
 * @param pack Pointer to DirectionFramePack
 */
void direction_framepack_init(DirectionFramePack* pack);

/**
 * Release a DirectionFramePack
 * @param pack Pointer to DirectionFramePack
 */
void direction_framepack_free(DirectionFramePack* pack);

/**
 * Get number of types
 * @param pack Pointer to DirectionFramePack
 * @return Number of types
 */
int direction_framepack_size(const DirectionFramePack* pack);

/**
 * Get DirectionArray for a type
 * @param pack Pointer to DirectionFramePack
 * @param type_idx Type index
 * @return Pointer to DirectionArray, or NULL if invalid index
 */
DirectionArray* direction_framepack_get(DirectionFramePack* pack, int type_idx);

/**
 * Load DirectionFramePack from file
 * @param pack Pointer to DirectionFramePack
 * @param filename Path to file
 * @return 1 on success, 0 on failure
 */
int direction_framepack_load(DirectionFramePack* pack, const char* filename);

/**
 * Load DirectionFramePack from file stream
 * @param pack Pointer to DirectionFramePack
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int direction_framepack_load_from_file(DirectionFramePack* pack, FILE* file);

/* ============================================================================
 * AnimationFramePack - [Type][Frame]
 * ============================================================================ */

/**
 * AnimationFramePack structure
 * 
 * Stores simple animation frame data without direction.
 * Structure: [Type][Frame] -> Frame
 */
typedef struct {
    vector(FrameArray) types;  /**< Array of FrameArrays */
} AnimationFramePack;

/**
 * Initialize an AnimationFramePack
 * @param pack Pointer to AnimationFramePack
 */
void animation_framepack_init(AnimationFramePack* pack);

/**
 * Release an AnimationFramePack
 * @param pack Pointer to AnimationFramePack
 */
void animation_framepack_free(AnimationFramePack* pack);

/**
 * Get number of types
 * @param pack Pointer to AnimationFramePack
 * @return Number of types
 */
int animation_framepack_size(const AnimationFramePack* pack);

/**
 * Get FrameArray for a type
 * @param pack Pointer to AnimationFramePack
 * @param type_idx Type index
 * @return Pointer to FrameArray, or NULL if invalid index
 */
FrameArray* animation_framepack_get(AnimationFramePack* pack, int type_idx);

/**
 * Load AnimationFramePack from file
 * @param pack Pointer to AnimationFramePack
 * @param filename Path to file
 * @return 1 on success, 0 on failure
 */
int animation_framepack_load(AnimationFramePack* pack, const char* filename);

/**
 * Load AnimationFramePack from file stream
 * @param pack Pointer to AnimationFramePack
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int animation_framepack_load_from_file(AnimationFramePack* pack, FILE* file);

/* ============================================================================
 * ImageFramePack - [Type] -> Frame
 * ============================================================================ */

/**
 * ImageFramePack structure
 * 
 * Stores single frame per type (for static images).
 * Structure: [Type] -> Frame
 */
typedef struct {
    vector(Frame) frames;  /**< Array of Frames */
} ImageFramePack;

/**
 * Initialize an ImageFramePack
 * @param pack Pointer to ImageFramePack
 */
void image_framepack_init(ImageFramePack* pack);

/**
 * Release an ImageFramePack
 * @param pack Pointer to ImageFramePack
 */
void image_framepack_free(ImageFramePack* pack);

/**
 * Get number of frames
 * @param pack Pointer to ImageFramePack
 * @return Number of frames
 */
int image_framepack_size(const ImageFramePack* pack);

/**
 * Get Frame at index
 * @param pack Pointer to ImageFramePack
 * @param index Frame index
 * @return Pointer to Frame, or NULL if invalid index
 */
Frame* image_framepack_get(ImageFramePack* pack, int index);

/**
 * Load ImageFramePack from file
 * @param pack Pointer to ImageFramePack
 * @param filename Path to file
 * @return 1 on success, 0 on failure
 */
int image_framepack_load(ImageFramePack* pack, const char* filename);

/**
 * Load ImageFramePack from file stream
 * @param pack Pointer to ImageFramePack
 * @param file FILE pointer
 * @return 1 on success, 0 on failure
 */
int image_framepack_load_from_file(ImageFramePack* pack, FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* FRAMEPACK_H */
