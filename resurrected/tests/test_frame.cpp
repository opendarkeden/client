/**
 * @file test_frame.cpp
 * @brief TArray and CFrame unit tests and property-based tests
 * 
 * Property 1: TArray 数据一致性
 * For any TArray, after Init(size) and setting values via operator[],
 * GetSize() should return size and operator[] should return the same values.
 * 
 * Property 2: CFrame 数据存储
 * For any CFrame, after Set(spriteID, cx, cy), GetSpriteID(), GetCX(), GetCY()
 * should return the same values.
 * 
 * Validates: Requirements 3.4, 3.5, 4.6
 */

#include "frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Test helper declarations */
extern "C" {
    void test_assert(int condition, const char* message);
    void test_assert_eq(int expected, int actual, const char* message);
}

/* Simple pseudo-random number generator for property testing */
static unsigned int frame_pbt_seed = 0;

static void frame_pbt_init_seed(void) {
    frame_pbt_seed = (unsigned int)time(NULL);
}

static unsigned int frame_pbt_random(void) {
    frame_pbt_seed = frame_pbt_seed * 1103515245 + 12345;
    return frame_pbt_seed;
}

static uint16_t frame_pbt_random_uint16(void) {
    return (uint16_t)(frame_pbt_random() >> 16);
}

static int16_t frame_pbt_random_int16(void) {
    return (int16_t)(frame_pbt_random() >> 16);
}

/* ============================================================================
 * Property 1: TArray 数据一致性
 * 
 * For any TArray<CFrame, uint16_t>, after Init(size) and setting values,
 * GetSize() should return size and operator[] should return the same values.
 * 
 * Validates: Requirements 4.6
 * ============================================================================ */

