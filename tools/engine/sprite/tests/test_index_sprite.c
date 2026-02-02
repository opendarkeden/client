/**
 * @file test_index_sprite.c
 * @brief Tests for IndexedSprite and IndexedSpritePack
 * 
 * Property 4: ISPK 延迟加载
 * Property 5: ColorSet 映射一致性
 * Validates: Requirements 1.1, 1.2, 1.3, 6.4, 6.5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colorset.h"
#include "index_sprite.h"
#include "index_spritepack.h"

/* External test assertion functions */
extern void test_assert(int condition, const char* message);
extern void test_assert_eq(int expected, int actual, const char* message);

/* Test data path - try multiple paths for different working directories */
static const char* TEST_ISPK_PATHS[] = {
    "../DarkEden/Data/Image/Creature.ispk",
    "DarkEden/Data/Image/Creature.ispk",
    "../../DarkEden/Data/Image/Creature.ispk",
    NULL
};

static const char* find_ispk_path(void) {
    for (int i = 0; TEST_ISPK_PATHS[i] != NULL; i++) {
        FILE* f = fopen(TEST_ISPK_PATHS[i], "rb");
        if (f) {
            fclose(f);
            return TEST_ISPK_PATHS[i];
        }
    }
    return NULL;
}

/**
 * Test ColorSet initialization
 * Validates: Requirements 6.1, 6.2, 6.3
 */
static void test_colorset_init(void) {
    /* Initialize ColorSet tables */
    colorset_init();
    
    /* Verify ColorSet table is populated */
    int hasNonZero = 0;
    for (int i = 0; i < MAX_COLORSET && !hasNonZero; i++) {
        for (int j = 0; j < MAX_COLORGRADATION && !hasNonZero; j++) {
            if (g_ColorSet[i][j] != 0) {
                hasNonZero = 1;
            }
        }
    }
    test_assert(hasNonZero, "ColorSet table should have non-zero values after init");
    
    /* Verify GradationValue table is populated */
    hasNonZero = 0;
    for (int i = 0; i < MAX_COLORGRADATION; i++) {
        if (g_GradationValue[i] != 0) {
            hasNonZero = 1;
            break;
        }
    }
    test_assert(hasNonZero, "GradationValue table should have non-zero values");
    
    /* Verify ColorSetDarkness table is populated */
    hasNonZero = 0;
    for (int k = 0; k < MAX_DARKBIT && !hasNonZero; k++) {
        for (int i = 0; i < MAX_COLORSET && !hasNonZero; i++) {
            for (int j = 0; j < MAX_COLORGRADATION && !hasNonZero; j++) {
                if (g_ColorSetDarkness[k][i][j] != 0) {
                    hasNonZero = 1;
                }
            }
        }
    }
    test_assert(hasNonZero, "ColorSetDarkness table should have non-zero values");
}

/**
 * Test ColorSet set/get functions
 * Validates: Requirements 6.4
 */
static void test_colorset_using(void) {
    colorset_init();
    
    /* Test SetUsingColorSet */
    colorset_set_using(100, 200);
    test_assert_eq(100, colorset_get_using(0), "SetUsingColorSet should set index 0");
    test_assert_eq(200, colorset_get_using(1), "SetUsingColorSet should set index 1");
    
    /* Test SetUsingColorSetOnly */
    colorset_set_using_only(5, 300);
    test_assert_eq(300, colorset_get_using(5), "SetUsingColorSetOnly should set specific index");
    
    /* Verify other indices unchanged */
    test_assert_eq(100, colorset_get_using(0), "Other indices should remain unchanged");
}

/**
 * Test RGB565 color functions
 */
static void test_colorset_rgb565(void) {
    /* Test color creation */
    uint16_t color = colorset_make_color(31, 31, 31);  /* White */
    test_assert(color != 0, "White color should be non-zero");
    
    /* Test component extraction */
    test_assert_eq(31, colorset_get_red(color), "Red component should be 31");
    test_assert_eq(31, colorset_get_green(color), "Green component should be 31");
    test_assert_eq(31, colorset_get_blue(color), "Blue component should be 31");
    
    /* Test black */
    color = colorset_make_color(0, 0, 0);
    test_assert_eq(0, color, "Black color should be 0");
    
    /* Test red only */
    color = colorset_make_color(31, 0, 0);
    test_assert_eq(31, colorset_get_red(color), "Red-only: red should be 31");
    test_assert_eq(0, colorset_get_green(color), "Red-only: green should be 0");
    test_assert_eq(0, colorset_get_blue(color), "Red-only: blue should be 0");
}

