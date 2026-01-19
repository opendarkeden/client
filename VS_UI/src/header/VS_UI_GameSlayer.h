/*-----------------------------------------------------------------------------

	VS_UI_GameSlayer.h

	Slayer interface.

	2000.8.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GAMESLAYER_H__
#define __VS_UI_GAMESLAYER_H__

#include "VS_UI_GameCommon.h"
#include "VS_UI_Util.h"
//#include "VS_UI_skill_tree.h"

#include "MSlayergear.h"

/*
//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_CHAT by larosel
//
// Slayer only chat menu interface object.
//-----------------------------------------------------------------------------
class C_VS_UI_SLAYER_CHAT : public Rect, public Exec//, public ButtonVisual
{
private:

	C_SPRITE_PACK 				m_chat_spk;

	CHAT_MENU	m_chat;
	bool		m_bl_whisper;

public:
	C_VS_UI_SLAYER_CHAT();
	~C_VS_UI_SLAYER_CHAT();

	void	AcquireDisappear() {}
	void	UnacquireMouseFocus();
	void	CancelPushState();
	void	ResetSize();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	MouseControl(UINT message, int _x, int _y);
	void	Start();
	void	Process();
	void	Run(id_t id);

	void	ShowButton(C_VS_UI_EVENT_BUTTON * p_button);
	CHAT_MENU GetCurrentMode() const	{ return m_chat; };
	bool	IsWhisper()const			{ return m_bl_whisper; };

};
*/
//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_QUICKITEM
//
// Slayer only Quick item interface object.
//-----------------------------------------------------------------------------
class C_VS_UI_SLAYER_QUICKITEM : public Window, public ButtonVisual, public Exec
{
private:
	enum 
	{ 
		QUICKSLOT_TOTAL = 8,
		QUICKSLOT_W = 30,
		QUICKSLOT_H = 30,
	};

	enum EXEC_ID
	{
		ALPHA_ID,
		PUSHPIN_ID,
		ROTATE_ID,
	};

public:
	enum
	{
		SLOT_1 = 0,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5,
		SLOT_6,
		SLOT_7,
		SLOT_8,
	};

private:
	enum SPK_INDEX
	{
		SLOT,
		SLOT_ALPHA,
		STATCH_EDGE,
		START_EDGE,
		START_EDGE_EXTEND,
		END_EDGE,
		END_EDGE_EXTEND,
		BUTTON_ROTATE_HILIGHTED,
		BUTTON_ROTATE_HILIGHTED_PUSHED,

		ROTATED_OFFSET,
	};

	ButtonGroup *				m_pC_button_group;

	C_SPRITE_PACK 				m_image_spk;

	int							m_p_slot_x[QUICKSLOT_TOTAL];

	int							m_focus_slot;

	bool							m_bl_width, m_bl_resize;

	bool	Click();

	int	GetPocketCount() const;

public:
	C_VS_UI_SLAYER_QUICKITEM();
	~C_VS_UI_SLAYER_QUICKITEM();

