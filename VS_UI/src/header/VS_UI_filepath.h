#ifndef __VS_UI_FILEPATH_H__
#define __VS_UI_FILEPATH_H__

// Client�� Release�Ǹ� Directory�� ���� ���� ���� ������ directory service ��� ����.

//////////////////////////////////////////////////////////////////////
// Cross-platform path separator definitions
//////////////////////////////////////////////////////////////////////
#ifdef PLATFORM_WINDOWS
	// Windows uses backslashes
	#define _ROOT							"data\\ui\\"
	#define TXT_ROOT						_ROOT"txt\\"
	#define IFR_ROOT						_ROOT"txt\\"
	#define SPK_ROOT						_ROOT"spk\\"
	#define ISPK_ROOT						_ROOT"ispk\\"
	#define CFPK_ROOT						_ROOT"cfpk\\"
	#define _DATA_DIR_ROOT					"data\\"
	#define _DATA_DIR_IMAGE_ROOT			_DATA_DIR_ROOT"Image\\"
	#define _DATA_DIR_INFO_ROOT				"data\\info\\"
	#define _PROFILE_ROOT					"UserSet\\"
#else
	// Unix/macOS uses forward slashes
	#define _ROOT							"data/ui/"
	#define TXT_ROOT						_ROOT"txt/"
	#define IFR_ROOT						_ROOT"txt/"
	#define SPK_ROOT						_ROOT"spk/"
	#define ISPK_ROOT						_ROOT"ispk/"
	#define CFPK_ROOT						_ROOT"cfpk/"
	#define _DATA_DIR_ROOT					"data/"
	#define _DATA_DIR_IMAGE_ROOT			_DATA_DIR_ROOT"Image/"
	#define _DATA_DIR_INFO_ROOT				"data/info/"
	#define _PROFILE_ROOT					"UserSet/"
#endif

//////////////////////////////////////////////////////////////////////
// Data file paths (cross-platform)
//////////////////////////////////////////////////////////////////////
#ifdef PLATFORM_WINDOWS
	#define _DATA_ROOT						"Data\\Info\\"
	#define _IMAGE_ROOT						"Data\\Image\\"
#else
	#define _DATA_ROOT						"Data/Info/"
	#define _IMAGE_ROOT						"Data/Image/"
#endif
//#define TXT_TUTORIAL_BOOKLIST			TXT_ROOT"Booklist.txt"
#define TXT_TUTORIAL_BRIEFING			"Briefing.txt"
#define TXT_TUTORIAL_COMPUTER			"Computer.xml"
#define INF_SLAYER_PORTAL				"SlayerPortal.inf"
#define TXT_TUTORIAL_WELCOME_SLAYER		"WelcomeSlayer.txt"
#define TXT_TUTORIAL_WELCOME_VAMPIRE	"WelcomeVampire.txt"
#define TXT_TUTORIAL_WELCOME_OUSTERS	"WelcomeOusters.txt"

//add by zdj
#define TXT_TUTORIAL_COMEBACKEVENT_NOTICE	 "WelcomeVampire.txt"
#define TXT_TUTORIAL_COMEBACKEVENT_PAY		 "WelcomeVampire.txt"
#define TXT_TUTORIAL_COMEBACKEVENT_RECOMMEND "WelcomeVampire.txt"


//----------------------------------------------------------------------
// rpk file
//----------------------------------------------------------------------
#define RPK_ITEM						TXT_ROOT"Item.rpk"
#define RPK_HELP						TXT_ROOT"Help.rpk"
#define RPK_SKILL						TXT_ROOT"Skill.rpk"
#define RPK_BOOK						TXT_ROOT"Book.rpk"
#define RPK_TUTORIAL_ETC				TXT_ROOT"TutorialEtc.rpk"
#define RPK_PROGRESS					TXT_ROOT"progress.rpk"
#define RPK_TITLE						TXT_ROOT"title.rpk"
#define RPK_PASSWORD					"darkeden"

