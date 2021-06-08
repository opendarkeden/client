/*-----------------------------------------------------------------------------

	VS_UI_Game.h

	Game interface.

	Game Object Components:
									Slayer 
									Vampire 
									Game menu
									NPC
									Exchange

	2000.8.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GAME_H__
#define __VS_UI_GAME_H__

#include "VS_UI_GameSlayer.h"
#include "VS_UI_GameVampire.h"
#include "VS_UI_GameOusters.h"
#include "VS_UI_Widget.h"

#include "VS_UI_Shop.h"
#include "VS_UI_Storage.h"
#include "VS_UI_Exchange.h"
#include "VS_UI_Tutorial.h" //by larosel
#include "VS_UI_Title.h"
//#include "VS_UI_SKILL_VIEW.h"
#include "VS_UI_ELEVATOR.h"
#include "VS_UI_PetStorage.h"
#include "VS_UI_WebBrowser.h"
/*
//-----------------------------------------------------------------------------
// class C_VS_UI_GAMEMENU_OPTION
//
// GameMenu에서 나오는 option 
//
// by sigi
//
//-----------------------------------------------------------------------------
class C_VS_UI_GAMEMENU_OPTION : public C_VS_UI_DIALOG	// by sigi
{
public :
	enum MENU
	{
//		DRAW_MINIMAP,
//		DRAW_ZONE_NAME,
//		DRAW_GAME_TIME,
//		DRAW_BLENDING_CHARACTER_SHADOW,
		PLAY_SOUND,
		PLAY_MUSIC,
		VOLUME_SOUND,
		VOLUME_MUSIC,
		PLAY_WAV,
		PLAY_MIDI,
		FILTERING_CURSE,
		BLOOD_DROP,
		PARTY_HPBAR_ALPHA,
		IFEEL,
		USE_GAMMA,
		GAMMA_VALUE,
		CHAT_BOX,
		MENU_COUNT,

		INVALID_ID
	};

	enum GAMEMENU_SPK_INDEX
	{
		SPK_NORMAL_BOX,
		SPK_FOCUSED_BOX,
		SPK_CHECKED_BOX,
		SPK_FOCUSED_CHECKED_BOX,
		SPK_MINUS,
		SPK_PLUS,
		SPK_SCROLL_BAR,
		SPK_SCROLL_NORMAL_SELECT,
		SPK_SCROLL_FOCUSED_SELECT
	};

private:
	C_SPRITE_PACK	*			m_pC_option_spk;

	ButtonGroup *				m_pC_button_group;


	C_VS_UI_EVENT_BUTTON*		m_pC_button[MENU_COUNT];	// 그냥 참조용. delete하면 안된다.

	static GAMEMENU_SPK_INDEX			m_sprite_id[MENU_COUNT][4];

	BOOL			m_Checked[MENU_COUNT];
	
	int				m_SoundVolume;
	int				m_MusicVolume;
	int				m_GammaValue;

	Rect			m_rectSoundVolume;
	Rect			m_rectMusicVolume;
	bool			m_bSoundVolumePushed;
	bool			m_bSoundVolumeFocused;
	bool			m_bMusicVolumePushed;
	bool			m_bMusicVolumeFocused;

	Rect			m_rectGammaValue;
	bool			m_bGammaValuePushed;
	bool			m_bGammaValueFocused;
	
private :
	void			SetMusicVolume(int _x);
	void			SetSoundVolume(int _x);
	void			SetGammaValue(int _x);

public:
	C_VS_UI_GAMEMENU_OPTION();
	~C_VS_UI_GAMEMENU_OPTION();

	void	Start();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	Run(id_t id);
	void	Show();
	void	Process();
	void	Finish();
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);

	bool	IsRunningMinimap();	// by larosel

};
*/

class C_VS_UI_ARROW_TILE;
class C_VS_UI_CRAZY_MINE;
class C_VS_UI_QUEST_INVENTORY;
class C_VS_UI_STATUS_CTF;
class C_VS_UI_REGEN_TOWER_MINIMAP;
class C_VS_UI_MAILBOX;
class C_VS_UI_PET_INFO;
class C_VS_UI_HELPDESC;
class C_VS_UI_SMS_MESSAGE;
class C_VS_UI_SMS_LIST;
class C_VS_UI_SMS_RECORD;
class C_VS_UI_NAMING;
class C_VS_UI_NicknameInfo;
class C_VS_UI_NAMING_CHANGE;
class C_VS_UI_QUEST_MANAGER;
class C_VS_UI_RANGER_CHAT;
class C_VS_UI_PERSNALSHOP_MESSAGE;
class C_VS_UI_POWER_JJANG;
class C_VS_UI_QUEST_NPC_DIALOG;
class C_VS_UI_WEBBROWSER;
class C_VS_UI_INVENTORY_SUB;
//-----------------------------------------------------------------------------
// C_VS_UI_GAMEMENU
//
//
//-----------------------------------------------------------------------------
class C_VS_UI_GAMEMENU : public Window, public Exec, public ButtonVisual
{
private:
	enum MENU
	{
//		TO_BOARD,
		OPTION,
		LOGOUT,
		CONTINUE,
		MENU_COUNT,

