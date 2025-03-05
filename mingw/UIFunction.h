 //-----------------------------------------------------------------------------
// UI Function.h
//-----------------------------------------------------------------------------
// packet부분에 include하면 warning이 좍~~난다.. 
// 어쩔 수 없이... 
//-----------------------------------------------------------------------------

#include "RaceType.h"

#ifndef	__UIFUNCTION_H__
#define	__UIFUNCTION_H__

class PCSlayerInfo;
class PCVampireInfo;
class PCOustersInfo;
class MItem;
class MShop;
class MStorage;
class MCreature;
////////////////////////////////////////////////////////////////////////
//  FirendChatting //by viva
///////////////////////////////////////////////////////////////////////
class GCFriendChatting;

extern void		UI_FriendChatting_UpdateFriendList(GCFriendChatting* pPacket);
extern void		UI_FriendChatting_Message(GCFriendChatting* pPacket);
//-----------------
class C_VS_UI_ASK_DIALOG;
extern void		UI_RunFriendRequest(GCFriendChatting* pPacket);
//extern void		UI_CanncelFriendRequest(C_VS_UI_ASK_DIALOG* pDialog);
//extern void		UI_CloseFriendRequest(C_VS_UI_ASK_DIALOG* pDialog);
extern void		UI_RunFriendRefuse(GCFriendChatting* pPacket);
extern void		UI_RunFriendWait(GCFriendChatting* pPacket);
extern void		UI_RunFriendOK(GCFriendChatting* pPacket);
extern void		UI_RunFriendExist(GCFriendChatting* pPacket);
extern void		UI_RunFriendBlack(GCFriendChatting* pPacket);
extern void		UI_RunFriendDelete(GCFriendChatting* pPakcet);




//-----------------------------------------------------------------------------
// Mode
//-----------------------------------------------------------------------------
extern void		UI_StartCharacterManager();
extern void		UI_DeleteCharacterOK();
extern void		UI_DeleteCharacterFailed();
extern void		UI_NewCharacterCreateOk();
extern void		UI_NewCharacterCreateFailed(int error);
extern void		UI_StartGame();
extern void		UI_CloseUserRegistrationWindow();

extern void		UI_SetNPCInfo( const char* pName, int npcID, int x, int y );
extern void		UI_SetCharacter(int slot, PCSlayerInfo* pInfo);
extern void		UI_SetCharacter(int slot, PCVampireInfo* pInfo);
extern void		UI_SetCharacter(int slot, PCOustersInfo* pInfo);

extern void		UI_CloseAllDialog();

//-----------------------------------------------------------------------------
// Server List
//-----------------------------------------------------------------------------
extern void		UI_SetServerList();
extern void		UI_SetWorldList();
extern void		UI_SetCurrentServerName(const char *name, int status);

//-----------------------------------------------------------------------------
// Interface 변경
//-----------------------------------------------------------------------------
extern Race		UI_GetInterfaceRace();
extern void		UI_ChangeInterfaceRace(Race race);
extern void		UI_SaveUserOption();
extern void		UI_AffectUserOption();

//-----------------------------------------------------------------------------
// status
//-----------------------------------------------------------------------------
extern void		UI_LevelUp();
extern void		UI_SetHP(int current, int max);
extern void		UI_SetMP(int current, int max);
extern void		UI_SetBonusPoint(int bp);
extern void		UI_SetSkillPoint(int sp);
extern void		UI_AddEffectStatus(int es, DWORD delayFrame);
extern void		UI_RemoveEffectStatus(int es);

