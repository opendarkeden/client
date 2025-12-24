/**
 * @file spritepack.h
 * @brief SpritePack file format parsing
 * 
 * Requirements: 4.1, 4.2, 4.3, 4.4, 4.5
 */

#ifndef SPRITEPACK_H
#define SPRITEPACK_H

#include "sprite.h"
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SpritePack structure
 */
typedef struct {
    uint16_t count;          /* Number of sprites in pack */
    Sprite* sprites;         /* Array of sprites */
    uint32_t* file_offsets;  /* File offsets from index file (optional) */
    FILE* pack_file;         /* File handle for lazy loading */
    int lazy_load;           /* Whether lazy loading is enabled */
    char* filename;          /* Pack filename for lazy loading */
} SpritePack;

/**
 * Load sprite pack (full load)
 * @param pack Pointer to sprite pack structure
 * @param filename Path to .spk file
 * @return 0 on success, negative error code on failure
 */
int spritepack_load(SpritePack* pack, const char* filename);

/**
 * Load sprite pack with lazy loading (requires index file)
 * @param pack Pointer to sprite pack structure
 * @param filename Path to .spk file
 * @return 0 on success, negative error code on failure
 */
int spritepack_load_lazy(SpritePack* pack, const char* filename);

/**
 * Get sprite from pack (triggers load if lazy loading)
 * @param pack Pointer to sprite pack
 * @param index Sprite index
 * @return Pointer to sprite, or NULL if invalid index
 */
Sprite* spritepack_get(SpritePack* pack, uint16_t index);

/**
 * Free sprite pack resources
 * @param pack Pointer to sprite pack structure
 */
void spritepack_free(SpritePack* pack);

#ifdef __cplusplus
}
#endif

#endif /* SPRITEPACK_H */
