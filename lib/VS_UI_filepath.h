#ifndef __VS_UI_FILEPATH_H__
#define __VS_UI_FILEPATH_H__

// Client가 Release되면 Directory가 변할 일이 없기 때문에 directory service 사용 안함.

#define _ROOT							"data\\ui\\"


#define TXT_ROOT						_ROOT"txt\\"
//#define TXT_TUTORIAL_BOOKLIST			TXT_ROOT"Booklist.txt"
#define TXT_TUTORIAL_BRIEFING			"Briefing.txt"
#define TXT_TUTORIAL_COMPUTER			"Computer.tre"
#define INF_SLAYER_PORTAL				"SlayerPortal.inf"
#define TXT_TUTORIAL_WELCOME_SLAYER		"WelcomeSlayer.txt"
#define TXT_TUTORIAL_WELCOME_VAMPIRE	"WelcomeVampire.txt"

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
#define	FILE_INFO_STR_EXP						"Data\\Info\\STRExp.inf"
#define	FILE_INFO_DEX_EXP						"Data\\Info\\DEXExp.inf"
#define	FILE_INFO_INT_EXP						"Data\\Info\\INTExp.inf"
#define	FILE_INFO_VAMPIRE_EXP					"Data\\Info\\VampireExp.inf"
#define	FILE_INFO_SKILL_DOMAIN_EXP				"Data\\Info\\DomainExp.inf"
#define	FILE_INFO_SKILL_INFO					"Data\\Info\\SkillInfo.inf"
#define FILE_INFO_ITEM_INFO						"Data\\Info\\item.inf"
#define FILE_INFO_CREATURE_INFO					"Data\\Info\\Creature.inf"
#define FILE_INFO_NPC_INFO						"Data\\Info\\NPC.inf"
#define FILE_INFO_SOUND_INFO					"Data\\Info\\Sound.inf"
#define FILE_INFO_GUILD_MAPPER					"Data\\Info\\GuildMapper.inf"
#define	FILE_INFO_RANK_BONUS_TABLE				"Data\\Info\\RankBonus.inf"
#define FILE_INFO_SLAYER_RANK_EXP				"Data\\Info\\SlayerRankExp.inf"
#define FILE_INFO_VAMPIRE_RANK_EXP				"Data\\Info\\VampireRankExp.inf"
#define FILE_INFO_ZONE_TABLE					"Data\\Info\\Zone.inf"
#define SPK_GAME_BACK							"data\\gameback.spk"
#define SPK_GUILD_MARK							"Data\\Image\\GuildMark.spk"

#define	SPK_GRADE_SLAYER						"Data\\Image\\GradeSlayer.spk"
#define	SPK_GRADE_VAMPIRE						"Data\\Image\\GradeVampire.spk"

#endif

// skill tree back image
//#define SPK_SKILLTREE_BACK_ENCHANT	_ROOT"spk\\skilltree_back_enchant.spk"
//#define SPK_SKILLTREE_BACK_SWORD		_ROOT"spk\\skilltree_back_sword.spk"
//#define SPK_SKILLTREE_BACK_BLADE		_ROOT"spk\\skilltree_back_blade.spk"
//#define SPK_SKILLTREE_BACK_HEAL		_ROOT"spk\\skilltree_back_heal.spk"
//#define SPK_SKILLTREE_BACK_GUN		_ROOT"spk\\skilltree_back_gun.spk"
//#define SPK_SKILLTREE_BACK_VAMPIRE	_ROOT"spk\\skilltree_back_vampire.spk"

// skill tree hightlight icon image
//#define SPK_SKILLTREE_ICO_ENCHANT	_ROOT"spk\\skilltree_ico_enchant.spk"
//#define SPK_SKILLTREE_ICO_SWORD		_ROOT"spk\\skilltree_ico_sword.spk"
//#define SPK_SKILLTREE_ICO_BLADE		_ROOT"spk\\skilltree_ico_blade.spk"
//#define SPK_SKILLTREE_ICO_HEAL		_ROOT"spk\\skilltree_ico_heal.spk"
//#define SPK_SKILLTREE_ICO_GUN			_ROOT"spk\\skilltree_ico_gun.spk"
//#define SPK_SKILLTREE_ICO_VAMPIRE	_ROOT"spk\\skilltree_ico_vampire.spk"

