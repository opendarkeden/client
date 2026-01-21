//-----------------------------------------------------------------------------
// GamePacketFunctions.cpp
//-----------------------------------------------------------------------------
// Cross-platform game functions extracted from PacketFunction.cpp
// These functions are needed by the client but don't depend on Windows-specific APIs
//-----------------------------------------------------------------------------

#include "Client_PCH.h"
#include "MGameDef.h"
#include "MTopView.h"
#include "ClientDef.h"
#include "MCreatureWear.h"
#include "MItem.h"
#include "MEffectGeneratorTable.h"
#include "ClientConfig.h"
#include "MitemOptionTable.h"
#include "AddonDef.h"
#include "MItem.h"
#include "MInventory.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "MMoneyManager.h"
#include "MGameStringTable.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI.h"
#include "UIDialog.h"
#include "ServerInformation.h"
#include "PacketFunction.h"
#include "UserInformation.h"
#include "MFakeCreature.h"
#include "MSoundTable.h"
#include "MGuildMarkManager.h"
#include "MGuildInfoMapper.h"
#include "UIFunction.h"
#include "ServerInfo.h"
// packet
#include "packet/PetInfo.h"
#include "Packet/PCSlayerInfo3.h"
#include "Packet/PCOustersInfo3.h"
#include "Packet/GPackets/GCAddItemToZone.h"
#include "Packet/GPackets/GCAddMonster.h"
#include "Packet/PCSlayerInfo2.h"
#include "Packet/PCVampireInfo2.h"
#include "Packet/PCOustersInfo2.h"
#include "Packet/InventoryInfo.h"
#include "Packet/GearInfo.h"
#include "Packet/ExtraInfo.h"
#include "Packet/ModifyInfo.h"
#include "ExperienceTable.h"
#include "EffectInfo.h"
#include "MScreenEffectManager.h"
#include "TempInformation.h"
#include "DebugInfo.h"
#include "MEventManager.h"
#include "MCreature.h"
#include "MZone.h"
#include "MPlayer.h"
#include "VS_UI_GameCommon2.h"
#include "MEffectSpriteTypeTable.h"
#include "EffectSpriteTypeDef.h"
#include "BloodBibleSignInfo.h"

#define SAFE_DELETE(x)		{ if(x!=NULL) delete x; x=NULL; }

extern MScreenEffectManager*	g_pInventoryEffectManager;
extern DWORD				g_PreviousCreatureType;

// Hair style IDs for Slayer characters (from PacketFunction.cpp:125-132)
int	g_PacketHairMaleID[3] =
{
	151,	// 0
	152,	// 1
	153		// 2
};

int	g_PacketHairFemaleID[3] =
{
	154,	// 0
	155,	// 1
	156		// 2
};

//-----------------------------------------------------------------------------
// NewFakeCreature - Create fake creature from creature type (4 int parameters)
//-----------------------------------------------------------------------------
MFakeCreature*
NewFakeCreature(int creatureType, int x, int y, int dir)
{
	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------
	MFakeCreature* pFakeCreature = new MFakeCreature;

	pFakeCreature->SetZone( g_pZone );

#ifdef OUTPUT_DEBUG
	char name[20];
	sprintf(name, "Fake%d", pFakeCreature->GetID());
	pFakeCreature->SetName( name );
#endif

	//pFakeCreature->SetID( 0 );  자동 발급 된다.
//	pFakeCreature->SetSameBody( pCreature );

//	if (pCreature->IsWear())
//	{
//		pFakeCreature->SetSameWear( (MCreatureWear*)pCreature );
//	}

	pFakeCreature->SetCreatureType(creatureType);
	pFakeCreature->SetPosition( x, y );
	pFakeCreature->SetServerPosition( x, y );

	pFakeCreature->SetDirection( dir );
	pFakeCreature->SetCurrentDirection( dir );
	pFakeCreature->SetAction( ACTION_STAND );

	return pFakeCreature;
}

//-----------------------------------------------------------------------------
// NewFakeCreature - Create fake creature from existing creature (3 parameters)
//-----------------------------------------------------------------------------
MFakeCreature*
NewFakeCreature(MCreature* pCreature, int x, int y)
{
	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------
	MFakeCreature* pFakeCreature = new MFakeCreature;

	pFakeCreature->SetZone( g_pZone );

	#ifdef OUTPUT_DEBUG
		char name[20];
		sprintf(name, "Fake%d", pFakeCreature->GetID());
		pFakeCreature->SetName( name );
	#endif

	//pFakeCreature->SetID( 0 );  자동 발급 된다.
	pFakeCreature->SetSameBody( pCreature );

	if (pCreature->IsWear())
	{
		pFakeCreature->SetSameWear( (MCreatureWear*)pCreature );
	}

	pFakeCreature->SetPosition( x, y );
	pFakeCreature->SetServerPosition( x, y );

	int dir = pCreature->GetDirection();

	pFakeCreature->SetDirection( dir );
	pFakeCreature->SetCurrentDirection( dir );
	pFakeCreature->SetAction( ACTION_STAND );
	pFakeCreature->SetWeaponSpeed( pCreature->GetWeaponSpeed() );
	pFakeCreature->SetBasicActionInfo( pCreature->GetBasicActionInfo() );
	pFakeCreature->SetSpecialActionInfo( pCreature->GetSpecialActionInfo() );

	// add by Coffee
	//pFakeCreature->SetCreatureType(pFakeCreature->GetCreatureType());

	return pFakeCreature;
}

