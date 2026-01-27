//----------------------------------------------------------------------
// EffectSpriteTypeTable.cpp
//----------------------------------------------------------------------
#include "EffectSpriteTypeTable.h"
#include <iostream>
#include <fstream>
#include <cstring>

typedef uint8_t BYTE;
typedef uint16_t WORD;

EffectSpriteTypeTable::EffectSpriteTypeTable()
: m_loaded(false), m_tableSize(0)
{
}

EffectSpriteTypeTable::~EffectSpriteTypeTable()
{
}

bool EffectSpriteTypeTable::LoadFromFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open EffectSpriteType table: " << filename << std::endl;
        return false;
    }

    // Read number of entries (first 4 bytes - int32_t)
    int32_t count;
    file.read((char*)&count, 4);
    if (!file.good()) {
        std::cerr << "Failed to read entry count from EffectSpriteType table" << std::endl;
        return false;
    }

    std::cout << "Loading EffectSpriteType table: " << count << " entries" << std::endl;

    // Store table size for validation
    m_tableSize = count;

    // Read each entry
    // Structure per entry:
    // BYTE BltType (1 byte)
    // WORD FrameID (2 bytes) - this is the EffectID
    // BYTE flag (1 byte)
    // WORD ActionEffectFrameID (2 bytes)
    // WORD FemaleEffectSpriteType (2 bytes)
    // BYTE numPair (1 byte) - number of pair frames
    // [numPair * WORD] Pair FrameIDs (2 bytes each)
    // Total: 9 bytes minimum + (numPair * 2) bytes

    int loaded = 0;
    for (int i = 0; i < count; i++) {
        BYTE bltType;
        WORD frameID;
        BYTE flag;
        WORD actionEffectFrameID;
        WORD femaleEffectSpriteType;
        BYTE numPair;

        file.read((char*)&bltType, 1);
        file.read((char*)&frameID, 2);
        file.read((char*)&flag, 1);
        file.read((char*)&actionEffectFrameID, 2);
        file.read((char*)&femaleEffectSpriteType, 2);
        file.read((char*)&numPair, 1);

        if (!file.good()) {
            std::cerr << "Failed to read entry " << i << " from EffectSpriteType table" << std::endl;
            break;
        }

        // Skip pair frame IDs (we don't need them for BltType mapping)
        if (numPair > 0) {
            // Each pair frame is 2 bytes
            int skipBytes = numPair * 2;
            file.seekg(skipBytes, std::ios_base::cur);
            if (!file.good()) {
                std::cerr << "Failed to skip pair frames for entry " << i << std::endl;
                break;
            }
        }

        // Store mapping: table index -> BltType
        // Use the table index (i) as key, NOT frameID!
        // Because multiple table entries can have the same frameID
        m_table[i] = (int)bltType;
        loaded++;

        // Debug: Print first few entries
        if (i < 10) {
            std::cout << "  Entry " << i << ": type=" << i
                      << ", FrameID=" << frameID
                      << ", BltType=" << (int)bltType
                      << " (numPair=" << (int)numPair << ")" << std::endl;
        }
    }

    file.close();

    if (loaded == count) {
        m_loaded = true;
        std::cout << "Successfully loaded " << loaded << " EffectSpriteType entries" << std::endl;
        return true;
    } else {
        std::cerr << "Warning: Only loaded " << loaded << " of " << count << " entries" << std::endl;
        m_loaded = true;
        return true;
    }
}

int EffectSpriteTypeTable::GetBltType(TYPE_FRAMEID effectID) const
{
    auto it = m_table.find(effectID);
    if (it != m_table.end()) {
        int bltType = it->second;
        // Validate BltType is in range 0-3
        if (bltType >= 0 && bltType <= 3) {
            return bltType;
        } else {
            // Invalid BltType, use default
            if (bltType != 1) {  // Only warn once for non-default values
                std::cout << "  Warning: EffectID=" << effectID
                          << " has invalid BltType=" << bltType
                          << ", using BLT_EFFECT (1)" << std::endl;
            }
            return 1;  // BLT_EFFECT
        }
    }

    // Default to BLT_EFFECT (1) if not found
    return 1;
}

