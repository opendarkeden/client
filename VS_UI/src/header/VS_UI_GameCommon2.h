/*-----------------------------------------------------------------------------

	VS_UI_GameCommon2.h

	Game common interface.

	Common: MiniGame

	2003.6.26. sonee

-----------------------------------------------------------------------------*/
#ifndef __VS_UI_GAME_COMMON2_H__
#define __VS_UI_GAME_COMMON2_H__

#include "VS_UI_Base.h"
#include "VS_UI_Util.h"
#include "VS_UI_description.h"
#include "VS_UI_ExtraDialog.h"

#include "SkillDef.h"
#include "MItem.h"
#include "MMoneyManager.h"
#include "MSkillManager.h"
#include "ClientFunction.h"
#include "MZone.h"
#include "MHelpMessageManager.h"
#include <map>

#include "SXml.h"
#include "MInventory.h"
//-----------------------------------------------------------------------------
// class C_VS_UI_HORN
//
//-----------------------------------------------------------------------------
class C_VS_UI_ARROW_TILE : public Window, public Exec, public ButtonVisual
{
public:
private:
	enum TILE_ATTRIBUTE
	{
		ATTR_NORMAL,
		ATTR_SLIDING,
		ATTR_2_MOVE,	
		ATTR_TRAP,
	};
	enum PLAYER_STATUS
	{
		PLAYER_STATUS_READY,
		PLAYER_STATUS_NORMAL,
		PLAYER_STATUS_TRAP,
		PLAYER_STATUS_DIE,
		PLAYER_STATUS_FINISH,
	};
	enum PLAYER_TYPE
	{
		PLAYER_USER,
		PLAYER_NPC,
		PLAYER_MONSTER,			
	};
	struct S_TILE
	{
		TILE_ATTRIBUTE		Attr;
		short				Direction;
		int					AniFrame[3];
		int					RemainRotationAngle;
	};
	struct S_CHARACTER
	{
		short				ID;
		int					X,Y;
		PLAYER_STATUS		Status;
		bool				bCanRotation;
		int					MoveCount;
		int					RotationCount;
		DWORD				StartTime;
		DWORD				MoveTime;
		DWORD				DelayTime;
		DWORD				EndTime;
		bool				bActive;
		PLAYER_TYPE			PlayerType;		
	};	
	enum EXEC_ID
	{
		ROTATION_LEFT_ID,
		ROTATION_RIGHT_ID,
		MOVE_ID,
		CLOSE_ID,
		HELP_ID
	};
	enum
	{
		DIRECTION_UP = 0,		
		DIRECTION_RIGHT,
		DIRECTION_DOWN,			
		DIRECTION_LEFT,
		DIRECTION_MAX
	};

	enum GAME_STAGE
	{
		STAGE_NONE,
		STAGE_5X5,
		STAGE_7X7,
		STAGE_9X9,
		STAGE_END,
		STAGE_MAX,
	};

	enum SPK
	{
		SPK_ARROW_UP,
		SPK_ARROW_UP_RIGHT_1,
		SPK_ARROW_UP_RIGHT_2,
		SPK_ARROW_UP_RIGHT_3,
		SPK_ARROW_RIGHT,
		SPK_ARROW_RIGHT_DOWN_1,
		SPK_ARROW_RIGHT_DOWN_2,
		SPK_ARROW_RIGHT_DOWN_3,
		SPK_ARROW_DOWN,
		SPK_ARROW_DOWN_LEFT_1,
		SPK_ARROW_DOWN_LEFT_2,
		SPK_ARROW_DOWN_LEFT_3,
		SPK_ARROW_LEFT,
		SPK_ARROW_LEFT_UP_1,
		SPK_ARROW_LEFT_UP_2,
		SPK_ARROW_LEFT_UP_3,
		
		
		SPK_ROTATION_RIGHT,
		SPK_ROTATION_RIGHT_HILIGHTED,
		SPK_ROTATION_RIGHT_PUSHED,
		SPK_ROTATION_LEFT,
		SPK_ROTATION_LEFT_HILIGHTED,
		SPK_ROTATION_LEFT_PUSHED,
		SPK_MOVE,
		SPK_MOVE_HILIGHTED,
		SPK_MOVE_PUSHED,
		SPK_GOAL,
		SPK_START,
		
		SPK_CLOSE,
		SPK_CLOSE_HILIGHTED,
		SPK_CLOSE_PUSHED,
		SPK_HELP,
		SPK_HELP_HILIGHTED,
		SPK_HELP_PUSHED,
		
		SPK_SLAYER_MALE_UP,
		SPK_SLAYER_MALE_RIGHT,
		SPK_SLAYER_MALE_DOWN,
		SPK_SLAYER_MALE_LEFT,
		SPK_SLAYER_FEMALE_UP,
		SPK_SLAYER_FEMALE_RIGHT,
		SPK_SLAYER_FEMALE_DOWN,
		SPK_SLAYER_FEMALE_LEFT,
		SPK_VAMPIRE_MALE_UP,
		SPK_VAMPIRE_MALE_RIGHT,
		SPK_VAMPIRE_MALE_DOWN,
		SPK_VAMPIRE_MALE_LEFT,
		SPK_VAMPIRE_FEMALE_UP,
		SPK_VAMPIRE_FEMALE_RIGHT,
		SPK_VAMPIRE_FEMALE_DOWN,
		SPK_VAMPIRE_FEMALE_LEFT,
		SPK_OUSTERS_UP,
		SPK_OUSTERS_RIGHT,
		SPK_OUSTERS_DOWN,
		SPK_OUSTERS_LEFT,
		SPK_MINEGAME_NORMAL,
		SPK_MINEGAME_NORMAL_PUSHED,
		SPK_MINEGAME_YELLOW,
		SPK_MINEGAME_YELLOW_PUSHED,
		SPK_MINEGAME_GREEN,
		SPK_MINEGAME_GREEN_PUSHED,
		SPK_FLAG,
		SPK_QUESTION_MARK,
		SPK_ARROW_BACK_1,
		SPK_ARROW_BACK_2,
		SPK_OUT_BOX_BOTTOM,

		SPK_OUT_BOX_LEFT_UP,
		SPK_OUT_BOX_RIGHT,
		SPK_OUT_BOX_DOWN,

		SPK_IN_BOX_LEFT_UP,
		SPK_IN_BOX_RIGHT,
		SPK_IN_BOX_DOWN,
		SPK_BACK,
		SPK_IN_BOX_BOTTOM,

		SPK_DEAD_BODY_UP,
		SPK_DEAD_BODY_RIGHT,
		SPK_DEAD_BODY_DOWN,
		SPK_DEAD_BODY_LEFT,
	};