		INVALID_ID
	};

	enum GAMEMENU_SPK_INDEX
	{
		GAMEMENU_WINDOW,
//		TO_BOARD_IMAGE,
		OPTION_IMAGE,
		LOGOUT_IMAGE,
		CANCEL_IMAGE,
	};
	
	C_SPRITE_PACK	*			m_pC_gamemenu_spk;

	ButtonGroup *				m_pC_button_group;
//	bool						m_bHangul;

public:
	C_VS_UI_GAMEMENU();
	~C_VS_UI_GAMEMENU();

	void	UnacquireMouseFocus() { m_pC_button_group->UnacquireMouseFocus(); }
	void	AcquireFirstSequence() { gpC_base->SendMessage(UI_REMOVE_BACKGROUND_MOUSE_FOCUS); }
	void	AcquireDisappear();
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);
	void	WindowEventReceiver(id_t event);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
};

//-----------------------------------------------------------------------------
// C_VS_UI_GAME
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_GAME : public PI_Processor, public HotKey
{
private:
	HotKey *					m_pC_hotkey;

	// common interface object
	C_VS_UI_CHATTING			*m_pC_chatting;
	C_VS_UI_SKILL				m_C_skill;

	C_VS_UI_SHOP *				m_pC_shop;
	C_VS_UI_STORAGE *			m_pC_storage;
	C_VS_UI_EXCHANGE *			m_pC_exchange;
//	C_VS_UI_SKILL_VIEW *		m_pC_skill_view;

	C_VS_UI_INFO *				m_pC_info;
	C_VS_UI_OTHER_INFO*			m_pC_other_info;
	C_VS_UI_DESC_DIALOG *		m_pC_desc_dialog;
	C_VS_UI_FILE_DIALOG *		m_pC_file_dialog;
	C_VS_UI_ELEVATOR *			m_pC_elevator;

	C_VS_UI_ASK_DIALOG*			m_pC_storage_buy_dialog;		// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_exchange_ask_dialog;		// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_exchange_cancel_dialog;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_tutorial_exit_ask_dialog;	// by larosel
	C_VS_UI_ASK_DIALOG*			m_pC_party_ask_dialog;		// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_party_cancel_dialog;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_enchant_dialog;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_no_search_result_dialog;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_deposit_limit;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_withdraw_limit;	// by sigi
	C_VS_UI_ASK_DIALOG*			m_pC_bring_fee_limit;
	C_VS_UI_ASK_DIALOG*			m_pC_trans_item_dialog;
	C_VS_UI_ASK_DIALOG*			m_pC_use_pet_food_dialog;
	C_VS_UI_ASK_DIALOG*			m_pC_keep_petitem_dialog;
	C_VS_UI_ASK_DIALOG*			m_pC_get_keep_petitem_dialog;
	C_VS_UI_ASK_DIALOG*			m_pC_use_askitem_dialog;
	C_VS_UI_ASK_DIALOG*			m_pC_get_event_item;	

	C_VS_UI_BRING_FEE*			m_pC_bring_fee;						// by sonee

	// vampire/slayer interface
	C_VS_UI_TRIBE *				m_pC_tribe_interface;
	C_VS_UI_GAMEMENU *			m_pC_gamemenu;

	// Tutorial by larosel
	C_VS_UI_BOOKCASE *			m_pC_bookcase;
//	C_VS_UI_BRIEFING *			m_pC_briefing;
	C_VS_UI_COMPUTER *			m_pC_computer;
//	C_VS_UI_TUTORIAL_EXIT *		m_pC_tutorial_exit;

	//TEAM
	C_VS_UI_TEAM_MEMBER_LIST *		m_pC_team_member_list;
	C_VS_UI_TEAM_LIST *				m_pC_team_list;
	C_VS_UI_TEAM_INFO *				m_pC_team_info;
	C_VS_UI_TEAM_MEMBER_INFO *		m_pC_team_member_info;
	C_VS_UI_TEAM_REGIST *			m_pC_team_regist;

	//SLAYER_PORTAL
	C_VS_UI_MAILBOX *				m_pC_mailbox;
	C_VS_UI_HORN *					m_pC_horn;
	C_VS_UI_SLAYER_PORTAL *			m_pC_slayer_portal;
	C_VS_UI_REQUEST_DIE *			m_pC_request_die;

	// party
	C_VS_UI_REQUEST_PARTY *			m_pC_request_party;
	C_VS_UI_PARTY_MANAGER *			m_pC_party_manager;

	C_VS_UI_TRACE *					m_pC_trace;
	C_VS_UI_XMAS_CARD *				m_pC_xmas_card;
	C_VS_UI_BULLETIN_BOARD *		m_pC_bulletin_board;
	C_VS_UI_WAR_LIST	*			m_pC_war_list;
	C_VS_UI_BLOOD_BIBLE_STATUS*		m_pC_blood_bible_status;
	C_VS_UI_INPUT_NAME*				m_pC_input_name;
	C_VS_UI_POPUP_MESSAGE*			m_pC_popup_message;
	C_VS_UI_LOTTERY_CARD*			m_pC_lottery_card;
	C_VS_UI_ITEM_LIST*				m_pC_item_list;
	C_VS_UI_IMAGE_NOTICE*			m_pC_image_notice;
	C_VS_UI_REQUEST_RESURRECT *		m_pC_request_resurrect;
	C_VS_UI_MIXING_FORGE *			m_pC_mixing_forge;
	C_VS_UI_REMOVE_OPTION *			m_pC_remove_option;
	C_VS_UI_OUSTERS_SKILL_INFO *	m_pC_ousters_skill_info;
	C_VS_UI_FINDING_MINE *			m_pC_finding_mine;
	C_VS_UI_ARROW_TILE*				m_pC_arrow_tile;
	C_VS_UI_CRAZY_MINE*				m_pC_crazy_mine;
	C_VS_UI_QUEST_INVENTORY*		m_pC_quest_inventory;
	C_VS_UI_STATUS_CTF*				m_pC_status_ctf;
	C_VS_UI_REGEN_TOWER_MINIMAP*	m_pC_regen_tower_minimap;
	C_VS_UI_PET_INFO*				m_pC_pet_info;
	C_VS_UI_HELPDESC*				m_pC_help_desc;
	
	// 2004, 5, 11 , sobeit add start - pet storage
	C_VS_UI_PETSTORAGE *			m_pC_petstorage;
	// 2004, 5, 11 , sobeit add end

	// 2004, 5, 27 sobeit add start - sms message
	C_VS_UI_SMS_MESSAGE*			m_pC_sms_message;
	C_VS_UI_SMS_LIST*				m_pC_sms_list;
	C_VS_UI_SMS_RECORD*				m_pC_sms_record;
	// 2004, 5, 27 sobeit add end

	// 2004, 6, 14 sobeit add start - naming window
	C_VS_UI_NAMING*					m_pC_naming;
	C_VS_UI_NAMING_CHANGE*			m_pC_naming_change;
	std::vector<C_VS_UI_NicknameInfo*>	m_NickNameList;
//	bool							m_ChangeableNickname;
//	bool							m_ChangeablePetNickname;
	// 2004, 6, 14 sobeit add end

	// 2004, 7, 9 sobeit add start
	C_VS_UI_QUEST_MANAGER		*m_pC_quest_manager;
	// 2004, 7, 9 sobeit add end

	// 2004, 10, 25, sobeit add start
	C_VS_UI_RANGER_CHAT				*m_pC_Ranger_Chat;
	// 2004, 10, 25, sobeit add end

	// 2004, 10, 25, sobeit add start
	C_VS_UI_POWER_JJANG				*m_pC_Powerjjang;
	// 2004, 10, 25, sobeit add end
	void	RunGameMenu();

	// 2004, 11, 18 thai00
	C_VS_UI_PERSNALSHOP				*m_pC_persnalshop;
	C_VS_UI_PERSNALSHOP_MESSAGE     *m_pC_persnalshopmessage;

//	bool	m_bl_set_save;
	C_VS_UI_QUEST_NPC_DIALOG		*m_pC_Quest_Npc_Dialog;

	// 2005, 2, 1, sobeit add start
	C_VS_UI_WEBBROWSER				*m_pC_WebBrowser;
	// 2005, 2, 1, sobeit add end

	C_VS_UI_INVENTORY_SUB			*m_pC_SubInventory;
public:
	C_VS_UI_GAME();
	~C_VS_UI_GAME();

	struct PETINFO* GetPetInfo();
	struct PETINFO* GetMyPetInfo();

		//-------------------------------------------------------m_pC_friend_wait_dialog : add by viva---------------------------
	C_VS_UI_ASK_DIALOG*			m_pC_friend_wait_dialog;	
	
	void	CloseOtherInfo();
	bool	IsNewMail();
	void	RequestDieTimerReset()			{ if(m_pC_request_die != NULL)m_pC_request_die->Timer(true); }
	void	SetBloodBibleStatusTimer(DWORD sec)	{ if(m_pC_blood_bible_status != NULL)m_pC_blood_bible_status->SetTimer(sec); }
	void	RefreshInfoImage()			{ if(m_pC_info != NULL)m_pC_info->RefreshImage(); if(m_pC_party_manager)m_pC_party_manager->RefreshFaceImage(); if(m_pC_other_info != NULL)m_pC_other_info->RefreshImage(); }
	bool	IsRunningMainHelp()			{ if(m_pC_desc_dialog != NULL)m_pC_desc_dialog->IsMainHelp(); return false; }
	bool	ReturnTeamRegistMember()	{ if(m_pC_team_regist!=NULL) return m_pC_team_regist->ReturnTeamRegistMember(); return false; }
	
	void	MouseControlExtra(UINT message, int _x, int _y) { if(m_pC_tribe_interface != NULL) m_pC_tribe_interface->MouseControlExtra(message, _x, _y); }

	bool	IsInputMode()				{ if(m_pC_chatting == NULL) return false; return m_pC_chatting->IsInputMode();	}
	// 윈도우 세팅 세이브 할까?
//	void	SetSaveSet(bool bl_save)	{ m_bl_set_save = bl_save; }

	// 지뢰설치
	void	StartInstallMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_tribe_interface)m_pC_tribe_interface->StartInstallMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsInstallMineProgress()		{ return gbl_mine_progress; }
	void	EndInstallMineProgress()	{ gbl_mine_progress = false; }
	// 지뢰만들기
	void	StartCreateMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_tribe_interface)m_pC_tribe_interface->StartCreateMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateMineProgress()		{ return gbl_mine_progress; }
	void	EndCreateMineProgress()	{ gbl_mine_progress = false; }
	//폭탄만들기
	void	StartCreateBombProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_tribe_interface)m_pC_tribe_interface->StartCreateBombProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateBombProgress()		{ return gbl_mine_progress; }
	void	EndCreateBombProgress()	{ gbl_mine_progress = false; }

	const char *GetFileName()	{ if(m_pC_file_dialog)return m_pC_file_dialog->GetFileName(); return NULL; }
	void	AddReadyTeamList(const C_VS_UI_TEAM_LIST::READY_TEAM_LIST &team_info)
	{
		if(m_pC_team_list)
			m_pC_team_list->AddReadyTeamList(team_info);
	}
	void	AddRegistTeamList(const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST &team_info)
	{
		if(m_pC_team_list)
			m_pC_team_list->AddRegistTeamList(team_info);
	}
	void	AddTeamMemberList(const C_VS_UI_TEAM_MEMBER_LIST::TEAM_MEMBER_LIST &member_list , BYTE bType)
	{
		if(m_pC_team_member_list)
			m_pC_team_member_list->AddMemberList(member_list, bType);
	}
	char*	GetTeamMember_SelectedID(){
		if(m_pC_team_member_list)
			return m_pC_team_member_list->GetSelectedID();
		else
			return NULL;
	}
	int		GetPartyManagerFocused()		{ if(m_pC_party_manager)return m_pC_party_manager->GetPartyManagerFocused(); return -1; }

	void	SetWhisperID(char *id)	{ if(m_pC_tribe_interface)m_pC_tribe_interface->SetWhisperID(id); }
	void	SetOtherInfo(C_VS_UI_OTHER_INFO::PLAYER_INFO &info) { m_pC_other_info->SetOtherInfo(info);}

	bool	ClosePopupWindow();

	bool	IsRunningStorage() const;
	bool	IsRunningPetStorage() const;
	bool	IsRunningExchange() const;
	bool	IsRunningShop() const;
	bool	IsRunningRepair() const;
	bool	IsRunningSilvering() const;
	bool	IsRunningSell() const;
	bool	IsRunningGameMenu(){ return (m_pC_gamemenu != NULL); }	// by larosel
	bool	IsRunningDescDialog(){ return (m_pC_desc_dialog != NULL); }	// by larosel
	bool	IsRunningFileDialog(){ return (m_pC_file_dialog != NULL); }	// by larosel
	bool	IsRunningElevator() const;

	void	RunQuickItemSlot();
	void	CloseQuickItemSlot();
	bool	IsRunningQuickItemSlot();
	void	RunMinimap();
	void	CloseMinimap();
	bool	IsRunningMinimap();

	void	RestoreWhenActivateGame();
	void	SetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, ACTIONINFO id);
	int	GetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, C_VS_UI_SKILL::HOTKEY_GRADE grade) const;
	void	FinishLevelUp();
	void	LevelUp();
	bool	IsRunningLevelUp();
	bool	IsRunningCharInfo();
	bool	IsRunningSkillInfo();
	bool	IsRunningGrade3Info();
	bool	IsRunningGrade2Info();
	bool	IsRunningGrade1Info();
	bool	IsRunningOtherInfo();
	void	CloseInfo();
