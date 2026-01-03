/**
 * @file spritepack.c
 * @brief SpritePack file format parsing implementation
 * 
 * SpritePack File Format (.spk):
 *   - Sprite Count (2 bytes, uint16_t, little-endian)
 *   - Sprite 0 Data
 *   - Sprite 1 Data
 *   - ... (repeat for each sprite)
 * 
 * Index File Format (.spki):
 *   - Sprite Count (2 bytes, uint16_t, little-endian)
 *   - Offset 0 (4 bytes, uint32_t)
 *   - Offset 1 (4 bytes, uint32_t)
 *   - ... (repeat for each sprite)
 * 
 * Requirements: 4.1, 4.2, 4.3, 4.4, 4.5
 */

#include "spritepack.h"
#include <stdlib.h>
#include <string.h>

/**
 * Initialize SpritePack structure to safe defaults
 */
static void spritepack_init(SpritePack* pack) {
    pack->count = 0;
    pack->sprites = NULL;
    pack->file_offsets = NULL;
    pack->pack_file = NULL;
    pack->lazy_load = 0;
    pack->filename = NULL;
}

uint16_t spritepack_size(SpritePack* pack) {
    if (!pack) return 0;
    return pack->count;
}

/**
 * Load sprite pack (full load)
 * Requirements: 4.1, 4.2, 4.5
 * 
 * @param pack Pointer to sprite pack structure
 * @param filename Path to .spk file
 * @return 0 on success, negative error code on failure
 *         -1: Invalid parameters
 *         -2: File open failed
 *         -3: Memory allocation failed
 *         -4: File format error
 */
int spritepack_load(SpritePack* pack, const char* filename) {
    if (!pack || !filename) {
        return -1;
    }
    
    /* Initialize pack structure */
    spritepack_init(pack);
    
    /* Open pack file */
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return -2;  /* File open failed */
    }
    
    /* Read sprite count (2 bytes, little-endian) - Requirement 4.1 */
    if (fread(&pack->count, 2, 1, file) != 1) {
        fclose(file);
        return -4;  /* File format error */
    }
    
    /* Handle empty pack */
    if (pack->count == 0) {
        fclose(file);
        return 0;
    }
    
    /* Allocate sprite array */
    pack->sprites = (Sprite*)calloc(pack->count, sizeof(Sprite));
    if (!pack->sprites) {
        fclose(file);
        pack->count = 0;
        return -3;  /* Memory allocation failed */
    }
    
    /* Load each sprite - Requirement 4.2 */
    for (uint16_t i = 0; i < pack->count; i++) {
        int result = sprite_load(&pack->sprites[i], file);
        if (result != 0) {
            /* Requirement 4.5: Continue loading remaining sprites and report error */
            pack->sprites[i].is_valid = 0;
            /* Note: We continue loading, but the sprite is marked invalid */
        }
    }
    
    fclose(file);
    
    /* Return success even if some sprites failed (per Requirement 4.5) */
    return 0;
}

/**
 * Load sprite pack with lazy loading (requires index file)
 * Requirements: 4.3, 4.4
 * 
 * @param pack Pointer to sprite pack structure
 * @param filename Path to .spk file
 * @return 0 on success, negative error code on failure
 *         -1: Invalid parameters
 *         -2: File open failed
 *         -3: Memory allocation failed
 *         -4: File format error
 *         -5: Index file not found or invalid
 */
