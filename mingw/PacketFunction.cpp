
//-----------------------------------------------------------------------------
// PacketFunction.cpp
//-----------------------------------------------------------------------------
// Packet에 관련된 utility함수
//-----------------------------------------------------------------------------

#include "Client_PCH.h"
#include <WINDOWS.H>
#include < nb30.h >
#include < wsipx.h >
#include < wsnwlink.h >
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
#include "Packet\PCSlayerInfo3.h"
#include "Packet/PCOustersInfo3.h"
#include "ExperienceTable.h"

#include "Packet\GPackets\GCAddItemToZone.h"
#include "Packet\GPackets\GCAddMonster.h"
#include "Packet\PCSlayerInfo2.h"
#include "Packet\PCVampireInfo2.h"
#include "Packet\PCOustersInfo2.h"
#include "Packet\InventoryInfo.h"
#include "Packet\GearInfo.h"
#include "Packet\ExtraInfo.h"
#include "Packet/Cpackets/CGSay.h"
#include "Packet/cpackets/CGCrashReport.h"
#include "EffectInfo.h"
#include "MScreenEffectManager.h"
#include "TempInformation.h"
#include "Packet\ModifyInfo.h"
#include "CrashReport.h"
#include "MCrashReportManager.h"
//#include "MFileDef.h"
#include "Properties.h"

#include "CImm.h"

#include "DebugInfo.h"
#include "MEventManager.h"
#include "MCreature.h"
#include "MZone.h"
#include "MPlayer.h"
#include "VS_UI_GameCommon2.h"
#include "MEffectSpriteTypeTable.h"
#include "EffectSpriteTypeDef.h"

#include "Packet/cpackets/CGAuthKey.h"
#include "BloodBibleSignInfo.h"

#ifdef __NPROTECT__
#include "PacketFunction.h"

extern CNPGameLib npgl;
#endif

#define SAFE_DELETE(x)		{ if(x!=NULL) delete x; x=NULL; }



extern MScreenEffectManager*	g_pInventoryEffectManager;
extern DWORD	g_PreviousCreatureType;

extern bool FileOpenBinary(const char* filename, ifstream& file);

extern BOOL g_bSetHotKey;

extern bool		g_bZoneLargeLoadImage;
extern bool		g_bZoneSmallLoadImage;
extern MZone*	g_pZoneLarge;
extern MZone*	g_pZoneSmall;
extern int		g_nZoneLarge;
extern int		g_nZoneSmall;
extern bool		g_bZonePlayerInLarge;
extern bool		g_bFullScreen;
int				g_DyeColorSet[48] = 
{400,359,312,267,222,174,130,86,381,342,297,252,207,159,115,70,369,327,283,237,193,145,101,57,
165,381,407,420,435,451,466,480,170,364,412,426,440,455,471,487,179,371,419,434,449,464,479,494};

// source화일 끝에 있다. header compile시간 아까버서 ..- -;
void	CheckItemForSkillIcon(const MItem* pItem);

extern BYTE g_macAddress[6];

//-----------------------------------------------------------------------------
// Global Packet HairID
//-----------------------------------------------------------------------------
int g_PacketHairMaleID[3] = 
{
	ADDONID_HAIR1_MALE,
	ADDONID_HAIR2_MALE,
	ADDONID_HAIR3_MALE
};

int g_PacketHairFemaleID[3] = 
{
	ADDONID_HAIR1_FEMALE,
	ADDONID_HAIR2_FEMALE,
	ADDONID_HAIR3_FEMALE
};

//-----------------------------------------------------------------------------
// Global Packet Item Table
//-----------------------------------------------------------------------------
MItem*		g_pPacketItemHelm[HELMET_MAX] = { NULL, };

MItem*		g_pPacketItemJacket[JACKET_MAX] = { NULL, };
MItem*		g_pPacketItemPants[PANTS_MAX] = { NULL, };
MItem*		g_pPacketItemWeapon[WEAPON_MAX] = { NULL, };
MItem*		g_pPacketItemShield[SHIELD_MAX] = { NULL, };
MItem*		g_pPacketItemMotorcycle[MOTORCYCLE_MAX] = { NULL, };
MItem*		g_pPacketItemShoulder[SHOULDER_MAX] = { NULL, };

MItem*		g_pPacketItemOustersCoat[OUSTERS_COAT_MAX] = { NULL, };
MItem*		g_pPacketItemOustersArm[OUSTERS_ARM_MAX] = { NULL, };

//-----------------------------------------------------------------------------
// Init PacketItemTable
//-----------------------------------------------------------------------------
void
InitPacketItemTable()
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
	MHelm* pHelm3 = new MHelm;	 pHelm3->SetItemType( 15 ); pHelm3->ClearItemOption();//by viva  //15
	MHelm* pHelm4 = new MHelm;	 pHelm4->SetItemType( 16 ); pHelm4->ClearItemOption();//by viva  //16
	MHelm* pHelm5 = new MHelm;	 pHelm5->SetItemType( 17 ); pHelm5->ClearItemOption();//by viva  //17

	g_pPacketItemHelm[HELMET_NONE]	= NULL;
	g_pPacketItemHelm[HELMET1]		= pHelm1;
	g_pPacketItemHelm[HELMET2]		= pHelm2;
	g_pPacketItemHelm[HELMET3]		= pHelm3;//by viva
	g_pPacketItemHelm[HELMET4]		= pHelm4;//by viva
	g_pPacketItemHelm[HELMET5]		= pHelm5;//by viva

	//------------------------------------------------------------
	// COAT
	//------------------------------------------------------------
	MCoat* pCoat1 = new MCoat;	pCoat1->SetItemType( 0 );	pCoat1->ClearItemOption();
	MCoat* pCoat2 = new MCoat;	pCoat2->SetItemType( 4 );	pCoat2->ClearItemOption();
	MCoat* pCoat3 = new MCoat;	pCoat3->SetItemType( 8 );	pCoat3->ClearItemOption();
	MCoat* pCoat4 = new MCoat;	pCoat4->SetItemType( 28 );	pCoat4->ClearItemOption();//add by viva //28 29
	MCoat* pCoat5 = new MCoat;	pCoat5->SetItemType( 30 );	pCoat5->ClearItemOption();//add by viva //30 31
	MCoat* pCoat6 = new MCoat;	pCoat6->SetItemType( 32 );	pCoat6->ClearItemOption();//add by viva //32 33

	g_pPacketItemJacket[JACKET_BASIC]	= NULL;	// pCoat1
	g_pPacketItemJacket[JACKET1]		= pCoat1;
	g_pPacketItemJacket[JACKET2]		= pCoat2;
	g_pPacketItemJacket[JACKET3]		= pCoat3;
	g_pPacketItemJacket[JACKET4]		= pCoat4;	//add by viva
	g_pPacketItemJacket[JACKET5]		= pCoat5;	//add by viva
	g_pPacketItemJacket[JACKET6]		= pCoat6;	//add by viva

	//------------------------------------------------------------
	// TROUSER
	//------------------------------------------------------------
	MTrouser* pTrouser1 = new MTrouser;		pTrouser1->SetItemType( 0 ); pTrouser1->ClearItemOption();
	MTrouser* pTrouser2 = new MTrouser;		pTrouser2->SetItemType( 4 ); pTrouser2->ClearItemOption();
	MTrouser* pTrouser3 = new MTrouser;		pTrouser3->SetItemType( 8 ); pTrouser3->ClearItemOption();

	g_pPacketItemPants[PANTS_BASIC]		= NULL;//pTrouser1;
	g_pPacketItemPants[PANTS1]			= pTrouser1;
	g_pPacketItemPants[PANTS2]			= pTrouser2;
	g_pPacketItemPants[PANTS3]			= pTrouser3;

	//------------------------------------------------------------
	// WEAPON
	//------------------------------------------------------------
	MSword*	pSword = new MSword;	pSword->SetItemType( 0 );	pSword->ClearItemOption();
	MSword*	pSword1 = new MSword;	pSword1->SetItemType( 16 );	pSword1->ClearItemOption();//by viva //16 17 18

	MBlade*	pBlade = new MBlade;	pBlade->SetItemType( 0 );	pBlade->ClearItemOption();
	MBlade*	pBlade1 = new MBlade;	pBlade1->SetItemType( 16 );	pBlade1->ClearItemOption();//by viva //16 17 18

	MGunAR*	pGunAR = new MGunAR;	pGunAR->SetItemType( 0 );	pGunAR->ClearItemOption();
	MGunAR*	pGunAR1 = new MGunAR;	pGunAR1->SetItemType( 14 );	pGunAR1->ClearItemOption();//by viva //14
	MGunAR*	pGunAR2 = new MGunAR;	pGunAR2->SetItemType( 15 );	pGunAR2->ClearItemOption();//by viva //15
	MGunAR*	pGunAR3 = new MGunAR;	pGunAR3->SetItemType( 16 );	pGunAR3->ClearItemOption();//by viva //16 17 18

	MGunTR*	pGunSR = new MGunTR;	pGunSR->SetItemType( 0 );	pGunSR->ClearItemOption();
	MGunTR*	pGunSR1 = new MGunTR;	pGunSR1->SetItemType( 14 );	pGunSR1->ClearItemOption();//by viva //14
	MGunTR*	pGunSR2 = new MGunTR;	pGunSR2->SetItemType( 15 );	pGunSR2->ClearItemOption();//by viva //15
	MGunTR*	pGunSR3 = new MGunTR;	pGunSR3->SetItemType( 16 );	pGunSR3->ClearItemOption();//by viva //16 17 18

	MGunSG*	pGunSG = new MGunSG;	pGunSG->SetItemType( 0 );	pGunSG->ClearItemOption();

	MGunSMG*pGunSMG = new MGunSMG;	pGunSMG->SetItemType( 0 );	pGunSMG->ClearItemOption();

	MCross*	pCross = new MCross;	pCross->SetItemType( 0 );	pCross->ClearItemOption();
	MCross*	pCross1 = new MCross;	pCross1->SetItemType( 14 );	pCross1->ClearItemOption();//by viva //14 15 16

	MMace*  pMace  = new MMace;     pMace->SetItemType( 0 );	pMace->ClearItemOption();
	MMace*  pMace1  = new MMace;     pMace1->SetItemType( 14 );	pMace1->ClearItemOption();//by viva //14 15 16

	g_pPacketItemWeapon[WEAPON_NONE]			= NULL;
	g_pPacketItemWeapon[WEAPON_SWORD]			= pSword;
	g_pPacketItemWeapon[WEAPON_SWORD1]			= pSword1;//by viva

	g_pPacketItemWeapon[WEAPON_BLADE]			= pBlade;
	g_pPacketItemWeapon[WEAPON_BLADE1]			= pBlade1;//by viva

	//g_pPacketItemWeapon[WEAPON_SHIELD]			= NULL;
	//g_pPacketItemWeapon[WEAPON_SWORD_SHIELD]	= pSword;
	g_pPacketItemWeapon[WEAPON_AR]				= pGunAR;
	g_pPacketItemWeapon[WEAPON_AR1]				= pGunAR1;//by viva
	g_pPacketItemWeapon[WEAPON_AR2]				= pGunAR2;//by viva
	g_pPacketItemWeapon[WEAPON_AR3]				= pGunAR3;//by viva

	g_pPacketItemWeapon[WEAPON_SR]				= pGunSR;
	g_pPacketItemWeapon[WEAPON_SR1]				= pGunSR1;//by viva
	g_pPacketItemWeapon[WEAPON_SR2]				= pGunSR2;//by viva
	g_pPacketItemWeapon[WEAPON_SR3]				= pGunSR3;//by viva

	g_pPacketItemWeapon[WEAPON_SG]				= pGunSG;
	g_pPacketItemWeapon[WEAPON_SMG]				= pGunSMG;

	g_pPacketItemWeapon[WEAPON_CROSS]			= pCross;	
	g_pPacketItemWeapon[WEAPON_CROSS1]			= pCross1;//by viva

	g_pPacketItemWeapon[WEAPON_MACE]			= pMace;
	g_pPacketItemWeapon[WEAPON_MACE1]			= pMace1;//by viva

	//------------------------------------------------------------
	// SHIELD
	//------------------------------------------------------------
	MShield* pShield1 = new MShield; pShield1->SetItemType( 0 ); pShield1->ClearItemOption();
	MShield* pShield2 = new MShield; pShield2->SetItemType( 5 ); pShield2->ClearItemOption();
	MShield* pShield3 = new MShield; pShield3->SetItemType( 15 ); pShield3->ClearItemOption();//by viva //15 16
	MShield* pShield4 = new MShield; pShield4->SetItemType( 17 ); pShield4->ClearItemOption();//by viva //17

	g_pPacketItemShield[SHIELD_NONE] = NULL;
	g_pPacketItemShield[SHIELD1] = pShield1;
	g_pPacketItemShield[SHIELD2] = pShield2;
	g_pPacketItemShield[SHIELD3] = pShield3;//by viva
	g_pPacketItemShield[SHIELD4] = pShield4;//by viva
	
	//------------------------------------------------------------
	// motorcycle
	//------------------------------------------------------------
	MMotorcycle* pMotor1 = new MMotorcycle;	pMotor1->SetItemType( 0 ); pMotor1->ClearItemOption();
	// 2004, 1, 5, sobeit modify start - type 변경 1->6, 2->7 로
	MMotorcycle* pMotor2 = new MMotorcycle;	pMotor2->SetItemType( 6 ); pMotor2->ClearItemOption();
	MMotorcycle* pMotor3 = new MMotorcycle;	pMotor3->SetItemType( 7 ); pMotor3->ClearItemOption();
	// 2004, 1, 5, sobeit modify end

	g_pPacketItemMotorcycle[MOTORCYCLE_NONE]	= NULL;
	g_pPacketItemMotorcycle[MOTORCYCLE1]		= pMotor1;
	g_pPacketItemMotorcycle[MOTORCYCLE2]		= pMotor2;
	g_pPacketItemMotorcycle[MOTORCYCLE3]		= pMotor3;

	
	//------------------------------------------------------------
	// shouler
	//------------------------------------------------------------

	MItem*		g_pPacketItemShoulder[SHOULDER_MAX] = { NULL, };
	
	MShoulderArmor* pShoulder1 = new MShoulderArmor;	pShoulder1->SetItemType( 0 ); pShoulder1->ClearItemOption();

	g_pPacketItemShoulder[SHOULDER_NONE]	= NULL;
	g_pPacketItemShoulder[SHOULDER1]		= pShoulder1;
	
}

//-----------------------------------------------------------------------------
// Init PacketItemTable
//-----------------------------------------------------------------------------
void
ReleasePacketItemTable()
{
	int i;

	//------------------------------------------------------------
	// OUSTERS_COAT
	//------------------------------------------------------------
	for (i=0; i<OUSTERS_COAT_MAX; i++)
	{
		if (g_pPacketItemOustersCoat[i]!=NULL)
		{
			delete g_pPacketItemOustersCoat[i];
			g_pPacketItemOustersCoat[i] = NULL;
		}
	}

	//------------------------------------------------------------
	// OUSTERS_ARM
	//------------------------------------------------------------
	for (i=0; i<OUSTERS_ARM_MAX; i++)
	{
		if (g_pPacketItemOustersArm[i]!=NULL)
		{
			delete g_pPacketItemOustersArm[i];
			g_pPacketItemOustersArm[i] = NULL;
		}
	}
	

	//------------------------------------------------------------
	// HELM
	//------------------------------------------------------------
	for (i=0; i<HELMET_MAX; i++)
	{
		if (i!=HELMET_NONE 
			&& g_pPacketItemHelm[i]!=NULL)
		{
			delete g_pPacketItemHelm[i];
		}
	}
	
	//------------------------------------------------------------
	// COAT
	//------------------------------------------------------------
	for (i=0; i<JACKET_MAX; i++)
	{
		if (i!=JACKET_BASIC 
			&& g_pPacketItemJacket[i]!=NULL)
		{
			delete g_pPacketItemJacket[i];
		}
	}	

	//------------------------------------------------------------
	// TROUSER
	//------------------------------------------------------------
	for (i=0; i<PANTS_MAX; i++)
	{
		if (i!=PANTS_BASIC
			&& g_pPacketItemPants[i]!=NULL)
		{
			delete g_pPacketItemPants[i];
		}
	}	

	//------------------------------------------------------------
	// WEAPON
	//------------------------------------------------------------
	for (i=0; i<WEAPON_MAX; i++)		
	{
		if (i!=WEAPON_NONE 
			&& g_pPacketItemWeapon[i]!=NULL)
		{
			delete g_pPacketItemWeapon[i];
		}
	}

	//------------------------------------------------------------
	// SHIELD
	//------------------------------------------------------------
	for (i=0; i<SHIELD_MAX; i++)
	{
		if (g_pPacketItemShield[i]!=NULL)
		{
			delete g_pPacketItemShield[i];
		}
	}
	
	//------------------------------------------------------------
	// motorcycle
	//------------------------------------------------------------
	for (i=0; i<MOTORCYCLE_MAX; i++)
	{
		if (i!=MOTORCYCLE_NONE
			&& g_pPacketItemMotorcycle[i]!=NULL)
		{
			delete g_pPacketItemMotorcycle[i];
		}
	}

	//------------------------------------------------------------
	// shoulder
	//------------------------------------------------------------
	for (i=0; i<SHOULDER_MAX; i++)
	{
		if (i!=SHOULDER_NONE
			&& g_pPacketItemShoulder[i]!=NULL)
		{
			delete g_pPacketItemShoulder[i];
		}
	}
}

