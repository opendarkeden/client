//----------------------------------------------------------------------
// EffectSpriteTypeTable.h
// Simple loader for EffectSpriteType.inf table
// Maps EffectID (FrameID) to BltType
//----------------------------------------------------------------------

#ifndef EFFECTSPRITETYPETABLE_H
#define EFFECTSPRITETYPETABLE_H

#include <map>
#include <cstdint>
#include <iostream>

// Type definitions (use game's types)
typedef uint16_t TYPE_FRAMEID;

// Store BltType as int to avoid enum redefinition issues
class EffectSpriteTypeTable {
public:
    EffectSpriteTypeTable();
    ~EffectSpriteTypeTable();

    // Load from EffectSpriteType.inf file
    bool LoadFromFile(const char* filename);

    // Get BltType for a given table index (effect type)
    // Note: The parameter is the table INDEX, NOT the FrameID!
    // Returns 1 (BLT_EFFECT) as default if not found
    int GetBltType(TYPE_FRAMEID tableIndex) const;

    // Check if table is loaded
    bool IsLoaded() const { return m_loaded; }

    // Get table size (returns actual table size from file, not map size)
    int GetSize() const { return m_tableSize; }

    // Game's macro emulation - for reproducing panics
    // These match the original game's GET_EFFECTSPRITETYPE_xxxxx macros
    static int GetEffectSpriteTypeIndex(TYPE_FRAMEID frameID, int bltType);

    // Safe table access with bounds checking
    bool IsValidIndex(int index) const;
    void DebugAccess(int index, const char* location) const;

    // Get table layout constants
    static int GetMaxAlphaEffect() { return MAX_EFFECTSPRITETYPE_ALPHAEFFECT; }
    static int GetMaxScreenEffect() { return MAX_EFFECTSPRITETYPE_SCREENEFFECT; }
    static int GetMaxNormalEffect() { return MAX_EFFECTSPRITETYPE_NORMALEFFECT; }

    // Get highest EffectID in the table
    TYPE_FRAMEID GetMaxEffectID() const;

private:
    std::map<TYPE_FRAMEID, int> m_table;  // EffectID -> BltType mapping
    bool m_loaded;
    int m_tableSize;  // Actual table size from file

    // Table layout constants (from EffectSpriteTypeDef.h)
    // These define the boundaries between different BltType sections in the table
    static const int MAX_EFFECTSPRITETYPE_ALPHAEFFECT = 1947 + 10;  // EFFECTSPRITETYPE_ACID_BALL_1
    static const int MAX_EFFECTSPRITETYPE_SCREENEFFECT = 2563;      // EFFECTSPRITETYPE_FLYING_HOLY_WATER
    static const int MAX_EFFECTSPRITETYPE_NORMALEFFECT = 2683;      // EFFECTSPRITETYPE_SUMMON_HELICOPTER (approx)
};

#endif // EFFECTSPRITETYPETABLE_H
