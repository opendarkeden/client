/**
 * @file test_color.c
 * @brief Color conversion property-based tests
 * 
 * Property 1: RGB565 颜色转换正确性
 * Property 2: Colorkey 透明度处理
 * Validates: Requirements 5.1, 5.2, 5.3, 5.4, 5.5, 5.6
 */

#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Test helper declarations */
extern void test_assert(int condition, const char* message);
extern void test_assert_eq(int expected, int actual, const char* message);

/* Simple pseudo-random number generator for property testing */
static unsigned int pbt_seed = 0;

static void pbt_init_seed(void) {
    pbt_seed = (unsigned int)time(NULL);
}

static uint16_t pbt_random_uint16(void) {
    pbt_seed = pbt_seed * 1103515245 + 12345;
    return (uint16_t)(pbt_seed >> 16);
}

/**
 * Property 1: RGB565 颜色转换正确性
 * 
 * For any RGB565 color value, converting to RGBA32 should correctly extract
 * red (bits 11-15), green (bits 5-10), and blue (bits 0-4), and scale each
 * component to 8-bit range.
 * 
 * Validates: Requirements 5.1, 5.2, 5.3, 5.4
 */
static int test_property1_rgb565_conversion_correctness(void) {
    const int NUM_ITERATIONS = 100;
    int passed = 1;
    
    printf("  Property 1: RGB565 color conversion correctness\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        uint16_t color = pbt_random_uint16();
        uint16_t colorkey = pbt_random_uint16();
        
        /* Ensure color != colorkey for this property test (alpha handling is Property 2) */
        while (color == colorkey) {
            colorkey = pbt_random_uint16();
        }
        
        RGBA32 result = rgb565_to_rgba32(color, colorkey);
        
        /* Extract expected components from RGB565 */
        uint8_t expected_r5 = (color >> 11) & 0x1F;
        uint8_t expected_g6 = (color >> 5) & 0x3F;
        uint8_t expected_b5 = color & 0x1F;
        
        /* Calculate expected 8-bit values */
        uint8_t expected_r = (expected_r5 * 255) / 31;
        uint8_t expected_g = (expected_g6 * 255) / 63;
        uint8_t expected_b = (expected_b5 * 255) / 31;
        
        /* Verify conversion */
        if (result.r != expected_r || result.g != expected_g || result.b != expected_b) {
            printf("    [FAIL] Iteration %d: color=0x%04X\n", i, color);
            printf("           Expected: R=%d, G=%d, B=%d\n", expected_r, expected_g, expected_b);
            printf("           Got:      R=%d, G=%d, B=%d\n", result.r, result.g, result.b);
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
 * Property 2: Colorkey 透明度处理
 * 
 * For any pixel value and colorkey, if the pixel equals colorkey then alpha
 * should be 0, otherwise alpha should be 255.
 * 
 * Validates: Requirements 5.5, 5.6
 */
static int test_property2_colorkey_transparency(void) {
    const int NUM_ITERATIONS = 100;
    int passed = 1;
    
    printf("  Property 2: Colorkey transparency handling\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        uint16_t color = pbt_random_uint16();
        uint16_t colorkey = pbt_random_uint16();
        
        RGBA32 result = rgb565_to_rgba32(color, colorkey);
        
        /* Verify alpha based on colorkey comparison */
        uint8_t expected_alpha = (color == colorkey) ? 0 : 255;
        
        if (result.a != expected_alpha) {
            printf("    [FAIL] Iteration %d: color=0x%04X, colorkey=0x%04X\n", i, color, colorkey);
            printf("           Expected alpha=%d, Got alpha=%d\n", expected_alpha, result.a);
            passed = 0;
            break;
        }
    }
    
    /* Also test explicit colorkey match case */
    if (passed) {
        for (int i = 0; i < 10; i++) {
            uint16_t color = pbt_random_uint16();
            RGBA32 result = rgb565_to_rgba32(color, color);  /* color == colorkey */
            
            if (result.a != 0) {
                printf("    [FAIL] Colorkey match test: color=0x%04X should have alpha=0, got %d\n", 
                       color, result.a);
                passed = 0;
                break;
            }
        }
    }
    
    if (passed) {
        printf("    [PASS] All iterations passed\n");
    }
    
    return passed;
}

/**
 * Additional unit tests for edge cases and known values
 */
static void test_rgb565_known_values(void) {
    printf("  Unit tests: RGB565 known values\n");
    
    /* Test black (0x0000) */
    RGBA32 black = rgb565_to_rgba32(0x0000, 0xFFFF);
    test_assert(black.r == 0 && black.g == 0 && black.b == 0, 
                "Black (0x0000) converts to R=0, G=0, B=0");
    
    /* Test white (0xFFFF) */
    RGBA32 white = rgb565_to_rgba32(0xFFFF, 0x0000);
    test_assert(white.r == 255 && white.g == 255 && white.b == 255, 
                "White (0xFFFF) converts to R=255, G=255, B=255");
    
    /* Test pure red (0xF800) - bits 11-15 all set */
    RGBA32 red = rgb565_to_rgba32(0xF800, 0x0000);
    test_assert(red.r == 255 && red.g == 0 && red.b == 0, 
                "Pure red (0xF800) converts to R=255, G=0, B=0");
    
    /* Test pure green (0x07E0) - bits 5-10 all set */
    RGBA32 green = rgb565_to_rgba32(0x07E0, 0x0000);
    test_assert(green.r == 0 && green.g == 255 && green.b == 0, 
                "Pure green (0x07E0) converts to R=0, G=255, B=0");
    
    /* Test pure blue (0x001F) - bits 0-4 all set */
    RGBA32 blue = rgb565_to_rgba32(0x001F, 0x0000);
    test_assert(blue.r == 0 && blue.g == 0 && blue.b == 255, 
                "Pure blue (0x001F) converts to R=0, G=0, B=255");
    
    /* Test colorkey transparency */
    RGBA32 transparent = rgb565_to_rgba32(0x1234, 0x1234);
    test_assert(transparent.a == 0, "Colorkey match results in alpha=0");
    
    RGBA32 opaque = rgb565_to_rgba32(0x1234, 0x5678);
    test_assert(opaque.a == 255, "Non-colorkey results in alpha=255");
}

/**
 * Test batch conversion function
 */
static void test_rgb565_batch_conversion(void) {
    printf("  Unit tests: RGB565 batch conversion\n");
    
    uint16_t src[5] = {0x0000, 0xFFFF, 0xF800, 0x07E0, 0x001F};
    uint32_t dst[5] = {0};
    uint16_t colorkey = 0xDEAD;  /* Not matching any source */
    
    rgb565_to_rgba32_batch(src, dst, 5, colorkey);
    
    /* Verify each conversion */
    for (int i = 0; i < 5; i++) {
        RGBA32 expected = rgb565_to_rgba32(src[i], colorkey);
        uint32_t expected_packed = ((uint32_t)expected.r) |
                                   ((uint32_t)expected.g << 8) |
                                   ((uint32_t)expected.b << 16) |
                                   ((uint32_t)expected.a << 24);
        
        if (dst[i] != expected_packed) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Batch conversion index %d matches single conversion", i);
            test_assert(0, msg);
            return;
        }
    }
    test_assert(1, "Batch conversion matches single conversion for all elements");
    
    /* Test NULL handling */
    rgb565_to_rgba32_batch(NULL, dst, 5, colorkey);
    rgb565_to_rgba32_batch(src, NULL, 5, colorkey);
    rgb565_to_rgba32_batch(src, dst, 0, colorkey);
    rgb565_to_rgba32_batch(src, dst, -1, colorkey);
    test_assert(1, "Batch conversion handles NULL/invalid inputs gracefully");
}

void test_color_conversion(void) {
    int property1_passed, property2_passed;
    
    /* Initialize random seed for property tests */
    pbt_init_seed();
    printf("  (PBT seed: %u)\n", pbt_seed);
    
    /* Run property-based tests */
    printf("\n  --- Property-Based Tests ---\n");
    property1_passed = test_property1_rgb565_conversion_correctness();
    property2_passed = test_property2_colorkey_transparency();
    
    /* Run unit tests */
    printf("\n  --- Unit Tests ---\n");
    test_rgb565_known_values();
    test_rgb565_batch_conversion();
    
    /* Summary */
    printf("\n  --- Property Test Summary ---\n");
    printf("  Property 1 (RGB565 conversion): %s\n", property1_passed ? "PASSED" : "FAILED");
    printf("  Property 2 (Colorkey handling): %s\n", property2_passed ? "PASSED" : "FAILED");
}