//----------------------------------------------------------------------
// server info file // test
//----------------------------------------------------------------------
#ifndef _LIB
#define	FILE_INFO_STR_EXP						_DATA_ROOT"STRExp.inf"
#define	FILE_INFO_DEX_EXP						_DATA_ROOT"DEXExp.inf"
#define	FILE_INFO_INT_EXP						_DATA_ROOT"INTExp.inf"
#define	FILE_INFO_VAMPIRE_EXP					_DATA_ROOT"VampireExp.inf"
#define	FILE_INFO_OUSTERS_EXP					_DATA_ROOT"OustersExp.inf"
#define	FILE_INFO_SKILL_DOMAIN_EXP				_DATA_ROOT"DomainExp.inf"
#define	FILE_INFO_SKILL_INFO					_DATA_ROOT"SkillInfo.inf"
#define FILE_INFO_ITEM_INFO						_DATA_ROOT"item.inf"
#define FILE_INFO_CREATURE_INFO					_DATA_ROOT"Creature.inf"
#define FILE_INFO_NPC_INFO						_DATA_ROOT"NPC.inf"
#define FILE_INFO_SOUND_INFO					_DATA_ROOT"Sound.inf"
#define FILE_INFO_GUILD_MAPPER					_DATA_ROOT"GuildMapper.inf"
#define	FILE_INFO_RANK_BONUS_TABLE				_DATA_ROOT"RankBonus.inf"
#define FILE_INFO_SLAYER_RANK_EXP				_DATA_ROOT"SlayerRankExp.inf"
#define FILE_INFO_VAMPIRE_RANK_EXP				_DATA_ROOT"VampireRankExp.inf"
#define FILE_INFO_OUSTERS_RANK_EXP				_DATA_ROOT"OustersRankExp.inf"
#define FILE_INFO_ADVANCEMENT_EXP				_DATA_ROOT"AdvancementClassExp.inf"
#define FILE_INFO_ZONE_TABLE					_DATA_ROOT"Zone.inf"
#define FILE_INFO_FILEDEF						_DATA_ROOT"FileDef.inf"

#define SPK_GAME_BACK							"data/gameback.spk"
#define SPK_GUILD_MARK							_IMAGE_ROOT"GuildMark.spk"

#define	SPK_GRADE_SLAYER						_IMAGE_ROOT"GradeMarkSlayer.spk"
#define	SPK_GRADE_VAMPIRE						_IMAGE_ROOT"GradeMarkVampire.spk"
#define	SPK_GRADE_OUSTERS						_IMAGE_ROOT"GradeMarkOusters.spk"
#define	SPK_LEVEL_MARK							_IMAGE_ROOT"LevelMarkPet.spk"
#endif

#define FILE_INFO_HORN							_DATA_ROOT"Horn.inf"
#define FILE_INFO_TESTSERVER					_DATA_ROOT"TestServer.inf"
// skill tree back image
//#define SPK_SKILLTREE_BACK_ENCHANT	SPK_ROOT"skilltree_back_enchant.spk"
//#define SPK_SKILLTREE_BACK_SWORD		SPK_ROOT"skilltree_back_sword.spk"
//#define SPK_SKILLTREE_BACK_BLADE		SPK_ROOT"skilltree_back_blade.spk"
//#define SPK_SKILLTREE_BACK_HEAL		SPK_ROOT"skilltree_back_heal.spk"
//#define SPK_SKILLTREE_BACK_GUN		SPK_ROOT"skilltree_back_gun.spk"
//#define SPK_SKILLTREE_BACK_VAMPIRE	SPK_ROOT"skilltree_back_vampire.spk"

// skill tree hightlight icon image
//#define SPK_SKILLTREE_ICO_ENCHANT	SPK_ROOT"skilltree_ico_enchant.spk"
//#define SPK_SKILLTREE_ICO_SWORD		SPK_ROOT"skilltree_ico_sword.spk"
//#define SPK_SKILLTREE_ICO_BLADE		SPK_ROOT"skilltree_ico_blade.spk"
//#define SPK_SKILLTREE_ICO_HEAL		SPK_ROOT"skilltree_ico_heal.spk"
//#define SPK_SKILLTREE_ICO_GUN			SPK_ROOT"skilltree_ico_gun.spk"
//#define SPK_SKILLTREE_ICO_VAMPIRE	SPK_ROOT"skilltree_ico_vampire.spk"

//#define SPK_SKILLTREE_ETC_SLAYER		SPK_ROOT"skilltree_etc_slayer.spk"
//#define SPK_SKILLTREE_ETC_VAMPIRE	SPK_ROOT"skilltree_etc_vampire.spk"

// IFR_ROOT is now defined with platform-specific path separator at the top of this file
#define	IFR_UI							IFR_ROOT"ui.ifr"
#define	IFR_ACTION						IFR_ROOT"action.ifr"
#define	IFR_SKILL						IFR_ROOT"skill.ifr"
#define	IFR_INVENTORY					IFR_ROOT"inventory.ifr"
#define	IFR_USE_ITEM					IFR_ROOT"useitem.ifr"

#define SPK_MINIMAP						SPK_ROOT"Minimap.spk"
#define SPKI_MINIMAP					SPK_ROOT"Minimap.spki"	// by sigi 2001.9.5

// tutorial image by larosel
#define SPK_TUTORIAL_BOOKCASE			SPK_ROOT"TutorialBookcase.spk"
#define SPK_TUTORIAL_BOOK				SPK_ROOT"TutorialBook.spk"
//#define SPK_TUTORIAL_BRIEFING			SPK_ROOT"Tutorial_Briefing.spk"
#define SPK_TUTORIAL_COMPUTER			SPK_ROOT"TutorialComputer.spk"
//#define SPK_TUTORIAL_EXIT				SPK_ROOT"Tutorial_Exit.spk"
#define SPK_ITEM						SPK_ROOT"ItemDescription.spk"
#define SPKI_ITEM						SPK_ROOT"ItemDescription.spki"

