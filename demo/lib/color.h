/**
 * @file color.h
 * @brief RGB565/RGB555 to RGBA32 color conversion
 * 
 * Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6
 */

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * RGBA32 color structure
 */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA32;

/**
 * Convert RGB565 color to RGBA32
 * @param color RGB565 color value
 * @param colorkey Transparent color key (alpha=0 if color==colorkey)
 * @return RGBA32 color
 */
RGBA32 rgb565_to_rgba32(uint16_t color, uint16_t colorkey);

/**
 * Convert RGB555 color to RGBA32
 * @param color RGB555 color value
 * @param colorkey Transparent color key (alpha=0 if color==colorkey)
 * @return RGBA32 color
 */
RGBA32 rgb555_to_rgba32(uint16_t color, uint16_t colorkey);

/**
 * Batch convert RGB565 colors to RGBA32
 * @param src Source RGB565 color array
 * @param dst Destination RGBA32 pixel array (packed as uint32_t)
 * @param count Number of pixels to convert
 * @param colorkey Transparent color key
 */
void rgb565_to_rgba32_batch(const uint16_t* src, uint32_t* dst, 
                            int count, uint16_t colorkey);

#ifdef __cplusplus
}
#endif

#endif /* COLOR_H */