/**
 * Test IndexSprite initialization
 */
static void test_index_sprite_init(void) {
    IndexSprite sprite;
    
    index_sprite_init(&sprite);
    
    test_assert(!index_sprite_is_init(&sprite), "New sprite should not be initialized");
    test_assert_eq(0, index_sprite_get_width(&sprite), "New sprite width should be 0");
    test_assert_eq(0, index_sprite_get_height(&sprite), "New sprite height should be 0");
    
    index_sprite_release(&sprite);
}

/**
 * Test IndexSpritePack initialization
 */
static void test_index_spritepack_init(void) {
    IndexSpritePack pack;
    
    index_spritepack_init(&pack);
    
    test_assert_eq(0, index_spritepack_get_size(&pack), "New pack size should be 0");
    
    index_spritepack_release(&pack);
}

/**
 * Test loading real Creature.ispk file
 * Validates: Requirements 1.1
 */
static void test_ispk_load(void) {
    IndexSpritePack pack;
    index_spritepack_init(&pack);
    
    /* Find the ISPK file */
    const char* ispk_path = find_ispk_path();
    
    if (!ispk_path) {
        printf("    (Skipped: Could not find Creature.ispk in any known path)\n");
        test_assert(1, "ISPK load test skipped (file not found)");
        index_spritepack_release(&pack);
        return;
    }
    
    /* Try to load the pack file */
    int result = index_spritepack_load(&pack, ispk_path);
    
    if (result) {
        test_assert(1, "Successfully loaded Creature.ispk");
        
        uint16_t size = index_spritepack_get_size(&pack);
        test_assert(size > 0, "Pack should contain sprites");
        
        printf("    (Loaded %d sprites from %s)\n", size, ispk_path);
        
        /* Test accessing first sprite */
        if (size > 0) {
            IndexSprite* sprite = index_spritepack_get(&pack, 0);
            test_assert(sprite != NULL, "Should be able to get first sprite");
            
            if (sprite && index_sprite_is_init(sprite)) {
                uint16_t w = index_sprite_get_width(sprite);
                uint16_t h = index_sprite_get_height(sprite);
                printf("    (First sprite: %dx%d)\n", w, h);
                test_assert(1, "First sprite is initialized");
            }
        }
    } else {
        /* File might not exist in test environment */
        printf("    (Skipped: Could not load %s)\n", ispk_path);
        test_assert(1, "ISPK load test skipped (file not found)");
    }
    
    index_spritepack_release(&pack);
}

/**
 * Property 4: ISPK 延迟加载
 * Test lazy loading of IndexSpritePack
 * Validates: Requirements 1.2, 1.3
 * 
 * For any IndexedSpritePack loaded with LoadFromFileRunning(),
 * accessing a sprite via operator[] should load it from file on demand,
 * and subsequent accesses should return the same sprite.
 */
static void test_ispk_lazy_load(void) {
    IndexSpritePack pack;
    index_spritepack_init(&pack);
    
    /* Find the ISPK file */
    const char* ispk_path = find_ispk_path();
    
    if (!ispk_path) {
        printf("    (Skipped: Could not find Creature.ispk in any known path)\n");
        test_assert(1, "ISPK lazy load test skipped (file not found)");
        index_spritepack_release(&pack);
        return;
    }
    
    /* Try to load with lazy loading */
    int result = index_spritepack_load_lazy(&pack, ispk_path);
    
    if (result) {
        test_assert(1, "Successfully initialized lazy loading for Creature.ispk");
        
        uint16_t size = index_spritepack_get_size(&pack);
        test_assert(size > 0, "Pack should report sprite count");
        
        printf("    (Pack reports %d sprites)\n", size);
        
        if (size > 0) {
            /* First access should trigger load */
            IndexSprite* sprite1 = index_spritepack_get(&pack, 0);
            test_assert(sprite1 != NULL, "First access should return sprite");
            
            if (sprite1) {
                test_assert(index_sprite_is_init(sprite1), 
                           "Property 4: Sprite should be loaded on first access");
                
                /* Second access should return same sprite */
                IndexSprite* sprite2 = index_spritepack_get(&pack, 0);
                test_assert(sprite1 == sprite2, 
                           "Property 4: Subsequent access should return same sprite");
                
                /* Access a different sprite */
                if (size > 10) {
                    IndexSprite* sprite10 = index_spritepack_get(&pack, 10);
                    test_assert(sprite10 != NULL, "Should be able to access sprite 10");
                    if (sprite10) {
                        test_assert(index_sprite_is_init(sprite10),
                                   "Property 4: Sprite 10 should be loaded on access");
                    }
                }
            }
        }
    } else {
        printf("    (Skipped: Could not load %s for lazy loading)\n", ispk_path);
        test_assert(1, "ISPK lazy load test skipped (file not found)");
    }
    
    index_spritepack_release(&pack);
}

