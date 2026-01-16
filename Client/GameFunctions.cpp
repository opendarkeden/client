//----------------------------------------------------------------------
// GameFunctions.cpp - macOS implementation of missing game functions
//----------------------------------------------------------------------
#include "../Client_PCH.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//----------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------
static bool g_bGoreLevel = false;
static int g_nWeather = 0;
static int g_nWeatherLevel = 0;

//----------------------------------------------------------------------
// Player Initialization
//----------------------------------------------------------------------
void InitPlayer(int race, int sex, int zoneID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Pet Info
//----------------------------------------------------------------------
void SetPetInfo(void* pPetInfo, unsigned int ownerID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Weather
//----------------------------------------------------------------------
void SetWeather(int weather, int level)
{
    g_nWeather = weather;
    g_nWeatherLevel = level;
}

//----------------------------------------------------------------------
// Gear Info
//----------------------------------------------------------------------
void SetGearInfo(void* pGearInfo, void* pBibleInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Extra Info
//----------------------------------------------------------------------
void SetExtraInfo(void* pExtraInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Effect Info
//----------------------------------------------------------------------
void SetEffectInfo(void* pCreature, void* pEffectInfo, int effectID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Inventory Info
//----------------------------------------------------------------------
void SetInventoryInfo(void* pInventoryInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// PC Slayer Info
//----------------------------------------------------------------------
void SetPCSlayerInfo(void* pInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// PC Vampire Info
//----------------------------------------------------------------------
void SetPCVampireInfo(void* pVampInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// PC Ousters Info
//----------------------------------------------------------------------
void SetPCOustersInfo(void* pOustInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Load Creature
//----------------------------------------------------------------------
void LoadCreature(int creatureID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Load Zone
//----------------------------------------------------------------------
void LoadZone(int zoneID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Load Zone Info
//----------------------------------------------------------------------
void LoadZoneInfo(int zoneID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Move Zone
//----------------------------------------------------------------------
void MoveZone(int zoneID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Add Item to Zone
//----------------------------------------------------------------------
void AddItemToZone(void* pItem, int zoneID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Fake Creature
//----------------------------------------------------------------------
void NewFakeCreature(void* pCreature, int X, int Y)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Portal
//----------------------------------------------------------------------
void ComeFromPortal(void* pCreature)
{
    // Stub implementation
}

void MoveIntoPortal(void* pCreature)
{
    // Stub implementation
}

void AddVampirePortal(int zoneID, const char* pName, unsigned short X, unsigned short Y,
                      unsigned long ownerID, int clanID, unsigned short portalID,
                      unsigned short portalType, bool bVisible)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Gore Level
//----------------------------------------------------------------------
void SetGoreLevel(bool bEnable)
{
    g_bGoreLevel = bEnable;
}

//----------------------------------------------------------------------
// Fade Effect
//----------------------------------------------------------------------
void SetFadeStart(char flag, char R, char G, char B, char R2, char G2, unsigned short duration)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Lightning
//----------------------------------------------------------------------
void SetLightning(unsigned long color)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Dragon Tornado (note: spelling "Torando" is intentional from original code)
//----------------------------------------------------------------------
void SetDragonTorando(int type, unsigned long color, int count, int delay)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// GDR Effect
//----------------------------------------------------------------------
void Add_GDR_Effect(int effectID, bool bEnable)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Wild Wolf
//----------------------------------------------------------------------
void Add_Wild_Wolf(void* pOwner, void* pTarget, bool bAdd)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Rocket Launcher (note: spelling "Runcher" is intentional)
//----------------------------------------------------------------------
void Add_RocketRuncher(void* pOwner, void* pTarget)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Skills
//----------------------------------------------------------------------
void SkillIllendue(void* pOwner, void* pTarget, int level)
{
    // Stub implementation
}

void SkillBlazeWalk(void* pOwner, void* pTarget, int count, unsigned char dir)
{
    // Stub implementation
}

void SkillShadowDancing(void* pOwner, void* pTarget, int level)
{
    // Stub implementation
}

void SkillCrossCounter(void* pOwner, void* pTarget, int level)
{
    // Stub implementation
}

void SkillToSector(unsigned short skillType, unsigned short X, unsigned short Y,
                   int count, unsigned long delay, void* pResult)
{
    // Stub implementation
}

void AddOustersElemental(void* pCreature, int elementalType, int count)
{
    // Stub implementation
}

void Add_Race_SlayerMonster(void* pMonster)
{
    // Stub implementation
}

void Add_Race_OustersMonster(void* pMonster)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Status Modify
//----------------------------------------------------------------------
void AffectModifyInfo(void* pStatus, void* pModifyInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Check Activate
//----------------------------------------------------------------------
bool CheckActivate(int skillID)
{
    return false;
}

//----------------------------------------------------------------------
// Action Result
//----------------------------------------------------------------------
void CreateActionResultNode(void* pCreature, int type, unsigned char count)
{
    // Stub implementation
}

void ExecuteActionInfoFromMainNode(unsigned short skillType, unsigned short X, unsigned short Y,
                                    int targetID, int count, unsigned int flag,
                                    unsigned short duration, unsigned short delay,
                                    int effectID, unsigned long color,
                                    void* pResult, bool bContinue,
                                    int continueX, int continueY)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Bomb/Mine Action
//----------------------------------------------------------------------
void* GetBombActionInfo(int bombID)
{
    return NULL;
}

void* GetMineActionInfo(int mineID)
{
    return NULL;
}

//----------------------------------------------------------------------
// Creature Type
//----------------------------------------------------------------------
int GetVampireCreatureType(int level, bool bFemale, int rank)
{
    return 0;
}

int GetOustersCreatureType(int level)
{
    return 0;
}

//----------------------------------------------------------------------
// Direction
//----------------------------------------------------------------------
unsigned char GetNextTileByDirection(int currentX, int currentY, unsigned char dir)
{
    return 0;
}

int GetDirectionToPosition(int fromX, int fromY, int toX, int toY)
{
    return 0;
}

//----------------------------------------------------------------------
// Item Position
//----------------------------------------------------------------------
bool GetMakeItemFitPosition(void* pItem, int itemClass, int maxWidth, POINT& pt)
{
    return false;
}

//----------------------------------------------------------------------
// Check Item for Skill Icon
//----------------------------------------------------------------------
bool CheckItemForSkillIcon(const void* pItem)
{
    return false;
}

//----------------------------------------------------------------------
// Conversion Functions
//----------------------------------------------------------------------
int ConvertAlignment(int alignment)
{
    return alignment;
}

int ConvertDurationToFrame(int duration)
{
    return duration;
}

int ConvertDurationToMillisecond(int duration)
{
    return duration;
}

int ConvertMillisecondToFrame(unsigned long ms)
{
    return (int)(ms / 30);
}

//----------------------------------------------------------------------
// Time Functions
//----------------------------------------------------------------------
bool CheckTime()
{
    return true;
}

bool CheckInvalidProcess()
{
    return false;
}

bool CheckMacScreenMode()
{
    return false;
}

//----------------------------------------------------------------------
// Addon Functions
//----------------------------------------------------------------------
void SetAddonToSlayer(void* pWear, const void* pInfo)
{
    // Stub implementation
}

void SetAddonToSlayer(void* pWear, int addonID)
{
    // Stub implementation
}

void SetAddonToOusters(void* pWear, const void* pInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Blood Bible
//----------------------------------------------------------------------
void SetBloodBibleSlot(void* pInfo)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------
void SetFlagTo(bool bFlag)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Server Group Name
//----------------------------------------------------------------------
void SetServerGroupName(const char* pName)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Mode
//----------------------------------------------------------------------
void SetMode(int mode)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Packet Item Table
//----------------------------------------------------------------------
void InitPacketItemTable()
{
    // Stub implementation
}

void ReleasePacketItemTable()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Release Useless Creature SPK
//----------------------------------------------------------------------
void ReleaseUselessCreatureSPKExcept(void* list)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Save Last Selected Character
//----------------------------------------------------------------------
void SaveLastSelectedCharacter(int index)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Use Item
//----------------------------------------------------------------------
void UseItemOK()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Use Skill Card
//----------------------------------------------------------------------
void UseSkillCardOK(unsigned char cardSlot)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Play Music
//----------------------------------------------------------------------
void PlayMusicCurrentZone()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Play Sound
//----------------------------------------------------------------------
void PlaySound(unsigned short soundID, bool bLoop, int volume, int pan)
{
    // Stub implementation
}

void PlaySound(unsigned short soundID)
{
    // Stub implementation
}

void PlaySoundForce(unsigned short soundID)
{
    // Stub implementation
}

void StopSound(unsigned short soundID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Error Popup
//----------------------------------------------------------------------
void PopupErrorMessage(ErrorID errorID)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Release Game Object
//----------------------------------------------------------------------
void ReleaseGameObject()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Update Disconnected
//----------------------------------------------------------------------
void UpdateDisconnected()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Bug/Crash Report
//----------------------------------------------------------------------
void SendBugReport(const char* pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    // Stub: would normally send to server
    va_end(args);
}

void SendCrashReport()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Netmarble Clipboard
//----------------------------------------------------------------------
bool GetNMClipData(char* pBuffer, unsigned int bufferSize, const char* pURL, bool bUseHTML)
{
    return false;
}

//----------------------------------------------------------------------
// nProtect (GameGuard anti-cheat)
//----------------------------------------------------------------------
// _NPGAMEMON_SUCCESS is defined in nProtectStub.cpp
#ifdef PLATFORM_MACOS
extern int _NPGAMEMON_SUCCESS;
#endif

void SendUserIDToGameMonA(const char* pUserID)
{
    // Stub implementation
}

void Send_nProtect_Auth(unsigned long authCode)
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Screen Shot
//----------------------------------------------------------------------
void MakeScreenShot()
{
    // Stub implementation
}

//----------------------------------------------------------------------
// Mac Address
//----------------------------------------------------------------------
bool GetMacAddressFromNetBIOS(unsigned char* pMac)
{
    // Stub: return false on macOS
    return false;
}

//----------------------------------------------------------------------
// Update Socket
//----------------------------------------------------------------------
void UpdateSocketInput()
{
    // Stub implementation
}

void UpdateSocketOutput()
{
    // Stub implementation
}
