/**
 * @file test_sprite.c
 * @brief Sprite parsing property-based tests
 * 
 * Property 3: Sprite 解析往返一致性
 * For any valid sprite data, decoding the RLE data should produce correct
 * pixel output where the decoded pixel count equals sprite width * height,
 * and transparent/color runs are correctly reconstructed.
 * 
 * Validates: Requirements 3.1, 3.2, 3.4, 3.5, 3.6, 3.7
 */

#include "sprite.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Test helper declarations */
extern void test_assert(int condition, const char* message);
extern void test_assert_eq(int expected, int actual, const char* message);

/* Simple pseudo-random number generator for property testing */
static unsigned int sprite_pbt_seed = 0;

static void sprite_pbt_init_seed(void) {
    sprite_pbt_seed = (unsigned int)time(NULL);
}

static unsigned int sprite_pbt_random(void) {
    sprite_pbt_seed = sprite_pbt_seed * 1103515245 + 12345;
    return sprite_pbt_seed;
}

static uint16_t sprite_pbt_random_uint16(void) {
    return (uint16_t)(sprite_pbt_random() >> 16);
}

/**
 * Generate a random valid sprite with RLE data
 * Returns a sprite with properly formatted RLE scanlines
 */
static int generate_random_sprite(Sprite* sprite, int max_width, int max_height) {
    /* Generate random dimensions (1 to max) */
    sprite->width = 1 + (sprite_pbt_random() % max_width);
    sprite->height = 1 + (sprite_pbt_random() % max_height);
    sprite->is_valid = 1;
    
    /* Allocate scanline arrays */
    sprite->scanlines = (uint16_t**)calloc(sprite->height, sizeof(uint16_t*));
    sprite->scanline_lens = (uint16_t*)calloc(sprite->height, sizeof(uint16_t));
    
    if (!sprite->scanlines || !sprite->scanline_lens) {
        return -1;
    }
    
    /* Generate RLE data for each scanline */
    for (int y = 0; y < sprite->height; y++) {
        /* Temporary buffer for building RLE data */
        uint16_t temp[1024];
        int temp_index = 1;  /* Reserve index 0 for segment count */
        int segment_count = 0;
        int x = 0;
        
        /* Generate segments until we fill the width */
        while (x < sprite->width) {
            /* Random transparent count (0 to remaining width) */
            int remaining = sprite->width - x;
            int trans_count = sprite_pbt_random() % (remaining + 1);
            
            /* Ensure we don't exceed width */
            if (trans_count > remaining) {
                trans_count = remaining;
            }
            
            x += trans_count;
            
            /* If we've reached the end with transparency, we're done */
            if (x >= sprite->width) {
                /* Only add segment if we have transparency to record */
                if (trans_count > 0 && segment_count == 0) {
                    /* Empty line - no segments needed */
                }
                break;
            }
            
            /* Random color count (1 to remaining width) */
            remaining = sprite->width - x;
            int color_count = 1 + (sprite_pbt_random() % remaining);
            
            /* Store segment */
            temp[temp_index++] = (uint16_t)trans_count;
            temp[temp_index++] = (uint16_t)color_count;
            
            /* Generate random color pixels (non-zero to avoid colorkey) */
            for (int c = 0; c < color_count; c++) {
                uint16_t pixel = sprite_pbt_random_uint16();
                /* Ensure non-zero (colorkey is typically 0) */
                if (pixel == 0) pixel = 1;
                temp[temp_index++] = pixel;
            }
            
            x += color_count;
            segment_count++;
        }
        
        /* Store segment count at index 0 */
        temp[0] = (uint16_t)segment_count;
        
        /* Allocate and copy RLE data */
        sprite->scanline_lens[y] = (uint16_t)temp_index;
        sprite->scanlines[y] = (uint16_t*)malloc(temp_index * sizeof(uint16_t));
        if (!sprite->scanlines[y]) {
            return -1;
        }
        memcpy(sprite->scanlines[y], temp, temp_index * sizeof(uint16_t));
    }
    
    return 0;
}

/**
 * Decode sprite manually to verify against sprite_decode
 * Returns pixel array or NULL on error
 */
