 /*-----------------------------------------------------------------------------

	VS_UI.h

	VS User Interface Master header file.

	2000.8.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_H__
#define __VS_UI_H__

#include "VS_UI_Game.h"
#include "VS_UI_Helper.h"
#include "VS_UI_Item.h"
#include "VS_UI_Title.h"
#include "VS_UI_description.h"
#include "VS_UI_Progress.h"
#include "ProfileManager.h"

#ifndef _LIB
#include "VS_UI_ExtraDialog.h"
#endif

//-----------------------------------------------------------------------------
// C_VS_UI
//
// VS UI 최상위 Object. Client 접근 가능하다.
//-----------------------------------------------------------------------------
class C_VS_UI : public HotKey
{
private:
	//---------------------------------------
	// Components
	//---------------------------------------
	C_VS_UI_TITLE *			m_pC_title;
	C_VS_UI_GAME *				m_pC_game;
	C_VS_UI_PROGRESS *		m_pC_progress;
	C_VS_UI_OPTION *		m_pC_option;
	

#ifndef _LIB
//	C_VS_UI_MONEY_DIALOG *	m_p_edit_dialog;
#endif

	void	EndGame();
	void	EndTitle();
	bool	AlreadyPress(UINT message, long extra) const;

	bool	m_bl_accel_mode;
	int		m_iAccel;
	bool	m_bl_hotkey;

	TYPE_OBJECTID m_otherojectid;
	//Logitech iFeel Mouse 지원 코드

public:
	C_VS_UI();
	~C_VS_UI();
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void	RunUsePetFood(DWORD UsingObjectID = 0, MItem* SubInventory = NULL);
	#else
		void	RunUsePetFood();
	#endif
	
	void	CloseUsePetFood();

	// 2004, 5, 11 sobeit add start - 펫 보관소
	void	RunKeepPetItemDialog();
	void	CloseKeepPetItemDialog();
	void	RunGetKeepPetItemDialog();
	void	CloseGetKeepPetItemDialog();
	// 2004, 5, 11 sobeit add end
	
	PETINFO* GetPetInfo() { if(m_pC_game != NULL)return m_pC_game->GetPetInfo(); return NULL; }
	PETINFO* GetMyPetInfo() { if(m_pC_game != NULL)return m_pC_game->GetMyPetInfo(); return NULL; }
	bool	IsNewMail()					{ if(m_pC_game)return m_pC_game->IsNewMail(); return false; }
	void	RequestDieTimerReset()		{ if(m_pC_game != NULL)m_pC_game->RequestDieTimerReset(); }
	void	CloseOtherInfo();
	void	SetBloodBibleStatusTimer(DWORD sec)	{ if(m_pC_game != NULL)m_pC_game->SetBloodBibleStatusTimer(sec); }

	// 넷마블용
	void	RunConnect()				{ if(m_pC_title != NULL) m_pC_title->RunConnect(); }

	bool	IsRunningTitle()			{ if(m_pC_title != NULL)return true; return false; }
	bool	IsRunningOtherInfo()	{if(m_pC_game != NULL) return m_pC_game->IsRunningOtherInfo(); return false;}
	void	RefreshInfoImage()			{ if(m_pC_game != NULL)m_pC_game->RefreshInfoImage(); }
	bool	IsRunningMainHelp()			{ if(m_pC_game != NULL)m_pC_game->IsRunningMainHelp(); return false; }

	bool	IsInputHotkey()	{ return m_bl_hotkey; }
	void	CloseAllDialog();
	bool	IsInputMode()	{ if(m_pC_game == NULL)return false; return m_pC_game->IsInputMode(); }

	//지뢰설치
	void	StartInstallMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_game)m_pC_game->StartInstallMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsInstallMineProgress()		{ return gbl_mine_progress; }
	void	EndInstallMineProgress()	{ gbl_mine_progress = false; }
	//지뢰만들기
	void	StartCreateMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_game)m_pC_game->StartCreateMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateMineProgress()		{ return gbl_mine_progress; }
	void	EndCreateMineProgress()	{ gbl_mine_progress = false; }
	//폭탄만들기
	void	StartCreateBombProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_game)m_pC_game->StartCreateBombProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateBombProgress()		{ return gbl_mine_progress; }
	void	EndCreateBombProgress()	{ gbl_mine_progress = false; }

	// gamemenu option
	void	RunOption(bool IsTitle = false);
	void	CloseOption();
	bool	IsRunningOption(){ return (m_pC_option != NULL); }	// by larosel

	void	SelectCharacter(int n)	{ if(m_pC_title)m_pC_title->SelectCharacter(n); }	// 캐릭터선택에서 임의선택
//	void	UsedID(bool used)	{ if(m_pC_title)m_pC_title->UsedID(used); }

	const char *GetFileName()	{ if(m_pC_game)return m_pC_game->GetFileName(); return NULL; }
	const char* GetCurrentOtherInfoName(){if(m_pC_game) return m_pC_game->GetCurrentOtherInfoName(); return NULL;}

	void	AddReadyTeamInfo(C_VS_UI_TEAM_LIST::READY_TEAM_LIST &team_info)
	{
		if(m_pC_game)
			m_pC_game->AddReadyTeamList(team_info);
	}
	void	AddRegistTeamInfo(C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST &team_info)
	{
		if(m_pC_game)
			m_pC_game->AddRegistTeamList(team_info);
	}
	void	AddTeamMemberInfo(C_VS_UI_TEAM_MEMBER_LIST::TEAM_MEMBER_LIST &member_info, BYTE bType = 0)
	{
		if(m_pC_game)
			m_pC_game->AddTeamMemberList(member_info, bType);
	}
	char*	GetTeamMember_SelectedID(){
		if(m_pC_game)
			return m_pC_game->GetTeamMember_SelectedID();
		else
			return NULL;
	}

	int		GetPartyManagerFocused()		{ if(m_pC_game)return m_pC_game->GetPartyManagerFocused();  return -1; }

	void	SetWhisperID(char *id)	{ if(m_pC_game)m_pC_game->SetWhisperID(id); }
	void	SetOtherInfo(C_VS_UI_OTHER_INFO::PLAYER_INFO &info){m_pC_game->SetOtherInfo(info); }

	void	SetAccelMode(BYTE accel_num)	{ m_bl_accel_mode = true; m_iAccel = accel_num; }
	bool	IsAccelMode()					{ return m_bl_accel_mode; }
	void	UnSetAccelMode()				{ m_bl_accel_mode = false; }
	int		GetAccelKey()					{ return m_iAccel; }


//server name by larosel
	void	SetServerList(LPSTR *name, int *id, int *status, int size, int default_id)	{ if(m_pC_title)m_pC_title->SetServerList(name, id, status, size, default_id); }
	void	SetCurrentServerName(const char *world_name, const char *server_name, int status) { if(m_pC_progress)m_pC_progress->SetServerName(world_name, server_name, status); } // StartProgress 후에 해준다.

	bool	IsGameMode()const		{ if(m_pC_game)return true; return false; }
	bool	IsRunningStorage() const;
	bool	IsRunningPetStorage() const;
	bool	IsRunningExchange() const;
	bool	IsRunningShop() const;
	bool	IsRunningRepair() const;
	bool	IsRunningSilvering() const;
	bool	IsRunningSell() const;
	bool	IsRunningGameMenu(){ if(m_pC_game) return m_pC_game->IsRunningGameMenu(); return false; }
//	bool	IsRunningOption(){ if(m_pC_game)return m_pC_game->IsRunningOption(); else if(m_pC_title)return m_pC_title->IsRunningOption(); return false; }
//	bool	IsRunningSkillView(){ return m_pC_game->IsRunningSkillView(); }
	bool	IsRunningDescDialog(){ if(m_pC_game) return m_pC_game->IsRunningDescDialog(); return false; }
	bool	IsRunningFileDialog(){ if(m_pC_game) return m_pC_game->IsRunningFileDialog(); return false; }
	bool	IsRunningElevator(){ if(m_pC_game) return m_pC_game->IsRunningElevator(); return false; }
	bool	IsRunningProgress(){ return (m_pC_progress != NULL); }
	bool	ReturnTeamRegistMember(){ if(m_pC_game) return m_pC_game->ReturnTeamRegistMember(); return false; }	
	
	void	RestoreWhenActivateGame();
	void	BackupPrevId(const char * sz_id);
	void	SetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, ACTIONINFO id);
	int	GetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, C_VS_UI_SKILL::HOTKEY_GRADE grade) const;
	void	FinishLevelUp();
	void	LevelUp();
	bool	IsRunningLevelUp();
	bool	IsRunningCharInfo();
	bool	IsRunningSkillInfo();
	bool	IsRunningGrade2Info();
	bool	IsRunningGrade1Info();
	bool	IsRunningGrade3Info();
//	void	RunInfo(bool skill_mode);
	void	RunInfo(C_VS_UI_INFO::INFO_MODE Mode = C_VS_UI_INFO::CHARACTER_INFO_MODE);
	void	RunOtherInfo();
	void	SetPetInfo(struct PETINFO *pPetInfo)		{ if(m_pC_game)m_pC_game->SetPetInfo(pPetInfo); }
	void	SetOtherPetInfo(struct PETINFO *pPetInfo)		{ if(m_pC_game)m_pC_game->SetOtherPetInfo(pPetInfo); }
	void	AcquireChatting();
	bool	IsEmptyChatting();

	// by sigi
	void	SetInputString(const char* pString)					{ m_pC_game->SetInputString(pString); }
	void	AddInputString(const char* pString)					{ m_pC_game->AddInputString(pString); }
	const char* GetInputString() const							{ return m_pC_game->GetInputString(); }
	void	SetInputStringColor(COLORREF rgb)					{ m_pC_game->SetInputStringColor(rgb); }
	
	void	ConnectionFailed();
//	void	CloseUserRegistrationWindow();
//	void	RunNewUserRegistration();
	void	CloseInfo();
	void	CloseGameMenu();
	Point GetInventoryPosition() const;
	Point	GetInventoryGridPosition(int grid_x, int grid_y) const;

	bool	ReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem);
	MItem * RemoveItemInGear(int slot);
	bool	CanReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem);
	const MItem * GetGearItem(int slot) const;
	const MItem * GetGearItem_PickUp(int &slot) const;
	const MItem * GetGearCoreZapItem(int slot) const; // 특정 슬롯에 붙어 있는 코어잽을 리턴, slot : 코어잽 밑의 슬롯
	const MItem * GetGearCoreZapedItem(int slot) const ;// 코어잽 밑에 깔린 아이템을 리턴, slot : 코어잽 슬롯
	const bool IsHasAllCoreZap(int CoreZapType = 0) const; // 모든 종류의 코어잽이 착용 됐는지..검사
	const bool IsCloseBloodBibleSlot(int slot) const;
	const int GetGearSize() const;

	// PDS interface
//	void	DisconnectPCS(int slot);
//	void	AcquireQuitPCSOnlineModeMessage();
//	void	PleaseConnectPCS(char * sz_name, long pcs_number, int slot);
//	void	OnLinePCS(char * sz_name, long pcs_number);
//	void	AcquirePDSClosedMessage();
//	void	GrantPCSWaitRoomToDuplex(int slot);
//	int	GetSendPossibleSlot() const;
	bool	SetSlayerWhisper(bool b)	{ if(m_pC_game)return m_pC_game->SetSlayerWhisper(b); return 0; }	// by larosel
	void	ChangeWhisperFocus()		{ if(m_pC_game)m_pC_game->ChangeWhisperFocus(); }	// by larosel
	bool	GetWhisperFocus()			{ if(m_pC_game)return m_pC_game->GetWhisperFocus(); return 0; }	// by larosel
	void	SetGuildChat(bool guild)	{ if(m_pC_game)m_pC_game->SetGuildChat(guild); }		// by larosel

	int		GetSelectedMainTab()		{ if(m_pC_game) return m_pC_game->GetSelectedMainTab(); return 0;}
	void	SetSelectMainTab(int tab)	{ if(m_pC_game) m_pC_game->SetMainTab(tab); }

	void	Init(CSpriteSurface *surface, void (*fp)(DWORD, int, int, void *));
	void	Release();		// by sigi
	void	Process();
	void	Show();
	void	DrawMousePointer();
	void	DrawTargetArrow(int TargetX, int TargetY);
	void	GetCurrentMousePointerInfo(MOUSEPOINTER_INFO &info);
	bool	MouseControl(UINT message, int x, int y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	DIKeyboardControl(CDirectInput::E_KEYBOARD_EVENT event, DWORD scan_code);

	void	StartTitle();
	void	StartGame();
	void	StartServerSelect(bool bGroup);
	void	StartCharacterManager(bool back = false);
	void	SetCharacter(int slot, S_SLOT &S_slot);
	int		GetZoneID();
	void	SetZone(int zone_id);
	void	SetBlock(int x,int y);
	void	SetTime(const char *str);
	void	SetDate(const char *str);
	void	SetXY(int x, int y);
	void	SetSize(SIZE size);
	int		GetX()							{ return m_pC_game->GetX(); }
	int		GetY()							{ return m_pC_game->GetY(); }
	void	SetSafetyZone(RECT rect, bool my_zone);
	void	SetNPC(int x, int y, int id,const char *name);
	void	SetNPC(MINIMAP_NPC npc);
	void	SetPortal(RECT rect, int id);
	void	ClearNPC();
//	void	SetPortal(int x, int y);

	void	ClearAllCharacter();
	void	NewCharacterCreateOk();
	void	NewCharacterCreateFailed(int error);
	void	DeleteCharacter(int slot);
	void	ChangeToSlayerInterface();
	void	ChangeToVampireInterface();
	void	AddChatToHistory(char * str, char * sz_id, CHAT_LINE_CONDITION condition=CLD_NORMAL, DWORD color = g_color[3][3]);
	void	PickUpItem(MItem * p_item, int item_x=INT_MIN, int item_y=INT_MIN);
	void	DropItem();
	void	SetProgress(WORD cur_val, WORD max_val);
	void	StartProgress();
	void	EndProgress();
//	void	SetProgressStyle(PROGRESS_SPK_INDEX style) { m_pC_progress->SetProgressStyle(style); }

	// lock/unlock
	void	LockItemTrade();
	void	UnlockItemTrade();
	void	LockItem();
	void	UnlockItem();
	void	LockGear();
	void	UnlockGear();

	// skill tree interface
//	void	RunGunSkillTree();
//	void	RunVampireSkillTree();
//	void	RunSwordSkillTree();
//	void	RunBladeSkillTree();
//	void	RunEnchantSkillTree();
//	void	RunHealSkillTree();
//	void	CloseSkillTree();
//	void	RunSkillView();
//	void	CloseSkillView();
//	C_VS_UI_SKILL_VIEW * GetSkillViewWindow();

	//add by viva : ASK_Friend_request-------------dialog-------------
	void	RunFriendRequestAsk(const char* pName);
	void	RunFriendRefuseAsk(const char* pName);
	void	RunFriendWaitAsk(const char* pName);
	//add by viva : ------------------------------event---------------
	void	RunFriendOK();
//	void	SendAddFriend(char* pName);
	bool	IsFriendWaitAskRuning()	{	return m_pC_game->m_pC_friend_wait_dialog != NULL ? true : false; }
	void	setFriendWaitAskNull()	{ m_pC_game->m_pC_friend_wait_dialog = NULL; }
	C_VS_UI_ASK_DIALOG* GetFriendWaitAsk() { return m_pC_game->m_pC_friend_wait_dialog; }
	void	RunFriendExistAsk(const char* pName);
	void	RunFriendBlackAsk(const char* pName);
	void	RunFriendDeleteAsk(const char* pName);

	// sell/buy interface
	void	RunShop();
	void	RunStorageBuy(int price);
	void	RunExchangeAsk(const char* pName);
	void	RunStorage();
	void	RunExchange();
	void	RunExchangeCancel(const char* pName);
	void	CloseShop();
	void	CloseStorage();
	void	ClosePetStorage();
	void	CloseExchange();
	void	CloseExchangeAsk();
	void	CloseExchangeCancel();
	void	SetShop(MShop* pShop);
	void	SetStorage(MStorage* pStorage);
	void	OpenInventoryToSell();
	void	OpenInventoryToRepair();
	void	OpenInventoryToSilvering();
	void	FinishItemSelling();
	void	FinishItemRepairing();
	void	FinishItemSilvering();
	void	RunDescDialog(int type, void* ptr, void *ptr2 = NULL);
	void	CloseDescDialog();
	void	RunFileDialog(C_VS_UI_FILE_DIALOG::MODE Mode, char *type = NULL);
	void	CloseFileDialog();
	void	RunElevator();
	void	CloseElevator();

	// tutorial interface by larosel
	void	RunBookcase();
	void	CloseBookcase();
	bool	IsRunningBookcase() const;
//	void	RunBriefing();
//	void	CloseBriefing();
//	bool	IsRunningBriefing() const;
	void	RunComputer();
	void	CloseComputer();
	bool	IsRunningComputer() const;
//	void	RunTutorialExit();
//	void	CloseTutorialExit();
//	bool	IsRunningTutorialExit() const;
//	void	RunTutorialExitAsk(const int select, const char* pName);
//	void	CloseTutorialExitAsk();
//	void	RunTutorialExitAccept();

	// slayer guild list
	void	RunTeamList(bool ready, bool IsUnion = false);
	void	CloseTeamList();
	bool	IsRunningTeamList(bool IsUnion = false) const;
	void	RunTeamMemberList();
	void	CloseTeamMemberList();
	bool	IsRunningTeamMemberList() const;
	void	RunTeamInfo(bool ready, void *info, bool IsUnion = false);
	void	CloseTeamInfo();
	// 2004, 10, 19, sobeit add start
	void	CloseTeamInfoDialogBox() ;
	// 2004, 10, 19, sobeit add end
	bool	IsRunningTeamInfo() const;
	void	RunTeamMemberInfo(C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO *info);
	void	CloseTeamMemberInfo();
	bool	IsRunningTeamMemberInfo() const;
	void	RunTeamRegist(bool member, int reg_fee, int rank = 0, char *date = NULL, char *team_name = NULL, int guild_id = -1);
	void	CloseTeamRegist();
	bool	IsRunningTeamRegist() const;
	//add by viva
	void CloseFriendChattingInfo(C_VS_UI_FRIEND_CHATTING_INFO* pInfo);
	void OpenFriendChattingInfo(C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList);
	//end

	// slayer portal
	void	RunSlayerPortal();
	void	CloseSlayerPortal();
	bool	IsRunningSlayerPortal() const;

	// gamemenu option  - by sigi
//	void	RunOption();
//	void	CloseOption();

	// title option  - by sigi
//	void	RunTitleOption();
//	void	CloseTitleOption();

	bool	GetInventoryOpenState() const;
	bool	GetGearOpenState() const;

	//--------------------------------------
	// Inside object communication interface
	//--------------------------------------
	void	ToggleESC4UI();
	void	ResetSlayerQuickItemSize();
	void	ResetOustersQuickItemSize();

	// ??
	void	UnselectSkill();
	void	ChangeSkillWheel(bool direct);
	void	SelectSkill(int id);

	void	RunQuickItemSlot();
	void	CloseQuickItemSlot();
	bool	IsRunningQuickItemSlot();
	void	RunMinimap();
	void	CloseMinimap();
	bool	IsRunningMinimap();

	void	FinishRequestDie();
	void	RequestDie(DWORD timer = 30000);
	bool	IsRunningRequestDie() const;

	void	RunNoSearchResult();	
	void	CloseNoSearchResult();	

	void	RunDepositLimit();	
	void	CloseDepositLimit();	

	void	RunWithdrawLimit();	
	void	CloseWithdrawLimit();	

	// party
	void	FinishRequestParty();
	void	RequestParty(const char *name, DWORD timer = 30000);
	bool	IsRunningRequestParty() const;
	bool	IsRunningAskParty()	const;

	void	RunPartyAsk(const char *name, C_VS_UI_REQUEST_PARTY::REQUEST_PARTY type);
	void	ClosePartyAsk();
	void	RunPartyCancel(const char *name);
	void	ClosePartyCancel();

	void	RunEnchant(int value);
	void	CloseEnchant();

	void	RunPartyManager();
	void	ClosePartyManager();
	bool	IsRunningPartyManager() const;

	void	RunTraceWindow();
	void	CloseTraceWindow();
	bool	IsRunningTraceWindow() const;
	bool	IsStartTrace();
	void	SetCannotTrace();

	void	RunXmasCardWindow(const MItem *pItem);
	void	CloseXmasCardWindow();
	bool	IsRunningXmasCardWindow() const;

	void	RunWarListWindow();
	void	CloseWarListWindow();
	bool	IsRunningWarListWindow() const;
	void	AddWarList(C_VS_UI_WAR_LIST::WarInfo &war_info);
	void	ClearWarList();


	void	RunBringFeeWindow(UINT totalfee = 0, UINT bringfee = 0);
	void	CloseBringFeeWindow();
	bool	IsRunningBringFeeWindow();
	void	SetTotalFee(UINT fee);
	void	SetBringFee(UINT fee);
	UINT	GetBringFee();
	UINT	GetTotalFee();
	void	RunBringFeeLimit();

	void	RunBloodBibleStatus();
	void	CloseBloodBibleStatus();
	bool	IsRunningBloodBibleStatus();
	void	SetBloodBibleStatus(int& blood_bible_id, int& zone_id, int& x,int& y, C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS_INFO status,const char* name);
	void	SetBloodBibleStatus(int& blood_bible_id, C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS &blood_bible_status);
	void	UnSetBloodBibleStatus(int blood_bible_id);

	void	RunInputNameWindow(C_VS_UI_INPUT_NAME::INPUT_NAME_MODE_LIST mode = C_VS_UI_INPUT_NAME::INPUT_NAME_HOPE_COUPLE);
	void	CloseInputNameWindow();
	bool	IsRunningInputNameWindow();
	const char*	GetCurrentInputName();
	void	SetInputNameWindow(const char* name);
	void	SetEnableInputName();
	void	SetDisableInputName();

	void	RunCannotPlayMessage();
	void	RunCannotPlayByAttrMessage();

	void	RunPopupMessage(const char *str = NULL, C_VS_UI_POPUP_MESSAGE::POPUP_TYPE type = C_VS_UI_POPUP_MESSAGE::POPUP_NORMAL);
	void	RunPopupMessage( C_VS_UI_POPUP_MESSAGE::PopupWindowStyle& style );
	void	ClosePopupMessage();
	bool	IsRunningPopupMessage();
	
	void	RunQuestStatusWindow();
	void	CloseQuestStatusWindow();
	bool	IsRunningQuestStatusWindow();
	void	SetQuestStatusInit();
	DWORD	GetQuestStatusID();
	void	SetQuestStatus(C_VS_UI_QUEST_STATUS::QUEST_STATUS &status);


	void	RunLotteryCard( int& step);
	void	CloseLotteryCard();
	bool	IsRunningLotteryCard();
	void	SetResultLotteryCard( bool bSuccess ) { if(m_pC_game) m_pC_game->SetResultLotteryCard( bSuccess ); }
	
	void	RunItemListWindow();
	bool	IsRunningItemListWindow();
	void	CloseItemListWindow();
	bool	AddItemToItemList(C_VS_UI_ITEM_LIST::ItemList pItem);
	bool	RemoveItemInItemList( DWORD id );
	
	void	RunImageNotice();
	void	AddNotice(const char *name, DWORD &id );
	void	CloseImageNotice();
	bool	IsRunningImageNotice();

	void	RunBulletinBoardWindow(const MItem *pItem);
	void	CloseBulletinBoardWindow();
	bool	IsRunningBulletinBoardWindow() const;

	void	RunTransItem();
	void	CloseTransItem();

	void	FinishRequestResurrect();
	void	RequestResurrect(bool f1, bool f2, bool eternity = false, bool IsSiegeAttacker = false);
	bool	IsRunningRequestResurrect() const;
	void	SetDelayRequestResurrect( C_VS_UI_REQUEST_RESURRECT::RESURRECT_MODE mode, int delay = 0 );

	void	ChangeToOustersInterface();
	
	void	RunMixingForge(C_VS_UI_MIXING_FORGE::FORGE_CLASS forge_class, C_VS_UI_MIXING_FORGE::FORGE_TYPE forge_type);
	bool	IsRunningMixingForge() const;
	void	CloseMixingForge();
	
	void	RunRemoveOptionFromRareItem(const MItem* pItem = NULL, const MItem* pItem2 = NULL);
	bool	IsRunningRemoveOptionFromRareItem() const;
	void	CloseRemoveOptionFromRareItem();

	void	RunOustersSkillInfo(int skillID, int window_x, int window_y, bool flag);
	bool	IsRunningOustersSkillInfo() const;
	void	CloseOustersSkillInfo();
	
	void	RunHorn(int currentZoneID);
	bool	IsRunningHorn() const;
	void	CloseHorn();

	void	RunMailBox();
	bool	IsRunningMailBox() const;
	void	CloseMailBox();
	// 2004, 10, 19, sobeit add start
	void	CloseMailBoxDialogBox() ;
	// 2004, 10, 19, sobeit add end
	void	AddMail(int tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open, int guildid=0, int guildtype = 0);
	void	AddMailContents(int tab_id, DWORD id, const char * contents);
	void	DeleteMail(int tab_id, DWORD id);

	// Mail-helpmessage
	void	AddHelpMail(DWORD id, bool open = false);

	void	SetOustersSkillDown();
	
	void	RunFindingMine();
	bool	IsRunningFindingMine() const;
	void	CloseFindingMine();
	
	void	RunArrowTile();
	bool	IsRunningArrowTile() const;
	void	CloseArrowTile();

	void	RunCrazyMine();
	bool	IsRunningCrazyMine() const;
	void	CloseCrazyMine();

	void	SetMiniGameScore(BYTE& Type,std::string& topname, WORD& topscore, WORD& myscore);

	void	RunQuestInventory(const MItem *pItem);
	bool	IsRunningQuestInventory() const;
	void	CloseQuestInventory();

	void	RunCTFStatusWindow();
	void	CloseCTFStatusWindow();
	bool	IsRunningCTFStatusWindow();
	void	SetCTFStatus(DWORD &endtime, int &flag_s, int &flag_v, int &flag_o);

	void	RunPetInfo(struct PETINFO *pPetItem);
	void	ClosePetInfo();
	bool	IsRunningPetInfo();

	void	RequestShrineMinimap(DWORD timer);
	void	CloseRequestShrineMinimap();
	bool	IsRunningRequestShrineMinimap();
	
		// 헬프 메세지 desc // 
	void	RunHelpDesc(const char *detail,const char *title,DWORD id);
	void	CloseHelpDesc();
	bool	IsRunningHelpDesc();

	// 2004, 5, 11 sobeit add start- pet storage
	void	RunPetStorage();
	void	SetPetStorage(MStorage* pStorage);
	int		GetPetStorageEmptySlot();
	int 	GetPetStorageFocusSlot();
	// 2004, 5, 11 sobeit add end- pet storage

	// 2004, 5, 31 sobeit add start - sms message
	void	RunSMSMessage();
	bool	IsRunningSMSMessage() const;
	void	CloseSMSMessage() ;
	void	RunSMSList();
	bool	IsRunningSMSList() const;
	void	CloseSMSList() ;
	void	RunSMSRecord();
	bool	IsRunningSMSRecord() const;
	void	CloseSMSRecord() ;
	void	SetSMSAddressList(void* pVoid) ;
	void	AddSMSAddressList(DWORD element, char* name, char* id, char* num) ;
	void	DeleteSMSAddressList(int id) ;
	void	AddSMSSendList(char* str) ;
	void	AcquireSMS();
	void	SetSMSCharage(int i);
	// 2004, 5, 31 sobeit add end - sms message
	// 2004, 6, 14 sobeit add start - naming
	void	RunNaming();
	bool	IsRunningNaming() const;
	void	CloseNaming();
	void	RunAskUseItemDialog(int AskType);
	void	CloseAskUseItemDialog();
	// 2004, 6, 14 sobeit add end
	// 2004, 6, 15 sobeit add start
	bool	IsRunningGearWindow() const;
	void	Change_Custom_Naming(int nID, char* szName);
	// 2004, 6, 15 sobeit add end
	// 2004, 6, 16 sobeit add start
	void	SetNickNameList(void* pData);
	void	AddNickNameList(void* pData);
	// 2004, 6, 16 sobeit add end
	// 2004, 6, 18 sobeit add start
//	bool	GetChangeableNickName();
//	bool	GetChangeablePetNickName() ;
//	void	SetChangeableNickName(bool Is);
//	void	SetChangeablePetNickName(bool Is);
	// 2004, 6, 18 sobeit add end
	// 2004, 6, 21 sobeit add start
	void	RunNamingChange(MItem* pItem, char* szCustom);
	bool	IsRunningNamingChange() const;
	void	CloseNamingChange();
	// 2004, 6, 21 sobeit add end
	// 2004, 6, 30 sobeit add start
	void	RunQuestManager();
	bool	IsRunningQuestManager() const;
	void	CloseQuestManager();
	bool	SetQuestManagerInfo(void* pVoid);
	bool	ModifyQuestManagerInfo(void* pVoid, int nType);
	void	SetQuestManagerSubWindow(DWORD dwID);
	void	SendQuestIntention(bool bIntention);
	void	ReleaseQuestXML();
	// 2004, 6, 30 sobeit add end
	// 2004, 7, 5 sobeit add start
	void	RunQuestList();
	bool	IsRunningQuestList() const;
	void	CloseQuestList();
	
	void	RunQuestDetail();
	bool	IsRunningQuestDetail() const;
	void	CloseQuestDetail();
	
	void	RunQuestMission();
	bool	IsRunningQuestMission() const;
	void	CloseQuestMission();
	
	void	RunQuestItem();
	bool	IsRunningQuestItem() const;
	void	CloseQuestItem();
	void	CloseQuestIcon(int JustClose);
	void	UpdateQuestItemInfo(std::vector<MItem*>& vItem);
	void	PushGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex);
	void	DeleteQuestItem(int nSlot);
	void	RunNextGQuestExcuteElement();
	// 2004, 7, 5 sobeit add end
	
	// 2004, 10, 25, sobeit add start - 세율 변경
	void	RunModifyTax();
	void	RunRangerChat();
	bool	IsRunngingRangerChat();
	void	CloseRangerChat();
	void	SetRangerChatString(char* str);
	// 2004, 10, 25, sobeit add end
	
	// 2004, 11,30, sobeit add start
	void	RunPowerjjang();
	bool	IsRunningPowerjjang();
	void	ClosePowerjjang();
	void	PowerjjangGambleResult(BYTE bItemCode);
	// 2004, 11,30, sobeit add end
	
	// 2005, 1, 3, sobeit add start
	void	OpenInventoryToSwapAdvanceItem();
	void	FinishSwapAdvancementItem();
	bool	IsRunningSwapAdvancementItem();
	// 2005, 1, 3, sobeit add end

	// 2005, 1, 11, sobeit add start - 불우이웃돕기 창
	void	Run_Campaign_Help_Unfortunate_Neighbors(int value = 0);
	// 2005, 1, 11, sobeit add end
	
	// 2005, 1, 17, sobeit add start - 퀘스트 관련
	void	SetQuestNpcDialog(void* pVoid);
	// 2005, 1, 17, sobeit add end - 퀘스트 관련

	// 2005, 1, 24, sobeit add start - 아이템 받기 이벤트
	void	Run_Confirm_GetItemEvent(int value);
	// 2005, 1, 24, sobeit add end

	// 2005, 2, 1, sobeit add start
	void	RunWebBrowser(HWND hWnd, char* szURL, void* pWebOjbect = NULL);
	bool	IsRunningWebBrowser();
	void	CloseWebBrowser();
	int		GetMouseCursorInfo();
	bool	IsInRectPointWebBrowser(int X, int Y);
	// 2005, 2, 1, sobeit add end

	// 2005, 2, 24, sobeit add start
	void	RunSubInventory(MItem* pItem);
	bool	IsRunningSubInventory();
	bool	AddItemToSubInventory(MItem* pItem, int X, int Y);
	MItem*	GetSubInventoryItem();
	bool	ReplaceSubInventoryItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);
	void	CloseSubInventory();
	// 2005, 2, 25, sobeit add end
	//---------------------------------------
	// Hot key
	//---------------------------------------
	// 2004, 5, 17 sobeit add start - summon pet hot key
	void HotKey_SummonPet(); // ctrl + space
	// 2004, 5, 17 sobeit add end
	void HotKey_PetInfo();	// ctrl + CapsLock
	void HotKey_MailBox();	// ctrl + b
	void HotKey_PartyChat();	// ctrl + a
	void HotKey_Quest();		// ctrl + q
	void HotKey_Party();		// ctrl + p
	void HotKey_Mark();			// '한자'키
	void HotKey_Help();			// ctrl + i
	void HotKey_SkillInfo();	// ctrl + k
	void HotKey_Minimap();		// ctrl + m
	void HotKey_CharInfo();		// ctrl + i
	
	void HotKey_Grade1Info();	// ctrl + i
	void HotKey_Grade2Info();	// ctrl + i

//	void HotKey_WindowToggle();
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void HotKey_Inventory(bool IsCheckSubInventory = false);
	#else
		void HotKey_Inventory();
	#endif
		
	void HotKey_Gear();			
	void HotKey_QuickItemSlot();
	void HotKey_Skill();
	void HotKey_Chat();			// ctrl + c
	void HotKey_ZoneChat();		// ctrl + z
	void HotKey_GuildChat();	// ctrl + g
	void HotKey_UnionChat();	// ctrl + d
	void HotKey_Whisper();		// ctrl + w
	void HotKey_ExtendChat();	// ctrl + e
	void HotKey_ESC();			
	void HotKey_F1();				
	void HotKey_F2();				
	void HotKey_F3();				
	void HotKey_F4();				
	void HotKey_F5();				
	void HotKey_F6();				
	void HotKey_F7();				
	void HotKey_F8();				
	void HotKey_F9();				
	void HotKey_F10();			
	void HotKey_F11();			
	void HotKey_F12();
	void ResetHotKey() { if(m_pC_game!=NULL) m_pC_game->ResetHotKey(); }
	void HotKey_Friend();		//add by viva

	//---------------------------------------
	// Message
	//---------------------------------------
	void	ServerDisconnectMessage();
	void	InvalidIdPasswordMessage();
	void	AleadyExistIdMessage();
	void	NoAleadyExistIdMessage();
	void	Invalid_SSN_Message();

	int		inventory_mode,m_mode;
	void	setinventory_mode(int mode) {inventory_mode = mode;}
	void	SetOtherObjectID(TYPE_OBJECTID id) { m_otherojectid = id; }
	TYPE_OBJECTID GetOtherObjectID()		   { return m_otherojectid; }

	// 2004, 11, 18 thai00
	bool	IsRunningPersnalShop() const;
	void    RunPersnalShop();
	void	ClosePersnalShop();
	void	SetPersnalShop(MStorage* pStorage);
	void	SetPersnalShoptoUI(int mode) { m_mode = mode;}
	int		GetPersnalShoptoUI() { return m_mode;}

	bool	IsRunningPersnalShopMessage() const;
	void    RunPersnalShopMessage();
	void	ClosePersnalShopMessage();
	

};

extern C_VS_UI		gC_vs_ui;

#define		MAX_LEVEL				150


#endif