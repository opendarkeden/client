/**
 * @file main.c
 * @brief SDL Item Viewer entry point
 */

#include "viewer.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

/**
 * Print usage information
 */
static void print_usage(const char* program) {
    printf("Item Viewer\n");
    printf("==========\n\n");
    printf("Usage: %s <item_pack.ispk>\n", program);
    printf("\nControls:\n");
    printf("  LEFT/RIGHT - Navigate to previous/next item\n");
    printf("  UP/DOWN    - Zoom in/out\n");
    printf("  I          - Toggle info display\n");
    printf("  ESC        - Exit\n");
    printf("\nNote: .ispk files are indexed sprite packs for items.\n");
    printf("      The corresponding .ispki index file must exist.\n");
}

/**
 * Main entry point
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* pack_file = argv[1];
    ItemViewer viewer;

    printf("Loading item pack: %s\n", pack_file);

    /* Initialize viewer with item pack */
    int result = viewer_init(&viewer, pack_file, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize viewer (error: %d)\n", result);
        return 1;
    }

    printf("Viewer initialized successfully\n");
    printf("Use LEFT/RIGHT to navigate, UP/DOWN to zoom, ESC to exit\n");

    /* Run main loop */
    viewer_run(&viewer);

    /* Cleanup */
    viewer_cleanup(&viewer);

    printf("Viewer closed\n");
    return 0;
}
