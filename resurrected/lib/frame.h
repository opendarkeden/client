/**
 * @file frame.h
 * @brief CFrame class compatible with original Dark Eden framelib
 * 
 * Requirements: 3.4, 3.5
 * 
 * CFrame stores a single animation frame's data:
 * - SpriteID: Index into SpritePack
 * - cX, cY: Coordinate offsets for rendering
 */

#ifndef FRAME_H
#define FRAME_H

#include <stdio.h>
#include <stdint.h>
#include "tarray.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Type Definitions - Compatible with original code
 * ============================================================================ */

/**
 * Sprite ID type - identifies a sprite within a SpritePack
 */
typedef uint16_t TYPE_SPRITEID;

/**
 * Frame ID type - identifies a frame within a FramePack
 */
typedef uint16_t TYPE_FRAMEID;

/**
 * Null/invalid sprite ID marker
 */
#define SPRITEID_NULL   0xFFFF

/**
 * Null/invalid frame ID marker
 */
#define FRAMEID_NULL    0xFFFF

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * CFrame class - stores single frame data
 * 
 * Compatible with original Dark Eden CFrame interface.
 * Each frame contains:
 * - m_SpriteID: Index into SpritePack (0-65535)
 * - m_cX, m_cY: Coordinate offsets for rendering position
 */
class CFrame {
public:
    /**
     * Constructor
     * @param spriteID Sprite index (default 0)
     * @param cx X coordinate offset (default 0)
     * @param cy Y coordinate offset (default 0)
     */
    CFrame(TYPE_SPRITEID spriteID = 0, short cx = 0, short cy = 0);
    
    /**
     * Set frame data
     * @param spriteID Sprite index
     * @param cx X coordinate offset
     * @param cy Y coordinate offset
     */
    void Set(TYPE_SPRITEID spriteID, short cx, short cy);
    
    /**
     * Get sprite ID
     * @return Sprite index
     */
    TYPE_SPRITEID GetSpriteID() const { return m_SpriteID; }
    
    /**
     * Get X coordinate offset
     * @return X offset
     */
    short GetCX() const { return m_cX; }
    
    /**
     * Get Y coordinate offset
     * @return Y offset
     */
    short GetCY() const { return m_cY; }
    
    /**
     * Assignment operator
     * @param frame Source frame to copy
     */
    void operator=(const CFrame& frame);
    
    /**
     * Save frame to file
     * @param file FILE pointer to write to
     * @return true on success, false on failure
     */
    bool SaveToFile(FILE* file);
    
    /**
     * Load frame from file
     * @param file FILE pointer to read from
     * @return true on success, false on failure
     */
    bool LoadFromFile(FILE* file);

protected:
    TYPE_SPRITEID m_SpriteID;   /**< Sprite index (0-65535) */
    short m_cX;                  /**< X coordinate offset */
    short m_cY;                  /**< Y coordinate offset */
};

/* ============================================================================
 * Frame Array Type Definitions - Compatible with original code
 * ============================================================================ */

/**
 * FRAME_ARRAY: Array of frames for a single direction
 * TArray<CFrame, WORD> - indexed by frame number
 */
typedef TArray<CFrame, uint16_t> FRAME_ARRAY;

/**
 * DIRECTION_FRAME_ARRAY: Array of frame arrays for all directions
 * TArray<FRAME_ARRAY, BYTE> - indexed by direction (0-7)
 */
typedef TArray<FRAME_ARRAY, uint8_t> DIRECTION_FRAME_ARRAY;

/**
 * ACTION_FRAME_ARRAY: Array of direction frame arrays for all actions
 * TArray<DIRECTION_FRAME_ARRAY, BYTE> - indexed by action type
 */
typedef TArray<DIRECTION_FRAME_ARRAY, uint8_t> ACTION_FRAME_ARRAY;

#endif /* __cplusplus */

#endif /* FRAME_H */