#define SPK_ELEVATOR					SPK_ROOT"Elevator.spk"
//#define SPK_TIME						SPK_ROOT"Time.spk"
//#define SPKI_TIME						SPK_ROOT"Time.spki"

// skill view
//#define SPK_SKILLVIEW_SLAYER			SPK_ROOT"SlayerSkillView.spk"
//#define SPK_SKILLVIEW_SLAYER_GRADE		SPK_ROOT"SlayerSkillViewGrade.spk"
//#define SPK_SKILLVIEW_SLAYER_DOMAIN		SPK_ROOT"SlayerSkillViewDomain.spk"
//#define SPK_SKILLVIEW_VAMPIRE			SPK_ROOT"VampireSkillView.spk"
//#define SPK_SKILLVIEW_VAMPIRE_GRADE		SPK_ROOT"VampireSkillViewGrade.spk"
//#define SPK_SKILLVIEW_VAMPIRE_DOMAIN	SPK_ROOT"VampireSkillViewDomain.spk"
#define SPK_SKILL_ICON					SPK_ROOT"SkillIcon.spk"
#define SPKI_SKILL_ICON					SPK_ROOT"SkillIcon.spki"
//#define SPK_SKILLVIEW_ETC				SPK_ROOT"SkillViewEtc.spk"

#define SPK_MINE_PROGRESS				SPK_ROOT"MineProgress.spk"

////////////
// Common
///////////
//#define SPK_COMMON_BUTTON				SPK_ROOT"common_button.spk"
//#define SPK_COMMON_IN_TITLE			SPK_ROOT"common_in_title.spk"

#define	SPK_BOX								SPK_ROOT"AssembleMessageBox.spk"
#define	SPK_SCROLL_BAR						SPK_ROOT"ScrollBar.spk"
#define SPK_ASSEMBLE_BOX_SLAYER				SPK_ROOT"AssembleMessageBoxSlayer.spk"
#define SPK_ASSEMBLE_BOX_BUTTON_SLAYER		SPK_ROOT"AssembleMessageBoxButtonSlayer.spk"
//#define SPK_ASSEMBLE_BOX_ETC				SPK_ROOT"AssembleMessageBoxEtc.spk"
#define SPK_ASSEMBLE_BOX_VAMPIRE			SPK_ROOT"AssembleMessageBoxVampire.spk"
#define SPK_ASSEMBLE_BOX_BUTTON_VAMPIRE		SPK_ROOT"AssembleMessageBoxButtonVampire.spk"
//#define SPK_ASSEMBLE_BOX_ETC_VAMPIRE		SPK_ROOT"AssembleMessageBoxEtcVampire.spk"
//#define SPK_ASSEMBLE_BOX_COMMON				SPK_ROOT"AssembleMessageBoxCommon.spk"
//#define SPK_ASSEMBLE_BOX_BUTTON_COMMON		SPK_ROOT"AssembleMessageBoxButtonCommon.spk"

#define SPK_EXCHANGE_SLAYER						SPK_ROOT"ExchangeSlayer.spk"
//#define SPK_EXCHANGE_VAMPIRE			SPK_ROOT"exchangeVampire.spk"
#define SPK_SLAYER_SHOP_STORAGE			SPK_ROOT"Shop&StorageSlayer.spk"
#define SPK_VAMPIRE_SHOP_STORAGE			SPK_ROOT"Shop&StorageVampire.spk"

// title
#define SPK_ANI_TITLE					SPK_ROOT"TitleAni.spk"
#define SPK_ANI_SYMBOL					SPK_ROOT"Symbol.spk"
#define SPK_TITLE_MENU_DEFAULT		SPK_ROOT"TitleMenuDefault.spk"
#define SPK_TITLE_MENU_SELECT			SPK_ROOT"TitleMenuSelect.spk"

// login
//#define SPK_LOGIN_MENU_DEFAULT		SPK_ROOT"login_menu_default.spk"
#define SPK_LOGIN_MENU			SPK_ROOT"LoginMenu.spk"

// characters
#define SPK_COMMON						SPK_ROOT"Common.spk"
#define SPK_SERVER_SELECT					SPK_ROOT"ServerSelect.spk"
#define	SPK_CHAR_MANAGER				SPK_ROOT"CharManager.spk"
#define SPK_CHAR_CREATE					SPK_ROOT"CharCreate.spk"

//#define SPK_CHARACTERS_MENU_DEFAULT	SPK_ROOT"characters_menu_default.spk"
//#define SPK_CHARACTERS_MENU_SELECT	SPK_ROOT"characters_menu_select.spk"

