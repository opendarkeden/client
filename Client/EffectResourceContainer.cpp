//----------------------------------------------------------------------
// EffectResourceContainer.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "EffectResourceContainer.h"
#include <iostream>
#include <stdio.h>

//----------------------------------------------------------------------
// Constructor/Destructor
//----------------------------------------------------------------------
EffectResourceContainer::EffectResourceContainer()
	: m_loaded(false)
{
}

EffectResourceContainer::~EffectResourceContainer()
{
	// Resources will be automatically released (RAII)
}

//----------------------------------------------------------------------
// Load - Load all Effect resources
//----------------------------------------------------------------------
bool EffectResourceContainer::Load(const char* basePath)
{
	if (m_loaded) {
		std::cerr << "[EffectResourceContainer] Already loaded" << std::endl;
		return true;
	}

	std::cout << "[EffectResourceContainer] Loading resources from: " << basePath << std::endl;

	// 1. Load Frame Pack
	if (!LoadFramePacks(basePath)) {
		std::cerr << "[EffectResourceContainer] Failed to load frame packs" << std::endl;
		return false;
	}

	// 2. Load Sprite Pack
	if (!LoadSpritePacks(basePath)) {
		std::cerr << "[EffectResourceContainer] Failed to load sprite packs" << std::endl;
		return false;
	}

	// 3. Load Palette Pack
	if (!LoadPalettePacks(basePath)) {
		std::cerr << "[EffectResourceContainer] Failed to load palette packs" << std::endl;
		return false;
	}

	m_loaded = true;
	std::cout << "[EffectResourceContainer] All resources loaded successfully" << std::endl;
	return true;
}

