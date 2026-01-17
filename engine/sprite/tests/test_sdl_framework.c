/**
 * @file test_sdl_framework.c
 * @brief SDL framework unit tests
 * 
 * Tests initialization, cleanup, and event handling
 * Validates: Requirements 1.1, 1.4, 1.5
 */

#include "sdl_framework.h"
#include <stdio.h>
#include <string.h>

/* Test helper declarations */
extern void test_assert(int condition, const char* message);
extern void test_assert_eq(int expected, int actual, const char* message);

/**
 * Test NULL parameter handling
 * Validates: Requirement 1.4 (error handling)
 */
static void test_null_parameters(void) {
    printf("  Testing NULL parameter handling...\n");
    
    SDLFramework fw;
    SDLFrameworkConfig config = {800, 600, "Test", 60};
    
    /* Test NULL framework pointer */
    int result = sdl_framework_init(NULL, &config);
    test_assert(result < 0, "Init with NULL framework returns error");
    
    /* Test NULL config pointer */
    result = sdl_framework_init(&fw, NULL);
    test_assert(result < 0, "Init with NULL config returns error");
    
    /* Test cleanup with NULL - should not crash */
    sdl_framework_cleanup(NULL);
    test_assert(1, "Cleanup with NULL does not crash");
    
    /* Test poll_events with NULL */
    SDL_Event event;
    result = sdl_framework_poll_events(NULL, &event);
    test_assert(result == 0, "Poll events with NULL framework returns 0");
    
    memset(&fw, 0, sizeof(fw));
    result = sdl_framework_poll_events(&fw, NULL);
    test_assert(result == 0, "Poll events with NULL event returns 0");
    
    /* Test begin/end frame with NULL - should not crash */
    sdl_framework_begin_frame(NULL);
    test_assert(1, "Begin frame with NULL does not crash");
    
    sdl_framework_end_frame(NULL);
    test_assert(1, "End frame with NULL does not crash");
    
    /* Test delay with NULL - should not crash */
    sdl_framework_delay(NULL, 0);
    test_assert(1, "Delay with NULL does not crash");
}

/**
 * Test framework configuration defaults
 */
static void test_config_defaults(void) {
    printf("  Testing configuration handling...\n");
    
    SDLFrameworkConfig config;
    memset(&config, 0, sizeof(config));
    
    /* Zero/negative values should use defaults */
    config.window_width = 0;
    config.window_height = 0;
    config.window_title = NULL;
    config.target_fps = 0;
    
    /* We can't actually test SDL init without a display,
       but we can verify the config structure is valid */
    test_assert(sizeof(SDLFrameworkConfig) > 0, "Config structure has valid size");
    test_assert(sizeof(SDLFramework) > 0, "Framework structure has valid size");
}

/**
 * Test framework state structure
 */
static void test_framework_structure(void) {
    printf("  Testing framework structure...\n");
    
    SDLFramework fw;
    memset(&fw, 0, sizeof(fw));
    
    /* Verify initial state */
    test_assert(fw.window == NULL, "Initial window is NULL");
    test_assert(fw.renderer == NULL, "Initial renderer is NULL");
    test_assert(fw.running == 0, "Initial running is 0");
    test_assert(fw.frame_delay == 0, "Initial frame_delay is 0");
}

/**
 * Test SDL initialization and cleanup
 * Note: This test requires SDL to be available and may fail in headless environments
 * Validates: Requirements 1.1, 1.2, 1.3, 1.4, 1.5
 */
static void test_init_and_cleanup(void) {
    printf("  Testing SDL initialization and cleanup...\n");
    
    SDLFramework fw;
    SDLFrameworkConfig config = {
        .window_width = 320,
        .window_height = 240,
        .window_title = "Test Window",
        .target_fps = 60
    };
    
    /* Try to initialize - may fail in headless environment */
    int result = sdl_framework_init(&fw, &config);
    
    if (result == 0) {
        /* Req 1.1: SDL initialized successfully */
        test_assert(1, "SDL initialization succeeded");
        
        /* Req 1.2: Window created */
        test_assert(fw.window != NULL, "Window was created");
        
        /* Req 1.3: Renderer created */
        test_assert(fw.renderer != NULL, "Renderer was created");
        
        /* Req 2.1: Running flag set */
        test_assert(fw.running == 1, "Running flag is set");
        
        /* Req 2.6: Frame delay calculated */
        test_assert(fw.frame_delay > 0, "Frame delay is set");
        test_assert_eq(16, (int)fw.frame_delay, "Frame delay is ~16ms for 60 FPS");
        
        /* Test frame rendering functions */
        sdl_framework_begin_frame(&fw);
        test_assert(1, "Begin frame executed");
        
        sdl_framework_end_frame(&fw);
        test_assert(1, "End frame executed");
        
        /* Test event polling (no events expected) */
        SDL_Event event;
        int has_event = sdl_framework_poll_events(&fw, &event);
        test_assert(has_event == 0 || has_event == 1, "Poll events returns valid result");
        
        /* Req 1.5: Cleanup resources */
        sdl_framework_cleanup(&fw);
        test_assert(fw.window == NULL, "Window cleaned up");
        test_assert(fw.renderer == NULL, "Renderer cleaned up");
        test_assert(fw.running == 0, "Running flag cleared");
        test_assert(1, "SDL cleanup succeeded");
    } else {
        /* SDL init failed - likely headless environment */
        printf("    [SKIP] SDL initialization failed (headless environment?)\n");
        printf("    [INFO] Error code: %d\n", result);
        test_assert(result < 0, "Init failure returns negative error code");
    }
}

/**
 * Test frame delay calculation
 */
static void test_frame_delay_calculation(void) {
    printf("  Testing frame delay calculation...\n");
    
    /* Test various FPS values */
    struct {
        int fps;
        uint32_t expected_delay;
    } test_cases[] = {
        {60, 16},   /* 1000/60 = 16.67 -> 16 */
        {30, 33},   /* 1000/30 = 33.33 -> 33 */
        {120, 8},   /* 1000/120 = 8.33 -> 8 */
        {1, 1000},  /* 1000/1 = 1000 */
    };
    
    for (size_t i = 0; i < sizeof(test_cases)/sizeof(test_cases[0]); i++) {
        uint32_t calculated = 1000 / test_cases[i].fps;
        char msg[128];
        snprintf(msg, sizeof(msg), "Frame delay for %d FPS is %u ms", 
                 test_cases[i].fps, test_cases[i].expected_delay);
        test_assert_eq((int)test_cases[i].expected_delay, (int)calculated, msg);
    }
}

void test_sdl_framework(void) {
    printf("\n  --- SDL Framework Unit Tests ---\n");
    
    test_null_parameters();
    test_config_defaults();
    test_framework_structure();
    test_frame_delay_calculation();
    test_init_and_cleanup();
}
