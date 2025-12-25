/**
 * @file test_main.c
 * @brief Simple test framework and test runner
 */

#include <stdio.h>
#include <stdlib.h>

/* Test result counters */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Test macros */
#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("  [PASS] %s\n", message); \
    } else { \
        tests_failed++; \
        printf("  [FAIL] %s\n", message); \
    } \
} while(0)

#define TEST_ASSERT_EQ(expected, actual, message) do { \
    tests_run++; \
    if ((expected) == (actual)) { \
        tests_passed++; \
        printf("  [PASS] %s\n", message); \
    } else { \
        tests_failed++; \
        printf("  [FAIL] %s (expected %d, got %d)\n", message, (int)(expected), (int)(actual)); \
    } \
} while(0)

/* External test functions */
extern void test_color_conversion(void);
extern void test_sdl_framework(void);
extern void test_sprite_parsing(void);
extern void test_spritepack_parsing(void);
extern void test_animation_frame(void);
extern void test_frame_and_tarray(void);
extern void test_framepack(void);
extern void test_index_sprite(void);
extern void test_shadow_sprite(void);

int main(void) {
    printf("=== SDL Sprite Viewer Tests ===\n\n");

    printf("Color Conversion Tests:\n");
    test_color_conversion();

    printf("\nSDL Framework Tests:\n");
    test_sdl_framework();

    printf("\nSprite Parsing Tests:\n");
    test_sprite_parsing();

    printf("\nSpritePack Parsing Tests:\n");
    test_spritepack_parsing();

    printf("\nAnimation Frame Tests:\n");
    test_animation_frame();

    printf("\nTArray and CFrame Tests:\n");
    test_frame_and_tarray();

    printf("\nFramePack Tests:\n");
    test_framepack();

    printf("\nIndexedSprite Tests:\n");
    test_index_sprite();

    printf("\nShadowSprite Tests:\n");
    test_shadow_sprite();

    printf("\n=== Test Summary ===\n");
    printf("Total: %d, Passed: %d, Failed: %d\n", 
           tests_run, tests_passed, tests_failed);

    return tests_failed > 0 ? 1 : 0;
}

/* Expose test macros to other test files */
void test_assert(int condition, const char* message) {
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("  [PASS] %s\n", message);
    } else {
        tests_failed++;
        printf("  [FAIL] %s\n", message);
    }
}

void test_assert_eq(int expected, int actual, const char* message) {
    tests_run++;
    if (expected == actual) {
        tests_passed++;
        printf("  [PASS] %s\n", message);
    } else {
        tests_failed++;
        printf("  [FAIL] %s (expected %d, got %d)\n", message, expected, actual);
    }
}