// New character
//#define SPK_NEWCHARACTER_BACK			SPK_ROOT"NewCharacter_back.spk"
//#define SPK_NEWCHARACTER_ETC			SPK_ROOT"NewCharacter_etc.spk"
//#define SPK_NEWCHARACTER_MENU_DEFAULT	SPK_ROOT"NewCharacter_menu_default.spk"
//#define SPK_NEWCHARACTER_MENU_SELECT	SPK_ROOT"NewCharacter_menu_select.spk"

// System button
#define SPK_SLAYER_SYS_BUTTON			SPK_ROOT"SysButtonSlayer.spk"
#define SPK_VAMPIRE_SYS_BUTTON		SPK_ROOT"SysButtonVampire.spk"

// Slayer skill etc
#define SPK_SLAYER_SKILL_ETC			SPK_ROOT"SkillEtcSlayer.spk"
#define SPK_VAMPIRE_SKILL_ETC			SPK_ROOT"SkillEtcVampire.spk"

//#define IMAGE_BOXBACK					_ROOT"back.tga"
#define SPK_MOUSE_POINTER				SPK_ROOT"MousePointer.spk"
#define SPK_TITLE							SPK_ROOT"Title.spk"
//#define SPK_BBS							SPK_ROOT"BBS.spk"
#define SPK_PROGRESS					SPK_ROOT"Progress.spk"
//#define SPK_PROGRESS_EVENT				SPK_ROOT"progressevent.spk"
#define SPK_PROGRESS_CHARACTER			SPK_ROOT"ProgressChar.spk"
#define SPKI_PROGRESS_CHARACTER			SPK_ROOT"ProgressChar.spki"
#define SPK_PROGRESS_EVENT				SPK_ROOT"ProgressEvent.spk"
//add by coffee 2006-12-21  
#define SPK_PROGRESS_EVENT_1024				SPK_ROOT"ProgressEvent_1024.spk"
// end 2006-12-21
//#define SPK_PROGRESS_VAMPIRE_RAND	SPK_ROOT"progress_vampire_rand.spk"
#define	SPK_INFO_SLAYER					SPK_ROOT"InfoSlayer.spk"
#define	SPK_INFO_VAMPIRE				SPK_ROOT"InfoVampire.spk"
#define	SPK_INFO_OUSTERS				SPK_ROOT"InfoOusters.spk"
#define	SPK_PET_INFO_WOLFDOG				SPK_ROOT"PetWolfDog.spk"
#define	SPK_PET_INFO_CENTAURO				SPK_ROOT"PetCentauro.spk"
#define	SPK_PET_INFO_STIRGE					SPK_ROOT"PetStirge.spk"
#define	SPK_PET_INFO_PIXIE					SPK_ROOT"PetPixie.spk"
#define SPK_PET_FACE						SPK_ROOT"PetFace.spk"
#define	SPK_PET_BUTTON_SLAYER				SPK_ROOT"PetButtonSlayer.spk"
#define	SPK_PET_BUTTON_VAMPIRE				SPK_ROOT"PetButtonVampire.spk"
#define	SPK_PET_BUTTON_OUSTERS				SPK_ROOT"PetButtonOusters.spk"
//add by viva 
#define SPK_PET_INFO_LUPENG				SPK_ROOT"Petlupeng.spk"
#define SPK_PET_INFO_MILITARYBEAR		SPK_ROOT"Petmilitarybear.spk"
#define SPK_PET_INFO_GARGOYLE			SPK_ROOT"Petgargoyle.spk"
//end

//#define SPK_CHAR_MAIN					SPK_ROOT"char_main.spk"
//#define SPK_SLAYER_CHARINFO				SPK_ROOT"SlayerCharInfo.spk"
//#define SPK_VAMPIRE_CHARINFO			SPK_ROOT"VampireCharInfo.spk"
//#define SPK_CHAR_DELETE					SPK_ROOT"char_delete.spk"
//#define SPK_CHAR_DELETE_VAMPIRE			SPK_ROOT"char_delete_vampire.spk"
//#define SPK_COMMON						SPK_ROOT"Common.spk"

#define SPK_SLAYER_GAMEMENU				SPK_ROOT"GameMenuSlayer.spk"
#define SPK_VAMPIRE_GAMEMENU			SPK_ROOT"GameMenuVampire.spk"

#define SPK_OPTION_SLAYER				SPK_ROOT"OptionSlayer.spk"
#define SPK_OPTION_VAMPIRE				SPK_ROOT"OptionVampire.spk"
#define SPK_OPTION						SPK_ROOT"Option.spk"