static uint32_t* manual_decode_sprite(const Sprite* sprite, uint16_t colorkey) {
    if (!sprite || sprite->width == 0 || sprite->height == 0) {
        return NULL;
    }
    
    size_t pixel_count = (size_t)sprite->width * sprite->height;
    uint32_t* pixels = (uint32_t*)calloc(pixel_count, sizeof(uint32_t));
    if (!pixels) {
        return NULL;
    }
    
    for (int y = 0; y < sprite->height; y++) {
        uint32_t* row = pixels + (y * sprite->width);
        
        if (!sprite->scanlines[y] || sprite->scanline_lens[y] == 0) {
            continue;
        }
        
        uint16_t* rle = sprite->scanlines[y];
        int count = rle[0];
        int idx = 1;
        int x = 0;
        
        for (int seg = 0; seg < count && x < sprite->width; seg++) {
            int trans = rle[idx++];
            int colors = rle[idx++];
            
            x += trans;
            
            for (int c = 0; c < colors && x < sprite->width; c++) {
                uint16_t pixel = rle[idx++];
                RGBA32 rgba = rgb565_to_rgba32(pixel, colorkey);
                row[x] = ((uint32_t)rgba.r) |
                         ((uint32_t)rgba.g << 8) |
                         ((uint32_t)rgba.b << 16) |
                         ((uint32_t)rgba.a << 24);
                x++;
            }
        }
    }
    
    return pixels;
}

/**
 * Property 3: Sprite 解析往返一致性
 * 
 * For any valid sprite data, decoding the RLE data should produce correct
 * pixel output. We verify this by:
 * 1. Generating random valid RLE sprite data
 * 2. Decoding with sprite_decode
 * 3. Manually decoding the same data
 * 4. Comparing the results
 * 
 * Validates: Requirements 3.1, 3.2, 3.4, 3.5, 3.6, 3.7
 */
static int test_property3_sprite_decode_consistency(void) {
    const int NUM_ITERATIONS = 100;
    const int MAX_WIDTH = 64;
    const int MAX_HEIGHT = 64;
    const uint16_t COLORKEY = 0;
    int passed = 1;
    
    printf("  Property 3: Sprite decode consistency\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        Sprite sprite;
        memset(&sprite, 0, sizeof(sprite));
        
        /* Generate random sprite */
        if (generate_random_sprite(&sprite, MAX_WIDTH, MAX_HEIGHT) != 0) {
            printf("    [FAIL] Iteration %d: Failed to generate sprite\n", i);
            passed = 0;
            break;
        }
        
        /* Decode using sprite_decode */
        DecodedSprite decoded;
        if (sprite_decode(&sprite, &decoded, COLORKEY) != 0) {
            printf("    [FAIL] Iteration %d: sprite_decode failed\n", i);
            sprite_free(&sprite);
            passed = 0;
            break;
        }
        
        /* Manually decode for comparison */
        uint32_t* expected = manual_decode_sprite(&sprite, COLORKEY);
        if (!expected) {
            printf("    [FAIL] Iteration %d: manual decode failed\n", i);
            decoded_sprite_free(&decoded);
            sprite_free(&sprite);
            passed = 0;
            break;
        }
        
        /* Compare dimensions */
        if (decoded.width != sprite.width || decoded.height != sprite.height) {
            printf("    [FAIL] Iteration %d: Dimension mismatch\n", i);
            printf("           Expected: %dx%d, Got: %dx%d\n",
                   sprite.width, sprite.height, decoded.width, decoded.height);
            free(expected);
            decoded_sprite_free(&decoded);
            sprite_free(&sprite);
            passed = 0;
            break;
        }
        
        /* Compare pixel data */
        size_t pixel_count = (size_t)sprite.width * sprite.height;
        int mismatch = 0;
        for (size_t p = 0; p < pixel_count; p++) {
            if (decoded.pixels[p] != expected[p]) {
                int x = p % sprite.width;
                int y = p / sprite.width;
                printf("    [FAIL] Iteration %d: Pixel mismatch at (%d, %d)\n", i, x, y);
                printf("           Expected: 0x%08X, Got: 0x%08X\n",
                       expected[p], decoded.pixels[p]);
                mismatch = 1;
                break;
            }
        }
        
        free(expected);
        decoded_sprite_free(&decoded);
        sprite_free(&sprite);
        
        if (mismatch) {
            passed = 0;
            break;
        }
    }
    
    if (passed) {
        printf("    [PASS] All %d iterations passed\n", NUM_ITERATIONS);
    }
    
    return passed;
}

/**
 * Unit test: Empty sprite handling
 */
static void test_empty_sprite(void) {
    printf("  Unit test: Empty sprite handling\n");
    
    Sprite sprite;
    memset(&sprite, 0, sizeof(sprite));
    sprite.width = 0;
    sprite.height = 0;
    sprite.is_valid = 1;
    sprite.scanlines = NULL;
    sprite.scanline_lens = NULL;
    
    DecodedSprite decoded;
    int result = sprite_decode(&sprite, &decoded, 0);
    
    test_assert(result == 0, "Empty sprite decodes successfully");
    test_assert(decoded.width == 0 && decoded.height == 0, 
                "Empty sprite has zero dimensions");
    test_assert(decoded.pixels == NULL, "Empty sprite has NULL pixels");
    
    decoded_sprite_free(&decoded);
}