//#define SPK_SKILLTREE_ETC_SLAYER		_ROOT"spk\\skilltree_etc_slayer.spk"
//#define SPK_SKILLTREE_ETC_VAMPIRE	_ROOT"spk\\skilltree_etc_vampire.spk"

#define	IFR_ROOT						_ROOT"txt\\"
#define	IFR_UI							IFR_ROOT"ui.ifr"
#define	IFR_ACTION						IFR_ROOT"action.ifr"
#define	IFR_SKILL						IFR_ROOT"skill.ifr"
#define	IFR_INVENTORY					IFR_ROOT"inventory.ifr"
#define	IFR_USE_ITEM					IFR_ROOT"useitem.ifr"

#define SPK_MINIMAP						_ROOT"spk\\Minimap.spk"
#define SPKI_MINIMAP					_ROOT"spk\\Minimap.spki"	// by sigi 2001.9.5

// tutorial image by larosel
#define SPK_TUTORIAL_BOOKCASE			_ROOT"spk\\TutorialBookcase.spk"
#define SPK_TUTORIAL_BOOK				_ROOT"spk\\TutorialBook.spk"
//#define SPK_TUTORIAL_BRIEFING			_ROOT"spk\\Tutorial_Briefing.spk"
#define SPK_TUTORIAL_COMPUTER			_ROOT"spk\\TutorialComputer.spk"
//#define SPK_TUTORIAL_EXIT				_ROOT"spk\\Tutorial_Exit.spk"
#define SPK_ITEM						_ROOT"spk\\ItemDescription.spk"
#define SPKI_ITEM						_ROOT"spk\\ItemDescription.spki"

#define SPK_ELEVATOR					_ROOT"spk\\Elevator.spk"
//#define SPK_TIME						_ROOT"spk\\Time.spk"
//#define SPKI_TIME						_ROOT"spk\\Time.spki"

// skill view
//#define SPK_SKILLVIEW_SLAYER			_ROOT"spk\\SlayerSkillView.spk"
//#define SPK_SKILLVIEW_SLAYER_GRADE		_ROOT"spk\\SlayerSkillViewGrade.spk"
//#define SPK_SKILLVIEW_SLAYER_DOMAIN		_ROOT"spk\\SlayerSkillViewDomain.spk"
//#define SPK_SKILLVIEW_VAMPIRE			_ROOT"spk\\VampireSkillView.spk"
//#define SPK_SKILLVIEW_VAMPIRE_GRADE		_ROOT"spk\\VampireSkillViewGrade.spk"
//#define SPK_SKILLVIEW_VAMPIRE_DOMAIN	_ROOT"spk\\VampireSkillViewDomain.spk"
#define SPK_SKILL_ICON					_ROOT"spk\\SkillIcon.spk"
#define SPKI_SKILL_ICON					_ROOT"spk\\SkillIcon.spki"
//#define SPK_SKILLVIEW_ETC				_ROOT"spk\\SkillViewEtc.spk"

#define SPK_MINE_PROGRESS				_ROOT"spk\\MineProgress.spk"

////////////
// Common
///////////
//#define SPK_COMMON_BUTTON				_ROOT"spk\\common_button.spk"
//#define SPK_COMMON_IN_TITLE			_ROOT"spk\\common_in_title.spk"

#define	SPK_BOX								_ROOT"spk\\AssembleMessageBox.spk"
#define	SPK_SCROLL_BAR						_ROOT"spk\\ScrollBar.spk"
#define SPK_ASSEMBLE_BOX_SLAYER				_ROOT"spk\\AssembleMessageBoxSlayer.spk"
#define SPK_ASSEMBLE_BOX_BUTTON_SLAYER		_ROOT"spk\\AssembleMessageBoxButtonSlayer.spk"
//#define SPK_ASSEMBLE_BOX_ETC				_ROOT"spk\\AssembleMessageBoxEtc.spk"
#define SPK_ASSEMBLE_BOX_VAMPIRE			_ROOT"spk\\AssembleMessageBoxVampire.spk"
#define SPK_ASSEMBLE_BOX_BUTTON_VAMPIRE		_ROOT"spk\\AssembleMessageBoxButtonVampire.spk"
//#define SPK_ASSEMBLE_BOX_ETC_VAMPIRE		_ROOT"spk\\AssembleMessageBoxEtcVampire.spk"
//#define SPK_ASSEMBLE_BOX_COMMON				_ROOT"spk\\AssembleMessageBoxCommon.spk"
//#define SPK_ASSEMBLE_BOX_BUTTON_COMMON		_ROOT"spk\\AssembleMessageBoxButtonCommon.spk"