#define SPK_SLAYER_MAIN					SPK_ROOT"MainSlayer.spk"
#define SPK_VAMPIRE_MAIN				SPK_ROOT"MainVampire.spk"
//#define SPK_BUTTON1						SPK_ROOT"ani_button1.spk"
#define SPK_LOGIN							SPK_ROOT"Login.spk"
#define SPK_SLAYER_GEAR					SPK_ROOT"GearSlayer.spk"
#define SPK_SLAYER_GEAR_SLOT			SPK_ROOT"GearSlotSlayer.spk"
#define SPK_VAMPIRE_GEAR_SLOT			SPK_ROOT"GearSlotVampire.spk"
#define SPK_VAMPIRE_GEAR				SPK_ROOT"GearVampire.spk"
#define SPK_SLAYER_INVENTORY			SPK_ROOT"InventorySlayer.spk"
#define SPK_VAMPIRE_INVENTORY			SPK_ROOT"InventoryVampire.spk"
//#define SPK_SLAYER_ETC					SPK_ROOT"slayer_etc.spk"
//#define SPK_VAMPIRE_ETC					SPK_ROOT"vampire_etc.spk"
#define SPK_SLAYER_QUICKITEM			SPK_ROOT"QuickitemSlayer.spk"
#define SPK_OUSTERS_QUICKITEM			SPK_ROOT"QuickitemOusters.spk"
//#define SPK_SLAYER_DESC					SPK_ROOT"SlayerDesc.spk"
//#define SPK_VAMPIRE_DESC				SPK_ROOT"VampireDesc.spk"
//#define SPK_AXS							SPK_ROOT"axs.spk"
//#define SPK_INVENTORY_BACK_COLOR		SPK_ROOT"InventoryBackColor.spk"
//#define SPK_INVENTORY_BACK_COLOR_VAMPIRE		SPK_ROOT"VampireInventoryBackColor.spk"
//#define SPK_BOX_LINE						SPK_ROOT"box_line.spk"
//#define SPK_OK								SPK_ROOT"ok.spk"

// PDS
//#define SPK_PDS_PCS						SPK_ROOT"pds_pcs.spk"
//#define SPK_PDS_BUTTON					SPK_ROOT"pds_button.spk"
//#define SPK_PDS_SUB						SPK_ROOT"pds_sub.spk"

#define SPK_PARTY_CURSOR					SPK_ROOT"PartyCursor.spk"

// CHAT
//#define SPK_SLAYER_CHAT_BUTTON					SPK_ROOT"SlayerChatButton.spk"
//#define SPK_VAMPIRE_CHAT_BUTTON					SPK_ROOT"VampireChatButton.spk"

//#define RBT_ITEM							_ROOT"rbt\\item.rbt"
//#define RBT_SKILL							_ROOT"rbt\\skill.rbt"
//#define RBT_ITEM_FRAME					_ROOT"rbt\\itemFrame.rbt"
#define ISPK_ITEM							SPK_ROOT"item.ispk"
#define SPK_SKILL_MINI						SPK_ROOT"SkillMini.spk"
#define SPKI_SKILL_MINI						SPK_ROOT"SkillMini.spki"

// ĳ������ Sprite
#define SPK_FACE							SPK_ROOT"Face.spk"
#define SPKI_FACE							SPK_ROOT"Face.spki"
#define SPK_FACE_MAKE						SPK_ROOT"FaceMake.spk"
#define SPK_PARTY_FACE						SPK_ROOT"FaceParty.spk"
#define SPK_PROFILE							_PROFILE_ROOT

#define ISPK_SLAYER						_DATA_DIR_IMAGE_ROOT"Addon.ispk"
#define ISPKI_SLAYER					_DATA_DIR_IMAGE_ROOT"Addon.ispki"
#define ISPK_VAMPIRE					_DATA_DIR_IMAGE_ROOT"Creature.ispk"
#define ISPKI_VAMPIRE					_DATA_DIR_IMAGE_ROOT"Creature.ispki"
#define ISPK_OUSTERS					_DATA_DIR_IMAGE_ROOT"Ousters.ispk"
#define ISPKI_OUSTERS					_DATA_DIR_IMAGE_ROOT"Ousters.ispki"
#define CFPK_SLAYER						_DATA_DIR_IMAGE_ROOT"Addon.cfpk"
#define CFPK_VAMPIRE					_DATA_DIR_IMAGE_ROOT"Creature.cfpk"
#define CFPK_OUSTERS					_DATA_DIR_IMAGE_ROOT"Ousters.cfpk"

// etc
#define SPK_NPCFACE						SPK_ROOT"NpcFace.spk"
#define SPKI_NPCFACE					SPK_ROOT"NpcFace.spki"
//#define SPK_CHAR_MARK					SPK_ROOT"char_mark.spk"
//#define SPK_LEVELUP_SLAYER				SPK_ROOT"SlayerLevelUp.spk"
#define SPK_LEVELUP			SPK_ROOT"LevelUp.spk"
#define SPK_CREDIT						SPK_ROOT"credit.spk"

//#define SPK_USER_REGISTRATION			SPK_ROOT"UserRegistration.spk"

#define SPK_SLAYER_BUY_DIALOG			SPK_ROOT"BuyDialogSlayer.spk"
#define SPK_VAMPIRE_BUY_DIALOG		SPK_ROOT"BuyDialogVampire.spk"
//#define SPK_MONEY_DIALOG		SPK_ROOT"MoneyDialog.spk"