//	void	RunInfo(bool skill_mode);
	void	RunInfo(C_VS_UI_INFO::INFO_MODE Mode);
	void	RunOtherInfo();
	void	SetPetInfo(struct PETINFO *pPetInfo)		{ if(m_pC_info)m_pC_info->SetPetInfo(pPetInfo); }
	void	SetOtherPetInfo(struct PETINFO *pPetInfo)		{ if(m_pC_other_info)m_pC_other_info->SetPetInfo(pPetInfo); }
	C_VS_UI_INFO * GetInfoWindow() { return m_pC_info; }
	void	CloseDescDialog();
	void	RunDescDialog(int type, void *ptr , void* ptr2 = NULL);
	void	CloseFileDialog();
	void	RunFileDialog(C_VS_UI_FILE_DIALOG::MODE Mode, char *type);
	void	CloseElevator();
	void	RunElevator();

	void	AcquireChatting();
	bool	IsEmptyChatting();
//	void	DisconnectPCS(int slot);
//	void	ConnectionFailed();
//	void	GrantPCSWaitRoomToDuplex(int slot);
//	int	GetSendPossibleSlot() const;
	void	ChangeWhisperFocus(){ m_pC_chatting->ChangeWhisperFocus(); }	// by larosel
	bool	SetSlayerWhisper( bool b ){return m_pC_chatting->SlayerWhisperMode(b);}	// by larosel
	bool	GetWhisperFocus(){ return m_pC_chatting->GetWhisperFocus(); }	// by larosel
	void	SetGuildChat(bool guild) { m_pC_chatting->SetGuildChat(guild); }

	void	CloseGameMenu();
	Point GetInventoryPosition() const;
	Point	GetInventoryGridPosition(int grid_x, int grid_y) const;
	bool	ReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem);
	MItem * RemoveItemInGear(int slot);
	bool	CanReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem);
	const MItem * GetGearItem(int slot) const;
	const MItem * GetGearItem_PickUp(int &slot) const;
	const MItem * GetGearCoreZapItem(int slot) const;
	const MItem * GetGearCoreZapedItem(int slot) const;
	const bool IsHasAllCoreZap(int CoreZapType) const;
	const bool IsCloseBloodBibleSlot(int slot) const;
	const int GetGearSize() const;
	const char* GetCurrentOtherInfoName(){if(m_pC_other_info) return m_pC_other_info->GetCurrentOtherInfoName(); return NULL;}

