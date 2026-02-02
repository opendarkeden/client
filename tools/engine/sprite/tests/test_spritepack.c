/**
 * @file test_spritepack.c
 * @brief SpritePack parsing property-based tests
 * 
 * Property 4: SpritePack 解析完整性
 * For any valid SpritePack file, loading should correctly read the sprite count,
 * and for each sprite index from 0 to count-1, the sprite should be loadable
 * and have valid dimensions.
 * 
 * Validates: Requirements 4.1, 4.2, 4.4
 */

#include "spritepack.h"
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
static unsigned int spritepack_pbt_seed = 0;

static void spritepack_pbt_init_seed(void) {
    spritepack_pbt_seed = (unsigned int)time(NULL);
}

static unsigned int spritepack_pbt_random(void) {
    spritepack_pbt_seed = spritepack_pbt_seed * 1103515245 + 12345;
    return spritepack_pbt_seed;
}

/**
 * Generate a random valid sprite and write to file
 * Returns number of bytes written, or -1 on error
 */
static int write_random_sprite_to_file(FILE* file, int max_width, int max_height) {
    /* Generate random dimensions (1 to max) */
    uint16_t width = 1 + (spritepack_pbt_random() % max_width);
    uint16_t height = 1 + (spritepack_pbt_random() % max_height);
    
    /* Write width and height */
    if (fwrite(&width, 2, 1, file) != 1) return -1;
    if (fwrite(&height, 2, 1, file) != 1) return -1;
    
    /* Write each scanline */
    for (int y = 0; y < height; y++) {
        /* Generate simple RLE data for this scanline */
        /* Format: [segment_count] [trans_count, color_count, pixels...] */
        uint16_t temp[256];
        int temp_index = 1;  /* Reserve index 0 for segment count */
        int segment_count = 0;
        int x = 0;
        
        while (x < width) {
            /* Random transparent count */
            int remaining = width - x;
            int trans_count = spritepack_pbt_random() % (remaining + 1);
            if (trans_count > remaining) trans_count = remaining;
            x += trans_count;
            
            if (x >= width) break;
            
            /* Random color count */
            remaining = width - x;
            int color_count = 1 + (spritepack_pbt_random() % remaining);
            
            temp[temp_index++] = (uint16_t)trans_count;
            temp[temp_index++] = (uint16_t)color_count;
            
            /* Generate random pixels */
            for (int c = 0; c < color_count; c++) {
                uint16_t pixel = (uint16_t)(spritepack_pbt_random() >> 16);
                if (pixel == 0) pixel = 1;  /* Avoid colorkey */
                temp[temp_index++] = pixel;
            }
            
            x += color_count;
            segment_count++;
        }
        
        temp[0] = (uint16_t)segment_count;
        
        /* Write scanline length and data */
        uint16_t len = (uint16_t)temp_index;
        if (fwrite(&len, 2, 1, file) != 1) return -1;
        if (fwrite(temp, 2, len, file) != len) return -1;
    }
    
    return 0;
}

/**
 * Create a temporary SpritePack file with random sprites
 * Returns filename on success, NULL on failure
 */
static char* create_temp_spritepack(uint16_t sprite_count, int max_width, int max_height) {
    static char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/test_spritepack_%u.spk", spritepack_pbt_seed);
    
    FILE* file = fopen(filename, "wb");
    if (!file) return NULL;
    
    /* Write sprite count */
    if (fwrite(&sprite_count, 2, 1, file) != 1) {
        fclose(file);
        return NULL;
    }
    
    /* Write each sprite */
    for (int i = 0; i < sprite_count; i++) {
        if (write_random_sprite_to_file(file, max_width, max_height) != 0) {
            fclose(file);
            return NULL;
        }
    }
    
    fclose(file);
    return filename;
}

/**
 * Create a temporary SpritePack file with index file
 * Returns filename on success, NULL on failure
 */