//�����̾� ��Ż
#define SPK_SLAYER_PORTAL_ETC			SPK_ROOT"PortalEtcSlayer.spk"
#define SPK_SLAYER_PORTAL_MAP			SPK_ROOT"PortalMapSlayer.spk"

//��Ƽ
#define SPK_SLAYER_PARTY				SPK_ROOT"PartySlayer.spk"
#define SPK_VAMPIRE_PARTY				SPK_ROOT"PartyVampire.spk"

//���
#define SPK_SLAYER_TEAM					SPK_ROOT"TeamSlayer.spk"
#define SPK_VAMPIRE_TEAM				SPK_ROOT"TeamVampire.spk"
#define SPK_SLAYER_TEAM_MEMBER			SPK_ROOT"TeamMemberSlayer.spk"
#define SPK_VAMPIRE_TEAM_MEMBER			SPK_ROOT"TeamMemberVampire.spk"

// HPBar
#define SPK_HPBAR_SLAYER				SPK_ROOT"HPBarSlayer.spk"
#define SPK_HPBAR_VAMPIRE				SPK_ROOT"HPBarVampire.spk"

// MINIMAP
#define SPK_MINIMAP_SLAYER				SPK_ROOT"MinimapSlayer.spk"
#define SPK_MINIMAP_VAMPIRE				SPK_ROOT"MinimapVampire.spk"

// CHATTING
#define SPK_CHATTING_SLAYER				SPK_ROOT"ChattingSlayer.spk"
#define SPK_CHATTING_VAMPIRE			SPK_ROOT"ChattingVampire.spk"

// EFFECT_STATUS
#define SPK_EFFECT_STATUS_SLAYER		SPK_ROOT"EffectStatusSlayer.spk"
#define SPK_EFFECT_STATUS_VAMPIRE		SPK_ROOT"EffectStatusVampire.spk"
//#define FILE_BACKUP_ID					"Set\\backupid"

// FILE DIALOG
#define SPK_FILE_DIALOG					SPK_ROOT"FileDialog.spk"
#define DIR_PROFILE							_PROFILE_ROOT"

#define SPK_QUESTION					SPK_ROOT"Question.spk"


// SoulChain
#define SPK_SOUL_CHAIN					SPK_ROOT"Trace.spk"

// �̿� ��� 
#define SPK_USE_GRADE					SPK_ROOT"UseGrade.spk"

#define SPK_XMAS_CARD					SPK_ROOT"XMasCard.spk"

#define SPK_LOTTERY_CARD				SPK_ROOT"LotteryCard.spk"
#define SPKI_LOTTERY_CARD				SPK_ROOT"LotteryCard.spki"

// �Խ���  2003.5.12 by sonee
#define SPK_BULLETIN_BOARD				SPK_ROOT"BulletinBoard.spk"

// �ƿ콺���� �������̽� ����
#define SPK_ASSEMBLE_BOX_OUSTERS		SPK_ROOT"AssembleMessageBoxOusters.spk"
#define SPK_ASSEMBLE_BOX_BUTTON_OUSTERS	SPK_ROOT"AssembleMessageBoxButtonOusters.spk"
#define SPK_OUSTERS_GAMEMENU			SPK_ROOT"GameMenuOusters.spk"
#define SPK_OPTION_OUSTERS				SPK_ROOT"OptionOusters.spk"
#define SPK_OUSTERS_MAIN				SPK_ROOT"MainOusters.spk"
#define SPK_OUSTERS_GEAR_SLOT			SPK_ROOT"GearSlotOusters.spk"
#define SPK_OUSTERS_GEAR				SPK_ROOT"GearOusters.spk"
#define SPK_OUSTERS_GEAR_SLOT			SPK_ROOT"GearSlotOusters.spk"
#define SPK_OUSTERS_INVENTORY			SPK_ROOT"InventoryOusters.spk"
#define SPK_OUSTERS_QUICKITEM			SPK_ROOT"QuickitemOusters.spk"
#define SPK_OUSTERS_PARTY				SPK_ROOT"PartyOusters.spk"
#define SPK_OUSTERS_TEAM				SPK_ROOT"TeamOusters.spk"
#define SPK_OUSTERS_TEAM_MEMBER			SPK_ROOT"TeamMemberOusters.spk"
#define SPK_HPBAR_OUSTERS				SPK_ROOT"HPBarOusters.spk"
#define SPK_MINIMAP_OUSTERS				SPK_ROOT"MinimapOusters.spk"
#define SPK_CHATTING_OUSTERS			SPK_ROOT"ChattingOusters.spk"
#define SPK_EFFECT_STATUS_OUSTERS		SPK_ROOT"EffectStatusOusters.spk"
#define SPK_OUSTERS_SHOP_STORAGE		SPK_ROOT"Shop&StorageOusters.spk"
#define SPK_OUSTERS_SYS_BUTTON			SPK_ROOT"SysButtonOusters.spk"
#define SPK_OUSTERS_SKILL_ETC			SPK_ROOT"SkillEtcOusters.spk"
#define SPK_EXCHANGE_OUSTERS			SPK_ROOT"ExchangeOusters.spk"
//add by viva
#define SPK_OUSTERS_FRIEND				SPK_ROOT"OustersFriend.spk"
#define SPK_SLAYER_FRIEND				SPK_ROOT"SlayerFriend.spk"
#define SPK_VAMPIRE_FRIEND				SPK_ROOT"VampireFriend.spk"

