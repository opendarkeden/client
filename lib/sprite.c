/**
 * @file sprite.c
 * @brief Sprite file format parsing and rendering implementation
 * 
 * Sprite File Format:
 *   - Width (2 bytes, uint16_t, little-endian)
 *   - Height (2 bytes, uint16_t, little-endian)
 *   - For each scanline:
 *     - Length (2 bytes, uint16_t) - number of WORDs in RLE data
 *     - RLE Data (Length * 2 bytes)
 * 
 * RLE Line Format:
 *   - Count (1 WORD) - number of segments
 *   - For each segment:
 *     - Trans Count (1 WORD) - transparent pixel count
 *     - Color Count (1 WORD) - color pixel count
 *     - Pixels (Color Count WORDs) - RGB565 pixel data
 */

#include "sprite.h"
#include "color.h"
#include <stdlib.h>
#include <string.h>

/**
 * Load sprite from file stream
 * Requirements: 3.1, 3.2, 3.3, 3.4, 3.5
 */
int sprite_load(Sprite* sprite, FILE* file) {
    if (!sprite || !file) {
        return -1;
    }
    
    /* Initialize sprite structure */
    memset(sprite, 0, sizeof(Sprite));
    sprite->is_valid = 0;
    
    /* Read width (2 bytes, little-endian) - Requirement 3.1 */
    if (fread(&sprite->width, 2, 1, file) != 1) {
        return -1;
    }
    
    /* Read height (2 bytes, little-endian) - Requirement 3.2 */
    if (fread(&sprite->height, 2, 1, file) != 1) {
        return -1;
    }
    
    /* Handle empty sprite - Requirement 3.3 */
    if (sprite->width == 0 || sprite->height == 0) {
        sprite->is_valid = 1;  /* Empty but valid */
        sprite->scanlines = NULL;
        sprite->scanline_lens = NULL;
        return 0;
    }
    
    /* Allocate scanline arrays */
    sprite->scanlines = (uint16_t**)calloc(sprite->height, sizeof(uint16_t*));
    if (!sprite->scanlines) {
        return -3;  /* Memory allocation failed */
    }
    
    sprite->scanline_lens = (uint16_t*)calloc(sprite->height, sizeof(uint16_t));
    if (!sprite->scanline_lens) {
        free(sprite->scanlines);
        sprite->scanlines = NULL;
        return -3;
    }
    
    /* Read each scanline - Requirements 3.4, 3.5 */
    for (int i = 0; i < sprite->height; i++) {
        uint16_t len;
        
        /* Read line data length (2 bytes) - Requirement 3.4 */
        if (fread(&len, 2, 1, file) != 1) {
            sprite_free(sprite);
            return -2;  /* File format error */
        }
        
        sprite->scanline_lens[i] = len;
        
        if (len > 0) {
            /* Allocate and read RLE data - Requirement 3.5 */
            sprite->scanlines[i] = (uint16_t*)malloc(len * sizeof(uint16_t));
            if (!sprite->scanlines[i]) {
                sprite_free(sprite);
                return -3;
            }
            
            if (fread(sprite->scanlines[i], 2, len, file) != len) {
                sprite_free(sprite);
                return -2;
            }
        } else {
            sprite->scanlines[i] = NULL;
        }
    }
    
    sprite->is_valid = 1;
    return 0;
}

/**
 * Free sprite resources
 */
void sprite_free(Sprite* sprite) {
    if (!sprite) {
        return;
    }
    
    if (sprite->scanlines) {
        for (int i = 0; i < sprite->height; i++) {
            if (sprite->scanlines[i]) {
                free(sprite->scanlines[i]);
            }
        }
        free(sprite->scanlines);
        sprite->scanlines = NULL;
    }
    
    if (sprite->scanline_lens) {
        free(sprite->scanline_lens);
        sprite->scanline_lens = NULL;
    }
    
    sprite->width = 0;
    sprite->height = 0;
    sprite->is_valid = 0;
}

/**
 * Decode sprite RLE data to RGBA32 pixels
 * Requirements: 3.6, 3.7, 6.3, 6.4, 6.5
 */