/**
 * Unit test: Single pixel sprite
 */
static void test_single_pixel_sprite(void) {
    printf("  Unit test: Single pixel sprite\n");
    
    Sprite sprite;
    memset(&sprite, 0, sizeof(sprite));
    sprite.width = 1;
    sprite.height = 1;
    sprite.is_valid = 1;
    
    /* Create RLE data for single pixel: [1 segment] [0 trans] [1 color] [pixel] */
    uint16_t rle_data[] = {1, 0, 1, 0xF800};  /* Pure red */
    
    sprite.scanlines = (uint16_t**)malloc(sizeof(uint16_t*));
    sprite.scanline_lens = (uint16_t*)malloc(sizeof(uint16_t));
    sprite.scanlines[0] = (uint16_t*)malloc(sizeof(rle_data));
    memcpy(sprite.scanlines[0], rle_data, sizeof(rle_data));
    sprite.scanline_lens[0] = 4;
    
    DecodedSprite decoded;
    int result = sprite_decode(&sprite, &decoded, 0);
    
    test_assert(result == 0, "Single pixel sprite decodes successfully");
    test_assert(decoded.width == 1 && decoded.height == 1,
                "Single pixel sprite has correct dimensions");
    
    if (decoded.pixels) {
        /* Check pixel is red with full alpha */
        uint32_t pixel = decoded.pixels[0];
        uint8_t r = pixel & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t b = (pixel >> 16) & 0xFF;
        uint8_t a = (pixel >> 24) & 0xFF;
        
        test_assert(r == 255 && g == 0 && b == 0 && a == 255,
                    "Single red pixel has correct RGBA values");
    }
    
    decoded_sprite_free(&decoded);
    sprite_free(&sprite);
}

/**
 * Unit test: Transparent line handling
 */
static void test_transparent_line(void) {
    printf("  Unit test: Transparent line handling\n");
    
    Sprite sprite;
    memset(&sprite, 0, sizeof(sprite));
    sprite.width = 10;
    sprite.height = 1;
    sprite.is_valid = 1;
    
    /* Create RLE data for fully transparent line: [0 segments] */
    uint16_t rle_data[] = {0};
    
    sprite.scanlines = (uint16_t**)malloc(sizeof(uint16_t*));
    sprite.scanline_lens = (uint16_t*)malloc(sizeof(uint16_t));
    sprite.scanlines[0] = (uint16_t*)malloc(sizeof(rle_data));
    memcpy(sprite.scanlines[0], rle_data, sizeof(rle_data));
    sprite.scanline_lens[0] = 1;
    
    DecodedSprite decoded;
    int result = sprite_decode(&sprite, &decoded, 0);
    
    test_assert(result == 0, "Transparent line decodes successfully");
    
    if (decoded.pixels) {
        int all_transparent = 1;
        for (int x = 0; x < 10; x++) {
            if (decoded.pixels[x] != 0) {
                all_transparent = 0;
                break;
            }
        }
        test_assert(all_transparent, "All pixels in transparent line are zero");
    }
    
    decoded_sprite_free(&decoded);
    sprite_free(&sprite);
}

/**
 * Unit test: NULL parameter handling
 */
static void test_null_parameters(void) {
    printf("  Unit test: NULL parameter handling\n");
    
    Sprite sprite;
    DecodedSprite decoded;
    
    test_assert(sprite_decode(NULL, &decoded, 0) == -1,
                "sprite_decode returns -1 for NULL sprite");
    test_assert(sprite_decode(&sprite, NULL, 0) == -1,
                "sprite_decode returns -1 for NULL decoded");
    
    /* Test sprite_free with NULL */
    sprite_free(NULL);  /* Should not crash */
    test_assert(1, "sprite_free handles NULL gracefully");
    
    /* Test decoded_sprite_free with NULL */
    decoded_sprite_free(NULL);  /* Should not crash */
    test_assert(1, "decoded_sprite_free handles NULL gracefully");
}

void test_sprite_parsing(void) {
    int property3_passed;
    
    /* Initialize random seed for property tests */
    sprite_pbt_init_seed();
    printf("  (PBT seed: %u)\n", sprite_pbt_seed);
    
    /* Run property-based tests */
    printf("\n  --- Property-Based Tests ---\n");
    property3_passed = test_property3_sprite_decode_consistency();
    
    /* Run unit tests */
    printf("\n  --- Unit Tests ---\n");
    test_empty_sprite();
    test_single_pixel_sprite();
    test_transparent_line();
    test_null_parameters();
    
    /* Summary */
    printf("\n  --- Property Test Summary ---\n");
    printf("  Property 3 (Sprite decode consistency): %s\n", 
           property3_passed ? "PASSED" : "FAILED");
}