//-----------------------------------------------------------------------------
// Char Info
//-----------------------------------------------------------------------------
extern void		UI_SetCharInfoName(char* pName);
extern void		UI_SetCharInfoCreatureType(enum Race eRace);
extern void		UI_SetCharInfoSex(bool female);
extern void		UI_SetCharInfoLevel(int value);
extern void		UI_SetCharInfoSTR(int value);
extern void		UI_SetCharInfoDEX(int value);
extern void		UI_SetCharInfoINT(int value);
extern void		UI_SetCharInfoSTR_MAX(int value);
extern void		UI_SetCharInfoDEX_MAX(int value);
extern void		UI_SetCharInfoINT_MAX(int value);
extern void		UI_SetCharInfoSTR_PUR(int value);
extern void		UI_SetCharInfoDEX_PUR(int value);
extern void		UI_SetCharInfoINT_PUR(int value);
extern void		UI_SetCharInfoDAM(int value, int value2=-1);
extern void		UI_SetCharInfoDefense(int value);
extern void		UI_SetCharInfoProtection(int value);
extern void		UI_SetCharInfoTOHIT(int value);
extern void		UI_SetCharInfoSilverDamage(int value);
extern void		UI_SetCharInfoHP(int value);
extern void		UI_SetCharInfoMP(int value);
extern void		UI_SetCharInfoHP_max(int value);
extern void		UI_SetCharInfoMP_max(int value);
//extern void		UI_SetCharInfoCC(int value);
extern void		UI_SetCharInfoFAME(int value);
//extern void		UI_SetCharInfoNOTERITY(int value);

//-----------------------------------------------------------------------------
// 좌표 
//-----------------------------------------------------------------------------
extern POINT	UI_GetInventoryGridPosition(int x, int y);
extern POINT	UI_GetInventoryPosition();

//-----------------------------------------------------------------------------
// Progress
//-----------------------------------------------------------------------------
extern void		UI_StartProgress(int zoneID=ZONEID_NULL);
extern void		UI_DrawProgress(int percent);
extern void		UI_EndProgress();

//-----------------------------------------------------------------------------
// item
//-----------------------------------------------------------------------------
extern void		UI_DropItem();
extern void		UI_PickUpItem( MItem* pItem );
extern void		UI_LockItem();
extern void		UI_UnlockItem();
extern void		UI_LockGear();
extern void		UI_UnlockGear();

//-----------------------------------------------------------------------------
// Chat
//-----------------------------------------------------------------------------
extern void		UI_AddChatToHistory(char* str, char* sz_id, int cond, DWORD color);

//-----------------------------------------------------------------------------
// Tutorial
//-----------------------------------------------------------------------------
extern BOOL		UI_IsRunningTutorialExit();
extern BOOL		UI_IsRunningBookcase();
extern BOOL		UI_IsRunningBriefing();
extern BOOL		UI_IsRunningComputer();

extern void		UI_RunBookcase();
extern void		UI_RunBriefing();
extern void		UI_RunComputer();
extern void		UI_RunTutorialExit();

extern void		UI_CloseBookcase();
extern void		UI_CloseBriefing();
extern void		UI_CloseComputer();
extern void		UI_CloseTutorialExit();


//-----------------------------------------------------------------------------
// Shop, 보관, 교환
//-----------------------------------------------------------------------------
extern BOOL		UI_IsRunningStorage();
extern BOOL		UI_IsRunningExchange();
extern BOOL		UI_IsRunningShop();
extern BOOL		UI_IsRunningRepair();
extern BOOL		UI_IsRunningSilvering();
extern BOOL		UI_IsRunningSell();
extern void		UI_OpenInventoryToSell();
extern void		UI_OpenInventoryToRepair();
extern void		UI_OpenInventoryToSilvering();
extern void		UI_CloseRepair();
extern void		UI_CloseSilvering();
extern void		UI_CloseSell();
extern void		UI_RunShop();
extern void		UI_SetShop(MShop* pShop);
extern void		UI_CloseShop();
extern void		UI_RunStorage();
extern void		UI_RunStorageBuy(int price);
extern void		UI_SetStorage(MStorage* pStorage);
extern void		UI_CloseStorage();
extern void		UI_LockItemTrade();
extern void		UI_UnlockItemTrade();
extern void		UI_RunExchangeAsk(TYPE_OBJECTID otherID);
extern void		UI_RunExchange(TYPE_OBJECTID otherID);
extern void		UI_RunExchangeCancel(const char* pName);
extern void		UI_CloseExchange();
extern void		UI_CloseExchangeAsk();
extern void		UI_CloseExchangeCancel();
extern void		UI_RunPartyRequest(TYPE_OBJECTID otherID);
extern void		UI_RunParty();
extern void		UI_RunPartyAsk(TYPE_OBJECTID otherID);
extern void		UI_RunPartyCancel(const char* pName);
extern void		UI_CloseParty();
extern void		UI_ClosePartyAsk();
extern void		UI_ClosePartyCancel();
extern void		UI_ClosePartyRequest();
extern bool		UI_IsRunningParty();

