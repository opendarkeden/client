//----------------------------------------------------------------------
// GameHelpers.cpp
//
// Stub implementations for game helper functions
// These are placeholder implementations for macOS/Linux compatibility
//----------------------------------------------------------------------

#include "Client_PCH.h"

// Forward declarations for incomplete types
class MCreature;
class MItem;
class MStatus;
class MActionResult;
template<typename T> class COrderedList;

// Packet info forward declarations
struct PetInfo;
struct GearInfo;
struct ExtraInfo;
struct InventoryInfo;
struct EffectInfo;
struct BloodBibleSignInfo;
struct ModifyInfo;
struct PCSlayerInfo2;
struct PCOustersInfo2;
struct PCVampireInfo2;
struct PCSlayerInfo3;
struct PCOustersInfo3;
struct GCAddMonster;
struct GCAddItemToZone;
struct PrintInfo;

// Client mode enum
enum CLIENT_MODE {
	CLIENT_MODE_WINDOW,
	CLIENT_MODE_FULLSCREEN,
	CLIENT_MODE_WINDOW_1024,
	CLIENT_MODE_FULLSCREEN_1024
};

// Note: DWORD, WORD, BYTE, etc. are defined in Platform.h
// No need to redefine them here

// Forward declarations only - types are defined in Platform.h
struct Rect;

// MCreatureWear forward declaration
class MCreatureWear;

// Item class enum placeholder
enum ITEM_CLASS { ITEM_CLASS_MAX = 0 };

// _NPGAMEMON_SUCCESS is defined in nProtectStub.cpp
extern int _NPGAMEMON_SUCCESS;

// Pet and Info functions
void SetPetInfo(PetInfo* pInfo, unsigned int flag)
{
	(void)pInfo; (void)flag;
}

void SetGearInfo(GearInfo* pGear, BloodBibleSignInfo* pBible)
{
	(void)pGear; (void)pBible;
}

void SetExtraInfo(ExtraInfo* pInfo)
{
	(void)pInfo;
}

void SetInventoryInfo(InventoryInfo* pInfo)
{
	(void)pInfo;
}

void SetBloodBibleSlot(BloodBibleSignInfo* pInfo)
{
	(void)pInfo;
}

void SetEffectInfo(MCreature* pCreature, EffectInfo* pInfo, int count)
{
	(void)pCreature; (void)pInfo; (void)count;
}

// Player info functions
void SetPCSlayerInfo(PCSlayerInfo2* pInfo)
{
	(void)pInfo;
}

void SetPCOustersInfo(PCOustersInfo2* pInfo)
{
	(void)pInfo;
}

void SetPCVampireInfo(PCVampireInfo2* pInfo)
{
	(void)pInfo;
}

// Addon functions
void SetAddonToSlayer(MCreatureWear* pWear, int value)
{
	(void)pWear; (void)value;
}

void SetAddonToSlayer(MCreatureWear* pWear, const PCSlayerInfo3* pInfo)
{
	(void)pWear; (void)pInfo;
}

void SetAddonToOusters(MCreatureWear* pWear, const PCOustersInfo3* pInfo)
{
	(void)pWear; (void)pInfo;
}

void AddOustersElemental(MCreature* pCreature, int type, int value)
{
	(void)pCreature; (void)value; (void)type;
}

// Creature functions
void NewFakeCreature(MCreature* pCreature, int type, int flag)
{
	(void)pCreature; (void)type; (void)flag;
}

void MoveIntoPortal(MCreature* pCreature)
{
	(void)pCreature;
}

void ComeFromPortal(MCreature* pCreature)
{
	(void)pCreature;
}

void Add_Wild_Wolf(MCreature* pCreature, MCreature* pMaster, bool flag)
{
	(void)pCreature; (void)pMaster; (void)flag;
}

void Add_RocketRuncher(MCreature* pCreature, MCreature* pTarget)
{
	(void)pCreature; (void)pTarget;
}

void Add_Race_SlayerMonster(GCAddMonster* pPacket)
{
	(void)pPacket;
}

void Add_Race_OustersMonster(GCAddMonster* pPacket)
{
	(void)pPacket;
}

void AddItemToZone(GCAddItemToZone* pPacket, int layer)
{
	(void)pPacket; (void)layer;
}

// Skill functions
void SkillToSector(WORD type, WORD x, WORD y, int dir, DWORD flag, MActionResult* pResult)
{
	(void)type; (void)x; (void)y; (void)dir; (void)flag; (void)pResult;
}

void SkillShadowDancing(MCreature* pCreature, MCreature* pTarget, int type)
{
	(void)pCreature; (void)pTarget; (void)type;
}

void SkillIllendue(MCreature* pCreature, MCreature* pTarget, int type)
{
	(void)pCreature; (void)pTarget; (void)type;
}

void SkillCrossCounter(MCreature* pCreature, MCreature* pTarget, int type)
{
	(void)pCreature; (void)pTarget; (void)type;
}

void SkillBlazeWalk(MCreature* pCreature, MCreature* pTarget, int type, BYTE flag)
{
	(void)pCreature; (void)pTarget; (void)type; (void)flag;
}

// Action and result functions
void ExecuteActionInfoFromMainNode(WORD type1, WORD type2, WORD type3, int val1, int val2,
	UINT flag, WORD x1, WORD y1, int dir, DWORD flag2, MActionResult* pResult,
	bool bFlag, int param1, int param2)
{
	(void)type1; (void)type2; (void)type3; (void)val1; (void)val2;
	(void)flag; (void)x1; (void)y1; (void)dir; (void)flag2;
	(void)pResult; (void)bFlag; (void)param1; (void)param2;
}

void CreateActionResultNode(MCreature* pCreature, int type, BYTE flag)
{
	(void)pCreature; (void)type; (void)flag;
}

// Status functions
void AffectModifyInfo(MStatus* pStatus, ModifyInfo* pInfo)
{
	(void)pStatus; (void)pInfo;
}

void ReleaseUselessCreatureSPKExcept(const COrderedList<int>& list)
{
	(void)list;
}

// Utility functions
void CheckItemForSkillIcon(const MItem* pItem)
{
	(void)pItem;
}

bool GetMakeItemFitPosition(MItem* pItem, ITEM_CLASS cls, int type, tagPOINT& point)
{
	(void)pItem; (void)cls; (void)type; (void)point;
	return false;
}

// Mode and display functions
// SetMode() is implemented in GameMain.cpp - this stub removed to avoid duplicate symbol
// void SetMode(CLIENT_MODE mode)
// {
// 	(void)mode;
// }

void SetFadeStart(char r, char g, char b, BYTE r2, BYTE g2, BYTE b2, WORD speed)
{
	(void)r; (void)g; (void)b; (void)r2; (void)g2; (void)b2; (void)speed;
}

// Print functions
void g_Print(int x, int y, const char* str, PrintInfo* pInfo)
{
	(void)x; (void)y; (void)str; (void)pInfo;
}

void g_PrintColorStr(int x, int y, const char* str, PrintInfo& info, DWORD color)
{
	(void)x; (void)y; (void)str; (void)info; (void)color;
}

void g_PrintColorStrOut(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
}

void g_PrintColorStrShadow(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
}

// g_BasicException is defined in BasicException.cpp
// g_SetNewHandler is defined in BasicException.cpp

// Drawing functions
void FillRect(S_SURFACEINFO* pInfo, Rect* pRect, int color)
{
	(void)pInfo; (void)pRect; (void)color;
}

void rectangle(S_SURFACEINFO* pInfo, Rect* pRect, int color)
{
	(void)pInfo; (void)pRect; (void)color;
}