	void	ToggleBelt();
//	bool	IsOpen()	{ return m_bl_opened; }
	void	Open();
	void	Close();
	void	AcquireDisappear() {}
	void	Use(int slot, bool bFunctionKey=true);
	void	UnacquireMouseFocus();
	void	ResetSize();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	CancelPushState();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);


	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	MouseControl(UINT message, int _x, int _y);
	void	Show();
	void	Start();
	void	Process();
};
/*
//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PDS
//
// Slayer only Personal Data System Interface Object.
//
// - no Window property.
//-----------------------------------------------------------------------------
class C_VS_UI_SLAYER_PDS : public Rect, public Exec, public ButtonVisual
{
public:
	// character value
	/*
	static long					m_str;
	static long					m_dex;
	static long					m_int;
	static long					m_hp;
	static long					m_mp;
	static long					m_th;
	static long					m_ac;
	static long					m_cc;

	static long					m_str_max;
	static long					m_dex_max;
	static long					m_int_max;
	static long					m_hp_max;
	static long					m_mp_max;
	static long					m_th_max;
	static long					m_ac_max;
	static long					m_cc_max;

	static long					m_pcs_number; // slayer pcs number ; it's private!

private:

	enum PDS_PCS_SPK_INDEX
	{
		PCS_WINDOW,
		PCS_SEL,
		PCS_MEM,
		PCS_DEL,
		PCS_END,
		PCS_SEND,

		// dial-pad number
		PCS_0,
		PCS_1,
		PCS_2,
		PCS_3,
		PCS_4,
		PCS_5,
		PCS_6,
		PCS_7,
		PCS_8,
		PCS_9,
		PCS_BACKSPACE,

		PCS_PHONE_PUSHED,
		PCS_PHONE,
		PCS_QUIT_PUSHED,
		PCS_QUIT,
		PCS_ICON_BOUNDARY,
		PCS_CONNECTION_MARK,
	};

	enum PDS_SUB_SPK_INDEX // common use
	{
		CLOSE_PUSHED_BUTTON,
		SCROLLUP_PUSHED_BUTTON,
		SCROLLDOWN_PUSHED_BUTTON,
	};

public:
	enum PDS_MENU_ALL_ID
	{
		PDS_CLOSE_ID, // PDS close, it's common menu.

		// pcs
		PCS_SEL_ID,
		PCS_MEM_ID,
		PCS_DEL_ID,
		PCS_END_ID,
		PCS_SEND_ID,

		PCS_0_ID,
		PCS_1_ID,
		PCS_2_ID,
		PCS_3_ID,
		PCS_4_ID,
		PCS_5_ID,
		PCS_6_ID,
		PCS_7_ID,
		PCS_8_ID,
		PCS_9_ID,
		PCS_BACKSPACE_ID,

		PCS_HISTORY_SCROLLUP_ID,
		PCS_HISTORY_SCROLLDOWN_ID,

		PCS_PHONE_1_ID,
		PCS_PHONE_2_ID,
		PCS_PHONE_3_ID,
		PCS_QUIT_ID, // access in C_VS_UI_SLAYER
	};	

private:
	enum
	{
		PDS_SCROLLBUTTON_W = 11,
		PDS_SCROLLBUTTON_H = 6,

		PCS_MESSAGE_X = 28,
		PCS_MESSAGE_Y = 561,
		PCS_MESSAGE_FONT_GAP = 16,
		PCS_HISTORY_SIZE = 40,
		PCS_HISTORY_LINE = 3,

		PCS_ICON_X = 21,
		PCS_PHNE1_Y = 512,
		PCS_PHNE2_Y = 534,
		PCS_PHNE3_Y = 555,
		PCS_QUIT_Y = 578,

		PCS_WAIT_ROOM_MAX = 3,
	};

	enum PCS_STATE
	{
		PCS_CONNECTING,
		PCS_ON_LINE,
		PCS_OFF_LINE,
	};

	C_SPRITE_PACK 				m_pds_button_spk;
	C_SPRITE_PACK 				m_pds_pcs_spk;
	C_SPRITE_PACK				m_pds_sub_spk;

	PDS_MENU						m_pds_menu;

	ButtonGroup *				m_pC_common_button_group; // close button
	ButtonGroup *				m_pC_pcs_button_group;
	ButtonGroup *				m_pC_pcs_button_group2; // when one-line mode

	LineEditorVisual			m_lev_pcs_number;
	Scrollbar					m_pcs_scrollbar;

	char *						m_pcs_history_buf[PCS_HISTORY_SIZE];
	int							m_pcs_history_input_index;
	int							m_pcs_history_start_index;
	PCS_STATE					m_pcs_state;

	long							m_last_dialing_pcs_num;
	bool							m_bl_ring;

	//-----------------------------------------------------------------------------------
	//
	//
	//
	// < chatting mode >
	//
	//
	//
	//-----------------------------------------------------------------------------------
	struct S_PCS_WAIT_ROOM
	{
		char *	sz_name;
		long		pcs_number;
		bool		bl_set;
		bool		bl_granted;
	};

	S_PCS_WAIT_ROOM			m_pcs_wait_room[PCS_WAIT_ROOM_MAX];
	S_PCS_WAIT_ROOM			m_backup_room;
	int							m_pcs_who_connection;

	//
	// m_pcs_chatting
	//
	//
	C_VS_UI_CHATTING			m_pcs_chatting[PCS_WAIT_ROOM_MAX];

	void	ShowConnectorInfo(int _x, int _y, S_PCS_WAIT_ROOM &pcs_wait_room);
	int	GetPCSHistorySizeForScroll() const;
	void	AddToPCSHistory(char * sz_str);
	
public:
	C_VS_UI_SLAYER_PDS();
	~C_VS_UI_SLAYER_PDS();

	const C_VS_UI_CHATTING * C_VS_UI_SLAYER_PDS::GetCurrentChatting() const;
	void	AddChatToPCShistory(char * str, int pcs_slot);
	long	GetBackupRoomPCSnumber() const { return m_backup_room.pcs_number; }
	char * GetBackupRoomName() const { return m_backup_room.sz_name; }
	bool	GetRing() const { return m_bl_ring; }
	void	PushToBackupRoom(char * sz_name, long pcs_number, int slot);
	void	Ring(bool state) { m_bl_ring = state; }
	void	Run(id_t id);
	void	AcquireQuitPCSOnlineModeMessage();
	void	CancelPushState();
	void	UnacquireMouseFocus();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButton(C_VS_UI_EVENT_BUTTON * p_button);

	int	GetSendPossibleSlot() const { return m_pcs_who_connection; }
	void	ControlPDS(PDS_MENU pds_menu);
	PDS_MENU GetPDS() const { return m_pds_menu; }
	bool	PCSOnline() { return (m_pcs_state == PCS_ON_LINE); }
	void	OnLinePCS(char * sz_name, long pcs_number);
	void	OffLinePCS();

	void	SetSlot(char * sz_name, long pcs_number, int slot);
	int	ConnectPCS(char * sz_name, long pcs_number, int slot);
	bool	GrantPCSConnection(int room_n);
	void	DisconnectPCS(int room_n);
	void	ConnectionFailed();
};
*/