// �ͽ� ���� 2003.5.20 by gony
#define SPK_MIXING_FORGE				SPK_ROOT"MixingForge.spk"

#define SPK_HORN						SPK_ROOT"PortalEtcOusters.spk"//SPK_ROOT"Horn.spk"
#define SPK_HORN_MAP					SPK_ROOT"PortalMapOusters.spk"//SPK_ROOT"HornMap.spk"

#define SPK_MAILBOX						SPK_ROOT"MailBox.spk"

// CTF Status 2003.7.24 by sonee

#define SPK_CTF_STATUS					SPK_ROOT"CTF.spk"

#define RPK_INFO						_DATA_DIR_INFO_ROOT"infodata.rpk"
#define KOREAN_INTERFACE_INFO			"Interface.inf"
#define CHINESE_INTERFACE_INFO			"ChinaInterface.inf"
#define JAPANESE_INTERFACE_INFO			"JapanInterface.inf"

// RegenTower 2003.10.14 by sonee

#define RPK_HOLYLAND_MINIMAP			SPK_ROOT"HolylandMinimap.spk"
// #define HELP_MESSAGE_DESC				SPK_ROOT"HelpMessage.spk"

//2004, 5, 31 sobeit add start - sms message
#define SPK_SLAYER_SMS_MESSAGE			SPK_ROOT"sms_slayer_main.spk"
#define SPK_VAMPIRE_SMS_MESSAGE			SPK_ROOT"sms_vampire_main.spk"
#define SPK_OUSTERS_SMS_MESSAGE			SPK_ROOT"sms_ousters_main.spk"
#define SPK_SLAYER_SMS_LIST			SPK_ROOT"sms_slayer_list.spk"
#define SPK_VAMPIRE_SMS_LIST			SPK_ROOT"sms_vampire_list.spk"
#define SPK_OUSTERS_SMS_LIST			SPK_ROOT"sms_ousters_list.spk"
#define SPK_SLAYER_SMS_RECORD			SPK_ROOT"sms_slayer_record.spk"
#define SPK_VAMPIRE_SMS_RECORD			SPK_ROOT"sms_vampire_record.spk"
#define SPK_OUSTERS_SMS_RECORD			SPK_ROOT"sms_ousters_record.spk"
#define SPK_SLAYER_SMS_SCROLL			SPK_ROOT"sms_slayer_scroll.spk"
#define SPK_VAMPIRE_SMS_SCROLL			SPK_ROOT"sms_vampire_scroll.spk"
#define SPK_OUSTERS_SMS_SCROLL			SPK_ROOT"sms_ousters_scroll.spk"

//2004, 5, 31 sobeit add end - sms message

// 2004, 6, 21 sobeit add start -  ���̹� ��Ʈ�� �̹���
#define SPK_SLAYER_NAMING			SPK_ROOT"NamingSlayer.spk"
#define SPK_VAMPIRE_NAMING			SPK_ROOT"NamingVampire.spk"
#define SPK_OUSTERS_NAMING			SPK_ROOT"NamingOusters.spk"
// 2004, 6, 21 sobeit add end -  ���̹� ��Ʈ�� �̹���
// 2004, 7, 2 sobeit add start -  ����Ʈ �Ŵ��� ui
#define SPK_SLAYER_QUESTMAMAGER			SPK_ROOT"QuestManager_Slayer.spk"
#define SPK_VAMPIRE_QUESTMAMAGER		SPK_ROOT"QuestManager_Vampire.spk"
#define SPK_OUSTERS_QUESTMAMAGER		SPK_ROOT"QuestManager_Ousters.spk"
// 2004, 7, 2 sobeit add end -  ����Ʈ �Ŵ��� ui

// 2004, 7, 9 sobeit add start
#define QUEST_XML_FILE					"SimpleGQuest.xml"
#define QUEST_EVENT_XML_FILE			"EventGQuest.xml"
// 2004, 7, 9 sobeit add end

// 2004, 7, 22 sobeit add start
#define SPK_QUEST_MODIFY						SPK_ROOT"QuestModify.spk"
// 2004, 7, 22 sobeit add end