//	void	AcquireQuitPCSOnlineModeMessage();
//	void	PleaseConnectPCS(char * sz_name, long pcs_number, int slot);
//	void	OnLinePCS(char * sz_name, long pcs_number);
//	void	AcquirePDSClosedMessage();

	void	ToggleESC4UI();
	void	ResetSlayerQuickItemSize();
	void	ResetOustersQuickItemSize();
	
	void	Start();
	//void	Show();
	void	Process();
	//bool	MouseControl(UINT message, int _x, int _y);
	//void	KeyboardControl(UINT message, UINT key, long extra);

	int		GetZoneID();
	void	SetZone(int zone_id);
	void	SetBlock(int x, int y );
	void	SetTime(const char *str);
	void	SetDate(const char *str);
	void	SetXY(int x, int y);
	void	SetSize(SIZE size);
	int		GetX()						{ return m_pC_tribe_interface->GetX(); }
	int		GetY()						{ return m_pC_tribe_interface->GetY(); }
	void	SetSafetyZone(RECT rect, bool my_zone);
	void	SetNPC(MINIMAP_NPC npc);
	void	ClearNPC();
	void	SetPortal(RECT rect, int id);

	void	ChangeToSlayerInterface();
	void	ChangeToVampireInterface();
	void	AddChatToHistory(char * str, char * sz_id, CHAT_LINE_CONDITION condition, DWORD color);

	void	LockItemTrade();
	void	UnlockItemTrade();
	void	LockItem();
	void	UnlockItem();

	void	LockGear();
	void	UnlockGear();
	bool	GetInventoryOpenState() const;
	bool	GetGearOpenState() const;

	// skill tree interface