//-----------------------------------------------------------------------------
// class C_VS_UI_SLAYER_GEAR
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_SLAYER_GEAR : public C_VS_UI_GEAR
{

public:
	enum SLOT_NAME
	{
		SN_HELM,
		SN_NECKLACE,
		SN_COAT,
		SN_LEFTHAND,
		SN_RIGHTHAND,
		SN_GLOVE,
		SN_BELT,
		SN_TROUSER,
		SN_LEFTBRACELET,
		SN_RIGHTBRACELET,
		SN_RIGHTRING1,
		SN_RIGHTRING2,
		SN_LEFTRING1,
		SN_LEFTRING2,
		SN_SHOES,
		
		SN_COREZAP1,
		SN_COREZAP2,
		SN_COREZAP3,
		SN_COREZAP4,

		SN_PDA,
        SN_SHOULDER,
	
		SN_BLOODBIBLE1,
		SN_BLOODBIBLE2,
		SN_BLOODBIBLE3,
		SN_BLOODBIBLE4,
		SN_BLOODBIBLE5,
		SN_BLOODBIBLE6,

		SLOT_SIZE,
		};
	C_VS_UI_SLAYER_GEAR();
	~C_VS_UI_SLAYER_GEAR();
private:


	enum GEAR_SLOT_SPK_INDEX
	{
		HELM,
		NECKLACE,
		COAT,
		SHIELD,
		SWORD,
		GLOVE,
		BELT,
		TROUSER,
		BRACELET,
		RING,
		SHOES,
		BLOODBIBLE,
		PDA,
		SHOULDER,
	};

	static int					m_slot_image[SLOT_SIZE];
	Rect							m_slot_rect[SLOT_SIZE];

protected:

};

