/**
 * @file main.c
 * @brief SDL Sprite Viewer entry point
 * 
 * Requirements: 7.1, 7.6
 */

#include "viewer.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

/**
 * Print usage information
 * Requirement 7.1: Accept sprite pack file path as command line argument
 */
static void print_usage(const char* program) {
    printf("SDL Sprite Viewer\n");
    printf("=================\n\n");
    printf("Usage: %s <sprite_pack_file.spk>\n", program);
    printf("\nControls:\n");
    printf("  LEFT/RIGHT - Navigate to previous/next sprite\n");
    printf("  UP/DOWN    - Zoom in/out\n");
    printf("  I          - Toggle info display\n");
    printf("  ESC        - Exit\n");
}

/**
 * Main entry point
 * Requirement 7.1: Accept sprite pack file path
 * Requirement 7.6: Exit cleanly
 */
int main(int argc, char* argv[]) {
    /* Requirement 7.1: Accept sprite pack file path as command line argument */
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* pack_file = argv[1];
    SpriteViewer viewer;

    printf("Loading sprite pack: %s\n", pack_file);

    /* Initialize viewer with sprite pack */
    int result = viewer_init(&viewer, pack_file, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize viewer (error: %d)\n", result);
        return 1;
    }

    printf("Viewer initialized successfully\n");
    printf("Use LEFT/RIGHT to navigate, UP/DOWN to zoom, ESC to exit\n");

    /* Run main loop */
    viewer_run(&viewer);

    /* Requirement 7.6: Exit cleanly */
    viewer_cleanup(&viewer);

    printf("Viewer closed\n");
    return 0;
}