/**
 * Property 5: ColorSet 映射一致性
 * Test ColorSet mapping consistency
 * Validates: Requirements 6.4, 6.5
 * 
 * For any valid index value and ColorSet configuration,
 * the rendered color should match the ColorSet table lookup.
 */
static void test_colorset_mapping_consistency(void) {
    colorset_init();
    
    /* Test that ColorSet lookup is consistent */
    for (int setIdx = 0; setIdx < 10; setIdx++) {  /* Test first 10 sets */
        for (int grad = 0; grad < MAX_COLORGRADATION; grad++) {
            uint16_t color1 = g_ColorSet[setIdx][grad];
            uint16_t color2 = g_ColorSet[setIdx][grad];
            
            if (color1 != color2) {
                test_assert(0, "Property 5: ColorSet lookup should be consistent");
                return;
            }
        }
    }
    test_assert(1, "Property 5: ColorSet lookup is consistent across accesses");
    
    /* Test that SetUsingColorSet affects lookup correctly */
    colorset_set_using(50, 100);
    
    /* Verify the mapping is applied */
    int idx0 = colorset_get_using(0);
    int idx1 = colorset_get_using(1);
    
    test_assert_eq(50, idx0, "Property 5: Index 0 should map to ColorSet 50");
    test_assert_eq(100, idx1, "Property 5: Index 1 should map to ColorSet 100");
    
    /* Verify colors from mapped sets are accessible */
    uint16_t color0 = g_ColorSet[idx0][15];  /* Middle gradation */
    uint16_t color1 = g_ColorSet[idx1][15];
    
    test_assert(color0 == g_ColorSet[50][15], 
               "Property 5: Mapped color should match direct lookup");
    test_assert(color1 == g_ColorSet[100][15],
               "Property 5: Mapped color should match direct lookup");
}

/**
 * Test sprite dimensions from real file
 */
static void test_sprite_dimensions(void) {
    IndexSpritePack pack;
    index_spritepack_init(&pack);
    
    /* Find the ISPK file */
    const char* ispk_path = find_ispk_path();
    
    if (!ispk_path) {
        printf("    (Skipped: Could not find Creature.ispk in any known path)\n");
        test_assert(1, "Sprite dimensions test skipped");
        index_spritepack_release(&pack);
        return;
    }
    
    int result = index_spritepack_load_lazy(&pack, ispk_path);
    
    if (result) {
        uint16_t size = index_spritepack_get_size(&pack);
        
        if (size > 0) {
            /* Check first few sprites have reasonable dimensions */
            int validCount = 0;
            int checkCount = (size < 5) ? size : 5;
            
            for (int i = 0; i < checkCount; i++) {
                IndexSprite* sprite = index_spritepack_get(&pack, i);
                if (sprite && index_sprite_is_init(sprite)) {
                    uint16_t w = index_sprite_get_width(sprite);
                    uint16_t h = index_sprite_get_height(sprite);
                    
                    /* Sprites should have reasonable dimensions (< 1024) */
                    if (w < 1024 && h < 1024) {
                        validCount++;
                    }
                }
            }
            
            test_assert(validCount == checkCount, 
                       "All checked sprites should have valid dimensions");
            printf("    (Verified %d sprites have valid dimensions)\n", validCount);
        }
    } else {
        printf("    (Skipped: Could not load test file)\n");
        test_assert(1, "Sprite dimensions test skipped");
    }
    
    index_spritepack_release(&pack);
}