#define SPK_EXCHANGE_SLAYER						_ROOT"spk\\ExchangeSlayer.spk"
//#define SPK_EXCHANGE_VAMPIRE			_ROOT"spk\\exchangeVampire.spk"
#define SPK_SLAYER_SHOP_STORAGE			_ROOT"spk\\Shop&StorageSlayer.spk"
#define SPK_VAMPIRE_SHOP_STORAGE			_ROOT"spk\\Shop&StorageVampire.spk"

// title
#define SPK_ANI_TITLE					_ROOT"spk\\TitleAni.spk"
#define SPK_ANI_SYMBOL					_ROOT"spk\\Symbol.spk"
#define SPK_TITLE_MENU_DEFAULT		_ROOT"spk\\TitleMenuDefault.spk"
#define SPK_TITLE_MENU_SELECT			_ROOT"spk\\TitleMenuSelect.spk"

// login
//#define SPK_LOGIN_MENU_DEFAULT		_ROOT"spk\\login_menu_default.spk"
#define SPK_LOGIN_MENU			_ROOT"spk\\LoginMenu.spk"

// characters
#define SPK_COMMON						_ROOT"spk\\Common.spk"
#define SPK_SERVER_SELECT					_ROOT"spk\\ServerSelect.spk"
#define	SPK_CHAR_MANAGER				_ROOT"spk\\CharManager.spk"
#define SPK_CHAR_CREATE					_ROOT"spk\\CharCreate.spk"

//#define SPK_CHARACTERS_MENU_DEFAULT	_ROOT"spk\\characters_menu_default.spk"
//#define SPK_CHARACTERS_MENU_SELECT	_ROOT"spk\\characters_menu_select.spk"

// New character
//#define SPK_NEWCHARACTER_BACK			_ROOT"spk\\NewCharacter_back.spk"
//#define SPK_NEWCHARACTER_ETC			_ROOT"spk\\NewCharacter_etc.spk"
//#define SPK_NEWCHARACTER_MENU_DEFAULT	_ROOT"spk\\NewCharacter_menu_default.spk"
//#define SPK_NEWCHARACTER_MENU_SELECT	_ROOT"spk\\NewCharacter_menu_select.spk"

// System button
#define SPK_SLAYER_SYS_BUTTON			_ROOT"spk\\SysButtonSlayer.spk"
#define SPK_VAMPIRE_SYS_BUTTON		_ROOT"spk\\SysButtonVampire.spk"

// Slayer skill etc
#define SPK_SLAYER_SKILL_ETC			_ROOT"spk\\SkillEtcSlayer.spk"
#define SPK_VAMPIRE_SKILL_ETC			_ROOT"spk\\SkillEtcVampire.spk"

//#define IMAGE_BOXBACK					_ROOT"back.tga"
#define SPK_MOUSE_POINTER				_ROOT"spk\\MousePointer.spk"
#define SPK_TITLE							_ROOT"spk\\Title.spk"
#define SPK_TITLE_1024							_ROOT"spk\\Title_1024.spk"
//#define SPK_BBS							_ROOT"spk\\BBS.spk"
#define SPK_PROGRESS					_ROOT"spk\\Progress.spk"
//#define SPK_PROGRESS_EVENT				_ROOT"spk\\progressevent.spk"
#define SPK_PROGRESS_CHARACTER			_ROOT"spk\\ProgressChar.spk"
#define SPKI_PROGRESS_CHARACTER			_ROOT"spk\\ProgressChar.spki"
#define SPK_PROGRESS_EVENT				_ROOT"spk\\ProgressEvent.spk"
//#define SPK_PROGRESS_VAMPIRE_RAND	_ROOT"spk\\progress_vampire_rand.spk"
#define	SPK_INFO_SLAYER					_ROOT"spk\\InfoSlayer.spk"
#define	SPK_INFO_VAMPIRE				_ROOT"spk\\InfoVampire.spk"
//#define SPK_CHAR_MAIN					_ROOT"spk\\char_main.spk"
//#define SPK_SLAYER_CHARINFO				_ROOT"spk\\SlayerCharInfo.spk"
//#define SPK_VAMPIRE_CHARINFO			_ROOT"spk\\VampireCharInfo.spk"
//#define SPK_CHAR_DELETE					_ROOT"spk\\char_delete.spk"
//#define SPK_CHAR_DELETE_VAMPIRE			_ROOT"spk\\char_delete_vampire.spk"
//#define SPK_COMMON						_ROOT"spk\\Common.spk"

