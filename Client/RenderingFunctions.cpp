//----------------------------------------------------------------------
// RenderingFunctions.cpp - macOS rendering functions
//----------------------------------------------------------------------
#include "../Client_PCH.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//----------------------------------------------------------------------
// Surface/DC Management
//----------------------------------------------------------------------
static void* g_pFL2Surface = NULL;
static void* g_pFL2DC = NULL;

void* g_FL2_GetDC()
{
    if (g_pFL2DC == NULL)
    {
        // Stub: Would normally get device context
    }
    return g_pFL2DC;
}

void g_FL2_ReleaseDC()
{
    // Stub: Would normally release device context
}

void g_SetFL2Surface(void* pSurface)
{
    g_pFL2Surface = pSurface;
}

//----------------------------------------------------------------------
// OpenGL Initialization
//----------------------------------------------------------------------
bool InitializeGL(int width, int height, int bpp, int fullscreen)
{
    // Stub: SDL backend handles this
    return true;
}

//----------------------------------------------------------------------
// Rectangle Drawing
//----------------------------------------------------------------------
void rectangle(S_SURFACEINFO* pSurfaceInfo, void* pRect, int color)
{
    // Stub: Would draw rectangle outline
}

void FillRect(S_SURFACEINFO* pSurfaceInfo, void* pRect, int color)
{
    // Stub: Would fill rectangle
}

//----------------------------------------------------------------------
// Text Rendering
//----------------------------------------------------------------------

// Basic print function
void g_Print(int x, int y, const char* pStr, void* pInfo)
{
    // Stub: Would render text using SDL_ttf or similar
}

// Colored string print
void g_PrintColorStr(int x, int y, const char* pStr, void* info, unsigned long color)
{
    // Stub: Would render colored text
}

// Colored string with shadow
void g_PrintColorStrShadow(int x, int y, const char* pStr, void* info,
                            unsigned long color, unsigned long shadowColor)
{
    // Stub: Would render text with shadow
}

// Colored string with outline
void g_PrintColorStrOut(int x, int y, const char* pStr, void* info,
                         unsigned long color, unsigned long outColor)
{
    // Stub: Would render text with outline
}

//----------------------------------------------------------------------
// String Measurements
//----------------------------------------------------------------------
int g_GetStringWidth(const char* pStr, void* pFont)
{
    if (pStr == NULL)
        return 0;

    int width = 0;
    const char* p = pStr;

    while (*p)
    {
        if ((*p & 0x80) != 0)
        {
            // Double-byte character (Korean, etc.)
            width += 16;
            p += 2;
        }
        else
        {
            // Single-byte character
            width += 8;
            p++;
        }
    }

    return width;
}

int g_GetStringHeight(const char* pStr, void* pFont)
{
    // Return fixed height for simplicity
    return 16;
}

//----------------------------------------------------------------------
// String Manipulation
//----------------------------------------------------------------------

// Check if string can be cut at width
bool g_PossibleStringCut(const char* pStr, int maxWidth)
{
    int currentWidth = 0;
    const char* p = pStr;

    while (*p)
    {
        if ((*p & 0x80) != 0)
        {
            currentWidth += 16;
            p += 2;
        }
        else
        {
            currentWidth += 8;
            p++;
        }

        if (currentWidth > maxWidth)
            return false;
    }

    return true;
}

// Get byte length (note: "Lenth" is intentional spelling from original code)
int g_GetByteLenth(const unsigned short* pStr, int maxChars)
{
    if (pStr == NULL)
        return 0;

    int count = 0;
    while (pStr[count] != 0 && count < maxChars)
    {
        count++;
    }

    return count * 2; // Each wchar_t is 2 bytes
}

// Convert DBCS to single byte
void g_Convert_DBCS_Ascii2SingleByte(const unsigned short* pSrc, int srcLen,
                                      char*& pDst)
{
    if (pSrc == NULL || pDst == NULL)
        return;

    for (int i = 0; i < srcLen; i++)
    {
        // Convert wide char to single byte (simplified)
        pDst[i] = (char)(pSrc[i] & 0xFF);
    }
}

// Number to string
const char* g_GetNumberString(int number)
{
    static char buffer[32];
    sprintf(buffer, "%d", number);
    return buffer;
}

// Money formatting
const char* g_GetStringByMoney(unsigned long money)
{
    static char buffer[64];

    if (money >= 100000000) // 1亿
    {
        sprintf(buffer, "%d.%02d亿", (int)(money / 100000000),
                (int)((money % 100000000) / 1000000));
    }
    else if (money >= 10000) // 1万
    {
        sprintf(buffer, "%d.%04d万", (int)(money / 10000),
                (int)(money % 10000));
    }
    else
    {
        sprintf(buffer, "%lu", money);
    }

    return buffer;
}

// g_BasicException and g_SetNewHandler are defined in BasicException.cpp and GameHelpers.cpp
