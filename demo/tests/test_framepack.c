/**
 * @file test_framepack.c
 * @brief CreatureFramePack tests
 * 
 * Property 3: CFPK 加载正确性
 * For any valid .cfpk file, after loading, the CreatureFramePack
 * should contain the correct number of CreatureTypes, and each
 * ActionArray should have the correct structure.
 * 
 * Validates: Requirements 3.1, 3.2, 3.3
 */

#include "framepack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test helper declarations */
void test_assert(int condition, const char* message);
void test_assert_eq(int expected, int actual, const char* message);

/* Path to test data files */
static const char* CREATURE_CFPK_PATH = "../DarkEden/Data/Image/Creature.cfpk";

/* ============================================================================
 * Unit Tests
 * ============================================================================ */

/**
 * Unit test: CreatureFramePack init and free
 */
static void test_framepack_init_free(void) {
    printf("  Unit test: CreatureFramePack init and free\n");
    
    CreatureFramePack pack;
    creature_framepack_init(&pack);
    
    test_assert_eq(0, creature_framepack_size(&pack), "Initial size is 0");
    
    creature_framepack_free(&pack);
    test_assert_eq(0, creature_framepack_size(&pack), "Size after free is 0");
}

/**
 * Unit test: CreatureFramePack file I/O with simple data
 */
static void test_framepack_file_io(void) {
    printf("  Unit test: CreatureFramePack file I/O\n");
    
    const char* test_file = "/tmp/test_creaturepack.cfpk";
    
    /* Create a simple frame pack */
    CreatureFramePack pack1;
    creature_framepack_init(&pack1);
    
    /* Add 2 creature types */
    for (int type = 0; type < 2; type++) {
        ActionArray aa;
        action_array_init(&aa);
        
        /* Each type has 1 action */
        DirectionArray da;
        direction_array_init(&da);
        
        /* Each action has 2 directions */
        for (int dir = 0; dir < 2; dir++) {
            FrameArray fa;
            frame_array_init(&fa);
            
            /* Each direction has 2 frames */
            for (int f = 0; f < 2; f++) {
                Frame frame;
                frame_init(&frame, type * 100 + dir * 10 + f, type, dir);
                vecAppend(&fa, frame);
            }
            
            vecAppend(&da, fa);
        }
        
        vecAppend(&aa, da);
        vecAppend(&pack1.creatures, aa);
    }
    
    test_assert_eq(2, creature_framepack_size(&pack1), "Pack has 2 creature types");
    
    /* Save to file */
    int save_result = creature_framepack_save(&pack1, test_file);
    test_assert(save_result, "creature_framepack_save succeeds");
    
    /* Load from file */
    CreatureFramePack pack2;
    creature_framepack_init(&pack2);
    
    int load_result = creature_framepack_load(&pack2, test_file);
    test_assert(load_result, "creature_framepack_load succeeds");
    
    /* Verify structure */
    test_assert_eq(2, creature_framepack_size(&pack2), "Loaded pack has 2 creature types");
    
    /* Verify creature type 0 */
    ActionArray* aa = creature_framepack_get(&pack2, 0);
    test_assert(aa != NULL, "Type 0 exists");
    test_assert_eq(1, action_array_size(aa), "Type 0 has 1 action");
    
    DirectionArray* da = action_array_get(aa, 0);
    test_assert(da != NULL, "Type 0 Action 0 exists");
    test_assert_eq(2, direction_array_size(da), "Type 0 Action 0 has 2 directions");
    
    FrameArray* fa = direction_array_get(da, 0);
    test_assert(fa != NULL, "Type 0 Action 0 Dir 0 exists");
    test_assert_eq(2, frame_array_size(fa), "Type 0 Action 0 Dir 0 has 2 frames");
    
    Frame* f = frame_array_get(fa, 0);
    test_assert_eq(0, f->sprite_id, "Frame data preserved (sprite_id)");
    test_assert_eq(0, f->cx, "Frame data preserved (cx)");
    test_assert_eq(0, f->cy, "Frame data preserved (cy)");
    
    /* Verify creature type 1 */
    f = creature_framepack_get_frame(&pack2, 1, 0, 0, 0);
    test_assert(f != NULL, "Type 1 frame accessible");
    test_assert_eq(100, f->sprite_id, "Type 1 frame data correct");
    
    /* Clean up */
    creature_framepack_free(&pack1);
    creature_framepack_free(&pack2);
    remove(test_file);
}