	GAME_STAGE					m_Stage;
	ButtonGroup					*m_pC_button_group;
	std::list<S_CHARACTER*>		m_MonsterList;
	std::vector<S_TILE>			m_Tile;
	S_CHARACTER					m_Player;
	int							m_RemainCountMoveMonster;
	DWORD						m_TimeMoveMonster;
	C_SPRITE_PACK				m_SPK;
	int							m_GameCode;
	bool						m_bCanStart;
	std::string					m_topName;
	WORD						m_topScore;
	WORD						m_MyBestScore;
	
public:
	C_VS_UI_ARROW_TILE();
	~C_VS_UI_ARROW_TILE();
	
	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	IsPixel(int _x, int _y);	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	

	void	SetScore(std::string& name, WORD& score, WORD& myscore){m_bCanStart = true;	m_topName = name; m_topScore = score; m_MyBestScore = myscore;	}
	//-----------------------------------------------------------------------
	// Init
	//-----------------------------------------------------------------------
	void	InitGame();
	void	InitMonster();
	void	ResetGame();
	void	InitStage();

	BYTE	GetDirection(S_CHARACTER *Character);
	void	AddMonster();	
	BYTE	GetMapSize();
	bool	IsExistMonster(short id, int x, int y );
	void	SetDie();

	//-----------------------------------------------------------------------
	// Move
	//-----------------------------------------------------------------------
	void	MoveMonster();
	bool	MoveRIGHT(S_CHARACTER *Character);
	bool	MoveLEFT(S_CHARACTER *Character);
	bool	MoveUP(S_CHARACTER *Character);
	bool	MoveDOWN(S_CHARACTER *Character);
	
	//-----------------------------------------------------------------------
	// Action
	//-----------------------------------------------------------------------
	void	ActionMove();
	void	ActionRotation(BYTE Direction);					// 0은 왼쪽 1은 오른쪽

	//-----------------------------------------------------------------------
	// Process
	//-----------------------------------------------------------------------	
	void	ProcessGameMain();
	void	ProcessTile();
	void	ProcessTimeExpired();
	void	ProcessRotation();
	void	ProcessCharacter();

	//-----------------------------------------------------------------------
	// Timer
	//-----------------------------------------------------------------------	
	bool	TimerMonsterMove(DWORD len);
};





//-----------------------------------------------------------------------------
// class C_VS_UI_CRAZY_MINE
//
//-----------------------------------------------------------------------------
class C_VS_UI_CRAZY_MINE : public Window, public Exec, public ButtonVisual
{
public:
private:
	
	class MineCell
	{
	public:
		MineCell() : isMine(false), isFlag(false), isSelected(false), isOpen(false), isQuestion(false) { }
		bool isMine;
		bool isFlag;
		bool isSelected;
		bool isOpen;
		bool isQuestion;
	};

	class SelectHistory
	{
	public:
		std::list<std::pair<int,int> > m_Selected;
		int m_X, m_O;
	};
	enum EXEC_ID
	{
		CLOSE_ID,
		HELP_ID
	};

	enum SPK
	{
		SPK_ARROW_UP,
		SPK_ARROW_UP_RIGHT_1,
		SPK_ARROW_UP_RIGHT_2,
		SPK_ARROW_UP_RIGHT_3,
		SPK_ARROW_RIGHT,
		SPK_ARROW_RIGHT_DOWN_1,
		SPK_ARROW_RIGHT_DOWN_2,
		SPK_ARROW_RIGHT_DOWN_3,
		SPK_ARROW_DOWN,
		SPK_ARROW_DOWN_LEFT_1,
		SPK_ARROW_DOWN_LEFT_2,
		SPK_ARROW_DOWN_LEFT_3,
		SPK_ARROW_LEFT,
		SPK_ARROW_LEFT_UP_1,
		SPK_ARROW_LEFT_UP_2,
		SPK_ARROW_LEFT_UP_3,
		
		
		SPK_ROTATION_RIGHT,
		SPK_ROTATION_RIGHT_HILIGHTED,
		SPK_ROTATION_RIGHT_PUSHED,
		SPK_ROTATION_LEFT,
		SPK_ROTATION_LEFT_HILIGHTED,
		SPK_ROTATION_LEFT_PUSHED,
		SPK_MOVE,
		SPK_MOVE_HILIGHTED,
		SPK_MOVE_PUSHED,
		SPK_GOAL,
		SPK_START,
		
		SPK_CLOSE,
		SPK_CLOSE_HILIGHTED,
		SPK_CLOSE_PUSHED,
		SPK_HELP,
		SPK_HELP_HILIGHTED,
		SPK_HELP_PUSHED,
		
		SPK_SLAYER_MALE_UP,
		SPK_SLAYER_MALE_RIGHT,
		SPK_SLAYER_MALE_DOWN,
		SPK_SLAYER_MALE_LEFT,
		SPK_SLAYER_FEMALE_UP,
		SPK_SLAYER_FEMALE_RIGHT,
		SPK_SLAYER_FEMALE_DOWN,
		SPK_SLAYER_FEMALE_LEFT,
		SPK_VAMPIRE_MALE_UP,
		SPK_VAMPIRE_MALE_RIGHT,
		SPK_VAMPIRE_MALE_DOWN,
		SPK_VAMPIRE_MALE_LEFT,
		SPK_VAMPIRE_FEMALE_UP,
		SPK_VAMPIRE_FEMALE_RIGHT,
		SPK_VAMPIRE_FEMALE_DOWN,
		SPK_VAMPIRE_FEMALE_LEFT,
		SPK_OUSTERS_UP,
		SPK_OUSTERS_RIGHT,
		SPK_OUSTERS_DOWN,
		SPK_OUSTERS_LEFT,
		SPK_MINEGAME_NORMAL,
		SPK_MINEGAME_NORMAL_PUSHED,
		SPK_MINEGAME_YELLOW,
		SPK_MINEGAME_YELLOW_PUSHED,
		SPK_MINEGAME_GREEN,
		SPK_MINEGAME_GREEN_PUSHED,
		SPK_FLAG,
		SPK_QUESTION_MARK,
		SPK_ARROW_BACK_1,
		SPK_ARROW_BACK_2,
		SPK_OUT_BOX_BOTTOM,

		SPK_OUT_BOX_LEFT_UP,
		SPK_OUT_BOX_RIGHT,
		SPK_OUT_BOX_DOWN,

		SPK_IN_BOX_LEFT_UP,
		SPK_IN_BOX_RIGHT,
		SPK_IN_BOX_DOWN,
		SPK_BACK,
		SPK_IN_BOX_BOTTOM,

	};

	MineCell	m_MineBoard[20][20];
	int			m_MineBoardSize;
	int			m_MineCount;

	int			m_SelectedCount;
	int			m_FlagCount;

	int			m_O, m_X;
	std::list<SelectHistory>	m_History;
	int			m_SelectedHistory;

	bool		m_bSuccess;
	bool		m_bDead;
	bool		m_bStart;
	bool		m_bCanStart;
	DWORD		m_StartTime;
	C_SPRITE_PACK	m_SPK;
	std::string		m_topName;
	WORD			m_topScore;
	WORD						m_MyBestScore;
	