static char* create_temp_spritepack_with_index(uint16_t sprite_count, int max_width, int max_height) {
    static char filename[256];
    static char index_filename[260];
    snprintf(filename, sizeof(filename), "/tmp/test_spritepack_idx_%u.spk", spritepack_pbt_seed);
    snprintf(index_filename, sizeof(index_filename), "%si", filename);
    
    FILE* data_file = fopen(filename, "wb");
    FILE* index_file = fopen(index_filename, "wb");
    if (!data_file || !index_file) {
        if (data_file) fclose(data_file);
        if (index_file) fclose(index_file);
        return NULL;
    }
    
    /* Write sprite count to both files */
    if (fwrite(&sprite_count, 2, 1, data_file) != 1 ||
        fwrite(&sprite_count, 2, 1, index_file) != 1) {
        fclose(data_file);
        fclose(index_file);
        return NULL;
    }
    
    /* Write each sprite and record offsets */
    for (int i = 0; i < sprite_count; i++) {
        /* Record current position as offset */
        uint32_t offset = (uint32_t)ftell(data_file);
        if (fwrite(&offset, 4, 1, index_file) != 1) {
            fclose(data_file);
            fclose(index_file);
            return NULL;
        }
        
        /* Write sprite data */
        if (write_random_sprite_to_file(data_file, max_width, max_height) != 0) {
            fclose(data_file);
            fclose(index_file);
            return NULL;
        }
    }
    
    fclose(data_file);
    fclose(index_file);
    return filename;
}

/**
 * Remove temporary files
 */
static void cleanup_temp_files(const char* filename) {
    if (filename) {
        remove(filename);
        
        /* Also remove index file if exists */
        char index_filename[260];
        snprintf(index_filename, sizeof(index_filename), "%si", filename);
        remove(index_filename);
    }
}

/**
 * Property 4: SpritePack 解析完整性
 * 
 * For any valid SpritePack file, loading should correctly read the sprite count,
 * and for each sprite index from 0 to count-1, the sprite should be loadable
 * and have valid dimensions.
 * 
 * Validates: Requirements 4.1, 4.2, 4.4
 */