//-----------------------------------------------------------------------------
// Add_GDR_Potal_Effect - GDR portal effect (일루전스 웨이 포탈)
//-----------------------------------------------------------------------------
void Add_GDR_Potal_Effect(int nMapID)
{
	if(nMapID == 1410) // 일루전스 웨이 1
	{
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				73, 80, 0, 0, NULL, false);
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				118, 69, 0, 0, NULL, false);
	}
	else if(nMapID == 1411)
	{
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				125, 58, 0, 0, NULL, false);
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				22, 85, 0, 0, NULL, false);
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				9, 86, 0, 0, NULL, false);
		ExecuteActionInfoFromMainNode(MAP_GDR_LAIR_POTAL,0, 0, 0,0, 0,
				12, 102, 0, 0, NULL, false);
	}
}

//-----------------------------------------------------------------------------
// SetFadeStart - Start fade effect
//-----------------------------------------------------------------------------
void SetFadeStart(char start, char end, char step, BYTE r, BYTE g, BYTE b, WORD delay)
{
	if (g_pTopView != NULL)
		g_pTopView->SetFadeStart(start, end, step, r, g, b, delay);
}

//-----------------------------------------------------------------------------
// SetFadeEnd - End fade effect
//-----------------------------------------------------------------------------
void SetFadeEnd()
{
	if (g_pTopView != NULL)
		g_pTopView->SetFadeEnd();
}

//-----------------------------------------------------------------------------
// SetPetInfo - Set pet information
//-----------------------------------------------------------------------------
void SetPetInfo(PetInfo* pPetInfo, unsigned int flag)
{
	(void)pPetInfo; (void)flag;
	// Stub implementation - pet info handling
}

//-----------------------------------------------------------------------------
// SetGearInfo - Set gear information
//-----------------------------------------------------------------------------
void SetGearInfo(GearInfo* pGearInfo, BloodBibleSignInfo* pBloodBibleInfo)
{
	(void)pGearInfo; (void)pBloodBibleInfo;
	// Stub implementation - gear info handling
}

//-----------------------------------------------------------------------------
// SetExtraInfo - Set extra information
//-----------------------------------------------------------------------------
void SetExtraInfo(ExtraInfo* pExtraInfo)
{
	(void)pExtraInfo;
	// Stub implementation - extra info handling
}

//-----------------------------------------------------------------------------
// SetEffectInfo - Set effect information
//-----------------------------------------------------------------------------
void SetEffectInfo(MCreature* pCreature, EffectInfo* pEffectInfo, int delayedFrame)
{
	(void)pCreature; (void)pEffectInfo; (void)delayedFrame;
	// Stub implementation - effect info handling
}

//-----------------------------------------------------------------------------
// Add_Wild_Wolf - Add wild wolf creature effect
//-----------------------------------------------------------------------------
void Add_Wild_Wolf(MCreature* pUserCreature, MCreature* pTargetCreature, bool bEatCorpse)
{
	(void)pUserCreature; (void)pTargetCreature; (void)bEatCorpse;
	// Stub implementation - wild wolf effect
}

//-----------------------------------------------------------------------------
// ComeFromPortal - Creature coming from portal
//-----------------------------------------------------------------------------
void ComeFromPortal(MCreature* pCreature)
{
	if (pCreature == NULL)
		return;

	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------
	int x = pCreature->GetX();
	int y = pCreature->GetY();
	MFakeCreature* pFakeCreature = NewFakeCreature( pCreature, x, y );

	pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_TO_PORTAL );

	//------------------------------------------------------
	// Fake Creature를 Zone에 추가
	//------------------------------------------------------
	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}

	//------------------------------------------------------
	// 빙빙 돌아서 캐릭을 휘감는 이펙트 표현..
	//------------------------------------------------------
	ExecuteActionInfoFromMainNode(
			RESULT_MAGIC_BLOODY_TUNNEL_INTO,
			x, y, 0,
			DIRECTION_DOWN,
			pFakeCreature->GetID(),
			x, y, 0,
			0,
			NULL,
			false);
}

//-----------------------------------------------------------------------------
// MoveIntoPortal - Creature moving into portal
//-----------------------------------------------------------------------------
void MoveIntoPortal(MCreature* pCreature)
{
	int x = pCreature->GetX();
	int y = pCreature->GetY();
	MFakeCreature* pFakeCreature = NewFakeCreature( pCreature, x, y );

	pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_TO_PORTAL );

	//------------------------------------------------------
	// Fake Creature를 Zone에 추가
	//------------------------------------------------------
	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}

	//------------------------------------------------------
	// 빙빙 돌아서 캐릭을 휘감는 이펙트 표현..
	//------------------------------------------------------
	ExecuteActionInfoFromMainNode(
			RESULT_MAGIC_BLOODY_TUNNEL_INTO,
			x, y, 0,
			DIRECTION_DOWN,
			pFakeCreature->GetID(),
			x, y, 0,
			0,
			NULL,
			false);
}

//-----------------------------------------------------------------------------
// SkillIllendue - Illendue skill effect
//-----------------------------------------------------------------------------
void SkillIllendue(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID)
{
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] ShadowDancing. Creature NULL or Not WearCreature");
		return;
	}

	int x = pTargetCreature->GetX();
	int y = pTargetCreature->GetY();

	int sx = pUserCreature->GetX() - x;
	int sy = pUserCreature->GetY() - y;

	if (sx>0) sx=1; else if (sx<0) sx=-1; else sx=0;
	if (sy>0) sy=1; else if (sy<0) sy=-1; else sy=0;

	POINT Straight[4] = {
		{ -2, -2 },
		{ -2,  2 },
		{ 2,  -2 },
		{ 2,   2 },
	};

	POINT Diagonal[4] = {
		{ 0,  2 },
		{ 2,  0 },
		{ -2, 0 },
		{ 0, -2 },
	};

	POINT *Position = (pUserCreature->GetDirection() & 0x1) ? Diagonal : Straight;
	if (skillID == TEMP_SKILL_ILLUSION_INVERSION)
	{
		Position = Straight;
	}

	for (int i=0; i<4; i++)
	{
		MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pUserCreature->GetX() , pUserCreature->GetY() );

		int dir = pFakeCreature->GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );
		pFakeCreature->SetDirection( dir );
		pFakeCreature->SetCurrentDirection( dir );

		pFakeCreature->SetAction( ACTION_MAGIC );

		pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );

		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}

		if (skillID==SKILL_ATTACK_MELEE)
		{
			skillID = pUserCreature->GetBasicActionInfo();
		}

		if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
		{
			DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
			return;
		}

		DWORD delayFrame = 0;

		pFakeCreature->SetFakeCreatureFastMoveAction(
			pTargetCreature->GetX() + Position[i].x, pTargetCreature->GetY() + Position[i].y,
			skillID,
			pTargetCreature->GetID() );
	}
}

