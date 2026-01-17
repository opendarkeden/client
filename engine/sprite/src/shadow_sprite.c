/**
 * @file shadow_sprite.c
 * @brief ShadowSprite implementation
 * 
 * Requirements: 2.3, 2.4
 * 
 * ShadowSprite file format (per scanline):
 *   [len: 2 bytes] - Length of scanline data in WORDs
 *   [data: len*2 bytes] - Scanline data
 * 
 * Scanline data format:
 *   [count] - Number of (trans, shadow) pairs
 *   For each pair:
 *     [transCount] - Transparent pixel count
 *     [shadowCount] - Shadow pixel count
 * 
 * Shadow rendering darkens destination pixels rather than copying colors.
 */

#include "shadow_sprite.h"
#include "color.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * Initialization and Cleanup
 * ============================================================================ */

void shadow_sprite_init(ShadowSprite* sprite) {
    if (!sprite) return;
    
    sprite->width = 0;
    sprite->height = 0;
    sprite->pixels = NULL;
    sprite->is_init = 0;
}

void shadow_sprite_release(ShadowSprite* sprite) {
    if (!sprite) return;
    
    if (sprite->pixels) {
        for (int i = 0; i < sprite->height; i++) {
            if (sprite->pixels[i]) {
                free(sprite->pixels[i]);
            }
        }
        free(sprite->pixels);
        sprite->pixels = NULL;
    }
    
    sprite->width = 0;
    sprite->height = 0;
    sprite->is_init = 0;
}

int shadow_sprite_is_init(const ShadowSprite* sprite) {
    return sprite ? sprite->is_init : 0;
}

uint16_t shadow_sprite_get_width(const ShadowSprite* sprite) {
    return sprite ? sprite->width : 0;
}

uint16_t shadow_sprite_get_height(const ShadowSprite* sprite) {
    return sprite ? sprite->height : 0;
}

/* ============================================================================
 * File I/O
 * ============================================================================ */

int shadow_sprite_load_from_file(ShadowSprite* sprite, FILE* file) {
    if (!sprite || !file) return 0;
    
    /* Release any existing data */
    shadow_sprite_release(sprite);
    
    /* Read width and height */
    if (fread(&sprite->width, 2, 1, file) != 1) return 0;
    if (fread(&sprite->height, 2, 1, file) != 1) return 0;
    
    /* Handle empty sprite */
    if (sprite->width == 0 || sprite->height == 0) {
        sprite->is_init = 1;
        return 1;
    }
    
    /* Allocate scanline pointers */
    sprite->pixels = (uint16_t**)malloc(sprite->height * sizeof(uint16_t*));
    if (!sprite->pixels) return 0;
    
    memset(sprite->pixels, 0, sprite->height * sizeof(uint16_t*));
    
    /* Read each scanline */
    for (int i = 0; i < sprite->height; i++) {
        uint16_t len;
        
        /* Read scanline length (in WORDs) */
        if (fread(&len, 2, 1, file) != 1) {
            shadow_sprite_release(sprite);
            return 0;
        }
        
        /* Allocate and read scanline data */
        sprite->pixels[i] = (uint16_t*)malloc(len * sizeof(uint16_t));
        if (!sprite->pixels[i]) {
            shadow_sprite_release(sprite);
            return 0;
        }
        
        if (fread(sprite->pixels[i], 2, len, file) != len) {
            shadow_sprite_release(sprite);
            return 0;
        }
    }
    
    sprite->is_init = 1;
    return 1;
}

/* ============================================================================
 * Rendering Functions
 * ============================================================================ */

void shadow_sprite_blt(const ShadowSprite* sprite, uint16_t* dest, uint16_t pitch) {
    if (!sprite || !sprite->is_init || !sprite->pixels || !dest) return;
    
    int count, shadowCount;
    uint16_t* pDestTemp;
    uint16_t* pPixels;
    
    for (int i = 0; i < sprite->height; i++) {
        pPixels = sprite->pixels[i];
        pDestTemp = dest;
        
        /* Number of (trans, shadow) pairs */
        count = *pPixels++;
        
        if (count > 0) {
            int j = count;
            do {
                /* Skip transparent pixels */
                pDestTemp += *pPixels++;
                
                /* Shadow pixel count */
                shadowCount = *pPixels++;
                
                /* Set shadow pixels to black */
                memset(pDestTemp, 0, shadowCount * sizeof(uint16_t));
                
                pDestTemp += shadowCount;
            } while (--j);
        }
        
        dest = (uint16_t*)((uint8_t*)dest + pitch);
    }
}


void shadow_sprite_blt_darkness(const ShadowSprite* sprite, uint16_t* dest,
                                uint16_t pitch, uint8_t darkBits) {
    if (!sprite || !sprite->is_init || !sprite->pixels || !dest) return;
    if (darkBits > 4) darkBits = 4;
    
    int count, shadowCount;
    uint16_t* pDestTemp;
    uint16_t* pPixels;
    
    /* Mask for shifting RGB565 values */
    /* After right shift, we need to mask out bits that wrapped around */
    uint16_t mask;
    switch (darkBits) {
        case 0: mask = 0xFFFF; break;
        case 1: mask = 0x7BEF; break;  /* 0111 1011 1110 1111 */
        case 2: mask = 0x39E7; break;  /* 0011 1001 1110 0111 */
        case 3: mask = 0x18E3; break;  /* 0001 1000 1110 0011 */
        case 4: mask = 0x0861; break;  /* 0000 1000 0110 0001 */
        default: mask = 0x7BEF; break;
    }
    
    for (int i = 0; i < sprite->height; i++) {
        pPixels = sprite->pixels[i];
        pDestTemp = dest;
        
        /* Number of (trans, shadow) pairs */
        count = *pPixels++;
        
        if (count > 0) {
            int j = count;
            do {
                /* Skip transparent pixels */
                pDestTemp += *pPixels++;
                
                /* Shadow pixel count */
                shadowCount = *pPixels++;
                
                /* Darken destination pixels */
                for (int k = 0; k < shadowCount; k++) {
                    *pDestTemp = (*pDestTemp >> darkBits) & mask;
                    pDestTemp++;
                }
            } while (--j);
        }
        
        dest = (uint16_t*)((uint8_t*)dest + pitch);
    }
}