	int			m_OIdMouseX, m_OldMouseY, m_LatestClickTime;
	
	
	ButtonGroup					*m_pC_button_group;	
	
public:
	C_VS_UI_CRAZY_MINE();
	~C_VS_UI_CRAZY_MINE();
	
	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	IsPixel(int _x, int _y);	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	SetScore(std::string& name, WORD& score, WORD& myscore){m_bCanStart = true;		m_topName = name; m_topScore = score; m_MyBestScore = myscore;	}
	

	void ActionDoubleClick(int x, int y);
	void CheckSuccess();
	SelectHistory MakeHistory();
	void ClearSelected();
	void updateMembers();
	void ActionRClick(int x, int y);
	void InitMineBoard(int size, int mine = -1);
	void ActionNewGame(int n);
	void ActionClick(int x,int y);
	void DrawBoard();
};

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_INVENTORY : public Window, public Exec, public ButtonVisual
{
private:
	MItem*					m_pCodeSheet;
	C_SPRITE_PACK			m_SPK;
	ButtonGroup				*m_pC_button_group;
	std::vector<char>		m_Inventory;
	POINT					m_FixPos;
	int						m_CurMousePos;
	int						m_ItemFrameMap[5];
	

	enum EXEC_ID
	{
		OK_ID,
		CLOSE_ID,
		HELP_ID,
	};

	enum INVENTORY_SPK_INDEX
	{
		INVENTORY_WINDOW = 0,
		INVENTORY_WINDOW_ALPHA,
		INVENTORY_ITEMBACK,
		INVENTORY_ITEMBACK_ALPHA,
		INVENTORY_WINDOW_BOTTOM,
	};


public :	
	C_VS_UI_QUEST_INVENTORY(const MItem* pItem);
	~C_VS_UI_QUEST_INVENTORY();

	void	SetInventory(int i, BYTE Option);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();		
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();		
	}

	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);

	void	DropToQuestInventory( const MItem* pItem);
	bool	CanDropToInventory(const MItem* pItem);
};

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_STATUS_CTF : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			m_pC_spk;
	ButtonGroup				*m_pC_button_group;

	DWORD					m_finish_time;
	int						m_num_flag[3];
	
	enum EXEC_ID
	{
		OK_ID,
		CLOSE_ID,
		HELP_ID,
		PUSHPIN_ID,
	};

	enum TRACE_SPK
	{
		MAIN_WINDOW = 0,
		HELP_BUTTON_PUSHED,
		HELP_BUTTON_HILIGHTED,
		HELP_BUTTON,		
	};

public :

	C_VS_UI_STATUS_CTF();
	~C_VS_UI_STATUS_CTF();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();		
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();		
	}

	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);

	void	SetStatus(DWORD &endtime, int &flag_s, int &flag_v, int &flag_o);
};

//-----------------------------------------------------------------------------
// class C_VS_UI_REQUEST_DIE
//
//-----------------------------------------------------------------------------
class C_VS_UI_REGEN_TOWER_MINIMAP : public Window
{
private:
	enum IMAGE_SPK_INDEX
	{
		MINIMAP,
		OUSTERS_EYE,
		VAMPIRE_EYE,
		SLAYER_EYE,
		GRAY_EYE
	};

	enum BUTTON_ID
	{
		CLOSE_ID,
		REGEN_1,
		REGEN_2,
		REGEN_3,
		REGEN_4,
		REGEN_5,
		REGEN_6,
		REGEN_7,
		REGEN_8,

		REGEN_OCTAVUS,
		REGEN_QUARTUS,
		REGEN_TERTIUS,
		REGEN_SEPTIMUS,
	};
	
	C_SPRITE_PACK				m_image_spk;

	bool						m_bl_pushed;
	bool						m_bl_focused;
	int							m_selected;

// TIMER
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_timer_tickcount;

public:
	C_VS_UI_REGEN_TOWER_MINIMAP(DWORD timer);
	~C_VS_UI_REGEN_TOWER_MINIMAP();

	bool	Timer(bool reset = false);
	
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	
	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	MouseControl(UINT message, int _x, int _y);

	void	CancelPushState() { m_bl_pushed = false; }
	void	UnacquireMouseFocus() { m_bl_focused = false; }
};

//-----------------------------------------------------------------------------
// class C_VS_UI_MAILBOX
//
//-----------------------------------------------------------------------------
class C_VS_UI_NPC_DIALOG;
class C_VS_UI_MAILBOX : public Window, public Exec, public ButtonVisual
{
private:
	
	ButtonGroup			*m_pC_button_group;
	C_VS_UI_SCROLL_BAR	*m_pC_scroll_bar;
	
	C_SPRITE_PACK		m_SPK;

	class C_VS_UI_MAIL
	{
	public:
		DWORD		id;
		bool		mark;
		MString		sender;
		MString		title;
		MString		contents;
		MString		date;
		SIZE		windowSize;
		bool		show;
		bool		open;
		int			guild_id;
		int			guild_type;

	public:
		C_VS_UI_MAIL();
		~C_VS_UI_MAIL();

		void SaveToFile(class ofstream &file);
		bool LoadFromFile(class ifstream &file);
		void SaveToFile(std::string filename);
		void LoadFromFile(std::string filename);
	};
	
	enum EXEC_ID
	{
		CLOSE_ID,
		MAILTAB_ID,
		HELPTAB_ID,
		MEMOTAB_ID,
		UP_ID,
		DOWN_ID,

		EXECID_MAX,
	};
	
	enum SPRITE_ID
	{
		MAIL_CLOSED,
		MAIL_OPENED,
		HAS_MAIL,
		HAS_MAIL2,
		HAS_HELP,
		HAS_HELP2,
		HAS_MEMO,
		HAS_MEMO2,
		TAB_MAIL_OUSTERS,
		TAB_HELP_OUSTERS,
		TAB_MEMO_OUSTERS,
		TAB_MAIL_SLAYER,
		TAB_HELP_SLAYER,
		TAB_MEMO_SLAYER,
		TAB_MAIL_VAMPIRE,
		TAB_HELP_VAMPIRE,
		TAB_MEMO_VAMPIRE,
		TITLE_OUSTERS,
		TITLE_SLAYER,
		TITLE_VAMPIRE,
		CONTENTS_OUSTERS,
		DETAIL_OUSTERS,
		CONTENTS_VAMPIRE,
		DETAIL_VAMPIRE,
		CONTENTS_SLAYER,
		DETAIL_SLAYER,

		SPRITEID_MAX,
	};

public:
	enum TAB_ID
	{
		TAB_MAIL_ID,
		TAB_HELP_ID,
		TAB_MEMO_ID,

		TABID_MAX,
		
		NEW_MAIL_ID,
		NEW_HELP_ID,
		NEW_MEMO_ID,
		
	};
	
private:
	TAB_ID	m_currentTab;
	POINT	m_ptTab, m_ptNewMail;
	Rect	m_rtContents;
	int		m_focusContents;
	C_VS_UI_NPC_DIALOG		*m_pC_dialog ;