//-----------------------------------------------------------------------------
// Get Mine ActionInfo
//-----------------------------------------------------------------------------
// 지뢰(item type)랑 관련된 actioninfo를 알아낸다.
// 없으면 -1을 return한다.
//-----------------------------------------------------------------------------
int
GetMineActionInfo(int itemType)
{
	const int maxMine = 5;

	const int mineSkillID[maxMine] = {
		MINE_ANKLE_KILLER,
		MINE_POMZ,
		MINE_AP_C1,
		MINE_DIAMONDBACK,		
		MINE_SWIFT_EX
	};

	if (itemType>=0 && itemType<maxMine)
	{		
		return mineSkillID[itemType];
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Get Bomb ActionInfo
//-----------------------------------------------------------------------------
// 폭탄(item type)랑 관련된 actioninfo를 알아낸다.
// 없으면 -1을 return한다.
//-----------------------------------------------------------------------------
int
GetBombActionInfo(int itemType)
{
	const int maxBomb = 5;

	const int bombSkillID[maxBomb] = {
		BOMB_SPLINTER,
		BOMB_ACER,
		BOMB_BULLS,
		BOMB_STUN,
		BOMB_CROSSBOW,		
	};

	if (itemType>=0 && itemType<maxBomb)
	{		
		return bombSkillID[itemType];
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Set Gore Level
//-----------------------------------------------------------------------------
void
SetGoreLevel(bool bGoreLevel)
{
	// Gore Level 할려면..
	// EffectGenerator에서 effectSpriteType이 EFFECTSPRITETYPE_NULL인 
	// 경우를 체크해야 한다. 
	// MTopView에서도 출력할때 NULL체크해야한다.
	bool bZoneRelease = false;

//	if (g_pUserInformation->GoreLevel)
	{
		//-------------------------------------------------------
		// Gore ---> not Gore
		//-------------------------------------------------------
		// effect들을 바꿔준다.
		//-------------------------------------------------------
		if (!bGoreLevel)
		{
			g_pActionInfoTable->SetChildMode();

			bZoneRelease = true;
		}
	}
//	else
	{
		//-------------------------------------------------------
		// not Gore --> Gore
		//-------------------------------------------------------
		// 다시 loading하면 된다.
		//-------------------------------------------------------
		if (bGoreLevel)
		{
			//-----------------------------------
			// Load From File
			//-----------------------------------
			ifstream actionInfoFile2;//(FILE_INFO_ACTION, ios::binary);
			if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ACTION").c_str(), actionInfoFile2))
				return;

			g_pActionInfoTable->LoadFromFile(actionInfoFile2);
			actionInfoFile2.close();

			bZoneRelease = true;
		}
	}

	if(bZoneRelease == true)
	{
		//------------------------------------------------------
		// Small Zone 제거
		//------------------------------------------------------
		if (g_pZoneSmall != NULL)
		{
			DEBUG_ADD("[ delete SmallZone ]");
			delete g_pZoneSmall;	
			g_pZoneSmall = NULL;
			g_nZoneSmall = ZONEID_NULL;	
			g_bZoneSmallLoadImage = false;
		}			
		
		//------------------------------------------------------
		// Large Zone 제거
		//------------------------------------------------------
		if (g_pZoneLarge != NULL)
		{
			DEBUG_ADD("[ delete LargeZone ]");
			delete g_pZoneLarge;	
			g_pZoneLarge = NULL;
			g_nZoneLarge = ZONEID_NULL;	
			g_bZoneLargeLoadImage = false;
		}
		
		g_pZone = NULL;
		
	}

	g_pUserInformation->GoreLevel = bGoreLevel;
}

//-----------------------------------------------------------------------------
// Get Direction To Position
//-----------------------------------------------------------------------------
BYTE		
GetDirectionToPosition(int originX, int originY, int destX, int destY)
{
	return MTopView::GetDirectionToPosition(originX, originY, destX, destY);
}

//-----------------------------------------------------------------------------
// Convert Alignment
//-----------------------------------------------------------------------------
int
ConvertAlignment(int alignment2)
{
	int alignment = alignment2;

	// 음수인 경우..
	if (alignment > 10000)
	{	
		alignment = - (short)(~alignment + 1);

		DEBUG_ADD_FORMAT("Modify AlignmentValue = %d --> %d", alignment2, alignment);
	}

	const int numAlignment = 5;
	int alignmentValue[numAlignment] =
	{
		-7501,		// evil+
		-2501,		// evil
		2500,		// neutral
		7500,		// good
		10000		// good+
	};

	for (int i=0; i<numAlignment; i++)
	{
		if (alignment <= alignmentValue[i])
		{
			return i;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Convert Duration To Frame
//-----------------------------------------------------------------------------
DWORD
ConvertDurationToFrame(int duration)
{
	// 초당 16 Frame
	// 0.1초당 1.6 Frame
	// 1 --> 0.1초
	// 10 --> 1초
	return duration * g_pClientConfig->FPS / 10;
}

//-----------------------------------------------------------------------------
// Convert Millisecond To Frame
//-----------------------------------------------------------------------------
DWORD
ConvertMillisecondToFrame(DWORD ms)
{
	// 1 ms = 1/1000
	// 1000 ms = 1
	// 1 초 = 16 frame
	return ms * g_pClientConfig->FPS / 1000;
}

//-----------------------------------------------------------------------------
// Convert Duration To Millisecond
//-----------------------------------------------------------------------------
DWORD
ConvertDurationToMillisecond(int duration)
{
	// 1 --> 0.1초	
	// 1 --> 100
	return duration * 100;
}

//-----------------------------------------------------------------------------
// Set VampireCreatureType
//-----------------------------------------------------------------------------
// 사람(남, 여), 늑대, 박쥐..
//-----------------------------------------------------------------------------
int
GetVampireCreatureType(int shape, bool bMale, int coatType)
{
	DEBUG_ADD_FORMAT("Shape=%d. Male=%d", shape, (int)bMale);
	
	//--------------------------------------------------
	// CreatureType 설정
	//--------------------------------------------------
	switch ( shape )
	{
	case SHAPE_NORMAL :
		if(bMale)
		{
			return (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonMaleFrameID;//(bMale)? CREATURETYPE_VAMPIRE_MALE : CREATURETYPE_VAMPIRE_FEMALE;
		}
		else
		{
			return (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonFemaleFrameID;//(bMale)? CREATURETYPE_VAMPIRE_MALE : CREATURETYPE_VAMPIRE_FEMALE;
		}		
		break;
	case SHAPE_BAT :
		return CREATURETYPE_BAT;
		break;
	case SHAPE_WOLF :
		return CREATURETYPE_WOLF;
		break;
	case SHAPE_WERWOLF :
		return CREATURETYPE_WER_WOLF;
		break;
	} 

	return SHAPE_MAX;
}


//-----------------------------------------------------------------------------
// Set OustersCreatureType
//-----------------------------------------------------------------------------
// 사람(남, 여), 늑대, 박쥐..
//-----------------------------------------------------------------------------
int
GetOustersCreatureType(int coatType)
{
	//--------------------------------------------------
	// CreatureType 설정
	//--------------------------------------------------
	return CREATURETYPE_OUSTERS;
	//	return (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonMaleFrameID;
}

//-----------------------------------------------------------------------------
// Set Addon To Slayer
//-----------------------------------------------------------------------------
// NPC 복장을 입힌다.
//-----------------------------------------------------------------------------
void
SetAddonToSlayer(MCreatureWear* pCreature, int npcID )
{
	//-------------------------------------------------------------
	// 값 체크
	//-------------------------------------------------------------
	if (npcID >= g_pCreatureTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error] SetAddonToSlayerNPC: npcID exceed Max = %d/%d", npcID, g_pCreatureTable->GetSize());
		
		return;
	}

	//-------------------------------------------------------------
	// 복장을 하나하나 입힌다.
	//-------------------------------------------------------------
	ITEM_WEARINFO* pInfo = (*g_pCreatureTable)[npcID].pItemWearInfo;

	if (pInfo==NULL)
	{
		DEBUG_ADD_FORMAT("[Error] SetAddonToSlayerNPC: ItemWearInfo is NULL. cType=%d", npcID);
		
		return;
	}
	
	//----------------------------------------	
	// 머리
	//----------------------------------------	
	if ((*g_pCreatureTable)[npcID].bMale)
	{
		pCreature->SetAddonHair(g_PacketHairMaleID[pInfo->hair], pInfo->hairColor);		
	}
	// slayer 여
	else
	{
		pCreature->SetAddonHair(g_PacketHairFemaleID[pInfo->hair], pInfo->hairColor);	
	}

	//----------------------------------------	
	// 복장
	//----------------------------------------	
	MItem* pCoat		= g_pPacketItemJacket[pInfo->jacket];
	MItem* pTrouser		= g_pPacketItemPants[pInfo->pants];
	MItem* pHelm		= g_pPacketItemHelm[pInfo->helmet];
	MItem* pWeapon		= g_pPacketItemWeapon[pInfo->weapon];
	MItem* pShield		= g_pPacketItemShield[pInfo->shield];
	MItem* pMotorcycle	= g_pPacketItemMotorcycle[pInfo->motorcycle];

	// 색깔정보 설정...
	//pCoat->SetItemOptionList( getHelmetColor() );
	//
	//
	//

	pCreature->SetAddonItem( pCoat );
	pCreature->SetAddonItem( pTrouser );
	pCreature->SetAddonItem( pHelm );
	pCreature->SetAddonItem( pWeapon );
	pCreature->SetAddonItem( pShield );
	pCreature->SetAddonItem( pMotorcycle );
	

	/*
	// 왼손엔 방패
	if (pInfo->getWeaponType()==WEAPON_SWORD_SHIELD || pInfo->getWeaponType()==WEAPON_SHIELD)
	{
		pCreature->SetAddonItem( pShield );

		if (pShield!=NULL)
		{
			pCreature->SetAddonColorSet1( ADDON_LEFTHAND, pInfo->getShieldColor() );
		}
	}
	*/

	//----------------------------------------	
	// 색깔 설정
	//----------------------------------------	
	if (pCoat!=NULL)
	{
		pCreature->SetAddonColorSet2( ADDON_COAT, pInfo->jacketColor );
	}

	pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->skinColor );
		

	if (pTrouser!=NULL)
	{	
		pCreature->SetAddonColorSet2( ADDON_TROUSER, pInfo->pantsColor );
	}
	
	pCreature->SetAddonColorSet1( ADDON_TROUSER, pInfo->skinColor );
		
	if (pHelm!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_HELM, pInfo->helmetColor );
	}

	if (pWeapon!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_RIGHTHAND, pInfo->weaponColor );	
	}

	if (pShield!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_LEFTHAND, pInfo->shieldColor );	
	}

	if (pMotorcycle!=NULL)
	{
		pCreature->SetAddonColorSet1( ADDON_MOTOR, pInfo->motorcycleColor );
	}

	//----------------------------------------	
	// 공격 속도 설정
	//----------------------------------------	
//	pCreature->SetStatus( MODIFY_ALIGNMENT, pInfo->getAlignment() );
//	pCreature->SetWeaponSpeed( pInfo->getAttackSpeed() );

	DEBUG_ADD("[SetAddonToSlayer] NPC Wear OK");
}

//-----------------------------------------------------------------------------
// Set Addon To Slayer
//-----------------------------------------------------------------------------
// 머리
//-----------------------------------------------------------------------------
void		
SetAddonToSlayer(MCreatureWear* pCreature, const PCSlayerInfo2* pInfo)
{		
	//--------------------------------------------------
	// 머리 설정
	//--------------------------------------------------
	// slayer남 
	if (pInfo->getSex()==MALE)
	{
		pCreature->SetAddonHair(g_PacketHairMaleID[(int)pInfo->getHairStyle()], pInfo->getHairColor());		
	}
	// slayer 여
	else
	{
		pCreature->SetAddonHair(g_PacketHairFemaleID[(int)pInfo->getHairStyle()], pInfo->getHairColor ());	
	}

	if(pInfo->getAdvancementLevel()>0) // 승직 캐릭터
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getHairColor () );
	else
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );

	pCreature->SetAddonColorSet1( ADDON_TROUSER, pInfo->getSkinColor() );
}

//-----------------------------------------------------------------------------
// Set Addon To Slayer
//-----------------------------------------------------------------------------
// 복장
//-----------------------------------------------------------------------------
void		
SetAddonToSlayer(MCreatureWear* pCreature, const PCSlayerInfo3* pInfo)
{	
	/*
	HairStyle getHairStyle () const throw () 
	HelmetType getHelmetType () const throw () 
	JacketType getJacketType () const throw () 
	PantsType getPantsType () const throw () 
	WeaponType getWeaponType () const throw ()
	MotorcycleType getMotorcycleType () const throw ()
	
	Color_t getHairColor () const throw () 
	Color_t getSkinColor () const throw () 
	Color_t getHelmetColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	Color_t getJacketColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	Color_t getPantsColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	Color_t getWeaponColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	Color_t getShieldColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	Color_t getMotorcycleColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	*/


	//----------------------------------------	
	//
	// option과 color는 따로 지정해야 한다.
	//
	//----------------------------------------	
	//Color_t getHairColor () const throw () 
	//Color_t getSkinColor () const throw () 
	//Color_t getHelmetColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	//Color_t getJacketColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	//Color_t getPantsColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	//Color_t getWeaponColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	//Color_t getShieldColor ( ColorType colorType = MAIN_COLOR ) const throw () 
	//Color_t getMotorcycleColor ( ColorType colorType = MAIN_COLOR ) const throw () 

	//----------------------------------------	
	// 머리
	//----------------------------------------	
	if (pInfo->getSex()==MALE)
	{
		pCreature->SetAddonHair(g_PacketHairMaleID[pInfo->getHairStyle()], pInfo->getHairColor());		
	}
	// slayer 여
	else
	{
		pCreature->SetAddonHair(g_PacketHairFemaleID[pInfo->getHairStyle()], pInfo->getHairColor());	
	}

	//----------------------------------------	
	// 복장
	//----------------------------------------	
	MItem* pCoat		= g_pPacketItemJacket[pInfo->getJacketType()];
//	MItem* pCoat		= g_pPacketItemJacket[JACKET_BASIC];//by viva
	MItem* pTrouser		= g_pPacketItemPants[pInfo->getPantsType()];
	MItem* pHelm		= g_pPacketItemHelm[pInfo->getHelmetType()];
	MItem* pWeapon		= g_pPacketItemWeapon[pInfo->getWeaponType()];
	MItem* pShield		= g_pPacketItemShield[pInfo->getShieldType()];
	MItem* pMotorcycle	= g_pPacketItemMotorcycle[pInfo->getMotorcycleType()];

	MItem* pShoulder	= g_pPacketItemShoulder[pInfo->getShoulderType()];

	// 색깔정보 설정...
	//pCoat->SetItemOptionList( getHelmetColor() );
	//
	//
	//

	pCreature->SetAddonItem( pCoat );
	pCreature->SetAddonItem( pTrouser );
	pCreature->SetAddonItem( pHelm );
	pCreature->SetAddonItem( pWeapon );
	pCreature->SetAddonItem( pShield );
	pCreature->SetAddonItem( pMotorcycle );

	pCreature->SetAddonItem( pShoulder );

	

	/*
	// 왼손엔 방패
	if (pInfo->getWeaponType()==WEAPON_SWORD_SHIELD || pInfo->getWeaponType()==WEAPON_SHIELD)
	{
		pCreature->SetAddonItem( pShield );

		if (pShield!=NULL)
		{
			pCreature->SetAddonColorSet1( ADDON_LEFTHAND, pInfo->getShieldColor() );
		}
	}
	*/

	//----------------------------------------	
	// 색깔 설정
	//----------------------------------------	
	if (pCoat!=NULL)
	{
		pCreature->SetAddonColorSet2( ADDON_COAT, pInfo->getJacketColor() );
	}

	if(pInfo->getAdvancementLevel()>0) // 승직 캐릭터
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getHairColor () );
	else
		pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );
//	pCreature->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );
	

	if (pTrouser!=NULL)
	{	
		pCreature->SetAddonColorSet2( ADDON_TROUSER, pInfo->getPantsColor() );
	}
	
	pCreature->SetAddonColorSet1( ADDON_TROUSER, pInfo->getSkinColor() );
		
	if (pHelm!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_HELM, pInfo->getHelmetColor() );
		pCreature->SetAddonColorSet2( ADDON_HELM, pInfo->getHelmetColor() );
	}

	if (pWeapon!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_RIGHTHAND, pInfo->getWeaponColor() );	
		pCreature->SetAddonColorSet2( ADDON_RIGHTHAND, pInfo->getWeaponColor() );	
	}

	if (pShield!=NULL)
	{	
		pCreature->SetAddonColorSet1( ADDON_LEFTHAND, pInfo->getShieldColor() );	
		pCreature->SetAddonColorSet2( ADDON_LEFTHAND, pInfo->getShieldColor() );	
	}

	if (pMotorcycle!=NULL)
	{
		pCreature->SetAddonColorSet1( ADDON_MOTOR, pInfo->getMotorcycleColor() );
		pCreature->SetAddonColorSet2( ADDON_MOTOR, pInfo->getMotorcycleColor() );
	}

	//----------------------------------------	
	// 공격 속도 설정
	//----------------------------------------	
	pCreature->SetWeaponSpeed( pInfo->getAttackSpeed() );
}

//-----------------------------------------------------------------------------
// Add Item To Zone
//-----------------------------------------------------------------------------
// bDropping은 방금 떨어지기 시작하는건지...(true)
// 떨어져 있던건지(false)를 결정하는 것이다.
//
// 생성된 Item을 return한다.
// 뭔가 다른 작업을 하기 위해서.. -_-;
//-----------------------------------------------------------------------------
MItem*
AddItemToZone(GCAddItemToZone* pPacket, BOOL bDropping)
{
	//------------------------------------------------
	// Item을 가지고 있다가 Zone으로 떨어뜨리는 경우
	//------------------------------------------------
	// 떨어뜨릴려는 item
	if (g_pPlayer->IsItemCheckBufferDropToZone())
	{
		//---------------------------------------------
		// 떨어뜨릴려는 item
		//---------------------------------------------
		MItem* pItem = g_pPlayer->GetItemCheckBuffer();
	
		if (pItem!=NULL)
		{
			//---------------------------------------------
			// 같은 ID인 경우만 떨어뜨려야 한다....
			//---------------------------------------------
			if (pItem->GetID() == pPacket->getObjectID())
			{	
//				__BEGIN_HELP_EVENT
//					if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
//					{
//						// [도움말] 돈 버릴 때
////						ExecuteHelpEvent( HE_ITEM_DROP_MONEY );	
//					}
//				__END_HELP_EVENT
				

				DEBUG_ADD_FORMAT("[Try To Drop Item to Zone] ID=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
				
				//---------------------------------------------
				// item check buffer를 없애준다.
				//---------------------------------------------
				g_pPlayer->ClearItemCheckBuffer();

				//---------------------------------------------
				// mouse cursor에서 제거
				//---------------------------------------------
				UI_DropItem();

				// 떨어뜨리는 좌표 설정
				pItem->SetPosition( pPacket->getX(), pPacket->getY() );

				// Zone에 Item을 추가한다.
				if (!g_pZone->AddItem( pItem, bDropping ))
				{
					DEBUG_ADD_FORMAT("[Can't Drop Item] ID=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
					
					SAFE_DELETE( pItem );

					return NULL;
				}

				//------------------------------------------------------------------------
				// 기술 아이콘 재설정..
				//------------------------------------------------------------------------
				// 성수, 폭탄
				//------------------------------------------------------------------------
				CheckItemForSkillIcon(pItem);

				DEBUG_ADD_FORMAT("[Drop Item to Zone OK] ID=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
				
				return pItem;
			}
		}
		else
		{
			DEBUG_ADD("[Can't Drop] Item CheckBuffer NULL");
			
			return NULL;
		}
	}

	//------------------------------------------------
	// Item을 생성해서 Zone에 추가한다.
	//------------------------------------------------
	DEBUG_ADD("Create NEW Item");
	
	MItem* pItem;

	if ( pPacket->getItemClass() >= g_pItemTable->GetSize() ||
		(*g_pItemTable)[pPacket->getItemClass()].GetSize() <= pPacket->getItemType() )
	{
		SendBugReport("[PacketFunction::AddItemToZone,%d] %d,%d",__LINE__,pPacket->getItemClass(), pPacket->getItemType() );
		return NULL;
	}

	pItem = MItem::NewItem( (enum ITEM_CLASS)pPacket->getItemClass() );
	pItem->SetItemType( pPacket->getItemType() );
	pItem->SetItemOptionList( pPacket->getOptionType() );

	pItem->SetID( pPacket->getObjectID() );		

	pItem->SetPosition( pPacket->getX(), pPacket->getY() );

	// 개수 설정
	pItem->SetNumber( 1 );

	// durability
	pItem->SetCurrentDurability( pPacket->getDurability() );
	pItem->SetSilver( pPacket->getSilver() );
	pItem->SetGrade( pPacket->getGrade() );
	pItem->SetEnchantLevel( pPacket->getEnchantLevel() );

	//-------------------------------------------------
	// 이름 설정 : 돈인 경우 액수 표시
	//-------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
	{
		char str[128];

		int lowWord = pItem->GetCurrentDurability();
		int highWord = pItem->GetSilver();
		int money = (highWord << 16) | lowWord;
		
		sprintf(str, (*g_pGameStringTable)[STRING_DRAW_ITEM_NAME_MONEY].GetString(), pItem->GetName(), money);

		pItem->SetName( str );
	}


	//------------------------------------------
	// 개수
	//------------------------------------------
	// 총인 경우
	//------------------------------------------
	if (pItem->IsGunItem())
	{
		MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

		// 의미 없음 - -;
		pMagazine->SetID( 0 );

		// 이거는 총에 맞춰서 해줘야된다.
		for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
		{
			pMagazine->SetItemType(	j );

			if (pMagazine->IsInsertToItem( pItem ))
			{
				break;
			}
		}
		
		if(pPacket->getSilver())
			pMagazine->SetItemType( pMagazine->GetItemType()+8);
		// 어예~ 은탄환 하드코딩 by 쑥갓

		// 의미 없음
		pMagazine->ClearItemOption();
	
		// 탄창 개수
		pMagazine->SetNumber( pPacket->getItemNum() );

		//------------------------------------
		// 탄창 설정
		//------------------------------------
		MGunItem* pGunItem = (MGunItem*)pItem;
		pGunItem->SetMagazine( pMagazine );
	}		
	//------------------------------------------
	// 총이 아닌 경우
	//------------------------------------------
	else
	{
		if (!pItem->IsChargeItem())
		{
			pItem->SetNumber( pPacket->getItemNum() );		
		}
	}

	//------------------------------------------
	//
	// Item에 다른 item들이 들어있는 경우
	//
	//------------------------------------------
	if (pPacket->getListNum()!=0)
	{
		DEBUG_ADD_FORMAT("This Item has Sub item(s) : size=%d", pPacket->getListNum());
		
		//------------------------------------------
		// Belt인 경우
		//------------------------------------------
		if (pItem->GetItemClass()==ITEM_CLASS_BELT)
		{
			MBelt* pBelt = (MBelt*)pItem;

			int size = pPacket->getListNum();
					
			for (int i=0; i<size; i++)
			{
				SubItemInfo * pSubItemInfo = pPacket->popFrontListElement();

				if (pSubItemInfo==NULL)
				{
					DEBUG_ADD("[Error] Sub Item is NULL");
				}
				else
				{
					//------------------------------------------
					// Sub Item의 정보를 설정한다.
					//------------------------------------------
					if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
						(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
					{
						SendBugReport("[PacketFunction::AddItemToZone,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
						continue;
					}
					MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
					pSubItem->SetItemType( pSubItemInfo->getItemType() );
					//pItem->SetItemOption( pSubItemInfo->getOptionType() );

					pSubItem->SetID( pSubItemInfo->getObjectID() );		

					pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

					//------------------------------------------
					// Belt의 정해진 slot에 item을 추가시킨다.
					//------------------------------------------
					pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

					delete pSubItemInfo;
				}
			}
		}else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
		{
			MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;
			
			int size = pPacket->getListNum();
			
			for (int i=0; i<size; i++)
			{
				SubItemInfo * pSubItemInfo = pPacket->popFrontListElement();
				
				if (pSubItemInfo==NULL)
				{
					DEBUG_ADD("[Error] Sub Item is NULL");
				}
				else
				{
					//------------------------------------------
					// Sub Item의 정보를 설정한다.
					//------------------------------------------
					if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
						(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
					{
						SendBugReport("[PacketFunction::AddItemToZone,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
						continue;
					}
					MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
					pSubItem->SetItemType( pSubItemInfo->getItemType() );
					//pItem->SetItemOption( pSubItemInfo->getOptionType() );
					
					pSubItem->SetID( pSubItemInfo->getObjectID() );		
					
					pSubItem->SetNumber( pSubItemInfo->getItemNum() );			
					
					//------------------------------------------
					// Belt의 정해진 slot에 item을 추가시킨다.
					//------------------------------------------
					pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );
					
					delete pSubItemInfo;
				}
			}
		}
		else
		{
			DEBUG_ADD_FORMAT("This Item can't have Sub item : class=%d", (int)pItem->GetItemClass());
		}
	}

	//------------------------------------------------	
	// Zone에 추가한다.
	//------------------------------------------------	
	if (!g_pZone->AddItem( pItem, bDropping ))
	{
		//------------------------------------------------	
		// Zone에 추가되지 않은 경우
		//------------------------------------------------	
		DEBUG_ADD_FORMAT("[Can't Add Item] ID=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
		
		// 이전에 있던걸 제거한다.
		TYPE_OBJECTID oldID = g_pZone->GetItemID( pItem->GetX(), pItem->GetY() );

		//------------------------------------------------	
		// 그 자리에는 다른 아이템이 있는 경우...
		//------------------------------------------------	
		if (oldID!=OBJECTID_NULL)
		{
			DEBUG_ADD_FORMAT("Exist Item ID = %d", oldID);
		
			//------------------------------------------------	
			// 다른 아이템인 경우.. 기존에걸 제거한다.
			//------------------------------------------------	
			if (oldID!=pItem->GetID())
			{
				if (g_pZone->RemoveItem( oldID ))
				{
					DEBUG_ADD_FORMAT("Remove Old Item(id=%d) & Add", oldID);
					
					// 다시 추가한다.
					if (!g_pZone->AddItem( pItem, bDropping ))
					{
						DEBUG_ADD("[Can't Re-Add Item]");
						
						// memory 제거
						delete pItem;
					}		
				}
				else
				{
					DEBUG_ADD_FORMAT("[Can't Remove Old Item] id=%d", oldID);
					
					// memory 제거
					delete pItem;
				}		
			}
			//------------------------------------------------	
			// 같은 item인 경우..
			//------------------------------------------------	
			else
			{
				DEBUG_ADD_FORMAT("[Error] Already Exist Same Item");
				
				delete pItem;
			}
		}
		//------------------------------------------------	
		// 그 자리에 아이템이 없다면...
		// 다른 자리에 같은 objectID를 가진 아이템이 있다는 의미이다.
		//------------------------------------------------	
		else
		{
			if (g_pZone->RemoveItem( pPacket->getObjectID() ))
			{
				DEBUG_ADD_FORMAT("Remove Old Item(id=%d) & Add", pPacket->getObjectID());
				
				// 다시 추가한다.
				if (!g_pZone->AddItem( pItem, bDropping ))
				{
					DEBUG_ADD("[Can't Re-Add Item2]");
					
					// memory 제거
					delete pItem;
				}		
			}
			else
			{
				DEBUG_ADD_FORMAT("[Can't Remove Old Item2] id=%d", pPacket->getObjectID());
				
				// memory 제거
				delete pItem;
			}		
		}

		return NULL;
	}

	// [도움말] 아이템 떨어질 때
//	__BEGIN_HELP_EVENT
//		ExecuteHelpEvent( HE_ITEM_APPEAR );	
//	__END_HELP_EVENT

	return pItem;
}

//-----------------------------------------------------------------------------
// SkillToSector
//-----------------------------------------------------------------------------
void
SkillToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, int dir, DWORD delayFrame, MActionResult* pResult)
{
	POINT point = MTopView::MapToPixel(sX, sY);

	//------------------------------------------------------
	//
	//				목표 생성
	//
	//------------------------------------------------------
	// 사용자가 없어서 .. -_-;;
	//------------------------------------------------------
	MEffectTarget* pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nActionInfo].GetSize() );

	pEffectTarget->Set( point.x, point.y, 0, OBJECTID_NULL );

	pEffectTarget->SetDelayFrame( delayFrame );

	// 결과 설정
	pEffectTarget->SetResult( pResult );




	int z = 0;
	//--------------------------------------------------------
	// 공중에서 시작하는 경우
	//--------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsStartSky())
	{
		z	= (*g_pActionInfoTable)[nActionInfo].GetValue();
	}

	//--------------------------------------------------------
	//
	//                   Effect생성		
	//
	//--------------------------------------------------------
	g_pEffectGeneratorTable->Generate(
			point.x, point.y, z,				// 시작 위치
			dir, 		// 방향
			1,					// power
			nActionInfo,		//	ActionInfoTable종류,
			pEffectTarget		// 목표 정보
	);	
}

//-----------------------------------------------------------------------------
// Add Vampire Portal
//-----------------------------------------------------------------------------
void		
AddVampirePortal(
				int serverID,								// 이펙트의 OID
				const char* ownerName,								// 포탈 주인
				TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY,		// 포탈의 좌표
				DWORD delayFrame,							// 포탈의 지속 시간
				
				// 목표 정보
				int zoneID,							// 포탈의 목표 존 ID
				TYPE_SECTORPOSITION zoneX,			// 포탈의 목표 좌표 x
				TYPE_SECTORPOSITION zoneY,			// 포탈의 목표 좌표 y

				bool bStartFromMainNode )
{
	int nActionInfo = RESULT_MAGIC_BLOODY_TUNNEL;

	if ((*g_pActionInfoTable)[nActionInfo].GetSize()!=0)
	{
		//--------------------------------------------------------
		//
		//					Effect 목표 설정
		//
		//--------------------------------------------------------
		MPortalEffectTarget* pEffectTarget = new MPortalEffectTarget( (*g_pActionInfoTable)[nActionInfo].GetSize() );

		pEffectTarget->Set( 0, 0, 0, OBJECTID_NULL );
		pEffectTarget->SetServerID( serverID );
		pEffectTarget->SetOwnerName( ownerName );
		pEffectTarget->SetPortal( zoneID, zoneX, zoneY );

		//--------------------------------------------------------
		// 지속 시간 설정
		//--------------------------------------------------------
		pEffectTarget->SetDelayFrame( delayFrame );		

		//--------------------------------------------------------
		//
		//				Main Node를 찾는다.
		//
		//--------------------------------------------------------
		if (bStartFromMainNode)
		{
			if ((*g_pActionInfoTable)[ nActionInfo ].HasMainNode())
			{
				// main node가 있으면 main node부터 시작하게 한다.
				for (int i=0; i<(*g_pActionInfoTable)[ nActionInfo ].GetMainNode(); i++)
				{
					pEffectTarget->NextPhase();
				}			
			}		
		}
		
		//--------------------------------------------------------
		//
		//					시작 위치를 결정한다.
		//
		//--------------------------------------------------------
		POINT point;	
		int x,y,z, dir = DIRECTION_DOWN;

		point = MTopView::MapToPixel(sX, sY);
		x = point.x;
		y = point.y;
		z = 0;
	
		//--------------------------------------------------------
		//
		//                   Effect생성		
		//
		//--------------------------------------------------------
		g_pEffectGeneratorTable->Generate(
				x,y,z,				// 시작 위치
				dir, 				// 방향
				1,					// power
				nActionInfo,		//	ActionInfoTable종류,
				pEffectTarget		// 목표 정보
		);	
		

		DEBUG_ADD_FORMAT("[AddVampirePortal] ai=%d", nActionInfo);
	}
}

//-----------------------------------------------------------------------------
// Execute ActionInfo From MainNode
//-----------------------------------------------------------------------------
// MainNode부터 기술을 실행한다.
//-----------------------------------------------------------------------------
void
ExecuteActionInfoFromMainNode(
			TYPE_ACTIONINFO nActionInfo,										// 사용 기술 번호
		
			TYPE_SECTORPOSITION startX, TYPE_SECTORPOSITION startY, int startZ, // 기술 사용하는 사람(?) 
			int direction,														// 사용 방향
			
			TYPE_OBJECTID targetID,												// 목표에 대한 정보
			TYPE_SECTORPOSITION targetX, TYPE_SECTORPOSITION targetY, int targetZ, 
			
			DWORD delayFrame,													// 기술의 (남은) 지속 시간		
			
			MActionResult* pActionResult,										// 결과 정보

			bool bStartFromMainNode,											// MainNode부터 시작하는가?

			int sX, int sY)		// 좌표 보정
{

	if (nActionInfo==ACTIONINFO_NULL)
	{
		if (pActionResult!=NULL)
		{
			delete pActionResult;
		}
		return;
	}

	if( nActionInfo >= g_pActionInfoTable->GetSize() )
	{
		DEBUG_ADD_FORMAT("[ExecuteActionInfoFromMainNode] skill type error %d", nActionInfo );
		return;
	}

	if ((*g_pActionInfoTable)[nActionInfo].GetSize()!=0)
	{
		POINT point;	

		//--------------------------------------------------------
		// 목표 위치 Pixel좌표
		//--------------------------------------------------------
		point = MTopView::MapToPixel(targetX, targetY);

		//--------------------------------------------------------
		//
		//					Effect 목표 설정
		//
		//--------------------------------------------------------
		MEffectTarget* pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nActionInfo].GetSize() );

		pEffectTarget->Set( point.x, point.y, targetZ, targetID );

		//--------------------------------------------------------
		// 지속 시간 설정
		//--------------------------------------------------------
		pEffectTarget->SetDelayFrame( delayFrame );		

		// 결과 설정 : 결과 처리는 EffectGenerator에 맡긴다.
		pEffectTarget->SetResult( pActionResult );

		//--------------------------------------------------------
		//
		//				Main Node를 찾는다.
		//
		//--------------------------------------------------------
		if (bStartFromMainNode)
		{
			if ((*g_pActionInfoTable)[ nActionInfo ].HasMainNode())
			{
				// main node가 있으면 main node부터 시작하게 한다.
				for (int i=0; i<(*g_pActionInfoTable)[ nActionInfo ].GetMainNode(); i++)
				{
					pEffectTarget->NextPhase();
				}			
			}		
		}
		
		//--------------------------------------------------------
		//
		//					시작 위치를 결정한다.
		//
		//--------------------------------------------------------
		int x,y,z, dir;

		//--------------------------------------------------------
		// User 위치에서 시작하는 경우
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nActionInfo].IsStartUser())
		{
			point = MTopView::MapToPixel(startX, startY);

			x			= point.x + sX;
			y			= point.y + sY;
			z			= startZ;//+60;			
		}
		//--------------------------------------------------------
		// Target 위치에서 시작하는 경우
		//--------------------------------------------------------
		else if ((*g_pActionInfoTable)[nActionInfo].IsStartTarget())
		{
			x			= point.x + sX;
			y			= point.y + sY;
			z			= startZ;//+60;			
		}

		//--------------------------------------------------------
		// 공중에서 시작하는 경우
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nActionInfo].IsStartSky())
		{
			z	= startZ + (*g_pActionInfoTable)[nActionInfo].GetValue();

			dir	= DIRECTION_DOWN;
		}
		//--------------------------------------------------------
		// 지상에서 시작하는 경우
		//--------------------------------------------------------
		else
		{
			dir	= direction;
		}

		//--------------------------------------------------------
		//
		//                   Effect생성		
		//
		//--------------------------------------------------------
		g_pEffectGeneratorTable->Generate(
				x,y,z,				// 시작 위치
				dir, 				// 방향
				1,					// power
				nActionInfo,		//	ActionInfoTable종류,
				pEffectTarget		// 목표 정보
		);	
		

		DEBUG_ADD_FORMAT("[ExecuteActionInfoFromMainNode] ai=%d", nActionInfo);
	}
	else
	{
		//------------------------------------------------------------
		// 결과를 처리해야하는 시점인가? - 당연하다고 본다 *_*;
		//------------------------------------------------------------				
		if (pActionResult != NULL)
		{					
			// 결과 실행
			pActionResult->Execute();
					
			// 메모리 제거
			delete pActionResult;		
		}				
	}
}