//	void	RunVampireSkillTree();
//	void	RunSwordSkillTree();
//	void	RunBladeSkillTree();
//	void	RunEnchantSkillTree();
//	void	RunHealSkillTree();
//	void	RunGunSkillTree();
//	void	CloseSkillTree();
//	void	RunSkillView();
//	void	CloseSkillView();
//	C_VS_UI_SKILL_VIEW * GetSkillViewWindow() { return m_pC_skill_view; }

	////add by viva
	void	RunFriendRequestAsk(const char* pName);
	void	CloseFriendRequestAsk(C_VS_UI_ASK_DIALOG* pDialog);
	void	RunFriendRefuseAsk(const char* pName);
	void	RunFriendWaitAsk(const char* pName);
	void	RunFriendOK();
//	void	SendAddFriend();
	void	RunFriendExistAsk(const char* pName);
	void	RunFriendBlackAsk(const char* pName);
	//friend_delete_ask
	void	RunFriendDeleteAsk(const char* pName);


	void	SetShop(MShop* pShop);
	void	SetStorage(MStorage* pStorage);
	void	SetPetStorage(MStorage* pStorage);
	void	RunShop();
	void	RunStorageBuy(int price);	
	void	RunExchangeAsk(const char* pName);	
	void	RunExchangeCancel(const char* pName);	
	void	RunTutorialExitAsk(const int select, const char* pName);	//by larosel
	void	RunStorage();
	void	RunExchange();
	void	RunExchangeCancel();
	void	CloseShop();
	void	CloseStorage();
	void	ClosePetStorage();
	void	CloseExchange();
	void	CloseExchangeAsk();
	void	CloseExchangeCancel();
	void	OpenInventoryToSell();
	void	FinishItemSelling();
	void	FinishItemRepairing();
	void	FinishItemSilvering();
	void	OpenInventoryToRepair();
	void	OpenInventoryToSilvering();

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
//	void	CloseTutorialExitAsk();
//	void	RunTutorialExitAccept();

	// mail
	void	AddMail(int tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open,int guildid, int guildtype);

	void	AddMailContents(int tab_id, DWORD id, const char * contents);
	void	DeleteMail(int tab_id, DWORD id);
	void	RunMailBox();
	void	CloseMailBox();
	// 2004, 10, 19, sobeit add start
	void	CloseMailBoxDialogBox() ;
	// 2004, 10, 19, sobeit add end
	bool	IsRunningMailBox() const;
	
	// mail-helpmessage//
	void	AddHelpMail(DWORD id, bool open);
	bool	IsRunningHelpMailDesc() {return m_pC_help_desc != NULL;}

	// horn
	void	RunHorn(int currentZoneID);
	void	CloseHorn();
	bool	IsRunningHorn() const;

	// finding mine
	void	RunFindingMine();
	void	CloseFindingMine();
	bool	IsRunningFindingMine() const;
	
	// minigame arrow tile
	void	RunArrowTile();
	void	CloseArrowTile();
	bool	IsRunningArrowTile()	const;

	// minigame crazy tile
	void	RunCrazyMine();
	void	CloseCrazyMine();
	bool	IsRunningCrazyMine()	const;
	
	// slayer_portal
	void	RunSlayerPortal();
	void	CloseSlayerPortal();
	bool	IsRunningSlayerPortal() const;

	// TEAMlist
	void	RunTeamList(bool ready, bool IsUnion);
	void	CloseTeamList();
	bool	IsRunningTeamList(bool IsUnion = false) const;
	void	RunTeamMemberList();
	void	CloseTeamMemberList();
	bool	IsRunningTeamMemberList() const;
	void	RunTeamInfo(bool ready, void *info, bool IsUnion);
	void	CloseTeamInfo();
	// 2004, 10, 19, sobeit add start
	void	CloseTeamInfoDialogBox() ;
	// 2004, 10, 19, sobeit add end
	bool	IsRunningTeamInfo() const;
	void	RunTeamMemberInfo(C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO *info);
	void	CloseTeamMemberInfo();
	bool	IsRunningTeamMemberInfo() const;
	void	RunTeamRegist(bool member, int reg_fee, int rank, char *date, char *team_name = NULL, int guild_id = -1);
	void	CloseTeamRegist();
	bool	IsRunningTeamRegist() const;

	//add by viva
	void	CloseFriendChattingInfo(C_VS_UI_FRIEND_CHATTING_INFO* pInfo);
	void	OpenFriendChattingInfo(C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList);
	//end

	void	UnselectSkill();
	void	ChangeSkillWheel(bool direct);
	void	SelectSkill(int id);

	void	RequestDie(DWORD timer);
	void	FinishRequestDie();
	bool	IsRunningRequestDie() const;

	void	RequestShrineMinimap(DWORD timer);
	void	FinishRequestShrineMinimap();
	bool	IsRunningRequestShrineMinimap() const;

	void	RunNoSearchResult();	
	void	CloseNoSearchResult();	

	void	RunDepositLimit();	
	void	CloseDepositLimit();	

	void	RunWithdrawLimit();	
	void	CloseWithdrawLimit();	

	// party
	void	FinishRequestParty();
	void	RequestParty(const char *name, DWORD timer);
	bool	IsRunningRequestParty() const;

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void	RunUsePetFood(DWORD UsingObjectID = 0, MItem* SubInventory = NULL);
	#else
		void	RunUsePetFood();
	#endif

	
	void	CloseUsePetFood();
	//2004, 5, 11 sobeit add start - 펫 보관소
	void	RunKeepPetItemDialog();
	void	CloseKeepPetItemDialog();
	void	RunGetKeepPetItemDialog();
	void	CloseGetKeepPetItemDialog();
	//2004, 5, 11 sobeit add end
	
	bool	IsRunningPartyAsk() const;
	void	RunPartyAsk(const char* pName, C_VS_UI_REQUEST_PARTY::REQUEST_PARTY type);
	void	ClosePartyAsk();
	void	RunPartyCancel(const char* pName);	
	void	ClosePartyCancel();	
	
	void	RunEnchant(int value);
	void	CloseEnchant();

	void	RunPartyManager();
	void	ClosePartyManager();
	bool	IsRunningPartyManager() const;

	void	RunTraceWindow();
	void	CloseTraceWindow();
	bool	IsStartTrace();
	void	SetCannotTrace();
	bool	IsRunningTraceWindow() const;

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
	void	SetTotalFee(UINT totalfee);
	void	SetBringFee(UINT	fee);
	UINT	GetWillBringFee();
	UINT	GetTotalFee();
	void	RunBringFeeLimit();

	void	RunBloodBibleStatusWindow();
	void	CloseBloodBibleStatus();
	bool	IsRunningBloodBibleStatus();
	void	SetBloodBibleStatus(int blood_bible_id, C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS &bbs);
	void	UnSetBloodBibleStatus(int blood_bible_id);

	void	RunInputNameWindow(C_VS_UI_INPUT_NAME::INPUT_NAME_MODE_LIST mode);
	void	CloseInputNameWindow();
	bool	IsRunningInputNameWindow();
	const char *GetCurrentInputName();
	void	SetInputNameWindow(const char* szName);
	void	SetEnableInputName();
	void	SetDisableInputName();

	void	RunPopupMessage(const char *str, C_VS_UI_POPUP_MESSAGE::POPUP_TYPE type);
	void	RunPopupMessage( C_VS_UI_POPUP_MESSAGE::PopupWindowStyle& style );
	void	ClosePopupMessage();
	bool	IsRunningPopupMessage();

	void	RunQuestStatusWindow();
	void	CloseQuestStatusWindow();
	bool	IsRunningQuestStatusWindow();
	void	SetQuestStatusInit();
	DWORD	GetQuestStatusID();
	void	SetQuestStatus(C_VS_UI_QUEST_STATUS::QUEST_STATUS &status);
	

	void	RunLotteryCard( int& step );
	void	CloseLotteryCard();
	bool	IsRunningLotteryCard();
	void	SetResultLotteryCard( bool bSuccess ) { if(m_pC_lottery_card) m_pC_lottery_card->SetResult( bSuccess ); }

	void	RunItemListWindow();
	bool	IsRunningItemListWindow();
	void	CloseItemListWindow();
	bool	AddItemToItemList(C_VS_UI_ITEM_LIST::ItemList pItem);
	bool	RemoveItemInItemList( DWORD ID );

	void	RunImageNotice();
	void	AddNotice(const char *name, DWORD& id);
	void	CloseImageNotice();
	bool	IsRunningImageNotice();

	void	RunBulletinBoardWindow(const MItem *pItem);
	void	CloseBulletinBoardWindow();
	bool	IsRunningBulletinBoardWindow() const;
	
	void	RunTransItem();
	void	CloseTransItem();

	void	RequestResurrect(bool f1, bool f2, bool eternity, bool IsSiegeAttacker);
	void	FinishRequestResurrect();
	bool	IsRunningRequestResurrect() const;
	void	SetDelayRequestResurrect( C_VS_UI_REQUEST_RESURRECT::RESURRECT_MODE &mode, int &delay );

	void	ChangeToOustersInterface();

	
	void	RunMixingForge(C_VS_UI_MIXING_FORGE::FORGE_CLASS forge_class, C_VS_UI_MIXING_FORGE::FORGE_TYPE forge_type);
	void	CloseMixingForge();
	bool	IsRunningMixingForge() const;

	void	RunRemoveOptionFromRareItem(const MItem* pItem, const MItem* pItem2);
	void	CloseRemoveOptionFromRareItem();
	bool	IsRunningRemoveOptionFromRareItem() const;

	void	RunOustersSkillInfo(int skillID, int window_x, int window_y, bool flag);
	void	CloseOustersSkillInfo();
	bool	IsRunningOustersSkillInfo() const;
	
	void	SetOustersDownSkill();
	void	SetMiniGameScore(BYTE& Type, std::string& topname, WORD& topscore, WORD& mybestscore);

	void	RunQuestInventory(const MItem* pItem);
	bool	IsRunningQuestInventory() const;
	void	CloseQuestInventory();

	void	RunCTFStatusWindow();
	void	CloseCTFStatusWindow();
	bool	IsRunningCTFStatusWindow();
	void	SetCTFStatus(DWORD &endtime, int &flag_s, int &flag_v, int &flag_o);
	
	void	RunPetInfo(struct PETINFO *petInfo);
	void	ClosePetInfo();
	bool	IsRunningPetInfo();

	// 헬프 메세지 desc // 
	void	RunHelpDesc(const char *detail,const char *title,DWORD id);
	void	CloseHelpDesc();
	bool	IsRunningHelpDesc();