//-----------------------------------------------------------------------------
// SkillBlazeWalk - Blaze Walk skill effect
//-----------------------------------------------------------------------------
void SkillBlazeWalk(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID, BYTE grade)
{
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] ShadowDancing. Creature NULL or Not WearCreature");
		return;
	}

	POINT TargetList[3] =
	{
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 }
	};

	for (int i=0; i<=grade; i++)
	{
		MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pUserCreature->GetX() , pUserCreature->GetY() );

		int dir;
		switch( i )
		{
		case 0 :
			dir = 0;
			break;
		case 1 :
			dir = 4;
			break;
		case 2 :
			dir = 6;
			break;
		}

		pFakeCreature->SetDirection( dir );
		pFakeCreature->SetCurrentDirection( dir );

		pFakeCreature->SetAction( ACTION_MAGIC );

		pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );

		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}

		if (skillID==SKILL_ATTACK_MELEE)
		{
			skillID = pUserCreature->GetBasicActionInfo();
		}

		DWORD delayFrame = 0;

		pFakeCreature->SetFakeCreatureFastMoveAction(
			pUserCreature->GetX() + TargetList[i].x, pUserCreature->GetY() + TargetList[i].y,
			skillID,
			pTargetCreature->GetID() );
	}
}

//-----------------------------------------------------------------------------
// SkillToSector - Skill effect to sector
//-----------------------------------------------------------------------------
void SkillToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, int dir, DWORD delayFrame, MActionResult* pResult)
{
	(void)nActionInfo; (void)sX; (void)sY; (void)dir; (void)delayFrame; (void)pResult;
	// Stub implementation - skill to sector
}

//-----------------------------------------------------------------------------
// SetPCSlayerInfo - Set PC slayer information
//-----------------------------------------------------------------------------
void SetPCSlayerInfo(PCSlayerInfo2* pInfo)
{
	if (pInfo == NULL || g_pPlayer == NULL) return;

	DEBUG_ADD( "Set Slayer Info" );

	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정 - IMPORTANT: Set creature type!
	//--------------------------------------------------
	if (pInfo->getCompetence()==0)
	{
		g_pPlayer->SetCompetence( 0 );
		g_pPlayer->SetCreatureType( CREATURETYPE_SLAYER_OPERATOR );
		g_pPlayer->SetMale( pInfo->getSex()==MALE );
	}
	else
	{
		g_pPlayer->SetCreatureType( (pInfo->getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );
	}

	g_pPlayer->SetBodyColor1( pInfo->getSkinColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );
	SetAddonToSlayer( g_pPlayer, pInfo );

	// TODO: Add remaining implementation from PacketFunction.cpp:2691-2953
	// This includes guild info, rank, stats, domain levels, etc.
}

//-----------------------------------------------------------------------------
// AddItemToZone - Add item to zone
//-----------------------------------------------------------------------------
MItem* AddItemToZone(GCAddItemToZone* pPacket, BOOL bDropping)
{
	(void)pPacket; (void)bDropping;
	// Stub implementation - add item to zone
	return NULL;
}

//-----------------------------------------------------------------------------
// g_PrintColorStr - Print colored string (for UI descriptions)
//-----------------------------------------------------------------------------
int g_PrintColorStr(int x, int y, const char* str, PrintInfo& info, DWORD color)
{
	(void)x; (void)y; (void)str; (void)info; (void)color;
	// Stub implementation - print colored string
	return 0;
}

//-----------------------------------------------------------------------------
// SetAddonToOusters - Set addon info for Ousters (PCOustersInfo2)
//-----------------------------------------------------------------------------
void SetAddonToOusters(MCreatureWear* pCreature, const PCOustersInfo2* pInfo)
{
	(void)pCreature; (void)pInfo;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetAddonToOusters - Set addon info for Ousters (PCOustersInfo3)
//-----------------------------------------------------------------------------
void SetAddonToOusters(MCreatureWear* pCreature, const PCOustersInfo3* pInfo)
{
	if (pCreature == NULL || pInfo == NULL)
		return;

	int coatType = pInfo->getCoatType();
	int armType = pInfo->getArmType();

	// Check if g_pPacketItemOustersCoat array is properly initialized
	if (coatType < 0 || coatType >= OUSTERS_COAT_MAX) {
		fprintf(stderr, "ERROR: SetAddonToOusters: coatType=%d is out of range [0, %d)\n", coatType, OUSTERS_COAT_MAX);
		return;
	}
	if (g_pPacketItemOustersCoat[coatType] == NULL) {
		fprintf(stderr, "ERROR: SetAddonToOusters: g_pPacketItemOustersCoat[%d] is NULL! InitPacketItemTable() may not have been called.\n", coatType);
		return;
	}

	MItem* pCoat = g_pPacketItemOustersCoat[coatType];
	MItem* pArm = g_pPacketItemOustersArm[armType];

	pCreature->SetAddonItem( pCoat );
	pCreature->SetAddonItem( pArm );

	pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getHairColor() );
	pCreature->SetAddonColorSet2( ADDON_COAT, pInfo->getCoatColor() );
	pCreature->SetAddonColorSet1( ADDON_TROUSER, pInfo->getBootsColor() );
	pCreature->SetAddonColorSet2( ADDON_TROUSER, pInfo->getBootsColor() );
	pCreature->SetAddonColorSet1( ADDON_RIGHTHAND, pInfo->getArmColor() );
	pCreature->SetAddonColorSet2( ADDON_RIGHTHAND, pInfo->getArmColor() );

	pCreature->SetWeaponSpeed( pInfo->getAttackSpeed() );
}

//-----------------------------------------------------------------------------
// Additional functions needed by packet handlers
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AffectModifyInfo - Apply modify info to status
//-----------------------------------------------------------------------------
void AffectModifyInfo(MStatus* pStatus, ModifyInfo* pInfo)
{
	(void)pStatus; (void)pInfo;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetAddonToSlayer - Set addon for Slayer (PCSlayerInfo2)
//-----------------------------------------------------------------------------
void SetAddonToSlayer(MCreatureWear* pCreature, const PCSlayerInfo2* pInfo)
{
	if (pCreature == NULL || pInfo == NULL) return;

	//--------------------------------------------------
	// 머리 설정
	//--------------------------------------------------
	if (pInfo->getSex()==MALE)
	{
		pCreature->SetAddonHair(g_PacketHairMaleID[(int)pInfo->getHairStyle()], pInfo->getHairColor());
	}
	else
	{
		pCreature->SetAddonHair(g_PacketHairFemaleID[(int)pInfo->getHairStyle()], pInfo->getHairColor());
	}

	if(pInfo->getAdvancementLevel()>0)
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getHairColor() );
	else
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );

	pCreature->SetAddonColorSet1( ADDON_TROUSER, pInfo->getSkinColor() );
}