//-----------------------------------------------------------------------------
// Get Position  Map  To  Screen
//-----------------------------------------------------------------------------
// map좌표 (sX,sY)를 화면 상의 좌표로 바꾼다.
//-----------------------------------------------------------------------------
POINT			
ConvertPositionMapToScreen(int sX, int sY)
{
	POINT point = g_pTopView->MapToScreen(sX, sY);

	return point;
}



//-----------------------------------------------------------------------------
// Set Inventory Info
//-----------------------------------------------------------------------------
// Player의 Inventory 정보를 설정한다.
//-----------------------------------------------------------------------------
void
SetInventoryInfo(InventoryInfo* pInventoryInfo)
{
	DEBUG_ADD( "Set Inventory Info" );
	
	//--------------------------------------------------
	// Inventory 초기화
	//--------------------------------------------------	
	g_pInventory->Init(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	
	if (pInventoryInfo!=NULL)
	{
		int itemNum = pInventoryInfo->getListNum();

		//--------------------------------------------------
		// Inventory에 아이템들을 추가한다.
		//--------------------------------------------------
		for (int i=0; i<itemNum; i++)
		{
			// 외부에서 지워줘야 한다.
			InventorySlotInfo * pSlotInfo = pInventoryInfo->popFrontListElement();

			if( pSlotInfo == NULL )
				continue;

			if( pSlotInfo->getItemClass() >= g_pItemTable->GetSize() ||
				pSlotInfo->getItemType() >= (*g_pItemTable)[pSlotInfo->getItemClass()].GetSize() )
			{
				SendBugReport("[%s] %d,%d",__FILE__, pSlotInfo->getItemClass(), pSlotInfo->getItemType());
				delete pSlotInfo;
				continue;
			}

			//------------------------------------------------
			// Item을 생성해서 Inventory에 추가한다.
			//------------------------------------------------
			MItem* pItem = MItem::NewItem( (ITEM_CLASS)pSlotInfo->getItemClass() );
			
			// object ID
			pItem->SetID( pSlotInfo->getObjectID() );

			// type / option
			pItem->SetItemType(	pSlotInfo->getItemType() );
			pItem->SetItemOptionList( pSlotInfo->getOptionType() );

			// inventory에서의 좌표
			pItem->SetGridXY( pSlotInfo->getInvenX(), pSlotInfo->getInvenY() );

			//------------------------------------------
			// 개수
			//------------------------------------------
			// 총인 경우
			//------------------------------------------
			if (pItem->IsGunItem())
			{
				MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

				// 의미 없음 - -;
				pMagazine->SetID( 0 );

				// 이거는 총에 맞춰서 해줘야된다.
				for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
				{
					pMagazine->SetItemType(	j );

					if (pMagazine->IsInsertToItem( pItem ))
					{
						break;
					}
				}

				if(pSlotInfo->getSilver())
					pMagazine->SetItemType( pMagazine->GetItemType()+8);
				// 어예~ 은탄환 하드코딩 by 쑥갓

				// 의미 없음
				pMagazine->ClearItemOption();
			
				// 탄창 개수
				pMagazine->SetNumber( pSlotInfo->getItemNum() );

				//------------------------------------
				// 탄창 설정
				//------------------------------------
				MGunItem* pGunItem = (MGunItem*)pItem;
				pGunItem->SetMagazine( pMagazine );
			}		
			//------------------------------------------
			// 총이 아닌 경우
			//------------------------------------------
			else
			{
				pItem->SetNumber( pSlotInfo->getItemNum() );
			}


			//------------------------------------------
			//
			// Item에 다른 item들이 들어있는 경우
			//
			//------------------------------------------
			if (pSlotInfo->getListNum()!=0)
			{
				DEBUG_ADD_FORMAT("This Item has Sub item(s) : size=%d", pSlotInfo->getListNum());
				
				//------------------------------------------
				// Belt인 경우
				//------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_BELT)
				{
					MBelt* pBelt = (MBelt*)pItem;

					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();

						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetInventoryInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}
							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOption( pSubItemInfo->getOptionType() );

							pSubItem->SetID( pSubItemInfo->getObjectID() );		

							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

							delete pSubItemInfo;
						}
					} 
				}
				else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
				{
					MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;
					
					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();
						
						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetInventoryInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}
							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOption( pSubItemInfo->getOptionType() );
							
							pSubItem->SetID( pSubItemInfo->getObjectID() );		
							
							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			
							
							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );
							
							delete pSubItemInfo;
						}
					}
				}
				else
				{
					DEBUG_ADD_FORMAT("This Item can't have Sub item : class=%d", (int)pItem->GetItemClass());
				}
			}


			// main색깔
			pItem->SetItemColorSet( pSlotInfo->getMainColor() );

			// 현재 내구성
			pItem->SetCurrentDurability( pSlotInfo->getDurability() );
			pItem->SetSilver( pSlotInfo->getSilver() );
			pItem->SetGrade( pSlotInfo->getGrade() );
			pItem->SetEnchantLevel( pSlotInfo->getEnchantLevel() );

			//---------------------------------------------
			// item을 inventory에 넣는다.
			//---------------------------------------------
			if (g_pInventory->AddItem( pItem, pItem->GetGridX(), pItem->GetGridY() ))
			{				
			}	
			else
			{
				// item을 inventory에 추가할 수 없는 경우
				DEBUG_ADD_FORMAT("[Error] Can't Add Item to Inventory. id=%d, cl=%d, tp=%d, xy=(%d,%d)", 					
												(int)pItem->GetID(),
												(int)pItem->GetItemClass(),
												(int)pItem->GetItemType(),
												(int)pItem->GetGridX(),
												(int)pItem->GetGridY());
				
				delete pItem;
			}

			delete pSlotInfo;
		}
	}

	//---------------------------------------------------------
	// 사용 가능 여부 체크
	//---------------------------------------------------------
	g_pInventory->CheckAffectStatusAll();

	//---------------------------------------------------------
	// inventory에 붙은 Effect제거
	//---------------------------------------------------------
	if (g_pInventoryEffectManager!=NULL)
	{
		g_pInventoryEffectManager->Release();
	}
}

//--------------------------------------------------------------------------------
// Gear Information
//--------------------------------------------------------------------------------
// Player의 Gear에 대한 info를 설정한다.
// SetGearInfo()를 하기 전에..
// g_pPlayer가 Slayer인지 Vampire인지가 결정이 나야 한다.
// 그래야지.. g_SlayerGear인지.. g_VampireGear인지를 알 수가 있다.
//--------------------------------------------------------------------------------
void
SetGearInfo(GearInfo* pGearInfo, BloodBibleSignInfo* pBloodBibleInfo)
{
	DEBUG_ADD( "Set Gear Info" );

	MPlayerGear *pGear = NULL;

	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
		g_pSlayerGear->Init();
		break;

	case RACE_VAMPIRE:
		g_pVampireGear->Init();
		break;

	case RACE_OUSTERS:
		g_pOustersGear->Init();
		break;
	}

	if (pGearInfo!=NULL)
	{
		int itemNum = pGearInfo->getListNum();
				
		bool bAdd;
		
		for (int i=0; i<itemNum; i++)
		{
			// 외부에서 지워줘야 한다.
			GearSlotInfo * pSlotInfo = pGearInfo->popFrontListElement();

			if( pSlotInfo->getItemClass() >= g_pItemTable->GetSize() ||
				pSlotInfo->getItemType() >= (*g_pItemTable)[pSlotInfo->getItemClass()].GetSize() )
			{
				SendBugReport("[%s,%d] %d,%d",__FILE__, __LINE__,pSlotInfo->getItemClass(), pSlotInfo->getItemType());
				delete pSlotInfo;
				continue;
			}
					
			//------------------------------------------------
			// Item을 생성해서 Gear에 추가한다.
			//------------------------------------------------
			MItem* pItem = MItem::NewItem( (ITEM_CLASS)pSlotInfo->getItemClass() );
		
			// object ID
			pItem->SetID( pSlotInfo->getObjectID() );

			// type / option
			pItem->SetItemType(	pSlotInfo->getItemType() );
			pItem->SetItemOptionList( pSlotInfo->getOptionType() );

			//------------------------------------------
			// 개수
			//------------------------------------------
			// 총인 경우
			//------------------------------------------
			if (pItem->IsGunItem())
			{
				MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

				// 의미 없음 - -;
				pMagazine->SetID( 0 );

				// 이거는 총에 맞춰서 해줘야된다.
				for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
				{
					pMagazine->SetItemType(	j );

					if (pMagazine->IsInsertToItem( pItem ))
					{
						break;
					}
				}

				if(pSlotInfo->getSilver())
					pMagazine->SetItemType( pMagazine->GetItemType()+8);
				// 어예~ 은탄환 하드코딩 by 쑥갓

				// 의미 없음
				pMagazine->ClearItemOption();
			
				// 탄창 개수
				pMagazine->SetNumber( pSlotInfo->getItemNum() );

				//------------------------------------
				// 탄창 설정
				//------------------------------------
				MGunItem* pGunItem = (MGunItem*)pItem;
				pGunItem->SetMagazine( pMagazine );
			}		
			//------------------------------------------
			// 총이 아닌 경우
			//------------------------------------------
			else
			{
				pItem->SetNumber( pSlotInfo->getItemNum() );
				pItem->SetSilver( pSlotInfo->getSilver() );
			}

			pItem->SetGrade( pSlotInfo->getGrade() );
			// gear에서의 좌표
			pItem->SetItemSlot( pSlotInfo->getSlotID() );

			// main색깔
			pItem->SetItemColorSet( pSlotInfo->getMainColor() );

			// 현재 내구성
			pItem->SetCurrentDurability( pSlotInfo->getDurability() );			
			pItem->SetEnchantLevel( pSlotInfo->getEnchantLevel() );		

			//------------------------------------------
			//
			// Item에 다른 item들이 들어있는 경우
			//
			//------------------------------------------
			if (pSlotInfo->getListNum()!=0)
			{
				DEBUG_ADD_FORMAT("This Item has Sub item(s) : size=%d", pSlotInfo->getListNum());
				
				//------------------------------------------
				// Belt인 경우
				//------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_BELT)
				{
					MBelt* pBelt = (MBelt*)pItem;

					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();

						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetGearInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}
							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOption( pSubItemInfo->getOptionType() );

							pSubItem->SetID( pSubItemInfo->getObjectID() );		

							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

							delete pSubItemInfo;
						}
					}
				}
				else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
				{
					MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;

					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();

						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetGearInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}
							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOption( pSubItemInfo->getOptionType() );

							pSubItem->SetID( pSubItemInfo->getObjectID() );		

							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

							delete pSubItemInfo;
						}
					}
				}
				else
				{
					DEBUG_ADD_FORMAT("This Item can't have Sub item : class=%d", (int)pItem->GetItemClass());
				}
			}

			//---------------------------------------------
			// item을 gear에 넣는다.
			//---------------------------------------------
			// slayer / vampire에 따라서 gear가 다르다.
			//---------------------------------------------
			switch(g_pPlayer->GetRace())
			{
			case RACE_SLAYER:
				bAdd = g_pSlayerGear->AddItem( pItem, (MSlayerGear::GEAR_SLAYER)pItem->GetItemSlot() );			
				break;

			case RACE_VAMPIRE:
				bAdd = g_pVampireGear->AddItem( pItem, (MVampireGear::GEAR_VAMPIRE)pItem->GetItemSlot() );
				break;
				
			case RACE_OUSTERS:
				bAdd = g_pOustersGear->AddItem( pItem, (MOustersGear::GEAR_OUSTERS)pItem->GetItemSlot() );
				break;
			}

			//---------------------------------------------
			// 복장 바꾼다.
			//---------------------------------------------
			g_pPlayer->SetAddonItem( pItem );

			//---------------------------------------------
			// Gear 추가할 수 없는 경우
			//---------------------------------------------
			if (!bAdd)		
			{
				// item을 Gear에 추가할 수 없는 경우
				DEBUG_ADD_FORMAT("[Error] Can't Add Item to Gear. id=%d, cl=%d, tp=%d, slot=%d", 
												(int)pItem->GetID(),
												(int)pItem->GetItemClass(),
												(int)pItem->GetItemType(),
												(int)pItem->GetItemSlot());
				
				delete pItem;
			}

			delete pSlotInfo;
		}
		
		SetBloodBibleSlot(pBloodBibleInfo);
	}

	pGear = g_pPlayer->GetGear();
	pGear->CheckAffectStatusAll();				

	//----------------------------------------------------
	// Skill 체크
	//----------------------------------------------------
	g_pSkillAvailable->SetAvailableSkills();
	
	//----------------------------------------------
	// 수치 계산을 한다.
	//----------------------------------------------
	g_pPlayer->CalculateStatus();

	//g_char_slot_ingame.CC		= g_pPlayer->GetCARRYWEIGHT();
	//g_char_slot_ingame.DAM		= g_pPlayer->GetDAM();
	//g_char_slot_ingame.AC		= g_pPlayer->GetAC();
	g_char_slot_ingame.TOHIT	= g_pPlayer->GetTOHIT();

}
// 2004, 10, 21, sobeit add start - blood bible slot
void SetBloodBibleSlot(BloodBibleSignInfo* pBloodBibleInfo)
{
	if(NULL == pBloodBibleInfo)
		return;

	std::vector<WORD>	TempSignList = pBloodBibleInfo->getList();
	for(int i = 0; i< HOLYLAND_BONUS_MAX; i++)
		g_abHolyLandBonusSkills[i] = false;
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
		{
			for(int i = 0; i<g_pSlayerGear->GetBloodBibleOpenSlot() ; i++)
			{
				MItem* pItem = g_pSlayerGear->RemoveItem((MSlayerGear::GEAR_SLAYER)(MSlayerGear::GEAR_SLAYER_BLOOD_BIBLE1+i));
				SAFE_DELETE(pItem);
			}

			g_pSlayerGear->SetBloodBibleOpenSlot(pBloodBibleInfo->getOpenNum());

			for(i = 0; i<TempSignList.size() ; i++)
			{
				MItem* pItem = MItem::NewItem( ITEM_CLASS_BLOOD_BIBLE_SIGN );
				pItem->SetItemType(	TempSignList[i] );
				pItem->SetID(i);
				g_pSlayerGear->AddItem(pItem, (MSlayerGear::GEAR_SLAYER)(MSlayerGear::GEAR_SLAYER_BLOOD_BIBLE1+i));
				g_abHolyLandBonusSkills[TempSignList[i]] = true;
			}
		}
		break;

	case RACE_VAMPIRE:
		{
			for(int i = 0; i<g_pVampireGear->GetBloodBibleOpenSlot() ; i++)
			{
				MItem* pItem = g_pVampireGear->RemoveItem((MVampireGear::GEAR_VAMPIRE)(MVampireGear::GEAR_VAMPIRE_BLOOD_BIBLE1+i));
				SAFE_DELETE(pItem);
			}
			g_pVampireGear->SetBloodBibleOpenSlot(pBloodBibleInfo->getOpenNum());

			for(i = 0; i<TempSignList.size() ; i++)
			{
				MItem* pItem = MItem::NewItem( ITEM_CLASS_BLOOD_BIBLE_SIGN );
				pItem->SetItemType(	TempSignList[i] );
				pItem->SetID(i);
				g_pVampireGear->AddItem(pItem, (MVampireGear::GEAR_VAMPIRE)(MVampireGear::GEAR_VAMPIRE_BLOOD_BIBLE1+i));
				g_abHolyLandBonusSkills[TempSignList[i]] = true;
			}
		}
		break;

	case RACE_OUSTERS:
		{
			for(int i = 0; i<g_pOustersGear->GetBloodBibleOpenSlot() ; i++)
			{
				MItem* pItem = g_pOustersGear->RemoveItem((MOustersGear::GEAR_OUSTERS)(MOustersGear::GEAR_OUSTERS_BLOOD_BIBLE1+i));
				SAFE_DELETE(pItem);
			}
			g_pOustersGear->SetBloodBibleOpenSlot(pBloodBibleInfo->getOpenNum());

			for(i = 0; i<TempSignList.size() ; i++)
			{ 
				MItem* pItem = MItem::NewItem( ITEM_CLASS_BLOOD_BIBLE_SIGN );
				pItem->SetItemType(	TempSignList[i] );
				pItem->SetID(i);
				g_pOustersGear->AddItem(pItem, (MOustersGear::GEAR_OUSTERS)(MOustersGear::GEAR_OUSTERS_BLOOD_BIBLE1+i));
				g_abHolyLandBonusSkills[TempSignList[i]] = true;
			}
		}
		break;
	}

	

}
// 2004, 10, 21, sobeit add end - blood bible slot
//--------------------------------------------------------------------------------
// Extra Information
//--------------------------------------------------------------------------------
// mouse에 들고 있는 item에 대한 정보 설정
//--------------------------------------------------------------------------------
void
SetExtraInfo(ExtraInfo* pExtraInfo)
{
	DEBUG_ADD( "Set Extra Info" );
	
	//------------------------------------------------------
	// 들고 있는 item을 없앤다.
	//------------------------------------------------------
	MItem* pCurrentItem = gpC_mouse_pointer->GetPickUpItem();

	if (pCurrentItem!=NULL)
	{
		delete pCurrentItem;
		UI_DropItem();
	}

	if (pExtraInfo!=NULL)
	{
		int itemNum = pExtraInfo->getListNum();

		//------------------------------------------------------
		// 당연히 1개밖에 없겠지만.. - -;;
		//------------------------------------------------------
		for (int i=0; i<itemNum; i++)
		{
			ExtraSlotInfo * pSlotInfo = pExtraInfo->popFrontListElement();

			//------------------------------------------------
			// Item을 생성해서 mouse에 들린다.
			//------------------------------------------------
			if ( pSlotInfo->getItemClass() >= g_pItemTable->GetSize() ||
				(*g_pItemTable)[pSlotInfo->getItemClass()].GetSize() <= pSlotInfo->getItemType() )
			{
				SendBugReport("[PacketFunction::SetExtraInfo,%d] %d,%d",__LINE__,pSlotInfo->getItemClass(), pSlotInfo->getItemType() );
				return;
			}
			MItem* pItem = MItem::NewItem( (ITEM_CLASS)pSlotInfo->getItemClass() );
			
			// object ID
			pItem->SetID( pSlotInfo->getObjectID() );

			// type / option
			pItem->SetItemType(	pSlotInfo->getItemType() );
			pItem->SetItemOptionList( pSlotInfo->getOptionType() );

			//------------------------------------------
			// 개수
			//------------------------------------------
			// 총인 경우
			//------------------------------------------
			if (pItem->IsGunItem())
			{
				MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

				// 의미 없음 - -;
				pMagazine->SetID( 0 );

				// 이거는 총에 맞춰서 해줘야된다.
				for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
				{
					pMagazine->SetItemType(	j );

					if (pMagazine->IsInsertToItem( pItem ))
					{
						break;
					}
				}

				if(pSlotInfo->getSilver())
					pMagazine->SetItemType( pMagazine->GetItemType()+8);
				// 어예~ 은탄환 하드코딩 by 쑥갓
				
				// 의미 없음
				pMagazine->ClearItemOption();
			
				// 탄창 개수
				pMagazine->SetNumber( pSlotInfo->getItemNum() );

				//------------------------------------
				// 탄창 설정
				//------------------------------------
				MGunItem* pGunItem = (MGunItem*)pItem;
				pGunItem->SetMagazine( pMagazine );
			}		
			//------------------------------------------
			// 총이 아닌 경우
			//------------------------------------------
			else
			{
				pItem->SetNumber( pSlotInfo->getItemNum() );
			}


			//------------------------------------------
			//
			// Item에 다른 item들이 들어있는 경우
			//
			//------------------------------------------
			if (pSlotInfo->getListNum()!=0)
			{
				DEBUG_ADD_FORMAT("This Item has Sub item(s) : size=%d", pSlotInfo->getListNum());
				
				//------------------------------------------
				// Belt인 경우
				//------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_BELT)
				{
					MBelt* pBelt = (MBelt*)pItem;

					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();

						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetExtraInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}
							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOptionList( pSubItemInfo->getOptionType() );

							pSubItem->SetID( pSubItemInfo->getObjectID() );		

							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

							delete pSubItemInfo;
						}
					}
				}
				else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
				{
					MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;
					
					int size = pSlotInfo->getListNum();
					
					for (int i=0; i<size; i++)
					{
						SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();
						
						if (pSubItemInfo==NULL)
						{
							DEBUG_ADD("[Error] Sub Item is NULL");
						}
						else
						{
							//------------------------------------------
							// Sub Item의 정보를 설정한다.
							//------------------------------------------
							if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
								(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
							{
								SendBugReport("[PacketFunction::SetExtraInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
								return;
							}

							MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
							pSubItem->SetItemType( pSubItemInfo->getItemType() );
							//pItem->SetItemOptionList( pSubItemInfo->getOptionType() );
							
							pSubItem->SetID( pSubItemInfo->getObjectID() );		
							
							pSubItem->SetNumber( pSubItemInfo->getItemNum() );			
							
							//------------------------------------------
							// Belt의 정해진 slot에 item을 추가시킨다.
							//------------------------------------------
							pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );
							
							delete pSubItemInfo;
						}
					}
				}
				else
				{
					DEBUG_ADD_FORMAT("This Item can't have Sub item : class=%d", (int)pItem->GetItemClass());
				}
			}


			// main색깔
			pItem->SetItemColorSet( pSlotInfo->getMainColor() );

			// 현재 내구성
			pItem->SetCurrentDurability( pSlotInfo->getDurability() );
			pItem->SetSilver( pSlotInfo->getSilver() );
			pItem->SetGrade( pSlotInfo->getGrade() );
			pItem->SetEnchantLevel( pSlotInfo->getEnchantLevel() );

			//------------------------------------------
			// Item을 든다.
			//------------------------------------------
			UI_PickUpItem( pItem );

			delete pSlotInfo;
		}
	}

}

//--------------------------------------------------------------------------------
// Set EffectInfo
//--------------------------------------------------------------------------------
// Player에게 붙어 있는 Effect를 설정해준다.
//
// delayedFrame은 ..
// 처리(특히 loading)를 한다고 client에서 소요된 시간만큼을 
// 빼주는 것이다.
//--------------------------------------------------------------------------------
void
SetEffectInfo(MCreature* pCreature, EffectInfo* pEffectInfo, int delayedFrame)
{
	DEBUG_ADD_FORMAT( "Set Effect Info. id=%d", pCreature->GetID() );
	
	if (pEffectInfo!=NULL)
	{
		//--------------------------------------------------
		// 각각의 Effect Status를 설정한다.
		//--------------------------------------------------
		for (int i=0; i<pEffectInfo->getListNum(); i++)
		{
			EFFECTSTATUS	status		= (EFFECTSTATUS)pEffectInfo->popFrontListElement();
			DWORD			duration	= pEffectInfo->popFrontListElement();

//			if( status == EFFECTSTATUS_VIEW_HP )
//			{
//				int a = 0;
//			}
			//--------------------------------------------------
			// 종류에 따라서..
			//--------------------------------------------------
			switch (status)
			{
				//--------------------------------------------------
				// 흡혈 당한거
				//--------------------------------------------------
				case EFFECTSTATUS_BLOOD_DRAIN :
					duration *= 10;
				break;

				//--------------------------------------------------
				// Coma상태
				//--------------------------------------------------
				case EFFECTSTATUS_COMA :
					pCreature->SetCorpse();
				break;

				case EFFECTSTATUS_BACK_STAB_3 :
				case EFFECTSTATUS_ICE_OF_SOUL_STONE :
				case EFFECTSTATUS_TRAP_TRIGGERED :
				case EFFECTSTATUS_TRAPPED :
				case EFFECTSTATUS_GROUND_ELEMENTAL_CENTER:
				{
					TYPE_ACTIONINFO		ActionInfo = (*g_pEffectStatusTable)[ status ].ActionInfo;
					
					if( ActionInfo != ACTIONINFO_NULL )
					{						
						if( status == EFFECTSTATUS_TRAP_TRIGGERED || status == EFFECTSTATUS_TRAPPED )
							duration = ConvertDurationToFrame( duration );
						else
							duration = 18;					

						if( status == EFFECTSTATUS_ICE_OF_SOUL_STONE )
							duration = 19;
						if( status == EFFECTSTATUS_GROUND_ELEMENTAL_CENTER)
							duration = 0xFFFF;
						
						ExecuteActionInfoFromMainNode(
							ActionInfo,						
							pCreature->GetX(), pCreature->GetY(), 0,
							pCreature->GetDirection(),						
							pCreature->GetID(),
							pCreature->GetX(), pCreature->GetY(), 0,						
							duration,						
							NULL, status == EFFECTSTATUS_GROUND_ELEMENTAL_CENTER
							);
					}
				}
				break;
			}

			int				delayFrame	= ConvertDurationToFrame( duration );


			// loading시간을 뺀 delay frame
			delayFrame -= delayedFrame;

			// 시간이 남아 있는 경우에만...
			if (delayFrame > 0)
			{
				pCreature->AddEffectStatus( status, delayFrame );
			}
		}
	}
}