#define SPK_SLAYER_GAMEMENU				_ROOT"spk\\GameMenuSlayer.spk"
#define SPK_VAMPIRE_GAMEMENU			_ROOT"spk\\GameMenuVampire.spk"

#define SPK_OPTION_SLAYER				_ROOT"spk\\OptionSlayer.spk"
#define SPK_OPTION_VAMPIRE				_ROOT"spk\\OptionVampire.spk"
#define SPK_OPTION						_ROOT"spk\\Option.spk"

#define SPK_SLAYER_MAIN					_ROOT"spk\\MainSlayer.spk"
#define SPK_VAMPIRE_MAIN				_ROOT"spk\\MainVampire.spk"
//#define SPK_BUTTON1						_ROOT"spk\\ani_button1.spk"
#define SPK_LOGIN							_ROOT"spk\\Login.spk"
#define SPK_SLAYER_GEAR					_ROOT"spk\\GearSlayer.spk"
#define SPK_SLAYER_GEAR_SLOT			_ROOT"spk\\GearSlotSlayer.spk"
#define SPK_VAMPIRE_GEAR_SLOT			_ROOT"spk\\GearSlotVampire.spk"
#define SPK_VAMPIRE_GEAR				_ROOT"spk\\GearVampire.spk"
#define SPK_SLAYER_INVENTORY			_ROOT"spk\\InventorySlayer.spk"
#define SPK_VAMPIRE_INVENTORY			_ROOT"spk\\InventoryVampire.spk"
//#define SPK_SLAYER_ETC					_ROOT"spk\\slayer_etc.spk"
//#define SPK_VAMPIRE_ETC					_ROOT"spk\\vampire_etc.spk"
#define SPK_SLAYER_QUICKITEM			_ROOT"spk\\QuickitemSlayer.spk"
//#define SPK_SLAYER_DESC					_ROOT"spk\\SlayerDesc.spk"
//#define SPK_VAMPIRE_DESC				_ROOT"spk\\VampireDesc.spk"
//#define SPK_AXS							_ROOT"spk\\axs.spk"
//#define SPK_INVENTORY_BACK_COLOR		_ROOT"spk\\InventoryBackColor.spk"
//#define SPK_INVENTORY_BACK_COLOR_VAMPIRE		_ROOT"spk\\VampireInventoryBackColor.spk"
//#define SPK_BOX_LINE						_ROOT"spk\\box_line.spk"
//#define SPK_OK								_ROOT"spk\\ok.spk"

// PDS
//#define SPK_PDS_PCS						_ROOT"spk\\pds_pcs.spk"
//#define SPK_PDS_BUTTON					_ROOT"spk\\pds_button.spk"
//#define SPK_PDS_SUB						_ROOT"spk\\pds_sub.spk"

#define SPK_PARTY_CURSOR					_ROOT"spk\\PartyCursor.spk"

// CHAT
//#define SPK_SLAYER_CHAT_BUTTON					_ROOT"spk\\SlayerChatButton.spk"
//#define SPK_VAMPIRE_CHAT_BUTTON					_ROOT"spk\\VampireChatButton.spk"

//#define RBT_ITEM							_ROOT"rbt\\item.rbt"
//#define RBT_SKILL							_ROOT"rbt\\skill.rbt"
//#define RBT_ITEM_FRAME					_ROOT"rbt\\itemFrame.rbt"
#define ISPK_ITEM							_ROOT"spk\\item.ispk"
#define SPK_SKILL_MINI						_ROOT"spk\\SkillMini.spk"
#define SPKI_SKILL_MINI						_ROOT"spk\\SkillMini.spki"

