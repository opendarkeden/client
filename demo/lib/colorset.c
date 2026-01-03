/**
 * @file colorset.c
 * @brief ColorSet system implementation
 * 
 * Requirements: 6.1, 6.2, 6.3, 6.4
 * 
 * This implements the ColorSet color lookup system used by IndexedSprite
 * for creature color variations. The system supports 495 ColorSets
 * (33 seeds × 15 modifications) with 30 gradation levels each.
 */

#include "colorset.h"
#include <string.h>

/* ============================================================================
 * Global Tables
 * ============================================================================ */

uint16_t g_ColorSet[MAX_COLORSET][MAX_COLORGRADATION];
uint16_t g_GradationValue[MAX_COLORGRADATION];
uint16_t g_ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];
uint8_t g_ColorToGradation[MAX_COLOR_TO_GRADATION];
int g_IndexValue[MAX_COLORSET_USE];

/* ============================================================================
 * RGB565 Helper Functions
 * ============================================================================ */

uint16_t colorset_make_color(uint8_t r, uint8_t g, uint8_t b) {
    /* RGB565: RRRRR GGGGGG BBBBB
     * Red:   5 bits at position 11-15
     * Green: 6 bits at position 5-10 (scale 5-bit to 6-bit: g * 2)
     * Blue:  5 bits at position 0-4
     */
    return ((uint16_t)(r & 0x1F) << 11) | 
           ((uint16_t)(g & 0x1F) << 6) |  /* Note: using 5-bit green, shifted to 6-bit position */
           ((uint16_t)(b & 0x1F));
}

uint8_t colorset_get_red(uint16_t color) {
    return (color >> 11) & 0x1F;
}

uint8_t colorset_get_green(uint16_t color) {
    /* Extract 6-bit green and scale to 5-bit for consistency */
    return (color >> 6) & 0x1F;
}

uint8_t colorset_get_blue(uint16_t color) {
    return color & 0x1F;
}

/* ============================================================================
 * Internal Helper Functions
 * ============================================================================ */

/**
 * Generate gradient colors between two RGB points
 * 
 * @param pColor Output array for colors
 * @param step Number of steps in gradient
 * @param r0,g0,b0 Start color (5-bit components)
 * @param r1,g1,b1 End color (5-bit components)
 */
static void get_index_color(uint16_t* pColor, int step,
                           int r0, int g0, int b0,
                           int r1, int g1, int b1) {
    float r = (float)r0;
    float g = (float)g0;
    float b = (float)b0;
    
    float step_1 = (float)step - 1.0f;
    float sr = (r1 - r0) / step_1;
    float sg = (g1 - g0) / step_1;
    float sb = (b1 - b0) / step_1;
    
    for (int i = 0; i < step; i++) {
        uint8_t red   = (uint8_t)r;
        uint8_t green = (uint8_t)g;
        uint8_t blue  = (uint8_t)b;
        
        *pColor++ = colorset_make_color(red, green, blue);
        
        r += sr;
        g += sg;
        b += sb;
    }
}

/* ============================================================================
 * Public Functions
 * ============================================================================ */