//--------------------------------------------------------------------------------
// Get FaceStyle
//--------------------------------------------------------------------------------
int 
UI_GetFaceStyle(bool bMale, int faceStyle)
{
	if (bMale)
	{
		switch (faceStyle)
		{
			case HAIR_STYLE1 :
				return M_FACE1;
			break;
			case HAIR_STYLE2 :
				return M_FACE2;
			break;
			case HAIR_STYLE3 :
				return M_FACE3;
			break;
		}
	}
	else
	{
		switch (faceStyle)
		{
			case HAIR_STYLE1 :
				return W_FACE1;
			break;
			case HAIR_STYLE2 :
				return W_FACE2;
			break;
			case HAIR_STYLE3 :
				return W_FACE3;
			break;
		}
	}

	return 0;
}


//--------------------------------------------------------------------------------
// Set PCSlayerInfo
//--------------------------------------------------------------------------------
// Player에게 slayer 정보를 설정한다.
//--------------------------------------------------------------------------------
void
SetPCSlayerInfo(PCSlayerInfo2* pInfo)
{
	DEBUG_ADD( "Set Slayer Info" );
	
	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정
	//--------------------------------------------------
	// slayer남 / 여
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

	// 피부색
	g_pPlayer->SetBodyColor1( pInfo->getSkinColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	// 머리			
	SetAddonToSlayer( g_pPlayer, pInfo );

	//--------------------------------------------------
	// 내 길드 설정
	//--------------------------------------------------
	int guildID = pInfo->getGuildID();

	g_pPlayer->SetGuildNumber( guildID );

	if (pInfo!=NULL)
	{
		g_pUserInformation->GuildName = pInfo->getGuildName().c_str();
	}
	else
	{
		g_pUserInformation->GuildName.Release();
	}
	g_pUserInformation->GuildGrade = pInfo->getGuildMemberRank();

	//--------------------------------------------------
	// 정보창에서 출력할 때 필요한 정보..
	//--------------------------------------------------
	g_char_slot_ingame.sz_name			= g_pUserInformation->CharacterID.GetString();
	if(g_pUserInformation->GuildName.GetString() == NULL)
		g_char_slot_ingame.sz_guild_name	= "";
	else
		g_char_slot_ingame.sz_guild_name	= g_pUserInformation->GuildName.GetString();

	g_char_slot_ingame.GUILD_ID			= guildID;
	g_char_slot_ingame.GUILD_GRADE		= g_pUserInformation->GuildGrade;

	g_char_slot_ingame.GRADE		= pInfo->getRank();
//	int accum = 0;
//	accum = g_pExperienceTable->GetRankInfo(g_char_slot_ingame.GRADE, g_pPlayer->GetRace()).AccumExp;
//	g_char_slot_ingame.GRADE_EXP_REMAIN = accum-pInfo->getRankExp();
	g_char_slot_ingame.GRADE_EXP_REMAIN = pInfo->getRankExp();

	g_char_slot_ingame.Race = RACE_SLAYER;
	g_char_slot_ingame.bl_female = !g_pPlayer->IsMale();

	g_char_slot_ingame.hair_color = pInfo->getHairColor();	
	g_char_slot_ingame.skin_color = pInfo->getSkinColor();

	g_pUserInformation->HairColor = pInfo->getHairColor();	
	g_pUserInformation->SkinColor = pInfo->getSkinColor();

	g_pGuildInfoMapper->SetGuildName( pInfo->getGuildID(), g_char_slot_ingame.sz_guild_name );

	//--------------------------------------------------
	// Effect 정보 제거
	//--------------------------------------------------
	g_char_slot_ingame.STATUS.clear();
	
	// 얼굴 설정
	if (g_pPlayer->IsMale())
	{
		g_char_slot_ingame.man_info.face = (CHAR_MAN)UI_GetFaceStyle(true, g_pUserInformation->FaceStyle);
	}
	else
	{
		g_char_slot_ingame.woman_info.face = (CHAR_WOMAN)UI_GetFaceStyle(false, g_pUserInformation->FaceStyle);
	}
	
	// 
	//g_pUserInformation->FaceStyle = pInfo->getHairStyle();
	
	// 기본 복장
	/*
	MItem* pCoat = MItem::NewItem( ITEM_CLASS_COAT );
	MItem* pTrouser = MItem::NewItem( ITEM_CLASS_TROUSER );
	
	pCoat->SetItemType( 0 );				
	pTrouser->SetItemType( 0 );

	g_pPlayer->SetAddonItem( pCoat );	
	g_pPlayer->SetAddonItem( pTrouser);

	delete pCoat;
	delete pTrouser;
	*/


	//--------------------------------------------------
	// 현재 상태 설정
	//--------------------------------------------------		
	int maxHP	= pInfo->getHP( ATTR_MAX );
	int HP		= pInfo->getHP( ATTR_CURRENT );
	int maxMP	= pInfo->getMP( ATTR_MAX );
	int MP		= pInfo->getMP( ATTR_CURRENT );
	int STR_CUR	= pInfo->getSTR( ATTR_CURRENT );
	int DEX_CUR	= pInfo->getDEX( ATTR_CURRENT );
	int INT_CUR	= pInfo->getINT( ATTR_CURRENT );
	int STR_MAX	= pInfo->getSTR( ATTR_MAX );
	int DEX_MAX	= pInfo->getDEX( ATTR_MAX );
	int INT_MAXX	= pInfo->getINT( ATTR_MAX );
	int STR_PURE	= pInfo->getSTR( ATTR_BASIC );
	int DEX_PURE	= pInfo->getDEX( ATTR_BASIC );
	int INT_PURE	= pInfo->getINT( ATTR_BASIC );
	int Fame	= pInfo->getFame();
	int STR_EXP_REMAIN	= pInfo->getSTRExp();
	int DEX_EXP_REMAIN	= pInfo->getDEXExp();
	int INT_EXP_REMAIN	= pInfo->getINTExp();
	//int Notority = pInfo->getNotoriety();

	//pInfo->getSkillDomainLevel( d );
	//pInfo->getSkillDomainExp( d );
	DEBUG_ADD( "Set Slayer Info : SetStatus" );
	
	g_pPlayer->SetStatus( MODIFY_MAX_HP, maxHP);
	g_pPlayer->SetStatus( MODIFY_MAX_MP, maxMP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_HP, HP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_MP, MP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_STR, STR_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_DEX, DEX_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_INT, INT_CUR);
	g_pPlayer->SetStatus( MODIFY_BASIC_STR, STR_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_DEX, DEX_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_INT, INT_PURE);
	g_pPlayer->SetStatus( MODIFY_MAX_STR, STR_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_DEX, DEX_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_INT, INT_MAXX);
	g_pPlayer->SetStatus( MODIFY_STR_EXP_REMAIN, STR_EXP_REMAIN);
	g_pPlayer->SetStatus( MODIFY_DEX_EXP_REMAIN, DEX_EXP_REMAIN);
	g_pPlayer->SetStatus( MODIFY_INT_EXP_REMAIN, INT_EXP_REMAIN);	
	g_pPlayer->SetStatus( MODIFY_FAME, Fame);
	g_pPlayer->SetStatus( MODIFY_ALIGNMENT, pInfo->getAlignment() );
	g_pPlayer->SetStatus( MODIFY_RANK, pInfo->getRank() );
	g_pPlayer->SetStatus( MODIFY_RANK_EXP_REMAIN, pInfo->getRankExp() );
	g_pPlayer->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, pInfo->getAdvancementLevel() );

	// 2005, 1, 18, sobeit add start - 승직 슬레는 보너스 포인트가 있다.
	int Bonus	= pInfo->getAttrBonus();
	g_pPlayer->SetStatus( MODIFY_BONUS_POINT, Bonus);
	// 2005, 1, 18, sobeit add end - 승직 슬레는 보너스 포인트가 있다.

	//g_pPlayer->SetStatus( MODIFY_ATTACK_SPEED,  pInfo->getAttackSpeed() );
	
	DEBUG_ADD( "Set Slayer Info: Set g_char_slot_ingame" );
	
	//--------------------------------------------------
	// UI에 정보 설정
	//--------------------------------------------------
	//UI_SetHP( HP, maxHP );
	//UI_SetMP( MP, maxMP );
	g_char_slot_ingame.level = 1;//g_pPlayer->GetVampExp();

	g_char_slot_ingame.alignment = (ALIGNMENT)ConvertAlignment(g_pPlayer->GetAlignment());

	g_char_slot_ingame.STR_CUR = STR_CUR;
	g_char_slot_ingame.DEX_CUR = DEX_CUR;
	g_char_slot_ingame.INT_CUR = INT_CUR;
	g_char_slot_ingame.STR_MAX = STR_MAX;
	g_char_slot_ingame.DEX_MAX = DEX_MAX;
	g_char_slot_ingame.INT_MAXX= INT_MAXX;
	g_char_slot_ingame.STR_PURE = STR_PURE;
	g_char_slot_ingame.DEX_PURE = DEX_PURE;
	g_char_slot_ingame.INT_PURE = INT_PURE;
	//g_char_slot_ingame.DAM =g_pPlayer->GetDAM();
	//g_char_slot_ingame.AC =g_pPlayer->GetAC();
	g_char_slot_ingame.TOHIT =g_pPlayer->GetTOHIT();
	g_char_slot_ingame.HP =g_pPlayer->GetHP();
	g_char_slot_ingame.MP =g_pPlayer->GetMP();
	g_char_slot_ingame.HP_MAX =g_pPlayer->GetMAX_HP();
	g_char_slot_ingame.MP_MAX =g_pPlayer->GetMAX_MP();
	//g_char_slot_ingame.CC =g_pPlayer->GetCARRYWEIGHT();
	g_char_slot_ingame.FAME =g_pPlayer->GetFAME();
//	g_char_slot_ingame.NOTERITY =g_pPlayer->GetNotoriety();

	g_char_slot_ingame.STR_EXP_REMAIN	= STR_EXP_REMAIN;
	g_char_slot_ingame.DEX_EXP_REMAIN	= DEX_EXP_REMAIN;
	g_char_slot_ingame.INT_EXP_REMAIN	= INT_EXP_REMAIN;

	if(pInfo->getAdvancementLevel() > 0)
		g_char_slot_ingame.EXP_REMAIN = pInfo->getAdvancementGoalExp();

	
	//--------------------------------------------------		
	// Domain Level 설정
	//--------------------------------------------------
	DEBUG_ADD( "Set Slayer Info : Domain" );
	
	const int numDomains = 5;
	int domains[numDomains] = 
	{ 
		SKILLDOMAIN_BLADE,
		SKILLDOMAIN_SWORD,
		SKILLDOMAIN_GUN,
		SKILLDOMAIN_HEAL,
		SKILLDOMAIN_ENCHANT,			
	};

	for (int i=0; i<numDomains; i++)
	{
		int domain = domains[i];

		(*g_pSkillManager)[domain].SetDomainLevel( pInfo->getSkillDomainLevel( (SkillDomain)domain ) );
		(*g_pSkillManager)[domain].SetDomainExpRemain( pInfo->getSkillDomainExp( (SkillDomain)domain ) );
	}	
	
	DEBUG_ADD( "Set Slayer Info : Money" );
	

	//--------------------------------------------------		
	// 돈 설정
	//--------------------------------------------------		
	g_pMoneyManager->SetMoney( pInfo->getGold() );

	//--------------------------------------------------
	//	PCS Number 초기화 
	//--------------------------------------------------
//	g_pUserInformation->PCSNumber = pInfo->getPhoneNumber();
//	C_VS_UI_SLAYER_PDS::m_pcs_number = pInfo->getPhoneNumber();

	//--------------------------------------------------
	// Skill Hot key설정
	//--------------------------------------------------
	if (!g_bSetHotKey)
	{		
		UI_SetHotKey( 0, pInfo->getHotKey( 0 ) );
		UI_SetHotKey( 1, pInfo->getHotKey( 1 ) );
		UI_SetHotKey( 2, pInfo->getHotKey( 2 ) );
		UI_SetHotKey( 3, pInfo->getHotKey( 3 ) );

		g_bSetHotKey = TRUE;
	}	

	DEBUG_ADD( "Set Slayer Info OK" );
}


//--------------------------------------------------------------------------------
// Set PCVampire Info
//--------------------------------------------------------------------------------
// Player에게 vampire 정보를 설정한다.
//--------------------------------------------------------------------------------
void		
SetPCVampireInfo(PCVampireInfo2* pInfo)
{
	DEBUG_ADD( "Set Vampire Info" );
	
	// 임시로 설정..
	//g_pPlayer->SetGuildNumber( 2 );

	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정
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
	// Vampire남 : 여
	else
	{
		if (pInfo->getCompetence()==0)
		{
			g_pPlayer->SetCompetence( 0 );
			// edit by Coffee 2006.11.5
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

//	_MinTrace("sdfdsf:%d\n", pInfo->getSex() );

	// 지상, 공중 이동 결정
	if (g_pPlayer->GetCreatureType()==CREATURETYPE_BAT)
	{
		g_pPlayer->SetFlyingCreature();		
	}
	else
	{
		g_pPlayer->SetGroundCreature();
	}

//	// 피부색
	g_pPlayer->SetBodyColor1( pInfo->getSkinColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	if( g_pPlayer->GetCreatureType() == CREATURETYPE_WER_WOLF )
		g_pPlayer->SetBodyColor1( 377 );
//		g_pPlayer->SetBodyColor1( pInfo->getCoatColor() );

	// 머리색으로 바꼈다
//	g_pPlayer->SetBodyColor1( pInfo->getHairColor() );

	if( pInfo->getBatColor() != 0 )
		g_pPlayer->SetBatColor( pInfo->getBatColor() );
	else
		g_pPlayer->SetBatColor( 0xFFFF );

	//--------------------------------------------------
	// [ TEST CODE ]
	//--------------------------------------------------
	// 옷 색깔 설정하기
	//--------------------------------------------------
//	g_pPlayer->SetBodyColor2( pInfo->getCoatColor() );
	///*
	if (g_pPlayer->IsMale())
	{
		g_pPlayer->SetBodyColor2( 377 );//91 );
	}
	else
	{
		g_pPlayer->SetBodyColor2( 377 );//38 );
	}
	//*/

//	g_pPlayer->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );
//	g_pPlayer->SetAddonColorSet2( ADDON_COAT, pInfo->getSkinColor() );


	//--------------------------------------------------
	// 내 길드 설정
	//--------------------------------------------------
	int guildID = pInfo->getGuildID();

	g_pPlayer->SetGuildNumber( guildID );

	if (pInfo!=NULL)
	{
		g_pUserInformation->GuildName = pInfo->getGuildName().c_str();
	}
	else
	{
		g_pUserInformation->GuildName.Release();
	}
	g_pUserInformation->GuildGrade = pInfo->getGuildMemberRank();

	g_pGuildInfoMapper->SetGuildName( pInfo->getGuildID(), g_char_slot_ingame.sz_guild_name );

	//--------------------------------------------------
	// 정보창에서 출력할 때 필요한 정보..
	//--------------------------------------------------
	g_char_slot_ingame.sz_name			= g_pUserInformation->CharacterID.GetString();
	if(g_pUserInformation->GuildName.GetString() == NULL)
		g_char_slot_ingame.sz_guild_name	= "";
	else
		g_char_slot_ingame.sz_guild_name	= g_pUserInformation->GuildName.GetString();

	g_char_slot_ingame.GUILD_ID			= guildID;
	g_char_slot_ingame.GUILD_GRADE		= g_pUserInformation->GuildGrade;

	g_char_slot_ingame.GRADE		= pInfo->getRank();
//	int accum = 0;
//	accum = g_pExperienceTable->GetRankInfo(g_char_slot_ingame.GRADE, g_pPlayer->GetRace()).AccumExp;
//	g_char_slot_ingame.GRADE_EXP_REMAIN = accum-pInfo->getRankExp();
	g_char_slot_ingame.GRADE_EXP_REMAIN	= pInfo->getRankExp();

	g_char_slot_ingame.hair_color = pInfo->getBatColor();
	g_char_slot_ingame.skin_color = pInfo->getSkinColor();

	g_pUserInformation->HairColor = pInfo->getBatColor();	
	g_pUserInformation->SkinColor = pInfo->getSkinColor();
	g_pUserInformation->BatColor  = pInfo->getBatColor();
	
	//--------------------------------------------------
	// Effect 정보 제거
	//--------------------------------------------------
	g_char_slot_ingame.STATUS.clear();
	
	//g_pUserInformation->FaceStyle = pInfo->getHairStyle();

	// vampire default 얼굴
	if (g_pPlayer->IsMale())
	{
		g_char_slot_ingame.man_info.face = M_FACE1;
	}
	else
	{
		g_char_slot_ingame.woman_info.face = W_FACE1;
	}

	//--------------------------------------------------
	// 현재 상태 설정
	//--------------------------------------------------
	int maxHP	= pInfo->getHP( ATTR_MAX );
	int HP		= pInfo->getHP( ATTR_CURRENT );
	int STR_CUR	= pInfo->getSTR( ATTR_CURRENT );
	int DEX_CUR	= pInfo->getDEX( ATTR_CURRENT );
	int INT_CUR	= pInfo->getINT( ATTR_CURRENT );
	int STR_MAX	= pInfo->getSTR( ATTR_MAX );
	int DEX_MAX	= pInfo->getDEX( ATTR_MAX );
	int INT_MAXX	= pInfo->getINT( ATTR_MAX );
	int STR_PURE	= pInfo->getSTR( ATTR_BASIC );
	int DEX_PURE	= pInfo->getDEX( ATTR_BASIC );
	int INT_PURE	= pInfo->getINT( ATTR_BASIC );
	//int STR_EXP	= pInfo->getSTRExp();
	//int DEX_EXP	= pInfo->getDEXExp();
	//int INT_EXP	= pInfo->getINTExp();
	int Exp		= pInfo->getExp();	
	int Bonus	= pInfo->getBonus();
	int Level	= pInfo->getLevel();	
	int Fame	= pInfo->getFame();
//	int Notority = pInfo->getNotoriety();

	g_pPlayer->SetStatus( MODIFY_MAX_HP, maxHP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_HP, HP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_STR, STR_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_DEX, DEX_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_INT, INT_CUR);
	g_pPlayer->SetStatus( MODIFY_BASIC_STR, STR_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_DEX, DEX_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_INT, INT_PURE);
	g_pPlayer->SetStatus( MODIFY_MAX_STR, STR_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_DEX, DEX_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_INT, INT_MAXX);
	g_pPlayer->SetStatus( MODIFY_BONUS_POINT, Bonus);
	g_pPlayer->SetStatus( MODIFY_LEVEL, Level);
	g_pPlayer->SetStatus( MODIFY_FAME, Fame);
//	g_pPlayer->SetStatus( MODIFY_NOTORIETY, Notority);
	g_pPlayer->SetStatus( MODIFY_VAMP_EXP_REMAIN, Exp);
	g_pPlayer->SetStatus( MODIFY_SILVER_DAMAGE, pInfo->getSilverDamage());	
	g_pPlayer->SetStatus( MODIFY_ALIGNMENT, pInfo->getAlignment() );
	g_pPlayer->SetStatus( MODIFY_RANK, pInfo->getRank() );
	g_pPlayer->SetStatus( MODIFY_RANK_EXP_REMAIN, pInfo->getRankExp() );
	g_pPlayer->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, pInfo->getAdvancementLevel() );

	//g_pPlayer->SetStatus( MODIFY_STR_EXP, STR_EXP);
	//g_pPlayer->SetStatus( MODIFY_DEX_EXP, DEX_EXP);
	//g_pPlayer->SetStatus( MODIFY_INT_EXP, INT_EXP);	
	
	//--------------------------------------------------
	// ui에 출력 정보 설정
	//--------------------------------------------------
	//g_char_slot_ingame.sz_name = g_pUserInformation->CharacterID.GetString();
	//g_char_slot_ingame.bl_vampire = g_pPlayer->IsVampire();
	//g_char_slot_ingame.bl_female = !g_pPlayer->IsMale();

	g_char_slot_ingame.level = g_pPlayer->GetLEVEL();
	g_char_slot_ingame.alignment = (ALIGNMENT)ConvertAlignment( g_pPlayer->GetAlignment() );

	g_char_slot_ingame.EXP_REMAIN	= pInfo->getExp();

	g_char_slot_ingame.STR_CUR = STR_CUR;
	g_char_slot_ingame.DEX_CUR = DEX_CUR;
	g_char_slot_ingame.INT_CUR = INT_CUR;
	g_char_slot_ingame.STR_MAX = STR_MAX;
	g_char_slot_ingame.DEX_MAX = DEX_MAX;
	g_char_slot_ingame.INT_MAXX = INT_MAXX;
	g_char_slot_ingame.STR_PURE = STR_PURE;
	g_char_slot_ingame.DEX_PURE = DEX_PURE;
	g_char_slot_ingame.INT_PURE = INT_PURE;
	//g_char_slot_ingame.DAM =g_pPlayer->GetDAM();
	//g_char_slot_ingame.AC =g_pPlayer->GetAC();
	g_char_slot_ingame.TOHIT =g_pPlayer->GetTOHIT();
	g_char_slot_ingame.HP =g_pPlayer->GetHP();
	g_char_slot_ingame.MP =g_pPlayer->GetMP();
	g_char_slot_ingame.HP_MAX =g_pPlayer->GetMAX_HP();
	g_char_slot_ingame.MP_MAX =g_pPlayer->GetMAX_MP();
	//g_char_slot_ingame.CC =g_pPlayer->GetCARRYWEIGHT();
	g_char_slot_ingame.FAME =g_pPlayer->GetFAME();
//	g_char_slot_ingame.NOTERITY =g_pPlayer->GetNotoriety();

	if(pInfo->getAdvancementLevel() > 0)
		g_char_slot_ingame.EXP_REMAIN = pInfo->getAdvancementGoalExp();

	//--------------------------------------------------
	// Domain Level 설정
	//--------------------------------------------------
	/*
	const int numDomains = 1;
	int domains[numDomains] = 
	{ 
		SKILLDOMAIN_VAMPIRE
	};

	for (int i=0; i<numDomains; i++)
	{
		int domain = domains[i];

		(*g_pSkillManager)[domain].SetDomainLevel( pInfo->getSkillDomainLevel( SkillDomain)domain ) );
	}	
	*/

	//--------------------------------------------------
	// UI에 정보 설정
	//--------------------------------------------------
	//UI_SetHP( HP, maxHP );

	//--------------------------------------------------		
	// 돈 설정
	//--------------------------------------------------		
	g_pMoneyManager->SetMoney( pInfo->getGold() );

	//--------------------------------------------------
	//	VampireGear 초기화
	//--------------------------------------------------			
	g_pVampireGear->Init();

	//--------------------------------------------------
	// Skill Hot key설정
	//--------------------------------------------------
	if (!g_bSetHotKey)
	{		
		UI_SetHotKey( 0, pInfo->getHotKey( 0 ) );
		UI_SetHotKey( 1, pInfo->getHotKey( 1 ) );
		UI_SetHotKey( 2, pInfo->getHotKey( 2 ) );
		UI_SetHotKey( 3, pInfo->getHotKey( 3 ) );
		UI_SetHotKey( 4, pInfo->getHotKey( 4 ) );
		UI_SetHotKey( 5, pInfo->getHotKey( 5 ) );
		UI_SetHotKey( 6, pInfo->getHotKey( 6 ) );
		UI_SetHotKey( 7, pInfo->getHotKey( 7 ) );


		g_bSetHotKey = TRUE;		
	}

	DEBUG_ADD( "Set Vampire Info OK" );
}

//--------------------------------------------------------------------------------
// Set PCVampire Info
//--------------------------------------------------------------------------------
// Player에게 vampire 정보를 설정한다.
//--------------------------------------------------------------------------------
void		
SetPCOustersInfo(PCOustersInfo2* pInfo)
{
	DEBUG_ADD( "Set Ousters Info" );
	
	// 임시로 설정..
	//g_pPlayer->SetGuildNumber( 2 );

	g_pPlayer->SetID( pInfo->getObjectID() );
	g_pPlayer->SetSight( pInfo->getSight() );

	//--------------------------------------------------
	// Player 몸 설정
	//--------------------------------------------------
	if (pInfo->getCompetence()==0)
	{
		g_pPlayer->SetCompetence( 0 );
		g_pPlayer->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
		g_pPlayer->SetMale( pInfo->getSex()==MALE );
	}
	else
	{
		int creatureType = CREATURETYPE_OUSTERS;
		g_pPlayer->SetCreatureType( creatureType );
	}
	
// 	g_pPlayer->SetMale( pInfo->getSex()==MALE );

	g_pPlayer->SetGroundCreature();

	// 머리색
	g_pPlayer->SetBodyColor1( pInfo->getHairColor() );
	g_pPlayer->SetMasterEffectType( pInfo->getMasterEffectColor() );

	//--------------------------------------------------
	// [ TEST CODE ]
	//--------------------------------------------------
	// 옷 색깔 설정하기
	//--------------------------------------------------
//	g_pPlayer->SetBodyColor2( pInfo->getCoatColor() );
	///*
//	if (g_pPlayer->IsMale())
//	{
//		g_pPlayer->SetBodyColor2( 377 );//91 );
//	}
//	else
//	{
//		g_pPlayer->SetBodyColor2( 377 );//38 );
//	}
	//*/

//	g_pPlayer->SetAddonColorSet1( ADDON_COAT, pInfo->getSkinColor() );
//	g_pPlayer->SetAddonColorSet2( ADDON_COAT, pInfo->getSkinColor() );


	//--------------------------------------------------
	// 내 길드 설정
	//--------------------------------------------------
	int guildID = pInfo->getGuildID();

	g_pPlayer->SetGuildNumber( guildID );

	if (pInfo!=NULL)
	{
		g_pUserInformation->GuildName = pInfo->getGuildName().c_str();
	}
	else
	{
		g_pUserInformation->GuildName.Release();
	}
	g_pUserInformation->GuildGrade = pInfo->getGuildMemberRank();

	//--------------------------------------------------
	// 정보창에서 출력할 때 필요한 정보..
	//--------------------------------------------------
	g_char_slot_ingame.sz_name			= g_pUserInformation->CharacterID.GetString();
	if(g_pUserInformation->GuildName.GetString() == NULL)
		g_char_slot_ingame.sz_guild_name	= "";
	else
		g_char_slot_ingame.sz_guild_name	= g_pUserInformation->GuildName.GetString();

	g_pGuildInfoMapper->SetGuildName( pInfo->getGuildID(), g_char_slot_ingame.sz_guild_name );

	g_char_slot_ingame.GUILD_ID			= guildID;
	g_char_slot_ingame.GUILD_GRADE		= g_pUserInformation->GuildGrade;

	g_char_slot_ingame.GRADE		= pInfo->getRank();
//	int accum = 0;
//	accum = g_pExperienceTable->GetRankInfo(g_char_slot_ingame.GRADE, g_pPlayer->GetRace()).AccumExp;
//	g_char_slot_ingame.GRADE_EXP_REMAIN = accum-pInfo->getRankExp();
	g_char_slot_ingame.GRADE_EXP_REMAIN	= pInfo->getRankExp();

	g_char_slot_ingame.hair_color = pInfo->getHairColor();
//	g_char_slot_ingame.skin_color = pInfo->getSkinColor();

	g_pUserInformation->HairColor = pInfo->getHairColor();	
//	g_pUserInformation->SkinColor = pInfo->getSkinColor();
	
	//--------------------------------------------------
	// Effect 정보 제거
	//--------------------------------------------------
	g_char_slot_ingame.STATUS.clear();
	
	//g_pUserInformation->FaceStyle = pInfo->getHairStyle();

	// vampire default 얼굴
	if (g_pPlayer->IsMale())
	{
		g_char_slot_ingame.man_info.face = M_FACE1;
	}
	else
	{
		g_char_slot_ingame.woman_info.face = W_FACE1;
	}

	//--------------------------------------------------
	// 현재 상태 설정
	//--------------------------------------------------
	int maxHP	= pInfo->getHP( ATTR_MAX );
	int maxMP	= pInfo->getMP( ATTR_MAX );
	int HP		= pInfo->getHP( ATTR_CURRENT );
	int MP		= pInfo->getMP( ATTR_CURRENT );
	int STR_CUR	= pInfo->getSTR( ATTR_CURRENT );
	int DEX_CUR	= pInfo->getDEX( ATTR_CURRENT );
	int INT_CUR	= pInfo->getINT( ATTR_CURRENT );
	int STR_MAX	= pInfo->getSTR( ATTR_MAX );
	int DEX_MAX	= pInfo->getDEX( ATTR_MAX );
	int INT_MAXX	= pInfo->getINT( ATTR_MAX );
	int STR_PURE	= pInfo->getSTR( ATTR_BASIC );
	int DEX_PURE	= pInfo->getDEX( ATTR_BASIC );
	int INT_PURE	= pInfo->getINT( ATTR_BASIC );
	//int STR_EXP	= pInfo->getSTRExp();
	//int DEX_EXP	= pInfo->getDEXExp();
	//int INT_EXP	= pInfo->getINTExp();
	int Exp		= pInfo->getExp();	
	int Bonus	= pInfo->getBonus();
	int skillPoint	= pInfo->getSkillBonus();
	int Level	= pInfo->getLevel();	
	int Fame	= pInfo->getFame();
//	int Notority = pInfo->getNotoriety();

	g_pPlayer->SetStatus( MODIFY_MAX_HP, maxHP);
	g_pPlayer->SetStatus( MODIFY_MAX_MP, maxMP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_HP, HP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_MP, MP);
	g_pPlayer->SetStatus( MODIFY_CURRENT_STR, STR_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_DEX, DEX_CUR);
	g_pPlayer->SetStatus( MODIFY_CURRENT_INT, INT_CUR);
	g_pPlayer->SetStatus( MODIFY_BASIC_STR, STR_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_DEX, DEX_PURE);
	g_pPlayer->SetStatus( MODIFY_BASIC_INT, INT_PURE);
	g_pPlayer->SetStatus( MODIFY_MAX_STR, STR_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_DEX, DEX_MAX);
	g_pPlayer->SetStatus( MODIFY_MAX_INT, INT_MAXX);
	g_pPlayer->SetStatus( MODIFY_BONUS_POINT, Bonus);
	g_pPlayer->SetStatus( MODIFY_SKILL_BONUS_POINT, skillPoint);
	g_pPlayer->SetStatus( MODIFY_LEVEL, Level);
	g_pPlayer->SetStatus( MODIFY_FAME, Fame);
//	g_pPlayer->SetStatus( MODIFY_NOTORIETY, Notority);
	g_pPlayer->SetStatus( MODIFY_VAMP_EXP_REMAIN, Exp);
	g_pPlayer->SetStatus( MODIFY_SILVER_DAMAGE, pInfo->getSilverDamage());	
	g_pPlayer->SetStatus( MODIFY_ALIGNMENT, pInfo->getAlignment() );
	g_pPlayer->SetStatus( MODIFY_RANK, pInfo->getRank() );
	g_pPlayer->SetStatus( MODIFY_RANK_EXP_REMAIN, pInfo->getRankExp() );
	g_pPlayer->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, pInfo->getAdvancementLevel() );	

	//g_pPlayer->SetStatus( MODIFY_STR_EXP, STR_EXP);
	//g_pPlayer->SetStatus( MODIFY_DEX_EXP, DEX_EXP);
	//g_pPlayer->SetStatus( MODIFY_INT_EXP, INT_EXP);	
	
	//--------------------------------------------------
	// ui에 출력 정보 설정
	//--------------------------------------------------
	//g_char_slot_ingame.sz_name = g_pUserInformation->CharacterID.GetString();
	//g_char_slot_ingame.bl_vampire = g_pPlayer->IsVampire();
	//g_char_slot_ingame.bl_female = !g_pPlayer->IsMale();

	g_char_slot_ingame.level = g_pPlayer->GetLEVEL();
	g_char_slot_ingame.alignment = (ALIGNMENT)ConvertAlignment( g_pPlayer->GetAlignment() );

	g_char_slot_ingame.EXP_REMAIN	= pInfo->getExp();

	g_char_slot_ingame.STR_CUR = STR_CUR;
	g_char_slot_ingame.DEX_CUR = DEX_CUR;
	g_char_slot_ingame.INT_CUR = INT_CUR;
	g_char_slot_ingame.STR_MAX = STR_MAX;
	g_char_slot_ingame.DEX_MAX = DEX_MAX;
	g_char_slot_ingame.INT_MAXX = INT_MAXX;
	g_char_slot_ingame.STR_PURE = STR_PURE;
	g_char_slot_ingame.DEX_PURE = DEX_PURE;
	g_char_slot_ingame.INT_PURE = INT_PURE;
	//g_char_slot_ingame.DAM =g_pPlayer->GetDAM();
	//g_char_slot_ingame.AC =g_pPlayer->GetAC();
	g_char_slot_ingame.TOHIT =g_pPlayer->GetTOHIT();
	g_char_slot_ingame.HP =g_pPlayer->GetHP();
	g_char_slot_ingame.MP =g_pPlayer->GetMP();
	g_char_slot_ingame.HP_MAX =g_pPlayer->GetMAX_HP();
	g_char_slot_ingame.MP_MAX =g_pPlayer->GetMAX_MP();
	//g_char_slot_ingame.CC =g_pPlayer->GetCARRYWEIGHT();
	g_char_slot_ingame.FAME =g_pPlayer->GetFAME();
//	g_char_slot_ingame.NOTERITY =g_pPlayer->GetNotoriety();

	if(pInfo->getAdvancementLevel() > 0)
		g_char_slot_ingame.EXP_REMAIN = pInfo->getAdvancementGoalExp();

	//--------------------------------------------------
	// Domain Level 설정
	//--------------------------------------------------
	/*
	const int numDomains = 1;
	int domains[numDomains] = 
	{ 
		SKILLDOMAIN_VAMPIRE
	};

	for (int i=0; i<numDomains; i++)
	{
		int domain = domains[i];

		(*g_pSkillManager)[domain].SetDomainLevel( pInfo->getSkillDomainLevel( SkillDomain)domain ) );
	}	
	*/

	//--------------------------------------------------
	// UI에 정보 설정
	//--------------------------------------------------
	//UI_SetHP( HP, maxHP );

	//--------------------------------------------------		
	// 돈 설정
	//--------------------------------------------------		
	g_pMoneyManager->SetMoney( pInfo->getGold() );

	//--------------------------------------------------
	//	OustersGear 초기화
	//--------------------------------------------------			
	g_pOustersGear->Init();

//	//--------------------------------------------------
//	// Skill Hot key설정
//	//--------------------------------------------------
//	if (!g_bSetHotKey)
//	{		
//		UI_SetHotKey( 0, pInfo->getHotKey( 0 ) );
//		UI_SetHotKey( 1, pInfo->getHotKey( 1 ) );
//		UI_SetHotKey( 2, pInfo->getHotKey( 2 ) );
//		UI_SetHotKey( 3, pInfo->getHotKey( 3 ) );
//		UI_SetHotKey( 4, pInfo->getHotKey( 4 ) );
//		UI_SetHotKey( 5, pInfo->getHotKey( 5 ) );
//		UI_SetHotKey( 6, pInfo->getHotKey( 6 ) );
//		UI_SetHotKey( 7, pInfo->getHotKey( 7 ) );
//
//
//		g_bSetHotKey = TRUE;		
//	}

	DEBUG_ADD( "Set Ousters Info OK" );
}

//-----------------------------------------------------------------------------
// Popup Error Message
//-----------------------------------------------------------------------------
void
PopupErrorMessage(ErrorID errorID)
{
	switch (errorID)
	{
		case INVALID_ID_PASSWORD : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_INVALID_ID_PASSWORD].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case ALREADY_CONNECTED : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ALREADY_CONNECTED].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case ALREADY_REGISTER_ID : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ALREADY_REGISTER_ID].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case ALREADY_REGISTER_SSN : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ALREADY_REGISTER_SSN].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case EMPTY_ID : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_EMPTY_ID].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case SMALL_ID_LENGTH : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_SMALL_ID_LENGTH].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case EMPTY_PASSWORD : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_EMPTY_PASSWORD].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case SMALL_PASSWORD_LENGTH : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_SMALL_PASSWORD_LENGTH].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case EMPTY_NAME : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_EMPTY_NAME].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case EMPTY_SSN : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_EMPTY_SSN].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case INVALID_SSN : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_INVALID_SSN].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case NOT_FOUND_PLAYER :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_NOT_FOUND_PLAYER].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case NOT_FOUND_ID :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_NOT_FOUND_ID].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case NOT_ALLOW_ACCOUNT:
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_NOT_ALLOW_ACCOUNT].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		case NOT_PAY_ACCOUNT:
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_NOT_PAY_ACCOUNT].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;

		// 2004, 03, 26 sobeit add start
		case CANNOT_AUTHORIZE_BILLING:   // 빌링 정보를 찾을 수 없습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_CANNOT_AUTHORIZE_BILLING].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;
		case CANNOT_CREATE_PC_BILLING:    // 유료 사용자가 아니라서 캐릭터를 못 만듭니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_CANNOT_CREATE_PC_BILLING].GetString(), -1,-1,UI_DIALOG_OK, true);
		break;
		// 2004, 03, 26 sobeit add end
		
		// 2004, 7, 19 sobeit add start
		case KEY_EXPIRED:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_KEY_EXPIRED].GetString(), -1,-1,UI_DIALOG_OK, true);
			break;
		case NOT_FOUND_KEY:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_NOT_FOUND_KEY].GetString(), -1,-1,UI_DIALOG_OK, true);
			break;
		// 2004, 7, 19 sobeit add end
		case ETC_ERROR:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_LOGIN_DENY].GetString(), -1,-1,UI_DIALOG_OK, true);
			break;
		case IP_DENYED:
