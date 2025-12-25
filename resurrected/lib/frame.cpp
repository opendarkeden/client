/**
 * @file frame.cpp
 * @brief CFrame class implementation
 * 
 * Requirements: 3.4, 3.5
 */

#include "frame.h"
#include <string.h>

/* ============================================================================
 * CFrame Implementation
 * ============================================================================ */

CFrame::CFrame(TYPE_SPRITEID spriteID, short cx, short cy)
{
    Set(spriteID, cx, cy);
}

void CFrame::Set(TYPE_SPRITEID spriteID, short cx, short cy)
{
    m_SpriteID = spriteID;
    m_cX = cx;
    m_cY = cy;
}

void CFrame::operator=(const CFrame& frame)
{
    m_SpriteID = frame.m_SpriteID;
    m_cX = frame.m_cX;
    m_cY = frame.m_cY;
}

bool CFrame::SaveToFile(FILE* file)
{
    if (file == NULL) return false;
    
    /* Write SpriteID (2 bytes) */
    if (fwrite(&m_SpriteID, sizeof(TYPE_SPRITEID), 1, file) != 1) {
        return false;
    }
    
    /* Write cX (2 bytes) */
    if (fwrite(&m_cX, sizeof(short), 1, file) != 1) {
        return false;
    }
    
    /* Write cY (2 bytes) */
    if (fwrite(&m_cY, sizeof(short), 1, file) != 1) {
        return false;
    }
    
    return true;
}

bool CFrame::LoadFromFile(FILE* file)
{
    if (file == NULL) return false;
    
    /* Read SpriteID (2 bytes) */
    if (fread(&m_SpriteID, sizeof(TYPE_SPRITEID), 1, file) != 1) {
        return false;
    }
    
    /* Read cX (2 bytes) */
    if (fread(&m_cX, sizeof(short), 1, file) != 1) {
        return false;
    }
    
    /* Read cY (2 bytes) */
    if (fread(&m_cY, sizeof(short), 1, file) != 1) {
        return false;
    }
    
    return true;
}