//----------------------------------------------------------------------
// GetEffectSpriteTypeIndex - Emulate game's GET_EFFECTSPRITETYPE_xxxxx macros
// This computes the table index for a given frameID and bltType
// Matches the logic in MTopViewDraw.inl and MTopView.cpp:20124
//----------------------------------------------------------------------
int EffectSpriteTypeTable::GetEffectSpriteTypeIndex(TYPE_FRAMEID frameID, int bltType)
{
    switch (bltType) {
        case 0:  // BLT_NORMAL
            return frameID + MAX_EFFECTSPRITETYPE_SCREENEFFECT;

        case 1:  // BLT_EFFECT (ALPHA)
            return frameID;

        case 2:  // BLT_SHADOW
            return frameID + MAX_EFFECTSPRITETYPE_NORMALEFFECT;

        case 3:  // BLT_SCREEN
            return frameID + MAX_EFFECTSPRITETYPE_ALPHAEFFECT;

        default:
            std::cerr << "Warning: Unknown BltType=" << bltType << ", treating as BLT_EFFECT" << std::endl;
            return frameID;
    }
}

//----------------------------------------------------------------------
// IsValidIndex - Check if an index is within table bounds
//----------------------------------------------------------------------
bool EffectSpriteTypeTable::IsValidIndex(int index) const
{
    return index >= 0 && index < (int)m_table.size();
}

//----------------------------------------------------------------------
// DebugAccess - Print debug information about table access
// This helps correlate game panics with effect_viewer behavior
//----------------------------------------------------------------------
void EffectSpriteTypeTable::DebugAccess(int index, const char* location) const
{
    std::cout << "=== EffectSpriteType Table Access ===" << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Table Index: " << index << std::endl;
    std::cout << "Table Size: " << m_table.size() << std::endl;

    if (index < 0) {
        std::cout << "  ❌ ERROR: Negative index!" << std::endl;
    } else if (index >= (int)m_table.size()) {
        std::cout << "  ❌ ERROR: Index out of bounds! Offset=" << (index - m_table.size() + 1) << std::endl;
        std::cout << "  This would cause heap-buffer-overflow in the game!" << std::endl;
    } else {
        std::cout << "  ✅ OK: Index is valid" << std::endl;
    }

    // Show which section this index falls into
    if (index < MAX_EFFECTSPRITETYPE_ALPHAEFFECT) {
        std::cout << "  Section: BLT_EFFECT (ALPHA) [0-" << (MAX_EFFECTSPRITETYPE_ALPHAEFFECT-1) << "]" << std::endl;
    } else if (index < MAX_EFFECTSPRITETYPE_SCREENEFFECT) {
        std::cout << "  Section: BLT_SCREEN [" << MAX_EFFECTSPRITETYPE_ALPHAEFFECT
                  << "-" << (MAX_EFFECTSPRITETYPE_SCREENEFFECT-1) << "]" << std::endl;
    } else if (index < MAX_EFFECTSPRITETYPE_NORMALEFFECT) {
        std::cout << "  Section: BLT_NORMAL [" << MAX_EFFECTSPRITETYPE_SCREENEFFECT
                  << "-" << (MAX_EFFECTSPRITETYPE_NORMALEFFECT-1) << "]" << std::endl;
    } else {
        std::cout << "  Section: BLT_SHADOW [" << MAX_EFFECTSPRITETYPE_NORMALEFFECT
                  << "-MAX]" << std::endl;
    }

    std::cout << "=====================================" << std::endl;
}

//----------------------------------------------------------------------
// GetMaxEffectID - Get the highest EffectID in the table
//----------------------------------------------------------------------
TYPE_FRAMEID EffectSpriteTypeTable::GetMaxEffectID() const
{
    TYPE_FRAMEID maxID = 0;
    for (auto it = m_table.begin(); it != m_table.end(); ++it) {
        if (it->first > maxID) {
            maxID = it->first;
        }
    }
    return maxID;
}