// 캐릭생성 Sprite
#define SPK_FACE							_ROOT"spk\\Face.spk"
#define SPKI_FACE							_ROOT"spk\\Face.spki"
#define SPK_FACE_MAKE						_ROOT"spk\\FaceMake.spk"
#define SPK_PARTY_FACE						_ROOT"spk\\FaceParty.spk"
#define SPK_PROFILE							"UserSet\\"

#define ISPK_SLAYER						"data\\Image\\Addon.ispk"
#define ISPK_VAMPIRE					"data\\Image\\Creature.ispk"
#define ISPKI_SLAYER					"data\\Image\\Addon.ispki"
#define ISPKI_VAMPIRE					"data\\Image\\Creature.ispki"
#define CFPK_SLAYER						"data\\Image\\Addon.cfpk"
#define CFPK_VAMPIRE					"data\\Image\\Creature.cfpk"

// etc
#define SPK_NPCFACE						_ROOT"spk\\NpcFace.spk"
#define SPKI_NPCFACE					_ROOT"spk\\NpcFace.spki"
//#define SPK_CHAR_MARK					_ROOT"spk\\char_mark.spk"
//#define SPK_LEVELUP_SLAYER				_ROOT"spk\\SlayerLevelUp.spk"
#define SPK_LEVELUP			_ROOT"spk\\LevelUp.spk"
#define SPK_CREDIT						_ROOT"spk\\credit.spk"

//#define SPK_USER_REGISTRATION			_ROOT"spk\\UserRegistration.spk"

#define SPK_SLAYER_BUY_DIALOG			_ROOT"spk\\BuyDialogSlayer.spk"
#define SPK_VAMPIRE_BUY_DIALOG		_ROOT"spk\\BuyDialogVampire.spk"
//#define SPK_MONEY_DIALOG		_ROOT"spk\\MoneyDialog.spk"

//슬레이어 포탈
#define SPK_SLAYER_PORTAL_ETC			_ROOT"spk\\PortalEtcSlayer.spk"
#define SPK_SLAYER_PORTAL_MAP			_ROOT"spk\\PortalMapSlayer.spk"

//파티
#define SPK_SLAYER_PARTY				_ROOT"spk\\PartySlayer.spk"
#define SPK_VAMPIRE_PARTY				_ROOT"spk\\PartyVampire.spk"

//길드
#define SPK_SLAYER_TEAM					_ROOT"spk\\TeamSlayer.spk"
#define SPK_VAMPIRE_TEAM				_ROOT"spk\\TeamVampire.spk"
#define SPK_SLAYER_TEAM_MEMBER			_ROOT"spk\\TeamMemberSlayer.spk"
#define SPK_VAMPIRE_TEAM_MEMBER			_ROOT"spk\\TeamMemberVampire.spk"

// HPBar
#define SPK_HPBAR_SLAYER				_ROOT"spk\\HPBarSlayer.spk"
#define SPK_HPBAR_VAMPIRE				_ROOT"spk\\HPBarVampire.spk"

// MINIMAP
#define SPK_MINIMAP_SLAYER				_ROOT"spk\\MinimapSlayer.spk"
#define SPK_MINIMAP_VAMPIRE				_ROOT"spk\\MinimapVampire.spk"

// CHATTING
#define SPK_CHATTING_SLAYER				_ROOT"spk\\ChattingSlayer.spk"
#define SPK_CHATTING_VAMPIRE			_ROOT"spk\\ChattingVampire.spk"

// EFFECT_STATUS
#define SPK_EFFECT_STATUS_SLAYER		_ROOT"spk\\EffectStatusSlayer.spk"
#define SPK_EFFECT_STATUS_VAMPIRE		_ROOT"spk\\EffectStatusVampire.spk"
//#define FILE_BACKUP_ID					"Set\\backupid"

// FILE DIALOG
#define SPK_FILE_DIALOG					_ROOT"spk\\FileDialog.spk"
#define DIR_PROFILE							"UserSet"

#define SPK_QUESTION					_ROOT"spk\\Question.spk"


// SoulChain
#define SPK_SOUL_CHAIN					_ROOT"spk\\Trace.spk"
// 이용 등급 
#define SPK_USE_GRADE					_ROOT"spk\\UseGrade.spk"

#define SPK_XMAS_CARD					_ROOT"spk\\XMasCard.spk"




#endif