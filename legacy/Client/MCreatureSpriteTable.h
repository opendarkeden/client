//----------------------------------------------------------------------
// MCreatureSpriteTable.h
//----------------------------------------------------------------------
// Creature의 Sprite종류를 저장하는 class
//----------------------------------------------------------------------
//
// - 동작 FrameID
// - Sprite FilePosition
// - First SpriteID
// 
//----------------------------------------------------------------------

#ifndef	__MCREATURESPRITETABLE_H__
#define	__MCREATURESPRITETABLE_H__


#include "MObject.h"
#include "SpriteLib\DrawTypeDef.h"
#include "CTypeTable.h"
#include "MString.h"
class ifstream;
class ofstream;

//----------------------------------------------------------------------
// Flag CreatureType
//----------------------------------------------------------------------
#define FLAG_CREATURESPRITE_SLAYER				0x01
#define FLAG_CREATURESPRITE_VAMPIRE				0x02
#define FLAG_CREATURESPRITE_OUSTERS				0x04
#define FLAG_CREATURESPRITE_PLAYER				0x10
#define FLAG_CREATURESPRITE_NPC					0x20
#define FLAG_CREATURESPRITE_MONSTER				0x40

// 조합
#define FLAG_CREATURESPRITE_PLAYER_SLAYER		0x11			// 0001 0001
#define FLAG_CREATURESPRITE_PLAYER_VAMPIRE		0x12			// 0001 0010
#define FLAG_CREATURESPRITE_PLAYER_OUSTERS		0x14			// 0001 0100
#define FLAG_CREATURESPRITE_PLAYER_ALL			0x17			// 0001 0111
#define FLAG_CREATURESPRITE_NPC_SLAYER			0x21			// 0010 0001
#define FLAG_CREATURESPRITE_NPC_VAMPIRE			0x22			// 0010 0010
#define FLAG_CREATURESPRITE_NPC_OUSTERS			0x24			// 0010 0100
#define FLAG_CREATURESPRITE_NPC_ALL				0x27			// 0010 0111
#define FLAG_CREATURESPRITE_MONSTER_SLAYER		0x41			// 0100 0001
#define FLAG_CREATURESPRITE_MONSTER_VAMPIRE		0x42			// 0100 0010
#define FLAG_CREATURESPRITE_MONSTER_OUSTERS		0x44			// 0100 0100
#define FLAG_CREATURESPRITE_MONSTER_ALL			0x47			// 0100 0111

//----------------------------------------------------------------------
//
// 한 종류의 Creature에 대한 정보
//
//----------------------------------------------------------------------
class CREATURESPRITETABLE_INFO {
	public :		
		TYPE_FRAMEID			FrameID;				// 동작 FrameID
		long					SpriteFilePosition;		// Sprite FilePosition
		long					SpriteShadowFilePosition;		// ShadowSprite FilePosition
		TYPE_SPRITEID			FirstSpriteID;			// First SpriteID
		TYPE_SPRITEID			LastSpriteID;			// Last SpriteID		
		TYPE_SPRITEID			FirstShadowSpriteID;	// First SpriteID
		TYPE_SPRITEID			LastShadowSpriteID;		// Last SpriteID		
		BYTE					CreatureType;				// 뭐하는 sprite인가?

		// 이거는 저장할 필요가 없다.
 		BOOL					bLoad;
		
	public :
		CREATURESPRITETABLE_INFO();
		~CREATURESPRITETABLE_INFO();

		//-------------------------------------------------------
		// CreatureType
		//-------------------------------------------------------
		BYTE					IsPlayerSprite() const		{ return CreatureType & FLAG_CREATURESPRITE_PLAYER; }
		BYTE					IsNPCSprite() const			{ return CreatureType & FLAG_CREATURESPRITE_NPC; }
		BYTE					IsMonsterSprite()	const	{ return CreatureType & FLAG_CREATURESPRITE_MONSTER; }
		BYTE					IsVampireSprite()	const	{ return CreatureType & FLAG_CREATURESPRITE_VAMPIRE; }
		BYTE					IsOustersSprite()	const	{ return CreatureType & FLAG_CREATURESPRITE_OUSTERS; }
		BYTE					IsSlayerSprite()	const	{ return CreatureType & FLAG_CREATURESPRITE_SLAYER; }
		BYTE					IsPlayerVampireOnlySprite()	const	{ return CreatureType==FLAG_CREATURESPRITE_PLAYER_VAMPIRE; }
		BYTE					IsPlayerOnlySprite() const	{ return 
			CreatureType==FLAG_CREATURESPRITE_PLAYER_VAMPIRE || 
			CreatureType==FLAG_CREATURESPRITE_PLAYER_SLAYER || 
			CreatureType==FLAG_CREATURESPRITE_PLAYER_OUSTERS || 
			CreatureType==FLAG_CREATURESPRITE_PLAYER_ALL; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void				SaveToFile(class ofstream& file);
		void				LoadFromFile(class ifstream& file);	
};


typedef CTypeTable<CREATURESPRITETABLE_INFO>	CREATURESPRITE_TABLE;

extern CREATURESPRITE_TABLE*	g_pCreatureSpriteTable;

//extern CREATURESPRITE_TABLE*	g_pAddonSpriteTable;
//extern CREATURESPRITE_TABLE*	g_pCreatureActionSpriteTable;

#endif

