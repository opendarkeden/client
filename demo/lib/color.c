/**
 * @file color.c
 * @brief RGB565/RGB555 to RGBA32 color conversion implementation
 * 
 * RGB565 format (16-bit):
 *   Bits 15-11: Red (5 bits)
 *   Bits 10-5:  Green (6 bits)
 *   Bits 4-0:   Blue (5 bits)
 * 
 * RGB555 format (16-bit):
 *   Bit 15:     Unused
 *   Bits 14-10: Red (5 bits)
 *   Bits 9-5:   Green (5 bits)
 *   Bits 4-0:   Blue (5 bits)
 */

#include "color.h"

RGBA32 rgb565_to_rgba32(uint16_t color, uint16_t colorkey) {
    RGBA32 result;
    
    /* Extract RGB565 components
     * Red:   bits 11-15 (5 bits)
     * Green: bits 5-10  (6 bits)
     * Blue:  bits 0-4   (5 bits)
     */
    uint8_t r5 = (color >> 11) & 0x1F;  /* 5 bits */
    uint8_t g6 = (color >> 5) & 0x3F;   /* 6 bits */
    uint8_t b5 = color & 0x1F;          /* 5 bits */
    
    /* Scale to 8-bit range
     * 5-bit to 8-bit: value * 255 / 31
     * 6-bit to 8-bit: value * 255 / 63
     */
    result.r = (r5 * 255) / 31;
    result.g = (g6 * 255) / 63;
    result.b = (b5 * 255) / 31;
    
    /* Set alpha based on colorkey comparison */
    result.a = (color == colorkey) ? 0 : 255;
    
    return result;
}

RGBA32 rgb555_to_rgba32(uint16_t color, uint16_t colorkey) {
    RGBA32 result;
    
    /* Extract RGB555 components
     * Red:   bits 10-14 (5 bits)
     * Green: bits 5-9   (5 bits)
     * Blue:  bits 0-4   (5 bits)
     * Bit 15 is unused
     */
    uint8_t r5 = (color >> 10) & 0x1F;  /* 5 bits */
    uint8_t g5 = (color >> 5) & 0x1F;   /* 5 bits */
    uint8_t b5 = color & 0x1F;          /* 5 bits */
    
    /* Scale to 8-bit range
     * 5-bit to 8-bit: value * 255 / 31
     */
    result.r = (r5 * 255) / 31;
    result.g = (g5 * 255) / 31;
    result.b = (b5 * 255) / 31;
    
    /* Set alpha based on colorkey comparison */
    result.a = (color == colorkey) ? 0 : 255;
    
    return result;
}

void rgb565_to_rgba32_batch(const uint16_t* src, uint32_t* dst, 
                            int count, uint16_t colorkey) {
    if (!src || !dst || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        RGBA32 rgba = rgb565_to_rgba32(src[i], colorkey);
        /* Pack RGBA32 into uint32_t (RGBA order) */
        dst[i] = ((uint32_t)rgba.r) |
                 ((uint32_t)rgba.g << 8) |
                 ((uint32_t)rgba.b << 16) |
                 ((uint32_t)rgba.a << 24);
    }
}