// 2004, 8, 2 sobeit add start
#define SPK_TITLE_OPTION						SPK_ROOT"OptionTitle.spk"
#define SPK_TITLE_OPTION_SCROLL					SPK_ROOT"OptionTitleScroll.spk"

#define SPK_PERSNALSHOP_SLAYER						   	SPK_ROOT"SlayerPersnalShop.spk"
#define SPK_PERSNALSHOP_VAMPIRE							SPK_ROOT"VampirePersnalShop.spk"
#define SPK_PERSNALSHOP_OUSTERS							SPK_ROOT"OustersPersnalShop.spk"
// 2004, 8, 2 sobeit add end

#define SPK_PERSNALSHOP_SLAYER						   	SPK_ROOT"SlayerPersnalShop.spk"
#define SPK_PERSNALSHOP_VAMPIRE							SPK_ROOT"VampirePersnalShop.spk"
#define SPK_PERSNALSHOP_OUSTERS							SPK_ROOT"OustersPersnalShop.spk"
// 2004, 12, 1, sobeit add start - ¯ ����Ʈ
#define SPK_SLAYER_POWERJJANG				SPK_ROOT"Powerjjang_Slayer.spk"
#define SPK_VAMPIRE_POWERJJANG				SPK_ROOT"Powerjjang_Vampire.spk"
#define SPK_OUSTERS_POWERJJANG				SPK_ROOT"Powerjjang_Ousters.spk"

#define SPK_ITEM_POWERJJANG					SPK_ROOT"Powerjjang_Item.spk"
#define SPK_SLAYER_ADVANCEMENTGEAR			SPK_ROOT"GearSlayerAdvancement.spk"
#define SPK_VAMPIRE_ADVANCEMENTGEAR			SPK_ROOT"GearVampireAdvancement.spk"
#define SPK_OUSTERS_ADVANCEMENTGEAR			SPK_ROOT"GearOustersAdvancement.spk"

// 2004, 12, 1, sobeit add end

// 2004-12-13 added about advancement class, by sonee;
#define ISPK_ADVANCEMENT_VAMPIRE_MAN	_DATA_DIR_IMAGE_ROOT"ACVampireMan.ispk"
#define ISPK_ADVANCEMENT_VAMPIRE_WOMAN	_DATA_DIR_IMAGE_ROOT"ACVampireWoman.ispk"
#define ISPK_ADVANCEMENT_OUSTERS		_DATA_DIR_IMAGE_ROOT"ACOusters.ispk"
#define ISPK_ADVANCEMENT_SLAYER_MAN		_DATA_DIR_IMAGE_ROOT"ACSlayerMan.ispk"
#define ISPK_ADVANCEMENT_SLAYER_WOMAN	_DATA_DIR_IMAGE_ROOT"ACSlayerWoman.ispk"

#define CFPK_ADVANCEMENT_SLAYER_MAN		_DATA_DIR_IMAGE_ROOT"ACSlayerMan.cfpk"
#define CFPK_ADVANCEMENT_SLAYER_WOMAN	_DATA_DIR_IMAGE_ROOT"ACSlayerWoman.cfpk"
#define CFPK_ADVANCEMENT_VAMPIRE_MAN	_DATA_DIR_IMAGE_ROOT"ACVampireMan.cfpk"
#define CFPK_ADVANCEMENT_VAMPIRE_WOMAN	_DATA_DIR_IMAGE_ROOT"ACVampireWoman.cfpk"
#define CFPK_ADVANCEMENT_OUSTERS		_DATA_DIR_IMAGE_ROOT"ACOusters.cfpk"

//add by zdj
#define SPK_SLAYER_INVENTORY_SUB			SPK_ROOT"InventorySlayerSub.spk"
#define SPK_VAMPIRE_INVENTORY_SUB			SPK_ROOT"InventoryVampireSub.spk"
#define SPK_OUSTERS_INVENTORY_SUB			SPK_ROOT"InventoryOustersSub.spk"

// add by Sonic
#define SPK_TITLE_1024							SPK_ROOT"Title_1024.spk"
#define SPK_CHATTING_OUSTERS_1024				SPK_ROOT"ChattingOusters_1024.spk"
#define SPK_CHATTING_SLAYER_1024				SPK_ROOT"ChattingSlayer_1024.spk"
#define SPK_CHATTING_VAMPIRE_1024				SPK_ROOT"ChattingVampire_1024.spk"
#define SPK_COMMON_1024							SPK_ROOT"Common_1024.spk"
#define SPK_LOGIN_1024							SPK_ROOT"Login_1024.spk"
#define SPK_TITLE_OPTION_1024					SPK_ROOT"OptionTitle_1024.spk"
#define SPK_PROGRESS_1024						SPK_ROOT"Progress_1024.spk"
#define	SPK_SCROLL_BAR_1024						SPK_ROOT"ScrollBar_1024.spk"
// New File
#define SPK_TITLE_BACK							SPK_ROOT"TitleBack.spk"


#endif