	typedef	std::vector<C_VS_UI_MAIL>	MAILVECTOR_TYPE;
	MAILVECTOR_TYPE	m_mail[3];
	int		m_listCount;
	int		m_overcnt;
	static int		m_cur_id;
	static int		m_guild_id;
	
	
public:
	C_VS_UI_MAILBOX();
	~C_VS_UI_MAILBOX();
	
	//void	AddMail(TAB_ID tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open);
	void	AddMail(TAB_ID tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open, int guildid=0,int guildtype = 0);
	void	AddMailContents(TAB_ID tab_id, DWORD id, const char * contents);
	void	DeleteMail(TAB_ID tab_id, DWORD id);
	// Help Message 
	void	AddHelpMail(DWORD id,  bool open);

	void	Show();
	void	Start(TAB_ID tab_id = TAB_MAIL_ID);
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	IsPixel(int _x, int _y);	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);

	void	ReadMail(TAB_ID tabID, DWORD id);
	bool	IsNewMail() { if(m_currentTab > TABID_MAX)return true; return false; };
	
	void	SetCurId(int id) { m_cur_id = id;}
	static int 	GetCurId()   { return m_cur_id;}

	void	SetGuildId(int id) { m_guild_id = id;}
	static int 	GetGuildId()   { return m_guild_id;}


	void	UnacquireMouseFocus()
	{
		//		m_pC_button_group->UnacquireMouseFocus();
		m_focusContents = -1;
		m_pC_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		//		m_pC_button_group->CancelPushState();
		m_pC_button_group->CancelPushState();
	}

	void SaveToFile(class ofstream &file);
	void LoadFromFile(class ifstream &file);
	void SaveToFile(std::string filename);
	void LoadFromFile(std::string filename);
	void CloseMailBoxDialogBox();
	
};

struct PETINFO
{
	bool EXIST;
	int HP;
	int ATTR;
	int ATTR_VALUE;
	int LEVEL;
	int FOODTYPE;
	int EXP_REMAIN;
	bool CUT_HEAD;
	bool GAMBLE;
	int OPTION;
	int ITEM_TYPE;
	TYPE_OBJECTID ITEM_ID;
	std::string NAME;
	bool ATTACK;
	std::string NICK_NAME;
	bool ChangeAbleNickName; // 내펫인지 아닌지..
};

//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_PET_INFO : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			m_pC_spk;
	C_SPRITE_PACK			m_pC_pet_face;
	ButtonGroup				*m_pC_button_group;
	PETINFO					m_PetInfo;
	POINT					m_MainWindowPosition;

	enum EXEC_ID
	{
		CLOSE_ID,
		GAMBLE_ID,
		NICKNAME_ID,
	};

	enum INFO_SPK
	{
		MAIN_WINDOW = 0,
		HP_DAY_BACK,
		HP_TIME_BACK,
		HP_DAY_BAR,
		HP_TIME_BAR,
		GAMBLE_BUTTON,
		GAMBLE_BUTTON_HILIGHTED,
		GAMBLE_BUTTON_PUSHED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		CLOSE_BUTTON_PUSHED,
		NICKNAME_BUTTON,
		NICKNAME_BUTTON_HILIGHTED,
		NICKNAME_BUTTON_PUSHED,
	};

	LineEditorVisual	m_AddName;
	bool				m_IsWolverine;
public :

	C_VS_UI_PET_INFO(PETINFO *petInfo);
	~C_VS_UI_PET_INFO();

	PETINFO*	GetPetInfo() { return &m_PetInfo; }
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();		
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();		
	}

	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);
};


//-----------------------------------------------------------------------------
// C_VS_UI_HELPDESC
//
// Desc : 헬프 메세지 클래스 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// C_VS_UI_HELPDESC
//
// Desc : 헬프 메세지 클래스 
//-----------------------------------------------------------------------------

class C_VS_UI_HELPDESC: public Window, public Exec , public ButtonVisual 
{
private:
	ButtonGroup			*m_pC_button_group; 
	C_VS_UI_SCROLL_BAR	*m_pC_scroll_bar;
	C_SPRITE_PACK		m_SPK;

	enum IMAGE_SPK_INDEX
	{
		HELPIMAGE,
		HELPIMAGE1,
		HELPIMAGE2,
		HELPIMAGE3
	};

	enum EXEC_ID
	{
		CLOSE_ID
	};

	DWORD m_helpindex;
	std::string m_detail;
	std::string m_title;

public:
	C_VS_UI_HELPDESC(const char *detail,const char *title, DWORD id);
	~C_VS_UI_HELPDESC();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	bool	IsPixel(int _x, int _y);	

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();		
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();		
	}
	void	HelpDescPasing();
	void    LoadCustomstr(char * customstrfilename);
	void	DrawImg(int m_width, int m_height ,const char * filename,const char * pos, int linecnt);
	char*   findkey(const char* tagstr ,char* keyword);
//
	std::vector<std::string>	parsing_data;	
	std::vector<std::string>	custom_strting;
	std::vector<std::string>	jpg_file;
	CDirectDrawSurface*		GetJpgFileLoading(std::string filename) { return  m_jpgData[filename];}
	void	LoadHelpJpg(std::string filename);
	void    ClearHelpJpg();
protected:
	typedef std::map<std::string , CDirectDrawSurface*> JPGIMG_MAP;
	JPGIMG_MAP m_jpgData;
};

class C_VS_UI_SMS_MESSAGE : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_SMS_spk;
	enum SMS_SPK
	{
		MAIN_WINDOW,

		SMS_ITEM_ICON,

		COMBO_LIST,
		COMBO_BUTTON_HILIGHT,
		COMBO_BUTTON_PUSH,

		PHONE_LIST_BUTTON_HILIGHT,
		PHONE_LIST_BUTTON_PUSH,

		SEND_BUTTON_HILIGHT,
		SEND_BUTTON_PUSH,

		SMILE_BUTTON_HILIGHT,
		SMILE_BUTTON_PUSH,

		EXIT_BUTTON_HILIGHT,
		EXIT_BUTTON_PUSH,

		COMBO_SELECTER,
	};

	enum EXEC_ID
	{
		SMS_SEND_ID,
		SMS_PHONE_LIST_ID,
		SMS_SMILE_ID,
		SMS_CLOSE_ID,
		SMS_COMBO_OTHER_1,
		SMS_COMBO_OTHER_2,
		SMS_COMBO_OTHER_3,
		SMS_COMBO_OTHER_4,
		SMS_COMBO_OTHER_5,
		SMS_COMBO_MINE,

		SMS_CHAT_MESSAGE_ID,
		SMS_CHAT_MINE_ID,
		SMS_CHAT_OTHER1_ID,
		SMS_CHAT_OTHER2_ID,
		SMS_CHAT_OTHER3_ID,
		SMS_CHAT_OTHER4_ID,
		SMS_CHAT_OTHER5_ID,
		EXEC_MAX,
	};
	enum
	{
		MARK_X = 12,
		MARK_Y = 17,
	};

