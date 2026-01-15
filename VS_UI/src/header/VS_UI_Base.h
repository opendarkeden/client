/*-----------------------------------------------------------------------------

	VS_UI_Base.h

	2000.8.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_BASE_H__
#define __VS_UI_BASE_H__

#include "BasicS.h"
#include "DLL.h"
#include "GL_import.h"
#include "Timer2.h"
#include "DXLib.h"
#ifdef PLATFORM_WINDOWS
#include "Unicorn.h"
#endif
#include "SP.h"
#include "CSpriteSurface.h"
//#include "VS_UI_Exception.h"
#include "VS_UI_ui_result_receiver.h"
#include "SimpleDataList.h"
#include "PI.h"
#include "PS.h"
#include "RaceType.h"

#include <assert.h>

// default help string
struct S_DEFAULT_HELP_STRING
{
	char *		sz_main_str;
	char *		sz_main_str2;
	char *		sz_sub_str;
};

struct S_BLOOD_BIBLE_DESCRIPTION
{
	char *		sz_name;
	char *		sz_place;
	char *		sz_status;
	char *		sz_shrine;
	char *		sz_option;
	char *		sz_player;
};

// �ֹι�ȣ�ڸ���(�ѱ�)
#define SSN_POS_COUNT					13
#define SSN_PART1_CHAR_COUNT			6
#define SSN_PART2_CHAR_COUNT			7
//#define _SHOW_WINDOW_ATTR

//
// SHOW_WINDOW_ATTR�� �� Window�� Window���¸� ����ϱ� ���� ���̴�. Window�� ������ϱ� ����
// ���� ���̴�.
//
#if (defined(_SHOW_WINDOW_ATTR) && !defined(_LIB))
#define SHOW_WINDOW_ATTR { \
								char buf[200]; \
								sprintf(buf, "[move=%d, pin=%d, topmost=%d, kctrl=%d", GetAttributes()->window_move, GetAttributes()->pin, GetAttributes()->topmost, GetAttributes()->keyboard_control); \
								g_Print(x, y, buf); \
								}
#else
#define SHOW_WINDOW_ATTR ;
#endif

//
// set/unset background mouse focus message
//
// client background�� mouse focus�� set/unset�Ѵ�.
// �̰��� mouse pointer�� UI Window�� acquire/unacquire�� client�� �˸��� ���̴�.
//
#define SET_BACKGROUND_MOUSE_FOCUS	if (gpC_window_manager->GetMouseFocusedWindow() == NULL) \
													gpC_base->SendMessage(UI_BACKGROUND_MOUSE_FOCUS, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY());
#define UNSET_BACKGROUND_MOUSE_FOCUS	gpC_base->SendMessage(UI_REMOVE_BACKGROUND_MOUSE_FOCUS);


#define TEXT_LINE(x)						(x*20) // 20 font height...

// LOWORD, HIWORD�� �˻��Ѵ�.
#define MAKEDWORD(a, b)					(DWORD)((a << 16)|b)

#define INTERFACE_BLINK_VALUE_MAX	4

extern bool gbl_global_empty_move;

//
// EMPTY_MOVE�� ������ ���콺�Է��� MOVE�� �� �� �߻��Ѵ�.
//
#define EMPTY_MOVE						gbl_global_empty_move = true;

//-----------------------------------------------------------------------------
// Global definition.
//-----------------------------------------------------------------------------
#define PCS_NUMBER_LIMIT				7 // 7�ڸ�

// rgb
#define RGB_WHITE							RGB(255, 255, 255)
#define RGB_RED							RGB(255, 0, 0)
#define RGB_GREEN							RGB(0, 255, 0)
#define RGB_BLUE							RGB(0, 0, 255)
#define RGB_YELLOW						RGB(255, 255, 0)
#define RGB_GRAY							RGB(192, 192, 192)
#define RGB_DARKGRAY						RGB(100, 100, 100)
#define RGB_GOLD							RGB(187, 187, 0)
#define RGB_BLACK							RGB(0, 0, 0)
#define RGB_PEARL							RGB(150,50,170)
// color
enum 
{ 
	BLACK				= 0,
	YELLOW			= 65504,
	DARKYELLOW		= 33792,
	GREEN				= 2016,
	YELLOWGREEN		= 34784,
	WHITE				= 0xFFFF,
	BLUE				= 0xFF,
	LIGHT_BLUE		= 0x421F,
	SKY_BLUE			= 33823,
	RED				= 49152,
	MAGENTA			= 63519,
	LIGHT_MAGENTA	= 0xFE1F,
	BORA				= 32799,
	GRAY				= 50712,
	DARKGRAY			= 25388,
	LIGHTRED			= 0xf800,
	DARKRED				= 0x8000,
};

// extended color
enum
{
	CURSOR_COLOR		= SKY_BLUE,
	CURSOR_COLOR_RED	= RED,

	POSSIBLE_COLOR		= LIGHT_BLUE,
	IMPOSSIBLE_COLOR	= RED,
	EXCHANGE_COLOR		= BORA,
};

// objective color
enum
{
	ERROR_MSG_COLOR = YELLOW,
};

// screen resolution
enum
{
	RESOLUTION_X = 1024,
	RESOLUTION_Y = 768,
};

// font id
// ������ �� �ִ� font ����.
enum font_id_t
{
	FONT_SLAYER,
	FONT_VAMPIRE,
	FONT_BBS,
	FONT_ITEM,
	FONT_ITEM_SMALL,
	FONT_NORMAL,
};

//---------------------------------------
// UI result receiver void ptr caster
//---------------------------------------
struct MOUSEPOINTER_INFO
{
	int		x; // screen������ (x, y)
	int		y;
	int		width;
	int		height;
};

// Client�� login�� ������ ���� ����ü.
struct DELETE_CHARACTER
{
	char *	sz_part1;
	char *	sz_part2;
	int		slot;
};

struct LOGIN
{
	char *	sz_id;
	char *	sz_password;
};

struct NEW_REGISTRATION
{
	char *	sz_id;
	char *	sz_password;
	char *	sz_repassword;
	char *	sz_name;

	char *	sz_email;
	char *	sz_address;
	char *	sz_ssn_number_part1; // �ֹι�ȣ
	char *	sz_ssn_number_part2;
	char *	sz_homepage;
	char *	sz_woo;
	char *	sz_phone;

	bool		bl_female;
	bool		bl_announce_my_info;
};

struct NEW_CHARACTER
{
	char *		sz_name;

	Race		race;
//	bool		bl_slayer;
	bool		bl_female;
	int			STR;
	int			DEX;
	int			INT;
	int			slot;

	int			face;
	int			skin_color;
	int			hair_color;
	int			coat_color;
	int			trouser_color;
};

//-----------------------------------------------------------------------------
// Base
//
// ����� �����ϴ� class�̴�.
// VS_UI System�� ���������� ������ ������ �͵��� ��� ���⿡ ���ǵȴ�.
//-----------------------------------------------------------------------------
class Base
{
	//
	// System condition flag.
	//
private:
	//
	// m_event_occured
	//
	// C_VS_UI_TRIBE class ����.
	//
	// event�� �߻��ϸ� set�ȴ�.
	//
	bool						m_event_occured;

public:
	bool	EventOccured() const { return m_event_occured; }
	void	StartEvent();
	void	FinishEvent();

	//
	// Video/Font defition.
	//
public:
	CSpriteSurface *		m_p_DDSurface_back; // UI�� ����� surface
	CSpriteSurface			m_DDSurface_offscreen; // effect�� ���� offscreen

private:
	int						m_colorkey_red; // offscreen colorkey

public:
	PrintInfo				m_small_pi;
	PrintInfo				m_chatting_pi;
	PrintInfo				m_user_id_pi;
	PrintInfo				m_value_pi; // �������ĵ� ���� ����..
	PrintInfo				m_value2_pi; // �������ĵ� ���� ����..
	PrintInfo				m_item_name_pi; // !item description�� ��µ� �� ���� ����� �� �ִ�.
	PrintInfo				m_item_desc_pi;
	PrintInfo				m_dialog_menu_pi;
	PrintInfo				m_dialog_msg_pi;
	PrintInfo				m_desc_menu_pi;
	PrintInfo				m_desc_msg_pi;
	PrintInfo				m_money_pi;
	PrintInfo				m_char_value_pi;
	PrintInfo				m_char_chat_large_pi;
	
	// use by Client
	PrintInfo				m_chat_dialog_pi;
	PrintInfo				m_info_pi;
	PrintInfo				m_item_pi;

	PrintInfo				m_char_name_pi; //by larosel
	PrintInfo				m_char_chat_pi; //by larosel

	// party
	PrintInfo				m_party_name_pi;

	// xmas
	PrintInfo				m_xmas_pi;
	
	PrintInfo				m_money2_pi; // ���� ���� ��

private:
	C_VS_UI_UI_RESULT_RECEIVER		m_C_ui_result_receiver;

	void	InitFont();
	void	InitSurface(CSpriteSurface *surface);
#ifdef PLATFORM_WINDOWS
	void	SetDefaultLogfont(LOGFONT &lf) const;
	void	SetFont(PrintInfo &pi, LOGFONT &lf, COLORREF textcolor, COLORREF backcolor=0, int bk_mode=TRANSPARENT, int align=TA_LEFT);
#endif

public:
	Base();
	~Base();

	void	Init(CSpriteSurface *surface, void (*fp)(DWORD, int, int, void *));
	void	DispatchMessage();
	void	SendMessage(DWORD message, int left = 0, int right = 0, void *void_ptr = NULL);
//	void	SelectFont(font_id_t id);

#ifndef _LIB
	int	GetMessageSize() const { return m_C_ui_result_receiver.GetMessageSize(); }
#endif
};

extern Base * gpC_base;

extern UINT	g_blink_value;
extern int ga_item_blink_color_table[];

extern bool gbl_sell_running; // ��� ��..
extern bool gbl_buy_running; // �Ĵ� ��..
extern bool gbl_repair_running; // ��ġ�� ��...
extern bool gbl_silvering_running; // ��ġ�� ��...
extern bool gbl_item_lock;
extern bool gbl_gear_lock;
extern bool gbl_item_trade_lock;
//extern bool gbl_vampire_interface;
extern Race g_eRaceInterface;
extern bool gbl_swap_advancement_item_running; // ��� ��..
#endif