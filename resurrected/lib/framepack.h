/**
 * @file framepack.h
 * @brief CFramePack template and CCreatureFramePack class
 * 
 * Requirements: 3.1, 3.2, 3.3
 * 
 * CFramePack is a template class that extends TArray to provide
 * file I/O for frame pack data. It supports both pack files and
 * index files for efficient random access.
 * 
 * CCreatureFramePack stores animation frame data for all creature types:
 * [CreatureType][Action][Direction][Frame] -> CFrame
 */

#ifndef FRAMEPACK_H
#define FRAMEPACK_H

#include <stdio.h>
#include <stdint.h>
#include "tarray.h"
#include "frame.h"

/**
 * CFramePack template class
 * 
 * Extends TArray to provide specialized file I/O for frame packs.
 * The Type parameter should be one of:
 * - FRAME_ARRAY (for ThingFramePack)
 * - DIRECTION_FRAME_ARRAY (for EffectFramePack)
 * - ACTION_FRAME_ARRAY (for CreatureFramePack)
 * 
 * @tparam Type The array type stored in the pack
 */
template <typename Type>
class CFramePack : public TArray<Type, TYPE_FRAMEID> {
public:
    /**
     * Constructor
     */
    CFramePack();
    
    /**
     * Destructor
     */
    ~CFramePack();
    
    /**
     * Save to pack file and index file
     * 
     * The pack file contains the actual frame data.
     * The index file contains file offsets for each entry.
     * 
     * @param packFile FILE pointer to pack file
     * @param indexFile FILE pointer to index file
     * @return true on success, false on failure
     */
    bool SaveToFile(FILE* packFile, FILE* indexFile);
    
    /**
     * Load from file (using TArray's LoadFromFile)
     * 
     * @param file FILE pointer to read from
     * @return true on success, false on failure
     */
    bool LoadFromFile(FILE* file);
    
    /**
     * Load from file by filename
     * 
     * @param filename Path to the pack file
     * @return true on success, false on failure
     */
    bool LoadFromFile(const char* filename);

protected:
    /* Use parent class members */
    using TArray<Type, TYPE_FRAMEID>::m_Size;
    using TArray<Type, TYPE_FRAMEID>::m_pData;
};

/* ============================================================================
 * CFramePack Implementation
 * ============================================================================ */

template <typename Type>
CFramePack<Type>::CFramePack()
{
    /* Parent constructor handles initialization */
}

template <typename Type>
CFramePack<Type>::~CFramePack()
{
    /* Parent destructor handles cleanup */
}

template <typename Type>
bool CFramePack<Type>::SaveToFile(FILE* packFile, FILE* indexFile)
{
    if (packFile == NULL || indexFile == NULL) {
        return false;
    }
    
    /* Write size to both files */
    if (fwrite(&m_Size, sizeof(TYPE_FRAMEID), 1, packFile) != 1) {
        return false;
    }
    if (fwrite(&m_Size, sizeof(TYPE_FRAMEID), 1, indexFile) != 1) {
        return false;
    }
    
    /* Nothing to write if empty */
    if (m_pData == NULL || m_Size == 0) {
        return false;
    }
    
    /* Allocate index array */
    long* pIndex = new long[m_Size];
    if (pIndex == NULL) {
        return false;
    }
    
    /* Write each element and record its file position */
    for (TYPE_FRAMEID i = 0; i < m_Size; i++) {
        /* Record current file position as index */
        pIndex[i] = ftell(packFile);
        
        /* Write element data */
        if (!m_pData[i].SaveToFile(packFile)) {
            delete[] pIndex;
            return false;
        }
    }
    
    /* Write index data */
    for (TYPE_FRAMEID i = 0; i < m_Size; i++) {
        if (fwrite(&pIndex[i], sizeof(long), 1, indexFile) != 1) {
            delete[] pIndex;
            return false;
        }
    }
    
    delete[] pIndex;
    return true;
}

template <typename Type>
bool CFramePack<Type>::LoadFromFile(FILE* file)
{
    /* Use parent class LoadFromFile */
    return TArray<Type, TYPE_FRAMEID>::LoadFromFile(file);
}

template <typename Type>
bool CFramePack<Type>::LoadFromFile(const char* filename)
{
    if (filename == NULL) {
        return false;
    }
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return false;
    }
    
    bool result = LoadFromFile(file);
    fclose(file);
    
    return result;
}

/* ============================================================================
 * FramePack Type Definitions - Compatible with original code
 * ============================================================================ */

/**
 * CImageFramePack: Single frame per entry
 * Used for simple image references
 */
typedef CFramePack<CFrame> CImageFramePack;

/**
 * CAnimationFramePack: Array of frames per entry
 * Used for simple animations without direction
 */
typedef CFramePack<FRAME_ARRAY> CAnimationFramePack;

/**
 * CDirectionFramePack: Direction-based frame arrays
 * Used for directional animations
 */
typedef CFramePack<DIRECTION_FRAME_ARRAY> CDirectionFramePack;

/**
 * CCreatureFramePack class
 * 
 * Stores animation frame data for all creature types.
 * Structure: [CreatureType][Action][Direction][Frame] -> CFrame
 * 
 * Each creature type has an ACTION_FRAME_ARRAY containing:
 * - Multiple actions (STAND, WALK, RUN, ATTACK, etc.)
 * - Each action has 8 directions (0-7)
 * - Each direction has multiple frames
 * - Each frame contains SpriteID and offset (cX, cY)
 */
class CCreatureFramePack : public CFramePack<ACTION_FRAME_ARRAY> {
public:
    /**
     * Constructor
     */
    CCreatureFramePack();
    
    /**
     * Destructor
     */
    ~CCreatureFramePack();
    
    /**
     * Output frame pack information to a text file (for debugging)
     * 
     * Writes all frame data in human-readable format:
     * [type][action][dir][frame] = (spriteID, cX, cY)
     * 
     * @param filename Path to output file
     */
    void InfoToFile(const char* filename);
};

#endif /* FRAMEPACK_H */
