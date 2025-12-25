/**
 * @file index_spritepack.h
 * @brief IndexedSpritePack for loading .ispk files
 * 
 * Requirements: 1.1, 1.2, 1.3
 * 
 * IndexedSpritePack manages a collection of IndexedSprites loaded from
 * .ispk (pack) and .ispki (index) files. Supports both full loading
 * and lazy (on-demand) loading.
 */

#ifndef INDEX_SPRITEPACK_H
#define INDEX_SPRITEPACK_H

#include "index_sprite.h"
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * IndexSpritePack Structure
 * ============================================================================ */

/**
 * IndexedSpritePack structure
 * 
 * Manages a collection of IndexedSprites with support for lazy loading.
 */
typedef struct {
    IndexSprite* sprites;    /**< Array of sprites */
    uint16_t size;           /**< Number of sprites in pack */
    int is_lazy_load;        /**< Whether lazy loading is enabled */
    uint16_t loaded_count;   /**< Number of sprites loaded (for lazy loading) */
    FILE* pack_file;         /**< File handle for lazy loading */
    int32_t* file_index;     /**< File offsets for each sprite */
    char* filename;          /**< Pack filename (for reference) */
} IndexSpritePack;

/* ============================================================================
 * Functions
 * ============================================================================ */

/**
 * Initialize an IndexSpritePack structure
 * 
 * @param pack Pointer to IndexSpritePack
 */
void index_spritepack_init(IndexSpritePack* pack);

/**
 * Release IndexSpritePack resources
 * 
 * @param pack Pointer to IndexSpritePack
 */
void index_spritepack_release(IndexSpritePack* pack);

/**
 * Get number of sprites in pack
 * 
 * @param pack Pointer to IndexSpritePack
 * @return Number of sprites
 */
uint16_t index_spritepack_get_size(const IndexSpritePack* pack);

/**
 * Get sprite by index (triggers load if lazy loading)
 * 
 * @param pack Pointer to IndexSpritePack
 * @param index Sprite index
 * @return Pointer to IndexSprite, or NULL if invalid index
 * 
 * Requirements: 1.3
 */
IndexSprite* index_spritepack_get(IndexSpritePack* pack, uint16_t index);

/**
 * Load IndexSpritePack from file (full load)
 * 
 * @param pack Pointer to IndexSpritePack
 * @param filename Path to .ispk file
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 1.1
 */
int index_spritepack_load(IndexSpritePack* pack, const char* filename);

/**
 * Load IndexSpritePack from file stream (full load)
 * 
 * @param pack Pointer to IndexSpritePack
 * @param file File handle
 * @return 1 on success, 0 on failure
 */
int index_spritepack_load_from_file(IndexSpritePack* pack, FILE* file);

/**
 * Load IndexSpritePack with lazy loading (requires index file)
 * 
 * Sprites are loaded on-demand when accessed via index_spritepack_get().
 * The index file (.ispki) must exist alongside the pack file.
 * 
 * @param pack Pointer to IndexSpritePack
 * @param filename Path to .ispk file
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 1.2, 1.3
 */
int index_spritepack_load_lazy(IndexSpritePack* pack, const char* filename);

/**
 * Load a range of sprites (for partial loading)
 * 
 * @param pack Pointer to IndexSpritePack
 * @param first First sprite index
 * @param last Last sprite index (inclusive)
 * @return 1 on success, 0 on failure
 */
int index_spritepack_load_part(IndexSpritePack* pack, int first, int last);

/**
 * Release a range of sprites (to free memory)
 * 
 * @param pack Pointer to IndexSpritePack
 * @param first First sprite index
 * @param last Last sprite index (inclusive)
 * @return 1 on success, 0 on failure
 */
int index_spritepack_release_part(IndexSpritePack* pack, int first, int last);

#ifdef __cplusplus
}
#endif

#endif /* INDEX_SPRITEPACK_H */
