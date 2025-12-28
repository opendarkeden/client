/*-----------------------------------------------------------------------------

	VS_UI_Dialog.h

	일반적인 Dialog box를 생성한다. 내부의 Widget은 scroll bar,	Button이다.
	이 객체는 공개되어 범용적으로 사용된다.

	2000.11.2. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_DIALOG_H__
#define __VS_UI_DIALOG_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"
#include <vector>
#include <string>


//-----------------------------------------------------------------------------
// Dialog 하단에 나오는 정의된 버튼.
//-----------------------------------------------------------------------------
#define DIALOG_NO_BUTTON				0x0000
#define DIALOG_OK						0x0001
#define DIALOG_CANCEL					0x0002
#define DIALOG_FRIEND_BLACK				0x0004		//add by viva
//-----------------------------------------------------------------------------
// Dialog 정의된 버튼 exec id.
//-----------------------------------------------------------------------------
#define DIALOG_EXECID_EXIT				0xFFFF0000
#define DIALOG_EXECID_OK				0xFFFF0001
#define DIALOG_EXECID_CANCEL			0xFFFF0002
#define DIALOG_EXECID_SCROLL_UP		0xFFFF0003
#define DIALOG_EXECID_SCROLL_DOWN	0xFFFF0004
#define DIALOG_EXECID_FRIEND_BLACK			0xFFFF0005		//add by viva

//-----------------------------------------------------------------------------
// Dialog menu struct.
//-----------------------------------------------------------------------------
struct DIALOG_MENU
{
	char *		sz_menu_str;
	id_t		exec_id;
};

struct DIALOG_MENU_INNER	// 내부에서 쓰임
{
	std::vector<std::string>	sz_menu_str;
	id_t		exec_id;
};

//-----------------------------------------------------------------------------
// DIALOG_BUTTON_X
//
// Button의 위치는 버튼의 개수에 따라서 달라지지만 기본적으로 box의 우측 끝에
// 정렬된다. button이 2개일 경우 button_n = 1이면 가장 우측이고 2이면 그 다음이다.
// 예를들어 Ok button은 Cancel button이 있을 경우 위치가 바뀐다.
//-----------------------------------------------------------------------------
#define DIALOG_BUTTON_EXTRA_HGAP		10

#define DIALOG_BUTTON_X(n)				(m_client_rect.Right()-DIALOG_BUTTON_WIDTH*(n)+35)
#define DIALOG_BUTTON_Y					(m_client_rect.Down()-DIALOG_BUTTON_HEIGHT-12)

#define DIALOG_BUTTON_POS(x)			DIALOG_BUTTON_X(m_button_count-(x))

#define TEXT_EXTRA_HGAP					8
#define MSG_EXTRA_HGAP					5

enum BUTTON_SEQUENCE
{
	BS_OK = 0,
	BS_CANCEL = 1,
	BS_FRIEND_BLACK = 2,	//add by viva
};

//-----------------------------------------------------------------------------
// SetMessage mode option
//-----------------------------------------------------------------------------
enum SETMESSAGE_MODE_OPTION
{
	SMO_FIT,	// Msg rect에 정확히 맞추고 정렬하지 않는다. 또한 필요하다면 scroll bar를 생성한다.
	SMO_NOFIT, // Msg rect 내에서 위/아래 가운데 정렬을 하며 scroll bar를 생성하지 않는다.
};

//-----------------------------------------------------------------------------
// C_VS_UI_DIALOG
//
// format>
//
//				+----------------------------+
//				|			Message			 | -> Message가 영역보다 크면 scroll bar가 붙는다.
//				|							 |
//				+----------------------------+
//				|							 |
//				|	Menu 1.					 |
//				|	Menu 2. 				 |
//				|							 |
//				|	Button 1.   Button 2.	 |
//				+----------------------------+
//
// 기본적으로 외부에서 box의 size를 결정한다. 그것이 더 범용적으로 쓰는 데 지장이
// 없기 때문이다. Button, Menu가 배치되고 Message area가 좁으면 자동으로 scroll bar를
// 부착한다.
//
// !box가 너무 작으면 Menu 및 Button이 안 나올 수 있다.
//
// 사용법>
//				// func는 menu의 실행함수이다.
//				C_VS_UI_DIALOG m_pC_dialog = new C_VS_UI_DIALOG(50, 20, 6, 2, func, DIALOG_OK);
//
//				DIALOG_MENU d_menu[] = {
//					{"사기", 0},
//					{"팔기", 1},
//					{"끝내기", DIALOG_EXECID_EXIT},
//				};
//				m_pC_dialog->SetMenu(d_menu, 3);
//
//				static char * pp_dmsg[] = { // Message는 반드시 static or global로 해야 한다.
//					"line 1",
//					"line 2",
//				};
//
//				m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *));//, SMO_NOFIT);
//
//-----------------------------------------------------------------------------
class C_VS_UI_DIALOG : public Window, public Exec, public ButtonVisual
{
protected:
	BOOL						m_flag_menu;
	void							(*m_exec_fp)(C_VS_UI_DIALOG *, id_t);

	//
	// center (x, y)
	//
	// 조립박스의 x, y center 개수.
	// default로 가로 2개 세로 2개가 있고 center의 조립이미지의 개수를 늘릴 수 있다.
	//
//	int							m_width, m_height;

	WORD							m_ddb;

	DIALOG_MENU_INNER *				m_p_menu;
	UINT							m_menu_count;

	std::vector<std::string>		m_vs_msg;

	UINT							m_line_count;
	UINT							m_print_line_count;
	SETMESSAGE_MODE_OPTION	m_message_mode;
	int							m_nofit_mode_msg_y; // no fit mode일 경우 msg의 y.

	C_VS_UI_SCROLL_BAR			* m_pC_msg_scroll_bar,*m_pC_menu_scroll_bar;
//	Scrollbar					m_scrollbar;
//	int							m_remained_track;
//	bool							m_available_tag;
//	bool							m_tag_pushed;
//	int							m_prev_tag_y;
//	int							m_tag_up_limit, m_tag_down_limit;

	int							DIALOG_BUTTON_WIDTH;
	int							DIALOG_BUTTON_HEIGHT;

	int							m_menu_str_height;
	int							m_msg_str_height;
	//
	// rect
	//
	// Box에서 Message rect와 Menu rect이다. 이것은 일단 Button을 제외한 영역이다.
	// Button -> Menu -> Message 순으로 rect를 형성하는 데, 공간이 없으면 그것에 해당하는
	// 것은 출력되지 않는다. Button은 최소 box size에서도 가능하며 제일 하단을 차지한다.
	//
	Rect							m_msg_rect;
	Rect							m_menu_rect;
	Rect							m_client_rect;
//	Rect							m_tag_rect;

	int							m_button_count;
	int								*m_button_y_list;		// 스크롤 될때 y 크기를 알기 위함.
	int								m_menu_y_size,m_temp_menu_rect_y;	// 메뉴 Rect 사이즈 변경시 변경된 y 크기



	int							m_TempValue1;


	enum
	{
		DECORATE_GAP = 20,
	};

	ButtonGroup *				m_pC_button_group;

	int	GetButtonGap() const;
	bool						m_bOkOnly;
protected:
	void	CancelPushState();
	void	UnacquireMouseFocus();
	void	AcquireDisappear() {}
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	Process();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	virtual bool	MouseControl(UINT message, int _x, int _y);
	virtual void	KeyboardControl(UINT message, UINT key, long extra);
	virtual void	Show();

public:
	C_VS_UI_DIALOG(int _x, int _y, int width, int height, void (*exec_fp)(C_VS_UI_DIALOG *, id_t) = NULL, WORD dd_button=DIALOG_NO_BUTTON);
	virtual ~C_VS_UI_DIALOG();

	void	Run(id_t id);
	virtual void	Start();
	void	Finish();

	void	SetMessage(char ** sz_msg, UINT line_count, SETMESSAGE_MODE_OPTION mode=SMO_FIT);
	void	SetMenu(const DIALOG_MENU * p_dialog_menu, UINT menu_count, bool menu_only=true);

	void	StartByPinMode();

	int		GetScrollPos();
	void	ProcessMenuScrollBar();

	void	SetOkOnly(bool bFlag) { m_bOkOnly = bFlag;}
	// 2005, 1, 18, sobeit add start - 불우이웃돕기 성금 이벤트에서 개인 성금인지 길드 성금 인지 체크 하기 위해
	int GetTempValue() { return m_TempValue1;}
	void SetTempValue(int TempValue) { m_TempValue1 = TempValue;}
	// 2005, 1, 18, sobeit add end - 불우이웃돕기 성금 이벤트에서 개인 성금인지 길드 성금 인지 체크 하기 위해
};
//
// exec function prototype
//
// void VS_UI_DialogRun(C_VS_UI_DIALOG * p_this_dialog, id_t id);
//

#endif