private:
	SIZE	m_Main,m_ComboListPos,m_IconPos,m_ChargePos;
	SIZE	m_ComboPos[6], szSMSMessagePos;
	int		m_bComboList;
	BYTE	m_bComboSelect[6];
	char	m_szPhoneNum[6][8];
	LineEditorVisual	m_lev_MyNum, m_lev_OtherNum[5],m_lev_SMSMessage;
	DWORD	m_Color;
	bool	m_SubWindow;
	Rect	m_sub_rect;
	POINT	m_sub_selected;
	C_VS_UI_SCROLL_BAR		*m_pC_sub_scroll_bar;

	std::string			m_szMyNum, m_szSMSMessage;
	std::list<std::string>	m_szOtherNum;
public:
	C_VS_UI_SMS_MESSAGE(); // team_name은 member인 경우에만
	~C_VS_UI_SMS_MESSAGE();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	AcquireSMS() {m_lev_SMSMessage.Acquire();}
	void	Start();
	void	Finish();
	void	AddSendList(char* str);


	void	Show();

};

class C_VS_UI_SMS_LIST : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_SMS_spk;
	enum SMS_SPK
	{
		PHONE_LIST_WINDOW,

		EXIT_BUTTON_HILIGHT,
		EXIT_BUTTON_PUSH,

		ADD_SEND_LIST_BUTTON_HILIGHT,
		ADD_SEND_LIST_BUTTON_PUSH,

		DELETE_PHONE_LIST_BUTTON_HILIGHT,
		DELETE_PHONE_LIST_BUTTON_PUSH,

		NEW_PHONE_LIST_BUTTON_HILIGHT,
		NEW_PHONE_LIST_BUTTON_PUSH,

		SELECT_PHONE_LIST_1,
		SELECT_PHONE_LIST_2,
		SELECT_PHONE_LIST_3,
		SELECT_PHONE_LIST_4,

		BUTTON_NAME_PUSH,
		BUTTON_ID_PUSH,
		BUTTON_NUM_PUSH,

	};

	enum EXEC_ID
	{
		
		SMS_ADD_SEND_ID,
		SMS_DELETE_LIST_ID,
		SMS_NEW_PHONE_ID,
		SMS_CLOSE_PHONE_LIST_ID,

		SMS_SORT_NAME_ID,
		SMS_SORT_CHAR_ID,
		SMS_SORT_NUMBER_ID,

		EXEC_MAX,
	};
	enum
	{
		SORT_BY_NAME = 0,
		SORT_BY_ID,
		SORT_BY_NUM,
	};

public:
	struct AddressUnit
	{
		DWORD	ElementID;
		std::string	CharacterName;
		std::string	CustomName;
		std::string	Number;
	};
	C_VS_UI_SMS_LIST(); // team_name은 member인 경우에만
	~C_VS_UI_SMS_LIST();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	AddList(DWORD element, char* name, char* id, char* num) ;
	void	DeleteList(int id);
	void	Start();
	void	Finish();

	void	Show();

	void	SetSMSList(void *pVoid);
	void	Address_Sort(int nSort);
	
private:
	SIZE	m_Main;
	C_VS_UI_SCROLL_BAR	*m_pC_scroll_bar;
	int		m_SelectPos;
	

	std::vector<AddressUnit*>	m_Addresses;

};

class C_VS_UI_SMS_RECORD : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_SMS_spk;
	enum SMS_SPK
	{
		PHONE_ADD_WINDOW,

		COMBO_LIST,
		COMBO_BUTTON_HILIGHT,
		COMBO_BUTTON_PUSH,

		EXIT_BUTTON_HILIGHT,
		EXIT_BUTTON_PUSH,

		RECORD_PHONE_LIST_BUTTON_HILIGHT,
		RECORD_PHONE_LIST_BUTTON_PUSH,

		COMBO_SELECTER,
	};

	enum EXEC_ID
	{
		SMS_RECORD_PHONE_ID,
		SMS_EXIT_ID,
		SMS_COMBO_NEW_5,

		SMS_CHAT_NAME_ID,
		SMS_CHAT_ID_ID,
		SMS_CHAT_NUM_ID,

		EXEC_MAX,
	};

private:
	SIZE	m_Main,m_ComboListPos,m_ComboPos;
	char	m_szPhoneNum[6][8];
	BYTE	m_bComboSelect;
	int		m_bComboList;
	LineEditorVisual	m_lev_AddNum,m_lev_AddName, m_lev_AddID; 

	std::string			m_szNum, m_szName, m_szID;
public:
	C_VS_UI_SMS_RECORD(); // team_name은 member인 경우에만
	~C_VS_UI_SMS_RECORD();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};

class C_VS_UI_NicknameInfo
{
public:
	enum
	{
		NICK_NONE = 0,		// 닉네임 없음
		NICK_BUILT_IN,		// 일반적으로 자동으로 주어지는 닉네임 (인덱스)
		NICK_QUEST,			// 퀘스트 클리어한 뒤 받는 닉네임 (인덱스)
		NICK_FORCED,		// 강제로 붙여진 닉네임 (인덱스)
		NICK_CUSTOM_FORCED,	// 강제로 붙여진 닉네임 (스트링)
		NICK_CUSTOM,		// 유저가 자유롭게 넣은 닉네임 (스트링)
	};

	WORD	getNicknameID() const { return m_NicknameID; }
	BYTE	getNicknameType() const { return m_NicknameType; }
	std::string	getNickname() const { return m_Nickname; }
	WORD	getNicknameIndex() const { return m_NicknameIndex; }

	void	setNicknameID( WORD id ) { m_NicknameID = id; }
	void	setNicknameType( BYTE type ) { m_NicknameType = type; }
	void	setNickname( const std::string& name ) { m_Nickname = name; }
	void	setNicknameIndex( WORD index ) { m_NicknameIndex = index; }
	void	setNickname( char* name ) { m_Nickname = name; }

private:
	WORD	m_NicknameID;
	BYTE	m_NicknameType;
	std::string	m_Nickname;
	WORD	m_NicknameIndex;	// 스트링 또는 인덱스 하나만 사용된다.
};
class C_VS_UI_NAMING : public Window, public Exec, public ButtonVisual
{

public :
	
private :

	ButtonGroup			*m_pC_button_group;
	C_SPRITE_PACK		m_pC_spk;

	enum INFO_SPK
	{
		NICKNAME_STR = 0,	// 닉네임
		NICKNAME_LIST_STR,	// 닉네임 리스트
		PETNAME_STR,		// 펫이름
		ADD_NICKNAME_STR,	// 추가 닉네임
	};
	enum EXEC_ID
	{
		CLOSE_ID,
//		ADD_OK_ID,
		SELECT_ID,
		PUSHPIN_ID,
		ALPHA_ID,
		HELP_ID,
	};