void colorset_init(void) {
    int i, j, k, set;
    uint16_t color;
    
    /* 33 seed colors (RGB 5-bit components) */
    static const uint8_t rgbPoint[MAX_COLORSET_SEED][3] = {
        { 0, 0, 31 },       /* Blue */
        { 0, 31, 0 },       /* Green */
        { 31, 0, 0 },       /* Red */
        { 0, 31, 31 },      /* Cyan */
        { 31, 0, 31 },      /* Magenta */
        { 31, 31, 0 },      /* Yellow */
        
        { 0, 0, 16 },       /* Dark Blue */
        { 0, 16, 0 },       /* Dark Green */
        { 16, 0, 0 },       /* Dark Red */
        { 0, 16, 16 },      /* Dark Cyan */
        { 16, 0, 16 },      /* Dark Magenta */
        { 16, 16, 0 },      /* Dark Yellow */
        
        { 16, 31, 0 },      /* Lime */
        { 16, 0, 31 },      /* Purple */
        { 31, 16, 0 },      /* Orange */
        { 0, 16, 31 },      /* Sky Blue */
        { 31, 0, 16 },      /* Pink */
        { 0, 31, 16 },      /* Spring Green */
        
        { 16, 31, 16 },     /* Light Green */
        { 16, 16, 31 },     /* Light Blue */
        { 31, 16, 16 },     /* Light Red */
        
        { 16, 31, 31 },     /* Light Cyan */
        { 31, 16, 31 },     /* Light Magenta */
        { 31, 31, 16 },     /* Light Yellow */
        
        { 16, 16, 16 },     /* Gray */
        { 24, 24, 24 },     /* Light Gray */
        { 8, 8, 8 },        /* Dark Gray */
        
        { 30, 24, 18 },     /* Skin tone 1 */
        { 25, 15, 11 },     /* Skin tone 2 */
        { 21, 12, 11 },     /* Skin tone 3 */
        { 19, 15, 13 },     /* Skin tone 4 */
        
        { 21, 18, 11 },     /* Brown 1 */
        { 22, 16, 9 }       /* Brown 2 */
    };
    
    /* Initialize index values to 0 */
    memset(g_IndexValue, 0, sizeof(g_IndexValue));
    
    /* Generate ColorSet table */
    set = 0;
    
    for (i = 0; i < MAX_COLORSET_SEED; i++) {
        int r = rgbPoint[i][0];
        int g = rgbPoint[i][1];
        int b = rgbPoint[i][2];
        
        /* Generate MAX_COLORSET_SEED_MODIFY variations per seed */
        for (j = MAX_COLORGRADATION_HALF; j >= 1; j--) {
            /* First row: gradient from white to seed color */
            if (j == MAX_COLORGRADATION_HALF) {
                get_index_color(g_ColorSet[set], j,
                               31, 31, 31,  /* White */
                               r, g, b);    /* Seed color */
            } else {
                /* Use color from base set for smoother gradients */
                color = g_ColorSet[i * MAX_COLORSET_SEED_MODIFY][MAX_COLORGRADATION_HALF - j];
                int r0 = colorset_get_red(color);
                int g0 = colorset_get_green(color);
                int b0 = colorset_get_blue(color);
                
                get_index_color(g_ColorSet[set], j,
                               r0, g0, b0,
                               r, g, b);
            }
            
            /* Second half: gradient from seed color to black */
            get_index_color(g_ColorSet[set] + j, MAX_COLORGRADATION - j,
                           r, g, b,
                           0, 0, 0);  /* Black */
            
            set++;
        }
    }
    
    /* Generate GradationValue table */
    for (j = 0; j < MAX_COLORGRADATION; j++) {
        color = g_ColorSet[0][j];
        g_GradationValue[j] = colorset_get_red(color) + 
                              colorset_get_green(color) + 
                              colorset_get_blue(color);
    }
    
    /* Generate Darkness tables */
    for (i = 0; i < MAX_COLORSET; i++) {
        for (j = 0; j < MAX_COLORGRADATION; j++) {
            color = g_ColorSet[i][j];
            
            for (k = 0; k < MAX_DARKBIT; k++) {
                int dr = (colorset_get_red(color) >> k);
                int dg = (colorset_get_green(color) >> k);
                int db = (colorset_get_blue(color) >> k);
                g_ColorSetDarkness[k][i][j] = colorset_make_color(dr, dg, db);
            }
        }
    }
    
    /* Generate ColorToGradation table */
    for (uint8_t cg = 0; cg < MAX_COLOR_TO_GRADATION; cg++) {
        g_ColorToGradation[cg] = colorset_color_to_gradation(cg);
    }
}

void colorset_set_using(int set1, int set2) {
    g_IndexValue[0] = set1;
    g_IndexValue[1] = set2;
}

void colorset_set_using_only(uint8_t setNumber, int colorSet) {
    g_IndexValue[setNumber] = colorSet;
}

int colorset_get_using(uint8_t n) {
    return g_IndexValue[n];
}