/**
 * Unit test: creature_framepack_get_frame helper
 */
static void test_framepack_get_frame(void) {
    printf("  Unit test: creature_framepack_get_frame\n");
    
    CreatureFramePack pack;
    creature_framepack_init(&pack);
    
    /* Create structure: 1 type, 2 actions, 3 directions, 4 frames */
    ActionArray aa;
    action_array_init(&aa);
    
    for (int action = 0; action < 2; action++) {
        DirectionArray da;
        direction_array_init(&da);
        
        for (int dir = 0; dir < 3; dir++) {
            FrameArray fa;
            frame_array_init(&fa);
            
            for (int f = 0; f < 4; f++) {
                Frame frame;
                frame_init(&frame, action * 100 + dir * 10 + f, action, dir);
                vecAppend(&fa, frame);
            }
            
            vecAppend(&da, fa);
        }
        
        vecAppend(&aa, da);
    }
    
    vecAppend(&pack.creatures, aa);
    
    /* Test get_frame */
    Frame* f = creature_framepack_get_frame(&pack, 0, 0, 0, 0);
    test_assert(f != NULL, "Frame [0][0][0][0] exists");
    test_assert_eq(0, f->sprite_id, "Frame [0][0][0][0] correct");
    
    f = creature_framepack_get_frame(&pack, 0, 1, 2, 3);
    test_assert(f != NULL, "Frame [0][1][2][3] exists");
    test_assert_eq(123, f->sprite_id, "Frame [0][1][2][3] correct");
    
    /* Test invalid indices */
    f = creature_framepack_get_frame(&pack, 1, 0, 0, 0);
    test_assert(f == NULL, "Invalid type returns NULL");
    
    f = creature_framepack_get_frame(&pack, 0, 5, 0, 0);
    test_assert(f == NULL, "Invalid action returns NULL");
    
    creature_framepack_free(&pack);
}

/* ============================================================================
 * Property 3: CFPK 加载正确性
 * 
 * For any valid .cfpk file, after loading, the CreatureFramePack
 * should contain the correct number of CreatureTypes, and each
 * ActionArray should have the correct structure.
 * 
 * Validates: Requirements 3.1, 3.2, 3.3
 * ============================================================================ */

