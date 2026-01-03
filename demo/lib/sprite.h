/**
 * @file sprite.h
 * @brief Sprite file format parsing and rendering
 * 
 * Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 6.1, 6.2, 6.3, 6.4, 6.5, 6.6
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Raw sprite data structure (RLE compressed)
 */
typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t** scanlines;    /* RLE data for each scanline */
    uint16_t* scanline_lens; /* Length of each scanline's RLE data */
    int is_valid;
} Sprite;

/**
 * Decoded sprite structure (ready for rendering)
 */
typedef struct {
    uint16_t width;
    uint16_t height;
    uint32_t* pixels;        /* RGBA32 pixel data */
    SDL_Texture* texture;    /* SDL texture (optional) */
} DecodedSprite;

/**
 * Load sprite from file stream
 * @param sprite Pointer to sprite structure
 * @param file File stream positioned at sprite data
 * @return 0 on success, negative error code on failure
 */
int sprite_load(Sprite* sprite, FILE* file);

/**
 * Free sprite resources
 * @param sprite Pointer to sprite structure
 */
void sprite_free(Sprite* sprite);

/**
 * Decode sprite RLE data to RGBA32 pixels
 * @param sprite Source sprite with RLE data
 * @param decoded Destination decoded sprite
 * @param colorkey Transparent color key
 * @return 0 on success, negative error code on failure
 */
int sprite_decode(const Sprite* sprite, DecodedSprite* decoded, 
                  uint16_t colorkey);

/**
 * Free decoded sprite resources
 * @param decoded Pointer to decoded sprite structure
 */
void decoded_sprite_free(DecodedSprite* decoded);

/**
 * Create SDL texture from decoded sprite
 * @param decoded Pointer to decoded sprite
 * @param renderer SDL renderer
 * @return 0 on success, negative error code on failure
 */
int decoded_sprite_create_texture(DecodedSprite* decoded, 
                                  SDL_Renderer* renderer);

#ifdef __cplusplus
}
#endif

#endif /* SPRITE_H */