//			g_pUIDialog->PopupFreeMessageDlg("퀭쏟莖돨댄轎되쩌늴鑒법뜩샀賈痰죄렷랬棍밈넋埼，헝瞳24鬼珂빈疼되쩌", -1,-1,UI_DIALOG_OK, true);
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_IP_DENY].GetString(), -1,-1,UI_DIALOG_OK, true);
			break;
		case CHECK_VERSION_ERROR:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_CHECK_VERSION_ERROR].GetString(), -1,-1,UI_DIALOG_OK, true);
		default : //case ETC_ERROR : 
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString());
				
	}

}

//-----------------------------------------------------------------------------
// Set ServerGroupName
//-----------------------------------------------------------------------------
void
SetServerGroupName( const char* pName )
{
	g_pServerInformation->SetServerGroupName( pName );
}

//-----------------------------------------------------------------------------
// Set ServerGroupName
//-----------------------------------------------------------------------------
void
SetServerGroupStatus( int status )
{
	g_pServerInformation->SetServerGroupStatus( status );
}

//-----------------------------------------------------------------------------
// Set ServerName
//-----------------------------------------------------------------------------
void
SetServerName( const char* pName )
{
//	g_pServerInformation->SetServerName( pName );
}

//-----------------------------------------------------------------------------
// Use Item OK
//-----------------------------------------------------------------------------
// Potion사용
//-----------------------------------------------------------------------------

bool UseSkillCardOK(BYTE CardType)
{
	MItem* pItem = NULL;
	if(g_pPlayer->IsSlayer())
		pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 5);
	else if(g_pPlayer->IsOusters())
		pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 7);
	else if(g_pPlayer->IsVampire())
		pItem = g_pInventory->FindItem(ITEM_CLASS_MOON_CARD, 6);
	if(pItem!=NULL)
	{
		if(CardType == 1) // TimeLimitItem
		{

		}
		else	//NonLimit
		{
			pItem->SetNumber( max(0, pItem->GetNumber() - 1) );
			if(pItem->GetNumber() == 0)
			{
				g_pInventory->RemoveItem( (TYPE_OBJECTID)pItem->GetID() );

				UI_RemoveDescriptor( (void*)pItem );

				delete pItem;	
			}
		}
	}
	else
		return false;
	return true;
}
bool
UseItemOK()
{
	//------------------------------------------------------------------
	//
	//				Item Check Buffer 확인
	//
	//------------------------------------------------------------------	
	MItem* pItem = g_pPlayer->GetItemCheckBuffer();

	//----------------------------------------------------
	// Check Buffer에 item이 있는 경우
	//----------------------------------------------------
	if (pItem!=NULL)
	{
		PlaySound( pItem->GetUseSoundID() );

		//-------------------------------------------------
		// Force Feel
		//-------------------------------------------------
		if (g_pUserOption->UseForceFeel && gpC_Imm!=NULL && gpC_Imm->IsDevice()
			&& pItem->GetUseSoundID() < g_pSoundTable->GetSize())
		{
			DEBUG_ADD_FORMAT("ForceAction-UseItemOK(%d)", pItem->GetUseSoundID());
			gpC_Imm->ForceAction( pItem->GetUseSoundID() );
		}

		MPlayer::ITEM_CHECK_BUFFER status =	g_pPlayer->GetItemCheckBufferStatus();

		if( status == MPlayer::ITEM_CHECK_BUFFER_ITEM_TO_ITEM )
		{
			g_pPlayer->ClearItemCheckBuffer();
			if( g_pTempInformation->GetMode() == TempInformation::MODE_ITEM_CODE_SHEET )
			{
				char type = char(g_pTempInformation->Value1)+1;
				
				int invenX = HIWORD( g_pTempInformation->Value2);
				int invenY = LOWORD( g_pTempInformation->Value2);
				
				std::list<TYPE_ITEM_OPTION> OldItemOption = pItem->GetItemOptionList();
				
				int offset = invenY * 10 + invenX;
				if( offset >=0 && offset<60 &&OldItemOption.size()==30)
				{
					pItem->ClearItemOption();
					int cur_off=0;
					std::list<TYPE_ITEM_OPTION>::iterator itr = OldItemOption.begin();
					std::list<TYPE_ITEM_OPTION>::iterator enditr = OldItemOption.end();
					
					while( itr != enditr )
					{
						TYPE_ITEM_OPTION option = *itr;
						if(offset >= cur_off && offset < cur_off+2)
						{
							if( offset == cur_off )
								option = (type<<4)|(option&0xf);
							else
								option = ((option&0xf0))|(type&0xf);
						}
						pItem->AddItemOption( option );						
						cur_off+=2;
						++itr;
					}					
				}				
				UI_DropItem();
				if( g_pTempInformation->pValue != NULL )
				{
					MItem *pMouseItem = (MItem*)(g_pTempInformation->pValue);
					if( pMouseItem != NULL )
						delete pMouseItem;
				}
				g_pTempInformation->SetMode( TempInformation::MODE_NULL );								
				gC_vs_ui.RunQuestInventory(pItem);
				g_pTempInformation->pValue = NULL;
			}
		} else
		//----------------------------------------------------
		// Inventory에서 사용
		//----------------------------------------------------		
		if (status==MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY)			
		{			
			// Item Check Buffer를 지운다.
			
		#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
			DWORD SubInvetoryID = g_pPlayer->GetItemIDCheckBufferSubInventory();
		#endif

			g_pPlayer->ClearItemCheckBuffer();

			//-------------------------------------------------
			// VampirePortal인 경우
			//-------------------------------------------------
			/*
			if (pItem->GetItemClass()==ITEM_CLASS_VAMPIRE_PORTAL_ITEM)
			{
				MVampirePortalItem* pPortalItem = (MVampirePortalItem*)pItem;

				if (!pPortalItem->IsMarked())
				{
					int zoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);
					int zoneX = g_pPlayer->GetServerX();
					int zoneY = g_pPlayer->GetServerY();
					pPortalItem->SetZone(zoneID, zoneX, zoneY);
				}
			}
			*/

			//-------------------------------------------------
			// 사용하는 아이템인 경우는 숫자를 줄인다.
			//-------------------------------------------------
			if (pItem->IsChargeItem() 
				|| pItem->IsPileItem())
			{
				pItem->SetNumber( max(0, pItem->GetNumber() - 1) );
			}

			if(pItem->GetItemClass() == ITEM_CLASS_DYE_POTION )
			{
				TYPE_ITEMTYPE	ItemType = pItem->GetItemType();
				// 헤어색은 슬레이어, 아우스터즈만
				if(ItemType >= 0 && ItemType <= 23 )
				{
					if(g_pPlayer->IsSlayer())
					{
						int color_set = g_DyeColorSet[ItemType];
						if(g_pPlayer->IsAdvancementClass())
							g_pPlayer->SetAddonColorSet1( ADDON_COAT, (WORD) color_set );
						else
							g_pPlayer->SetAddonHair( g_pPlayer->GetHairFrameID(), (WORD) color_set );
						g_pUserInformation->HairColor = color_set;
					}
					else if (g_pPlayer->IsOusters() )
					{
						int color_set = g_DyeColorSet[ItemType];
						g_pPlayer->SetBodyColor1( color_set );
						g_pUserInformation->HairColor = color_set;
					}
				} else
				if( ItemType >=24 && ItemType <= 47 )
				{
					int color_set = g_DyeColorSet[ItemType];
					if(g_pPlayer->IsSlayer())
					{
						g_pPlayer->SetAddonColorSet1(ADDON_COAT, color_set);
					}						
					else
						g_pPlayer->SetBodyColor1( color_set );
					g_pUserInformation->SkinColor = color_set;
				} else
				if( ItemType == 48 )
				{
					// 성전환 아이템이다.
					if( !g_pPlayer->IsOusters() )
					{
						
						if( g_pPlayer->IsSlayer() )
						{
							const MCreatureWear::ADDON_INFO& addon= g_pPlayer->GetAddonInfo( ADDON_HAIR );
						
							g_pPlayer->SetCreatureType( (g_pPlayer->IsMale()==true)? CREATURETYPE_SLAYER_FEMALE: CREATURETYPE_SLAYER_MALE );
							g_pPlayer->SetMale( !g_pPlayer->IsMale() );

							
							if (g_pPlayer->IsMale()==true)
							{
								g_pPlayer->SetAddonHair(g_PacketHairMaleID[addon.FrameID], addon.ColorSet1);		
							}
							// slayer 여
							else
							{
								g_pPlayer->SetAddonHair(g_PacketHairFemaleID[addon.FrameID], addon.ColorSet1);		
							}
						}
						else
						{
							int creatureType = (g_pPlayer->IsMale()==true)? CREATURETYPE_VAMPIRE_MALE1 : CREATURETYPE_VAMPIRE_FEMALE1;
							g_pPlayer->SetCreatureType( creatureType );
							g_pPlayer->SetMale( !g_pPlayer->IsMale() );
						
						}
							
						
						// 가지고 있는 모든 아이템에 대해서 체크한다.
						if( g_pPlayer->IsSlayer() )
							g_pSlayerGear->CheckAffectStatusAll();
						else if( g_pPlayer->IsVampire() )
							g_pVampireGear->CheckAffectStatusAll();
						
						g_pInventory->CheckAffectStatusAll();
						
						g_char_slot_ingame.bl_female = g_pPlayer->IsFemale();
						if( g_pPlayer->IsFemale() )
						{
							g_char_slot_ingame.woman_info.hair = (CHAR_WOMAN)g_char_slot_ingame.man_info.hair;
							g_char_slot_ingame.woman_info.face = (CHAR_WOMAN)g_char_slot_ingame.man_info.face;
						} else
						{
							g_char_slot_ingame.man_info.hair = (CHAR_MAN)g_char_slot_ingame.woman_info.hair;
							g_char_slot_ingame.man_info.face = (CHAR_MAN)g_char_slot_ingame.woman_info.face;
						}
						
					}
				}
				else if( ItemType >= 49 && ItemType <= 57 ) // 마스터 이펙트 컬러 변경
				{
					WORD colorset_list[9] = {23, 2, 33, 82, 380, 50, 151, 395, 0xFFFF};
					int myColor = ItemType - 49;
					
					g_pPlayer->SetBatColor( colorset_list[myColor] );
					UI_PopupMessage( STRING_MESSAGE_SUCCESS_CHANGED_BAT_COLOR );
				}
				else if( ItemType >= 58 && ItemType <= 62 )
				{
					if(ItemType == 61)
						g_pPlayer->ChangeMasterEffectType(0);
					else
						g_pPlayer->ChangeMasterEffectType(ItemType - 57);
					UI_PopupMessage( STRING_MESSAGE_SUCCESS_CHANGE );
				}

			}
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
			else if(pItem->GetItemClass() == ITEM_CLASS_SUB_INVENTORY )
			{
				// sub inventory 처리는 GCSubInventoryInfo에서 한다.
				g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
			}
	#endif
			// 2004, 6, 18 sobeit add start - naming pet used
//			if(pItem->GetItemClass() == ITEM_CLASS_EVENT_GIFT_BOX )
//			{
//				if(pItem->GetItemType() == 22)
//					gC_vs_ui.SetChangeableNickName(true);
//				else if(pItem->GetItemType() == 23)
//					gC_vs_ui.SetChangeablePetNickName(true);
//			}
			// 2004, 6, 18 sobeit add end - naming pet used
			//-------------------------------------------------
			// 없어지지 않는 경우
			//-------------------------------------------------
			if (pItem->IsPileItem() && pItem->GetNumber()>0
				|| pItem->IsChargeItem() && pItem->GetNumber()>0
				|| pItem->GetItemClass()==ITEM_CLASS_SLAYER_PORTAL_ITEM
				|| pItem->GetItemClass()==ITEM_CLASS_OUSTERS_SUMMON_ITEM
				|| pItem->GetItemClass()==ITEM_CLASS_PET_ITEM)
				//add by zdj
				//|| pItem->GetItemClass()==ITEM_CLASS_SUB_INVENTORY)
			{
			}
			//-------------------------------------------------
			// 제거하는 경우
			//-------------------------------------------------
			else
			{
				#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관

				if(SubInvetoryID != 0 && SubInvetoryID != OBJECTID_NULL)
				{
					MSubInventory* SubInventoryItem = (MSubInventory*)g_pInventory->GetItem(SubInvetoryID);
					if(NULL != SubInventoryItem)
					{
						SubInventoryItem->RemoveItem((TYPE_OBJECTID)pItem->GetID());
						// item정보 제거
						UI_RemoveDescriptor( (void*)pItem );

						// memory에서 제거
						delete pItem;		
					}
				}
				else
				{
					// inventory에서 제거
					g_pInventory->RemoveItem( (TYPE_OBJECTID)pItem->GetID() );

					// item정보 제거
					UI_RemoveDescriptor( (void*)pItem );

					// memory에서 제거
					delete pItem;			
				}
				#else
					// inventory에서 제거
					g_pInventory->RemoveItem( (TYPE_OBJECTID)pItem->GetID() );

					// item정보 제거
					UI_RemoveDescriptor( (void*)pItem );

					// memory에서 제거
					delete pItem;	

				#endif
		
			}
		
		}
		//----------------------------------------------------
		// QuickSlot에서 사용
		//----------------------------------------------------
		else if (status==MPlayer::ITEM_CHECK_BUFFER_USE_FROM_QUICKSLOT)
		{	
			// Item Check Buffer를 지운다.
			g_pPlayer->ClearItemCheckBuffer();

			// 2004, 10, 5, sobeit add start - 퀵슬롯에서 오토바이키 사용
			if(pItem->GetItemClass() == ITEM_CLASS_KEY)
			{
				// 할게 없네..^^
			}
			// 2004, 10, 5, sobeit add start
			//-------------------------------------------------
			// 개수 체크를 해야한다.
			//-------------------------------------------------
			else if (pItem->IsPileItem() && pItem->GetNumber()>1)
			{
				// 아직 개수가 더 남아 있는 경우,
				// 개수를 하나 줄여준다.
				pItem->SetNumber( pItem->GetNumber() - 1 );
			}
			//-------------------------------------------------
			// 다 사용한 경우 --> 제거한다.
			//-------------------------------------------------
			else
			{
				// quickSlot에서 제거
				if( g_pPlayer->IsSlayer() )
					g_pQuickSlot->RemoveItem( (BYTE)pItem->GetItemSlot() );
				else if (g_pPlayer->IsOusters() )
				{
					MOustersArmsBand* pQuickSlot = NULL;
					// 아이템이 있는 암스밴드를 검색한다.
					
					if( g_pArmsBand1 != NULL && g_pArmsBand1->GetItemToModify( pItem->GetID() ) == pItem )
						pQuickSlot = g_pArmsBand1;
					if( g_pArmsBand2 != NULL && g_pArmsBand2->GetItemToModify( pItem->GetID() ) == pItem )
						pQuickSlot = g_pArmsBand2;
					
					if( pQuickSlot != NULL )
						pQuickSlot->RemoveItem( (BYTE)pItem->GetItemSlot() );
				}
				// item정보 제거
				UI_RemoveDescriptor( (void*)pItem );

				// memory에서 제거
				delete pItem;		
			}

			//----------------------------------------------------
			// 벨트 못 없애도록 한거.. 취소
			//----------------------------------------------------
			UI_UnlockGear();
		}
		//----------------------------------------------------
		// 다른 상태??
		//----------------------------------------------------
		else if( status == MPlayer::ITEM_CHECK_BUFFER_USE_FROM_GEAR)
		{
			g_pPlayer->ClearItemCheckBuffer();

			if( pItem->GetItemClass() == ITEM_CLASS_COUPLE_RING || 
				pItem->GetItemClass() == ITEM_CLASS_VAMPIRE_COUPLE_RING )
			{
				// -_- 커플링이면........ status...는 사용 안함.
				
				if( !g_pPlayer->IsWaitVerifyNULL() )
					return false;
				
				g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_LOVE_CHAIN );
				g_pPlayer->AddEffectStatus( EFFECTSTATUS_LOVE_CHAIN , 100 );	
//				g_pSystemMessage->AddFormat((*g_pGameStringTable)[UI_STRING_MESSAGE_TRACE].GetString(), (*g_pGameStringTable)[STRING_MESSAGE_COUPLE].GetString());
				
				MEvent event;
				event.eventID = EVENTID_LOVECHAIN;
				event.eventDelay = 10000;
				event.eventFlag = EVENTFLAG_SHOW_DELAY_STRING;
				event.eventType = EVENTTYPE_ZONE;
				event.m_StringsID.push_back(STRING_MESSAGE_COUPLE_MOVE_START);
				event.m_StringsID.push_back(STRING_MESSAGE_MOVE_DELAY_SEC);

				g_pEventManager->AddEvent(event);
			} 
		}
		// 2004, 9, 13, sobeit add start - 퀘스트 인벤 아이템 사용 했다
		else if(status == MPlayer::ITEM_CHECK_BUFFER_USE_FROM_GQUEST_INVENTORY)
		{
			g_pPlayer->ClearItemCheckBuffer();
			gC_vs_ui.DeleteQuestItem(pItem->GetItemSlot());
		}
		// 2004, 9, 13, sobeit add end - 퀘스트 인벤 아이템 사용 했다
		else
		{
			DEBUG_ADD_FORMAT("[Error] ItemCheck Buffer is not Use Status: status=%d", (int)status);

			return false;
		}

	}
	//----------------------------------------------------
	// item이 없는 경우.. - -;;
	//----------------------------------------------------
	else
	{
		DEBUG_ADD("[Error] No Item in CheckBuffer");
		
		return false;
	}

	return true;
}

