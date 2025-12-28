/*-----------------------------------------------------------------------------

	VS_UI_GameCommon.h

	Game common interface.

	Common:
				Chatting interface
				NPC
				Exchange

	2000.8.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GAME_COMMON_H__
#define __VS_UI_GAME_COMMON_H__

#include "VS_UI_Base.h"
#include "VS_UI_Util.h"
#include "VS_UI_description.h"
#include "VS_UI_ExtraDialog.h"
#include "VS_UI_Title.h" // for S_SLOT

//#include "VS_UI_GameSlayer.h"
class C_VS_UI_SLAYER_QUICKITEM;
class C_VS_UI_OUSTERS_QUICKITEM;

extern bool gbl_mine_progress;

#include "SkillDef.h"
#include "MItem.h"
#include "MMoneyManager.h"
#include "MSkillManager.h"
#include "ClientFunction.h"
#include "MZone.h"

#include <vector>	//by larosel

#define SLAYER_FONT_COLOR				RGB(186, 189, 191)//RGB(20, 70, 0)	// by larosel
#define VAMPIRE_FONT_COLOR				RGB(186, 189, 191)//RGB(255, 255, 255)

#define ITEM_DISABLE_COLOR_SET			40

//-----------------------------------------------------------------------------
// class C_VS_UI_LEVELUP
//
// vampire/slayer level up시 나타나는 button.
//-----------------------------------------------------------------------------
class C_VS_UI_LEVELUP : public Window
{
private:
	enum IMAGE_SPK_INDEX
	{
		LVU_GUARD_SLAYER,
		LVU_GUARD_VAMPIRE,
		LVU_GUARD_OUSTERS,
		PARTY_REQUEST,
		PARTY_INVITE,
		RESURRECT_SLAYER,
		RESURRECT_VAMPIRE,
		RESURRECT_OUSTERS,
		LVU_BUTTON,
		LVU_BUTTON_OUSTERS,
	};

	C_SPRITE_PACK				m_image_spk;

	bool							m_bl_pushed;
	bool							m_bl_focused;

public:
	C_VS_UI_LEVELUP();
	~C_VS_UI_LEVELUP();

	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
	void	CancelPushState() { m_bl_pushed = false; }
	void	UnacquireMouseFocus() { m_bl_focused = false; }
};

//-----------------------------------------------------------------------------
// class C_VS_UI_REQUEST_PARTY
//
//-----------------------------------------------------------------------------
class C_VS_UI_REQUEST_PARTY : public Window
{
private:
	enum IMAGE_SPK_INDEX
	{
		REQUEST_GUARD_SLAYER,
		REQUEST_GUARD_VAMPIRE,
		REQUEST_GUARD_OUSTERS,
		PARTY_REQUEST,
		PARTY_INVITE,
		RESURRECT_SLAYER,
		RESURRECT_VAMPIRE,
		RESURRECT_OUSTERS,
		LVU_BUTTON,
		LVU_BUTTON_OUSTERS,
		RESURRECT_SCROLL,
		ELIXIR_SCROLL,
	};

	C_SPRITE_PACK				m_image_spk;

	bool							m_bl_pushed;
	bool							m_bl_focused;
	std::string					m_name;

// TIMER
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_timer_tickcount;

	bool	Timer(bool reset = false);

public:
	enum REQUEST_PARTY
	{
		REQUEST,
		INVITE,
	};

private:
	REQUEST_PARTY				m_type;

public:
	C_VS_UI_REQUEST_PARTY(const char *name, DWORD timer);
	~C_VS_UI_REQUEST_PARTY();

	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
	void	CancelPushState() { m_bl_pushed = false; }
	void	UnacquireMouseFocus() { m_bl_focused = false; }
};

//-----------------------------------------------------------------------------
// class C_VS_UI_REQUEST_DIE
//
//-----------------------------------------------------------------------------
class C_VS_UI_REQUEST_DIE : public Window
{
private:
	enum IMAGE_SPK_INDEX
	{
		RESURRECT_GUARD_SLAYER,
		RESURRECT_GUARD_VAMPIRE,
		RESURRECT_GUARD_OUSTERS,
		PARTY_REQUEST,
		PARTY_INVITE,
		RESURRECT_SLAYER,
		RESURRECT_VAMPIRE,
		RESURRECT_OUSTERS,
		LVU_BUTTON,
		LVU_BUTTON_OUSTERS,
		RESURRECT_SCROLL,
		ELIXIR_SCROLL,
	};

	C_SPRITE_PACK				m_image_spk;

	bool							m_bl_pushed;
	bool							m_bl_focused;

// TIMER
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_timer_tickcount;

public:
	C_VS_UI_REQUEST_DIE(DWORD timer);
	~C_VS_UI_REQUEST_DIE();

	bool	Timer(bool reset = false);
	
	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
	void	CancelPushState() { m_bl_pushed = false; }
	void	UnacquireMouseFocus() { m_bl_focused = false; }
};

//-----------------------------------------------------------------------------
// class C_VS_UI_PARTY_MANAGER
//
//-----------------------------------------------------------------------------
class C_VS_UI_PARTY_MANAGER : public Window, public Exec, public ButtonVisual
{
private:
	enum
	{
		window_default_height = 18, 
		window_gap = 41, 
		window_bottom_gap = 7,
		away_x = 95,
		away_y = 8,
	};

	enum IMAGE_SPK_INDEX
	{
		PARTY_WINDOW,
		PARTY_WINDOW_ALPHA,
		PARTY_WINDOW_BOTTOM,
		DENY_BUTTON,
		DENY_BUTTON_PUSHED,
		DENY_BUTTON_HILIGHTED,
		DENY_BUTTON_HILIGHTED_PUSHED,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		HELP_BUTTON_HILIGHTED,
		HELP_BUTTON_PUSHED_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_PUSHED,
		CLOSE_BUTTON_HILIGHTED,
		CLOSE_BUTTON_PUSHED_HILIGHTED,
		AWAY_BUTTON,
		AWAY_BUTTON_HILIGHTED,
		AWAY_BUTTON_HILIGHTED_PUSHED,
		FACE_GRID,
		NAME_GRID,
		HPBAR,
	};

	enum EXEC_ID
	{
		ALPHA_ID,
		DENY_ID,
		HELP_ID,
		CLOSE_ID,

		AWAY_I,
		AWAY_0,
		AWAY_1,
		AWAY_2,
		AWAY_3,
		AWAY_4,
	};

	C_SPRITE_PACK				* m_p_face_spk;
	C_SPRITE_PACK				* m_p_large_face_spk;
	C_SPRITE_PACK				* m_p_image_spk;
	ButtonGroup *				m_pC_bottom_button_group;
	ButtonGroup *				m_pC_button_group;

	std::vector<C_SPRITE_PACK *>		m_vp_face;
	std::vector<std::string>					m_v_face_name;

	bool	m_away_button_focused;
	bool	m_away_pushed;
	int		m_old_away_focused;
	int		m_away_focused;
	bool	m_bl_show_face_large;
	DWORD	m_dw_show_face_large_time;

public:
	C_VS_UI_PARTY_MANAGER();
	~C_VS_UI_PARTY_MANAGER();

	void	RefreshFaceImage();

	int		GetPartyManagerFocused()	{ return m_away_focused; }
	void	UnacquireMouseFocus() { m_pC_bottom_button_group->UnacquireMouseFocus(); m_pC_button_group->UnacquireMouseFocus(); m_old_away_focused = -1; m_away_focused= -1;}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); m_pC_bottom_button_group->CancelPushState(); }
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	void	Process();

	bool	Timer(bool reset = false);

	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
};

//-----------------------------------------------------------------------------
// HotKey (abstract base class)
//
// 
//-----------------------------------------------------------------------------
class HotKey
{
public:

	virtual void HotKey_PetInfo() = 0;		// ctrl+CapsLock
	virtual void HotKey_MailBox() = 0;		// ctrl+b
	virtual void HotKey_Party() = 0;		// ctrl+p
	virtual void HotKey_Mark() = 0;			// '한자'키
//	virtual void HotKey_WindowToggle()=0;	// ctrl + space

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		virtual void HotKey_Inventory(bool IsCheckSubInventory = false)=0;		// tab
	#else
		virtual void HotKey_Inventory()=0;		// tab
	#endif
	virtual void HotKey_Friend()=0;			//add by viva
	virtual void HotKey_Gear()=0;			// ctrl + tab
	virtual void HotKey_QuickItemSlot() {}	// ctrl + ~
	virtual void HotKey_Skill()=0;			// ctrl + s
	virtual void HotKey_CharInfo()=0;		// ctrl + i
	virtual void HotKey_SkillInfo()=0;		// ctrl + k
	virtual void HotKey_Grade2Info()=0;		// ctrl + k
	virtual void HotKey_Grade1Info()=0;		// ctrl + k
	virtual void HotKey_Help()=0;			// ctrl + h
	virtual void HotKey_Minimap()=0;		// ctrl + m
	virtual void HotKey_Chat()=0;			// ctrl + c
	virtual void HotKey_ZoneChat()=0	;	// ctrl + z
	virtual void HotKey_GuildChat()=0;		// ctrl + g
	virtual void HotKey_UnionChat()=0;		// ctrl + d
	virtual void HotKey_Whisper()=0;		// ctrl + w
	virtual void HotKey_ExtendChat()=0;		// ctrl + e

	virtual void HotKey_ESC() {}			// esc

	virtual void HotKey_F1() {}				// f1
	virtual void HotKey_F2() {}				// f2
	virtual void HotKey_F3() {}				// f3
	virtual void HotKey_F4() {}				// f4
	virtual void HotKey_F5() {}				// f5
	virtual void HotKey_F6() {}				// f6
	virtual void HotKey_F7() {}				// f7
	virtual void HotKey_F8() {}				// f8
	virtual void HotKey_F9()=0;				// f9
	virtual void HotKey_F10()=0;			// f10
	virtual void HotKey_F11()=0;			// f11
	virtual void HotKey_F12()=0;			// f12

	virtual void HotKey_PartyChat() = 0;	// ctrl + a
	virtual void HotKey_Quest() = 0;		// ctrl + q

	// 2004, 5, 17 sobeit add start
	virtual void HotKey_SummonPet()=0;	// ctrl + space
	// 2004, 5, 17 sobeit add end
};

enum CHAT_LINE_CONDITION
{
	CLD_NORMAL,
	CLD_ZONECHAT,
	CLD_WHISPER,
	CLD_PARTY,
	CLD_GUILD,
	CLD_UNION,
	CLD_INFO, // 정보 메시지.
	CLD_MASTER,	// 운영자 메시지

	CLD_TOTAL = CLD_MASTER,
};

//-----------------------------------------------------------------------------
// class StringCell
//
// Single string class.
//-----------------------------------------------------------------------------
class StringCell
{
private:
	char *						m_sz_string;

public:
	void	Release()
	{
		DeleteNewArray(m_sz_string);
	}

	StringCell()
	{
		m_sz_string = NULL;
	}

	virtual ~StringCell()
	{
		Release();
	}

	void	SetString(const char * sz_str)
	{
		if (sz_str == NULL)
			return;

		Release();
		m_sz_string = new char[strlen(sz_str)+1];
		strcpy(m_sz_string, sz_str);
	}

	const char * GetString() const { return m_sz_string; }
};

extern COLORREF g_color[4][4];

//-----------------------------------------------------------------------------
// class C_VS_UI_CHAT_LINE
//
//
//-----------------------------------------------------------------------------
class C_VS_UI_CHAT_LINE
{
private:
	StringCell					m_id_cell;
	StringCell					m_msg_cell;
	CHAT_LINE_CONDITION			m_condition;
	DWORD						m_Color;
		
public:
	C_VS_UI_CHAT_LINE()
	{
		m_condition = CLD_NORMAL;
		m_Color = g_color[0][0];
	}

	virtual ~C_VS_UI_CHAT_LINE() {}

	void	Set(const char * sz_id, const char * sz_msg, CHAT_LINE_CONDITION condition, DWORD color)
	{
		m_id_cell.SetString(sz_id);
		m_msg_cell.SetString(sz_msg);
		m_condition = condition;
		m_Color = color;
	}

	const char * GetIdString() const { return m_id_cell.GetString(); }
	const char * GetMsgString() const { return m_msg_cell.GetString(); }
	CHAT_LINE_CONDITION	GetCondition() const { return m_condition; }
	DWORD	GetColor() const { return m_Color; }
};

//-----------------------------------------------------------------------------
// class C_VS_CHAT_LIST
//
//
//-----------------------------------------------------------------------------
class C_VS_CHAT_LIST
{
private:
	SimpleDataList<C_VS_UI_CHAT_LINE *>		m_list;

public:
	C_VS_CHAT_LIST() {}
	~C_VS_CHAT_LIST()
	{
		C_VS_UI_CHAT_LINE * data;
		for (int i=0; i < m_list.Size(); i++)
			if (m_list.Data(i, data))
				delete data;
	}

	void	Add(const char * sz_id, const char * sz_msg, CHAT_LINE_CONDITION condition, DWORD color)
	{
		C_VS_UI_CHAT_LINE * p_line = new C_VS_UI_CHAT_LINE;
		p_line->Set(sz_id, sz_msg, condition, color);

		m_list.Add(p_line);
	}

	int	Size() const { return m_list.Size(); }

	C_VS_UI_CHAT_LINE *	PopBack()
	{
		// !data를 delete하지는 않는다.

		C_VS_UI_CHAT_LINE * data;

		if (m_list.Data(0, data))
		{
			m_list.Delete(data);
			return data;
		}

		return NULL;
	}

	C_VS_UI_CHAT_LINE *	GetLine(int n) const
	{
		// n이 0에 가까울수록 최근에 입력된 line이다.

		if (n < 0)
			return NULL;

		int index = m_list.Size()-n-1;

		C_VS_UI_CHAT_LINE * data;

		if (m_list.Data(index, data))
			return data;

		return NULL;
	}
};

class PAPERING_HISTORY
{
	public:
	std::string			m_string;
	std::vector<DWORD>	m_timer;
};

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING
//
// 일반적인 chatting과 귓속말(slayer = PCS)을 수행한다.
//-----------------------------------------------------------------------------
class C_VS_UI_CHATTING : public Window, public Exec, public ButtonVisual
{
public:
	enum EXEC_ID
	{
		ALPHA_ID,
		PUSHPIN_ID,
		MARK_ID,
		COLOR_ID,
		LANGUAGE_ID,
		SPREAD_ID,

		// CHAT
		CHAT_NORMAL_ID,
		CHAT_ZONE_ID,
		CHAT_WHISPER_ID,
		CHAT_PARTY_ID,
		CHAT_GUILD_ID,
		CHAT_UNION_ID,

		// FILTER
		FILTER_NORMAL_ID,
		FILTER_ZONE_ID,
		FILTER_WHISPER_ID,
		FILTER_PARTY_ID,
		FILTER_GUILD_ID,
		FILTER_UNION_ID,

	};

	
private:
	enum MAIN_SPK_INDEX
	{
		MAIN,
		MAIN_ALPHA,
		MAIN_RIGHT,
		MAIN_BOTTOM,
		INPUT,
		INPUT_ALPHA,
		INPUT_RIGHT,
		BUTTON_SPREAD,
		BUTTON_SPREAD_HILIGHTED,
		BUTTON_SPREAD_HILIGHTED_PUSHED,
		BUTTON_FILTER,
		BUTTON_FILTER_HILIGHTED,
		BUTTON_FILTER_SELECTED,
		BUTTON_MARK,
		BUTTON_MARK_HILIGHTED,
		BUTTON_MARK_HILIGHTED_PUSHED,
		BUTTON_COLOR,
		BUTTON_COLOR_HILIGHTED,
		BUTTON_COLOR_HILIGHTED_PUSHED,
		BUTTON_HAN,
		BUTTON_HAN_HILIGHTED,
		BUTTON_HAN_HILIGHTED_PUSHED,
		BUTTON_ENG,
		BUTTON_ENG_HILIGHTED,
		BUTTON_ENG_HILIGHTED_PUSHED,
		BUTTON_NORMAL,
		BUTTON_NORMAL_HILIGHTED,
		BUTTON_NORMAL_HILIGHTED_PUSHED,
		BUTTON_NORMAL_SELECTED,
		BUTTON_NORMAL_SELECTED_PUSHED,
		BUTTON_WHISPER,
		BUTTON_WHISPER_HILIGHTED,
		BUTTON_WHISPER_HILIGHTED_PUSHED,
		BUTTON_WHISPER_SELECTED,
		BUTTON_WHISPER_SELECTED_PUSHED,
		BUTTON_ZONE,
		BUTTON_ZONE_HILIGHTED,
		BUTTON_ZONE_HILIGHTED_PUSHED,
		BUTTON_ZONE_SELECTED,
		BUTTON_ZONE_SELECTED_PUSHED,
		BUTTON_PARTY,
		BUTTON_PARTY_HILIGHTED,
		BUTTON_PARTY_HILIGHTED_PUSHED,
		BUTTON_PARTY_SELECTED,
		BUTTON_PARTY_SELECTED_PUSHED,
		BUTTON_GUILD,
		BUTTON_GUILD_HILIGHTED,
		BUTTON_GUILD_HILIGHTED_PUSHED,
		BUTTON_GUILD_SELECTED,
		BUTTON_GUILD_SELECTED_PUSHED,
		BUTTON_UNION,
		BUTTON_UNION_HILIGHTED,
		BUTTON_UNION_HILIGHTED_PUSHED,
		BUTTON_UNION_SELECTED,
		BUTTON_UNION_SELECTED_PUSHED,
		BUTTON_CHANNEL,
	};

	enum
	{
		CHAT_INPUT_LIMIT = 80,
	};

	enum HISTORY
	{
		HISTORY_SIZE = 200,
		HISTORY_SIZE_TEMP = 250,
	};

	enum
	{
		MARK_X = 12,
		MARK_Y = 17,
		COLOR_X = 20,
		COLOR_Y = 17,
	};

	enum RESIZE_STATE
	{
		RESIZE_NOT,
		RESIZE_LEFT,
		RESIZE_TOP,
		RESIZE_RIGHT,
		RESIZE_BOTTOM,
		RESIZE_LEFTTOP,
		RESIZE_RIGHTTOP,
		RESIZE_LEFTBOTTOM,
		RESIZE_RIGHTBOTTOM,
	};

private:
	int						CHAT_LINE_START_X, CHAT_LINE_START_Y;
	int						CHAT_HISTORY_START_Y;
	int						CHAT_WINDOW_WIDTH, CHAT_INPUT_WIDTH;

	bool					m_bl_input_mode;
	bool					m_chat_filter[CLD_TOTAL];
	CHAT_LINE_CONDITION		m_chat_mode;

	Rect					m_backup_rect;

	int						m_sub_window;
	Rect					m_sub_rect;
	POINT					m_sub_selected;

	RESIZE_STATE			m_resize;
	POINT					m_backup_window_point;

	C_VS_UI_SCROLL_BAR		*m_pC_scroll_bar, *m_pC_sub_scroll_bar;
	C_SPRITE_PACK			* m_pC_chatting_spk;
	
	ButtonGroup *			m_pC_button_group;
	ButtonGroup *			m_pC_input_button_group;
	ButtonGroup *			m_pC_input_right_button_group;

	int							m_history_line;
	std::vector<PAPERING_HISTORY>	m_history;

//	int							m_string_output_x; // 주석처리 by larosel

	void	ScrollHistory();

	std::vector<std::string>	m_v_help_string;
	std::vector<bool>			m_v_help_check;

	//
	// Chat history buffer.
	//
	C_VS_CHAT_LIST				m_pC_history_list;
	C_VS_CHAT_LIST				m_pC_history_temp_list;

	int scroll_up_x, scroll_up_y, scroll_down_x, scroll_down_y, scrolltag_x, scrolltag_y, scrolltrack_x, scrolltrack_y;	// by larosel

	bool							m_chat_stacked[CLD_TOTAL];

	LineEditorVisual				m_lev_chatting;

	COLORREF						m_color_tab[CLD_TOTAL+1];

	bool							m_bl_spreadID;

	std::vector<std::string>		m_sz_whisper_id;	// by larosel
	std::string						m_sz_whisper_backup;	// by larosel
	bool							m_bl_whisper_mode;	// by larosel
	bool							m_bl_focus_whisper;	// by larosel
	int								m_whisper_index;	// by larosel

	//
	// Vampire/Slayer 둘 다 나타날 수 있는 Window의 수는 2개(Inventory, Gear)이다.
	// 화면 이 둘이 나타났을 경우 click focus가 있기 때문에 그것을 구별해줘야 한다.
	//

//	int								m_slayer_chat_mode;	// 주석처리 by larosel
//	void	AddChatToHistoryTemp(char * str, char * sz_id, CHAT_LINE_CONDITION condition);	// 주석처리 by laorsel

	enum
	{
		TIMER_NONE,
		TIMER_ZONECHAT,
		TIMER_REP,
		TIMER_PAPERING,
		TIMER_HIDE,
	};

// TIMER
	std::vector<DWORD>			m_dw_rep_tickcount;
	std::vector<DWORD>			m_dw_papering_tickcount;
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_zonechat_timer;
	DWORD						m_dw_rep_timer;
	DWORD						m_dw_papering_timer;
	DWORD						m_dw_help_timer;
	DWORD						m_dw_help_prev_tickcount;
	DWORD						m_dw_hide_timer;
	DWORD						m_dw_hide_prev_tickcount;
	int							m_timer;

	bool	Timer(bool reset = false);
	bool	TimerHelp();
	bool	TimerHide(bool reset = false);

public:
	void	Extend();

	void	SetSubWindow(int type)		{ m_sub_window = type; }
	//-------------------------
	// WHISPER	// by larosel
	//-------------------------
	std::string	GetWhisperID(int num = -1);	// by larosel
	int		GetWhisperSize(){ return m_sz_whisper_id.size();}	// by larosel
	bool	AddWhisperID(const char *sz_ID);	// by larosel
	void	ChangeWhisperFocus();	// by larosel
	bool	GetWhisperFocus(){ return m_bl_focus_whisper; }	// by larosel
	bool	SlayerWhisperMode(bool mode);	// by larosel
	void	TribeChanged();
	bool	IsSpreadID(){ return m_bl_spreadID; }
	bool	IsPixel(int _x, int _y);
	void	SetWhisperID(char *id);
	bool	IsInputMode()	{ return m_bl_input_mode; }

	//---------------------------
	// GUILDCHAT // by larosel
	//---------------------------
	void	SetGuildChat( bool guild );

	void	ResetScroll();	// by larosel

	C_VS_UI_CHATTING();
	~C_VS_UI_CHATTING();

	void	Acquire() { m_lev_chatting.Acquire(); }
	bool	IsEmpty()	{ return (m_lev_chatting.Size() <= 0); }
	void	Start();
	void	Finish();

	void	RestoreHistoryTemp();
	void	Show();
	bool	MouseControl(UINT message, int _x, int _y);
//	bool	MouseControlExtra(UINT message, int x, int y);	// by sigi
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	AddChatToHistory(const char * str, const char * sz_id, CHAT_LINE_CONDITION condition, DWORD color);
	void	AddToChatHistory(const char * sz_str, const char * sz_id, CHAT_LINE_CONDITION condition, DWORD color);
	void	SetInputString(const char* pString);	// by sigi
	void	AddInputString(const char* pString);	// by sigi
	void	SetInputStringColor(COLORREF rgb)	{ m_lev_chatting.SetInputStringColor(rgb); } // by sigi
	const char* GetInputString() const			{ return (const char*)m_lev_chatting.GetString(); }
	void	InsertMark(char_t ch)				{ m_lev_chatting.InsertMark(ch); }

	bool	IsNormalChatStacked()						{ return m_chat_stacked[CLD_NORMAL]; }
	bool	IsWhisperStacked()							{ return m_chat_stacked[CLD_WHISPER]; }
	bool	IsZoneChatStacked()							{ return m_chat_stacked[CLD_ZONECHAT]; }
	bool	IsGuildChatStacked()						{ return m_chat_stacked[CLD_GUILD]; }
	bool	IsPartyChatStacked()						{ return m_chat_stacked[CLD_PARTY]; }
	bool	IsInfoStacked()								{ return m_chat_stacked[CLD_INFO]; }

	// common
	void	UnacquireMouseFocus() 
	{
		m_pC_button_group->UnacquireMouseFocus();
		m_pC_input_button_group->UnacquireMouseFocus();
		m_pC_input_right_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();
		m_pC_input_button_group->CancelPushState();
		m_pC_input_right_button_group->CancelPushState();
	}
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	Run(id_t id);

	void	Process();

};
//class C_VS_UI_FRIEND_INFO;
//add by viva : friend system

class C_VS_UI_FRIEND_INFO : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup*		m_pC_button_group;
	C_SPRITE_PACK		m_friend_spk;
	int					m_scroll;
	int					m_iFocus;
	bool				m_bl_set_load;
	bool				m_bl_scrolling;
	int					m_tag_y, m_tag_diff_y;

	int scroll_up_x, scroll_up_y, scroll_down_x, scroll_down_y;
	int kind_x, kind_y, black_x, black_y;

	
	enum FRIEND_SPK
	{
		FRIEDN_WINDOW=0,
		KIND_BUTTON,
		KIND_BUTTON_HILIGHTED,
		KIND_BUTTON_PUSHED,
		BLACK_BUTTON,
		BLACK_HILIGHTED,
		BLACK_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		//FIND_BUTTON,
		//FIND_BUTTON_PUSHED,
		//ADD_BUTTON,
		//ADD_BUTTON_PUSHED,
		//SETUP_BUTTON,
		//SETUP_BUTTON_HILIGHTED,
		//SETUP_BUTTON_PUSHED,
		BAR_HILIGHTED
	};
	enum EXEC_ID
	{	
		KIND_ID,
		BLACK_ID,
		SCROLL_BAR_ID,
		SCROLL_UP_ID,
		SCROLL_DOWN_ID,
		FIND_ID,
		ADD_ID,
		SETUP_ID,
		EXEC_MAX,
	};
	EXEC_ID				m_ButtonID_Down;
public:
	C_VS_UI_FRIEND_INFO();
	~C_VS_UI_FRIEND_INFO();

	struct FRIEND_CHATTING_INFO
	{
		std::string Content;
		std::string Date;
		
	};
	struct HISTORY_LIST
	{
		std::string strLine;
		bool IsBlank;
	};
	struct FRIEND_LIST
	{
		std::string Name;
		std::vector<FRIEND_CHATTING_INFO*> m_v_pChatting_info;
		int blIsOnline;
		int blIsShow;
		std::vector<HISTORY_LIST*> m_v_pHistory_List;
		FRIEND_LIST()
		{
			blIsOnline = 0;
			blIsShow = 0;
		}
	};


	std::vector<FRIEND_LIST*> m_v_pFriendList;
	std::vector<FRIEND_LIST*> m_v_pBlackList;
	//std::vector<FRIEND_LIST*> m_v_pKindList;
	void*	GetFriendList(const string Name);
	void*	GetBlackList(const string Name);
	void	AddChattingMessageToHistory(FRIEND_LIST* pList, string Name, string strMessage);
//	void	GetStrLine(std::vector<std::string>& v_strlist, char* str, int strLen, bool IsBlank);
	void	UpdateFriendList(FRIEND_LIST& List);
	void	SortOrder();

	void	Process();
	void	UnacquireMouseFocus(){}
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra){}
//	void	WindowEventReceiver(id_t event);
	void	ResetRect();
	
	void	Start(bool bl_set_load);
	void	Finish();
	void	Show();

};


class C_VS_UI_FRIEND_CHATTING_INFO : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup*		m_pC_button_group;
	C_SPRITE_PACK		m_chatting_spk;
	int					m_scroll;
	bool				m_bl_scrolling;
	int					m_tag_y, m_tag_diff_y;

	int scroll_up_x, scroll_up_y, scroll_down_x, scroll_down_y;
	int history_x, history_y, history_width;
	int	lev_send_width;
	int close_x, close_y;

	enum CHATTING_SPK
	{
		CHATTING_WINDOW=14,
//		BLACK_BUTTON,
//		BLACK_BUTTON_HILIGHTED,
//		BLACK_BUTTON_PUSHED,
//		SEND_BUTTON,
//		SEND_BUTTON_HILIGHTED,
//		SEND_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG
	};
	enum EXEC_ID
	{	
		BLACK_ID,
		SEND_ID,
		SCROLL_BAR_ID,
		SCROLL_UP_ID,
		SCROLL_DOWN_ID,
		CLOSE_ID,
		EXEC_MAX,
	};
public:
	C_VS_UI_FRIEND_CHATTING_INFO(C_VS_UI_FRIEND_INFO::FRIEND_LIST* pFriend_List);
	~C_VS_UI_FRIEND_CHATTING_INFO();
	C_VS_UI_FRIEND_INFO::FRIEND_LIST* m_pList;
	LineEditorVisual			m_lev_send;
	void AddLocalMessageToHistory(string strMessage);

	struct FRIEND_SEND_MESSAGE
	{
		std::string Name;
		std::string Message;
	};
	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
//	void	WindowEventReceiver(id_t event);
	void	ResetRect(){};

	void	Start();
	void	Finish();
	void	Show();
	
};
//end

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
class C_VS_UI_INVENTORY : public Window, public Exec, public ButtonVisual
{
	//-----------
	// Data
	//-----------
public:
	enum 
	{ 
		GRID_X = 10, 
		GRID_Y = 6,
		GRID_UNIT_PIXEL_X = 30,
		GRID_UNIT_PIXEL_Y = 30,
	};

private:

protected:
	enum INVENTORY_SPK_INDEX
	{
		INVENTORY_WINDOW,
		INVENTORY_WINDOW_ALPHA,
		INVENTORY_ITEMBACK,
		INVENTORY_ITEMBACK_ALPHA,
		INVENTORY_WINDOW_BOTTOM,
	};

	enum EXEC_ID
	{
		MONEY_ID,
		DESC_ID,
//		BIKE_ID,
//		EXCHANGE_ID,
		
		HELP_ID,
		CLOSE_ID,
		ALPHA_ID,
	};

	ButtonGroup *				m_pC_button_group;

	bool						m_bl_shift, m_bl_set_load;;

	C_SPRITE_PACK *			m_pC_inventory_spk;
	Rect							m_grid_rect; // 전체 Grid rect.

	int							m_focus_grid_x, m_focus_grid_y;

	// 상속받는 class에서 반드시 set해줘야 한다.
	int							m_grid_start_offset_x, m_grid_start_offset_y;
	int							m_money_button_offset_x, m_money_button_offset_y;

	//------------
	// Method
	//------------
private:
	void	SetDebugStart();
	void	SetDebugEnd();
	void	WriteLogLine(int line);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	UnacquireMouseFocus();
	void	AcquireDisappear();
	void	AcquireMouseFocus();
//	void	CancelPushState();
	bool	AllMatchWindowPixel(int _x, int _y) const;
	bool	AnyMatchWindowPixel(int _x, int _y) const;

protected:
	bool	Click(int grid_start_x, int grid_start_y);
	void	Use();


public:
// TIMER
	static bool		Timer(bool reset = false);
	static DWORD						m_dw_millisec;

	static C_SPRITE_PACK *			m_pC_mine_progress_spk;
	enum MINE_PROGRESS_SPK_INDEX
	{
		INVENTORY_BAR_BACK,
		INVENTORY_BAR,
		SKILL_BAR_BACK,
		SKILL_BAR,
	};

	static int					m_mine_grid_x, m_mine_grid_y;

	C_VS_UI_INVENTORY();
	virtual ~C_VS_UI_INVENTORY();

	//지뢰설치
	bool	StartInstallMineProgress(int focus_grid_x, int focus_grid_y);
	bool	IsInstallMineProgress()		{ return gbl_mine_progress; }
	void	EndInstallMineProgress()	{ gbl_mine_progress = false; }
	//지뢰만들기
	bool	StartCreateMineProgress(int focus_grid_x, int focus_grid_y);
	bool	IsCreateMineProgress()		{ return gbl_mine_progress; }
	void	EndCreateMineProgress()	{ gbl_mine_progress = false; }
	//폭탄 만들기
	bool	StartCreateBombProgress(int focus_grid_x, int focus_grid_y);
	bool	IsCreateBombProgress()		{ return gbl_mine_progress; }
	void	EndCreateBombProgress()	{ gbl_mine_progress = false; }

	void	Start(bool bl_set_load = true);
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

	static void	AutoMove( int grid_x, int grid_y );
	
};

//-----------------------------------------------------------------------------
// class C_VS_UI_GEAR
//
// vampire/slayer gear base class.
//-----------------------------------------------------------------------------
class C_VS_UI_GEAR : public Window, public Exec, public ButtonVisual
{
	//-----------
	// Data
	//-----------
protected:
	enum GEAR_SPK_INDEX
	{
		GEAR_WINDOW,
		GEAR_WINDOW_ALPHA,
		GEAR_BLOOD_BIBLE_BLOCK,
		// 여기서부턴 슬레이어 전용
		BUTTON_FIRST,
		BUTTON_FIRST_HILIGHTED,
		BUTTON_FIRST_HILIGHTED_PUSHED,
		BUTTON_SECOND,
		BUTTON_SECOND_HILIGHTED,
		BUTTON_SECOND_HILIGHTED_PUSHED,
	};

	enum EXEC_ID
	{
		HELP_ID,
		CLOSE_ID,

		// 여기서부턴 슬레이어 전용
		FIRST_ID,
		SECOND_ID,

		ALPHA_ID,
	};

	ButtonGroup *				m_pC_button_group;

	C_SPRITE_PACK *			m_pC_gear_spk; // gear Window image
	C_SPRITE_PACK *			m_pC_gear_slot_spk; // gear slot image
	//by csm 2004.12.30  
	C_SPRITE_PACK *			m_pC_advance_gear_spk; // Advencegear Window image
	

	int							m_focus_slot;
	bool						m_bl_set_load;

	// 상속받는 class에서 반드시 set해줘야 한다.
	int								m_slot_size;
	Rect *							m_p_slot_rect;
	int *							m_p_slot_image;
	BOOL 							m_bl_Abvencement;
//	bool							m_bl_close_button_pushed;
//	bool							m_bl_help_button_pushed;
//	bool							m_bl_close_button_focused;
//	bool							m_bl_help_button_focused;
//	int							m_close_button_offset_x, m_close_button_offset_y;
//	int							m_help_button_offset_x, m_help_button_offset_y;


	void	Use();
	//------------
	// Method
	//------------
private:
//	void	CancelPushState();
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear();
	void	AcquireMouseFocus();
	void	UnacquireMouseFocus();
	bool	AnyMatchWindowPixel(int _x, int _y) const;
	bool	AllMatchWindowPixel(int _x, int _y) const;
	bool	TestSlotRect(int _x, int _y) const;
	bool	IsPixel(int _x, int _y);

public:
	C_VS_UI_GEAR();
	virtual ~C_VS_UI_GEAR();

	void	Start(bool bl_set_load = true);
	void	Finish();
	
	bool	Click(int window_x, int window_y, Rect * slot_rect);
	bool	MouseControl(UINT message, int _x, int _y);
	void	Show() ;
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	void	SetFocusSlot(BYTE Slot) { m_focus_slot = Slot;}
};

//-----------------------------------------------------------------------------
// class C_VS_UI_SKILL
//
// vampire/slayer skill base class.
//-----------------------------------------------------------------------------
class C_VS_UI_SKILL : public Window
{
public:
	static CSpritePack				m_C_spk; // Skill icon Sprite list
	static CSpritePack				m_C_spk_mini; // Skill icon Sprite list

	enum HOTKEY
	{
		HOTKEY1,
		HOTKEY2,
		HOTKEY3,
		HOTKEY4,

		//뱀파용 핫키
		HOTKEY5,
		HOTKEY6,
		HOTKEY7,
		HOTKEY8,

		HOTKEY_MAX,
	};

	enum HOTKEY_GRADE
	{
		GRADE1,
		GRADE2,
		GRADE3,
		GRADE4,

		GRADE_MAX,
	};

protected:
	int							m_skill_hotkey_buf[HOTKEY_MAX][GRADE_MAX];
	int							m_current_skill_set, m_current_skill_grade;
	enum 
	{ 
		// 한 캐릭터가 배울 수 있는 skill 수는 100개 이하라고...
		SPREAD_X_MAX = 10, // 펼쳐졌을 때의 가로의 최대 개수(첫 line은 Spread button을 포함한다).
		SPREAD_Y_MAX = 10,
	};

	C_SPRITE_PACK				m_etc_spk;

	// 상속받는 객체에서 set해줄 것.
	int							m_skill_guard_x, m_skill_guard_y;
	int							m_skill_start_x, m_skill_start_y;
	int							m_skill_guard_extra_offset;
	int							m_extra_icon_offset_x, m_extra_icon_offset_y;

	enum ETC_SPK_INDEX
	{
		SKILL_GUARD, // skill icon 테두리
		SKILL_EMPTY_BUTTON,
		SKILL_PUSHED_BUTTON,
	};

	int							m_selected_skillid; // NOT_SELECTED이면 선택되지 않은 것이다. ACTIONINFO중 하나이다.
	int							m_focused_slot; // Spread button을 0으로 하고 좌에서 우로 증가한다.
	bool							m_bl_pushed;
	bool							m_bl_open;

	void	ResetSize();
	int		GetFocusSlot(int _x, int _y) const;
	
	bool	AbleToUse(int id) const;
	bool	IsEnableSkill(int id) const;
	int		GetDelay(int id) const;

	bool	NoPassive(int n);
	bool	findSkillAvailable(ACTIONINFO id);

// TIMER
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	bool	Timer(bool reset = false);

public:
	C_VS_UI_SKILL();
	virtual ~C_VS_UI_SKILL();

	void	SetStartPoint(int x, int y)	{ m_skill_start_x = x; m_skill_start_y = y; ResetSize(); }
	int		GetSelectedSkillID()	{ return m_selected_skillid; }
	void	SelectSkill(int id);
	int		GetHotkey(HOTKEY hotkey, HOTKEY_GRADE grade) const { return m_skill_hotkey_buf[hotkey][grade]; }
	void	ChangeSkillWheel(bool direct);
	void	ChangeSkillByHotkey(HOTKEY hotkey);
	int		GetSkillID(int n);
	int		GetFocusedSlot() const { return m_focused_slot; }
	void	SetHotkey(HOTKEY hotkey, ACTIONINFO id);
	void	ToggleWindow();
	Point	GetIconPoint(int n) const;
	void	CloseInterface();
	void	UnselectSkill();

	void	AcquireDisappear() {}
	void	CancelPushState();
	void	UnacquireMouseFocus();	
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y)	{ return false; }
	bool	IsPixel2(int _x, int _y);

	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show() { return; }
	void	Show2();
	void	Start();
	void	Process();
	void	ResetSkillSet();
	void	ResetHotkey();
};

//-----------------------------------------------------------------------------
// class C_VS_UI_HPBAR
//
// vampire/slayer HPBar class
//-----------------------------------------------------------------------------
class C_VS_UI_HPBAR : public Window, public Exec, public ButtonVisual
{
private:
	bool					m_width_mode, m_small_mode;
	C_SPRITE_PACK			* m_pC_hpbar_spk;

	ButtonGroup *			m_pC_width_button_group;
	ButtonGroup *			m_pC_height_button_group;
	ButtonGroup *			m_pC_small_width_button_group;
	ButtonGroup *			m_pC_small_height_button_group;

	enum HPBAR_SPK_INDEX
	{
		MAIN_WIDTH,
		MAIN_HEIGHT,
		HPBAR_WIDTH,
		HPBAR_HEIGHT,

		// slayer
		MPBAR_WIDTH,
		MPBAR_HEIGHT,
		HPBAR_BLOOD_WIDTH,
		HPBAR_BLOOD_HEIGHT,
		CHANGE_BUTTON_WIDTH,
		CHANGE_BUTTON_WIDTH_HILIGHTED,
		CHANGE_BUTTON_HEIGHT,
		CHANGE_BUTTON_HEIGHT_HILIGHTED,

		SLAYER_MAX,
		
		// vampire
		EXPBAR_WIDTH = MPBAR_WIDTH,
		EXPBAR_HEIGHT,
		HPBAR_SILVER_WIDTH,
		HPBAR_SILVER_HEIGHT,
		CHANGE_BUTTON,
		CHANGE_BUTTON_PUSHED,
		CHANGE_BUTTON_HILIGHTED,
		CHANGE_BUTTON_HILIGHTED_PUSHED,
		SMALL_BUTTON,
		SMALL_BUTTON_PUSHED,
		SMALL_BUTTON_HILIGHTED,
		SMALL_BUTTON_HILIGHTED_PUSHED,

		VAMPIRE_MAX,

		// ousters				
		OUSTERS_EXPBAR_WIDTH = HPBAR_BLOOD_WIDTH,
		OUSTERS_EXPBAR_HEIGHT,
		OUSTERS_HPBAR_BLOOD_WIDTH,
		OUSTERS_HPBAR_BLOOD_HEIGHT,
		OUSTERS_HPBAR_SILVER_WIDTH,
		OUSTERS_HPBAR_SILVER_HEIGHT,
		OUSTERS_CHANGE_BUTTON_WIDTH,
		OUSTERS_CHANGE_BUTTON_WIDTH_PUSHED,
		OUSTERS_CHANGE_BUTTON_HEIGHT,
		OUSTERS_CHANGE_BUTTON_HEIGHT_PUSHED,
		OUSTERS_SMALL_BUTTON_WIDTH,
		OUSTERS_SMALL_BUTTON_WIDTH_PUSHED,
		OUSTERS_SMALL_BUTTON_HEIGHT,
		OUSTERS_SMALL_BUTTON_HEIGHT_PUSHED,

		OUSTERS_MAX,

		OUSTERS_2ND_MPBAR_WIDTH = 40,
		OUSTERS_2ND_MPBAR_HEIGHT,
		OUSTERS_3ND_MPBAR_WIDTH,
		OUSTERS_3ND_MPBAR_HEIGHT,

		OUSTERS_2ND_SMALL_MPBAR_WIDTH,
		OUSTERS_2ND_SMALL_MPBAR_HEIGHT,
		OUSTERS_3ND_SMALL_MPBAR_WIDTH,
		OUSTERS_3ND_SMALL_MPBAR_HEIGHT,
	};

	enum EXEC_ID
	{
		CHANGE_ID,
		RESIZE_ID,
	};

	int	m_small_offset;

public:
	C_VS_UI_HPBAR();
	~C_VS_UI_HPBAR();

	void	UnacquireMouseFocus() 
	{
		m_pC_width_button_group->UnacquireMouseFocus();
		m_pC_height_button_group->UnacquireMouseFocus();
		m_pC_small_width_button_group->UnacquireMouseFocus();
		m_pC_small_height_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_width_button_group->CancelPushState();
		m_pC_height_button_group->CancelPushState();
		m_pC_small_width_button_group->CancelPushState();
		m_pC_small_height_button_group->CancelPushState();
	}
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
};

//-----------------------------------------------------------------------------
// class C_VS_UI_EFFECT_STATUS
//
// vampire/slayer EFFECT_STATUS class
//-----------------------------------------------------------------------------
class C_VS_UI_EFFECT_STATUS : public Window, public Exec, public ButtonVisual
{
private:
	int						m_scroll;
	bool					m_width_mode;
	bool					m_bl_effect_size_null;
	C_SPRITE_PACK			* m_pC_effect_status_spk;

	ButtonGroup *			m_pC_width_button_group;
	ButtonGroup *			m_pC_height_button_group;

	enum EFFECT_STATUS_SPK_INDEX
	{
		MAIN_WIDTH,
		MAIN_WIDTH_RIGHT,
		MAIN_HEIGHT,
		MAIN_HEIGHT_BOTTOM,

		BUTTON_LEFT,
		BUTTON_LEFT_HILIGHTED,
		BUTTON_LEFT_HILIGHTED_PUSHED,
		BUTTON_RIGHT,
		BUTTON_RIGHT_HILIGHTED,
		BUTTON_RIGHT_HILIGHTED_PUSHED,
		BUTTON_UP,
		BUTTON_UP_HILIGHTED,
		BUTTON_UP_HILIGHTED_PUSHED,
		BUTTON_DOWN,
		BUTTON_DOWN_HILIGHTED,
		BUTTON_DOWN_HILIGHTED_PUSHED,
		BUTTON_CHANGE,
		BUTTON_CHANGE_HILIGHTED,
		BUTTON_CHANGE_HILIGHTED_PUSHED,
	};

	enum EXEC_ID
	{
		PUSHPIN_ID,
		CHANGE_ID,
		UP_ID,
		DOWN_ID,
	};

public:
	C_VS_UI_EFFECT_STATUS();
	~C_VS_UI_EFFECT_STATUS();

	void	ResetSize();
	void	UnacquireMouseFocus() 
	{
		m_pC_width_button_group->UnacquireMouseFocus();
		m_pC_height_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_width_button_group->CancelPushState();
		m_pC_height_button_group->CancelPushState();
	}
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
};


struct MINIMAP_NPC
{
	int x;
	int y;
	int id;
	MString name;
};

struct MINIMAP_SHRINE
{
	int x;
	int y;
	int id;
	MString name;
	BYTE type;
};

//-----------------------------------------------------------------------------
// class C_VS_UI_MINIMAP
//
// vampire/slayer MINIMAP class
//-----------------------------------------------------------------------------
class C_VS_UI_MINIMAP : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			* m_pC_minimap_spk;
	
	CSpriteSurface			* m_p_minimap_surface;
	ButtonGroup *			m_pC_button_group;

	bool								m_bl_refresh;
	POINT								m_map_start_point;
	std::vector<RECT>					m_portal;
	std::vector<int>					m_portal_zone_id;
	std::vector<MINIMAP_NPC>			m_npc;
	std::vector<MINIMAP_SHRINE>			m_shrine;
	std::vector<POINT>					m_Block;
	std::vector<MINIMAP_NPC>				m_Flag;

	std::string m_zone_name;
	int									m_surface_w, m_surface_h;
	int									m_map_x, m_map_y, m_map_w, m_map_h, m_zone_id;
	int									m_board_x, m_board_y;

	//timer
	DWORD						m_dw_minimap_prev_tickcount;
	DWORD						m_dw_minimap_millisec;

	bool	TimerMinimap();

	enum MINIMAP_SPK_INDEX
	{
		MINIMAP_MAIN,
		MINIMAP_MAIN_ALPHA,
		MINIMAP_RIGHT,
		MINIMAP_BOARD,
		MINIMAP_ICON_SELF,
		MINIMAP_ICON_PARTY,
	};

	enum EXEC_ID
	{
		ALPHA_ID,
		PUSHPIN_ID,
	};

public:
	void SetBlock(int x,int y);
	C_VS_UI_MINIMAP();
	~C_VS_UI_MINIMAP();

// 	static	int GetZoneNumber(int zone_id);
// 	static	char * GetZoneName(int zone_id);

	int		GetZoneID()							{ return m_zone_id; }
	void	SetZone(int zone_id);
	void	SetSize(SIZE size) { m_map_w = size.cx; m_map_h = size.cy; }
	void	SetPortal(RECT rect, int id);
	void	SetNPC(MINIMAP_NPC npc);
	void	ClearNPC() { m_npc.clear(); }
	void	SetSafetyZone(RECT rect, bool my_zone);
	void	SetFlagArea(POINT pt);
	void	SetXY(int x, int y)	{ m_map_x = x; m_map_y = y; }
	int		GetX()	{ return m_map_x; }
	int		GetY()	{ return m_map_y; }

	void	TogglePushPin() { Run(PUSHPIN_ID); }

	void	UnacquireMouseFocus() 
	{
		m_pC_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();
	}
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	MouseControlExtra(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
};

//////////////////////////////////////////////////////////////////////////
// 各썹뒈暠
//////////////////////////////////////////////////////////////////////////
class C_VS_UI_WORLDMAP : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			* m_pC_minimap_spk;
	
	CSpriteSurface			* m_p_minimap_surface;
	ButtonGroup *			m_pC_button_group;

	bool								m_bl_refresh;
	POINT								m_map_start_point;
	std::vector<RECT>					m_portal;
	std::vector<int>					m_portal_zone_id;
	std::vector<MINIMAP_NPC>			m_npc;
	std::vector<MINIMAP_SHRINE>			m_shrine;
	std::vector<POINT>					m_Block;
	std::vector<MINIMAP_NPC>				m_Flag;

	std::string m_zone_name;
	int									m_surface_w, m_surface_h;
	int									m_map_x, m_map_y, m_map_w, m_map_h, m_zone_id;
	int									m_board_x, m_board_y;

	//timer
	DWORD						m_dw_minimap_prev_tickcount;
	DWORD						m_dw_minimap_millisec;

	bool	TimerMinimap();

	enum MINIMAP_SPK_INDEX
	{
		MINIMAP_MAIN,
		MINIMAP_MAIN_ALPHA,
		MINIMAP_RIGHT,
		MINIMAP_BOARD,
		MINIMAP_ICON_SELF,
		MINIMAP_ICON_PARTY,
	};

	enum EXEC_ID
	{
		ALPHA_ID,
		PUSHPIN_ID,
	};

public:
	void SetBlock(int x,int y);
	C_VS_UI_WORLDMAP();
	~C_VS_UI_WORLDMAP();

// 	static	int GetZoneNumber(int zone_id);
// 	static	char * GetZoneName(int zone_id);

	int		GetZoneID()							{ return m_zone_id; }
	void	SetZone(int zone_id);
	void	SetSize(SIZE size) { m_map_w = size.cx; m_map_h = size.cy; }
	void	SetPortal(RECT rect, int id);
	void	SetNPC(MINIMAP_NPC npc);
	void	ClearNPC() { m_npc.clear(); }
	void	SetSafetyZone(RECT rect, bool my_zone);
	void	SetFlagArea(POINT pt);
	void	SetXY(int x, int y)	{ m_map_x = x; m_map_y = y; }
	int		GetX()	{ return m_map_x; }
	int		GetY()	{ return m_map_y; }

	void	TogglePushPin() { Run(PUSHPIN_ID); }

	void	UnacquireMouseFocus() 
	{
		m_pC_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();
	}
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	MouseControlExtra(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
};
////////////////////////////////////////////////////////////////////////// end ->
#define MARK_MAX 6
extern char g_mark[MARK_MAX][9][3];

#define BLANK_TIMER 500
//-----------------------------------------------------------------------------
// class C_VS_UI_QUEST_STATUS
//
//
//-----------------------------------------------------------------------------
class C_VS_UI_QUEST_STATUS : public Window, public Exec, public ButtonVisual
{
public :
	struct QUEST_STATUS
	{
		char*			Title;
		DWORD			QuestID;
		DWORD			current_point;
		DWORD			quest_time;
	};
	
	QUEST_STATUS		m_quest_status;					// 현재 진행중인 퀘스트 정보
	bool				m_bl_active;
	bool				m_bl_timeover;
	bool				m_bl_focus;
	
	DWORD				m_timer;
	DWORD				m_timer2;
	CSpriteSurface *	m_p_back_surface;
	std::vector<std::string>					m_hard_cording;		// 날 막아줘!!!


private :

	ButtonGroup			*m_pC_button_group;

	enum EXEC_ID
	{
		PUSHPIN_ID,
		ALPHA_ID,
		CLOSE_ID,
		HELP_ID,
		
		DETAIL_ID,
	};

public :
	void IncreaseQuestPoint();

	C_VS_UI_QUEST_STATUS();
	~C_VS_UI_QUEST_STATUS();
	
	
	DWORD	GetQuestStatusID() { return m_quest_status.QuestID;}
	void	SetQuestStatusInit();
	void	SetQuestStatus(QUEST_STATUS &status);
	void	ToggleWindow();
	void	ShowQuestDescription(int _x, int _y);

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	std::string		GetDetailInfo();

	bool	IsPixel(int _x,int _y);
	int		Timer(int time = -1);
	int		Timer2(int time = -1);

	void	ShowDesc(int strX,int strY,const char *str);

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
};


//class C_VS_UI_FRIEND_INFO;	//add by viva : friend system
class C_VS_UI_TRIBE : public Window, public HotKey, public Exec, public ButtonVisual
{
protected:
	C_VS_UI_SKILL	*			m_pC_skill;

	C_VS_UI_LEVELUP *			m_pC_level_up;
	Point						m_backup_inventory_xy, m_backup_gear_xy;
	ButtonGroup *				m_pC_common_button_group;
	ButtonGroup *				m_pC_menu_button_group;
	ButtonGroup *				m_pC_guild_button_group;
	ButtonGroup *				m_pC_msg_button_group;
	ButtonGroup *				m_pC_util_button_group;
	ButtonGroup *				m_pC_help_button_group;

	C_VS_UI_HPBAR *				m_pC_hpbar;
	C_VS_UI_EFFECT_STATUS *		m_pC_effect_status;
	C_VS_UI_MINIMAP *			m_pC_minimap;
	// add by Coffee 警속各썹뒈暠
	C_VS_UI_WORLDMAP *          m_pC_worldmap;
	// end 
	C_VS_UI_CHATTING *			m_pC_chatting;
	C_VS_UI_SLAYER_QUICKITEM *	m_pC_quickitem;
	
	C_VS_UI_OUSTERS_QUICKITEM * m_pC_armsband;
	C_VS_UI_QUEST_STATUS *		m_pC_quest_status;

	C_VS_UI_INVENTORY *			m_pC_inventory;
	//add by viva : friend system
	C_VS_UI_FRIEND_INFO*		m_pC_friend;
	//end
	//C_VS_UI_QUEST_STATUS *		m_pC_quest;

	C_SPRITE_PACK *				m_pC_main_spk;
	C_SPRITE_PACK * 			m_pC_sys_button_spk;

	enum MAIN_SPK_INDEX	// by larosel
	{
		MAIN_WINDOW,
		ICON_SUN,
		ICON_MOON,
		BUTTON_SYSTEM,
		BUTTON_SYSTEM_HILIGHTED,
		BUTTON_MEMO,
		BUTTON_WASTE,
		SKILL_EXP_BAR,
		EXP_BACK,
		EXP_BAR,
		
		OUSTERS_EXP_BACK = SKILL_EXP_BAR,
		OUSTERS_EXP_BAR,
	};

	enum SYSTEM_BUTTON_INDEX
	{
		TAB_MENU,
		TAB_MENU_NEW,
		TAB_MENU_HILIGHTED,
		TAB_EXP,
		TAB_EXP_NEW,
		TAB_EXP_HILIGHTED,
		TAB_GUILD,
		TAB_GUILD_NEW,
		TAB_GUILD_HILIGHTED,
		TAB_MSG,
		TAB_MSG_NEW,
		TAB_MSG_HILIGHTED,
		TAB_HELP,
		TAB_HELP_NEW,
		TAB_HELP_HILIGHTED,

		// menu tab
		BUTTON_INVENTORY,
		BUTTON_INVENTORY_HILIGHTED,
		BUTTON_INVENTORY_HILIGHTED_PUSHED,
		BUTTON_GEAR,
		BUTTON_GEAR_HILIGHTED,
		BUTTON_GEAR_HILIGHTED_PUSHED,
		BUTTON_INFO,
		BUTTON_INFO_HILIGHTED,
		BUTTON_INFO_HILIGHTED_PUSHED,
		BUTTON_PARTY,
		BUTTON_PARTY_HILIGHTED,
		BUTTON_PARTY_HILIGHTED_PUSHED,
		BUTTON_QUEST,
		BUTTON_QUEST_HILIGHTED,
		BUTTON_QUEST_HILIGHTED_PUSHED,
		
		// help tab
		BUTTON_HELP,
		BUTTON_HELP_HILIGHTED,
		BUTTON_HELP_HILIGHTED_PUSHED,
		BUTTON_CHAT_HELP,
		BUTTON_CHAT_HELP_HILIGHTED,
		BUTTON_CHAT_HELP_HILIGHTED_PUSHED,
		BUTTON_BATTLE_HELP,
		BUTTON_BATTLE_HELP_HILIGHTED,
		BUTTON_BATTLE_HELP_HILIGHTED_PUSHED,
		BUTTON_SKILL_HELP,
		BUTTON_SKILL_HELP_HILIGHTED,
		BUTTON_SKILL_HELP_HILIGHTED_PUSHED,

		// guild tab
		BUTTON_TEAM_INFO,
		BUTTON_TEAM_INFO_HILIGHTED,
		BUTTON_TEAM_INFO_HILIGHTED_PUSHED,
		BUTTON_TEAM_MEMBER_LIST,
		BUTTON_TEAM_MEMBER_LIST_HILIGHTED,
		BUTTON_TEAM_MEMBER_LIST_HILIGHTED_PUSHED,

		// etc-_- 히승미워 by 쑥갓
		BUTTON_MAIL,					// 메뉴탭에 넣쟈
		BUTTON_MAIL_HILIGHTED,
		BUTTON_MAIL_HILIGHTED_PUSHED,

		BUTTON_SMS,
		BUTTON_SMS_HILIGHTED,
		BUTTON_SMS_HILIGHTED_PUSHED,

		BUTTON_NAMING,
		BUTTON_NAMING_HILIGHTED,
		BUTTON_NAMING_HILIGHTED_PUSHED,

		BUTTON_GUILD_LIST,
		BUTTON_GUILD_LIST_HILIGHTED,
		BUTTON_GUILD_LIST_HILIGHTED_PUSHED,

		BUTTON_WAIT_GUILD_LIST,
		BUTTON_WAIT_GUILD_LIST_HILIGHTED,
		BUTTON_WAIT_GUILD_LIST_HILIGHTED_PUSHED,

		BUTTON_UNION,
		BUTTON_UNION_HILIGHTED,
		BUTTON_UNION_HILIGHTED_PUSHED,
		
		TAB_UTIL,
		TAB_UTIL_NEW,
		TAB_UTIL_HILIGHTED,

		BUTTON_STORE,
		BUTTON_STORE_HILIGHTED,
		BUTTON_STORE_HILIGHTED_PUSHED,

		BUTTON_POWER_JJANG,
		BUTTON_POWER_JJANG_HILIGHTED,
		BUTTON_POWER_JJANG_HILIGHTED_PUSHED,


//modify by viva : canncel notice
		BUTTON_TEAM_COMMAND,
		BUTTON_TEAM_COMMAND_HILIGHTED,
		BUTTON_TEAM_COMMAND_HILIGHTED_PUSHED,
		BUTTON_TEAM_LIST,
		BUTTON_TEAM_LIST_HILIGHTED,
		BUTTON_TEAM_LIST_HILIGHTED_PUSHED,
		BUTTON_TEAM_WAIT_LIST,
		BUTTON_TEAM_WAIT_LIST_HILIGHTED,
		BUTTON_TEAM_WAIT_LIST_HILIGHTED_PUSHED,
		BUTTON_TEAM_UNION,
		BUTTON_TEAM_UNION_HILIGHTED,
		BUTTON_TEAM_UNION_HILIGHTED_PUSHED,
//end
		//add by viva : friend system
		BUTTON_FRIEND,
		BUTTON_FRIEND_HILIGHTED,
		BUTTON_FRIEND_HILIGHTED_PUSHED,
		//end
		
	};

	enum EXEC_INDEX	// by larosel
	{
		// COMMON
		PUSHPIN_ID,
		SYSTEM_ID,
		TAB_MENU_ID,
		TAB_EXP_ID,
		TAB_GUILD_ID,
		TAB_MSG_ID,
		TAB_UTIL_ID,
		TAB_HELP_ID,

		// MENU TAB
		INVENTORY_ID,
		GEAR_ID,
		INFO_ID,
		PARTY_ID,
		QUEST_ID,
		MAIL_ID,
		SMS_ID,
		NAMING_ID,
		
		// TEAM TAB
		TEAM_INFO_ID,
		TEAM_MEMBER_LIST_ID,
//		TEAM_COMMAND_ID, // 길드 명령
		TEAM_LIST_ID, // 길드 리스트
		TEAM_WAIT_LIST_ID, // 대기 길드 리스트
		TEAM_UNION_ID,		// 연합 명령

		// UTIL TAB
		UTIL_STORE_ID,
		UTIL_POWER_JJANG_ID,

		
		// HELP TAB
		HELP_ID,
		CHAT_HELP_ID,
		BATTLE_HELP_ID,
		SKILL_HELP_ID,
		GUILD_HELP_ID,

		//add by viva : friend system
		FRIEND_ID,
		//end
		EXEC_MAX
	};


	std::string m_time;
	std::string m_date;

	int							m_selected_tab;

//	bool						m_bl_help, m_bl_party;

	//timer
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	bool	Timer();

public:
	void	MouseControlExtra(UINT message, int _x, int _y) 
	{
		if(m_pC_minimap != NULL)
		{
			m_pC_minimap->IsInRect(_x, _y);
			m_pC_minimap->MouseControlExtra(message, _x, _y); 
		}
	}

	//지뢰설치
	void	StartInstallMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_inventory)m_pC_inventory->StartInstallMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsInstallMineProgress()		{ return gbl_mine_progress; }
	void	EndInstallMineProgress()	{ gbl_mine_progress = false; }
	//지뢰만들기
	void	StartCreateMineProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_inventory)m_pC_inventory->StartCreateMineProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateMineProgress()		{ return gbl_mine_progress; }
	void	EndCreateMineProgress()	{ gbl_mine_progress = false; }
	//폭탄 만들기
	void	StartCreateBombProgress(int focus_grid_x, int focus_grid_y)
	{ if(m_pC_inventory)m_pC_inventory->StartCreateBombProgress(focus_grid_x, focus_grid_y); }
	bool	IsCreateBombProgress()		{ return gbl_mine_progress; }
	void	EndCreateBombProgress()	{ gbl_mine_progress = false; }
	int		GetSelectedTab() { return m_selected_tab; }
	void	SetSelectTab(int tab);

	void	SetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, ACTIONINFO id)
	{
		GetSkillWindow()->SetHotkey(hotkey, id);
	}

	void	ResetSkillSet()
	{
		if(m_pC_skill!=NULL)
			m_pC_skill->ResetSkillSet();
	}

	int	GetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, C_VS_UI_SKILL::HOTKEY_GRADE grade) const
	{
		return GetSkillWindow()->GetHotkey(hotkey, grade);
	}

	void	SetTime(const char *str)			{ m_time = str; }
	void	SetDate(const char *str)			{ m_date = str; }
	void	ToggleMinimap()						{ m_pC_minimap->TogglePushPin(); }

	virtual void	RunQuickItemSlot() = 0;
	virtual void	CloseQuickItemSlot() = 0;
	virtual bool	IsRunningQuickItemSlot() = 0;
	void	RunMinimap() {  }
	void	CloseMinimap() {  }
	bool	IsRunningMinimap() { return false; }
	void	RunMark()		{  }
	void	CloseMark()		{  }
	bool	IsRunningMark()	{ return false; }
	void	SetWhisperID(char *id)	{ if(m_pC_chatting)m_pC_chatting->SetWhisperID(id); }

	bool	IsRunningQuestStatusWindow() { if(m_pC_quest_status) return m_pC_quest_status->Running(); return false;}
	void	OpenQuestStatusWindow() { if(!m_pC_quest_status->Running()) m_pC_quest_status->Start(); }
	void	CloseQuestStatusWindow() { if(m_pC_quest_status->Running()) m_pC_quest_status->Finish(); }
	void	SetQuestStatusWindow(C_VS_UI_QUEST_STATUS::QUEST_STATUS& status)	{ if(m_pC_quest_status) m_pC_quest_status->SetQuestStatus( status ); }
	DWORD	GetQuestStatusID()	{ 
		if(m_pC_quest_status) return m_pC_quest_status->GetQuestStatusID( ); 
		else return 0xffffffff;
	}
	void	UnSetQuestStatusWindow() { if(m_pC_quest_status) m_pC_quest_status->SetQuestStatusInit(); }

	void	LevelUp()
	{
		// 이미 떠 있으면 아무것도 안한다.
		// 닫는 것은 closing interface로...

		if (!m_pC_level_up)
		{
			m_pC_level_up = new C_VS_UI_LEVELUP;
			m_pC_level_up->Start();
		}
	}

	bool	IsRunningLevelUp()
	{
		return (m_pC_level_up != NULL);
	}

	void	FinishLevelUp()
	{
		DeleteNew(m_pC_level_up);
	}

	void	AcquireChatting()
	{
		m_pC_chatting->Acquire();
	}

	bool	IsEmptyChatting()
	{
		return m_pC_chatting->IsEmpty();
	}

	virtual void	DoCommonActionBeforeEventOccured(); // 발생하기 전
	virtual void	DoCommonActionAfterEventOccured()  // 발생한 후
	{
		CloseInventory();
		CloseGear();

//		InitWindowToggleFlag();
		GetInventoryWindow()->AttrWindowMove(true);
		GetGearWindow()->AttrWindowMove(true);

		// restore xy
		InventoryXY(m_backup_inventory_xy.x, m_backup_inventory_xy.y);
		GearXY(m_backup_gear_xy.x, m_backup_gear_xy.y);

		gpC_base->FinishEvent();
	}

	void	OpenInventoryToSell();
	void	FinishItemSelling();

	void	OpenInventoryToRepair();
	void	FinishItemRepairing();

	void	OpenInventoryToSilvering();
	void	FinishItemSilvering();

	void	InventoryXY(int _x, int _y)
	{
		GetInventoryWindow()->XY(_x, _y);
		GetInventoryWindow()->ResetRect();
	}

	void	GearXY(int _x, int _y)
	{
		GetGearWindow()->XY(_x, _y);
	}

	void	ShopRunningAnnounced(); // Shop을 위한 필요한 Window를 띄운다.
	
	void	StorageRunningAnnounced();
	
	void	GetItemBoxRunningAnnounced();
	
	void	ExchangeRunningAnnounced();
	
	void	SetChattingInterface(C_VS_UI_CHATTING * p_chatting);

	virtual void	ResetSlayerQuickItemSize() {}
	virtual void	ResetOustersQuickItemSize() {}
	virtual void	OpenGear(bool bl_set_load = true)=0;
	virtual void	CloseGear()=0;
	virtual C_VS_UI_GEAR	* GetGearWindow() const=0;

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);

	C_VS_UI_TRIBE();
	virtual ~C_VS_UI_TRIBE();

	void	Start();

	void Finish()
	{
		PI_Processor::Finish();
	
		gpC_window_manager->DisappearWindow(this);
	}

	void	Process();

	void	AcquireDisappear() {}

	virtual void CancelPushState()
	{
		m_pC_common_button_group->CancelPushState();
		m_pC_menu_button_group->CancelPushState();
		m_pC_guild_button_group->CancelPushState();
		m_pC_msg_button_group->CancelPushState();
		m_pC_util_button_group->CancelPushState();
		m_pC_help_button_group->CancelPushState();
	}

	virtual void UnacquireMouseFocus()
	{
		m_pC_common_button_group->UnacquireMouseFocus();
		m_pC_menu_button_group->UnacquireMouseFocus();
		m_pC_guild_button_group->UnacquireMouseFocus();
		m_pC_msg_button_group->UnacquireMouseFocus();
		m_pC_util_button_group->UnacquireMouseFocus();
		m_pC_help_button_group->UnacquireMouseFocus();
	}

	void AcquireFirstSequence()
	{

	}

	Point GetInventoryPosition() const
	{
		Point point(GetInventoryWindow()->x, GetInventoryWindow()->y);

		return point;
	}

	Point	GetInventoryGridPosition(int grid_x, int grid_y) const
	{
		Point point(GetInventoryWindow()->x + GetInventoryWindow()->GetGridRect().x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*grid_x, 
						GetInventoryWindow()->y + GetInventoryWindow()->GetGridRect().y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*grid_y); 

		return point;
	}

	void	Run(id_t id);
	bool	IsPixel(int _x, int _y);
	C_VS_UI_SKILL * GetSkillWindow() const { return m_pC_skill; }
	C_VS_UI_INVENTORY * GetInventoryWindow() const { return m_pC_inventory; }
	void	CloseInventory() { m_pC_inventory->Finish(); }
	void	OpenInventory(bool bl_set_load = true) { if( m_pC_inventory != NULL ) m_pC_inventory->Start(bl_set_load); }

	void	CloseFriend()	{ m_pC_friend->Finish(); }			//add by viva
	void	OpenFriend(bool bl_set_load = true)	{ if(m_pC_friend != NULL) m_pC_friend->Start(bl_set_load); }	//add by viva

	void	WindowEventReceiver(id_t event);
	bool	MouseControl(UINT message, int _x, int _y);

	void	SetZone(int zone_id)				{ m_pC_minimap->SetZone(zone_id); }
	int		GetZoneID()							{ return m_pC_minimap->GetZoneID(); }
	void	SetBlock(int x,int y)				{ m_pC_minimap->SetBlock(x,y);}
	void	SetSize(SIZE size)					{ m_pC_minimap->SetSize(size); }
	void	SetXY(int x, int y)					{ m_pC_minimap->SetXY(x, y); }
	int		GetX()								{ return m_pC_minimap->GetX(); }
	int		GetY()								{ return m_pC_minimap->GetY(); }
	void	SetSafetyZone(RECT rect, bool my_zone)	{ m_pC_minimap->SetSafetyZone(rect, my_zone); }
	void	SetNPC(MINIMAP_NPC npc)					{ m_pC_minimap->SetNPC(npc); }
	void	ClearNPC()								{ m_pC_minimap->ClearNPC(); }
	void	SetPortal(RECT rect, int id)			{ m_pC_minimap->SetPortal(rect, id); }


	bool	CloseInventoryGearWindow();
	bool	GetGearOpenState() const;
	bool	GetInventoryOpenState() const;
	
	bool	GetFriendOpenState() const;	//add by viva

	void	UnselectSkill() { GetSkillWindow()->UnselectSkill(); }
	void	ChangeSkillWheel(bool direct) { GetSkillWindow()->ChangeSkillWheel(direct); }
	void	SelectSkill(int id)	{ GetSkillWindow()->SelectSkill(id); }

	void	HotKey_Party();
	void	HotKey_Mark();
	void	HotKey_Help();
	void	HotKey_SkillInfo();
	void	HotKey_Grade2Info();
	void	HotKey_Grade1Info();
	void	HotKey_Minimap();
	void	HotKey_CharInfo();
//	void	HotKey_WindowToggle();
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void	HotKey_Inventory(bool IsCheckSubInventory = false);
	#else
		void	HotKey_Inventory();	
	#endif
	
	void	HotKey_Friend();		//add by viva
	void	HotKey_Gear();
	void	HotKey_Skill();
	void	HotKey_ExtendChat();		// ctrl + e
	void	HotKey_Chat();			// ctrl + c
	void	HotKey_ZoneChat();		// ctrl + z
	void	HotKey_GuildChat();		// ctrl + g
	void	HotKey_UnionChat();		// ctrl + d
	void	HotKey_Whisper();		// ctrl + w
	void	HotKey_F9();	
	void	HotKey_F10();
	void	HotKey_F11();
	void	HotKey_F12();
	void	HotKey_PartyChat();	// ctrl + a
	void	HotKey_Quest();		// ctrl + q
	void	HotKey_MailBox();		// ctrl + b
	void	HotKey_PetInfo();		// ctrl + CapsLock
	void    HotKey_SummonPet();		// ctrl + space
	void	HotKey_SMSWindow();
	void	HotKey_NamingWindow();

	// 2005, 1, 3, sobeit add start
	void	OpenInventoryToSwapAdvanceItem();
	void	FinishSwapAdvancementItem();
	// 2005, 1, 3, sobeit add end
};

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_INFO : public Window, public Exec, public ButtonVisual
{
public :
	
	enum INFO_MODE
	{
		CHARACTER_INFO_MODE = 0,
		SKILL_INFO_MODE,
		GRADE1_INFO_MODE,
		GRADE2_INFO_MODE,
		GRADE3_INFO_MODE,
	};

	static int				m_selected_grade_skill;
	static int				m_selected_skill_domain;
	static int				m_ousters_Magic;

private:
	struct PETINFO			*m_pPetInfo;
	C_SPRITE_PACK			m_petButtonSpk;

	C_VS_UI_DIALOG			*m_pC_learn_grade_skill_confirm;
	Rect					m_rt_tab;

	int						m_focused_skill_id;
	//bool					m_skill_info_mode;
	INFO_MODE				m_info_mode;
	int						m_iDomain;
	int						m_iGrade;
	int						m_draw_grade_skill_mark;
	int						m_advance_skill_count;
	int						m_modify_wide;
	
	std::vector<int>				m_grade_skill_id;
	
	SKILLDOMAIN				m_skill_domain;

	
	ButtonGroup *			m_pC_common_button_group;
	ButtonGroup *			m_pC_char_button_group;
	ButtonGroup *			m_pC_skill_button_group;
	ButtonGroup *			m_pC_grade1_button_group;	
	ButtonGroup *			m_pC_grade2_button_group;	
	ButtonGroup *			m_pC_grade3_button_group;	
	C_VS_UI_SCROLL_BAR *	m_pC_char_scroll_bar;
	C_VS_UI_SCROLL_BAR *	m_pC_skill_scroll_bar;
	C_VS_UI_SCROLL_BAR *	m_pC_skill_scroll_bar_width;
	C_VS_UI_SCROLL_BAR *	m_pC_grade1_scroll_bar;
	C_VS_UI_SCROLL_BAR *	m_pC_grade3_scroll_bar;
	CSprite					*m_p_guild_mark;

	RECT					m_rcSkillDesciption;

	POINT					m_CenterPos;
	bool					m_bOustersDownSkill;

	enum EXEC_ID
	{
		ALPHA_ID,
		CLOSE_ID,
		HELP_ID,

		// SLAYER SKILL INFO
		BLADE_ID,
		SWORD_ID,
		GUN_ID,
		HEAL_ID,
		ENCHANT_ID,
		TOTAL_ID,

		// VAMPIRE SKILL INFO
		POISON_ID,
		ACID_ID,
		CURSE_ID,
		SUMMON_ID,
		BLOOD_ID,
		INNATE_ID,

		// OUSTERS SKILL INFO
		COMBAT_ID,
		ELEMENTAL_ID,

		// VAMPIRE CHARACTER INFO
		STR_PLUS_ID,
		DEX_PLUS_ID,
		INT_PLUS_ID,
		
		PIVATE_ID,
		SERENT_ID,
		FEACEL_ID,
		LITENA_ID,
		KAINEL_ID,
		GENEAL_ID,
		FORE_GENEAL_ID,
		MAJORIS_GENEAL_ID,								
		CLOEL_GENEAL_ID,
		MARSHAL_ID,

		RITTER_ID,
		REICHSRITTER_ID,
		BARONET_ID,
		PREYHER_ID,
		GRAF_ID,
		MARKGRAF_ID,
		PFALZGRAF_ID,
		FURST_ID,								
		HERZOG_ID,
		LANDESHER_ID,

		MALCHUT_ID,					// 마르쿠트
		YESOD_ID,						// 예소드
		HOD_ID,						// 호드
		NETRETH_ID,					// 네차크
		TIPHRETH_ID,					// 티파레트
		GEBURAH_ID,
		CHESED_ID,
		BINSH_ID,								
		CHOKMA_ID,
		KEATHER_ID,
		ADVENCEMENT_ID,
		// GRADE1 BUTTON
		GRADE1_BUTTON1_ID,
		GRADE1_BUTTON2_ID,
		GRADE1_BUTTON3_ID,
		GRADE1_BUTTON4_ID,
		GRADE1_BUTTON5_ID,
		GRADE1_BUTTON6_ID,
		GRADE1_BUTTON7_ID,
		GRADE1_BUTTON8_ID,
		GRADE2_BUTTON1_ID,
		GRADE2_BUTTON2_ID,
		GRADE2_BUTTON3_ID,
		GRADE2_BUTTON4_ID,
		GRADE2_BUTTON5_ID,
		GRADE2_BUTTON6_ID,
		GRADE2_BUTTON7_ID,
		GRADE2_BUTTON8_ID,
		GRADE3_BUTTON1_ID,
		GRADE3_BUTTON2_ID,
		GRADE3_BUTTON3_ID,
		GRADE3_BUTTON4_ID,
		GRADE3_BUTTON5_ID,
		GRADE3_BUTTON6_ID,
//		GRADE3_BUTTON_TAB_ID1,
//		GRADE3_BUTTON_TAB_ID2,
//		GRADE3_BUTTON_TAB_ID3,
//		GRADE3_BUTTON_TAB_ID4,
//		GRADE3_BUTTON_TAB_ID5,
//		GRADE3_BUTTON_TAB_ID6,
//		GRADE3_BUTTON_TAB_ID7,
//		GRADE3_BUTTON_TAB_ID8,
//		GRADE3_BUTTON_TAB_ID9,
//		GRADE3_BUTTON_TAB_ID10,
		CHANGE_IMAGE_ID,		// 이미지 변경 버튼
		PET_INFO_ID,
		
		// grade 2
			
		
		
	
		
	};

	enum GRADESKILL_STATUS
	{
		DISABLE,
		CAN_LEARN,
		LEARNED,
	};


	Rect					m_rt_char_box;
	CSpritePack *			m_p_face;

public:
	
	

	C_VS_UI_INFO();
	~C_VS_UI_INFO();

	void	RefreshImage();

	void	UnacquireMouseFocus()
	{
//		m_pC_button_group->UnacquireMouseFocus();
		m_pC_common_button_group->UnacquireMouseFocus();
		m_pC_char_button_group->UnacquireMouseFocus();
		m_pC_skill_button_group->UnacquireMouseFocus();
		m_pC_grade1_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
//		m_pC_button_group->CancelPushState();
		m_pC_common_button_group->CancelPushState();
		m_pC_char_button_group->CancelPushState();
		m_pC_skill_button_group->CancelPushState();
		m_pC_grade1_button_group->CancelPushState();
	}

	void	SetPetInfo(struct PETINFO *pPetInfo);
	struct PETINFO* GetPetInfo();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireDisappear() {}
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);	
	bool	CharacterInfoMouseControl(UINT message, int _x, int _y);
	bool	SkillInfoMouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	CheckGradeSkillIDList();
	
	void	Show();
	void	_Show1();	// Skill Info
	void	_Show2();	// Character Info
	void	_Show3();	// Grade1
	void	_Show4();	// Grade2
	//by csm 2004.12.30 전직 관련 인터페이스 추가 
	void	_Show5();	// Grade2
	void	Process();
//	void	Start(bool skill_mode);
	void	Start(INFO_MODE	Info_Mode);
	void	Finish();

	bool	IsCharacterMode()	{ return m_info_mode == CHARACTER_INFO_MODE; }
	bool	IsSkillMode()	{ return m_info_mode == SKILL_INFO_MODE; }
	bool	IsGrade1Mode()	{ return m_info_mode == GRADE1_INFO_MODE; }
	bool	IsGrade2Mode()	{ return m_info_mode == GRADE2_INFO_MODE; }
	bool	IsGrade3Mode()	{ return m_info_mode == GRADE3_INFO_MODE; }

	void	SetOustersDownSkill() { m_bOustersDownSkill = true; }
	void	SetCorrectGrade();
	const char * GetChinhoLevel(const int level);
};

class C_VS_UI_WINDOW_MANAGER
{
public:
	enum WINDOW_ID
	{
		HPBAR,
		MINIMAP,
		MAINMENU,
		CHATTING,
		CHATTING_OLD,
		INVENTORY,
		GEAR,
		INFO,
		QUICKITEMSLOT,
		PARTY,
		EFFECT_STATUS,
		OTHER_INFO,
		TRACE_WINDOW,
		BLOOD_BIBLE_WINDOW,
		QUEST_STATUS_WINDOW,
		
		CTF_STATUS,
		PET_INFO,

		NAMING_WINDOW,

		QUEST_MANAGER_LIST, // 메인 창
		QUEST_MANAGER_DETAIL, // 세부사항
		QUEST_MANAGER_MISSION, // 임무
		QUEST_MANAGER_ITEM, // 인벤토리
		INVENTORY_SUB,
		FRIEND_WINDOW,//add by viva
		WINDOW_TOTAL,
	};

	C_VS_UI_WINDOW_MANAGER();
	~C_VS_UI_WINDOW_MANAGER();

	void SetFilter(CHAT_LINE_CONDITION filter, bool set);
	bool GetFilter(CHAT_LINE_CONDITION filter) { return m_filter[filter]; }

	void SetAlpha(WINDOW_ID id, bool bl_alpha);
	bool IsAlpha(WINDOW_ID id) { return m_alpha[id]; }

	void SetAutoHide(WINDOW_ID id, Window::ATTRIBUTES_HIDE hide);
	Window::ATTRIBUTES_HIDE GetAutoHide(WINDOW_ID id) { return m_autohide[id]; }

	void SetRect(WINDOW_ID id, Rect rect);
	Rect& GetRect(WINDOW_ID id) { return m_rect[id]; }

	void SetHPBarSmall(bool bl_small) { m_bl_hpbar_small = bl_small; }
	bool IsHPBarSmall() { return m_bl_hpbar_small; }

	void SetHPBarHeight(bool bl_height) { m_bl_hpbar_height = bl_height; }
	bool IsHPBarHeight() { return m_bl_hpbar_height; }

	void SetQuickItemSlotHeight(bool bl_height) { m_bl_quickitemslot_height = bl_height; }
	bool IsQuickItemSlotHeight() { return m_bl_quickitemslot_height; }

	void SetEffectStatusHeight(bool bl_height) { m_bl_effectstatus_height = bl_height; }
	bool IsEffectStatusHeight() { return m_bl_effectstatus_height; }

	void SetMainTab(int index) { m_i_main_tab = index; }
	int	 GetMainTab() { return m_i_main_tab; }

	void SaveToFile(class ofstream &file);
	void LoadFromFile(class ifstream &file);

	void SetDefault();
	void SetHotKey(int **hotkey = NULL);

	int** GetHotKey()	{ return (int **)m_skill_hotkey_buf; }

	void SetSMSMynum(char* str);
	char* GetSMSMyNum() {return &m_SMS_MyNum[0];}

private:
	Rect m_rect[WINDOW_TOTAL];
	Window::ATTRIBUTES_HIDE m_autohide[WINDOW_TOTAL];
	bool m_alpha[WINDOW_TOTAL];

	bool m_filter[CLD_TOTAL];
	int	 m_skill_hotkey_buf[C_VS_UI_SKILL::HOTKEY_MAX][C_VS_UI_SKILL::GRADE_MAX];

	bool m_bl_hpbar_small, m_bl_hpbar_height, m_bl_quickitemslot_height, m_bl_effectstatus_height;
	int m_i_main_tab;

	char m_SMS_MyNum[12];
};


class C_VS_UI_TEAM_LIST : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_spk;
	enum GUILD_SPK
	{
		LIST_WINDOW = 0,
		INFO_WINDOW,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		X_BUTTON,
		X_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		BAR_DARKEN,
		BAR_HILIGHTED,
		BAR_SEARCH,
		TITLE_TEAM_REGISTRATION,
		TITLE_MEMBER_REGISTRATION,
		TITLE_TEAM_INFO,
		TITLE_MEMBER_INFO,
		TITLE_TEAM_LIST,
		TITLE_TEAM_LIST_HILIGHTED,

		SEARCH_NAME,
		SEARCH_NAME_HILIGHTED,
		SEARCH_NAME_HILIGHTED_PUSHED,
		SEARCH_LEADER,
		SEARCH_LEADER_HILIGHTED,
		SEARCH_LEADER_HILIGHTED_PUSHED,
		SEARCH_ALL,
		SEARCH_ALL_HILIGHTED,
		SEARCH_ALL_HILIGHTED_PUSHED,
		SEARCH_SEARCH,
		SEARCH_SEARCH_HILIGHTED,
		SEARCH_SEARCH_HILIGHTED_PUSHED,

		REGISTER_BUTTON,
		REGISTER_BUTTON_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		JOIN_BUTTON,
		JOIN_BUTTON_HILIGHTED,
		EXPEL_BUTTON,
		EXPEL_BUTTON_HILIGHTED,
		ACCEPT_BUTTON,
		ACCEPT_BUTTON_HILIGHTED,
		DENY_BUTTON,
		DENY_BUTTON_HILIGHTED,		
		MODIFY_BUTTON,
		MODIFY_BUTTON_HILIGHTED,		

		INDEX_TEAM,
		INDEX_TEAM_HILIGHTED,
		INDEX_LEADER,
		INDEX_LEADER_HILIGHTED,
		INDEX_EXPIREDATE,
		INDEX_EXPIREDATE_HILIGHTED,
		INDEX_MEMBERS,
		INDEX_MEMBERS_HILIGHTED,
		INDEX_RANKING,
		INDEX_RANKING_HILIGHTED,

		TITLE_WAR_LIST,
		TITLE_WAR_LIST_HILIGHTED,
		INDEX_WAR_CHALLENGER,
		INDEX_WAR_CHALLENGER_HILIGHTED,
		INDEX_WAR_DATE,
		INDEX_WAR_DATE_HILIGHTED,

		INDEX_UNITED_JOIN,				// 연합가입
		INDEX_UNITED_JOIN_HILIGHTED,	
		INDEX_UNITED_WITHDRAWAL,		// 연합탈퇴
		INDEX_UNITED_WITHDRAWAL_HILIGHTED,
		INDEX_UNITED_EXPEL,				// 연합추방
		INDEX_UNITED_EXPEL_HILIGHTED,
		INDEX_GUILD_APPOINT,			// 임명
		INDEX_GUILD_APPOINT_HILIGHTED,


	};

	enum EXEC_ID
	{
		HELP_ID,
		X_ID,
		CLOSE_ID,
		REGISTER_ID,

		SCROLL_UP_ID,
		SCROLL_DOWN_ID,

		TEAM_LIST_ID,

		INDEX_TEAM_ID,
		INDEX_LEADER_ID,
		INDEX_EXPIREDATE_ID,
		INDEX_MEMBERS_ID,
		INDEX_RANKING_ID,

		SEARCH_TYPE_ID,
		SEARCH_ID,


		EXEC_MAX,
	};
public:
	struct TEAM_LIST_BASE
	{
		std::string TEAM_NAME, LEADER_NAME;
		int	guild_id;
		int MEMBERS;
	};

	struct REGIST_TEAM_LIST:public TEAM_LIST_BASE
	{
		int RANKING;
	};
	
	struct READY_TEAM_LIST:public TEAM_LIST_BASE
	{
		std::string EXPIRE_DATE;
		int MEMBERS_MAX;
	};

private:
	POINT	m_vampire_point;
	bool	m_bl_ready;
	int		m_scroll;
	bool	m_bl_scrolling;

	int		m_search_type;
	int		m_print_x[4], m_print_y, m_print_gap;

	int		m_iFocus;

	LineEditorVisual			m_lev_search;

	std::vector<READY_TEAM_LIST>	m_v_ready_team_search_list;
	std::vector<READY_TEAM_LIST>	m_v_ready_team_list;
	std::vector<REGIST_TEAM_LIST>	m_v_regist_team_search_list;
	std::vector<REGIST_TEAM_LIST>	m_v_regist_team_list;

	int		m_last_sort_type;

	bool	m_bIsUnion;
public:
	C_VS_UI_TEAM_LIST(bool ready, bool IsUnion = false);
	~C_VS_UI_TEAM_LIST();

	void	AddReadyTeamList(const READY_TEAM_LIST &team_info);
	void	AddRegistTeamList(const REGIST_TEAM_LIST &team_info);

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	bool	AllMatchWindowPixel(int _x, int _y) const;
	bool	IsUnionInfo() {return m_bIsUnion;}

	void	Start();
	void	Finish();

	void	Show();

};

class C_VS_UI_TEAM_MEMBER_LIST : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_member_list_spk;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;
	
	bool						m_SortFlag_Id;
	bool						m_SortFlag_Server;
	bool						m_SortFlag_Grade;
	BYTE						m_ButtonFlag[3];
	enum TEAM_MEMBER_SPK
	{
		BUTTON_CLOSE,
		BUTTON_CLOSE_HILIGHTED,
		BUTTON_CLOSE_HILIGHTED_PUSHED,
		IMAGE_NULL,
	};

	enum EXEC_ID
	{
		CLOSE_ID,
		SORT_ID,
		SORT_SERVER,
		SORT_GRADE,
		EXEC_MAX,
	};
public:
	struct TEAM_MEMBER_LIST
	{
		std::string MEMBER_NAME;
		std::string SERVER_NAME;
		int member_grade;
//		int server_type;
		bool bLogOn;
	};

private:
	int		m_print_x[2], m_print_y, m_print_gap;
	int		m_focus;
	std::vector<TEAM_MEMBER_LIST>	m_v_member_list;
	BYTE	m_bAvailableRecall; // 타입에 따라 길원 호출하기 
	char	m_SelectedID[16];
	char	m_AskingMessage[128];
public:
	C_VS_UI_TEAM_MEMBER_LIST();
	~C_VS_UI_TEAM_MEMBER_LIST();

	void	AddMemberList(const TEAM_MEMBER_LIST &member_list, BYTE bType);

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();
	void	_Sort(int nSort);
	BYTE	GetAvailableRecall() {return m_bAvailableRecall;}
	void	SetAvailableRecall(BYTE bFlag) {m_bAvailableRecall = bFlag;}
	char*	GetSelectedID() {return m_SelectedID;}
};

class C_VS_UI_NPC_DIALOG;
class C_VS_UI_TEAM_INFO : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_spk;
	enum GUILD_SPK
	{
		LIST_WINDOW = 0,
		INFO_WINDOW,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		X_BUTTON,
		X_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		BAR_DARKEN,
		BAR_HILIGHTED,
		BAR_SEARCH,
		TITLE_TEAM_REGISTRATION,
		TITLE_MEMBER_REGISTRATION,
		TITLE_TEAM_INFO,
		TITLE_MEMBER_INFO,
		TITLE_TEAM_LIST,
		TITLE_TEAM_LIST_HILIGHTED,

		SEARCH_NAME,
		SEARCH_NAME_HILIGHTED,
		SEARCH_NAME_HILIGHTED_PUSHED,
		SEARCH_LEADER,
		SEARCH_LEADER_HILIGHTED,
		SEARCH_LEADER_HILIGHTED_PUSHED,
		SEARCH_ALL,
		SEARCH_ALL_HILIGHTED,
		SEARCH_ALL_HILIGHTED_PUSHED,
		SEARCH_SEARCH,
		SEARCH_SEARCH_HILIGHTED,
		SEARCH_SEARCH_HILIGHTED_PUSHED,

		REGISTER_BUTTON,
		REGISTER_BUTTON_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		JOIN_BUTTON,
		JOIN_BUTTON_HILIGHTED,
		EXPEL_BUTTON,
		EXPEL_BUTTON_HILIGHTED,
		ACCEPT_BUTTON,
		ACCEPT_BUTTON_HILIGHTED,
		DENY_BUTTON,
		DENY_BUTTON_HILIGHTED,		
		MODIFY_BUTTON,
		MODIFY_BUTTON_HILIGHTED,		

		INDEX_TEAM,
		INDEX_TEAM_HILIGHTED,
		INDEX_LEADER,
		INDEX_LEADER_HILIGHTED,
		INDEX_EXPIREDATE,
		INDEX_EXPIREDATE_HILIGHTED,
		INDEX_MEMBERS,
		INDEX_MEMBERS_HILIGHTED,
		INDEX_RANKING,
		INDEX_RANKING_HILIGHTED,

		TITLE_WAR_LIST,
		TITLE_WAR_LIST_HILIGHTED,
		INDEX_WAR_CHALLENGER,
		INDEX_WAR_CHALLENGER_HILIGHTED,
		INDEX_WAR_DATE,
		INDEX_WAR_DATE_HILIGHTED,

		INDEX_UNITED_JOIN,				// 연합가입
		INDEX_UNITED_JOIN_HILIGHTED,	
		INDEX_UNITED_WITHDRAWAL,		// 연합탈퇴
		INDEX_UNITED_WITHDRAWAL_HILIGHTED,
		INDEX_UNITED_EXPEL,				// 연합추방
		INDEX_UNITED_EXPEL_HILIGHTED,
		INDEX_GUILD_APPOINT,			// 임명
		INDEX_GUILD_APPOINT_HILIGHTED,


	};
	enum EXEC_ID
	{
		HELP_ID,
		X_ID,
		CLOSE_ID,
		JOIN_ID,

		SCROLL_UP_ID,
		SCROLL_DOWN_ID,

		MODIFY_ID,
		UNITED_JOIN_ID,
		UNITED_WITHDRAWAL_ID,
		UNUTED_EXPEL_ID,
		
		EXEC_MAX,
	};
public:
	struct TEAM_INFO_BASE
	{
		std::string TEAM_NAME, LEADER_NAME, INTRODUCTION;
		int guild_id, MEMBERS, REG_FEE;
	};

	struct REGIST_TEAM_INFO:public TEAM_INFO_BASE
	{
		std::string REGISTERED_DATE;
		int RANKING;
	};
	
	struct READY_TEAM_INFO:public TEAM_INFO_BASE
	{
		std::string EXPIRE_DATE;
		std::vector<std::string> MEMBERS_NAME;
		int MEMBERS_MAX;
	};

private:
	bool	m_master;
	bool	m_bl_ready;
	int		m_scroll;
	int		m_scroll_max;
	bool	m_bl_scrolling;

	static REGIST_TEAM_INFO			m_regist_info;
	READY_TEAM_INFO				m_ready_info;
	C_VS_UI_NPC_DIALOG		*m_pC_dialog ;
	LineEditorVisual			m_lev_intro;

	bool	m_IsUnion;

public:
	C_VS_UI_TEAM_INFO(bool ready, void *info, bool IsUnion = false);
	~C_VS_UI_TEAM_INFO();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	static int		GetGuildId() { return m_regist_info.guild_id; }
	void	CloseTeamInfoDialogBox();

	void	Start();
	void	Finish();
	void	Show();

};

class C_VS_UI_TEAM_MEMBER_INFO : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_member_spk;
	enum GUILD_SPK
	{
		LIST_WINDOW = 0,
		INFO_WINDOW,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		X_BUTTON,
		X_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		BAR_DARKEN,
		BAR_HILIGHTED,
		BAR_SEARCH,
		TITLE_TEAM_REGISTRATION,
		TITLE_MEMBER_REGISTRATION,
		TITLE_TEAM_INFO,
		TITLE_MEMBER_INFO,
		TITLE_TEAM_LIST,
		TITLE_TEAM_LIST_HILIGHTED,

		SEARCH_NAME,
		SEARCH_NAME_HILIGHTED,
		SEARCH_NAME_HILIGHTED_PUSHED,
		SEARCH_LEADER,
		SEARCH_LEADER_HILIGHTED,
		SEARCH_LEADER_HILIGHTED_PUSHED,
		SEARCH_ALLL,
		SEARCH_ALL_HILIGHTED,
		SEARCH_ALL_HILIGHTED_PUSHED,
		SEARCH_SEARCH,
		SEARCH_SEARCH_HILIGHTED,
		SEARCH_SEARCH_HILIGHTED_PUSHED,

		REGISTER_BUTTON,
		REGISTER_BUTTON_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		JOIN_BUTTON,
		JOIN_BUTTON_HILIGHTED,
		EXPEL_BUTTON,
		EXPEL_BUTTON_HILIGHTED,
		ACCEPT_BUTTON,
		ACCEPT_BUTTON_HILIGHTED,
		DENY_BUTTON,
		DENY_BUTTON_HILIGHTED,		
		MODIFY_BUTTON,
		MODIFY_BUTTON_HILIGHTED,		

		INDEX_TEAM,
		INDEX_TEAM_HILIGHTED,
		INDEX_LEADER,
		INDEX_LEADER_HILIGHTED,
		INDEX_EXPIREDATE,
		INDEX_EXPIREDATE_HILIGHTED,
		INDEX_MEMBERS,
		INDEX_MEMBERS_HILIGHTED,
		INDEX_RANKING,
		INDEX_RANKING_HILIGHTED,

		TITLE_WAR_LIST,
		TITLE_WAR_LIST_HILIGHTED,
		INDEX_WAR_CHALLENGER,
		INDEX_WAR_CHALLENGER_HILIGHTED,
		INDEX_WAR_DATE,
		INDEX_WAR_DATE_HILIGHTED,

		INDEX_UNITED_JOIN,				// 연합가입
		INDEX_UNITED_JOIN_HILIGHTED,	
		INDEX_UNITED_WITHDRAWAL,		// 연합탈퇴
		INDEX_UNITED_WITHDRAWAL_HILIGHTED,
		INDEX_UNITED_EXPEL,				// 연합추방
		INDEX_UNITED_EXPEL_HILIGHTED,
		INDEX_GUILD_APPOINT,			// 임명
		INDEX_GUILD_APPOINT_HILIGHTED,

	};

	enum EXEC_ID
	{
		HELP_ID,
		X_ID,
		CLOSE_ID,

		EXPEL_ID,
		ACCEPT_ID,
		DENY_ID,

		SCROLL_UP_ID,
		SCROLL_DOWN_ID,

		MODIFY_ID,

		APPOINT_ID,

		EXEC_MAX,
	};
public:
	struct MEMBER_INFO
	{
		std::string NAME, INTRODUCTION;
		int guild_id;
		BYTE GRADE;
	};

private:
	int		m_scroll;
	int		m_scroll_max;
	bool	m_bl_scrolling;
	bool	m_own_info;

	MEMBER_INFO m_member_info;

	LineEditorVisual			m_lev_intro;

public:
	C_VS_UI_TEAM_MEMBER_INFO(MEMBER_INFO *info);
	~C_VS_UI_TEAM_MEMBER_INFO();

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

class C_VS_UI_TEAM_REGIST : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_spk;
	enum GUILD_SPK
	{
		LIST_WINDOW = 0,
		INFO_WINDOW,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		X_BUTTON,
		X_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		BAR_DARKEN,
		BAR_HILIGHTED,
		BAR_SEARCH,
		TITLE_TEAM_REGISTRATION,
		TITLE_MEMBER_REGISTRATION,
		TITLE_TEAM_INFO,
		TITLE_MEMBER_INFO,
		TITLE_TEAM_LIST,
		TITLE_TEAM_LIST_HILIGHTED,

		SEARCH_NAME,
		SEARCH_NAME_HILIGHTED,
		SEARCH_NAME_HILIGHTED_PUSHED,
		SEARCH_LEADER,
		SEARCH_LEADER_HILIGHTED,
		SEARCH_LEADER_HILIGHTED_PUSHED,
		SEARCH_ALL,
		SEARCH_ALL_HILIGHTED,
		SEARCH_ALL_HILIGHTED_PUSHED,
		SEARCH_SEARCH,
		SEARCH_SEARCH_HILIGHTED,
		SEARCH_SEARCH_HILIGHTED_PUSHED,

		REGISTER_BUTTON,
		REGISTER_BUTTON_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		JOIN_BUTTON,
		JOIN_BUTTON_HILIGHTED,
		EXPEL_BUTTON,
		EXPEL_BUTTON_HILIGHTED,
		ACCEPT_BUTTON,
		ACCEPT_BUTTON_HILIGHTED,
		DENY_BUTTON,
		DENY_BUTTON_HILIGHTED,		
		MODIFY_BUTTON,
		MODIFY_BUTTON_HILIGHTED,		

		INDEX_TEAM,
		INDEX_TEAM_HILIGHTED,
		INDEX_LEADER,
		INDEX_LEADER_HILIGHTED,
		INDEX_EXPIREDATE,
		INDEX_EXPIREDATE_HILIGHTED,
		INDEX_MEMBERS,
		INDEX_MEMBERS_HILIGHTED,
		INDEX_RANKING,
		INDEX_RANKING_HILIGHTED,

		TITLE_WAR_LIST,
		TITLE_WAR_LIST_HILIGHTED,
		INDEX_WAR_CHALLENGER,
		INDEX_WAR_CHALLENGER_HILIGHTED,
		INDEX_WAR_DATE,
		INDEX_WAR_DATE_HILIGHTED,

		INDEX_UNITED_JOIN,				// 연합가입
		INDEX_UNITED_JOIN_HILIGHTED,	
		INDEX_UNITED_WITHDRAWAL,		// 연합탈퇴
		INDEX_UNITED_WITHDRAWAL_HILIGHTED,
		INDEX_UNITED_EXPEL,				// 연합추방
		INDEX_UNITED_EXPEL_HILIGHTED,
		INDEX_GUILD_APPOINT,			// 임명
		INDEX_GUILD_APPOINT_HILIGHTED,


	};

	enum EXEC_ID
	{
		HELP_ID,
		X_ID,
		CLOSE_ID,
		REGIST_ID,

		EXEC_MAX,
	};

private:
	POINT	m_vampire_point;
	bool	m_bl_member;
	int		m_scroll;
	bool	m_bl_scrolling;
	int		m_print_x, m_print_y, m_print_gap;
	int		m_mark_x, m_mark_y;

	LineEditorVisual			m_lev_intro, m_lev_team_name;

	int		m_guild_id;
	int		m_reg_fee, m_rank;
	std::string m_team_name, m_introduction, m_date;
	CDirectDrawSurface				*m_guild_mark;

public:
	C_VS_UI_TEAM_REGIST(bool member, int reg_fee, int rank, char *team_name = NULL, char *date = NULL, int guild_id = -1); // team_name은 member인 경우에만
	~C_VS_UI_TEAM_REGIST();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	bool	ReturnTeamRegistMember(){return m_bl_member;}

	void	Start();
	void	Finish();

	void	Show();

};

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_OTHER_INFO : public Window, public Exec, public ButtonVisual
{
private:
//	C_SPRITE_PACK			*m_pC_info_spk;
	ButtonGroup				*m_pC_button_group;
	CSprite					*m_p_guild_mark;
	//CSprite					*m_p_guild_mark;
	struct PETINFO			*m_pPetInfo;
	C_SPRITE_PACK			m_petButtonSpk;

	enum EXEC_ID
	{
		ALPHA_ID,
		CLOSE_ID,
		HELP_ID,
		PET_INFO_ID,
	};

public:
	struct PLAYER_INFO
	{
		std::string PLAYER_NAME;
		int DOMAINLEVEL[5];
		int GRADE, LEVEL, FAME;
		int ALIGNMENT;
		int STR_CUR, DEX_CUR, INT_CUR;
		int STR_PURE, DEX_PURE, INT_PURE;
		int guild_id;
		int face_type;
		bool bl_female;		
	};
private :

	CSpritePack *				m_p_face;
//	C_SPRITE_PACK				*m_p_face_spk;
	PLAYER_INFO					m_player_info;

	int TopDomain;
	
public :

	C_VS_UI_OTHER_INFO();
	~C_VS_UI_OTHER_INFO();

	void	SetPetInfo(struct PETINFO *pPetInfo);
	void	RefreshImage();

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
	void	SetOtherInfo(PLAYER_INFO &info);
	const char*		GetCurrentOtherInfoName();

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);
	//bool	IsRunningOtherInfo();
};

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_TRACE : public Window, public Exec, public ButtonVisual
{
private:
	bool status;
	C_SPRITE_PACK			*m_pC_trace_spk;
	ButtonGroup				*m_pC_button_group;

	enum EXEC_ID
	{
		OK_ID,
		CLOSE_ID,
		HELP_ID,
	};

	enum TRACE_SPK
	{
		MAIN_WINDOW = 0,
		TRACE_START,
		TRACE_START_HILIGHTED,
		TRACE_START_PUSHED,
		TRACE_CANCEL,
		TRACE_CANCEL_HILIGHTED,
		TRACE_CANCEL_PUSHED,
	};

	LineEditorVisual		m_lev_name;	
	std::string					m_status_msg;
	std::string					name;
public :

	C_VS_UI_TRACE();
	~C_VS_UI_TRACE();

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
	bool	IsStartTrace();
	void	SetCannotTrace();
};

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_XMAS_CARD : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			*m_pC_card_spk;
	ButtonGroup				*m_pC_button_group;
	bool					status;

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
	};

	enum TRACE_SPK
	{
		MAIN_WINDOW = 0,
		BUTTON_OK,
		BUTTON_OK_HILIGHTED,
		BUTTON_OK_PUSHED,
		BUTTON_CANCEL,
		BUTTON_CANCEL_HILIGHTED,
		BUTTON_CANCEL_PUSHED,
	};

//	LineEditorVisual		m_lev_from;	
	LineEditorVisual		m_lev_to;	
	LineEditorVisual		m_lev_message;	

	MItem					*m_pItem;
	std::string				m_szTreeMessage;


public :

	C_VS_UI_XMAS_CARD(const MItem *pItem);
	~C_VS_UI_XMAS_CARD();

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
// class C_VS_UI_BRING_FEE
//
// 세금 가져오기
//
//-----------------------------------------------------------------------------
class C_VS_UI_BRING_FEE : public Window, public Exec, public ButtonVisual
{
private :
	UINT		m_TotalFee;
	UINT		m_BringFee;
	int		m_money_x,m_money_y;

	ButtonGroup			*m_pC_button_group;

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
		MONEY_ID,
	};

public :
//	static	C_VS_UI_MONEY_DIALOG		*m_pC_bring_fee_dialog;
	C_VS_UI_BRING_FEE(UINT totalfee = 0,UINT bringfee = 0);
	~C_VS_UI_BRING_FEE();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);

	void	SetTotalFee(UINT fee) {m_TotalFee = fee; }
	void	SetBringFee(UINT fee) {m_BringFee = fee; }
	UINT		GetTotalFee() {return m_TotalFee;}
	UINT		GetBringFee() {return m_BringFee;}

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
};

class C_VS_UI_WAR_LIST : public Window, public Exec, public ButtonVisual
{
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_guild_spk;
	enum GUILD_SPK
	{
		LIST_WINDOW = 0,
		INFO_WINDOW,
		HELP_BUTTON,
		HELP_BUTTON_PUSHED,
		X_BUTTON,
		X_BUTTON_PUSHED,
		SCROLL_BAR,
		SCROLL_UP_BUTTON,
		SCROLL_UP_BUTTON_PUSHED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_BUTTON_PUSHED,
		SCROLL_TAG,
		BAR_DARKEN,
		BAR_HILIGHTED,
		BAR_SEARCH,
		TITLE_TEAM_REGISTRATION,
		TITLE_MEMBER_REGISTRATION,
		TITLE_TEAM_INFO,
		TITLE_MEMBER_INFO,
		TITLE_TEAM_LIST,
		TITLE_TEAM_LIST_HILIGHTED,

		SEARCH_NAME,
		SEARCH_NAME_HILIGHTED,
		SEARCH_NAME_HILIGHTED_PUSHED,
		SEARCH_LEADER,
		SEARCH_LEADER_HILIGHTED,
		SEARCH_LEADER_HILIGHTED_PUSHED,
		SEARCH_ALL,
		SEARCH_ALL_HILIGHTED,
		SEARCH_ALL_HILIGHTED_PUSHED,
		SEARCH_SEARCH,
		SEARCH_SEARCH_HILIGHTED,
		SEARCH_SEARCH_HILIGHTED_PUSHED,

		REGISTER_BUTTON,
		REGISTER_BUTTON_HILIGHTED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		JOIN_BUTTON,
		JOIN_BUTTON_HILIGHTED,
		EXPEL_BUTTON,
		EXPEL_BUTTON_HILIGHTED,
		ACCEPT_BUTTON,
		ACCEPT_BUTTON_HILIGHTED,
		DENY_BUTTON,
		DENY_BUTTON_HILIGHTED,		
		MODIFY_BUTTON,
		MODIFY_BUTTON_HILIGHTED,		

		INDEX_TEAM,
		INDEX_TEAM_HILIGHTED,
		INDEX_LEADER,
		INDEX_LEADER_HILIGHTED,
		INDEX_EXPIREDATE,
		INDEX_EXPIREDATE_HILIGHTED,
		INDEX_MEMBERS,
		INDEX_MEMBERS_HILIGHTED,
		INDEX_RANKING,
		INDEX_RANKING_HILIGHTED,

		TITLE_WAR_LIST,
		TITLE_WAR_LIST_HILIGHTED,
		INDEX_WAR_CHALLENGER,
		INDEX_WAR_CHALLENGER_HILIGHTED,
		INDEX_WAR_DATE,
		INDEX_WAR_DATE_HILIGHTED,

		INDEX_UNITED_JOIN,				// 연합가입
		INDEX_UNITED_JOIN_HILIGHTED,	
		INDEX_UNITED_WITHDRAWAL,		// 연합탈퇴
		INDEX_UNITED_WITHDRAWAL_HILIGHTED,
		INDEX_UNITED_EXPEL,				// 연합추방
		INDEX_UNITED_EXPEL_HILIGHTED,
		INDEX_GUILD_APPOINT,			// 임명
		INDEX_GUILD_APPOINT_HILIGHTED,


	};
	enum EXEC_ID
	{
		HELP_ID,
		X_ID,
		CLOSE_ID,
		
		SCROLL_UP_ID,
		SCROLL_DOWN_ID,

		WAR_LIST_ID,
		INDEX_CHALLENGER_ID,
		INDEX_DATE_ID,
		
		EXEC_MAX,
	};
public:
	class WarInfo
	{
		public:
		WarInfo()
		{
			for(int i = 0; i< 5; i++)
				challengerGuildID[i] = 0;
			reinforceGuildID = 0;
		};
	
		BYTE warType;		// 0 : 동족간 1 : 종족간
		WORD year;
		BYTE month;
		BYTE day;
		BYTE hour;
		WORD		challengerGuildID[5];
		std::string challengerGuildName[5];
		WORD		reinforceGuildID;
		std::string	reinforceGuildName;
	};
	
	struct _WarList
	{
		BYTE warType;		// 0 : 동족간 1 : 종족간
		WORD year;
		BYTE month;
		BYTE day;
		BYTE hour;
		WORD		challengerGuildID;
		std::string challengerGuildName;
	};
private:
	std::vector<_WarList>			m_v_war_list;
	POINT							m_vampire_point;
	bool							m_bl_scrolling;
	int								m_scroll;
	
public:
	C_VS_UI_WAR_LIST();
	~C_VS_UI_WAR_LIST();


	void	AddWarList(const WarInfo	&war_info);
	void	ClearWarList();
	
	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	bool	AllMatchWindowPixel(int _x, int _y) const;

	void	Start();
	void	Finish();

	void	Show();
};

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_BLOOD_BIBLE_STATUS : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup				*m_pC_button_group;
	
	enum EXEC_ID
	{
		PUSHPIN_ID,
		ALPHA_ID,
		CLOSE_ID,
		HELP_ID,
	};
	
public :

	enum BLOOD_BIBLE_STATUS_INFO
	{
		BLOOD_BIBLE_NONE,
		BLOOD_BIBLE_DROP,
		BLOOD_BIBLE_HAS_SLAYER,
		BLOOD_BIBLE_HAS_VAMPIRE,
		BLOOD_BIBLE_SHRINE_SLAYER,
		BLOOD_BIBLE_SHRINE_VAMPIRE,
		BLOOD_BIBLE_SHRINE_OUSTERS,
		BLOOD_BIBLE_HAS_OUSTERS,
	};
	struct BLOOD_BIBLE_STATUS
	{
		int zone_id;
		int x;
		int y;
		BYTE shrine_race;
		BLOOD_BIBLE_STATUS_INFO status;
		//BYTE bonus;										-_- 안쓴다. 
		std::string name;
	};

	BLOOD_BIBLE_STATUS		m_BloodBibleStatus[12];

	DWORD					m_tickCount;
	DWORD					m_sec;

public :

	C_VS_UI_BLOOD_BIBLE_STATUS();
	~C_VS_UI_BLOOD_BIBLE_STATUS();

	void	SetTimer(DWORD sec) { m_sec = sec; m_tickCount = timeGetTime(); }

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

	void	SetBloodBible(int blood_bible_id, BLOOD_BIBLE_STATUS &blood_bible_status);
	void	UnSetBloodBible(int blood_bible_id);

	void	Show();
	void	Process();
	void	Start();
	void	Finish();
	bool	IsPixel(int _x, int _y);
};

//-----------------------------------------------------------------------------
// class C_VS_UI_BRING_FEE
//
// 이름 입력하기
// -_- 다용도로 씁시다.
//-----------------------------------------------------------------------------
class C_VS_UI_INPUT_NAME : public Window, public Exec, public ButtonVisual
{
public :
	enum INPUT_NAME_MODE_LIST
	{
		INPUT_NAME_HOPE_COUPLE = 0,					// 커플 희망시
		INPUT_NAME_BREAK_UP,					// 커플 결별시		
		INPUT_NAME_FORCE_BREAK_UP,

		INPUT_NAME_MAX,
	};

	enum	INPUT_STATUS_LIST
	{
		INPUT_STATUS_NORMAL,
		INPUT_STATUS_DISABLE,
		INPUT_STATUS_MAX,
		INPUT_STATUS_NULL,
	};
private :
	LineEditorVisual		m_lev_name;	
	int		m_name_x,m_name_y;
	INPUT_NAME_MODE_LIST	m_Mode;
	INPUT_STATUS_LIST		m_Status;

	ButtonGroup			*m_pC_button_group;

	std::string			m_name,m_status;

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,		
	};

public :	
//	static	C_VS_UI_MONEY_DIALOG		*m_pC_bring_fee_dialog;
	C_VS_UI_INPUT_NAME(INPUT_NAME_MODE_LIST mode = INPUT_NAME_HOPE_COUPLE);
	~C_VS_UI_INPUT_NAME();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);

	const char* GetCurrentName();
	void	SetName(const char* name);

	
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

	void	SetEnable() { m_Status = INPUT_STATUS_NORMAL; }
	void	SetDisable() { m_Status = INPUT_STATUS_DISABLE; }
};

//-----------------------------------------------------------------------------
// class C_VS_UI_BRING_FEE
//
// 이름 입력하기
// -_- 다용도로 씁시다.
//-----------------------------------------------------------------------------
class C_VS_UI_POPUP_MESSAGE : public Window, public Exec, public ButtonVisual
{
public :
	enum POPUP_TYPE
	{		
		POPUP_NORMAL,
		POPUP_LARGE,
		POPUP_WELCOME,
		POPUP_EVENT_QUEST,
		POPUP_MINI_WINDOW,
		POPUP_ASK_BEGINNER_ZONE,			// 초보자 존으로 갈래?
		
		POPUP_MAX,
		POPUP_NULL,
	};

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
	};

	enum
	{
		STYLE_OK_BUTTON = 1,
		STYLE_CANCEL_BUTTON = 2,
		STYLE_JOIN_BUTTON = 4,
		STYLE_ABSENCE_BUTTON = 8,
	};
	
	struct PopupWindowStyle
	{
		short			WindowX,WindowY;
		POPUP_TYPE		Type;
		DWORD			Style;
		DWORD			SendID;
		std::string		str;

		DWORD			Value1;
		DWORD			Value2;
		
		void*			pValue;
	};

private :
	std::string				m_Str;
	POPUP_TYPE				m_Type;
	CSpritePack				m_p_NPCFace;
	DWORD					m_SendID;

	ButtonGroup			*m_pC_button_group;

public :	
//	static	C_VS_UI_MONEY_DIALOG		*m_pC_bring_fee_dialog;
	C_VS_UI_POPUP_MESSAGE(const char *str, POPUP_TYPE type = POPUP_NORMAL);
	C_VS_UI_POPUP_MESSAGE(PopupWindowStyle &style);
	~C_VS_UI_POPUP_MESSAGE();

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
};

//-----------------------------------------------------------------------------
// class C_VS_UI_LOTTERY_CARD
//
//
//-----------------------------------------------------------------------------
class C_VS_UI_LOTTERY_CARD : public Window, public Exec, public ButtonVisual
{
public :

	enum LOTTERY_STATUS
	{
		LOTTERY_TYPE_READY,
		LOTTERY_TYPE_WAIT_CLIENT,
		LOTTERY_TYPE_SCRATCH,
		LOTTERY_TYPE_CLEARING,		
		LOTTERY_TYPE_CLOSE,
		LOTTERY_TYPE_MAX,		
	};

	int						m_step;
	C_SPRITE_PACK *			m_pC_lottery_spk;
	CSpriteSurface *		m_p_cover_surface;
	LOTTERY_STATUS			m_Type;	
	int						m_radio_select;
	int						m_line_select;
	bool					m_bl_scratch_enable;
	bool					m_bl_press;
	int						m_backimage[3];
	bool					m_bl_result;
	int						m_old_point_x;
	int						m_old_point_y;


private :
	
	struct GiftInfo
	{
		DWORD	id;
		BOOL	Active;
		std::string name;
		int		image;
	};

	std::vector<GiftInfo*>	m_GiftList;
	ButtonGroup			*m_pC_button_group;
	ButtonGroup			*m_pC_radio_group;

	enum EXEC_ID
	{
		BUTTON_ID,
		RADIO1,
		RADIO2,
		RADIO3,
		RADIO4,
		RADIO5,
	};

	enum LOTTERY_SPK
	{
		BACK = 0,
		BOTTOM,
		COVER,
		LOTTERY_READY,
		LOTTERY_READY_HILIGHTED,
		LOTTERY_READY_PUSHED,		
		LOTTERY_CLOSE,
		LOTTERY_CLOSE_HILIGHTED,
		LOTTERY_CLOSE_PUSHED,
		RADIO_BACK,
		RADIO_CHECK,
		
		GIFT_TICKET_1,
		DARKEDEN_TSHIRTS,
		DARKEDEN_PENDANT,
		DARKEDEN_TICKET_1,
		INLINE_SKATE,
//		VOICE_MAN,
//		MP3_PLAYER,
		SPEAKER,
		GIFT_TICKET_2,
		WATCH,
//		DVD_COMBO,
		CAMCORDER,
		CAMERA,
		GIFT_TICKET_3,
		HOME_THEATER,
		FLAT_TV,
		TFT_MONITOR,
		COMPUTER,
		DIGITAL_TV,
		DARKEDEN_TICKET_2,
		REFRIGERATOR,		
		DVD_PLAYER,
		HEELYS,
		IRIVER_395,
	};

public :	

	C_VS_UI_LOTTERY_CARD(int step = -1);
	~C_VS_UI_LOTTERY_CARD();

	void	Show();
	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);

	void	ClearGiftList();
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	bool	IsPixel(int _x,int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	SetResult(bool	bSuccess);
	void	UnacquireMouseFocus()
	{
		m_pC_button_group->UnacquireMouseFocus();
	}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();
	}

	void	ScratchCover(int _x, int _y);
	void	ClearingCover();
	void	ClearAllCover();
	int		PeelRate();	
	void	SetScratchEnable();
	void	LoadInfo(int &step);
	void	ShowDesc(int strX,int strY,const char *str);
};

//-----------------------------------------------------------------------------
// class C_VS_UI_ITEM_LIST
//
// 이름 입력하기
// -_- 다용도로 씁시다.
//-----------------------------------------------------------------------------
class C_VS_UI_ITEM_LIST : public Window, public Exec, public ButtonVisual
{
public :
	enum EXEC_ID
	{
		NEXT_ID,
		PREV_ID,
		BRING_ID,

		OK_ID,
		CANCEL_ID,
	};
	
	struct ItemList 
	{
		MItem*	pItem;
		DWORD	TimeLimit;
	};

	static MItem*				m_pItem;

private :
	std::string					m_Str;
	std::vector<ItemList>		m_ItemList;
	int							m_CurrentItem;
	int							m_MouseFocus;

	ButtonGroup					*m_pC_button_group;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;

public :	
	C_VS_UI_ITEM_LIST();
	~C_VS_UI_ITEM_LIST();

	void	Show();
	void	ShowDesc( ItemList pItem );
	std::string		GetItemName( int Focus );

	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	IsPixel(int _x,int _y);

	bool	AddItem( ItemList pItem );
	bool	DeleteItem( DWORD ID );
	void	ClearItem();

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
};

//-----------------------------------------------------------------------------
// class C_VS_UI_IMAGE_NOTICE
//
// 이름 입력하기
// -_- 다용도로 씁시다.
//-----------------------------------------------------------------------------
class C_VS_UI_IMAGE_NOTICE : public Window, public Exec, public ButtonVisual
{
public :	
	struct SNotice
	{
		DWORD				m_Time;
		std::string			m_Name;
		std::string			m_GiftName;
		char				m_Alpha;
	};

private :
	int		m_CloseTime;
		
	C_SPRITE_PACK *			m_pC_image_spk;
		
	std::vector<SNotice*>			m_NoticeList;

public :		

	C_VS_UI_IMAGE_NOTICE();
	~C_VS_UI_IMAGE_NOTICE();

	void	Show();
	std::string	LoadInfo(DWORD ID);
	void	AddNotice( const char* name, DWORD id );
	void	ClearNotice();
	void	DeleteNotice(int n);

	void	Start();
	void	Finish();
	void	Process();
	void	Run(id_t id);
	
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	IsPixel(int _x,int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	WindowEventReceiver(id_t event);
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus()
	{
	}
	void	CancelPushState() 
	{
	}
};

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_BULLETIN_BOARD : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			*m_pC_back_spk;
	ButtonGroup				*m_pC_button_group;
	bool					status;

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
	};

	enum TRACE_SPK
	{
		MAIN_WINDOW = 0,
		BUTTON_OK,
		BUTTON_OK_HILIGHTED,
		BUTTON_OK_PUSHED,
		BUTTON_CANCEL,
		BUTTON_CANCEL_HILIGHTED,
		BUTTON_CANCEL_PUSHED,
	};

	LineEditorVisual		m_lev_from;	
	LineEditorVisual		m_lev_message;	

	MItem					*m_pItem;
	std::string				m_szTreeMessage;

	int						m_pos_from_x;
	int						m_pos_from_y;
	int						m_pos_msg_x;
	int						m_pos_msg_y;

public :

	C_VS_UI_BULLETIN_BOARD(const MItem *pItem);
	~C_VS_UI_BULLETIN_BOARD();

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
// class C_VS_UI_REQUEST_RESURRECT
//
//-----------------------------------------------------------------------------
class C_VS_UI_REQUEST_RESURRECT : public Window
{
public :
	enum IMAGE_SPK_INDEX
	{
		RESURRECT_GUARD_SLAYER,
		RESURRECT_GUARD_VAMPIRE,
		RESURRECT_GUARD_OUSTERS,
		PARTY_REQUEST,
		PARTY_INVITE,
		RESURRECT_SLAYER,
		RESURRECT_VAMPIRE,
		RESURRECT_OUSTERS,
		LVU_BUTTON,
		LVU_BUTTON_OUSTERS,
		RESURRECT_SCROLL,
		ELIXIR_SCROLL,
		ETERNITY,
	};

	enum RESURRECT_MODE
	{
		RESURRECT_MODE_RESURRECT_SCROLL = 0,
		RESURRECT_MODE_ELIXIR,
		RESURRECT_MODE_ETERNITY,
		// 2004, 11, 16, sobeit add start
		RESURRECT_MODE_SIEGE_ATTACKER,
		// 2004, 11, 16, sobeit add end
		RESURRECT_MODE_MAX
	};

	class CResurrect
	{
	public :
		CResurrect() : m_Delay(0), m_Time(0) { memset(&m_ButtonRect, 0, sizeof(RECT) ); m_Enable = false; m_Image = -1; }
		void		SetRect( int x,int y,int w,int h ) { m_ButtonRect.left = x; m_ButtonRect.right = x+w; m_ButtonRect.top = y; m_ButtonRect.bottom = y+h;}

		RECT		m_ButtonRect;
		bool		m_Enable;
		int			m_Image;
		int			m_Delay;
		DWORD		m_Time;
	};

	C_SPRITE_PACK				m_image_spk;

	CResurrect					m_ResurrectButton[ RESURRECT_MODE_MAX ];

	int							m_focus;
	int							m_pushed;

public:
	C_VS_UI_REQUEST_RESURRECT(bool resurrect, bool elixir, bool eternity, bool IsSiegeAttacker);
	~C_VS_UI_REQUEST_RESURRECT();

	void	Show();
	bool	IsPixel(int _x, int _y);
	void	Start();
	void	Finish();
	bool	MouseControl(UINT message, int _x, int _y);
	void	CancelPushState() { m_pushed = -1; }
	void	UnacquireMouseFocus() { m_pushed = -1; }
	void	SetDelay( RESURRECT_MODE mode, int delay );
	void	Process();
};

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE
// 
//-----------------------------------------------------------------------------
class C_VS_UI_MIXING_FORGE : public Window, public Exec, public ButtonVisual
{
private:
	C_SPRITE_PACK			*m_pC_back_spk;
	C_SPRITE_PACK			*m_pC_inventory_spk;
	ButtonGroup				*m_pC_button_group;
	
	Rect					m_my_grid_rect, m_your_grid_rect;
	int						m_focus_grid_x, m_focus_grid_y;
		
	MItem					*m_p_select_item[2];
	
	std::vector<bool>		m_v_check;
	

	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
	};

	enum MIXING_FORGE_SPK
	{
		MAIN_WINDOW = 0,
		BUTTON_OK,
		BUTTON_OK_HILIGHTED,
		BUTTON_OK_PUSHED,
		BUTTON_CANCEL,
		BUTTON_CANCEL_HILIGHTED,
		BUTTON_CANCEL_PUSHED,
	};

	enum INVENTORY_SPK_INDEX
	{
		INVENTORY_WINDOW,
		INVENTORY_WINDOW_ALPHA,
		INVENTORY_ITEMBACK,
		INVENTORY_ITEMBACK_ALPHA,
		INVENTORY_WINDOW_BOTTOM,
	};	
	
public :

	enum FORGE_CLASS
	{
		CLASS_WEAPON,
		CLASS_ARMOR,
		CLASS_ACCESSORY,
	};

	enum FORGE_TYPE
	{
		TYPE_A,
		TYPE_B,
		TYPE_C,
		TYPE_D,
	};

	FORGE_CLASS				m_forge_class;
	FORGE_TYPE				m_forge_type;

	C_VS_UI_MIXING_FORGE( FORGE_CLASS forge_class, FORGE_TYPE forge_type );
	~C_VS_UI_MIXING_FORGE();

	bool	IsCorrectClass(ITEM_CLASS item_class);
	bool	IsCorrectType(TYPE_ITEMTYPE item_type, ITEM_CLASS item_class);
	bool	IsCorrectOption(MItem *p_item1, MItem *p_item2);

	int		GetFocusedItemGridX(const MItem * p_item);
	int		GetFocusedItemGridY(const MItem * p_item);	

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
	bool	Check(int grid_start_x, int grid_start_y);	
};

//-----------------------------------------------------------------------------
// class C_VS_UI_REMOVE_OPTION
//
//-----------------------------------------------------------------------------
class C_VS_UI_REMOVE_OPTION : public Window, public Exec, public ButtonVisual
{
public:
	enum TYPE
	{
		TYPE_A,
		TYPE_B,
		TYPE_C,
	};

	enum STATUS
	{
		STATUS_SELECT_OPTION,
		STATUS_QUESTION,
	};

	int					m_select_option;
	STATUS				m_status;
	std::string			m_str_option[2];
	bool				m_b_press;
	const MItem		*m_pMouseItem,*m_pCurItem;
	
private:

	ButtonGroup			*m_pC_button_group;
	bool				m_IsBokjory;

	
	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,
	};

public:
	C_VS_UI_REMOVE_OPTION(const MItem *pItem, const MItem *pCurItem);
	~C_VS_UI_REMOVE_OPTION();

	static	bool	IsCanRemoveOption_Puritas(const MItem* pItem, const MItem* pTItem);

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
	void	CheckOption();
	
};

//-----------------------------------------------------------------------------
// class C_VS_UI_OUSTERS_SKILL_INFO
//
//-----------------------------------------------------------------------------
class C_VS_UI_OUSTERS_SKILL_INFO : public C_VS_UI_DESC, public Window, public Exec, public ButtonVisual
{
public:
private:
	
	ButtonGroup			*m_pC_button_group;
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;
	bool				m_bDownSkill;	
	C_SPRITE_PACK		m_SPK;
	
	enum EXEC_ID
	{
		CLOSE_ID,
		HELP_ID,
		ALPHA_ID,
		LEARN_ID,
	};
	int		m_skillID;

public:
	C_VS_UI_OUSTERS_SKILL_INFO(int skillID, int window_x, int window_y,bool DownSkill = false);
	~C_VS_UI_OUSTERS_SKILL_INFO();
	
	void	SetSkillID(int skillID);
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
	
};


//-----------------------------------------------------------------------------
// class C_VS_UI_HORN
//
//-----------------------------------------------------------------------------
class C_VS_UI_HORN : public Window, public Exec, public ButtonVisual
{
public:
private:
	
	ButtonGroup			*m_pC_button_group;
	
	C_SPRITE_PACK		m_SPK, m_MapSPK;
	
	enum EXEC_ID
	{
		CLOSE_ID,
		UP_ID,
		DOWN_ID,
	};

	enum SPRITE_ID
	{
		MAIN_WINDOW,
		UD_BUTTON,
		UD_BUTTON_HILIGHTED,
		UD_BUTTON_PUSHED,
		CLOSE_BUTTON,
		CLOSE_BUTTON_HILIGHTED,
		CLOSE_BUTTON_PUSHED,
	};

	int		m_CurrentZone;
	int		m_MapX, m_MapY;
	int		m_focus;
	int		m_Cost;

//	UI_PORTAL_VECTOR		m_portal;
	
public:
	C_VS_UI_HORN(int currentZoneID);
	~C_VS_UI_HORN();
	
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

	int		GetNext(int map, bool bUp);	
};

//-----------------------------------------------------------------------------
// class C_VS_UI_FINDING_MINE
//
//-----------------------------------------------------------------------------
class C_VS_UI_FINDING_MINE : public Window, public Exec, public ButtonVisual
{
public:
private:
	
	ButtonGroup			*m_pC_button_group;
	
	enum EXEC_ID
	{
		CLOSE_ID,
		START_ID,
		LEVEL0_ID,
		LEVEL1_ID,
		LEVEL2_ID,
	};
	
//	enum SPRITE_ID
//	{
//		MAIN_WINDOW,
//			UD_BUTTON,
//			UD_BUTTON_HILIGHTED,
//			UD_BUTTON_PUSHED,
//			CLOSE_BUTTON,
//			CLOSE_BUTTON_HILIGHTED,
//			CLOSE_BUTTON_PUSHED,
//	};


	enum GAME_SIZES
	{
		BLOCK_WIDTH = 16,
		BLOCK_HEIGHT = 16,
		BLOCK_START_X = 12,
		BLOCK_START_Y = 96,
	};

	enum GAMELEVEL
	{
		GAMELEVEL_0,
		GAMELEVEL_1,
		GAMELEVEL_2,

		GAMELEVEL_MAX,
	};

	enum GAME_STATUS
	{
		GAMESTATUS_GAME,	// 게임중
		GAMESTATUS_DIE,		// 게임 끝남 터져서-ㅅ-
		GAMESTATUS_CLEAR,	// 게임 끝남 다 찾아서
		GAMESTATUS_READY,	// 새 게임
	};

	enum LEFTMINE
	{
		LEFTMINE_LEVEL0 = 10,
		LEFTMINE_LEVEL1 = 40,
		LEFTMINE_LEVEL2 = 99,
	};

	enum BOARDSIZE
	{
		BOARDSIZE_LEVEL0_WIDTH = 10,
		BOARDSIZE_LEVEL0_HEIGHT = 10,
		BOARDSIZE_LEVEL1_WIDTH = 16,
		BOARDSIZE_LEVEL1_HEIGHT = 16,
		BOARDSIZE_LEVEL2_WIDTH = 30,
		BOARDSIZE_LEVEL2_HEIGHT = 16,
	};

	enum BLOCK_STATUS
	{
		BLOCKSTATUS_CLOSE,
		BLOCKSTATUS_OPEN,
		BLOCKSTATUS_QUESTION,
		BLOCKSTATUS_FLAG,
	};

	struct BLOCK_STRUCT
	{
		BLOCK_STRUCT()	{ num = 0; status = (BLOCK_STATUS)0; bl_focused = false; }

		int num;
		BLOCK_STATUS status;
		bool		bl_focused;
	};

	DWORD	m_startTime;
	int		m_leftMineCount;
	SIZE	m_boardSize;
	GAME_STATUS	m_status;

	BLOCK_STRUCT	*m_Blocks;

	Rect	m_BlockRect;

	GAMELEVEL m_level;
	bool bLButton, bRButton;

	void	DrawBlock(int x, int y, BLOCK_STRUCT &block);
	void	DrawRect(RECT &rect, int depth, WORD color, WORD color2, WORD color3);
	void	ClearBlockFocus(int x = -1, int y = -1, int w = -1, int h = -1);
	void	OpenBlock(int x, int y, bool bCenter = false);
	bool	CheckClear();
	void	DrawNumbers(int x, int y, int number);
	void	DrawNumber(int x, int y, BYTE flag);

public:
	C_VS_UI_FINDING_MINE();
	~C_VS_UI_FINDING_MINE();
	
	void	NewGame(GAMELEVEL level);
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
	
};

extern C_VS_UI_WINDOW_MANAGER* gpC_vs_ui_window_manager;

//extern C_VS_UI_FRIEND_INFO* gpC_vs_ui_friend_info;

#define ITEM_REF_POINT_COUNT			4

extern S_SLOT	g_char_slot_ingame;
extern Point g_item_ref_point[];

extern int g_HISTORY_LINE;	// 채팅창 길이

void g_SetItemRefPoint(int item_rect_w, int item_rect_h);

#endif