	C_VS_UI_SCROLL_BAR	*m_pC_scroll_bar;
//	LineEditorVisual	m_AddName;
	std::vector<C_VS_UI_NicknameInfo*>	m_NameList;
//	std::string			m_szCustomName;
//	int					m_CustomIndex;
	int					m_SelectPos;
	bool				m_bl_focus;
public :
	C_VS_UI_NAMING(std::vector<C_VS_UI_NicknameInfo*> &_NamingList);
	~C_VS_UI_NAMING();

	
	
	void	ToggleWindow();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	bool	IsPixel(int _x,int _y);
	
	void	ShowDesc(int strX,int strY,const char *str);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);

	void	SetNameList(std::vector<C_VS_UI_NicknameInfo*> &_NamingList);
	void	AddNameList(C_VS_UI_NicknameInfo* Info) ;
	void	DeleteNameList(int id);
	void	Change_Custom_Naming(int nID, char* szName);
	
};

class C_VS_UI_NAMING_CHANGE : public Window, public Exec, public ButtonVisual
{
public :

private :

	ButtonGroup			*m_pC_button_group;
	C_SPRITE_PACK		m_pC_spk;
	enum INFO_SPK
	{
		NICKNAME_STR = 0,	// 닉네임
		NICKNAME_LIST_STR,	// 닉네임 리스트
		PETNAME_STR,		// 펫이름
		ADD_NICKNAME_STR,	// 추가 닉네임
	};
	enum EXEC_ID
	{
		CHANGE_ID,
		CLOSE_ID,
		HELP_ID,
	};

	LineEditorVisual	m_ChangeNick;
	MItem*				m_PenItem;
	std::string			m_szCustomString;
	std::string			m_szEditName;
public :
	C_VS_UI_NAMING_CHANGE(MItem* pItem, char* szCustom);
	~C_VS_UI_NAMING_CHANGE();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	bool	IsPixel(int _x,int _y);
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);

};

//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_MANAGER
//
// 퀘스트 ui 관련 이미지와 퀘스트 정보를 처리 하자..
//-----------------------------------------------------------------------------
class	C_VS_UI_QUEST_LIST;
class	C_VS_UI_QUEST_DETAIL;
class	C_VS_UI_QUEST_MISSION;
class	C_VS_UI_QUEST_ITEM;
class	C_VS_UI_QUEST_ICON;

/* 
퀘스트 관련 npc창을 띄운다..
*/
class C_VS_UI_QUEST_NPC_DIALOG 
{
private :
	C_VS_UI_NPC_DIALOG			*m_pC_dialog;
public :
	C_VS_UI_QUEST_NPC_DIALOG();
	~C_VS_UI_QUEST_NPC_DIALOG();

	void SetQuestNpcDialog(C_VS_UI_NPC_DIALOG* TempDialog);
};

class C_VS_UI_QUEST_MANAGER 
{
public:
	struct _GMissionInfo{
		std::string		szMissionTitle;
		std::string		m_StrArg;
		DWORD			m_NumArg;
		BYTE			bStatus;
		BYTE			bCondition;	// 어느 조건에 있는가 0 : Happen, 1 : Complete, 2 : Fail, 3 : Reward
		WORD			bIndex;		// 해당 조건의 몇번째 element인가
		DWORD			dwTimeLimit;// 시간 제한 퀘스트일 경우 처음 남음 시간 세팅 
	};
	struct _GQuestInfo{
		DWORD			dwQuestID;
		BYTE			bStatus;
		std::string		szQuestTitle;
		std::string		szQuestDescription;
		std::vector<_GMissionInfo*> vMissionList;
	};

	struct _GQuestExcuteElement{
		DWORD qID;
		BYTE bCondition;
		WORD wIndex;
	};

private:

	C_VS_UI_QUEST_LIST			*m_Ui_Quest_List;
	C_VS_UI_QUEST_DETAIL		*m_Ui_Quest_Detail;
	C_VS_UI_QUEST_MISSION		*m_Ui_Quest_Mission;
	C_VS_UI_QUEST_ITEM			*m_Ui_Quest_Item;

	C_VS_UI_QUEST_ICON			*m_Ui_Quest_Icon;

	CRarFile					m_Quest_XML_file;
	XMLTree						m_Quest_XML_Tree;

	DWORD						m_SelectedQuestID;
	std::vector<_GQuestInfo*>	m_QuestListInfo; // 퀘스트 정보 리스트
	std::vector<MItem*>			m_QuestItemInfo; // 퀘스트 아이템 리스트
	std::vector<_GQuestExcuteElement*>	m_QuestExcutElement; // 퀘스트 ExcuteElement 리스트
	
	bool						UpdateQuestInfo(_GQuestInfo *QInfo, int nType = -1);
	char*						GetQuestTitle(const XMLTree *pElement);
	char*						GetQuestDescription(const XMLTree *pElement);
	char*						GetMissionTitle(const XMLTree *pElement, WORD Index, BYTE Condition);
	const XMLTree *				GetMissionElement(const XMLTree *pElement, BYTE Condition);
public:
	enum INFO_SPK
	{
		INVENTORY_SLOT,			// 인벤토리 슬롯 뒷배경
		TAB_BUTTON_PROCESS,		// 탭버튼 - 진행
		TAB_BUTTON_COMPLETE,	// 탭버튼 - 완료
		BUTTON_ACCEPT,			// 수락 버튼
		BUTTON_ACCEPT_HILIGHT,	// 수락 버튼 하이라이트
		BUTTON_ACCEPT_PUSH,		// 수락 버튼 푸쉬
		BUTTON_GIVEUP,			// 포기 버튼
		BUTTON_GIVEUP_HILIGHT,	// 포기 버튼 하이라이트
		BUTTON_GIVEUP_PUSH,		// 포기 버튼 푸쉬
		QUEST_LIST_SUBJECT,		// 제목,상태
		QUEST_MISSION_SUBJECT,	// 임무,상태
		QUEST_TITLE_BG,			// 퀘스트 목록 뒷 배경
		QUEST_TABLE_BG,			// 퀘스트 목록 뒷 배경
		QUEST_DETAIL_BG,		// 퀘스트 상세 뒷 배경

		QUEST_LIST_TITLE,		// 퀘스트 목록 타이틀
		QUEST_DETAIL_TITLE,		// 퀘스트 세부 타이틀
		QUEST_MISSION_TITLE,	// 퀘스트 임무 타이틀
		QUEST_ITEM_TITLE,		// 퀘스트 아이템 타이틀

	};
	
	C_VS_UI_QUEST_MANAGER();
	~C_VS_UI_QUEST_MANAGER();

	C_SPRITE_PACK*				m_pImage_Spk;
	C_SPRITE_PACK*				m_pImage_Spk_MailBox;
	
	bool	m_Show_Quest_Modify;

	bool	SetQuestManagerInfo(void* pVoid); // AddForce
	bool	ModifyQuestManagerInfo(void* pVoid, int nType = 0);