//----------------------------------------------------------------------
// LoadFramePacks - Load frame animation packs
//----------------------------------------------------------------------
bool EffectResourceContainer::LoadFramePacks(const char* basePath)
{
	char path[256];
	std::ifstream file;

	// Normal Effect Frame Pack
	snprintf(path, sizeof(path), "%sNormalEffect.efpk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectNormalFPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	// Alpha Effect Frame Pack
	// Try multiple filename variants
	bool alphaLoaded = false;
	const char* alphaFilenames[] = {"Effect.efpk", "effect.efpk", "EffectAlpha.efpk"};
	for (int i = 0; i < 3; i++) {
		snprintf(path, sizeof(path), "%s%s", basePath, alphaFilenames[i]);
		file.open(path, std::ios::binary);
		if (file.is_open()) {
			if (m_EffectAlphaFPK.LoadFromFile(file)) {
				std::cout << "  Loaded: " << path << std::endl;
				alphaLoaded = true;
			}
			file.close();
			break;
		}
	}
	if (!alphaLoaded) {
		std::cerr << "  Warning: Failed to open any Alpha Effect FPK" << std::endl;
	}

	// Shadow Effect Frame Pack
	snprintf(path, sizeof(path), "%sShadowEffect.efpk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectShadowFPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	// Screen Effect Frame Pack
	snprintf(path, sizeof(path), "%sEffectScreen.efpk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectScreenFPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	return true;
}

//----------------------------------------------------------------------
// LoadSpritePacks - Load sprite packs
//----------------------------------------------------------------------
bool EffectResourceContainer::LoadSpritePacks(const char* basePath)
{
	char path[256];
	std::ifstream file;

	// Normal Effect Sprite Pack
	snprintf(path, sizeof(path), "%sEffect.spk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectNormalSPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << " (" << m_EffectNormalSPK.GetSize() << " sprites)" << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	// Alpha Effect Sprite Pack
	bool alphaSpkLoaded = false;
	const char* alphaSpkFilenames[] = {"Effect.aspk", "effect.aspk"};
	for (int i = 0; i < 2; i++) {
		snprintf(path, sizeof(path), "%s%s", basePath, alphaSpkFilenames[i]);
		file.open(path, std::ios::binary);
		if (file.is_open()) {
			if (m_EffectAlphaSPK.LoadFromFile(file)) {
				std::cout << "  Loaded: " << path << " (" << m_EffectAlphaSPK.GetSize() << " sprites)" << std::endl;
				alphaSpkLoaded = true;
			}
			file.close();
			break;
		}
	}
	if (!alphaSpkLoaded) {
		std::cerr << "  Warning: Failed to open any Alpha Effect SPK" << std::endl;
	}

	// Shadow Effect Sprite Pack
	snprintf(path, sizeof(path), "%sEffect.sspk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectShadowSPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << " (" << m_EffectShadowSPK.GetSize() << " sprites)" << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	// Screen Effect Sprite Pack
	snprintf(path, sizeof(path), "%sEffectScreen.spk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectScreenSPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << " (" << m_EffectScreenSPK.GetSize() << " sprites)" << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	return true;
}

//----------------------------------------------------------------------
// LoadPalettePacks - Load palette packs
//----------------------------------------------------------------------
bool EffectResourceContainer::LoadPalettePacks(const char* basePath)
{
	char path[256];
	std::ifstream file;

	// Alpha Effect Palette Pack
	bool alphaPpkLoaded = false;
	const char* alphaPpkFilenames[] = {"Effect.ppk", "effect.ppk", "EffectAlpha.ppk"};
	for (int i = 0; i < 3; i++) {
		snprintf(path, sizeof(path), "%s%s", basePath, alphaPpkFilenames[i]);
		file.open(path, std::ios::binary);
		if (file.is_open()) {
			if (m_EffectAlphaPPK.LoadFromFile(file)) {
				std::cout << "  Loaded: " << path << std::endl;
				alphaPpkLoaded = true;
			}
			file.close();
			break;
		}
	}
	if (!alphaPpkLoaded) {
		std::cerr << "  Warning: Failed to open any Alpha Effect PPK" << std::endl;
	}

	// Screen Effect Palette Pack
	snprintf(path, sizeof(path), "%sEffectScreen.ppk", basePath);
	file.open(path, std::ios::binary);
	if (file.is_open()) {
		if (m_EffectScreenPPK.LoadFromFile(file)) {
			std::cout << "  Loaded: " << path << std::endl;
		}
		file.close();
	} else {
		std::cerr << "  Warning: Failed to open " << path << std::endl;
	}

	return true;
}

//----------------------------------------------------------------------
// GetFramePack - Get corresponding Frame Pack
//----------------------------------------------------------------------
CEffectFramePack* EffectResourceContainer::GetFramePack(BLT_TYPE bltType)
{
	switch (bltType) {
		case BLT_NORMAL:
			return &m_EffectNormalFPK;
		case BLT_EFFECT:
			return &m_EffectAlphaFPK;
		case BLT_SHADOW:
			return &m_EffectShadowFPK;
		case BLT_SCREEN:
			return &m_EffectScreenFPK;
		default:
			std::cerr << "[EffectResourceContainer] Invalid BltType: " << bltType << std::endl;
			return nullptr;
	}
}

//----------------------------------------------------------------------
// GetEffectFrame - Safely get Effect frame (with boundary checking)
//----------------------------------------------------------------------
CEffectFrame* EffectResourceContainer::GetEffectFrame(BLT_TYPE bltType, TYPE_FRAMEID type,
													   BYTE direction, BYTE frame)
{
	CEffectFramePack* pack = GetFramePack(bltType);
	if (!pack) {
		return nullptr;
	}

	// Boundary check
	if (type >= pack->GetSize()) {
		// Silent failure to avoid log flooding
		return nullptr;
	}

	// Get direction array
	auto& directionArray = (*pack)[type];
	if (direction >= directionArray.GetSize()) {
		return nullptr;
	}

	// Get frame array
	auto& frameArray = directionArray[direction];
	if (frame >= frameArray.GetSize()) {
		return nullptr;
	}

	return &frameArray[frame];
}

//----------------------------------------------------------------------
// GetSprite - Safely get Sprite (with boundary checking)
// Note: BLT_SCREEN returns CSpritePal*, others return CSprite*
//----------------------------------------------------------------------
void* EffectResourceContainer::GetSprite(BLT_TYPE bltType, int spriteID)
{
	switch (bltType) {
		case BLT_NORMAL:
			if (spriteID >= 0 && spriteID < m_EffectNormalSPK.GetSize()) {
				return (void*)&m_EffectNormalSPK[spriteID];
			}
			break;

		case BLT_SHADOW:
			if (spriteID >= 0 && spriteID < m_EffectShadowSPK.GetSize()) {
				return (void*)GetShadowSprite(spriteID);
			}
			break;

		case BLT_SCREEN:
			if (spriteID >= 0 && spriteID < m_EffectScreenSPK.GetSize()) {
				return (void*)&m_EffectScreenSPK[spriteID];
			}
			break;

		case BLT_EFFECT:
			// BLT_EFFECT uses Alpha Sprite, need to use GetAlphaSprite
			std::cerr << "[EffectResourceContainer] BLT_EFFECT should use GetAlphaSprite" << std::endl;
			return nullptr;

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------
// GetAlphaSprite - Get Alpha Sprite (for BLT_EFFECT)
//----------------------------------------------------------------------
CAlphaSpritePal* EffectResourceContainer::GetAlphaSprite(int spriteID)
{
	if (spriteID >= 0 && spriteID < m_EffectAlphaSPK.GetSize()) {
		return &m_EffectAlphaSPK[spriteID];
	}
	return nullptr;
}

//----------------------------------------------------------------------
// GetShadowSprite - Get Shadow Sprite (for BLT_SHADOW)
//----------------------------------------------------------------------
CShadowSprite* EffectResourceContainer::GetShadowSprite(int spriteID)
{
	if (spriteID >= 0 && spriteID < m_EffectShadowSPK.GetSize()) {
		return &m_EffectShadowSPK[spriteID];
	}
	return nullptr;
}

//----------------------------------------------------------------------
// GetPalette - Safely get Palette (with boundary checking)
// Returns single MPalette pointer
//----------------------------------------------------------------------
MPalette* EffectResourceContainer::GetPalette(BLT_TYPE bltType, TYPE_FRAMEID type)
{
	switch (bltType) {
		case BLT_EFFECT:
			if (type >= 0 && type < m_EffectAlphaPPK.GetSize()) {
				return &m_EffectAlphaPPK[type];
			}
			break;

		case BLT_SCREEN:
			if (type >= 0 && type < m_EffectScreenPPK.GetSize()) {
				return &m_EffectScreenPPK[type];
			}
			break;

		default:
			// BLT_NORMAL and BLT_SHADOW don't need Palette
			return nullptr;
	}

	return nullptr;
}