//------------------------------------------------------------------
// Affect ModifyInfo ( MStatus*, ModifyInfo* )
//------------------------------------------------------------------
void
AffectModifyInfo(MStatus* pStatus, ModifyInfo* pInfo)
{
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	int i;

	SHORTDATA sData;
	LONGDATA lData;

	DEBUG_ADD("AMo");

	int shortNum = pInfo->getShortCount();
	int longNum = pInfo->getLongCount();


	for (i=0; i<shortNum; i++)
	{
		pInfo->popShortData( sData );
		
		pStatus->SetStatus( sData.type, sData.value );		
	}

	DEBUG_ADD("LD");
	
	for (i=0; i<longNum; i++)
	{
		pInfo->popLongData( lData );
		
		pStatus->SetStatus( lData.type, lData.value );		
	}

	DEBUG_ADD("AM_ok");
}

//------------------------------------------------------------------
// New FakeCreature
//------------------------------------------------------------------
// (x,y)에 pCreature의 FakeCreature를 생성한다.
//------------------------------------------------------------------
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

//------------------------------------------------------------------
// New FakeCreature
//------------------------------------------------------------------
// (x,y)에 pCreature의 FakeCreature를 생성한다.
//------------------------------------------------------------------
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

//------------------------------------------------------------------
// Come From Portal
//------------------------------------------------------------------
// 포탈 안에서 나오는 모습 표현.. 
// 뱀파만 .. 뭐 관계는 없겠지..
//------------------------------------------------------------------
void		
ComeFromPortal(MCreature* pCreature)
{
	if(pCreature == NULL)
		return;

	int cutStart = 80;
	int cutInc = -5;

	int body = pCreature->GetCreatureFrameID(0);
	int action = ACTION_STAND;
	int direction = 2;

	const FRAME_ARRAY& FA = g_pTopView->GetCreatureFrameArray(body, action, direction);
	if(FA.GetSize() != 0)
	{
		const CFrame &Frame = FA[0];					
		int sprite	= Frame.GetSpriteID();

		const CIndexSprite* pSprite = g_pTopView->GetCreatureSprite(sprite);

		if(pSprite != NULL)
		{
			WORD height = pSprite->GetHeight();

			cutStart = (height+15)-(height+15)%16;
			if(height == 0)
			{
				cutInc = 1;
			}
			else
			{
				cutInc = max(1, cutStart/16);
			}
		}
	}
	
	pCreature->SetCutHeightFrom( cutStart, 0, -cutInc );

	int x = pCreature->GetX();
	int y =	pCreature->GetY();

	//------------------------------------------------------
	// 빙빙 돌아서 캐릭을 휘감는 이펙트 표현..
	//------------------------------------------------------
	// 울버린 일때
	if(pCreature->GetCreatureType() >= 693 && pCreature->GetCreatureType() <= 697 )
	{
		ExecuteActionInfoFromMainNode(
					RESULT_SUMMON_ULBERINE,										// 사용 기술 번호
				
					x, y, 0,
					DIRECTION_DOWN, // 사용 방향
					
					pCreature->GetID(),												// 목표에 대한 정보
					x, y, 0, 
					
					0,													// 기술의 (남은) 지속 시간		
					
					NULL,
					
					false);	
	}
	// 그레이트 러피언 일 때
	else if(pCreature->GetCreatureType() == 765) 
	{
		ExecuteActionInfoFromMainNode(
					SKILL_CLIENT_GREAT_RUFFIAN_ALIVE,										// 사용 기술 번호
				
					x, y, 0,
					DIRECTION_DOWN, // 사용 방향
					
					pCreature->GetID(),												// 목표에 대한 정보
					x, y, 0, 
					
					0,													// 기술의 (남은) 지속 시간		
					
					NULL,
					
					false);	
	}
	else
	{
		ExecuteActionInfoFromMainNode(
				RESULT_MAGIC_BLOODY_TUNNEL_INTO,										// 사용 기술 번호
			
				x, y, 0,
				DIRECTION_DOWN, // 사용 방향
				
				pCreature->GetID(),												// 목표에 대한 정보
				x, y, 0, 
				
				0,													// 기술의 (남은) 지속 시간		
				
				NULL,
				
				false);	
	}
}

//------------------------------------------------------------------
// Move Into Portal
//------------------------------------------------------------------
// 포탈 안으로 사라지는 모습 표현.. 
// 뱀파만 .. 뭐 관계는 없겠지..
//------------------------------------------------------------------
void		
MoveIntoPortal(MCreature* pCreature)
{
	int x = pCreature->GetX();
	int y =	pCreature->GetY();
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
				RESULT_MAGIC_BLOODY_TUNNEL_INTO,										// 사용 기술 번호
			
				x, y, 0,
				DIRECTION_DOWN, // 사용 방향
				
				pFakeCreature->GetID(),												// 목표에 대한 정보
				x, y, 0, 
				
				0,													// 기술의 (남은) 지속 시간		
				
				NULL,
				
				false);	
}

//------------------------------------------------------------------
// Packet CrossCounter
//------------------------------------------------------------------
// user --> target 
//------------------------------------------------------------------
void
SkillCrossCounter(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID)
{
	//------------------------------------------------------------------
	// 체크 체크~
	//------------------------------------------------------------------
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] CrossCounter. Creature NULL or Not WearCreature");
		
		return;
	}

	// [ TEST CODE ]
	// 진행중인 Effect이면 Result로 추가해야 한다.
	// Result를 생성하고..
	// pUserCreature->>AddActionResult( ... )
		
	//------------------------------------------------------
	// Fake Creature의 좌표 계산
	//------------------------------------------------------
	int x = pTargetCreature->GetX();
	int y = pTargetCreature->GetY();
	
	// 맞는 애에서 player방향으로 한 칸..
	int sx = pUserCreature->GetX() - x;
	int sy = pUserCreature->GetY() - y;

	if (sx>0) sx=1; else if (sx<0) sx=-1; else sx=0;
	if (sy>0) sy=1; else if (sy<0) sy=-1; else sy=0;

	x += sx;
	y += sy;					

	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------
	MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, x, y );

	int dir = pFakeCreature->GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );
	pFakeCreature->SetDirection( dir );
	pFakeCreature->SetCurrentDirection( dir );

	pFakeCreature->SetAction( ACTION_ATTACK );		// 그냥.. - -;;

	//------------------------------------------------------
	// 잔상 한번 보여주고 사라지게 설정
	//------------------------------------------------------
	pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FADE_ACTION );	

	//------------------------------------------------------
	// Fake Creature를 Zone에 추가
	//------------------------------------------------------
	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}

	//------------------------------------------------------
	// 기술 사용
	//------------------------------------------------------
	if (skillID==SKILL_ATTACK_MELEE)
	{	
		skillID = pUserCreature->GetBasicActionInfo();
	}

	if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
	{
		DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
		return;
	}

	MActionResult* pResult = new MActionResult;

	DWORD delayFrame = 0;//ConvertDurationToFrame( pPacket->getDuration() );

	pResult->Add( new MActionResultNodeActionInfo( 
								skillID, 
								pUserCreature->GetID(),
								pTargetCreature->GetID(),
								x,
								y,
								delayFrame ) );

	//------------------------------------------------------
	// EffectStatus가 있다면 붙인다.
	//------------------------------------------------------
	/*
	EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
	
	if (es!=EFFECTSTATUS_NULL)
	{
		pResult->Add( new MActionResultNodeAddEffectStatus( pFakeCreature->GetID(),
																es, 
																delayFrame ) );
	}
	*/

	pFakeCreature->PacketSpecialActionToOther(
						skillID, 
						pTargetCreature->GetID(),
						pResult			// 결과
	);			
	
}

//------------------------------------------------------------------
// Skill ShadowDancing // [새기술]
//------------------------------------------------------------------
// user --> target 
//------------------------------------------------------------------
void
SkillShadowDancing(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID)
{
	//------------------------------------------------------------------
	// 체크 체크~
	//------------------------------------------------------------------
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] ShadowDancing. Creature NULL or Not WearCreature");
		
		return;
	}

	// [ TEST CODE ]
	// 진행중인 Effect이면 Result로 추가해야 한다.
	// Result를 생성하고..
	// pUserCreature->>AddActionResult( ... )
		
	//------------------------------------------------------
	// Fake Creature의 좌표 계산
	//------------------------------------------------------
	int x = pTargetCreature->GetX();
	int y = pTargetCreature->GetY();
	
	// 맞는 애에서 player방향으로 한 칸..
	int sx = pUserCreature->GetX() - x;
	int sy = pUserCreature->GetY() - y;

	if (sx>0) sx=1; else if (sx<0) sx=-1; else sx=0;
	if (sy>0) sy=1; else if (sy<0) sy=-1; else sy=0;

	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------
	POINT cxy[3] = 
	{
		{ sx, sy },
		{ sx, -sy },
		{ -sx, sy }
	};

	for (int i=0; i<3; i++)
	{	
		MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pUserCreature->GetX(), pUserCreature->GetY());

		int dir = pFakeCreature->GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );
		pFakeCreature->SetDirection( dir );
		pFakeCreature->SetCurrentDirection( dir );

		pFakeCreature->SetAction( ACTION_SLAYER_SWORD );		// 그냥.. - -;;

		//------------------------------------------------------
		// 잔상 한번 보여주고 사라지게 설정
		//------------------------------------------------------
		pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );						

		//------------------------------------------------------
		// Fake Creature를 Zone에 추가
		//------------------------------------------------------
		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}

		//------------------------------------------------------
		// 기술 사용
		//------------------------------------------------------
		if (skillID==SKILL_ATTACK_MELEE)
		{	
			skillID = pUserCreature->GetBasicActionInfo();
		}

		if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
		{
			DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
			return;
		}


		pFakeCreature->SetFakeCreatureFastMoveAction(x+cxy[i].x, y+cxy[i].y, 
													skillID, 
													pTargetCreature->GetID());
	}

}

//-----------------------------------------------------------------------------
// Create ActionResultNode
//-----------------------------------------------------------------------------
// Creature에 skillID에 붙는 ActionResultNode를 생성한다.
//-----------------------------------------------------------------------------
MActionResultNode*
CreateActionResultNode(MCreature* pCreature, int skillID, BYTE grade)
{
	//------------------------------------------------------
	//
	// skill에 결과가 있으면 적용 시킨다.
	//
	//------------------------------------------------------
	MActionResultNode* pActionResultNode = NULL;

	switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
	{
		//------------------------------------------------------
		// Burrow
		//------------------------------------------------------
		case ACTIONRESULTNODE_CREATURE_BURROW :
			DEBUG_ADD("[ACTIONRESULTNODE_CREATURE_BURROW]");
			
			pActionResultNode = new MActionResultNodeCreatureBurrow(pCreature->GetID());
		break;

		//------------------------------------------------------
		// Invisible
		//------------------------------------------------------
		case ACTIONRESULTNODE_CREATURE_INVISIBLE :
			DEBUG_ADD("[ACTIONRESULTNODE_CREATURE_INVISIBLE]");
			
			pActionResultNode = new MActionResultNodeCreatureInvisible(pCreature->GetID());
		break;		

		//------------------------------------------------------
		// Visible
		//------------------------------------------------------
		case ACTIONRESULTNODE_CREATURE_VISIBLE :
			DEBUG_ADD("[ACTIONRESULTNODE_CREATURE_VISIBLE]");
			
			pActionResultNode = new MActionResultNodeCreatureVisible(pCreature->GetID());

			pCreature->RemoveEffectStatus( EFFECTSTATUS_INVISIBILITY );
			pCreature->RemoveEffectStatus( EFFECTSTATUS_SNIPPING_MODE );
		break;

		//------------------------------------------------------
		// Snipping
		//------------------------------------------------------
		case ACTIONRESULTNODE_CREATURE_SNIPPING :			
			DEBUG_ADD("[ACTIONRESULTNODE_CREATURE_SNIPPING]");
			
			pActionResultNode = new MActionResultNodeCreatureSnipping(pCreature->GetID());						
		break;

		//------------------------------------------------------
		// Turning
		//------------------------------------------------------
		case ACTIONRESULTNODE_CREATURE_TURNING :			
			DEBUG_ADD("[ACTIONRESULTNODE_CREATURE_TURNING]");

			if( skillID == SKILL_TYPHOON )
			{
				if( grade == 1 )
					pActionResultNode = new MActionResultNodeCreatureTurning(pCreature->GetID(), 16);
			}
			else
				pActionResultNode = new MActionResultNodeCreatureTurning(pCreature->GetID(), 16);
		break;

		//------------------------------------------------------
		// Casket [새기술]
		//------------------------------------------------------
		case ACTIONRESULTNODE_SUMMON_CASKET :			
			DEBUG_ADD("[ACTIONRESULTNODE_SUMMON_CASKET]");
			
			pActionResultNode = new MActionResultNodeSummonCasket(pCreature->GetID(), 0);
		break;

		//------------------------------------------------------
		// FakeDie
		//------------------------------------------------------
		case ACTIONRESULTNODE_FAKE_DIE:
			DEBUG_ADD("[ACTIONRESULTNODE_FAKE_DIE]");
			
			pActionResultNode = new MActionResultNodeFakeDie(pCreature->GetID());
			pCreature->SetFakeDie();
		break;

		//------------------------------------------------------
		// EVENT
		//------------------------------------------------------
		case ACTIONRESULTNODE_EVENT:
			DEBUG_ADD("[ACTIONRESULTNODE_EVENT]");
			
			pActionResultNode = new MActionResultNodeEvent(skillID);
		break;

	}


	return pActionResultNode;
}

//-----------------------------------------------------------------------------
// Check Item For SkillIcon
//-----------------------------------------------------------------------------
// 이 부분은 MItem에 각 item마다의 skill icon을 체크하는
// virtual member function을 만들어야 한다.
// 지금은 헤더 컴파일과 시간 관계상 이렇게 간다. - -; T_T;
//-----------------------------------------------------------------------------
void
CheckItemForSkillIcon(const MItem* pItem)
{
	ITEM_CLASS itemClass = pItem->GetItemClass();

	// 특정 item class인 경우는 skill icon이 바뀐다.
	if (g_pPlayer->IsSlayer()
		&& (itemClass==ITEM_CLASS_HOLYWATER
			|| itemClass==ITEM_CLASS_SLAYER_PORTAL_ITEM
			|| itemClass==ITEM_CLASS_BOMB
			|| itemClass==ITEM_CLASS_MINE
			|| itemClass==ITEM_CLASS_BOMB_MATERIAL)

		|| g_pPlayer->IsVampire()
		&& (itemClass==ITEM_CLASS_VAMPIRE_PORTAL_ITEM
			|| itemClass==ITEM_CLASS_VAMPIRE_ETC)
		)
	{
		g_pSkillAvailable->SetAvailableSkills();
	}	
}


void		SetFadeStart(char start, char end, char step, BYTE r, BYTE g, BYTE b,WORD delay)
{
	g_pTopView->SetFadeStart(start, end, step, r, g, b, delay);
}

void		SetFadeEnd()
{
	g_pTopView->SetFadeEnd();
}

//------------------------------------------------------------------
// Skill ShadowDancing // [새기술]
//------------------------------------------------------------------
// user --> target 
//------------------------------------------------------------------
void
SkillIllendue(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID)
{
	//------------------------------------------------------------------
	// 체크 체크~
	//------------------------------------------------------------------
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] ShadowDancing. Creature NULL or Not WearCreature");
		
		return;
	}

	// [ TEST CODE ]
	// 진행중인 Effect이면 Result로 추가해야 한다.
	// Result를 생성하고..
	// pUserCreature->>AddActionResult( ... )
		
	//------------------------------------------------------
	// Fake Creature의 좌표 계산
	//------------------------------------------------------
	int x = pTargetCreature->GetX();
	int y = pTargetCreature->GetY();
	
	// 맞는 애에서 player방향으로 한 칸..
	int sx = pUserCreature->GetX() - x;
	int sy = pUserCreature->GetY() - y;

	if (sx>0) sx=1; else if (sx<0) sx=-1; else sx=0;
	if (sy>0) sy=1; else if (sy<0) sy=-1; else sy=0;

	//------------------------------------------------------
	// Fake Creature 생성
	//------------------------------------------------------

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
	if (skillID ==TEMP_SKILL_ILLUSION_INVERSION)
	{
		Position = Straight;	
	}
	for (int i=0; i<4; i++)
	{	
		//MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pTargetCreature->GetX() + Position[i].x, pTargetCreature->GetY() + Position[i].y);
		MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pUserCreature->GetX() , pUserCreature->GetY() );

		int dir = pFakeCreature->GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );
		pFakeCreature->SetDirection( dir );
		pFakeCreature->SetCurrentDirection( dir );

		pFakeCreature->SetAction( ACTION_MAGIC );		// 그냥.. - -;;

		//------------------------------------------------------
		// 잔상 한번 보여주고 사라지게 설정
		//------------------------------------------------------
		//pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_BRIGHTNESS );	
		
		pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );

		//------------------------------------------------------
		// Fake Creature를 Zone에 추가
		//------------------------------------------------------
		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}

		//------------------------------------------------------
		// 기술 사용
		//------------------------------------------------------
		if (skillID==SKILL_ATTACK_MELEE)
		{	
			skillID = pUserCreature->GetBasicActionInfo();
		}

		if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
		{
			DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
			return;
		}

//		MActionResult* pResult = new MActionResult;
		
		DWORD delayFrame = 0;//ConvertDurationToFrame( pPacket->getDuration() );
		
		pFakeCreature->SetFakeCreatureFastMoveAction(
			pTargetCreature->GetX() + Position[i].x, pTargetCreature->GetY() + Position[i].y,
			skillID, 
			pTargetCreature->GetID() );
		
//		pResult->Add( new MActionResultNodeActionInfo( 
//			skillID, 
//			pUserCreature->GetID(),
//			pTargetCreature->GetID(),
//			x,
//			y,
//			delayFrame ) );
//		
//		pFakeCreature->PacketSpecialActionToOther(
//			skillID, 
//			pTargetCreature->GetID(),
//			pResult			// 결과
//			);			
	}
}

void		
SetAddonToOusters(MCreatureWear* pCreature, const PCOustersInfo2* pInfo)
{	
}

void		
SetAddonToOusters(MCreatureWear* pCreature, const PCOustersInfo3* pInfo)
{	
	MItem* pCoat		= g_pPacketItemOustersCoat[pInfo->getCoatType()];
	MItem* pArm			= g_pPacketItemOustersArm[pInfo->getArmType()];	
		

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

void
SkillBlazeWalk(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID, BYTE grade)
{
		//------------------------------------------------------------------
	// 체크 체크~
	//------------------------------------------------------------------
	if (pUserCreature==NULL || pTargetCreature==NULL)
	{
		DEBUG_ADD("[Error] ShadowDancing. Creature NULL or Not WearCreature");
		
		return;
	}

	// [ TEST CODE ]
	// 진행중인 Effect이면 Result로 추가해야 한다.
	// Result를 생성하고..
	// pUserCreature->>AddActionResult( ... )
	
	POINT TargetList[3] = 
	{
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 }
	};
		
	for (int i=0; i<=grade; i++)
	{			
		MFakeCreature* pFakeCreature = NewFakeCreature( pUserCreature, pUserCreature->GetX() , pUserCreature->GetY() );
		
//		int dir = pFakeCreature->GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

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
		pFakeCreature->SetAction( ACTION_STAND );
		
		pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );
		
		//------------------------------------------------------
		// Fake Creature를 Zone에 추가
		//------------------------------------------------------
		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}
		
		//------------------------------------------------------
		// 기술 사용
		//------------------------------------------------------
		if (skillID==SKILL_ATTACK_MELEE)
		{	
			skillID = pUserCreature->GetBasicActionInfo();
		}
		
		if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
		{
			DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
			return;
		}
		
		pFakeCreature->SetFakeCreatureFastMoveAction(
			pTargetCreature->GetX()+TargetList[i].x, pTargetCreature->GetY() +TargetList[i].y,
			skillID, 
			pTargetCreature->GetID() );

		pFakeCreature->SetDelay( (i+1)*4 );
	}
}

void
SendBugReport(const char *bug, ...)
{	
	if( bug == NULL )
		return;

	
	va_list		vl;
	char Buffer[256];

	va_start(vl, bug);
	vsprintf(Buffer, bug, vl);    
    va_end(vl);
	

#ifdef __DEBUG_OUTPUT__
	DEBUG_ADD_FORMAT("[BUG_REPORT] %s",Buffer);
#endif
	
	int len = strlen(Buffer);

	if( len <= 1 )
		return;

	if( len >= 100 )
		Buffer[100] = '\0';

	std::string message;

	message = "*bug_report ";
	message += Buffer;

	CGSay _CGSay;

	_CGSay.setMessage( message );
	_CGSay.setColor( 0 );
	
	if( g_pSocket != NULL )
		g_pSocket->sendPacket( &_CGSay );
	
}