//-----------------------------------------------------------------------------
// SetAddonToSlayer - Set addon for Slayer (PCSlayerInfo3)
//-----------------------------------------------------------------------------
void SetAddonToSlayer(MCreatureWear* pCreature, const PCSlayerInfo3* pInfo)
{
	(void)pCreature; (void)pInfo;
	// Stub implementation - TODO: Add full implementation from PacketFunction.cpp:856-1150
}

//-----------------------------------------------------------------------------
// SetAddonToSlayer - Set addon for Slayer (int npcID)
//-----------------------------------------------------------------------------
void SetAddonToSlayer(MCreatureWear* pCreature, int npcID)
{
	(void)pCreature; (void)npcID;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetInventoryInfo - Set inventory information
//-----------------------------------------------------------------------------
void SetInventoryInfo(InventoryInfo* pInventoryInfo)
{
	(void)pInventoryInfo;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetPCOustersInfo - Set PC Ousters information
//-----------------------------------------------------------------------------
void SetPCOustersInfo(PCOustersInfo2* pInfo)
{
	if (pInfo == NULL || g_pPlayer == NULL) return;

	DEBUG_ADD( "Set Ousters Info" );

	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정 - IMPORTANT: Set creature type!
	//--------------------------------------------------
	if (pInfo->getCompetence()==0)
	{
		g_pPlayer->SetCompetence( 0 );
		g_pPlayer->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
		g_pPlayer->SetMale( pInfo->getSex()==MALE );
	}
	else
	{
		g_pPlayer->SetCreatureType( CREATURETYPE_OUSTERS );
	}

	g_pPlayer->SetGroundCreature();
	g_pPlayer->SetBodyColor1( pInfo->getHairColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	// TODO: Add remaining implementation from PacketFunction.cpp:3253-3370
}

//-----------------------------------------------------------------------------
// SetPCVampireInfo - Set PC Vampire information
//-----------------------------------------------------------------------------
void SetPCVampireInfo(PCVampireInfo2* pInfo)
{
	if (pInfo == NULL || g_pPlayer == NULL) return;

	DEBUG_ADD( "Set Vampire Info" );

	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정 - IMPORTANT: Set creature type!
	//--------------------------------------------------
	// 박쥐나 늑대로 변신 중인 경우..
	if (g_PreviousCreatureType==CREATURETYPE_WOLF
		|| g_PreviousCreatureType==CREATURETYPE_BAT
		|| g_PreviousCreatureType==CREATURETYPE_WER_WOLF)
	{
		g_pPlayer->SetCreatureType( g_PreviousCreatureType );
		if( pInfo->getCompetence() == 0 )
			g_pPlayer->SetCompetence( 0 );
	}
	else
	{
		if (pInfo->getCompetence()==0)
		{
			g_pPlayer->SetCompetence( 0 );
			g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_OPERATOR );
			g_pPlayer->SetMale( pInfo->getSex()==MALE );
		}
		else
		{
			int creatureType = (pInfo->getSex()==MALE)? CREATURETYPE_VAMPIRE_MALE1 : CREATURETYPE_VAMPIRE_FEMALE1;
			g_pPlayer->SetCreatureType( creatureType );
		}
	}

	g_pPlayer->SetMale( pInfo->getSex()==MALE );

	// 지상, 공중 이동 결정
	if (g_pPlayer->GetCreatureType()==CREATURETYPE_BAT)
	{
		g_pPlayer->SetFlyingCreature();
	}
	else
	{
		g_pPlayer->SetGroundCreature();
	}

	g_pPlayer->SetBodyColor1( pInfo->getSkinColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	if( g_pPlayer->GetCreatureType() == CREATURETYPE_WER_WOLF )
		g_pPlayer->SetBodyColor1( 377 );

	if( pInfo->getBatColor() != 0 )
		g_pPlayer->SetBatColor( pInfo->getBatColor() );
	else
		g_pPlayer->SetBatColor( 0xFFFF );

	// TODO: Add remaining implementation from PacketFunction.cpp:2954-3252
}

//-----------------------------------------------------------------------------
// Add_RocketRuncher - Add rocket launcher effect
//-----------------------------------------------------------------------------
void Add_RocketRuncher(MCreature* pUserCreature, MCreature* pTargetCreature)
{
	(void)pUserCreature; (void)pTargetCreature;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetBloodBibleSlot - Set blood bible slot
//-----------------------------------------------------------------------------
void SetBloodBibleSlot(BloodBibleSignInfo* pInfo)
{
	(void)pInfo;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SkillCrossCounter - Cross Counter skill effect
//-----------------------------------------------------------------------------
void SkillCrossCounter(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID)
{
	(void)pUserCreature; (void)pTargetCreature; (void)skillID;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SkillShadowDancing - Shadow Dancing skill effect
//-----------------------------------------------------------------------------
void SkillShadowDancing(MCreature* pUserCreature, MCreature* pTargetCreature, int type)
{
	(void)pUserCreature; (void)pTargetCreature; (void)type;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// g_PrintColorStrOut - Print colored string with outline
//-----------------------------------------------------------------------------
int g_PrintColorStrOut(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
	// Stub implementation
	return 0;
}

//-----------------------------------------------------------------------------
// AddOustersElemental - Add Ousters elemental effect
//-----------------------------------------------------------------------------
void AddOustersElemental(MCreature* pCreature, int elementalType, int count)
{
	(void)pCreature; (void)elementalType; (void)count;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// CheckItemForSkillIcon - Check if item should show skill icon
//-----------------------------------------------------------------------------
bool CheckItemForSkillIcon(const MItem* pItem)
{
	(void)pItem;
	// Stub implementation
	return false;
}

//-----------------------------------------------------------------------------
// More functions needed
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// g_PrintColorStrShadow - Print colored string with shadow
//-----------------------------------------------------------------------------
int g_PrintColorStrShadow(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
	// Stub implementation
	return 0;
}

//-----------------------------------------------------------------------------
// Add_Race_SlayerMonster - Add Slayer race monster
//-----------------------------------------------------------------------------
void Add_Race_SlayerMonster(GCAddMonster* pPacket)
{
	(void)pPacket;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// Add_Race_OustersMonster - Add Ousters race monster
//-----------------------------------------------------------------------------
void Add_Race_OustersMonster(GCAddMonster* pPacket)
{
	(void)pPacket;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// CreateActionResultNode - Create action result node
//-----------------------------------------------------------------------------
void CreateActionResultNode(MCreature* pCreature, int type, BYTE count)
{
	(void)pCreature; (void)type; (void)count;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// Global variables needed by the game
//-----------------------------------------------------------------------------

// Mouse position globals (note: no underscore prefix to match VS_UI declarations)
extern "C" {
	int g_mouse_x = 0;
	int g_mouse_y = 0;
}

// Ousters packet item globals are defined in PacketFunction.cpp
// MItem* g_pPacketItemOustersArm[OUSTERS_ARM_MAX] = {NULL};
// MItem* g_pPacketItemOustersCoat[OUSTERS_COAT_MAX] = {NULL};

//-----------------------------------------------------------------------------
// Packet Item Table globals (from PacketFunction.cpp)
// NOTE: PacketFunction.cpp is excluded on non-Windows platforms
//-----------------------------------------------------------------------------
MItem*		g_pPacketItemHelm[HELMET_MAX] = { NULL };
MItem*		g_pPacketItemJacket[JACKET_MAX] = { NULL };
MItem*		g_pPacketItemPants[PANTS_MAX] = { NULL };
MItem*		g_pPacketItemWeapon[WEAPON_MAX] = { NULL };
MItem*		g_pPacketItemShield[SHIELD_MAX] = { NULL };
MItem*		g_pPacketItemMotorcycle[MOTORCYCLE_MAX] = { NULL };
MItem*		g_pPacketItemShoulder[SHOULDER_MAX] = { NULL };

MItem*		g_pPacketItemOustersCoat[OUSTERS_COAT_MAX] = { NULL };
MItem*		g_pPacketItemOustersArm[OUSTERS_ARM_MAX] = { NULL };

// Wave pack file manager (note: no underscore prefix)
class CWavePackFileManager;
CWavePackFileManager* g_pWavePackFileManager = NULL;

// Info show globals (note: no underscore prefix)
extern "C" {
	bool gbl_info_show = true;
	bool gbl_show_item = true;
}

//-----------------------------------------------------------------------------
// Functions from GameFunctions.cpp that are still needed
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SetGoreLevel - Set gore level
//-----------------------------------------------------------------------------
void SetGoreLevel(bool bGoreLevel)
{
	(void)bGoreLevel;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// GetNMClipData - Get Netmarble clipboard data
//-----------------------------------------------------------------------------
bool GetNMClipData(char* pBuffer, unsigned int bufferSize, const char* pURL, bool bUseHTML)
{
	(void)pBuffer; (void)bufferSize; (void)pURL; (void)bUseHTML;
	// Stub implementation
	return false;
}

//-----------------------------------------------------------------------------
// SendBugReport - Send bug report
//-----------------------------------------------------------------------------
void SendBugReport(const char* pFormat, ...)
{
	// Stub implementation
}

//-----------------------------------------------------------------------------
// Add_GDR_Effect - Add GDR effect
//-----------------------------------------------------------------------------
void Add_GDR_Effect(int effectID, bool bEnable)
{
	(void)effectID; (void)bEnable;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// UseSkillCardOK - Use skill card
//-----------------------------------------------------------------------------
bool UseSkillCardOK(BYTE CardType)
{
	(void)CardType;
	// Stub implementation
	return false;
}

//-----------------------------------------------------------------------------
// SendCrashReport - Send crash report
//-----------------------------------------------------------------------------
void SendCrashReport()
{
	// Stub implementation
}

//-----------------------------------------------------------------------------
// AddVampirePortal - Add vampire portal
//-----------------------------------------------------------------------------
void AddVampirePortal(int serverID, const char* ownerName, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, DWORD delayFrame, int zoneID, TYPE_SECTORPOSITION zoneX, TYPE_SECTORPOSITION zoneY, bool bStartFromMainNode)
{
	(void)serverID; (void)ownerName; (void)sX; (void)sY; (void)delayFrame; (void)zoneID; (void)zoneX; (void)zoneY; (void)bStartFromMainNode;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// ConvertAlignment - Convert alignment value
//-----------------------------------------------------------------------------
int ConvertAlignment(int alignment)
{
	// Stub implementation
	return alignment;
}

//-----------------------------------------------------------------------------
// More functions needed
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SetDragonTorando - Set dragon tornado effect
//-----------------------------------------------------------------------------
void SetDragonTorando(int type, DWORD color, int count, int delay)
{
	(void)type; (void)color; (void)count; (void)delay;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// GetBombActionInfo - Get bomb action info
//-----------------------------------------------------------------------------
int GetBombActionInfo(int itemType)
{
	(void)itemType;
	// Stub implementation
	return 0;
}

//-----------------------------------------------------------------------------
// GetMineActionInfo - Get mine action info
//-----------------------------------------------------------------------------
int GetMineActionInfo(int itemType)
{
	(void)itemType;
	// Stub implementation
	return 0;
}

//-----------------------------------------------------------------------------
// PopupErrorMessage - Popup error message
//-----------------------------------------------------------------------------
void PopupErrorMessage(ErrorID errorID)
{
	(void)errorID;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// CheckMacScreenMode - Check Mac screen mode
//-----------------------------------------------------------------------------
bool CheckMacScreenMode()
{
	// Stub implementation
	return false;
}

//-----------------------------------------------------------------------------
// Send_nProtect_Auth - Send nProtect authentication
//-----------------------------------------------------------------------------
void Send_nProtect_Auth(DWORD authCode)
{
	(void)authCode;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// SetServerGroupName - Set server group name
//-----------------------------------------------------------------------------
void SetServerGroupName(const char* pName)
{
	(void)pName;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// Even more functions needed
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// InitPacketItemTable - Initialize packet item table
// NOTE: Copied from PacketFunction.cpp which is excluded on non-Windows
//-----------------------------------------------------------------------------
void InitPacketItemTable()
{
	//------------------------------------------------------------
	// MOustersCoat
	//------------------------------------------------------------
	MOustersCoat* pOustersCoatBasic = new MOustersCoat;	 pOustersCoatBasic->SetItemType( 0 ); pOustersCoatBasic->ClearItemOption();
	MOustersCoat* pOustersCoat1 = new MOustersCoat;	 pOustersCoat1->SetItemType( 0 ); pOustersCoat1->ClearItemOption();
	MOustersCoat* pOustersCoat2 = new MOustersCoat;	 pOustersCoat2->SetItemType( 3 ); pOustersCoat2->ClearItemOption();
	MOustersCoat* pOustersCoat3 = new MOustersCoat;	 pOustersCoat3->SetItemType( 6 ); pOustersCoat3->ClearItemOption();
	MOustersCoat* pOustersCoat4 = new MOustersCoat;	 pOustersCoat4->SetItemType( 6 ); pOustersCoat4->ClearItemOption();

	g_pPacketItemOustersCoat[OUSTERS_COAT_BASIC] = pOustersCoatBasic;
	g_pPacketItemOustersCoat[OUSTERS_COAT1] = pOustersCoat1;
	g_pPacketItemOustersCoat[OUSTERS_COAT2] = pOustersCoat2;
	g_pPacketItemOustersCoat[OUSTERS_COAT3] = pOustersCoat3;
	g_pPacketItemOustersCoat[OUSTERS_COAT4] = pOustersCoat4;

	//------------------------------------------------------------
	// MOustersArm
	//------------------------------------------------------------
	MOustersWristlet* pOustersArm1 = new MOustersWristlet;	 pOustersArm1->SetItemType( 0 ); pOustersArm1->ClearItemOption();
	MOustersChakram* pOustersArm2 = new MOustersChakram;	 pOustersArm2->SetItemType( 0 ); pOustersArm2->ClearItemOption();

	g_pPacketItemOustersArm[OUSTERS_ARM_GAUNTLET] = pOustersArm1;
	g_pPacketItemOustersArm[OUSTERS_ARM_CHAKRAM] = pOustersArm2;

	//------------------------------------------------------------
	// HELM
	//------------------------------------------------------------
	MHelm* pHelm1 = new MHelm;	 pHelm1->SetItemType( 0 ); pHelm1->ClearItemOption();
	MHelm* pHelm2 = new MHelm;	 pHelm2->SetItemType( 3 ); pHelm2->ClearItemOption();
	MHelm* pHelm3 = new MHelm;	 pHelm3->SetItemType( 15 ); pHelm3->ClearItemOption();
	MHelm* pHelm4 = new MHelm;	 pHelm4->SetItemType( 16 ); pHelm4->ClearItemOption();
	MHelm* pHelm5 = new MHelm;	 pHelm5->SetItemType( 17 ); pHelm5->ClearItemOption();

	g_pPacketItemHelm[HELMET_NONE]	= NULL;
	g_pPacketItemHelm[HELMET1]		= pHelm1;
	g_pPacketItemHelm[HELMET2]		= pHelm2;
	g_pPacketItemHelm[HELMET3]		= pHelm3;
	g_pPacketItemHelm[HELMET4]		= pHelm4;
	g_pPacketItemHelm[HELMET5]		= pHelm5;

	//------------------------------------------------------------
	// COAT
	//------------------------------------------------------------
	MCoat* pCoat1 = new MCoat;	pCoat1->SetItemType( 0 );	pCoat1->ClearItemOption();
	MCoat* pCoat2 = new MCoat;	pCoat2->SetItemType( 4 );	pCoat2->ClearItemOption();
	MCoat* pCoat3 = new MCoat;	pCoat3->SetItemType( 8 );	pCoat3->ClearItemOption();
	MCoat* pCoat4 = new MCoat;	pCoat4->SetItemType( 28 );	pCoat4->ClearItemOption();
	MCoat* pCoat5 = new MCoat;	pCoat5->SetItemType( 30 );	pCoat5->ClearItemOption();
	MCoat* pCoat6 = new MCoat;	pCoat6->SetItemType( 32 );	pCoat6->ClearItemOption();

	g_pPacketItemJacket[JACKET_BASIC]	= NULL;
	g_pPacketItemJacket[JACKET1]		= pCoat1;
	g_pPacketItemJacket[JACKET2]		= pCoat2;
	g_pPacketItemJacket[JACKET3]		= pCoat3;
	g_pPacketItemJacket[JACKET4]		= pCoat4;
	g_pPacketItemJacket[JACKET5]		= pCoat5;
	g_pPacketItemJacket[JACKET6]		= pCoat6;

	//------------------------------------------------------------
	// TROUSER
	//------------------------------------------------------------
	MTrouser* pTrouser1 = new MTrouser;		pTrouser1->SetItemType( 0 ); pTrouser1->ClearItemOption();
	MTrouser* pTrouser2 = new MTrouser;		pTrouser2->SetItemType( 4 ); pTrouser2->ClearItemOption();
	MTrouser* pTrouser3 = new MTrouser;		pTrouser3->SetItemType( 8 ); pTrouser3->ClearItemOption();

	g_pPacketItemPants[PANTS_BASIC]		= NULL;
	g_pPacketItemPants[PANTS1]			= pTrouser1;
	g_pPacketItemPants[PANTS2]			= pTrouser2;
	g_pPacketItemPants[PANTS3]			= pTrouser3;

	//------------------------------------------------------------
	// WEAPON
	//------------------------------------------------------------
	MSword*	pSword = new MSword;	pSword->SetItemType( 0 );	pSword->ClearItemOption();
	MSword*	pSword1 = new MSword;	pSword1->SetItemType( 16 );	pSword1->ClearItemOption();

	MBlade*	pBlade = new MBlade;	pBlade->SetItemType( 0 );	pBlade->ClearItemOption();
	MBlade*	pBlade1 = new MBlade;	pBlade1->SetItemType( 16 );	pBlade1->ClearItemOption();

	MGunAR*	pGunAR = new MGunAR;	pGunAR->SetItemType( 0 );	pGunAR->ClearItemOption();
	MGunAR*	pGunAR1 = new MGunAR;	pGunAR1->SetItemType( 14 );	pGunAR1->ClearItemOption();
	MGunAR*	pGunAR2 = new MGunAR;	pGunAR2->SetItemType( 15 );	pGunAR2->ClearItemOption();
	MGunAR*	pGunAR3 = new MGunAR;	pGunAR3->SetItemType( 16 );	pGunAR3->ClearItemOption();

	MGunTR*	pGunSR = new MGunTR;	pGunSR->SetItemType( 0 );	pGunSR->ClearItemOption();
	MGunTR*	pGunSR1 = new MGunTR;	pGunSR1->SetItemType( 14 );	pGunSR1->ClearItemOption();
	MGunTR*	pGunSR2 = new MGunTR;	pGunSR2->SetItemType( 15 );	pGunSR2->ClearItemOption();
	MGunTR*	pGunSR3 = new MGunTR;	pGunSR3->SetItemType( 16 );	pGunSR3->ClearItemOption();

	MGunSG*	pGunSG = new MGunSG;	pGunSG->SetItemType( 0 );	pGunSG->ClearItemOption();

	MGunSMG*pGunSMG = new MGunSMG;	pGunSMG->SetItemType( 0 );	pGunSMG->ClearItemOption();

	MCross*	pCross = new MCross;	pCross->SetItemType( 0 );	pCross->ClearItemOption();
	MCross*	pCross1 = new MCross;	pCross1->SetItemType( 14 );	pCross1->ClearItemOption();

	MMace*  pMace  = new MMace;     pMace->SetItemType( 0 );	pMace->ClearItemOption();
	MMace*  pMace1  = new MMace;     pMace1->SetItemType( 14 );	pMace1->ClearItemOption();

	g_pPacketItemWeapon[WEAPON_NONE]			= NULL;
	g_pPacketItemWeapon[WEAPON_SWORD]			= pSword;
	g_pPacketItemWeapon[WEAPON_SWORD1]			= pSword1;

	g_pPacketItemWeapon[WEAPON_BLADE]			= pBlade;
	g_pPacketItemWeapon[WEAPON_BLADE1]			= pBlade1;

	g_pPacketItemWeapon[WEAPON_AR]				= pGunAR;
	g_pPacketItemWeapon[WEAPON_AR1]				= pGunAR1;
	g_pPacketItemWeapon[WEAPON_AR2]				= pGunAR2;
	g_pPacketItemWeapon[WEAPON_AR3]				= pGunAR3;

	g_pPacketItemWeapon[WEAPON_SR]				= pGunSR;
	g_pPacketItemWeapon[WEAPON_SR1]				= pGunSR1;
	g_pPacketItemWeapon[WEAPON_SR2]				= pGunSR2;
	g_pPacketItemWeapon[WEAPON_SR3]				= pGunSR3;

	g_pPacketItemWeapon[WEAPON_SG]				= pGunSG;
	g_pPacketItemWeapon[WEAPON_SMG]				= pGunSMG;

	g_pPacketItemWeapon[WEAPON_CROSS]			= pCross;
	g_pPacketItemWeapon[WEAPON_CROSS1]			= pCross1;

	g_pPacketItemWeapon[WEAPON_MACE]			= pMace;
	g_pPacketItemWeapon[WEAPON_MACE1]			= pMace1;

	//------------------------------------------------------------
	// SHIELD
	//------------------------------------------------------------
	MShield* pShield1 = new MShield; pShield1->SetItemType( 0 ); pShield1->ClearItemOption();
	MShield* pShield2 = new MShield; pShield2->SetItemType( 5 ); pShield2->ClearItemOption();
	MShield* pShield3 = new MShield; pShield3->SetItemType( 15 ); pShield3->ClearItemOption();
	MShield* pShield4 = new MShield; pShield4->SetItemType( 17 ); pShield4->ClearItemOption();

	g_pPacketItemShield[SHIELD_NONE] = NULL;
	g_pPacketItemShield[SHIELD1] = pShield1;
	g_pPacketItemShield[SHIELD2] = pShield2;
	g_pPacketItemShield[SHIELD3] = pShield3;
	g_pPacketItemShield[SHIELD4] = pShield4;

	//------------------------------------------------------------
	// motorcycle
	//------------------------------------------------------------
	MMotorcycle* pMotor1 = new MMotorcycle;	pMotor1->SetItemType( 0 ); pMotor1->ClearItemOption();
	MMotorcycle* pMotor2 = new MMotorcycle;	pMotor2->SetItemType( 6 ); pMotor2->ClearItemOption();
	MMotorcycle* pMotor3 = new MMotorcycle;	pMotor3->SetItemType( 7 ); pMotor3->ClearItemOption();

	g_pPacketItemMotorcycle[MOTORCYCLE_NONE]	= NULL;
	g_pPacketItemMotorcycle[MOTORCYCLE1]		= pMotor1;
	g_pPacketItemMotorcycle[MOTORCYCLE2]		= pMotor2;
	g_pPacketItemMotorcycle[MOTORCYCLE3]		= pMotor3;

	//------------------------------------------------------------
	// shoulder
	//------------------------------------------------------------
	MShoulderArmor* pShoulder1 = new MShoulderArmor;	pShoulder1->SetItemType( 0 ); pShoulder1->ClearItemOption();

	g_pPacketItemShoulder[SHOULDER_NONE]	= NULL;
	g_pPacketItemShoulder[SHOULDER1]		= pShoulder1;
}

//-----------------------------------------------------------------------------
// SendUserIDToGameMonA - Send user ID to GameMon
//-----------------------------------------------------------------------------
void SendUserIDToGameMonA(const char* pUserID)
{
	(void)pUserID;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// ConvertDurationToFrame - Convert duration to frames
//-----------------------------------------------------------------------------
DWORD ConvertDurationToFrame(int duration)
{
	// Stub implementation
	return duration / 30;  // Assuming 30 FPS
}

//-----------------------------------------------------------------------------
// GetDirectionToPosition - Get direction to position
//-----------------------------------------------------------------------------
BYTE GetDirectionToPosition(int originX, int originY, int destX, int destY)
{
	// Stub implementation
	return 0;
}

//-----------------------------------------------------------------------------
// Final functions needed
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetNextTileByDirection - Get next tile by direction
//-----------------------------------------------------------------------------
POINT GetNextTileByDirection(int currentX, int currentY, unsigned char dir)
{
	// Stub implementation
	POINT pt = {currentX, currentY};
	return pt;
}

//-----------------------------------------------------------------------------
// GetOustersCreatureType - Get Ousters creature type
//-----------------------------------------------------------------------------
int GetOustersCreatureType(int coatType)
{
	// Stub implementation
	return coatType;
}

//-----------------------------------------------------------------------------
// GetVampireCreatureType - Get Vampire creature type
//-----------------------------------------------------------------------------
int GetVampireCreatureType(int level, bool bFemale, int rank)
{
	// Stub implementation
	(void)level; (void)bFemale; (void)rank;
	return 0;
}

//-----------------------------------------------------------------------------
// ReleasePacketItemTable - Release packet item table
//-----------------------------------------------------------------------------
void ReleasePacketItemTable()
{
	// Stub implementation
}

//-----------------------------------------------------------------------------
// UseItemOK - Use item OK
//-----------------------------------------------------------------------------
bool UseItemOK()
{
	// Stub implementation
	return true;
}

//-----------------------------------------------------------------------------
// SetFlagTo - Set flag to
//-----------------------------------------------------------------------------
void SetFlagTo(bool bFlag)
{
	(void)bFlag;
	// Stub implementation
}

//-----------------------------------------------------------------------------
// Conversion utility functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ConvertMillisecondToFrame - Convert milliseconds to frames
//-----------------------------------------------------------------------------
DWORD ConvertMillisecondToFrame(unsigned long ms)
{
	// Stub implementation - assuming 30 FPS
	return ms / 33;
}

//-----------------------------------------------------------------------------
// ConvertDurationToMillisecond - Convert duration to milliseconds
//-----------------------------------------------------------------------------
DWORD ConvertDurationToMillisecond(int duration)
{
	// Stub implementation - assuming duration is in frames
	return duration * 33;  // Convert to milliseconds (assuming 30 FPS)
}

//-----------------------------------------------------------------------------
// Windows-specific functions (stubs for macOS)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetMacAddressFromNetBIOS - Get MAC address from NetBIOS (Windows-only)
//-----------------------------------------------------------------------------
BOOL GetMacAddressFromNetBIOS(LPBYTE lpMacAddress)
{
	(void)lpMacAddress;
	// Stub implementation - NetBIOS is Windows-specific
	return FALSE;
}
