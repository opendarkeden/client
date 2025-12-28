/**
 * @file shadow_spritepack.c
 * @brief ShadowSpritePack implementation
 * 
 * Requirements: 2.1, 2.2
 * 
 * File format:
 * 
 * .sspk (pack file):
 *   [size: 2 bytes] - Number of sprites
 *   [sprite data...] - Sequential sprite data
 * 
 * .sspki (index file):
 *   [size: 2 bytes] - Number of sprites
 *   [offset: 4 bytes] × size - File offsets for each sprite
 */

#include "shadow_spritepack.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * Initialization and Cleanup
 * ============================================================================ */

void shadow_spritepack_init(ShadowSpritePack* pack) {
    if (!pack) return;
    
    pack->sprites = NULL;
    pack->size = 0;
    pack->is_lazy_load = 0;
    pack->loaded_count = 0;
    pack->pack_file = NULL;
    pack->file_index = NULL;
    pack->filename = NULL;
}

void shadow_spritepack_release(ShadowSpritePack* pack) {
    if (!pack) return;
    
    /* Release all sprites */
    if (pack->sprites) {
        for (int i = 0; i < pack->size; i++) {
            shadow_sprite_release(&pack->sprites[i]);
        }
        free(pack->sprites);
        pack->sprites = NULL;
    }
    
    /* Close lazy load file */
    if (pack->pack_file) {
        fclose(pack->pack_file);
        pack->pack_file = NULL;
    }
    
    /* Free file index */
    if (pack->file_index) {
        free(pack->file_index);
        pack->file_index = NULL;
    }
    
    /* Free filename */
    if (pack->filename) {
        free(pack->filename);
        pack->filename = NULL;
    }
    
    pack->size = 0;
    pack->is_lazy_load = 0;
    pack->loaded_count = 0;
}

uint16_t shadow_spritepack_get_size(const ShadowSpritePack* pack) {
    return pack ? pack->size : 0;
}

/* ============================================================================
 * Sprite Access
 * ============================================================================ */

ShadowSprite* shadow_spritepack_get(ShadowSpritePack* pack, uint16_t index) {
    if (!pack || !pack->sprites || index >= pack->size) {
        return NULL;
    }
    
    /* If lazy loading and sprite not yet loaded */
    if (pack->is_lazy_load && !shadow_sprite_is_init(&pack->sprites[index])) {
        if (pack->pack_file && pack->file_index) {
            /* Seek to sprite position */
            fseek(pack->pack_file, pack->file_index[index], SEEK_SET);
            
            /* Load sprite */
            if (shadow_sprite_load_from_file(&pack->sprites[index], pack->pack_file)) {
                pack->loaded_count++;
                
                /* If all sprites loaded, close file and free resources */
                if (pack->loaded_count >= pack->size) {
                    pack->is_lazy_load = 0;
                    fclose(pack->pack_file);
                    pack->pack_file = NULL;
                    free(pack->file_index);
                    pack->file_index = NULL;
                }
            }
        }
    }
    
    return &pack->sprites[index];
}

/* ============================================================================
 * File Loading
 * ============================================================================ */

int shadow_spritepack_load_from_file(ShadowSpritePack* pack, FILE* file) {
    if (!pack || !file) return 0;
    
    /* Release any existing data */
    shadow_spritepack_release(pack);
    
    /* Read sprite count */
    if (fread(&pack->size, 2, 1, file) != 1) {
        return 0;
    }
    
    if (pack->size == 0) {
        return 1;
    }
    
    /* Allocate sprite array */
    pack->sprites = (ShadowSprite*)malloc(pack->size * sizeof(ShadowSprite));
    if (!pack->sprites) {
        pack->size = 0;
        return 0;
    }
    
    /* Initialize all sprites */
    for (int i = 0; i < pack->size; i++) {
        shadow_sprite_init(&pack->sprites[i]);
    }
    
    /* Load all sprites */
    for (int i = 0; i < pack->size; i++) {
        if (!shadow_sprite_load_from_file(&pack->sprites[i], file)) {
            /* Continue loading even if one sprite fails */
        }
    }
    
    return 1;
}

