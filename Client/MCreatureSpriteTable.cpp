//----------------------------------------------------------------------
// MCreatureSpriteTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MCreatureSpriteTable.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
CREATURESPRITE_TABLE*	g_pCreatureSpriteTable = NULL;
// CREATURESPRITE_TABLE*	g_pAddonSpriteTable = NULL;
//CREATURESPRITE_TABLE*	g_pCreatureActionSpriteTable = NULL;

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
CREATURESPRITETABLE_INFO::CREATURESPRITETABLE_INFO()
{
	bLoad = FALSE;
	CreatureType = 0;
}

CREATURESPRITETABLE_INFO::~CREATURESPRITETABLE_INFO()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void			
CREATURESPRITETABLE_INFO::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&FrameID, SIZE_FRAMEID);	
	file.write((const char*)&SpriteFilePosition, 4);
	file.write((const char*)&SpriteShadowFilePosition, 4);
	file.write((const char*)&FirstSpriteID, SIZE_SPRITEID);
	file.write((const char*)&LastSpriteID, SIZE_SPRITEID);
	file.write((const char*)&FirstShadowSpriteID, SIZE_SPRITEID);
	file.write((const char*)&LastShadowSpriteID, SIZE_SPRITEID);
	file.write((const char*)&CreatureType, 1);	
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void			
CREATURESPRITETABLE_INFO::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&FrameID, SIZE_FRAMEID);
	file.read((char*)&SpriteFilePosition, 4);
	file.read((char*)&SpriteShadowFilePosition, 4);
	file.read((char*)&FirstSpriteID, SIZE_SPRITEID);
	file.read((char*)&LastSpriteID, SIZE_SPRITEID);
	file.read((char*)&FirstShadowSpriteID, SIZE_SPRITEID);
	file.read((char*)&LastShadowSpriteID, SIZE_SPRITEID);
	file.read((char*)&CreatureType, 1);
	//add by viva  //McreatureSpriteTable  NewVampire180 CreatureType:0xFC,0xFD
//	if(FrameID==247||FrameID==248)
//		CreatureType = 18;
}
