/**
 * @file shadow_spritepack.h
 * @brief ShadowSpritePack for loading .sspk files
 * 
 * Requirements: 2.1, 2.2
 * 
 * ShadowSpritePack manages a collection of ShadowSprites loaded from
 * .sspk (pack) and .sspki (index) files. Supports both full loading
 * and lazy (on-demand) loading.
 */

#ifndef SHADOW_SPRITEPACK_H
#define SHADOW_SPRITEPACK_H

#include "shadow_sprite.h"
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * ShadowSpritePack Structure
 * ============================================================================ */

/**
 * ShadowSpritePack structure
 * 
 * Manages a collection of ShadowSprites with support for lazy loading.
 */
typedef struct {
    ShadowSprite* sprites;   /**< Array of sprites */
    uint16_t size;           /**< Number of sprites in pack */
    int is_lazy_load;        /**< Whether lazy loading is enabled */
    uint16_t loaded_count;   /**< Number of sprites loaded (for lazy loading) */
    FILE* pack_file;         /**< File handle for lazy loading */
    int32_t* file_index;     /**< File offsets for each sprite */
    char* filename;          /**< Pack filename (for reference) */
} ShadowSpritePack;

/* ============================================================================
 * Functions
 * ============================================================================ */

/**
 * Initialize a ShadowSpritePack structure
 * 
 * @param pack Pointer to ShadowSpritePack
 */
void shadow_spritepack_init(ShadowSpritePack* pack);

/**
 * Release ShadowSpritePack resources
 * 
 * @param pack Pointer to ShadowSpritePack
 */
void shadow_spritepack_release(ShadowSpritePack* pack);

/**
 * Get number of sprites in pack
 * 
 * @param pack Pointer to ShadowSpritePack
 * @return Number of sprites
 */
uint16_t shadow_spritepack_get_size(const ShadowSpritePack* pack);

/**
 * Get sprite by index (triggers load if lazy loading)
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param index Sprite index
 * @return Pointer to ShadowSprite, or NULL if invalid index
 * 
 * Requirements: 2.2
 */
ShadowSprite* shadow_spritepack_get(ShadowSpritePack* pack, uint16_t index);

/**
 * Load ShadowSpritePack from file (full load)
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param filename Path to .sspk file
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 2.1
 */
int shadow_spritepack_load(ShadowSpritePack* pack, const char* filename);

/**
 * Load ShadowSpritePack from file stream (full load)
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param file File handle
 * @return 1 on success, 0 on failure
 */
int shadow_spritepack_load_from_file(ShadowSpritePack* pack, FILE* file);

/**
 * Load ShadowSpritePack with lazy loading (requires index file)
 * 
 * Sprites are loaded on-demand when accessed via shadow_spritepack_get().
 * The index file (.sspki) must exist alongside the pack file.
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param filename Path to .sspk file
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 2.1, 2.2
 */
int shadow_spritepack_load_lazy(ShadowSpritePack* pack, const char* filename);

/**
 * Load a range of sprites (for partial loading)
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param first First sprite index
 * @param last Last sprite index (inclusive)
 * @return 1 on success, 0 on failure
 */
int shadow_spritepack_load_part(ShadowSpritePack* pack, int first, int last);

/**
 * Release a range of sprites (to free memory)
 * 
 * @param pack Pointer to ShadowSpritePack
 * @param first First sprite index
 * @param last Last sprite index (inclusive)
 * @return 1 on success, 0 on failure
 */
int shadow_spritepack_release_part(ShadowSpritePack* pack, int first, int last);

#ifdef __cplusplus
}
#endif

#endif /* SHADOW_SPRITEPACK_H */
