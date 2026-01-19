/*-----------------------------------------------------------------------------

	u_window.h

	Window property base class.

	2000.8.14. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __U_WINDOW_H__
#define __U_WINDOW_H__

#pragma warning(disable:4786)

#include "Basics.h"
#include "PI.h"
#include <list>

#include "u_edit.h"

#define SCR2WIN_X(screen_x)			((screen_x)-x)
#define SCR2WIN_Y(screen_y)			((screen_y)-y)
#define WIN2SCR_X(win_x)				((win_x)+x)
#define WIN2SCR_Y(win_y)				((win_y)+y)

enum MOUSE_STATE
{
	MOUSE_NOSTATE,
	MOUSE_MOVE,
	MOUSE_LEFTDOWN,
	MOUSE_LEFTUP,
	MOUSE_RIGHTDOWN,
	MOUSE_RIGHTUP,
};

//-----------------------------------------------------------------------------
// Window
//

//







//
//

//


//
//		Process
//


//
//		Show
//


//
//		Control
//


//
//-----------------------------------------------------------------------------
//

//



//

//







//

//




//

//







//





//

//






//


//
//

//


//
//
// [2000.8.21]
//

//
//
// [2000.10.13]
//

//


//
//-----------------------------------------------------------------------------
class Window : public Rect, public PI_Visual
{
private:
	//
	// m_bl_window_move_ready
	//



	//
	bool							m_bl_window_move_ready;

	Point							m_origin;

	//
	// m_u_mouse_state
	//


	//


	//
	MOUSE_STATE					m_u_mouse_state;

public:
	//
	// Local attributes flags
	//
	enum ATTRIBUTES_HIDE
	{
		ATTRIBUTES_HIDE_NOT,
		ATTRIBUTES_HIDE_WIDTH,
		ATTRIBUTES_HIDE_HEIGHT,
	};

	struct ATTRIBUTES
	{
		bool				window_move;
		bool				pin;
		bool				topmost;
		bool				keyboard_control;
		bool				alpha;
		bool				statch;
		ATTRIBUTES_HIDE		autohide;
	};

private:
	ATTRIBUTES					m_attributes;

	virtual void	Init();

public:
	//

	//
	virtual void WindowEventReceiver(id_t event) {}

	enum // event list
	{
		//EVENT_WINDOW_CREATE,
		//EVENT_WINDOW_DESTROY,
		EVENT_WINDOW_MOVE,
	};

public:
	Window(int _x, int _y, int _w, int _h);
	Window();
	virtual ~Window();

	virtual bool MouseControl(UINT message, int _x, int _y);
	virtual void KeyboardControl(UINT message, UINT key, long extra);
	virtual bool IsPixel(int _x, int _y) = 0;

	virtual void AcquireMouseFocus() {}
	virtual void UnacquireMouseFocus() {}



	virtual void AcquireFirstSequence() {} 




	virtual void AcquireMouseClick() {}		

	virtual void CancelPushState() {}

	virtual void AcquireDisappear() {}

	MOUSE_STATE	GetMouseInputState();

	//
	// Local attributes 
	//
	void	InitAttributes();
	void	AttrTopmost(bool state);
	void	AttrKeyboardControl(bool state);
	void	AttrWindowMove(bool new_state);
	void	AttrPin(bool new_state);
	void	AttrAlpha(bool new_state);
	void	AttrAutoHide(ATTRIBUTES_HIDE new_state);
	void	AttrStatch(bool new_state);
	void	ProcessHide(int gap = 0, bool alpha_window = false);
	
	const Window::ATTRIBUTES *	GetAttributes() const { return &m_attributes; }

	//
	// Window moving
	//
	bool	Moving() const { return m_bl_window_move_ready; }
	void	MoveReady();
	void	SetOrigin(int ox, int oy);
	void	GetOrigin(Point &orgin) const { orgin = m_origin; }

private:
	bool	Move(int new_ox, int new_oy);
	void	MoveOk();

public:
	void	ClearInputState();

	//
	// Place other Widget.
	//
private:
	SimpleDataList<LineEditorVisual *>		m_sdl_lev;

public:
	void	ShowWidget() const;
	void	Attach(LineEditorVisual * p_lev);
};

//-----------------------------------------------------------------------------
// WindowManager
//


//
//

//

//
//------------------------------------------------------------------------------
// < Focused Window >
//



//
// Focused Window:
//






//


//
//		  o AcquireMouseFocus()
//		  o UnacquireMouseFocus()
//


//



//		  * Mouse pointer Move
//
//
// Acquired Window:
//




//
// 
//-----------------------------------------------------------------------------
typedef std::list<Window *> List;

class WindowManager : public SimpleDataList<Window *>
{
private:
	//
	// handler
	//

	//
	void							(*m_fp_handler_acquire_mousefocus)(void);
	void							(*m_fp_handler_unacquire_mousefocus)(void);
	void							(*m_fp_handler_acquire_disappear)(void);

	//
	// (m_current_mouse_x, m_current_mouse_y)
	//


	//
	int							m_current_mouse_x, m_current_mouse_y;

	//
	// m_pC_mouse_focused_window
	// 


	//




	//


	//
	Window *						m_pC_mouse_focused_window;
	Window *						m_pC_keydown_window;

	//
	// m_pC_pushed_window
	//
	Window *						m_pC_pushed_window;

	//
	// m_pC_keyboard_control_window
	//

	//




	//
	Window *						m_pC_keyboard_control_window;

	//
	// m_show_list
	//

	//
	List							m_show_list;

	//
	// m_show_list_pinned_window
	//





	//
	List							m_show_list_pinned_window;

	//
	// m_pC_topmost_window
	//


	//---------------------------------------------------------------------------------------

	//






	//
	Window *						m_pC_topmost_window;

	//
	// m_pC_mouse_click_window
	//


	//
	Window *						m_pC_mouse_click_window;

	int							m_old_origin_x;
	int							m_old_origin_y;
	int							m_res_x, m_res_y; // resolution

	enum DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		DIRECTION,
	};

	void	SetNextTopmostWindow();
	void	SetNextKeyboardControlWindow();
	void	SetKeyboardControlWindow(Window * p_window);

	void	RunAcquireMouseFocusHandler() { if (m_fp_handler_acquire_mousefocus) m_fp_handler_acquire_mousefocus(); }
	void	RunAcquireDisappearHandler() { if (m_fp_handler_acquire_disappear) m_fp_handler_acquire_disappear(); }
	void	RunUnacquireMouseFocusHandler() { if (m_fp_handler_unacquire_mousefocus) m_fp_handler_unacquire_mousefocus(); }

public:
	WindowManager();
	~WindowManager();

	//
	// Window register/Unregister
	//
	Window*	GetTopmostWindow()	{ return m_pC_topmost_window; }
	void	Register(Window * p_window);
	void	Unregister(Window * p_window);

	bool	AlreadyRegistered(Window * p_window) const;

	void	Process();
	void	Show();

	//
	// Window input control

	//
	bool	MouseControl(UINT message, int x, int y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	SetResolution(int res_x, int res_y) { m_res_x = res_x; m_res_y = res_y; }

public:
	//
	// set handler
	//
	void	SetAcquireMouseFocusHandler(void (*fp)(void)) { m_fp_handler_acquire_mousefocus = fp; }
	void	SetUnacquireMouseFocusHandler(void (*fp)(void)) { m_fp_handler_unacquire_mousefocus = fp; }
	void	SetAcquireDisappearHandler(void (*fp)(void)) { m_fp_handler_acquire_disappear = fp; }

	Window *GetFocusedWindow(int x, int y);
	//
	// Window priority
	//
private: 

	void	SendMouseMoveMessageToMouseFocusedWindow();

public:
	void	SetMouseMoveFocusedWindow();

	//
	// Window interface
	//
	void	FirstPriority(Window * p_this_window);
	void	AppearWindow(Window * p_this_window);
	bool	DisappearWindow(Window * p_this_window);
	int	GetSequence(Window * p_this_window);
	Window * GetMouseFocusedWindow() const { return m_pC_mouse_focused_window; }
	Window * GetFirstPriorityWindow() const;
	Window * GetMovingWindow() const;
	bool	CancelPushStateOfCurrentPushedWindow();

	int	ShowedWindowSize() const;
	int	ShowedPinnedWindowSize() const { return m_show_list_pinned_window.size(); }
	//int	RegisteredWindowSize() const { return m_window_buf_size; } // -> use 'Size()'
	bool	GetShowState(Window * p_window) const;
};

extern WindowManager *	gpC_window_manager;

void	g_RegisterWindow(Window * p_window);
void	g_UnregisterWindow(Window * p_window);

#endif