void
SetPetInfo(PetInfo* pPetInfo, TYPE_OBJECTID objectID)
{
	MCreature *pCreature = g_pZone->GetCreature(objectID);

	if(pCreature != NULL)
	{
		if(pPetInfo->getPetType() == PET_NONE)
		{
			if(pCreature->GetPetID() != OBJECTID_NULL)
			{
				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(pCreature->GetPetID());

				if(pCreature != g_pPlayer)
				{
					if(gC_vs_ui.GetCurrentOtherInfoName() != NULL &&
						strcmp(pCreature->GetName(), gC_vs_ui.GetCurrentOtherInfoName()) == 0 )
					{
						gC_vs_ui.SetOtherPetInfo(NULL);
					}
				}

				if(pFakeCreature != NULL)
				{
					g_pZone->RemoveFakeCreature(pCreature->GetPetID());
				}
				else
				{
					DEBUG_ADD("[GCPetInfoHandler] 펫을 없앨려고 보니까 주인놈은 펫이 있는데 막상 펫이 없네 그려");
				}
			}
			else
			{
				DEBUG_ADD("[GCPetInfoHandler] 펫을 없앨려고 보니까 주인놈이 펫이 없네 그려");
			}
		}
		else
		{
			MPetItem *pItem = NULL;

			ObjectID_t itemID = pPetInfo->getPetItemObjectID();

			if(pCreature == g_pPlayer)		// 자기 펫인 경우
			{

			#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
				DWORD SubInvetoryID = g_pPlayer->GetItemIDCheckBufferSubInventory();
				if(SubInvetoryID != 0 && SubInvetoryID != OBJECTID_NULL)
				{
					MSubInventory* SubInventoryItem = (MSubInventory*)g_pInventory->GetItem(SubInvetoryID);
					if(NULL != SubInventoryItem)
					{
						pItem = dynamic_cast<MPetItem *>(SubInventoryItem->GetItemToModify(itemID));
					}
				}
				else
				{
					MItem* pSubInventory = NULL;
					pItem = dynamic_cast<MPetItem *>(g_pInventory->GetItemToModifyAll( itemID, pSubInventory ));

				//	pItem = dynamic_cast<MPetItem *>(g_pInventory->GetItemToModify(itemID));
				}
			#else
				pItem = dynamic_cast<MPetItem *>(g_pInventory->GetItemToModify(itemID));
			#endif


			}
			else
			{
				pItem = dynamic_cast<MPetItem *>(MItem::NewItem( ITEM_CLASS_PET_ITEM ));
				pItem->SetItemType( pPetInfo->getPetType() );
			}

			if(pItem != NULL)
			{
				MFakeCreature* pFakeCreature = NULL;
				bool bNewCreature = pPetInfo->isSummonInfo()>0?true:false;

				if(pCreature->GetPetID() != OBJECTID_NULL)
				{
					pFakeCreature = dynamic_cast<MFakeCreature *>(g_pZone->GetFakeCreature(pCreature->GetPetID()));

					if(pFakeCreature == NULL)
						return;

					if(pCreature == g_pPlayer)		// 자기 펫인 경우
					{
						MPetItem *pPetItem = pFakeCreature->GetPetItem();
						if(pPetItem != NULL)
						{
							if(pItem->GetID() == pFakeCreature->GetPetItem()->GetID())	// 원래께 또 날라온경우
							{
								// 새 펫이 레벨이 1높은경우
								if(pPetInfo->getPetLevel() == pPetItem->GetNumber()+1)
								{
									int num1 = pPetInfo->getPetLevel() % 10;
									if (num1==2 || num1==4 || num1==5 || num1==9)
									{
										g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_NEW_PET_LEVEL_1].GetString(), pPetItem->GetPetName().c_str(), pPetInfo->getPetLevel());
									}
									else
									{
										g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_NEW_PET_LEVEL_2].GetString(), pPetItem->GetPetName().c_str(), pPetInfo->getPetLevel());
									}

									// 레벨 10이 되서 속성을 부여할 수 있다
									if(pPetInfo->getPetLevel() == 10)
									{
										g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_PET_CAN_GET_ATTR].GetString());
									}
									else
									// 레벨이 49가 되서 옵션을 붙일 수 있다.
									if(pPetInfo->getPetLevel() == 49)
									{
										g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_CAN_GET_OPTION].GetString(), pPetItem->GetPetName().c_str());
									}

									// 아직 2차 능력이 없고
									// 겜블 할 수 있는 경우
									if(!pPetInfo->canCutHead() && pPetInfo->canGamble())
									{
										g_pGameMessage->Add((*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_ENCHANT_PET].GetString());
									}
								}
								// 2차 겜블 성공
								if(pPetInfo->canCutHead() != false && pPetItem->IsCanCutHead() == false)
								{
									g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_GAMBLE_OK].GetString(), pPetItem->GetPetName().c_str());
									if(g_pUserOption->UseTeenVersion == TRUE)
									{
										g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_PET_GAMBLE_DESC_TEEN].GetString());
									}
									else
									{
										g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_PET_GAMBLE_DESC].GetString());
									}
									UI_UnlockItem();
									
								}
								// 3차 겜블 성공
								else if(pPetInfo->canAttack() != false && pPetItem->IsCanAttack() == false)
								{
									g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_3RD_GAMBLE_OK].GetString(), pPetItem->GetPetName().c_str());
									UI_UnlockItem();
								}
								// 겜블 실패
								else if(pPetInfo->canGamble() == false && pPetItem->IsCanGamble() != false)
								{
									if(pPetInfo->canCutHead())
										g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_3RD_GAMBLE_FAIL].GetString(), pPetItem->GetPetName().c_str());
									else
										g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_GAMBLE_FAIL].GetString(), pPetItem->GetPetName().c_str());
									UI_UnlockItem();
								}
							}
						}

						pFakeCreature->SetCreatureType(pPetInfo->getPetCreatureType());
					}
				}
				else
				{
					POINT p = MFakeCreature::CaculatePetPosition(pCreature->GetX(), pCreature->GetY(), 2, 0, MCreature::CREATURE_FAKE_GROUND, pCreature->GetX(), pCreature->GetY());
					int dir = rand()%8;
					pFakeCreature = NewFakeCreature(pPetInfo->getPetCreatureType(), p.x, p.y, dir );

					//------------------------------------------------------
					// Fake Creature를 Zone에 추가
					//------------------------------------------------------
					if (!g_pZone->AddFakeCreature( pFakeCreature ))
					{
						delete pFakeCreature;
						if(pCreature != g_pPlayer)		// 자기 펫인 경우
						{
							delete pItem;
						}
					}

					pFakeCreature->SetZone(g_pZone);
				}

				if(pFakeCreature)
				{
					if(pPetInfo->getPetAttrLevel() > 0)	// 속성이 있는 경우는 색 표시
					{
						ITEMOPTION_TABLE::ITEMOPTION_PART optionPart = static_cast<ITEMOPTION_TABLE::ITEMOPTION_PART>(pPetInfo->getPetAttr());

						int size = g_pItemOptionTable->GetSize();

						for(int i = 1; i < size; i++)
						{
							ITEMOPTION_INFO &optionInfo = g_pItemOptionTable->Get(i);
							if(optionInfo.Part == optionPart && optionInfo.UpgradeOptionType == 0)
							{
								pFakeCreature->SetBodyColor1(optionInfo.ColorSet);
								pFakeCreature->SetBodyColor2(optionInfo.ColorSet);
								pItem->SetItemColorSet(optionInfo.ColorSet);
								
								break;
							}
						}
					}
					else
					{
						pFakeCreature->SetBodyColor1((*g_pItemOptionTable)[0].ColorSet);
						pFakeCreature->SetBodyColor2((*g_pItemOptionTable)[0].ColorSet);
						pItem->SetItemColorSet((*g_pItemOptionTable)[0].ColorSet);
					}

					pCreature->SetPetID( pFakeCreature->GetID() );

					pFakeCreature->SetOwnerID(pCreature->GetID());
					pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_TRACE );

					if(pItem->GetItemType() >= 4)	// 펫이 날라 다닐때 4:뱀2차펫, 5:아우2차펫
						pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
					else
						pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_GROUND);
					// 2004, 6, 18 sobeit add start - add nick name
					pFakeCreature->SetNickName(NicknameInfo::NICK_CUSTOM , (char*)pPetInfo->getNickname().c_str());
					// 2004, 6, 18 sobeit add end - add nick name
					pFakeCreature->SetFakePosition( pCreature->GetX(), pCreature->GetY() );
					pFakeCreature->SetTraceID( objectID ); 
					pFakeCreature->SyncTurretDirection();
					
					// FakeCreature가 정상적으로 추가 되었다
					pFakeCreature->SetPetItem(pItem);

					pItem->SetCurrentDurability( pPetInfo->getPetHP() );
					pItem->SetEnchantLevel( pPetInfo->getPetAttr() );
					pItem->SetSilver( pPetInfo->getPetAttrLevel() );
					pItem->SetNumber( pPetInfo->getPetLevel() );
					pItem->SetPetFoodType( pPetInfo->getFoodType() );
					int accum = 0;
					accum = g_pExperienceTable->GetPetExp(pPetInfo->getPetLevel()).AccumExp;
					pItem->SetPetExpRemain(accum-pPetInfo->getPetExp());
//					pItem->SetPetExpRemain( pPetInfo->getPetExp() );
					pItem->SetPetCutHead( pPetInfo->canCutHead()?true:false );
					pItem->SetPetGamble( pPetInfo->canGamble()?true:false );
					pItem->SetPetAttack( pPetInfo->canAttack()?true:false );
					
					pItem->ClearItemOption();
					if(pPetInfo->getPetOption() != NULL)
					{
						pItem->AddItemOption(pPetInfo->getPetOption());
					}

					pFakeCreature->SetName(pItem->GetPetName().c_str());

					if(bNewCreature)
					{
						ComeFromPortal( pFakeCreature );

						if(pCreature == g_pPlayer)	// 자기 펫인 경우
						{
							g_pSystemMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_SUMMON].GetString(), pItem->GetPetName().c_str());
						}
					}
				}
			}
			else
			{
				DEBUG_ADD_FORMAT("펫 붙일려고 아이템 찾아보니 아이템(%d) 없음", itemID);
			}
		}
	}
	else
	{
		DEBUG_ADD("[GCPetInfoHandler] 펫으로 먼가 해볼라니까 주인놈이 없네 그려");
		return;
	}

	MPetItem *pPetItem = NULL;
	PETINFO petInfo;
	if(pCreature != NULL && pCreature->GetPetID() != OBJECTID_NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(pCreature->GetPetID());
		if(pFakeCreature != NULL)
		{
			pPetItem = pFakeCreature->GetPetItem();
			if(pPetItem != NULL)
			{
				petInfo.HP = pPetItem->GetCurrentDurability();
				petInfo.ATTR = pPetItem->GetEnchantLevel();
				petInfo.ATTR_VALUE = pPetItem->GetSilver();
				petInfo.LEVEL = pPetItem->GetNumber();
				petInfo.FOODTYPE = pPetItem->GetPetFoodType();
				petInfo.EXP_REMAIN = pPetItem->GetPetExpRemain();
				petInfo.CUT_HEAD = pPetItem->IsCanCutHead();
				petInfo.GAMBLE = pPetItem->IsCanGamble();
				if(pPetItem->GetItemOptionListCount() == 0)
					petInfo.OPTION = -1;
				else
					petInfo.OPTION = pPetItem->GetItemOptionList().front();
				petInfo.ITEM_TYPE = pPetItem->GetItemType();
				petInfo.ITEM_ID = pPetItem->GetID();
				
				petInfo.NAME = pPetItem->GetPetName();
				petInfo.ATTACK = pPetItem->IsCanAttack();
			}
		}
	}

	// 자기 펫인 경우
	if(pCreature == g_pPlayer)
	{
		if(pPetItem == NULL)
			gC_vs_ui.SetPetInfo(NULL);
		else
			gC_vs_ui.SetPetInfo(&petInfo);
		g_pPlayer->CalculateLightSight();
		g_pPlayer->CalculateStatus();
		
		g_pPlayer->SetPetDelay(GetTickCount());
	}
	else if(pCreature != NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(pCreature->GetPetID());

		if(pFakeCreature != NULL)
		{
			if(gC_vs_ui.GetCurrentOtherInfoName() != NULL &&
				strcmp(pCreature->GetName(), gC_vs_ui.GetCurrentOtherInfoName()) == 0 )
			{
				if(pPetItem == NULL)
					gC_vs_ui.SetOtherPetInfo(NULL);
				else
					gC_vs_ui.SetOtherPetInfo(&petInfo);
			}
		}
	}

}

void	AddOustersElemental( MCreature *pOwnerCreature, int creatureType, int status )
{
	if(pOwnerCreature == NULL)
		return;
	
	if(pOwnerCreature->GetElementalID() != OBJECTID_NULL)
		return;

	POINT p = MFakeCreature::CaculatePetPosition(pOwnerCreature->GetX(), pOwnerCreature->GetY(), 2, 0, MCreature::CREATURE_FAKE_FLYING, pOwnerCreature->GetX(), pOwnerCreature->GetY());
	int dir = rand()%8;
	MFakeCreature *pFakeCreature = NewFakeCreature(creatureType, p.x, p.y, dir );

	//------------------------------------------------------
	// Fake Creature를 Zone에 추가
	//------------------------------------------------------
	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}

	pFakeCreature->SetZone(g_pZone);
	pOwnerCreature->SetElementalID( pFakeCreature->GetID() );
	pFakeCreature->SetOwnerID(pOwnerCreature->GetID());
	pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_TRACE );
	pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
	pFakeCreature->SetFakePosition( pOwnerCreature->GetX(), pOwnerCreature->GetY() );
	pFakeCreature->SetTraceID( pOwnerCreature->GetID() ); 
	pFakeCreature->AddEffectStatus( (EFFECTSTATUS)status, 0xFFFF );
}

void	SendCrashReport()
{
	if( gC_ci->IsKorean() == true )
	{
		MCrashReportManager crm;
		crm.LoadFromFile(CRASH_LOG_FILENAME);
		int sizeCrashReport = crm.GetSize();
		for(int i = 0; i < sizeCrashReport; i++)
		{
			MCrashReport &cr = crm[i];

			CGCrashReport _CGCrashReport;
			if(NULL != cr.GetAddress())			_CGCrashReport.setAddress(cr.GetAddress());
			if(NULL != cr.GetCallStack())		_CGCrashReport.setCallStack(cr.GetCallStack());
			if(NULL != cr.GetExecutableTime())	_CGCrashReport.setExecutableTime(cr.GetExecutableTime());
			if(NULL != cr.GetMessage())			_CGCrashReport.setMessage(cr.GetMessage());
			if(NULL != cr.GetOS())				_CGCrashReport.setOS(cr.GetOS());
			_CGCrashReport.setVersion(cr.GetVersion());

			if( g_pSocket != NULL )
				g_pSocket->sendPacket( &_CGCrashReport );
		}
		
		DeleteFile(OLD_CRASH_LOG_FILENAME);
		DeleteFile(CRASH_LOG_FILENAME);
	}
}

void	SetFlagTo( bool bTae )
{
	if( g_pTopView == NULL )
		return;

	if( bTae == true )
	{
		(*g_pItemTable)[ITEM_CLASS_EVENT_ITEM][27].SetFrameID( 856, 879,0 );
		(*g_pItemTable)[ITEM_CLASS_EVENT_ITEM][27].SetDropFrameID( 856 );
		
		(g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_HEAD] = (g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_HEAD_TAE];
		(g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_GROUND] = (g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_GROUND_TAE];

		(g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_HEAD] = (g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_HEAD_TAE];
		(g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_GROUND] = (g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_GROUND_TAE];
	}
	else
	{
		(*g_pItemTable)[ITEM_CLASS_EVENT_ITEM][27].SetFrameID( 750,764,0 );
		(*g_pItemTable)[ITEM_CLASS_EVENT_ITEM][27].SetDropFrameID( 750 );

		(g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_HEAD] = (g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_HEAD_ORIGINAL];
		(g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_GROUND] = (g_pTopView->m_EffectAlphaFPK)[EFFECTSPRITETYPE_FLAG_GROUND_ORIGINAL];
		
		(g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_HEAD] = (g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_HEAD_ORIGINAL];
		(g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_GROUND] = (g_pTopView->m_EffectAlphaPPK)[EFFECTSPRITETYPE_FLAG_GROUND_ORIGINAL];
	}
}

BOOL GetMacAddressFromNetBIOS(LPBYTE lpMacAddress)
{
    NCB ncb ;
    LANA_ENUM le ;
    
	int MacAddressSize = 6; // 6byte
	
    struct tagADAPTER
    {
        ADAPTER_STATUS adapt ;
        NAME_BUFFER    NameBuff[256] ;
    } adapter ;

	if(NULL == lpMacAddress)
		return FALSE;
//    ASSERT(lpMacAddress!=(LPBYTE)NULL) ;

    memset(&ncb,0x00,sizeof(NCB)) ;                  
    memset(&le,0x00,sizeof(LANA_ENUM)) ;             
    ncb.ncb_command = NCBENUM ;
    ncb.ncb_buffer = (UCHAR*)&le ;
    ncb.ncb_length = sizeof(LANA_ENUM) ;

    if(Netbios(&ncb)==NRC_GOODRET)
    {
        memset(&ncb,0x00,sizeof(NCB)) ;
        ncb.ncb_command = NCBRESET ;
//          ncb.ncb_callname[0] = 20 ;                   
        ncb.ncb_lana_num = le.lana[0] ;              

        if(Netbios(&ncb)==NRC_GOODRET)
        {
            memset(&ncb,0x00,sizeof(NCB)) ;
            memset(&ncb.ncb_callname,' ',NCBNAMSZ) ;
            ncb.ncb_callname[0] = '*' ;
//              ncb.ncb_callname[NCBNAMSZ-1] = NULL ;    
            ncb.ncb_command = NCBASTAT ;
            ncb.ncb_lana_num = le.lana[0] ;          
            ncb.ncb_buffer = (UCHAR*)&adapter ;
            ncb.ncb_length = sizeof(tagADAPTER) ;

            if(Netbios(&ncb)==NRC_GOODRET)
            {
                memcpy(lpMacAddress,&adapter.adapt.adapter_address,
                    MacAddressSize) ;
                return(TRUE) ;
            }
        }
    }
    return(FALSE) ;
}

BOOL GetMacAddressFromSock()
{
    int          iAdapters ;
    int          iOpt  = sizeof(iAdapters) ;
    int          iSize = sizeof(SOCKADDR_IPX) ;
    BOOL         bRet  = FALSE ;
    SOCKET       sock ;
    SOCKADDR_IPX adrIPX ;
    WSADATA      wsaData ;

	WORD wVersionRequested;
  
	wVersionRequested = MAKEWORD( 2, 0 );
//    #define WINSOCK_11 0x0101
//	char name[255];
//	PHOSTENT hostinfo;


    if(WSAStartup(wVersionRequested,&wsaData)==0)
    {
//		if( gethostname ( name, sizeof(name)) == 0)
//        {
//           if((hostinfo = gethostbyname(name)) != NULL)
//           {
//              g_IPAddress = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
//           }
//        }      
		sock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX);
        if(sock !=INVALID_SOCKET)
        {
			
            memset(&adrIPX,0x00,sizeof(SOCKADDR_IPX)) ;
            adrIPX.sa_family = AF_IPX ;

            if(bind(sock,(SOCKADDR*)&adrIPX,iSize)!=SOCKET_ERROR)
            {
                if(getsockopt(sock,NSPROTO_IPX,IPX_MAX_ADAPTER_NUM,
                    (char*)&iAdapters,&iOpt)!=SOCKET_ERROR)
                {
                    while(iAdapters)
                    {
                        IPX_ADDRESS_DATA ipxAdrData ;
                        memset( &ipxAdrData,0x00,
                            sizeof(IPX_ADDRESS_DATA)) ;
                        ipxAdrData.adapternum = --iAdapters ;
                        iOpt = sizeof(IPX_ADDRESS_DATA) ;
                        if(getsockopt(sock,NSPROTO_IPX,IPX_ADDRESS,
                            (char*)&ipxAdrData,&iOpt)!=SOCKET_ERROR)
                        {
                            if(ipxAdrData.status==TRUE && 
                                ipxAdrData.wan==FALSE)
                            {
                                memcpy(g_macAddress,
                                    &ipxAdrData.nodenum,
                                    6) ;
                                bRet = TRUE ;
                                break ;
                            }
                        }
                    }
                }
            }
            closesocket(sock) ;
        }

        WSACleanup() ;
    }
    return(bRet) ;
}

// 2004, 8, 26, sobeit add start - mac address 체크해서 윈도 모드 변경
std::string GetLocalIP()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );
	std::string ip;	
	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		if( gethostname ( name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
        }      
        WSACleanup( );
	} 
	return ip;
}

void 
CheckMacScreenMode()
{
	typedef std::map<std::string,std::string>			MACINFO_MAP;
	MACINFO_MAP MacInfo_Map;

	// ip, MacAddress insert
	// 프로그램팀_박숭욱.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0E-A6-74-38-F7", "220.117.154.99")); // ip
	// Report-심철 하드웨어.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-4C-ED-43-AE", "220.117.154.77")); // ip
	// D1기획최서우.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-4C-0B-C5-B2", "220.117.154.110")); // ip
	// (기획팀 박한수).htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-C0-26-8A-2C-D7", "220.117.154.75")); // ip
	// 리키한 PC사양.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-50-04-C0-88-5C", "220.117.154.79")); // ip
	// D1 그래픽팀 한광식 대리.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-04-75-9D-04-7F", "220.117.154.85")); // ip
	// D1 그래픽팀 최호진 대리.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-4C-0B-BC-AA", "220.117.154.81")); // ip
	// D1 그래픽팀 이준호 과장.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-04-75-99-30-3C", "220.117.154.82")); // ip
	// D1 그래픽팀 이승환 사원.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-04-75-99-07-A9", "220.117.154.90")); // ip
	// 프로그램팀_DB백업서버.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0C-6E-60-78-E8", "220.117.154.78")); // ip
	// 프로그램팀_한민훈.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B3-05", "221.148.70.152")); // ip
	// 프로그램팀_장홍창.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0D-60-2D-A8-67", "220.117.154.124")); // ip
	// 프로그램팀_이청환.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-3F", "221.148.70.153")); // ip
	// 프로그램팀_소스세이프서버.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-4C-ED-28-51", "220.117.154.106")); // ip
	// 프로그램팀_모니터링.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-29-38-D8-A5", "220.117.154.103")); // ip
	// 프로그램팀_지성욱.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0B-2B-0A-B9-0B", "220.117.154.74")); // ip
	// 프로그램팀_손희승.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0E-A6-8D-E9-C8", "220.117.154.71")); // ip
	// 프로그램팀_양승명.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-50-CE-30-B6-12", "220.117.154.70")); // ip
	// 프로그램팀_해외테스트컴퓨터.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-50-DA-6E-42-C8", "192.168.1.12")); // ip
	// 프로그램팀_최석민.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-18-FB-1A-D6", "220.117.154.72")); // ip
	// 고객지원팀_추연우.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-75", "221.148.70.132")); // ip
	// 고객지원팀_주성빈.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-4F", "221.148.70.133")); // ip
	// 프로그램팀_배재형.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0E-A6-27-B4-F3", "220.117.154.69")); // ip
	// 프로그램팀_이창규.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0B-6A-F4-2D-5A", "220.117.154.92")); // ip
	// 고객지원팀_조장용.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B2-DF", "221.148.70.136")); // ip
	// 고객지원팀_이재성.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-7F", "221.148.70.141")); // ip
	// 고객지원팀_김이동.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-7E", "221.148.70.134")); // ip
//	// 고객지원팀_서보경.htm
//	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-88", "221.148.70.140")); // ip
	// 고객지원팀_김충연.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-8C", "221.148.70.142")); // ip
	// 고객지원팀_김진한.htm -  이분껀 없네..-_-
	//MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-8C", "221.148.70.142")); // ip
//	// 고객지원팀_이수정.htm
//	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-7F", "221.148.70.143")); // ip
	// 고객지원팀_박세화.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-87", "221.148.70.135")); // ip
	// 고객지원팀_신동일.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-71", "221.148.70.138")); // ip
	// 지과장님 노트북.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-E0-91-02-07-3A", "192.168.1.100")); // ip
	// 지과장님 노트북.htm
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-0E-35-0F-11-DC", "192.168.1.100")); // ip
	// 운영팀 신영은
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-43", "221.148.70.150")); // ip
	// 운영팀 김진한
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-8B", "221.148.70.139")); // ip
	// 운영팀 김정수
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-88", "221.148.70.137")); // ip
	// 시스템팀 고승범씨
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-86", "221.148.70.180")); // ip
	// 운영팀 백경은 
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B6-89", "221.148.70.140")); // ip
	// GM파트 김건형
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B3-78", "221.148.70.147")); // ip
	// 스스템팀 박정우씨
	MacInfo_Map.insert(MACINFO_MAP::value_type("00-02-78-F7-B1-AB", "221.148.70.182")); // ip

	char szMyMac[32];
	sprintf(szMyMac, "%02X-%02X-%02X-%02X-%02X-%02X", g_macAddress[0], g_macAddress[1], g_macAddress[2], g_macAddress[3], g_macAddress[4], g_macAddress[5]);
	MACINFO_MAP::const_iterator itr = MacInfo_Map.find( szMyMac ); // 내 Mac Addresss가 있나 찾기.
	if(itr != MacInfo_Map.end())
	{
		std::string CheckIP = (itr->second);
		std::string MyIPAddress = GetLocalIP();
		if(0 == stricmp(MyIPAddress.c_str(), CheckIP.c_str()))
		{
			if(IDYES == MessageBox(g_hWnd, "윈도우 모드로 접속하시겠습니까?", "QUESTION", MB_YESNO|MB_ICONQUESTION))
				g_bFullScreen = false;
			else
			g_bFullScreen = true;
//yckou					g_bFullScreen = false;
			
			MacInfo_Map.clear();
			return ;
		}
	}
	MacInfo_Map.clear();
}
// 2004, 8, 26, sobeit add end - mac address 체크해서 윈도 모드 변경


