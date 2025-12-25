/**
 * @file colorset.h
 * @brief ColorSet system for IndexedSprite color mapping
 * 
 * Requirements: 6.1, 6.2, 6.3, 6.4
 * 
 * ColorSet provides color lookup tables for IndexedSprite rendering.
 * Each ColorSet contains 30 gradation levels, and there are 495 ColorSets
 * (33 seeds × 15 modifications).
 */

#ifndef COLORSET_H
#define COLORSET_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Constants
 * ============================================================================ */

/**
 * Maximum number of ColorSet seeds (base colors)
 */
#define MAX_COLORSET_SEED           33

/**
 * Number of modifications per seed
 */
#define MAX_COLORSET_SEED_MODIFY    15

/**
 * Total number of ColorSets (33 × 15 = 495)
 */
#define MAX_COLORSET                495

/**
 * Number of gradation levels per ColorSet
 */
#define MAX_COLORGRADATION          30

/**
 * Half of gradation levels
 */
#define MAX_COLORGRADATION_HALF     15

/**
 * Maximum value for color to gradation conversion (R+G+B max)
 */
#define MAX_COLOR_TO_GRADATION      93

/**
 * Number of darkness levels
 */
#define MAX_DARKBIT                 5

/**
 * Maximum number of ColorSets that can be used per sprite
 */
#define MAX_COLORSET_USE            256

/**
 * Index values for special pixels
 */
#define INDEX_NULL                  0xFF
#define INDEX_SELECT                0xFE
#define INDEX_TRANS                 0xFD
#define INDEXSET_NULL               0xFF

/* ============================================================================
 * ColorSet Tables (Global)
 * ============================================================================ */

/**
 * Main ColorSet table [495][30]
 * ColorSet[setIndex][gradation] = RGB565 color
 */
extern uint16_t g_ColorSet[MAX_COLORSET][MAX_COLORGRADATION];

/**
 * Gradation values for each level
 */
extern uint16_t g_GradationValue[MAX_COLORGRADATION];

/**
 * Darkness-adjusted ColorSet table [5][495][30]
 */
extern uint16_t g_ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];

/**
 * Color to gradation mapping table
 */
extern uint8_t g_ColorToGradation[MAX_COLOR_TO_GRADATION];

/**
 * Currently active ColorSet indices for rendering
 * s_IndexValue[setNumber] = colorSetIndex
 */
extern int g_IndexValue[MAX_COLORSET_USE];

/* ============================================================================
 * Functions
 * ============================================================================ */

/**
 * Initialize ColorSet tables with computed values
 * This generates all 495 ColorSets from 33 seed colors
 * 
 * Requirements: 6.1, 6.2, 6.3
 */
void colorset_init(void);

/**
 * Set the active ColorSets for rendering
 * 
 * @param set1 ColorSet index for primary color (index 0)
 * @param set2 ColorSet index for secondary color (index 1)
 * 
 * Requirements: 6.4
 */
void colorset_set_using(int set1, int set2);

/**
 * Set a specific ColorSet slot
 * 
 * @param setNumber Slot number (0-255)
 * @param colorSet ColorSet index to use
 */
void colorset_set_using_only(uint8_t setNumber, int colorSet);

/**
 * Get the ColorSet index for a slot
 * 
 * @param n Slot number
 * @return ColorSet index
 */
int colorset_get_using(uint8_t n);

/**
 * Load ColorSet tables from file
 * 
 * @param file File handle to read from
 * @return 1 on success, 0 on failure
 */
int colorset_load_from_file(FILE* file);

/**
 * Save ColorSet tables to file
 * 
 * @param file File handle to write to
 * @return 1 on success, 0 on failure
 */
int colorset_save_to_file(FILE* file);

/**
 * Convert a color value to gradation index
 * 
 * @param color RGB565 color value
 * @return Gradation index (0 to MAX_COLORGRADATION-1)
 */
uint8_t colorset_get_index_color(uint16_t color);

/**
 * Convert R+G+B sum to gradation index
 * 
 * @param spriteGradation Sum of R+G+B components (0-93)
 * @return Gradation index
 */
uint8_t colorset_color_to_gradation(uint8_t spriteGradation);

/**
 * Create RGB565 color from 5-bit components
 * 
 * @param r Red component (0-31)
 * @param g Green component (0-31, will be scaled to 6-bit)
 * @param b Blue component (0-31)
 * @return RGB565 color value
 */
uint16_t colorset_make_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * Extract red component from RGB565
 * 
 * @param color RGB565 color
 * @return Red component (0-31)
 */
uint8_t colorset_get_red(uint16_t color);

/**
 * Extract green component from RGB565
 * 
 * @param color RGB565 color
 * @return Green component (0-31, scaled from 6-bit)
 */
uint8_t colorset_get_green(uint16_t color);

/**
 * Extract blue component from RGB565
 * 
 * @param color RGB565 color
 * @return Blue component (0-31)
 */
uint8_t colorset_get_blue(uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* COLORSET_H */