	bool	LoadQuestXML();
	void	ReleaseQuestXML();
	
	void	SetSubWindowInfo(DWORD qID, bool bOpenSubWindows = true);
	void	SendQuestIntention(bool bIntention);
	void	UpdateQuestItemInfo(std::vector<MItem*>& vItem);

	void	PushGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex);
	void	RunGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex);
	void	RunNextGQuestExcuteElement();
	void	RunGQuestExcuteElementAction(DWORD qID, BYTE bCondition, WORD wIndex);

	void	RunQuestFail_or_Sucess(const XMLTree *pElement, int nType);
	
	
	void	ClearQuestInfo();
	void	ClearQuestItem();
	void	ClearExcuteElement();
	
	bool	IsAnyWindowOpened() const;
	void	ToggleWindow();
	void	RunAllWinow();
	void	CloseAllWinow();

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

	void	RunQuestIcon();
	bool	IsRunningQuestIcon() const;
	void	CloseQuestIcon(int JustClose = 0);

	void	DeleteQuestItem(int nSlot);

};


//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_LIST
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_LIST : public Window, public Exec, public ButtonVisual/*, public C_VS_UI_QUEST_MANAGER*/
{
public:
	enum QUEST_LIST_STATUS
	{
		CANNOT = 0,		// 아직 불가능
		CAN_ACCEPT,		// 수행가능
		DOING,			// 수행중
		SUCCESS,		// 성공 (아직 보상받지 않음)
		COMPLETE,		// 완료 (보상받음, 재수행 불가능)
		FAIL,			// 실패 (재수행 불가능)
		CAN_REPLAY,		// 재수행 가능
	};

private:
	enum EXEC_ID
	{
		CLOSE_ID,
		PUSHPIN_ID,
		ALPHA_ID,
		TAB1_ID,
		TAB2_ID,
	};

	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK*				m_pImage_Spk;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;	
	int							m_SelectPos;
	int							m_SelectContents;
	BYTE						m_TabID; // 목록/완료 탭 선택
	bool						m_bl_focus;

	int							m_OustersOffset;// 아우스터즈 픽셀 보정

	std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>		m_QuestListInfo[2];		// 0 : 수행가능, 수행중,성공 일때 퀘스트 리스트
															// 1 : 완료, 실패, 재수행 가능 일때 퀘스트 리스트

public:

	C_VS_UI_QUEST_LIST(C_SPRITE_PACK* spr);
	~C_VS_UI_QUEST_LIST();

	void	KeyboardControl(UINT message, UINT key, long extra)	;
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);
	void	Process();
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);

	void	SetQuestListInfo(void* pVoid);
};

//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_DETAIL
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_DETAIL : public C_VS_UI_DESC, public Window, public Exec, public ButtonVisual/*, public C_VS_UI_QUEST_MANAGER*/
{
private:
	enum EXEC_ID
	{
		CLOSE_ID,
		PUSHPIN_ID,
		ALPHA_ID,
//		TAB1_ID,
//		TAB2_ID,
		ACCEPT_ID,
	};
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK*				m_pImage_Spk;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;	
	int							m_SelectPos;
	bool						m_bl_focus;
	int							m_OustersOffset;// 아우스터즈 픽셀 보정

	int							m_IsAbleRunQuest ; // 0 : disable 1 : 수행 2 : 포기
	std::string					m_szTitle;
public:
	C_VS_UI_QUEST_DETAIL(C_SPRITE_PACK*	spr);
	~C_VS_UI_QUEST_DETAIL();

	void	KeyboardControl(UINT message, UINT key, long extra)	;
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);
	void	Process();
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);

	void	SetQuestDetailInfo(char* szTitle, char* szDescription, int Status);

};

//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_MISSION
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_MISSION :  public Window, public Exec, public ButtonVisual/*, public C_VS_UI_QUEST_MANAGER*/
{
public:
	enum QUEST_MISSION_STATUS
	{
		HIDE = 0,			// 보여주지 않음
		CURRENT,			// 지금 해야 되는 미션 (수행중)
		SUCCESS,			// 이미 성공한 미션
		FAIL,				// 실패
	};

private:
	enum EXEC_ID
	{
		CLOSE_ID,
		PUSHPIN_ID,
		ALPHA_ID,
		TAB1_ID,
		TAB2_ID,
	};
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK*				m_pImage_Spk;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;	
	int							m_SelectPos;
	bool						m_bl_focus;
	int							m_OustersOffset;// 아우스터즈 픽셀 보정

	std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*> m_QuestMissionInfo;
public:
	C_VS_UI_QUEST_MISSION(C_SPRITE_PACK* spr);
	~C_VS_UI_QUEST_MISSION();

	void	KeyboardControl(UINT message, UINT key, long extra)	;
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);
	void	Process();
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);

	void	SetQuestMissionInfo(std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>& Info);
};


//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_ITEM
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_ITEM : public Window, public Exec, public ButtonVisual/*, public C_VS_UI_QUEST_MANAGER*/
{
private:
	enum EXEC_ID
	{
		CLOSE_ID,
		PUSHPIN_ID,
		ALPHA_ID,
//		TAB1_ID,
//		TAB2_ID,

		SLOT_X_COUNT = 5,
		SLOT_Y_COUNT = 2,

		// 2x3 grid size
		SLOT_WIDTH = 60,
		SLOT_HEIGHT = 100,
	};
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK*				m_pImage_Spk;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;	
	int							m_SelectPos;
	bool						m_bl_focus;
	int							m_OustersOffset;// 아우스터즈 픽셀 보정

	std::vector<MItem*>			m_QuestItemInfo;

public:
	C_VS_UI_QUEST_ITEM(C_SPRITE_PACK* spr);
	~C_VS_UI_QUEST_ITEM();

	void	KeyboardControl(UINT message, UINT key, long extra)	;
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);
	void	Process();
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start(std::vector<MItem*>& Info);
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);

	void	SetQuestItemInfo(std::vector<MItem*>& Info);
};


//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_ICON
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_ICON : public Window, public Exec/*, public C_VS_UI_QUEST_MANAGER*/
{
public:
private:
	C_SPRITE_PACK*				m_pImage_Spk;
	int							m_StartPosX;
	DWORD						m_dwDelay;
public:
	C_VS_UI_QUEST_ICON(C_SPRITE_PACK* spr);
	~C_VS_UI_QUEST_ICON();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button) {};
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button){};
	void	Run(id_t id) {};
	void	Process(){};
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);

};


//-----------------------------------------------------------------------------
// class C_VS_UI_RANGER_CHAT
//
//-----------------------------------------------------------------------------
class C_VS_UI_RANGER_CHAT :  public Window, public Exec, public ButtonVisual
{
public:

private:
	enum EXEC_ID
	{
		PUSHPIN_ID,
		ALPHA_ID,
	};
	ButtonGroup *				m_pC_button_group;
	std::string					m_Str;
	bool						m_bl_focus;
	int							m_Offset;
public:
	C_VS_UI_RANGER_CHAT();
	~C_VS_UI_RANGER_CHAT();