int sprite_decode(const Sprite* sprite, DecodedSprite* decoded, 
                  uint16_t colorkey) {
    if (!sprite || !decoded) {
        return -1;
    }
    
    /* Initialize decoded sprite */
    memset(decoded, 0, sizeof(DecodedSprite));
    decoded->width = sprite->width;
    decoded->height = sprite->height;
    decoded->texture = NULL;
    
    /* Handle empty sprite */
    if (sprite->width == 0 || sprite->height == 0) {
        decoded->pixels = NULL;
        return 0;
    }
    
    /* Allocate pixel buffer */
    size_t pixel_count = (size_t)sprite->width * sprite->height;
    decoded->pixels = (uint32_t*)calloc(pixel_count, sizeof(uint32_t));
    if (!decoded->pixels) {
        return -3;
    }
    
    /* Decode each scanline */
    for (int y = 0; y < sprite->height; y++) {
        uint32_t* row = decoded->pixels + (y * sprite->width);
        
        /* Initialize row to transparent */
        for (int x = 0; x < sprite->width; x++) {
            row[x] = 0;  /* RGBA(0,0,0,0) - Requirement 6.4 */
        }
        
        if (!sprite->scanlines[y] || sprite->scanline_lens[y] == 0) {
            continue;  /* Empty scanline */
        }
        
        uint16_t* rle_data = sprite->scanlines[y];
        
        /* Read segment count - Requirement 3.6 */
        int count = rle_data[0];
        int rle_index = 1;
        int x = 0;
        
        /* Process each segment - Requirement 3.7 */
        for (int seg = 0; seg < count && x < sprite->width; seg++) {
            int trans_count = rle_data[rle_index++];  /* Transparent pixels */
            int color_count = rle_data[rle_index++];  /* Color pixels */
            
            /* Skip transparent pixels (already initialized to 0) */
            x += trans_count;
            
            /* Copy color pixels - Requirement 6.5 */
            for (int c = 0; c < color_count && x < sprite->width; c++) {
                uint16_t pixel = rle_data[rle_index++];
                RGBA32 rgba = rgb565_to_rgba32(pixel, colorkey);
                
                /* Pack RGBA32 into uint32_t (ABGR order for SDL) */
                row[x] = ((uint32_t)rgba.r) |
                         ((uint32_t)rgba.g << 8) |
                         ((uint32_t)rgba.b << 16) |
                         ((uint32_t)rgba.a << 24);
                x++;
            }
        }
    }
    
    return 0;
}

/**
 * Free decoded sprite resources
 */
void decoded_sprite_free(DecodedSprite* decoded) {
    if (!decoded) {
        return;
    }
    
    if (decoded->pixels) {
        free(decoded->pixels);
        decoded->pixels = NULL;
    }
    
    if (decoded->texture) {
        SDL_DestroyTexture(decoded->texture);
        decoded->texture = NULL;
    }
    
    decoded->width = 0;
    decoded->height = 0;
}

/**
 * Create SDL texture from decoded sprite
 * Requirements: 6.1, 6.2, 6.6
 */
int decoded_sprite_create_texture(DecodedSprite* decoded, 
                                  SDL_Renderer* renderer) {
    if (!decoded || !renderer) {
        return -1;
    }
    
    /* Handle empty sprite */
    if (decoded->width == 0 || decoded->height == 0 || !decoded->pixels) {
        decoded->texture = NULL;
        return 0;
    }
    
    /* Destroy existing texture if any */
    if (decoded->texture) {
        SDL_DestroyTexture(decoded->texture);
        decoded->texture = NULL;
    }
    
    /* Create texture with RGBA32 format - Requirement 6.1 */
    decoded->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC,
        decoded->width,
        decoded->height
    );
    
    if (!decoded->texture) {
        return -1;
    }
    
    /* Set blend mode for transparency - Requirement 6.2 */
    if (SDL_SetTextureBlendMode(decoded->texture, SDL_BLENDMODE_BLEND) != 0) {
        SDL_DestroyTexture(decoded->texture);
        decoded->texture = NULL;
        return -1;
    }
    
    /* Upload pixel data to texture - Requirement 6.3 */
    int pitch = decoded->width * sizeof(uint32_t);
    if (SDL_UpdateTexture(decoded->texture, NULL, decoded->pixels, pitch) != 0) {
        SDL_DestroyTexture(decoded->texture);
        decoded->texture = NULL;
        return -1;
    }
    
    return 0;
}