uint8_t colorset_get_index_color(uint16_t color) {
    /* Calculate gradation from R+G+B sum */
    uint16_t spriteGradation = colorset_get_red(color) + 
                               colorset_get_green(color) + 
                               colorset_get_blue(color);
    
    if (spriteGradation >= MAX_COLOR_TO_GRADATION) {
        spriteGradation = MAX_COLOR_TO_GRADATION - 1;
    }
    
    return g_ColorToGradation[spriteGradation];
}

uint8_t colorset_color_to_gradation(uint8_t spriteGradation) {
    int g;
    
    /* Find the closest gradation value */
    for (g = 0; g < MAX_COLORGRADATION; g++) {
        if (spriteGradation > g_GradationValue[g]) {
            break;
        }
    }
    
    /* Handle edge cases */
    if (g == 0 || g == MAX_COLORGRADATION - 1) {
        return (uint8_t)g;
    }
    
    /* Find closer match */
    uint16_t value1 = g_GradationValue[g - 1] - spriteGradation;
    uint16_t value2 = spriteGradation - g_GradationValue[g];
    
    if (value1 < value2) {
        return (uint8_t)(g - 1);
    } else if (value1 > value2) {
        return (uint8_t)g;
    }
    
    return (uint8_t)(g - 1);
}

int colorset_load_from_file(FILE* file) {
    int i, j, k;
    int cg, cs, db;
    
    if (!file) {
        return 0;
    }
    
    /* Read header values */
    if (fread(&cg, 4, 1, file) != 1) return 0;
    if (fread(&cs, 4, 1, file) != 1) return 0;
    if (fread(&db, 4, 1, file) != 1) return 0;
    
    /* Validate header (allow mismatch for compatibility) */
    /* Original code doesn't fail on mismatch */
    
    /* Read ColorSet table */
    for (i = 0; i < MAX_COLORSET; i++) {
        for (j = 0; j < MAX_COLORGRADATION; j++) {
            if (fread(&g_ColorSet[i][j], 2, 1, file) != 1) return 0;
        }
    }
    
    /* Read GradationValue table (note: original reads MAX_COLORSET entries) */
    for (i = 0; i < MAX_COLORSET; i++) {
        uint16_t dummy;
        if (fread(&dummy, 2, 1, file) != 1) return 0;
        if (i < MAX_COLORGRADATION) {
            g_GradationValue[i] = dummy;
        }
    }
    
    /* Read ColorSetDarkness table */
    for (k = 0; k < MAX_DARKBIT; k++) {
        for (i = 0; i < MAX_COLORSET; i++) {
            for (j = 0; j < MAX_COLORGRADATION; j++) {
                if (fread(&g_ColorSetDarkness[k][i][j], 2, 1, file) != 1) return 0;
            }
        }
    }
    
    return 1;
}

int colorset_save_to_file(FILE* file) {
    int i, j, k;
    
    if (!file) {
        return 0;
    }
    
    int cg = MAX_COLORGRADATION;
    int cs = MAX_COLORSET;
    int db = MAX_DARKBIT;
    
    /* Write header values */
    if (fwrite(&cg, 4, 1, file) != 1) return 0;
    if (fwrite(&cs, 4, 1, file) != 1) return 0;
    if (fwrite(&db, 4, 1, file) != 1) return 0;
    
    /* Write ColorSet table */
    for (i = 0; i < MAX_COLORSET; i++) {
        for (j = 0; j < MAX_COLORGRADATION; j++) {
            if (fwrite(&g_ColorSet[i][j], 2, 1, file) != 1) return 0;
        }
    }
    
    /* Write GradationValue table (note: original writes MAX_COLORSET entries) */
    for (i = 0; i < MAX_COLORSET; i++) {
        uint16_t value = (i < MAX_COLORGRADATION) ? g_GradationValue[i] : 0;
        if (fwrite(&value, 2, 1, file) != 1) return 0;
    }
    
    /* Write ColorSetDarkness table */
    for (k = 0; k < MAX_DARKBIT; k++) {
        for (i = 0; i < MAX_COLORSET; i++) {
            for (j = 0; j < MAX_COLORGRADATION; j++) {
                if (fwrite(&g_ColorSetDarkness[k][i][j], 2, 1, file) != 1) return 0;
            }
        }
    }
    
    return 1;
}