void shadow_sprite_blt_alpha(const ShadowSprite* sprite, uint16_t* dest,
                             uint16_t pitch, uint8_t alpha) {
    if (!sprite || !sprite->is_init || !sprite->pixels || !dest) return;
    
    int count, shadowCount;
    uint16_t* pDestTemp;
    uint16_t* pPixels;
    int invAlpha = 255 - alpha;
    
    for (int i = 0; i < sprite->height; i++) {
        pPixels = sprite->pixels[i];
        pDestTemp = dest;
        
        /* Number of (trans, shadow) pairs */
        count = *pPixels++;
        
        if (count > 0) {
            int j = count;
            do {
                /* Skip transparent pixels */
                pDestTemp += *pPixels++;
                
                /* Shadow pixel count */
                shadowCount = *pPixels++;
                
                /* Alpha blend shadow (black) with destination */
                for (int k = 0; k < shadowCount; k++) {
                    uint16_t dstColor = *pDestTemp;
                    
                    /* Extract RGB components (RGB565) */
                    int r = (dstColor >> 11) & 0x1F;
                    int g = (dstColor >> 5) & 0x3F;
                    int b = dstColor & 0x1F;
                    
                    /* Blend with black (shadow color is 0,0,0) */
                    /* result = src * alpha + dst * (1-alpha) */
                    /* Since src is black, result = dst * invAlpha / 255 */
                    r = (r * invAlpha) / 255;
                    g = (g * invAlpha) / 255;
                    b = (b * invAlpha) / 255;
                    
                    /* Pack back to RGB565 */
                    *pDestTemp = (uint16_t)((r << 11) | (g << 5) | b);
                    pDestTemp++;
                }
            } while (--j);
        }
        
        dest = (uint16_t*)((uint8_t*)dest + pitch);
    }
}

/* ============================================================================
 * SDL Rendering Functions
 * ============================================================================ */

/**
 * Decode ShadowSprite to RGBA32 pixel buffer
 */
static uint32_t* shadow_sprite_decode_rgba(const ShadowSprite* sprite, uint8_t alpha) {
    if (!sprite || !sprite->is_init || !sprite->pixels) return NULL;
    if (sprite->width == 0 || sprite->height == 0) return NULL;
    
    uint32_t* pixels = (uint32_t*)malloc(sprite->width * sprite->height * sizeof(uint32_t));
    if (!pixels) return NULL;
    
    /* Initialize to fully transparent */
    memset(pixels, 0, sprite->width * sprite->height * sizeof(uint32_t));
    
    /* Shadow color: black with specified alpha */
    uint32_t shadowColor = (uint32_t)alpha << 24;  /* RGBA: 0, 0, 0, alpha */
    
    for (int y = 0; y < sprite->height; y++) {
        uint16_t* pPixels = sprite->pixels[y];
        uint32_t* pDest = pixels + y * sprite->width;
        int x = 0;
        
        int count = *pPixels++;
        
        for (int j = 0; j < count && x < sprite->width; j++) {
            /* Skip transparent pixels */
            int transCount = *pPixels++;
            x += transCount;
            pDest += transCount;
            
            /* Shadow pixels */
            int shadowCount = *pPixels++;
            for (int k = 0; k < shadowCount && x < sprite->width; k++) {
                *pDest++ = shadowColor;
                x++;
            }
        }
    }
    
    return pixels;
}

SDL_Texture* shadow_sprite_create_texture(const ShadowSprite* sprite,
                                          SDL_Renderer* renderer,
                                          uint8_t alpha) {
    if (!sprite || !sprite->is_init || !renderer) return NULL;
    if (sprite->width == 0 || sprite->height == 0) return NULL;
    
    uint32_t* pixels = shadow_sprite_decode_rgba(sprite, alpha);
    if (!pixels) return NULL;
    
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STATIC,
                                             sprite->width, sprite->height);
    if (!texture) {
        free(pixels);
        return NULL;
    }
    
    SDL_UpdateTexture(texture, NULL, pixels, sprite->width * sizeof(uint32_t));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    
    free(pixels);
    return texture;
}

int shadow_sprite_render(const ShadowSprite* sprite, SDL_Renderer* renderer,
                         int x, int y) {
    return shadow_sprite_render_alpha(sprite, renderer, x, y, 128);  /* Default 50% alpha */
}

int shadow_sprite_render_alpha(const ShadowSprite* sprite, SDL_Renderer* renderer,
                               int x, int y, uint8_t alpha) {
    if (!sprite || !sprite->is_init || !renderer) return -1;
    if (sprite->width == 0 || sprite->height == 0) return 0;
    
    SDL_Texture* texture = shadow_sprite_create_texture(sprite, renderer, alpha);
    if (!texture) return -1;
    
    SDL_Rect dstRect = { x, y, sprite->width, sprite->height };
    int result = SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    
    SDL_DestroyTexture(texture);
    return result;
}