// 2004, 04, 3 sobeit add start - 질드레 맵 이펙트 추가 , 삭제
// nEffect : 1 (위쪽 이펙트), 2 (아래쪽 이펙트)
// bAppearBossMonster : true( 맵 로딩시 혹은 보스몹이 죽었을 때) , false ( 보스몹이 나오는 순간 이펙트 )
void 
Add_GDR_Effect(int nEffect, bool bAppearBossMonster)
{
	if(1413 != g_pZone->GetID()) // 질드레 하드 only
		return;
	if(1 == nEffect) // 위쪽 이펙트
	{
		if(bAppearBossMonster) // 보스몹이 나오는 순간 이펙트
		{
//			MEvent event;
//			event.eventID = EVENTID_CONTINUAL_GROUND_ATTACK;
//			event.eventType = EVENTTYPE_ZONE;
//			event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
//			event.parameter2 = 30 << 16;
//			event.parameter3 = 5;
//			event.eventDelay = 4*1000;
//			g_pEventManager->AddEvent(event);

//			if (g_pUserOption->PlaySound)
//			{
//				PlaySound( SOUND_S_DIE, 
//					false,
//				g_pPlayer->GetX() + rand()%20-10, g_pPlayer->GetY() + rand()%20-10 );			
//
//			}
			g_pZone->RemoveTileEffect(136, 86,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(136, 86-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(136-1, 86-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			g_pZone->RemoveTileEffect(136+1, 86-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(136-1, 86,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(136+1, 86,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			g_pZone->RemoveTileEffect(136-1, 86+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(136+1, 86+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(136, 86+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			
			g_pZone->RemoveTileEffect(137, 80,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(143, 86,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(136, 93,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(130, 87,EFFECTSTATUS_BLACK_LARGE_SMOKE);

			ExecuteActionInfoFromMainNode(MAP_BLACK_HALL,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					136, 86, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					137, 80, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					143, 86, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					136, 93, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					130, 87, 0, 0, NULL, false);	
		}
		else // 기본 effect ( 보스몹이 죽거나 맵 로딩시)
		{
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136, 86, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136, 86-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136-1, 86-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136+1, 86-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136-1, 86, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136+1, 86, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136-1, 86+1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136+1, 86+1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				136, 86+1, 0, 0, NULL, false);			
			
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				137, 80, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				143, 86, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				136, 93, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				130, 87, 0, 0, NULL, false);	
		}
	}
	else if(2 == nEffect) // 아래쪽 이펙트
	{
		if(bAppearBossMonster) // 보스몹이 나오는 순간 이펙트
		{
			g_pZone->RemoveTileEffect(83, 139,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(83, 139-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(83-1, 139-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			g_pZone->RemoveTileEffect(83+1, 139-1,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(83-1, 139,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(83+1, 139,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			g_pZone->RemoveTileEffect(83-1, 139+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_1);
			g_pZone->RemoveTileEffect(83+1, 139+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_2);
			g_pZone->RemoveTileEffect(83, 139+1,EFFECTSTATUS_BLACK_SMALL_SMOKE_3);
			
			g_pZone->RemoveTileEffect(83, 134,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(89, 140,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(82, 147,EFFECTSTATUS_BLACK_LARGE_SMOKE);
			g_pZone->RemoveTileEffect(76, 141,EFFECTSTATUS_BLACK_LARGE_SMOKE);

			ExecuteActionInfoFromMainNode(MAP_BLACK_HALL,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					83, 139, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					83, 134, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					89, 140, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					82, 147, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_ELECTRIC_POST,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
					76, 141, 0, 0, NULL, false);	
		}
		else // 기본 effect ( 보스몹이 죽거나 맵 로딩시)
		{
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83, 139, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83, 139-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83-1, 139-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83+1, 139-1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83-1, 139, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83+1, 139, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_1,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83-1, 139+1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_2,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83+1, 139+1, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_SMALL_SMOKE_3,0, 0, 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
				83, 139+1, 0, 0, NULL, false);			
			
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				83, 134, 0, 0, NULL, false);								
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				89, 140, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				82, 147, 0, 0, NULL, false);						
			ExecuteActionInfoFromMainNode(MAP_BLACK_LARGE_SMOKE,0, 0, 0,g_pPlayer->GetDirection(),	0,	
				76, 141, 0, 0, NULL, false);
		}
	}
}
// 2004, 04, 3 sobeit add end - 질드레 맵 이펙트 추가

// 2004, 06, 26 sobeit add start - 일루전스 웨이 포탈
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

// 2004, 06, 26 sobeit add end - 일루전스 웨이 포탈

// 2004, 08, 6 sobeit add start - 각 종족별 몬스터 추가
void Add_Race_SlayerMonster(GCAddMonster * pPacket)
{
	MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());
	if(NULL == pCreature)
	{
		int CreatureType = pPacket->getMonsterType();

		MCreatureWear* pCreatureWear = new MCreatureWear;

		pCreatureWear->SetZone( g_pZone );

		pCreature = (MCreature*)pCreatureWear;

		if(CreatureType == 797)
			CreatureType = CREATURETYPE_SLAYER_FEMALE;
//		else
//			CreatureType = CREATURETYPE_SLAYER_MALE;
		pCreatureWear->SetCreatureType( CreatureType ); 
		pCreatureWear->SetGuildNumber( 1 );

		pCreatureWear->SetGroundCreature();
		pCreatureWear->SetID(pPacket->getObjectID());
		pCreatureWear->SetPosition( pPacket->getX(), pPacket->getY() );
		pCreatureWear->SetServerPosition( pPacket->getX(), pPacket->getY() );
		pCreatureWear->SetDirection( /*pPacket->getDir()*/2 );
		pCreatureWear->SetCurrentDirection( pPacket->getDir() );
		pCreatureWear->SetAction( ACTION_STAND );

		// 피부색
//			pCreatureWear->SetBodyColor1( si.getSkinColor() );
		pCreatureWear->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
		pCreatureWear->SetStatus( MODIFY_CURRENT_HP,  pPacket->getCurrentHP() );
//			pCreatureWear->SetStatus( MODIFY_ALIGNMENT, si.getAlignment() );
//			pCreatureWear->SetStatus( MODIFY_RANK, si.getRank() );

		// 이름
		pCreatureWear->SetName( pPacket->getMonsterName().c_str() );

		MItem* pCoat		= NULL;
		MItem* pTrouser		= NULL;
		MItem* pHelm		= NULL;
		MItem* pWeapon		= NULL;
		MItem* pShield		= NULL;
		MItem* pMotorcycle	= NULL;
		
		
		if(pPacket->getMonsterType() == 796 || pPacket->getMonsterType() == 797)
		{	
			pCoat		= g_pPacketItemJacket[JACKET_BASIC];
			pTrouser	= g_pPacketItemPants[PANTS_BASIC];
			pHelm		= g_pPacketItemHelm[HELMET_NONE];
			pWeapon		= g_pPacketItemWeapon[WEAPON_NONE];
			pShield		= g_pPacketItemShield[SHIELD_NONE];
			pMotorcycle	= g_pPacketItemMotorcycle[MOTORCYCLE_NONE];

			pCreatureWear->SetAddonColorSet1(ADDON_COAT, 393);
			pCreatureWear->SetAddonColorSet2(ADDON_COAT, 393);
			pCreatureWear->SetAddonColorSet1(ADDON_TROUSER, 393);
			pCreatureWear->SetAddonColorSet2(ADDON_TROUSER, 393);
		}
		else
		{
			//----------------------------------------	
			// 복장 - 암꺼나 입히자..-_-; 총은 sr
			//----------------------------------------	
			pCoat		= g_pPacketItemJacket[JACKET3];
			pTrouser		= g_pPacketItemPants[PANTS3];
			pHelm		= g_pPacketItemHelm[HELMET2];
			pWeapon		= g_pPacketItemWeapon[WEAPON_SR];
			pShield		= g_pPacketItemShield[SHIELD_NONE];
			pMotorcycle	= g_pPacketItemMotorcycle[MOTORCYCLE_NONE];
		}		
		pCreatureWear->SetAddonItem( pCoat );
		pCreatureWear->SetAddonItem( pTrouser );
		pCreatureWear->SetAddonItem( pHelm );
		pCreatureWear->SetAddonItem( pWeapon );
		pCreatureWear->SetAddonItem( pShield );
		pCreatureWear->SetAddonItem( pMotorcycle );


		if (!g_pZone->AddCreature( pCreatureWear ))
		{
			delete pCreatureWear;
			pCreatureWear = NULL;
			pCreature = NULL;
		}

		if(pPacket->getMonsterType() == 793)
		{
			pCreature->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 0xffff);
			ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,pCreature->GetX(), pCreature->GetY(), 0,2,	pCreature->GetID(),	
					pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
		}
	}
	else
	{
		pCreature->SetGuildNumber( 1 );

		pCreature->SetGroundCreature();
		pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
		pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
		pCreature->SetDirection( pPacket->getDir() );
		pCreature->SetCurrentDirection( pPacket->getDir() );
		pCreature->SetAction( ACTION_STAND );

		// 피부색
//		pCreature->SetBodyColor1( si.getSkinColor() );

		pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
		pCreature->SetStatus( MODIFY_CURRENT_HP,  pPacket->getCurrentHP() );
//			pCreature->SetStatus( MODIFY_ALIGNMENT, si.getAlignment() );
//			pCreature->SetStatus( MODIFY_RANK, si.getRank() );

		if(pPacket->getMonsterType() == 793)
		{
			pCreature->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 0xffff);
			ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,pCreature->GetX(), pCreature->GetY(), 0,2,	pCreature->GetID(),	
					pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
		}
	}
}
void Add_Race_OustersMonster(GCAddMonster * pPacket)
{
	MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());
	if (pCreature==NULL)
	{
		int CreatureType = pPacket->getMonsterType();
		MCreatureWear* pCreatureWear = new MCreatureWear;

		pCreature = (MCreature*)pCreatureWear;

		pCreature->SetZone( g_pZone );

		pCreature->SetName( pPacket->getMonsterName().c_str() );

		//--------------------------------------------------
		// CreatureType 설정
		//--------------------------------------------------
		//ItemType_t coatType = oi.getCoatType();

		pCreature->SetCreatureType( CreatureType );

		pCreature->SetID(pPacket->getObjectID());
		//pCreature->SetAction(ACTION_MOVE);
		pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
		pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
		
		pCreature->SetDirection( /*pPacket->getDir()*/2 );
		pCreature->SetCurrentDirection( pPacket->getDir() );
		pCreature->SetAction( ACTION_STAND );
		// 색깔
//			pCreature->SetBodyColor1( oi.getHairColor() );
//			pCreature->SetBodyColor2( oi.getCoatColor() );

		
		pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
		pCreature->SetStatus( MODIFY_CURRENT_HP,  pPacket->getCurrentHP() );
//			pCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );
//			pCreature->SetStatus( MODIFY_RANK, oi.getRank() );
	
		pCreature->SetWeaponSpeed( 40 );

		pCreature->SetGuildNumber( 1 );

		if(CreatureType == 800)
		{
			MItem* pCoat		= g_pPacketItemOustersCoat[OUSTERS_COAT_BASIC];
			MItem* pArm			= g_pPacketItemOustersArm[OUSTERS_ARM_CHAKRAM];	

			pCreatureWear->SetAddonItem( pCoat );
			pCreatureWear->SetAddonItem( pArm );

			pCreatureWear->SetAddonColorSet1(ADDON_COAT, 393);
			pCreatureWear->SetAddonColorSet2(ADDON_COAT, 393);
			pCreatureWear->SetAddonColorSet1(ADDON_RIGHTHAND, 393);
			pCreatureWear->SetAddonColorSet2(ADDON_RIGHTHAND, 393);
			
			pCreature->SetBodyColor1( 393 );
			pCreature->SetBodyColor2( 393 );
			pCreature->SetFade();

		}
		
		if (!g_pZone->AddCreature( pCreature ))
		{
			delete pCreature;
			pCreature = NULL;
		}
		if(pPacket->getMonsterType() == 795)
		{
			pCreature->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 0xffff);
			ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,pCreature->GetX(), pCreature->GetY(), 0,2,	pCreature->GetID(),	
					pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
		}
		
	}
	//--------------------------------------------------
	// 이미 있는 Creature인 경우
	//--------------------------------------------------
	else
	{
		//--------------------------------------------------
		// CreatureType 설정
		//--------------------------------------------------
	//	ItemType_t coatType = oi.getCoatType();

	//	pCreature->SetCreatureType( CREATURETYPE_OUSTERS );

		// 임시로
		pCreature->SetGuildNumber( 1 );
		
		//pCreature->SetAction(ACTION_MOVE);
		pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
		pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
		pCreature->SetDirection( /*pPacket->getDir()*/2 );
		pCreature->SetCurrentDirection( pPacket->getDir() );
		pCreature->SetAction( ACTION_STAND );

		// 색깔
//			pCreature->SetBodyColor1( oi.getHairColor() );
//			pCreature->SetBodyColor2( oi.getCoatColor() );


		//--------------------------------------------------
		// [ TEST CODE ]
		//--------------------------------------------------
		// 옷 색깔 설정하기
		//--------------------------------------------------
		/*
		if (pCreature->IsMale())
		{
			pCreature->SetBodyColor2( 91 );
		}
		else
		{
			pCreature->SetBodyColor2( 38 );
		}
		*/

		pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
		pCreature->SetStatus( MODIFY_CURRENT_HP,  pPacket->getCurrentHP() );
//			pCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );
//			pCreature->SetStatus( MODIFY_RANK, oi.getRank() );

		pCreature->SetWeaponSpeed(40);

		if(pPacket->getMonsterType() == 795)
		{
			pCreature->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 0xffff);
			ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,pCreature->GetX(), pCreature->GetY(), 0,2,	pCreature->GetID(),	
					pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
		}
		
	}
}

// 2004, 8, 26, sobeit add start - nProtct 인증 패킷
void 
Send_nProtect_Auth(DWORD dwVal)
{
	CGAuthKey _CGAuthKey;
	_CGAuthKey.setKey(dwVal);

	if( g_pSocket != NULL )
	{
		g_pSocket->sendPacket( &_CGAuthKey );
//		MessageBox(g_hWnd, "_CGAuthKey보냄", "Warning", MB_OK);
	}
}
// 2004, 8, 26, sobeit add end - nProtct 인증 패킷

POINT GetNextTileByDirection(int TileX, int TileY, byte Dir)
{
	POINT FinalTile;
	FinalTile.x = TileX; 
	FinalTile.y = TileY;
	switch (Dir)
	{
		case DIRECTION_LEFTDOWN		: FinalTile.x--;	FinalTile.y++;	break;
		case DIRECTION_RIGHTUP		: FinalTile.x++;	FinalTile.y--;	break;
		case DIRECTION_LEFTUP		: FinalTile.x--;	FinalTile.y--;	break;
		case DIRECTION_RIGHTDOWN	: FinalTile.x++;	FinalTile.y++;	break;
		case DIRECTION_LEFT			: FinalTile.x--;				break;
		case DIRECTION_DOWN			:			FinalTile.y++;	break;
		case DIRECTION_UP			:			FinalTile.y--;	break;
		case DIRECTION_RIGHT		: FinalTile.x++;				break;
	}

	return FinalTile;
}
// 2004, 11, 16, sobeit add start
void Add_RocketRuncher(MCreature* UserCreature, MCreature* TargetCreature)
{
	if(NULL == UserCreature || NULL == TargetCreature)
		return;
	int TempDir = MTopView::GetDirectionToPosition(UserCreature->GetX(), UserCreature->GetY(), TargetCreature->GetX(), TargetCreature->GetY());
	POINT _Tile = GetNextTileByDirection(UserCreature->GetX(), UserCreature->GetY(), TempDir);

	MFakeCreature *pFakeCreature = NewFakeCreature(CREATURETYPE_ROCKET_LUNCHER, _Tile.x, _Tile.y, UserCreature->GetDirection());
	int delayFrame = max(abs(TargetCreature->GetX()-_Tile.x), abs(TargetCreature->GetY()-_Tile.y)) * 250;

	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}
	else
	{
		pFakeCreature->SetZone(g_pZone);
		pFakeCreature->SetFakeCreatureType(MFakeCreature::FAKE_CREATURE_ROCKET);
		pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
		pFakeCreature->AddEffectStatus( (EFFECTSTATUS)(EFFECTSTATUS_ROCKET_LUNCHER), 0xFFFF );
		pFakeCreature->SetTraceID(TargetCreature->GetID());
		pFakeCreature->SetTime(timeGetTime() + delayFrame); 
		pFakeCreature->SetGhost(0xff, 2);
	}
}

// 2004, 11, 16, sobeit add end

// 2005, 1, 3, sobeit add start - wild wolf
void Add_Wild_Wolf(MCreature* UserCreature, MCreature* TargetCreature, bool IsEatCorpse)
{
	if(NULL == UserCreature || NULL == TargetCreature)
		return;
	int TempDir = MTopView::GetDirectionToPosition(UserCreature->GetX(), UserCreature->GetY(), TargetCreature->GetX(), TargetCreature->GetY());
	POINT StartTile = GetNextTileByDirection(UserCreature->GetX(), UserCreature->GetY(), TempDir);
//	POINT EndTile = GetNextTileByDirection(TargetCreature->GetX(), TargetCreature->GetY(), TempDir);
	
	UserCreature->SetDirection(TempDir);
//	int TempDir = MTopView::GetDirectionToPosition(g_pPlayer->GetX()+5, g_pPlayer->GetY()-5, g_pPlayer->GetX(), g_pPlayer->GetY());
//	POINT StartTile = GetNextTileByDirection(g_pPlayer->GetX()+5, g_pPlayer->GetY()-5, TempDir);
//	POINT EndTile = GetNextTileByDirection(g_pPlayer->GetX(), g_pPlayer->GetY(), TempDir);


	MFakeCreature *pFakeCreature = NewFakeCreature(CREATURETYPE_WILD_WOLF, StartTile.x, StartTile.y, TempDir);

	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}
	else
	{
		pFakeCreature->SetZone(g_pZone);
		pFakeCreature->SetFakeCreatureType(MFakeCreature::FAKE_CREATURE_WILD_WOLF);
		pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
		pFakeCreature->SetTraceID(TargetCreature->GetID());
//		pFakeCreature->SetTraceID(g_pPlayer->GetID());
		pFakeCreature->SetIsEatCorpse(IsEatCorpse);
		
		pFakeCreature->SetTargetTile(TargetCreature->GetX(), TargetCreature->GetY());
	}
}
// 2005, 1, 3, sobeit add end

// 2005, 1, 6, sobeit add start - dragon tornado
void SetDragonTorando(int Type, DWORD ObjectID, int TileX, int TileY)
{
	if(g_pZone == NULL) return;
	MFakeCreature *pFakeCreature = (MFakeCreature*)(g_pZone->GetFakeCreature(ObjectID));
	if(pFakeCreature == NULL)
	{
		// 없으면 생성
		int TempCreatureType = Type==EFFECTSTATUS_DRAGON_TORNADO? 788:789;
		pFakeCreature = NewFakeCreature(TempCreatureType, TileX, TileY, 0);
		pFakeCreature->SetID(ObjectID);
		if (!g_pZone->AddFakeCreature( pFakeCreature ))
		{
			delete pFakeCreature;
		}
		else
		{
			pFakeCreature->SetZone(g_pZone);
			pFakeCreature->SetFakeCreatureType(MFakeCreature::FAKE_CREATURE_DRAGON_TORNADO);
			pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
			pFakeCreature->AddEffectStatus((EFFECTSTATUS)Type, 0xffff);
			pFakeCreature->SetAction(ACTION_STAND);
		}
	}
	else
	{
		// 좌표가 틀리면 이동
		if(TileX != pFakeCreature->GetX() || TileY != pFakeCreature->GetY())
		{
			int TempDir = pFakeCreature->GetDirectionToPosition(TileX, TileY);
			pFakeCreature->SetFakeMoveOnce(TempDir);
		}
	}
}
// 2005, 1, 6, sobeit add end
#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관

void SetSubInventoryInfo(InventoryInfo* pInventoryInfo)
{
	DEBUG_ADD( "Set SubInventory Info" );
	
	if(pInventoryInfo == NULL) return ;
	int itemNum = pInventoryInfo->getListNum();

	//--------------------------------------------------
	// Inventory에 아이템들을 추가한다.
	//--------------------------------------------------
	for (int i=0; i<itemNum; i++)
	{
		// 외부에서 지워줘야 한다.
		InventorySlotInfo * pSlotInfo = pInventoryInfo->popFrontListElement();

		if( pSlotInfo == NULL )
			continue;

		if( pSlotInfo->getItemClass() >= g_pItemTable->GetSize() ||
			pSlotInfo->getItemType() >= (*g_pItemTable)[pSlotInfo->getItemClass()].GetSize() )
		{
			SendBugReport("[%s] %d,%d",__FILE__, pSlotInfo->getItemClass(), pSlotInfo->getItemType());
			delete pSlotInfo;
			continue;
		}

		//------------------------------------------------
		// Item을 생성해서 Inventory에 추가한다.
		//------------------------------------------------
		MItem* pItem = MItem::NewItem( (ITEM_CLASS)pSlotInfo->getItemClass() );
		
		// object ID
		pItem->SetID( pSlotInfo->getObjectID() );

		// type / option
		pItem->SetItemType(	pSlotInfo->getItemType() );
		pItem->SetItemOptionList( pSlotInfo->getOptionType() );

		// inventory에서의 좌표
		pItem->SetGridXY( pSlotInfo->getInvenX(), pSlotInfo->getInvenY() );

		//------------------------------------------
		// 개수
		//------------------------------------------
		// 총인 경우
		//------------------------------------------
		if (pItem->IsGunItem())
		{
			MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

			// 의미 없음 - -;
			pMagazine->SetID( 0 );

			// 이거는 총에 맞춰서 해줘야된다.
			for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
			{
				pMagazine->SetItemType(	j );

				if (pMagazine->IsInsertToItem( pItem ))
				{
					break;
				}
			}

			if(pSlotInfo->getSilver())
				pMagazine->SetItemType( pMagazine->GetItemType()+8);
			// 어예~ 은탄환 하드코딩 by 쑥갓

			// 의미 없음
			pMagazine->ClearItemOption();
		
			// 탄창 개수
			pMagazine->SetNumber( pSlotInfo->getItemNum() );

			//------------------------------------
			// 탄창 설정
			//------------------------------------
			MGunItem* pGunItem = (MGunItem*)pItem;
			pGunItem->SetMagazine( pMagazine );
		}		
		//------------------------------------------
		// 총이 아닌 경우
		//------------------------------------------
		else
		{
			pItem->SetNumber( pSlotInfo->getItemNum() );
		}


		//------------------------------------------
		//
		// Item에 다른 item들이 들어있는 경우
		//
		//------------------------------------------
		if (pSlotInfo->getListNum()!=0)
		{
			DEBUG_ADD_FORMAT("This Item has Sub item(s) : size=%d", pSlotInfo->getListNum());
			
			//------------------------------------------
			// Belt인 경우
			//------------------------------------------
			if (pItem->GetItemClass()==ITEM_CLASS_BELT)
			{
				MBelt* pBelt = (MBelt*)pItem;

				int size = pSlotInfo->getListNum();
				
				for (int i=0; i<size; i++)
				{
					SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();

					if (pSubItemInfo==NULL)
					{
						DEBUG_ADD("[Error] Sub Item is NULL");
					}
					else
					{
						//------------------------------------------
						// Sub Item의 정보를 설정한다.
						//------------------------------------------
						if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
							(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
						{
							SendBugReport("[PacketFunction::SetInventoryInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
							return;
						}
						MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
						pSubItem->SetItemType( pSubItemInfo->getItemType() );
						//pItem->SetItemOption( pSubItemInfo->getOptionType() );

						pSubItem->SetID( pSubItemInfo->getObjectID() );		

						pSubItem->SetNumber( pSubItemInfo->getItemNum() );			

						//------------------------------------------
						// Belt의 정해진 slot에 item을 추가시킨다.
						//------------------------------------------
						pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );

						delete pSubItemInfo;
					}
				} 
			}
			else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
			{
				MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;
				
				int size = pSlotInfo->getListNum();
				
				for (int i=0; i<size; i++)
				{
					SubItemInfo * pSubItemInfo = pSlotInfo->popFrontListElement();
					
					if (pSubItemInfo==NULL)
					{
						DEBUG_ADD("[Error] Sub Item is NULL");
					}
					else
					{
						//------------------------------------------
						// Sub Item의 정보를 설정한다.
						//------------------------------------------
						if ( pSubItemInfo->getItemClass() >= g_pItemTable->GetSize() ||
							(*g_pItemTable)[pSubItemInfo->getItemClass()].GetSize() <= pSubItemInfo->getItemType() )
						{
							SendBugReport("[PacketFunction::SetInventoryInfo,%d] %d,%d",__LINE__,pSubItemInfo->getItemClass(), pSubItemInfo->getItemType() );
							return;
						}
						MItem* pSubItem = MItem::NewItem( (enum ITEM_CLASS)pSubItemInfo->getItemClass() );
						pSubItem->SetItemType( pSubItemInfo->getItemType() );
						//pItem->SetItemOption( pSubItemInfo->getOptionType() );
						
						pSubItem->SetID( pSubItemInfo->getObjectID() );		
						
						pSubItem->SetNumber( pSubItemInfo->getItemNum() );			
						
						//------------------------------------------
						// Belt의 정해진 slot에 item을 추가시킨다.
						//------------------------------------------
						pBelt->AddItem( pSubItem, pSubItemInfo->getSlotID() );
						
						delete pSubItemInfo;
					}
				}
			}
			else
			{
				DEBUG_ADD_FORMAT("This Item can't have Sub item : class=%d", (int)pItem->GetItemClass());
			}
		}


		// main색깔
		pItem->SetItemColorSet( pSlotInfo->getMainColor() );

		// 현재 내구성
		pItem->SetCurrentDurability( pSlotInfo->getDurability() );
		pItem->SetSilver( pSlotInfo->getSilver() );
		pItem->SetGrade( pSlotInfo->getGrade() );
		pItem->SetEnchantLevel( pSlotInfo->getEnchantLevel() );

		//---------------------------------------------
		// item을 inventory에 넣는다.
		//---------------------------------------------
		if(false == UI_AddItemToSubInventory(pItem, pItem->GetGridX(), pItem->GetGridY()))
		{
			// item을 inventory에 추가할 수 없는 경우
			DEBUG_ADD_FORMAT("[Error] Can't Add Item to Inventory. id=%d, cl=%d, tp=%d, xy=(%d,%d)", 					
											(int)pItem->GetID(),
											(int)pItem->GetItemClass(),
											(int)pItem->GetItemType(),
											(int)pItem->GetGridX(),
											(int)pItem->GetGridY());
			
			delete pItem;
		}

		delete pSlotInfo;
	}
}

#endif

