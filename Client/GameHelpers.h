//----------------------------------------------------------------------
// GameHelpers.h
//
// Forward declarations for game helper functions
//----------------------------------------------------------------------

#ifndef __GAMEHELPERS_H__
#define __GAMEHELPERS_H__

#include "Types.h"
#include "PacketInfo.h"
#include "ModifyInfo.h"
#include "EffectInfo.h"
#include "PetInfo.h"
#include "GearInfo.h"
#include "ExtraInfo.h"
#include "InventoryInfo.h"
#include "PCSlayerInfo2.h"
#include "PCOustersInfo2.h"
#include "PCVampireInfo2.h"
#include "PCSlayerInfo3.h"
#include "PCOustersInfo3.h"
#include "BloodBibleSignInfo.h"
#include "MStatus.h"
#include "MActionResult.h"
#include "GCAddMonster.h"
#include "GCAddItemToZone.h"
#include "VS_UI/src/widget/LineEditor.h"

// Forward declarations
class MCreature;
class MItem;
template<typename T> class COrderedList;

// Client mode enum
enum CLIENT_MODE {
	CLIENT_MODE_WINDOW,
	CLIENT_MODE_FULLSCREEN,
	CLIENT_MODE_WINDOW_1024,
	CLIENT_MODE_FULLSCREEN_1024
};

// Global variable
extern int _NPGAMEMON_SUCCESS;

// Pet and Info functions
void SetPetInfo(PetInfo* pInfo, unsigned int flag);
void SetGearInfo(GearInfo* pGear, BloodBibleSignInfo* pBible);
void SetExtraInfo(ExtraInfo* pInfo);
void SetInventoryInfo(InventoryInfo* pInfo);
void SetBloodBibleSlot(BloodBibleSignInfo* pInfo);
void SetEffectInfo(MCreature* pCreature, EffectInfo* pInfo, int count);

// Player info functions
void SetPCSlayerInfo(PCSlayerInfo2* pInfo);
void SetPCOustersInfo(PCOustersInfo2* pInfo);
void SetPCVampireInfo(PCVampireInfo2* pInfo);

// Addon functions
void SetAddonToSlayer(MCreatureWear* pWear, int value);
void SetAddonToSlayer(MCreatureWear* pWear, const PCSlayerInfo3* pInfo);
void SetAddonToOusters(MCreatureWear* pWear, const PCOustersInfo3* pInfo);
void AddOustersElemental(MCreature* pCreature, int type, int value);

// Creature functions
void NewFakeCreature(MCreature* pCreature, int type, int flag);
void MoveIntoPortal(MCreature* pCreature);
void ComeFromPortal(MCreature* pCreature);
void Add_Wild_Wolf(MCreature* pCreature, MCreature* pMaster, bool flag);
void Add_RocketRuncher(MCreature* pCreature, MCreature* pTarget);
void Add_Race_SlayerMonster(GCAddMonster* pPacket);
void Add_Race_OustersMonster(GCAddMonster* pPacket);
void AddItemToZone(GCAddItemToZone* pPacket, int layer);

// Skill functions
void SkillToSector(WORD type, WORD x, WORD y, int dir, DWORD flag, MActionResult* pResult);
void SkillShadowDancing(MCreature* pCreature, MCreature* pTarget, int type);
void SkillIllendue(MCreature* pCreature, MCreature* pTarget, int type);
void SkillCrossCounter(MCreature* pCreature, MCreature* pTarget, int type);
void SkillBlazeWalk(MCreature* pCreature, MCreature* pTarget, int type, BYTE flag);

// Action and result functions
void ExecuteActionInfoFromMainNode(WORD type1, WORD type2, WORD type3, int val1, int val2,
	UINT flag, WORD x1, WORD y1, int dir, DWORD flag2, MActionResult* pResult,
	bool bFlag, int param1, int param2);
void CreateActionResultNode(MCreature* pCreature, int type, BYTE flag);

// Status functions
void AffectModifyInfo(MStatus* pStatus, ModifyInfo* pInfo);
void ReleaseUselessCreatureSPKExcept(const COrderedList<int>& list);

// Utility functions
void CheckItemForSkillIcon(const MItem* pItem);
bool GetMakeItemFitPosition(MItem* pItem, ITEM_CLASS cls, int type, tagPOINT& point);

// Mode and display functions
void SetMode(CLIENT_MODE mode);
void SetFadeStart(char r, char g, char b, BYTE r2, BYTE g2, BYTE b2, WORD speed);

// Print functions
void g_Print(int x, int y, const char* str, PrintInfo* pInfo);
void g_PrintColorStr(int x, int y, const char* str, PrintInfo& info, DWORD color);
void g_PrintColorStrOut(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2);
void g_PrintColorStrShadow(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2);

// Exception handler
void g_BasicException(EXCEPTION_CODE code, char* file, char* func, DWORD line);

// Drawing functions
void FillRect(S_SURFACEINFO* pInfo, Rect* pRect, int color);
void rectangle(S_SURFACEINFO* pInfo, Rect* pRect, int color);

#endif // __GAMEHELPERS_H__
