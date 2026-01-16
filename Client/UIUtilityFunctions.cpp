//----------------------------------------------------------------------
// UIUtilityFunctions.cpp - macOS UI utility functions
//----------------------------------------------------------------------
#include "../Client_PCH.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//----------------------------------------------------------------------
// String Reduction Functions
//----------------------------------------------------------------------

// Reduce string to fit width (basic version)
void ReduceString(char* pStr, int maxWidth)
{
    if (pStr == NULL || maxWidth <= 0)
        return;

    int len = strlen(pStr);
    if (len <= maxWidth)
        return;

    // Truncate and add "..."
    pStr[maxWidth - 3] = '.';
    pStr[maxWidth - 2] = '.';
    pStr[maxWidth - 1] = '.';
    pStr[maxWidth] = '\0';
}

// Reduce string variant 2 (for DBCS strings)
void ReduceString2(char* pStr, int maxWidth)
{
    if (pStr == NULL || maxWidth <= 0)
        return;

    int len = 0;
    char* p = pStr;

    // Count characters (considering DBCS)
    while (*p && len < maxWidth)
    {
        if ((*p & 0x80) != 0)
        {
            // Double-byte character
            p += 2;
            len += 2;
        }
        else
        {
            p++;
            len++;
        }
    }

    if (len > maxWidth - 3)
    {
        pStr[maxWidth - 3] = '.';
        pStr[maxWidth - 2] = '.';
        pStr[maxWidth - 1] = '.';
        pStr[maxWidth] = '\0';
    }
}

// Reduce string variant 3 (with ellipsis in middle)
void ReduceString3(char* pStr, int maxWidth)
{
    if (pStr == NULL || maxWidth <= 0)
        return;

    int len = strlen(pStr);
    if (len <= maxWidth)
        return;

    // Truncate from end and add "..."
    int cutLen = maxWidth - 3;
    if (cutLen > 0)
    {
        pStr[cutLen] = '.';
        pStr[cutLen + 1] = '.';
        pStr[cutLen + 2] = '.';
        pStr[cutLen + 3] = '\0';
    }
}

//----------------------------------------------------------------------
// Face Style
//----------------------------------------------------------------------
int UI_GetFaceStyle(bool bMale, int index)
{
    // Stub: Would return appropriate face style index
    if (bMale)
    {
        return index % 5; // Assume 5 male face styles
    }
    else
    {
        return (index % 5) + 10; // Assume 5 female face styles starting at 10
    }
}

//----------------------------------------------------------------------
// Input Update Functions
//----------------------------------------------------------------------
void UpdateInput()
{
    // Stub: SDL input system handles this
}

void UpdateMouse()
{
    // Stub: SDL input system handles this
}

//----------------------------------------------------------------------
// File I/O Helper
//----------------------------------------------------------------------
bool FileOpenBinary(const char* pFilename, std::ifstream& file)
{
    if (pFilename == NULL)
        return false;

    file.open(pFilename, std::ios::binary);
    return file.is_open();
}

//----------------------------------------------------------------------
// SPRITE_FILEPOSITION_NODE
//----------------------------------------------------------------------
// Methods already exist elsewhere - commenting out stubs
// bool SPRITE_FILEPOSITION_NODE::LoadFromFile(std::ifstream& file)
// {
//     if (!file.is_open())
//         return false;
//     file.read((char*)&Position, sizeof(POINT));
//     file.read((char*)&Width, sizeof(WORD));
//     file.read((char*)&Height, sizeof(WORD));
//     return file.good();
// }