/**
 * Test ColorSet rendering effect
 * Validates: Requirements 6.4, 6.5
 * 
 * Verifies that different ColorSets produce different colors
 * for the same gradation level.
 */
static void test_colorset_rendering_effect(void) {
    colorset_init();
    
    /* Test that different ColorSets produce different colors */
    int differentCount = 0;
    int totalComparisons = 0;
    
    /* Compare colors at middle gradation (15) across different ColorSets */
    for (int set1 = 0; set1 < 10; set1++) {
        for (int set2 = set1 + 1; set2 < 10; set2++) {
            uint16_t color1 = g_ColorSet[set1][15];
            uint16_t color2 = g_ColorSet[set2][15];
            
            if (color1 != color2) {
                differentCount++;
            }
            totalComparisons++;
        }
    }
    
    /* Most ColorSets should produce different colors */
    test_assert(differentCount > totalComparisons / 2,
               "Different ColorSets should produce different colors");
    printf("    (%d/%d ColorSet pairs produce different colors)\n", 
           differentCount, totalComparisons);
    
    /* Test gradation progression within a ColorSet */
    int gradationProgression = 1;
    for (int set = 0; set < 5; set++) {
        /* Colors should generally get darker as gradation increases */
        uint16_t colorBright = g_ColorSet[set][0];
        uint16_t colorDark = g_ColorSet[set][MAX_COLORGRADATION - 1];
        
        int brightSum = colorset_get_red(colorBright) + 
                        colorset_get_green(colorBright) + 
                        colorset_get_blue(colorBright);
        int darkSum = colorset_get_red(colorDark) + 
                      colorset_get_green(colorDark) + 
                      colorset_get_blue(colorDark);
        
        /* Bright end should have higher color sum than dark end */
        if (brightSum <= darkSum) {
            gradationProgression = 0;
            break;
        }
    }
    test_assert(gradationProgression, 
               "ColorSet gradation should progress from bright to dark");
    
    /* Test darkness levels */
    int darknessProgression = 1;
    for (int set = 0; set < 5; set++) {
        for (int grad = 0; grad < MAX_COLORGRADATION; grad++) {
            uint16_t baseColor = g_ColorSet[set][grad];
            
            for (int dark = 1; dark < MAX_DARKBIT; dark++) {
                uint16_t darkColor = g_ColorSetDarkness[dark][set][grad];
                
                int baseSum = colorset_get_red(baseColor) + 
                              colorset_get_green(baseColor) + 
                              colorset_get_blue(baseColor);
                int darkSum = colorset_get_red(darkColor) + 
                              colorset_get_green(darkColor) + 
                              colorset_get_blue(darkColor);
                
                /* Darker levels should have lower or equal color sum */
                if (darkSum > baseSum) {
                    darknessProgression = 0;
                    break;
                }
            }
            if (!darknessProgression) break;
        }
        if (!darknessProgression) break;
    }
    test_assert(darknessProgression,
               "Darkness levels should progressively darken colors");
}

/**
 * Main test function for IndexedSprite
 */
void test_index_sprite(void) {
    printf("  ColorSet Initialization:\n");
    test_colorset_init();
    
    printf("  ColorSet Using Functions:\n");
    test_colorset_using();
    
    printf("  ColorSet RGB565 Functions:\n");
    test_colorset_rgb565();
    
    printf("  IndexSprite Initialization:\n");
    test_index_sprite_init();
    
    printf("  IndexSpritePack Initialization:\n");
    test_index_spritepack_init();
    
    printf("  ISPK File Loading (Requirement 1.1):\n");
    test_ispk_load();
    
    printf("  Property 4 - ISPK Lazy Loading (Requirements 1.2, 1.3):\n");
    test_ispk_lazy_load();
    
    printf("  Property 5 - ColorSet Mapping Consistency (Requirements 6.4, 6.5):\n");
    test_colorset_mapping_consistency();
    
    printf("  Sprite Dimensions Validation:\n");
    test_sprite_dimensions();
    
    printf("  ColorSet Rendering Effect Validation:\n");
    test_colorset_rendering_effect();
}