//class C_VS_UI_ADVANCEMENTSLAYER_GEAR : public C_VS_UI_GEAR
//{
//
//public:
//	enum SLOT_NAME
//	{
//		SN_HELM,
//		SN_NECKLACE,
//		SN_COAT,
//		SN_LEFTHAND,
//		SN_RIGHTHAND,
//		SN_GLOVE,
//		SN_BELT,
//		SN_TROUSER,
//		SN_LEFTBRACELET,
//		SN_RIGHTBRACELET,
//		SN_RIGHTRING1,
//		SN_RIGHTRING2,
//		SN_LEFTRING1,
//		SN_LEFTRING2,
//		SN_SHOES,
//		
//		
//
//		SN_PDA,				// PDA
//
//
//		SLOT_SIZE,
//	};
//	C_VS_UI_ADVANCEMENTSLAYER_GEAR();
//	~C_VS_UI_ADVANCEMENTSLAYER_GEAR();
//private:
//
//
//	enum GEAR_SLOT_SPK_INDEX
//	{
//		HELM,
//		NECKLACE,
//		COAT,
//		SHIELD,
//		SWORD,
//		GLOVE,
//		BELT,
//		TROUSER,
//		BRACELET,
//		RING,
//		SHOES,
//		BLOODBIBLE,
//		PDA,
//		SHOULDER,
//	};
//
//	static int						m_slot_image[SLOT_SIZE];
//	Rect							m_slot_rect[SLOT_SIZE];
//
//protected:
//
//};


//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER
//
// Slayer Main interface.
//-----------------------------------------------------------------------------
class C_VS_UI_SLAYER : public C_VS_UI_TRIBE
{
private:
	// data object
	C_VS_UI_SLAYER_GEAR *		m_pC_gear;
//	C_VS_UI_ADVANCEMENTSLAYER_GEAR * m_pC_advence_gear;

public:
	C_VS_UI_SLAYER();
	~C_VS_UI_SLAYER();


	void	RunQuickItemSlot();
	void	CloseQuickItemSlot();
	bool	IsRunningQuickItemSlot();

	C_VS_UI_GEAR * GetGearWindow() const;
//	C_VS_UI_ADVANCEMENTSLAYER_GEAR * GetAdvenceGearWindow() const;
	void	CloseGear();
	void	OpenGear(bool bl_set_load = true);

//	void	AdvenceCloseGear();
//	void	AdvenceOpenGear(bool bl_set_load1= true);




	void	UnacquireMouseFocus() 
	{ 
		C_VS_UI_TRIBE::UnacquireMouseFocus();
	}

	void	CancelPushState() 
	{ 
		C_VS_UI_TRIBE::CancelPushState();
	}

	void	DoCommonActionBeforeEventOccured();
	void	DoCommonActionAfterEventOccured();

	void	ResetSlayerQuickItemSize();

	void	Start();
	void	Show();
	void	Process();
//	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	//---------------------------------------
	// Hot key
	//---------------------------------------
	void HotKey_QuickItemSlot();
	void HotKey_F1();	
	void HotKey_F2();	
	void HotKey_F3();	
	void HotKey_F4();	
	void HotKey_F5();	
	void HotKey_F6();	
	void HotKey_F7();	
	void HotKey_F8();	

};

class C_VS_UI_SLAYER_PORTAL : public Window, public Exec, public ButtonVisual
{
private:
	enum ETC_SPK_INDEX
	{
		LEFT,
		LEFT_HILIGHT,
		LEFT_HILIGHT_PUSHED,
		RIGHT,
		RIGHT_HILIGHT,
		RIGHT_HILIGHT_PUSHED,
		CLOSE,
		CLOSE_HILIGHT,
		CLOSE_HILIGHT_PUSHED,
		FLAG,
		FLAG_HILIGHT = FLAG + 7,
	};

	enum MAP_SPK_INDEX
	{
		LIMBO,
		ESLANIAN,
		RODIN,
		DROBETA,
		PERONA,
		TIMORE,

		MAP_MAX,
	};

	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_etc_spk;
	C_SPRITE_PACK				m_map_spk;

	int m_left_x, m_left_y, m_right_x, m_right_y, m_close_x, m_close_y;
	int m_map;

	std::vector<UI_PORTAL_FLAG>	m_flag[MAP_MAX];
	std::vector<int>			m_zoneidList[MAP_MAX];

public:
	C_VS_UI_SLAYER_PORTAL();
	~C_VS_UI_SLAYER_PORTAL();

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

	int		GetNext(int map, bool bLeft);
	

};

#endif