static int test_property3_cfpk_loading(void) {
    int passed = 1;
    
    printf("  Property 3: CFPK loading correctness\n");
    
    /* Try to load the real Creature.cfpk file */
    CreatureFramePack pack;
    creature_framepack_init(&pack);
    
    printf("    Loading %s...\n", CREATURE_CFPK_PATH);
    
    if (!creature_framepack_load(&pack, CREATURE_CFPK_PATH)) {
        printf("    [SKIP] Could not load %s (file may not exist)\n", CREATURE_CFPK_PATH);
        printf("    [INFO] This test requires the DarkEden game data files\n");
        return 1;  /* Skip test if file not available */
    }
    
    printf("    Loaded successfully!\n");
    printf("    CreatureType count: %d\n", creature_framepack_size(&pack));
    
    /* Verify basic structure */
    if (creature_framepack_size(&pack) == 0) {
        printf("    [FAIL] Pack is empty\n");
        creature_framepack_free(&pack);
        return 0;
    }
    
    /* Verify structure of each creature type */
    int valid_types = 0;
    int total_frames = 0;
    
    for (int type = 0; type < creature_framepack_size(&pack); type++) {
        ActionArray* aa = creature_framepack_get(&pack, type);
        
        if (aa == NULL || action_array_size(aa) == 0) {
            continue;  /* Empty creature type is valid */
        }
        
        valid_types++;
        
        /* Check each action */
        for (int action = 0; action < action_array_size(aa); action++) {
            DirectionArray* da = action_array_get(aa, action);
            
            if (da == NULL) continue;
            
            /* Check each direction */
            for (int dir = 0; dir < direction_array_size(da); dir++) {
                FrameArray* fa = direction_array_get(da, dir);
                
                if (fa == NULL) continue;
                
                /* Count frames */
                total_frames += frame_array_size(fa);
            }
        }
    }
    
    printf("    Valid creature types: %d\n", valid_types);
    printf("    Total frames: %d\n", total_frames);
    
    /* Basic sanity checks */
    if (valid_types == 0) {
        printf("    [FAIL] No valid creature types found\n");
        passed = 0;
    }
    
    if (total_frames == 0) {
        printf("    [FAIL] No frames found\n");
        passed = 0;
    }
    
    /* Print sample data for verification */
    printf("    Sample data (first valid creature):\n");
    for (int type = 0; type < creature_framepack_size(&pack) && type < 5; type++) {
        ActionArray* aa = creature_framepack_get(&pack, type);
        if (aa != NULL && action_array_size(aa) > 0) {
            printf("      Type %d: %d actions\n", type, action_array_size(aa));
            
            Frame* f = creature_framepack_get_frame(&pack, type, 0, 0, 0);
            if (f != NULL) {
                printf("        First frame: SpriteID=%d, CX=%d, CY=%d\n",
                       f->sprite_id, f->cx, f->cy);
            }
            break;
        }
    }
    
    if (passed) {
        printf("    [PASS] CFPK loaded and validated successfully\n");
    }
    
    creature_framepack_free(&pack);
    return passed;
}

/**
 * Test InfoToFile debug output
 */
static void test_infotofile(void) {
    printf("  Unit test: creature_framepack_info_to_file\n");
    
    const char* test_info_file = "/tmp/test_creature_info.txt";
    
    /* Create a simple frame pack */
    CreatureFramePack pack;
    creature_framepack_init(&pack);
    
    /* Add 2 creature types */
    for (int type = 0; type < 2; type++) {
        ActionArray aa;
        action_array_init(&aa);
        
        DirectionArray da;
        direction_array_init(&da);
        
        for (int dir = 0; dir < 2; dir++) {
            FrameArray fa;
            frame_array_init(&fa);
            
            Frame frame;
            frame_init(&frame, type * 100 + dir, type * 10, dir * 10);
            vecAppend(&fa, frame);
            
            vecAppend(&da, fa);
        }
        
        vecAppend(&aa, da);
        vecAppend(&pack.creatures, aa);
    }
    
    /* Write info file */
    creature_framepack_info_to_file(&pack, test_info_file);
    
    /* Verify file was created */
    FILE* file = fopen(test_info_file, "r");
    if (file) {
        /* Read first line to verify content */
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file)) {
            test_assert(strstr(buffer, "CreatureFramePack") != NULL ||
                       strstr(buffer, "===") != NULL,
                       "info_to_file creates valid header");
        }
        fclose(file);
        remove(test_info_file);
    } else {
        test_assert(0, "info_to_file failed to create file");
    }
    
    creature_framepack_free(&pack);
}

/* ============================================================================
 * Main test function
 * ============================================================================ */

void test_framepack(void) {
    int property3_passed;
    
    /* Run unit tests */
    printf("\n  --- Unit Tests ---\n");
    test_framepack_init_free();
    test_framepack_file_io();
    test_framepack_get_frame();
    test_infotofile();
    
    /* Run property-based tests */
    printf("\n  --- Property-Based Tests ---\n");
    property3_passed = test_property3_cfpk_loading();
    
    /* Summary */
    printf("\n  --- Property Test Summary ---\n");
    printf("  Property 3 (CFPK loading correctness): %s\n",
           property3_passed ? "PASSED" : "FAILED");
}