extern void		UI_RunPetStorage();
extern void		UI_SetPetStorage(MStorage* pStorage);

//-----------------------------------------------------------------------------
// 포탈 관련
//-----------------------------------------------------------------------------
extern bool		UI_IsRunningSelectWayPoint();
extern void		UI_RunSelectWayPoint();
extern void		UI_CloseSelectWayPoint();


//-----------------------------------------------------------------------------
// PCS
//-----------------------------------------------------------------------------
/*
extern void		UI_AcquireQuitPCSOnlineModeMessage();
extern void		UI_PleaseConnectPCS(char * sz_name, long pcs_number, int slot);
extern void		UI_OnLinePCS(char* pName, int pcsNumber);
extern void		UI_AcquirePDSClosedMessage();
extern void		UI_PCSConnectionFailed();
extern void		UI_DisconnectPCS(int slot);
*/

extern void		UI_AlreadyExistIDMessage();
extern void		UI_NoAlreadyExistIDMessage();


//-----------------------------------------------------------------------------
// Skill
//-----------------------------------------------------------------------------
extern void		UI_RunSkillTree(int domain, int maxLevel);
extern BOOL		UI_IsRunningSelectExplosive();
extern void		UI_RunSelectExplosive();
extern void		UI_RunSelectBomb();
extern void		UI_RunSelectMine();
extern void		UI_CloseSelectExplosive();

//-----------------------------------------------------------------------------
// Skill Hot Key
//-----------------------------------------------------------------------------
extern void		UI_SaveHotKeyToServer();
extern void		UI_SetHotKey(int hotkey, int id);
extern int		UI_GetHotKey(int hotkey);

//-----------------------------------------------------------------------------
// ID기억
//-----------------------------------------------------------------------------
extern void		UI_BackupLoginID(char *);

//-----------------------------------------------------------------------------
// mouse pointer
//-----------------------------------------------------------------------------
extern MItem*	UI_GetMouseItem();

//-----------------------------------------------------------------------------
// RemoveDescriptor
//-----------------------------------------------------------------------------
extern void		UI_RemoveDescriptor(void* pPtr=NULL);

//-----------------------------------------------------------------------------
// 완성형 <--> 조합형
//-----------------------------------------------------------------------------
//extern void		UI_WansungToJohap(const char* wansung, char* johap);
//extern void		UI_JohapToWansung(const char* johap, char* wansung);

extern void		UI_ResetQuickItemSlot();

class GCWaitGuildList;
class GCActiveGuildList;
class GCGuildMemberList;
class GCShowWaitGuildInfo;
class GCShowGuildInfo;
class GCShowGuildMemberInfo;
class GCWarScheduleList;
class GCBloodBibleStatus;
class GCSelectQuestID;
class GCQuestStatus;
class GCShowUnionInfo;

//-----------------------------------------------------------------------------
// Guild
//-----------------------------------------------------------------------------
extern void		UI_ShowWaitGuildList( GCWaitGuildList *pPacket);
extern void		UI_ShowUnionGuildMemberInfo(GCShowUnionInfo *pPacket);
extern void		UI_ShowActiveGuildList( GCActiveGuildList *pPacket);
extern void		UI_ShowGuildMemberList( GCGuildMemberList *pPacket);
extern void		UI_ShowGuildRegist(DWORD reg_fee);
extern void		UI_ShowWaitGuildInfo(GCShowWaitGuildInfo *pPacket);
extern void		UI_ShowGuildStartJoin(DWORD join_fee);
extern void		UI_ShowGuildJoin(DWORD join_fee, int rank, const char *date, const char *team_name, int guild_id);
extern void		UI_ShowGuildInfo(GCShowGuildInfo *pPacket);
extern void		UI_ShowGuildMemberInfo(GCShowGuildMemberInfo *pPacket);
extern void		UI_ShowGuildQuit();

extern void		UI_SetWeaponSpeed(int speed, BYTE type);

extern void		UI_CloseEnchant();
extern void		UI_SetGuild(WORD guild_id, BYTE guild_rank, std::string guild_name);

extern void		UI_RunOtherInfo(MCreature *pCreature = NULL);
extern const char *UI_GetOtherInfoName();

// 넷마블용
extern void		UI_RunConnect();