static int test_property4_spritepack_completeness(void) {
    const int NUM_ITERATIONS = 100;
    const int MAX_SPRITES = 10;
    const int MAX_WIDTH = 32;
    const int MAX_HEIGHT = 32;
    int passed = 1;
    
    printf("  Property 4: SpritePack parsing completeness\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        /* Generate random sprite count (1 to MAX_SPRITES) */
        uint16_t expected_count = 1 + (spritepack_pbt_random() % MAX_SPRITES);
        
        /* Create temporary spritepack file */
        char* filename = create_temp_spritepack(expected_count, MAX_WIDTH, MAX_HEIGHT);
        if (!filename) {
            printf("    [FAIL] Iteration %d: Failed to create temp file\n", iter);
            passed = 0;
            break;
        }
        
        /* Load the spritepack */
        SpritePack pack;
        int result = spritepack_load(&pack, filename);
        
        if (result != 0) {
            printf("    [FAIL] Iteration %d: spritepack_load failed with code %d\n", iter, result);
            cleanup_temp_files(filename);
            passed = 0;
            break;
        }
        
        /* Verify sprite count - Requirement 4.1 */
        if (pack.count != expected_count) {
            printf("    [FAIL] Iteration %d: Count mismatch (expected %d, got %d)\n",
                   iter, expected_count, pack.count);
            spritepack_free(&pack);
            cleanup_temp_files(filename);
            passed = 0;
            break;
        }
        
        /* Verify each sprite is accessible and valid - Requirement 4.2 */
        int sprite_error = 0;
        for (uint16_t i = 0; i < pack.count; i++) {
            Sprite* sprite = spritepack_get(&pack, i);
            if (!sprite) {
                printf("    [FAIL] Iteration %d: spritepack_get returned NULL for index %d\n",
                       iter, i);
                sprite_error = 1;
                break;
            }
            
            if (!sprite->is_valid) {
                printf("    [FAIL] Iteration %d: Sprite %d is not valid\n", iter, i);
                sprite_error = 1;
                break;
            }
            
            /* Verify dimensions are within expected range */
            if (sprite->width == 0 || sprite->width > MAX_WIDTH ||
                sprite->height == 0 || sprite->height > MAX_HEIGHT) {
                printf("    [FAIL] Iteration %d: Sprite %d has invalid dimensions (%dx%d)\n",
                       iter, i, sprite->width, sprite->height);
                sprite_error = 1;
                break;
            }
        }
        
        spritepack_free(&pack);
        cleanup_temp_files(filename);
        
        if (sprite_error) {
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
 * Property 4b: SpritePack lazy loading completeness
 * 
 * For any valid SpritePack file with index, lazy loading should correctly
 * read the sprite count and allow random access to any sprite.
 * 
 * Validates: Requirements 4.3, 4.4
 */
static int test_property4b_spritepack_lazy_loading(void) {
    const int NUM_ITERATIONS = 50;
    const int MAX_SPRITES = 10;
    const int MAX_WIDTH = 32;
    const int MAX_HEIGHT = 32;
    int passed = 1;
    
    printf("  Property 4b: SpritePack lazy loading completeness\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        /* Generate random sprite count (1 to MAX_SPRITES) */
        uint16_t expected_count = 1 + (spritepack_pbt_random() % MAX_SPRITES);
        
        /* Create temporary spritepack file with index */
        char* filename = create_temp_spritepack_with_index(expected_count, MAX_WIDTH, MAX_HEIGHT);
        if (!filename) {
            printf("    [FAIL] Iteration %d: Failed to create temp files\n", iter);
            passed = 0;
            break;
        }
        
        /* Load the spritepack with lazy loading */
        SpritePack pack;
        int result = spritepack_load_lazy(&pack, filename);
        
        if (result != 0) {
            printf("    [FAIL] Iteration %d: spritepack_load_lazy failed with code %d\n", iter, result);
            cleanup_temp_files(filename);
            passed = 0;
            break;
        }
        
        /* Verify sprite count - Requirement 4.4 */
        if (pack.count != expected_count) {
            printf("    [FAIL] Iteration %d: Count mismatch (expected %d, got %d)\n",
                   iter, expected_count, pack.count);
            spritepack_free(&pack);
            cleanup_temp_files(filename);
            passed = 0;
            break;
        }
        
        /* Verify lazy load flag is set */
        if (!pack.lazy_load) {
            printf("    [FAIL] Iteration %d: lazy_load flag not set\n", iter);
            spritepack_free(&pack);
            cleanup_temp_files(filename);
            passed = 0;
            break;
        }
        
        /* Access sprites in random order to test lazy loading - Requirement 4.3 */
        int sprite_error = 0;
        for (int access = 0; access < expected_count; access++) {
            /* Pick a random sprite index */
            uint16_t idx = spritepack_pbt_random() % expected_count;
            
            Sprite* sprite = spritepack_get(&pack, idx);
            if (!sprite) {
                printf("    [FAIL] Iteration %d: spritepack_get returned NULL for index %d\n",
                       iter, idx);
                sprite_error = 1;
                break;
            }
            
            if (!sprite->is_valid) {
                printf("    [FAIL] Iteration %d: Sprite %d is not valid after lazy load\n", iter, idx);
                sprite_error = 1;
                break;
            }
            
            /* Verify dimensions are within expected range */
            if (sprite->width == 0 || sprite->width > MAX_WIDTH ||
                sprite->height == 0 || sprite->height > MAX_HEIGHT) {
                printf("    [FAIL] Iteration %d: Sprite %d has invalid dimensions (%dx%d)\n",
                       iter, idx, sprite->width, sprite->height);
                sprite_error = 1;
                break;
            }
        }
        
        spritepack_free(&pack);
        cleanup_temp_files(filename);
        
        if (sprite_error) {
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
 * Unit test: Empty spritepack handling
 */
static void test_empty_spritepack(void) {
    printf("  Unit test: Empty spritepack handling\n");
    
    /* Create empty spritepack file */
    const char* filename = "/tmp/test_empty_spritepack.spk";
    FILE* file = fopen(filename, "wb");
    if (file) {
        uint16_t count = 0;
        fwrite(&count, 2, 1, file);
        fclose(file);
        
        SpritePack pack;
        int result = spritepack_load(&pack, filename);
        
        test_assert(result == 0, "Empty spritepack loads successfully");
        test_assert_eq(0, pack.count, "Empty spritepack has zero count");
        
        spritepack_free(&pack);
        remove(filename);
    } else {
        test_assert(0, "Failed to create test file");
    }
}

/**
 * Unit test: NULL parameter handling
 */
static void test_null_parameters(void) {
    printf("  Unit test: NULL parameter handling\n");
    
    SpritePack pack;
    
    test_assert(spritepack_load(NULL, "test.spk") == -1,
                "spritepack_load returns -1 for NULL pack");
    test_assert(spritepack_load(&pack, NULL) == -1,
                "spritepack_load returns -1 for NULL filename");
    
    test_assert(spritepack_load_lazy(NULL, "test.spk") == -1,
                "spritepack_load_lazy returns -1 for NULL pack");
    test_assert(spritepack_load_lazy(&pack, NULL) == -1,
                "spritepack_load_lazy returns -1 for NULL filename");
    
    test_assert(spritepack_get(NULL, 0) == NULL,
                "spritepack_get returns NULL for NULL pack");
    
    /* Test spritepack_free with NULL */
    spritepack_free(NULL);  /* Should not crash */
    test_assert(1, "spritepack_free handles NULL gracefully");
}

/**
 * Unit test: Invalid file handling
 */
static void test_invalid_file(void) {
    printf("  Unit test: Invalid file handling\n");
    
    SpritePack pack;
    
    test_assert(spritepack_load(&pack, "/nonexistent/path/file.spk") == -2,
                "spritepack_load returns -2 for nonexistent file");
    
    test_assert(spritepack_load_lazy(&pack, "/nonexistent/path/file.spk") == -5,
                "spritepack_load_lazy returns -5 for missing index file");
}

/**
 * Unit test: Index out of bounds
 */
static void test_index_bounds(void) {
    printf("  Unit test: Index out of bounds\n");
    
    /* Create a small spritepack */
    spritepack_pbt_seed = 12345;  /* Fixed seed for reproducibility */
    char* filename = create_temp_spritepack(3, 8, 8);
    
    if (filename) {
        SpritePack pack;
        int result = spritepack_load(&pack, filename);
        
        if (result == 0) {
            test_assert(spritepack_get(&pack, 0) != NULL, "Index 0 is valid");
            test_assert(spritepack_get(&pack, 2) != NULL, "Index 2 is valid");
            test_assert(spritepack_get(&pack, 3) == NULL, "Index 3 returns NULL (out of bounds)");
            test_assert(spritepack_get(&pack, 100) == NULL, "Index 100 returns NULL (out of bounds)");
            test_assert(spritepack_get(&pack, 0xFFFF) == NULL, "Index 0xFFFF returns NULL (out of bounds)");
            
            spritepack_free(&pack);
        } else {
            test_assert(0, "Failed to load test spritepack");
        }
        
        cleanup_temp_files(filename);
    } else {
        test_assert(0, "Failed to create test file");
    }
}

void test_spritepack_parsing(void) {
    int property4_passed;
    int property4b_passed;
    
    /* Initialize random seed for property tests */
    spritepack_pbt_init_seed();
    printf("  (PBT seed: %u)\n", spritepack_pbt_seed);
    
    /* Run property-based tests */
    printf("\n  --- Property-Based Tests ---\n");
    property4_passed = test_property4_spritepack_completeness();
    property4b_passed = test_property4b_spritepack_lazy_loading();
    
    /* Run unit tests */
    printf("\n  --- Unit Tests ---\n");
    test_empty_spritepack();
    test_null_parameters();
    test_invalid_file();
    test_index_bounds();
    
    /* Summary */
    printf("\n  --- Property Test Summary ---\n");
    printf("  Property 4 (SpritePack parsing completeness): %s\n", 
           property4_passed ? "PASSED" : "FAILED");
    printf("  Property 4b (SpritePack lazy loading): %s\n", 
           property4b_passed ? "PASSED" : "FAILED");
}