int shadow_spritepack_load(ShadowSpritePack* pack, const char* filename) {
    if (!pack || !filename) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (!file) return 0;
    
    int result = shadow_spritepack_load_from_file(pack, file);
    
    fclose(file);
    return result;
}

int shadow_spritepack_load_lazy(ShadowSpritePack* pack, const char* filename) {
    if (!pack || !filename) return 0;
    
    /* Release any existing data */
    shadow_spritepack_release(pack);
    
    /* Build index filename (.sspki) */
    size_t len = strlen(filename);
    char* indexFilename = (char*)malloc(len + 2);
    if (!indexFilename) return 0;
    
    strcpy(indexFilename, filename);
    strcat(indexFilename, "i");
    
    /* Open index file */
    FILE* indexFile = fopen(indexFilename, "rb");
    if (!indexFile) {
        free(indexFilename);
        return 0;
    }
    
    /* Read sprite count from index file */
    if (fread(&pack->size, 2, 1, indexFile) != 1) {
        fclose(indexFile);
        free(indexFilename);
        return 0;
    }
    
    if (pack->size == 0) {
        fclose(indexFile);
        free(indexFilename);
        return 1;
    }
    
    /* Allocate sprite array */
    pack->sprites = (ShadowSprite*)malloc(pack->size * sizeof(ShadowSprite));
    if (!pack->sprites) {
        fclose(indexFile);
        free(indexFilename);
        pack->size = 0;
        return 0;
    }
    
    /* Initialize all sprites */
    for (int i = 0; i < pack->size; i++) {
        shadow_sprite_init(&pack->sprites[i]);
    }
    
    /* Allocate and read file index */
    pack->file_index = (int32_t*)malloc(pack->size * sizeof(int32_t));
    if (!pack->file_index) {
        free(pack->sprites);
        pack->sprites = NULL;
        fclose(indexFile);
        free(indexFilename);
        pack->size = 0;
        return 0;
    }
    
    for (int i = 0; i < pack->size; i++) {
        if (fread(&pack->file_index[i], 4, 1, indexFile) != 1) {
            free(pack->file_index);
            pack->file_index = NULL;
            free(pack->sprites);
            pack->sprites = NULL;
            fclose(indexFile);
            free(indexFilename);
            pack->size = 0;
            return 0;
        }
    }
    
    fclose(indexFile);
    
    /* Open pack file for lazy loading */
    pack->pack_file = fopen(filename, "rb");
    if (!pack->pack_file) {
        free(pack->file_index);
        pack->file_index = NULL;
        free(pack->sprites);
        pack->sprites = NULL;
        free(indexFilename);
        pack->size = 0;
        return 0;
    }
    
    /* Skip the size header in pack file */
    uint16_t packSize;
    if (fread(&packSize, 2, 1, pack->pack_file) != 1) {
        fclose(pack->pack_file);
        pack->pack_file = NULL;
        free(pack->file_index);
        pack->file_index = NULL;
        free(pack->sprites);
        pack->sprites = NULL;
        free(indexFilename);
        pack->size = 0;
        return 0;
    }
    
    /* Store filename for reference */
    pack->filename = (char*)malloc(len + 1);
    if (pack->filename) {
        strcpy(pack->filename, filename);
    }
    
    pack->is_lazy_load = 1;
    pack->loaded_count = 0;
    
    free(indexFilename);
    return 1;
}

int shadow_spritepack_load_part(ShadowSpritePack* pack, int first, int last) {
    if (!pack || !pack->sprites) return 0;
    
    /* Clamp range */
    if (first < 0) first = 0;
    if (last >= pack->size) last = pack->size - 1;
    if (first > last) return 0;
    
    /* Load each sprite in range */
    for (int i = first; i <= last; i++) {
        shadow_spritepack_get(pack, (uint16_t)i);
    }
    
    return 1;
}

int shadow_spritepack_release_part(ShadowSpritePack* pack, int first, int last) {
    if (!pack || !pack->sprites) return 0;
    
    /* Clamp range */
    if (first < 0) first = 0;
    if (last >= pack->size) last = pack->size - 1;
    if (first > last) return 0;
    
    /* Release each sprite in range */
    for (int i = first; i <= last; i++) {
        shadow_sprite_release(&pack->sprites[i]);
    }
    
    return 1;
}