extern void		UI_SetCharInfoSilverDamage(int min_damage, int max_damage);

// 2003.1.21

extern void		UI_OpenBringFee(UINT totalfee);
extern void		UI_SetTotalFee(UINT fee);
extern void		UI_ClearNPC();

// 2003.1.26

extern void		UI_RunWarList(GCWarScheduleList* pPacket);

// 2003.2.11
extern void		UI_CloseBloodBibleStatus();
extern void		UI_RunBloodBibleStatus();
extern void		UI_SetBloodBibleStatus(GCBloodBibleStatus *pPacket);
extern void		UI_SetBloodBibleStatusTimer(DWORD sec);
extern void		UI_RunInputNameWindow(int ID);
extern void		UI_CloseInputNameWindow();

// 2003.3.10

extern void		UI_RunAskGoBilingPage(int n);

// 2003.4.10
extern void		UI_RunQuestList(GCSelectQuestID * pPacket);
extern void		UI_RunWelcomeMessage();
extern void		UI_RunQuestStatus(GCQuestStatus * pPacket );
extern void		UI_InitQuestStatus();
extern void		UI_CloseQuestStatus();

extern void		UI_RunPopupMessage(int type);
extern void		UI_LotteryResult(bool bSuccess);
extern void		UI_RunLottery(int step);
extern void		UI_RunImageNotice(const char* name, DWORD id );
extern void		UI_REQUEST_DIE_TIMER_RESET();

class GCGoodsList;
class GCMiniGameScores;

extern void		UI_RunItemShop(GCGoodsList *pPacket);
extern void		UI_RemoveItemFromItemShop( DWORD id );

extern void		UI_CloseTransItem();
extern void		UI_OkMixingForge(DWORD parameter, MItem* pItem, MItem* pItem2);
extern void		UI_CloseMixingForge();
extern void		UI_OkRemoveOption(DWORD parameter, MItem* pItem);

extern void		UI_SetElementalValue(BYTE type, int value);
extern void		UI_RunHorn(int currentZoneID);
extern void		UI_MasterLairMessage(BYTE type,short value, TYPE_ZONEID ZoneID);
extern void		UI_PopupMessage(int gamestringID);
extern void		UI_DownSkill();
extern void		UI_RunMinigame(int game);
extern void		UI_MiniGameScores(GCMiniGameScores* pPacket);
extern void		UI_SetCTFStatus( DWORD endTime, int flag_s, int flag_v, int flag_o );
extern void		UI_CloseCTFStatusWindow();
extern void		UI_SetDelayEternity();
extern void		UI_CloseRequestResurrectWindow();
extern void		UI_SetGuildName(WORD guildID, std::string& guildName);
extern void		UI_RefreshInfoImage();
extern void		UI_RunNotice(DWORD sendID, DWORD parameter);
extern void		UI_DeleteNotice(DWORD sendID);

extern void		UI_SetSMSList(void* pVoid);
extern void		UI_AddSMSList(DWORD element, char* name, char* id, char* num);
extern void		UI_DeleteSMSList(DWORD id);
extern void		UI_SMS_Set_Charge(int i);
extern void		UI_SMS_ERROR(DWORD param);
extern void		UI_RunModifyTax();
extern void		UI_RunRangerChat();
extern void		UI_CloseRangerChat();
extern void		UI_SetRangerChatString(char* str);
extern bool		UI_IsMonsterKillQuest_Monster(DWORD nID);
extern void		UI_ShowTargetArrow(int x, int y);
extern void		UI_Show_Swap_Advancement_Item();
extern bool		UI_IsRunningSwapAdvancementItem();

extern void		UI_Run_Campaign_Help_Unfortunate_Neighbors(int value);
extern void		UI_Run_Confirm_GetEventItem(int value);
extern void		UI_Run_WebBrowser(char* szURL);
extern bool		UI_IsRunning_WebBrowser();
extern int		UI_GetMouseCursorInfo_WebBrowser();
extern void		UI_Close_WebBrowser();
extern void		UI_ShowWindowCursor();
extern void		UI_HiddenWindowCursor();
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		extern void		UI_RunSubInventory(MItem* pItem);
		extern void		UI_CloseSubInventory();
		extern bool		UI_AddItemToSubInventory(MItem *pItem, int X, int Y);
	#endif



#endif
