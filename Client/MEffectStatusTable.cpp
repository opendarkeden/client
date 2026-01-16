//----------------------------------------------------------------------
// MEffectStatusTable.h
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffectStatusTable.h"


//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
EFFECTSTATUS_TABLE*		g_pEffectStatusTable = NULL;

//----------------------------------------------------------------------
//
// member funtions
//
//----------------------------------------------------------------------
EFFECTSTATUS_NODE::EFFECTSTATUS_NODE()
{
	bUseEffectSprite	= true;
	bAttachGround		= false;

	EffectSpriteType	= EFFECTSPRITETYPE_NULL;
	EffectColor			= 0xFFFF;
	EffectColorPart		= ADDON_NULL;	// 없거나.. 전체 --;
	
	ActionInfo			= ACTIONINFO_NULL;
	OriginalActionInfo	= ACTIONINFO_NULL;
	SoundID				= SOUNDID_NULL;	
}

//----------------------------------------------------------------------
//
// member funtions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void			
EFFECTSTATUS_NODE::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&bUseEffectSprite, 1);
	file.write((const char*)&bAttachGround, 1);

	file.write((const char*)&EffectSpriteType, SIZE_EFFECTSPRITETYPE);
	file.write((const char*)&EffectColor, 2);

	BYTE part = EffectColorPart;
	file.write((const char*)&part, 1);
	
	file.write((const char*)&ActionInfo, SIZE_ACTIONINFO);
	file.write((const char*)&OriginalActionInfo, SIZE_ACTIONINFO);
	file.write((const char*)&SoundID, sizeof(int) );	
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void			
EFFECTSTATUS_NODE::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&bUseEffectSprite, 1);
	file.read((char*)&bAttachGround, 1);

	file.read((char*)&EffectSpriteType, SIZE_EFFECTSPRITETYPE);	
	file.read((char*)&EffectColor, 2);

	BYTE part;
	file.read((char*)&part, 1);
	EffectColorPart = (ADDON)part;

	file.read((char*)&ActionInfo, SIZE_ACTIONINFO);
	file.read((char*)&OriginalActionInfo, SIZE_ACTIONINFO);
	file.read((char*)&SoundID, sizeof(int) );	
}