static int test_property1_tarray_consistency(void) {
    const int NUM_ITERATIONS = 100;
    const int MAX_SIZE = 256;
    int passed = 1;
    
    printf("  Property 1: TArray data consistency\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        /* Generate random size (1 to MAX_SIZE) */
        uint16_t size = 1 + (frame_pbt_random() % MAX_SIZE);
        
        /* Create TArray */
        TArray<CFrame, uint16_t> array;
        array.Init(size);
        
        /* Verify size */
        if (array.GetSize() != size) {
            printf("    [FAIL] Iteration %d: Size mismatch\n", i);
            printf("           Expected: %d, Got: %d\n", size, array.GetSize());
            passed = 0;
            break;
        }
        
        /* Generate and store random CFrame values */
        TYPE_SPRITEID* expected_ids = new TYPE_SPRITEID[size];
        short* expected_cx = new short[size];
        short* expected_cy = new short[size];
        
        for (uint16_t j = 0; j < size; j++) {
            expected_ids[j] = frame_pbt_random_uint16();
            expected_cx[j] = frame_pbt_random_int16();
            expected_cy[j] = frame_pbt_random_int16();
            
            array[j].Set(expected_ids[j], expected_cx[j], expected_cy[j]);
        }
        
        /* Verify all values can be retrieved correctly */
        int mismatch = 0;
        for (uint16_t j = 0; j < size; j++) {
            if (array[j].GetSpriteID() != expected_ids[j] ||
                array[j].GetCX() != expected_cx[j] ||
                array[j].GetCY() != expected_cy[j]) {
                printf("    [FAIL] Iteration %d: Value mismatch at index %d\n", i, j);
                printf("           Expected: ID=%d, CX=%d, CY=%d\n",
                       expected_ids[j], expected_cx[j], expected_cy[j]);
                printf("           Got: ID=%d, CX=%d, CY=%d\n",
                       array[j].GetSpriteID(), array[j].GetCX(), array[j].GetCY());
                mismatch = 1;
                break;
            }
        }
        
        delete[] expected_ids;
        delete[] expected_cx;
        delete[] expected_cy;
        
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

/* ============================================================================
 * Property 2: CFrame 数据存储
 * 
 * For any CFrame, after Set(spriteID, cx, cy), GetSpriteID(), GetCX(), GetCY()
 * should return the same values.
 * 
 * Validates: Requirements 3.4, 3.5
 * ============================================================================ */

static int test_property2_cframe_storage(void) {
    const int NUM_ITERATIONS = 100;
    int passed = 1;
    
    printf("  Property 2: CFrame data storage\n");
    printf("    Running %d iterations...\n", NUM_ITERATIONS);
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        /* Generate random values */
        TYPE_SPRITEID spriteID = frame_pbt_random_uint16();
        short cx = frame_pbt_random_int16();
        short cy = frame_pbt_random_int16();
        
        /* Test constructor */
        CFrame frame1(spriteID, cx, cy);
        
        if (frame1.GetSpriteID() != spriteID ||
            frame1.GetCX() != cx ||
            frame1.GetCY() != cy) {
            printf("    [FAIL] Iteration %d: Constructor mismatch\n", i);
            printf("           Expected: ID=%d, CX=%d, CY=%d\n", spriteID, cx, cy);
            printf("           Got: ID=%d, CX=%d, CY=%d\n",
                   frame1.GetSpriteID(), frame1.GetCX(), frame1.GetCY());
            passed = 0;
            break;
        }
        
        /* Test Set method */
        CFrame frame2;
        frame2.Set(spriteID, cx, cy);
        
        if (frame2.GetSpriteID() != spriteID ||
            frame2.GetCX() != cx ||
            frame2.GetCY() != cy) {
            printf("    [FAIL] Iteration %d: Set method mismatch\n", i);
            printf("           Expected: ID=%d, CX=%d, CY=%d\n", spriteID, cx, cy);
            printf("           Got: ID=%d, CX=%d, CY=%d\n",
                   frame2.GetSpriteID(), frame2.GetCX(), frame2.GetCY());
            passed = 0;
            break;
        }
        
        /* Test assignment operator */
        CFrame frame3;
        frame3 = frame1;
        
        if (frame3.GetSpriteID() != spriteID ||
            frame3.GetCX() != cx ||
            frame3.GetCY() != cy) {
            printf("    [FAIL] Iteration %d: Assignment operator mismatch\n", i);
            printf("           Expected: ID=%d, CX=%d, CY=%d\n", spriteID, cx, cy);
            printf("           Got: ID=%d, CX=%d, CY=%d\n",
                   frame3.GetSpriteID(), frame3.GetCX(), frame3.GetCY());
            passed = 0;
            break;
        }
    }
    
    if (passed) {
        printf("    [PASS] All %d iterations passed\n", NUM_ITERATIONS);
    }
    
    return passed;
}

/* ============================================================================
 * Unit Tests
 * ============================================================================ */

/**
 * Unit test: TArray Init and Release
 */
static void test_tarray_init_release(void) {
    printf("  Unit test: TArray Init and Release\n");
    
    TArray<CFrame, uint16_t> array;
    
    /* Initial state */
    test_assert_eq(0, array.GetSize(), "Initial size is 0");
    
    /* Init with size */
    array.Init(10);
    test_assert_eq(10, array.GetSize(), "Size after Init(10) is 10");
    
    /* Re-init with different size */
    array.Init(5);
    test_assert_eq(5, array.GetSize(), "Size after Init(5) is 5");
    
    /* Release */
    array.Release();
    test_assert_eq(0, array.GetSize(), "Size after Release is 0");
    
    /* Init with 0 */
    array.Init(0);
    test_assert_eq(0, array.GetSize(), "Size after Init(0) is 0");
}

/**
 * Unit test: TArray assignment operator
 */
static void test_tarray_assignment(void) {
    printf("  Unit test: TArray assignment operator\n");
    
    TArray<CFrame, uint16_t> array1;
    array1.Init(3);
    array1[0].Set(100, 10, 20);
    array1[1].Set(200, 30, 40);
    array1[2].Set(300, 50, 60);
    
    TArray<CFrame, uint16_t> array2;
    array2 = array1;
    
    test_assert_eq(3, array2.GetSize(), "Assigned array has correct size");
    test_assert_eq(100, array2[0].GetSpriteID(), "Element 0 SpriteID copied correctly");
    test_assert_eq(200, array2[1].GetSpriteID(), "Element 1 SpriteID copied correctly");
    test_assert_eq(300, array2[2].GetSpriteID(), "Element 2 SpriteID copied correctly");
}

/**
 * Unit test: TArray += operator
 */
static void test_tarray_append(void) {
    printf("  Unit test: TArray += operator\n");
    
    TArray<CFrame, uint16_t> array1;
    array1.Init(2);
    array1[0].Set(100, 0, 0);
    array1[1].Set(200, 0, 0);
    
    TArray<CFrame, uint16_t> array2;
    array2.Init(2);
    array2[0].Set(300, 0, 0);
    array2[1].Set(400, 0, 0);
    
    array1 += array2;
    
    test_assert_eq(4, array1.GetSize(), "Combined array has correct size");
    test_assert_eq(100, array1[0].GetSpriteID(), "Element 0 preserved");
    test_assert_eq(200, array1[1].GetSpriteID(), "Element 1 preserved");
    test_assert_eq(300, array1[2].GetSpriteID(), "Element 2 appended");
    test_assert_eq(400, array1[3].GetSpriteID(), "Element 3 appended");
}

/**
 * Unit test: CFrame default constructor
 */
static void test_cframe_default_constructor(void) {
    printf("  Unit test: CFrame default constructor\n");
    
    CFrame frame;
    
    test_assert_eq(0, frame.GetSpriteID(), "Default SpriteID is 0");
    test_assert_eq(0, frame.GetCX(), "Default CX is 0");
    test_assert_eq(0, frame.GetCY(), "Default CY is 0");
}

/**
 * Unit test: CFrame file I/O
 */
static void test_cframe_file_io(void) {
    printf("  Unit test: CFrame file I/O\n");
    
    const char* test_file = "/tmp/test_cframe.bin";
    
    /* Create and save frame */
    CFrame frame1(12345, -100, 200);
    
    FILE* file = fopen(test_file, "wb");
    if (file) {
        int save_result = frame1.SaveToFile(file) ? 1 : 0;
        fclose(file);
        test_assert(save_result, "CFrame SaveToFile succeeds");
        
        /* Load frame */
        CFrame frame2;
        file = fopen(test_file, "rb");
        if (file) {
            int load_result = frame2.LoadFromFile(file) ? 1 : 0;
            fclose(file);
            test_assert(load_result, "CFrame LoadFromFile succeeds");
            
            test_assert_eq(12345, frame2.GetSpriteID(), "Loaded SpriteID matches");
            test_assert_eq(-100, frame2.GetCX(), "Loaded CX matches");
            test_assert_eq(200, frame2.GetCY(), "Loaded CY matches");
        } else {
            test_assert(0, "Failed to open file for reading");
        }
        
        /* Clean up */
        remove(test_file);
    } else {
        test_assert(0, "Failed to open file for writing");
    }
}

/**
 * Unit test: TArray file I/O
 */
static void test_tarray_file_io(void) {
    printf("  Unit test: TArray file I/O\n");
    
    const char* test_file = "/tmp/test_tarray.bin";
    
    /* Create and save array */
    TArray<CFrame, uint16_t> array1;
    array1.Init(3);
    array1[0].Set(100, 10, 20);
    array1[1].Set(200, 30, 40);
    array1[2].Set(300, 50, 60);
    
    FILE* file = fopen(test_file, "wb");
    if (file) {
        int save_result = array1.SaveToFile(file) ? 1 : 0;
        fclose(file);
        test_assert(save_result, "TArray SaveToFile succeeds");
        
        /* Load array */
        TArray<CFrame, uint16_t> array2;
        file = fopen(test_file, "rb");
        if (file) {
            int load_result = array2.LoadFromFile(file) ? 1 : 0;
            fclose(file);
            test_assert(load_result, "TArray LoadFromFile succeeds");
            
            test_assert_eq(3, array2.GetSize(), "Loaded array has correct size");
            test_assert_eq(100, array2[0].GetSpriteID(), "Element 0 loaded correctly");
            test_assert_eq(200, array2[1].GetSpriteID(), "Element 1 loaded correctly");
            test_assert_eq(300, array2[2].GetSpriteID(), "Element 2 loaded correctly");
        } else {
            test_assert(0, "Failed to open file for reading");
        }
        
        /* Clean up */
        remove(test_file);
    } else {
        test_assert(0, "Failed to open file for writing");
    }
}

/**
 * Unit test: Nested TArray (DIRECTION_FRAME_ARRAY)
 */
static void test_nested_tarray(void) {
    printf("  Unit test: Nested TArray (DIRECTION_FRAME_ARRAY)\n");
    
    /* Create DIRECTION_FRAME_ARRAY (8 directions) */
    DIRECTION_FRAME_ARRAY dir_array;
    dir_array.Init(8);
    
    test_assert_eq(8, dir_array.GetSize(), "Direction array has 8 elements");
    
    /* Initialize each direction with different frame counts */
    for (uint8_t d = 0; d < 8; d++) {
        dir_array[d].Init(d + 1);  /* Direction 0 has 1 frame, direction 7 has 8 frames */
        
        for (uint16_t f = 0; f < dir_array[d].GetSize(); f++) {
            dir_array[d][f].Set(d * 100 + f, d, f);
        }
    }
    
    /* Verify structure */
    test_assert_eq(1, dir_array[0].GetSize(), "Direction 0 has 1 frame");
    test_assert_eq(8, dir_array[7].GetSize(), "Direction 7 has 8 frames");
    test_assert_eq(0, dir_array[0][0].GetSpriteID(), "Direction 0, Frame 0 has correct ID");
    test_assert_eq(705, dir_array[7][5].GetSpriteID(), "Direction 7, Frame 5 has correct ID");
}

/* ============================================================================
 * Main test function
 * ============================================================================ */

extern "C" void test_frame_and_tarray(void) {
    int property1_passed;
    int property2_passed;
    
    /* Initialize random seed for property tests */
    frame_pbt_init_seed();
    printf("  (PBT seed: %u)\n", frame_pbt_seed);
    
    /* Run property-based tests */
    printf("\n  --- Property-Based Tests ---\n");
    property1_passed = test_property1_tarray_consistency();
    property2_passed = test_property2_cframe_storage();
    
    /* Run unit tests */
    printf("\n  --- Unit Tests ---\n");
    test_tarray_init_release();
    test_tarray_assignment();
    test_tarray_append();
    test_cframe_default_constructor();
    test_cframe_file_io();
    test_tarray_file_io();
    test_nested_tarray();
    
    /* Summary */
    printf("\n  --- Property Test Summary ---\n");
    printf("  Property 1 (TArray data consistency): %s\n", 
           property1_passed ? "PASSED" : "FAILED");
    printf("  Property 2 (CFrame data storage): %s\n", 
           property2_passed ? "PASSED" : "FAILED");
}