//	void	RunCannotPlayMessage();
//	void	RunCannotPlayByAttrMessage();

//	void	CloseCannotPlayMessage();

	// 2004, 5, 11 sobeit add start - pet storage
	void	RunPetStorage();
	int		GetPetStorageEmptySlot();
	int		GetPetStorageFocusSlot();
	// 2004, 5, 11 sobeit add end

	// 2004, 5, 27 sobeit add start - sms message
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
	// 2004, 5, 27 sobeit add end

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

//	// 2004, 6, 18 sobeit add start
//	bool	GetChangeableNickName() { return m_ChangeableNickname;}
//	bool	GetChangeablePetNickName() { return m_ChangeablePetNickname;}
//	void	SetChangeableNickName(bool Is) { m_ChangeableNickname = Is;}
//	void	SetChangeablePetNickName(bool Is) { m_ChangeablePetNickname = Is;}
//	// 2004, 6, 18 sobeit add end
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
	void	RunNextGQuestExcuteElement();
	// 2004, 7, 5 sobeit add end
	void	DeleteQuestItem(int nSlot);
	
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
	void OpenInventoryToSwapAdvanceItem();
	void FinishSwapAdvancementItem();
	bool IsRunningSwapAdvancementItem() const;
	// 2005, 1, 3, sobeit add end
	
	// 2005, 1, 11, sobeit add start - 불우이웃돕기 창
	void	Run_Campaign_Help_Unfortunate_Neighbors(int value);
	// 2005, 1, 11, sobeit add end

	// 2005, 1, 17, sobeit add start - 퀘스트 관련
	void	SetQuestNpcDialog(void* pVoid);
	// 2005, 1, 17, sobeit add end - 퀘스트 관련
	
	// 2005, 1, 24, sobeit add start - 아이템 받기 이벤트
	void	Run_Confirm_GetItemEvent(int value);
	// 2005, 1, 24, sobeit add end

	// 2005, 2, 1, sobeit add start
	void	RunWebBrowser(HWND hWnd, char* szURL, void* pWebOjbect);
	bool	IsRunningWebBrowser();
	void	CloseWebBrowser();
	int		GetMouseCursorInfo();
	bool	IsInRectPointWebBrowser(int X, int Y);
	// 2005, 2, 1, sobeit add end

	// 2005, 2, 24, sobeit add start
	void	RunSubInventory(MItem* pItem);
	bool	IsRunningSubInventory();
	MItem*	GetSubInventoryItem();
	bool	AddItemToSubInventory(MItem* pItem, int X, int Y);
	bool	ReplaceSubInventoryItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);
	void	CloseSubInventory();
	// 2005, 2, 25, sobeit add end
	
	//---------------------------------------------------------------
	// 입력중인 문자열을 바로 설정한다.by sigi
	//---------------------------------------------------------------
	void	SetInputString(const char* pString)
	{
		m_pC_chatting->SetInputString( pString );
	}

	//---------------------------------------------------------------
	// 입력중인 문자열에 추가 by sigi
	//---------------------------------------------------------------
	void	AddInputString(const char* pString)
	{
		m_pC_chatting->AddInputString( pString );
	}

	//---------------------------------------------------------------
	// 입력중인 문자열 읽어오기 by sigi
	//---------------------------------------------------------------
	const char* GetInputString() const
	{
		return m_pC_chatting->GetInputString();
	}

	//---------------------------------------------------------------
	// 입력중인 글자의 색깔 바꾸기 by sigi
	//---------------------------------------------------------------
	void	SetInputStringColor(COLORREF rgb)
	{
		m_pC_chatting->SetInputStringColor(rgb);
	}

	//---------------------------------------------------------------
	// -_- 현재 탭 알아내기
	int		GetSelectedMainTab()
	{
		if(m_pC_tribe_interface != NULL)
			return m_pC_tribe_interface->GetSelectedTab();
		
		return 0;
	}

	void	SetMainTab(int tab)
	{
		if(m_pC_tribe_interface != NULL)
			m_pC_tribe_interface->SetSelectTab(tab);
	}
	//---------------------------------------------------------------

	
	//---------------------------------------
	// Hot key
	//---------------------------------------
	void HotKey_Friend();		//add by viva
	void HotKey_PetInfo();		// ctrl + CapsLock
	void HotKey_MailBox();		// ctrl + b
	void HotKey_PartyChat();	// ctrl + a
	void HotKey_Quest();		// ctrl + q
	void HotKey_Party();		// ctrl + p
	void HotKey_Mark();			// '한자'키
	void HotKey_Help();			// ctrl +h
	void HotKey_SkillInfo();	// ctrl +k
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
	// 2004, 5, 17 sobeit add start
	void HotKey_SummonPet();	// ctrl + space
	// 2004, 5, 17 sobeit add end

	void	ResetHotKey();

		// 2004, 11, 18 thai00
	bool	IsRunningPersnalShop() const;
	void    RunPersnalShop();
	void	ClosePersnalShop();

	bool	IsRunningPersnalShopMessage() const;
	void    RunPersnalShopMessage();
	void	ClosePersnalShopMessage();
	void	SetPersnalShop(MStorage* pStorage);
};

#endif