int spritepack_load_lazy(SpritePack* pack, const char* filename) {
    if (!pack || !filename) {
        return -1;
    }
    
    /* Initialize pack structure */
    spritepack_init(pack);
    
    /* Build index filename by appending 'i' */
    size_t filename_len = strlen(filename);
    char* index_filename = (char*)malloc(filename_len + 2);
    if (!index_filename) {
        return -3;
    }
    strcpy(index_filename, filename);
    strcat(index_filename, "i");
    
    /* Open index file - Requirement 4.3 */
    FILE* index_file = fopen(index_filename, "rb");
    if (!index_file) {
        free(index_filename);
        return -5;  /* Index file not found */
    }
    
    /* Read sprite count from index file - Requirement 4.4 */
    if (fread(&pack->count, 2, 1, index_file) != 1) {
        fclose(index_file);
        free(index_filename);
        return -4;
    }
    
    /* Handle empty pack */
    if (pack->count == 0) {
        fclose(index_file);
        free(index_filename);
        return 0;
    }
    
    /* Allocate file offsets array - Requirement 4.4 */
    pack->file_offsets = (uint32_t*)malloc(pack->count * sizeof(uint32_t));
    if (!pack->file_offsets) {
        fclose(index_file);
        free(index_filename);
        pack->count = 0;
        return -3;
    }
    
    /* Read file offsets (4 bytes each) - Requirement 4.4 */
    for (uint16_t i = 0; i < pack->count; i++) {
        if (fread(&pack->file_offsets[i], 4, 1, index_file) != 1) {
            fclose(index_file);
            free(index_filename);
            free(pack->file_offsets);
            pack->file_offsets = NULL;
            pack->count = 0;
            return -4;
        }
    }
    
    fclose(index_file);
    
    /* Allocate sprite array (all uninitialized) */
    pack->sprites = (Sprite*)calloc(pack->count, sizeof(Sprite));
    if (!pack->sprites) {
        free(index_filename);
        free(pack->file_offsets);
        pack->file_offsets = NULL;
        pack->count = 0;
        return -3;
    }
    
    /* Open pack file and keep it open for lazy loading */
    pack->pack_file = fopen(filename, "rb");
    if (!pack->pack_file) {
        free(index_filename);
        free(pack->file_offsets);
        free(pack->sprites);
        pack->file_offsets = NULL;
        pack->sprites = NULL;
        pack->count = 0;
        return -2;
    }
    
    /* Store filename for reference */
    pack->filename = (char*)malloc(filename_len + 1);
    if (pack->filename) {
        strcpy(pack->filename, filename);
    }
    
    pack->lazy_load = 1;
    
    free(index_filename);
    return 0;
}

/**
 * Get sprite from pack (triggers load if lazy loading)
 * Requirements: 4.3 (random access loading)
 * 
 * @param pack Pointer to sprite pack
 * @param index Sprite index
 * @return Pointer to sprite, or NULL if invalid index
 */
Sprite* spritepack_get(SpritePack* pack, uint16_t index) {
    if (!pack || !pack->sprites) {
        return NULL;
    }
    
    /* Check index bounds */
    if (index >= pack->count) {
        return NULL;
    }
    
    /* If lazy loading and sprite not yet loaded */
    if (pack->lazy_load && !pack->sprites[index].is_valid && 
        pack->pack_file && pack->file_offsets) {
        
        /* Seek to sprite position using index */
        if (fseek(pack->pack_file, pack->file_offsets[index], SEEK_SET) != 0) {
            return NULL;
        }
        
        /* Load the sprite */
        int result = sprite_load(&pack->sprites[index], pack->pack_file);
        if (result != 0) {
            /* Mark as invalid but return pointer anyway */
            pack->sprites[index].is_valid = 0;
        }
    }
    
    return &pack->sprites[index];
}

/**
 * Free sprite pack resources
 * 
 * @param pack Pointer to sprite pack structure
 */
void spritepack_free(SpritePack* pack) {
    if (!pack) {
        return;
    }
    
    /* Free all sprites */
    if (pack->sprites) {
        for (uint16_t i = 0; i < pack->count; i++) {
            sprite_free(&pack->sprites[i]);
        }
        free(pack->sprites);
        pack->sprites = NULL;
    }
    
    /* Free file offsets */
    if (pack->file_offsets) {
        free(pack->file_offsets);
        pack->file_offsets = NULL;
    }
    
    /* Close pack file if open */
    if (pack->pack_file) {
        fclose(pack->pack_file);
        pack->pack_file = NULL;
    }
    
    /* Free filename */
    if (pack->filename) {
        free(pack->filename);
        pack->filename = NULL;
    }
    
    pack->count = 0;
    pack->lazy_load = 0;
}
