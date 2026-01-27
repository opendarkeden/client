//----------------------------------------------------------------------
// EffectResourceContainer.h
//----------------------------------------------------------------------
// Encapsulates all Effect resources, replaces global access
// Supports dependency injection, reduces dependency on g_pTopView
//
// Purpose:
// 1. Centrally manages all Effect resources (FramePack, SpritePack, PalettePack)
// 2. Provides safe access methods with boundary checking
// 3. Supports dependency injection to MEffect and subclasses
// 4. Facilitates unit testing and isolated environment usage
//----------------------------------------------------------------------

#ifndef	__EFFECTRESOURCECONTAINER_H__
#define	__EFFECTRESOURCECONTAINER_H__

#include "DrawTypeDef.h"
#include "framelib/CFramePack.h"
#include "SpriteLib/CSpritePack.h"
#include "SpriteLib/CAlphaSpritePack.h"
#include "SpriteLib/CShadowSprite.h"  // CShadowSpritePack typedef here
#include "SpriteLib/CSpritePal.h"      // CSpritePalPack typedef here

class EffectResourceContainer {
public:
	// Constructor/Destructor
	EffectResourceContainer();
	~EffectResourceContainer();

	//----------------------------------------------------------------------
	// Resource Loading
	//----------------------------------------------------------------------
	// Load all Effect resources from specified path
	// basePath: resource file directory (e.g., "Data/Image/")
	// Returns: true on success, false on failure
	bool Load(const char* basePath);

	// Check if resources are loaded
	bool IsLoaded() const { return m_loaded; }

	//----------------------------------------------------------------------
	// Safe Access Methods (with boundary checking)
	//----------------------------------------------------------------------
	// Get Effect frame
	// Parameters: bltType - render type, type - effect type ID, direction - direction(0-7), frame - frame number
	// Returns: pointer to frame, nullptr on failure
	CEffectFrame* GetEffectFrame(BLT_TYPE bltType, TYPE_FRAMEID type,
								  BYTE direction, BYTE frame);

	// Get Sprite
	// Parameters: bltType - render type, spriteID - sprite ID
	// Returns: pointer to Sprite (void*, because different BltType returns different types), nullptr on failure
	void* GetSprite(BLT_TYPE bltType, int spriteID);
	CAlphaSpritePal* GetAlphaSprite(int spriteID);
	CShadowSprite* GetShadowSprite(int spriteID);

	// Get Palette (returns single MPalette pointer, not MPalettePack*)
	// Parameters: bltType - render type, type - effect type ID
	// Returns: pointer to Palette, nullptr on failure
	MPalette* GetPalette(BLT_TYPE bltType, TYPE_FRAMEID type);

	//----------------------------------------------------------------------
	// Public Resource Packs (same as in MTopView)
	//----------------------------------------------------------------------
	// Frame Pack - frame animation data
	CEffectFramePack m_EffectAlphaFPK;
	CEffectFramePack m_EffectNormalFPK;
	CEffectFramePack m_EffectShadowFPK;
	CEffectFramePack m_EffectScreenFPK;

	// Sprite Pack - sprite images
	CAlphaSpritePalPack m_EffectAlphaSPK;
	CSpritePack m_EffectNormalSPK;
	CShadowSpritePack m_EffectShadowSPK;
	CSpritePalPack m_EffectScreenSPK;

	// Palette Pack - color palettes
	MPalettePack m_EffectAlphaPPK;
	MPalettePack m_EffectScreenPPK;

private:
	bool m_loaded;

	// Internal helper methods
	CEffectFramePack* GetFramePack(BLT_TYPE bltType);
	bool LoadFramePacks(const char* basePath);
	bool LoadSpritePacks(const char* basePath);
	bool LoadPalettePacks(const char* basePath);
};

#endif	// __EFFECTRESOURCECONTAINER_H__