	void	KeyboardControl(UINT message, UINT key, long extra)	;
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);
	void	Process();
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
	void	SetRangerChatString(char* str){	if(str != NULL) m_Str = str; }
};

class C_VS_UI_PERSNALSHOP_MESSAGE : public Window, public Exec, public ButtonVisual
{
private :
	LineEditorVisual		m_lev_content;	
	ButtonGroup			*m_pC_button_group;

	std::string			m_content,m_status;

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,		
	};

public :	
	C_VS_UI_PERSNALSHOP_MESSAGE();
	~C_VS_UI_PERSNALSHOP_MESSAGE();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);

	const char* GetCurrentContent();
	void	SetContent(const char* Content);

	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	bool	IsPixel(int _x,int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();		
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();		
	}



//	void	SetEnable() { m_Status = INPUT_STATUS_NORMAL; }
//	void	SetDisable() { m_Status = INPUT_STATUS_DISABLE; }

};

class C_VS_UI_POWER_JJANG : public Window, public Exec, public ButtonVisual
{

public :
	
private :

	ButtonGroup			*m_pC_button_group;
	C_SPRITE_PACK		m_pC_spk;
	C_SPRITE_PACK		m_pC_item_spk;

	enum INFO_SPK
	{
		JJANG_MAIN,
		JJANG_BUTTON_EXCHANGE,			// 교환 버튼
		JJANG_BUTTON_EXCHANGE_HILIGHT,	// 교환 버튼 하이라이트
		JJANG_BUTTON_EXCHANGE_PUSH,		// 교환 버튼 푸쉬
	};
	enum ITEM_CODE
	{
		CANDY = 0,				// 사탕 1개 
		RESURRECTION_SCROLL,	// 부활 스크롤 1개
		ELIXIR_SCROLL,			// 엘릭서 스크롤 1개
		MEGAPHONE,				// 확성기 30분
		NAMING_PEN,				// 네이밍 펜 1개
		SIGNPOST,				// 알림판 6시간
		BLACK_RICE_CAKE_SOUP	// 검은 떡국 1개
	};
	enum EXEC_ID
	{
		CLOSE_ID,
		EXCHANGE_ID,
		PHONENUM_ID,
		PUSHPIN_ID,
		ALPHA_ID,
		HELP_ID,
	};

	struct POWER_JJANG_ITEM{
		BYTE			bItemClass;
		BYTE			bItemType;
		BYTE			bItemImageNum;
		MItem*			pItem;
	};

	C_VS_UI_SCROLL_BAR	*m_pC_scroll_bar;
	bool				m_bl_focus;

	std::vector<POWER_JJANG_ITEM*>	m_Powerjjang_ItemList;
	int					m_SelectPos;
	LineEditorVisual	m_EditPhoneNumber;
	std::string			m_szMyPhoneNumber;
	WORD				m_AvailablePoint; // 겜블 가능 포인트

	int					m_GambleMode;
	DWORD				m_dwSpeed;
	DWORD				m_dwCurrentTime;
	DWORD				m_dwOutCurrentTime;
	BYTE				m_OutLinePositon;
public :
	C_VS_UI_POWER_JJANG();
	~C_VS_UI_POWER_JJANG();

	
	
	void	ToggleWindow();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	
	bool	IsPixel(int _x,int _y);
	
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);

	void	SetItemList();
	void	ReleaseItemList();

	void	PowerjjangGambleResult(BYTE bItemCode);
};

#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관


//-----------------------------------------------------------------------------
// class C_VS_UI_INVENTORY
//
// slayer/vampire inventory base class.
//
// - slayer/vampire inventory 공통의 interface를 정의한다. 
// - slayer에서 vampire로 바뀌거나 또는 그 반대일 때, 객체를 재생성하여
//   inventory는 초기화되지만, inventory의 item은 그대로 유지된다. interface상에서의
//   inventory는 item을 저장하고 있지 않다. 그래서 이 class를 base로 하는 것이다.
//-----------------------------------------------------------------------------
class C_VS_UI_INVENTORY_SUB : public Window, public Exec, public ButtonVisual
{
	//-----------
	// Data
	//-----------
public:
	enum 
	{ 
		GRID_UNIT_PIXEL_X = 30,
		GRID_UNIT_PIXEL_Y = 30,
	};

private:

protected:
	enum INVENTORY_SPK_INDEX
	{
		INVENTORY_ITEMBACK,
		INVENTORY_ITEMBACK_ALPHA,
		INVENTORY_WINDOW_2X4,
		INVENTORY_WINDOW_2X4_ALPHA,
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		INVENTORY_WINDOW_4X6,
		INVENTORY_WINDOW_4X6_ALPHA,
	#endif

	};

	enum EXEC_ID
	{
		CLOSE_ID,
		ALPHA_ID,
	};

	enum 
	{ 
		TYPE_2X4 = 0,
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		TYPE_4X6 = 1,
	#endif
		
	};

	ButtonGroup *				m_pC_button_group;

	
	C_SPRITE_PACK *			m_pC_inventory_spk;

	BYTE					m_SubInventoryType;
	
	Rect					m_grid_rect; // 전체 Grid rect.

	int						m_focus_grid_x, m_focus_grid_y;

	// 상속받는 class에서 반드시 set해줘야 한다.
	int							m_grid_start_offset_x, m_grid_start_offset_y;

	int						m_MainWindowSpk;
	int						n_nType;


	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		MSubInventory*			m_MultiPackItem;
	#endif


	int						m_GridX, m_GridY;
	BYTE**					m_bl_Alpha;
	//------------
	// Method
	//------------
private:
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	UnacquireMouseFocus();
	void	AcquireDisappear();
	void	AcquireMouseFocus();
	bool	AllMatchWindowPixel(int _x, int _y) const;
	bool	AnyMatchWindowPixel(int _x, int _y) const;

protected:
	bool	Click(int grid_start_x, int grid_start_y);

public:
	C_VS_UI_INVENTORY_SUB(MItem* pItem);
	virtual ~C_VS_UI_INVENTORY_SUB();

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		MItem*	GetSubInventoryItem() { return m_MultiPackItem; }
	#endif


	void	Start();
	void	Finish();
	void	Show();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);

	void	ResetRect();
	bool	TestGridRect(int _x, int _y) const;
	int	GetFocusedItemGridX(const MItem * p_item);
	int	GetFocusedItemGridY(const MItem * p_item);
	int	GetFocusedItemGridH(const MItem * p_item);
	Rect	GetGridRect() const { return m_grid_rect; }
	
	int		GetFocusGridX() {return  m_focus_grid_x;}
	int		GetFocusGridY() {return  m_focus_grid_y;}

	bool	ReplaceSubInventoryItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);
	bool	AddItemToSubInventory(MItem* pItem, int X, int Y);
	
};
#endif
#endif
