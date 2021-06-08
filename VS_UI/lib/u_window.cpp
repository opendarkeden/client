// u_window.cpp
#include "client_PCH.h"
#pragma warning(disable:4786)


#include "u_window.h"
#include <math.h>
#include "vs_ui.h"
#include "CImm.h"
#include "UserOption.h"

#define STATCH_VALUE 10
#define HIDE_GAP	4
#define HIDE_SPEED	10
extern RECT g_GameRect;

//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------
WindowManager *	gpC_window_manager; // App에서 할당한다.

//-----------------------------------------------------------------------------
// Window
//
// 
//-----------------------------------------------------------------------------
Window::Window(int _x, int _y, int _w, int _h) : Rect(_x, _y, _w, _h)
{ 
	assert(gpC_window_manager != NULL);

	gpC_window_manager->Register(this);

	Init();

	//
	// constructor/destructor에서는 pure virtual이 불가능하다. 왜그러는지는
	// 모르겠다. 아마 그렇게하면 instatance가 불가능하다는 건지... 음...
	// 그래서 이 event는 이 방식으로는 불가능하다.
	//
	//WindowEventReceiver(EVENT_WINDOW_CREATE);
}

Window::Window()
{
	//assert(gpC_window_manager != NULL);

	//gpC_window_manager->Register(this);

	Init();

	//WindowEventReceiver(EVENT_WINDOW_CREATE);
}

//-----------------------------------------------------------------------------
// ~Window
//
// 
//-----------------------------------------------------------------------------
Window::~Window()
{
	//assert(gpC_window_manager != NULL);

	//gpC_window_manager->Unregister(this);

	//WindowEventReceiver(EVENT_WINDOW_DESTROY);

//	gC_ci->.FinishImeRunning();
//	gC_ci->.ClearCurrentIMEComposition();

}

//-----------------------------------------------------------------------------
// ShowWidget
//
// Window 위에 놓이는 Widget들을 출력한다.
//
// !이것은 외부의 Show() 루틴 마지막에서 실행되야 한다.
//-----------------------------------------------------------------------------
void Window::ShowWidget() const
{
	LineEditorVisual * data;
	for (int i=0; i < m_sdl_lev.Size(); i++)
		if (m_sdl_lev.Data(i, data))
			data->Show(); // 내부에서 dc를 Get/Release 한다.
}

//-----------------------------------------------------------------------------
// Attach
//
// p_lev를 Window에 붙인다.
//-----------------------------------------------------------------------------
void Window::Attach(LineEditorVisual * p_lev)
{
	if (p_lev == NULL)
		_Error(NULL_REF);

	m_sdl_lev.Add(p_lev); // 이미 존재하면 실패이다.
}

//-----------------------------------------------------------------------------
// InitAttributes
//
// 
//-----------------------------------------------------------------------------
void Window::InitAttributes()
{
	m_attributes.window_move = true;
	m_attributes.pin = false;
	m_attributes.topmost = false;
	m_attributes.keyboard_control = false;
	m_attributes.alpha = false;
	m_attributes.statch = true;
	m_attributes.autohide = ATTRIBUTES_HIDE_NOT;
}

//-----------------------------------------------------------------------------
// AttrTopmost
//
// topmost 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrTopmost(bool state)
{
	m_attributes.topmost = state;
}

//-----------------------------------------------------------------------------
// AttrKeyboardControl
//
// keyboard control 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrKeyboardControl(bool state)
{
	m_attributes.keyboard_control = state;
	gC_ci->FinishImeRunning();
	gC_ci->ClearCurrentIMEComposition();
//	gC_ci->.SetEngInput();
}

//-----------------------------------------------------------------------------
// AttrWindowMove
//
// Window 이동 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrWindowMove(bool new_state)
{ 
	m_attributes.window_move = new_state;

	// move ready가 true로 된 후 window_move attr이 false로 될 수 있다.
	m_bl_window_move_ready = false;
}

//-----------------------------------------------------------------------------
// AttrAlpha
//
// Window 이동 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrAlpha(bool new_state)
{ 
	m_attributes.alpha = new_state;
}

//-----------------------------------------------------------------------------
// AttrStatch
//
// Window 이동 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrStatch(bool new_state)
{ 
	m_attributes.statch = new_state;
}

//-----------------------------------------------------------------------------
// AttrAutoHide
//
// Window 이동 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::AttrAutoHide(ATTRIBUTES_HIDE new_state)
{ 
	m_attributes.autohide = new_state;
}

//-----------------------------------------------------------------------------
// ProcessHide
//
// Window 이동 속성을 설정한다.
//-----------------------------------------------------------------------------
void Window::ProcessHide(int gap, bool alpha_window)
{
	gap += HIDE_GAP;
	bool bl_move = false;
	if(GetAttributes()->autohide == ATTRIBUTES_HIDE_NOT)
	{
		if(x < 0)
		{
				x = 0;
				bl_move = true;
		}
		else if(x+w > g_GameRect.right)
		{
				x = g_GameRect.right-w;
				bl_move = true;
		}
		if(y < 0)
		{
				y = 0;
				bl_move = true;
		}
		else if(y+h > g_GameRect.bottom)
		{
				y = g_GameRect.bottom-h;
				bl_move = true;
		}
		
		if(bl_move)WindowEventReceiver(EVENT_WINDOW_MOVE);
		return;
	}

	if(IsPixel(gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY()) && (gpC_window_manager->GetMouseFocusedWindow() == this || alpha_window))
	// 마우스가 위에 있으면 열린다.
	{
		if(x < 0)
		{
			if(g_pUserOption->AutoHideSmoothScroll)
				x = min(0, x+HIDE_SPEED);
			else
				x = 0;
			bl_move = true;
		}
		else if(x+w > g_GameRect.right)
		{
			if(g_pUserOption->AutoHideSmoothScroll)
				x = max(g_GameRect.right-w, x-HIDE_SPEED);
			else
				x = g_GameRect.right-w;
			bl_move = true;
		}
		if(y < 0)
		{
			if(g_pUserOption->AutoHideSmoothScroll)
				y = min(0, y+HIDE_SPEED);
			else
				y = 0;
			bl_move = true;
		}
		else if(y+h > g_GameRect.bottom)
		{
			if(g_pUserOption->AutoHideSmoothScroll)
				y = max(g_GameRect.bottom-h, y-HIDE_SPEED);
			else
				y = g_GameRect.bottom-h;
			bl_move = true;
		}
	}
	else
	// 아니면 닫히든가-_-;
	{
		if(!(GetAttributes()->autohide == ATTRIBUTES_HIDE_HEIGHT
			&& (y <= 0 && y+h > gap || y+h >= g_GameRect.bottom && y < g_GameRect.bottom-gap))
			&& (x <= 0 && x+w > gap || x+w >= g_GameRect.right && x < g_GameRect.right-gap)
			&& y+h != gap && y != g_GameRect.bottom-gap)
		{
			if(x <= 0 && x+w > gap)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					x = max(-(w-gap), x-HIDE_SPEED);
				else
					x = -(w-gap);
			}
			else if(x+w >= g_GameRect.right && x < g_GameRect.right-gap)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					x = min(g_GameRect.right-gap, x+HIDE_SPEED);
				else
					x = g_GameRect.right-gap;
			}
			else if(x+w < gap) x = gap-w;
			else if(x > g_GameRect.right-gap)x = g_GameRect.right-gap;
			// 열린다
			if(y < 0)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					y = min(0, y+HIDE_SPEED);
				else
					y = 0;
			}
			else if(y+h > g_GameRect.bottom)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					y = max(g_GameRect.bottom-h, y-HIDE_SPEED);
				else
					y = g_GameRect.bottom-h;
			}

			bl_move = true;
		}
		else if(x+w != gap && x != g_GameRect.right-gap)
		{
			if(y <= 0 && y+h > gap)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					y = max(-(h-gap), y-HIDE_SPEED);
				else
					y = -(h-gap);
			}
			else if(y+h >= g_GameRect.bottom && y < g_GameRect.bottom-gap)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					y = min(g_GameRect.bottom-gap, y+HIDE_SPEED);
				else
					y = g_GameRect.bottom-gap;
			}
			else if(y+h < gap) y = gap-h;
			else if(y > g_GameRect.bottom-gap)y = g_GameRect.bottom-gap;
			// 열린다
			if(x < 0)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					x = min(0, x+HIDE_SPEED);
				else
					x = 0;
			}
			else if(x+w > g_GameRect.right)
			{
				if(g_pUserOption->AutoHideSmoothScroll)
					x = max(g_GameRect.right-w, x-HIDE_SPEED);
				else
					x = g_GameRect.right-w;
			}
			bl_move = true;
		}
	}
	if(bl_move)WindowEventReceiver(EVENT_WINDOW_MOVE);

}

//-----------------------------------------------------------------------------
// AttrPin
//
// Window에 pin꽂기 여부를 설정한다.
// pin을 꽂으면 pin을 꽂지 않은 Window(WindowManager::m_show_list)보다 높은 
// 우선순위에 놓인다. pin을 꽂은 Window(WindowManager::m_show_list_pinned_window)는 
// 그들끼리 우선순위를 결정한다.
//-----------------------------------------------------------------------------
void Window::AttrPin(bool new_state)
{
	if (gpC_window_manager->GetShowState(this) == true)
	{
		// Window가 보여지고 있는 상태이면 이전 상태의 show_list를 해제하고 new_state의
		// show_list에 저장한다.
		gpC_window_manager->DisappearWindow(this);

		m_attributes.pin = new_state;
		gpC_window_manager->AppearWindow(this);
	}
	else
	{
		// 보여지고 있지 않은 상태면 그냥 attr만 바꾼다. 이것은 다음 AppearWindow가
		// 실행되면서 show_list에 정상적으로 저장될 것이다.
		m_attributes.pin = new_state;
	}
}

//-----------------------------------------------------------------------------
// ClearInputState
//
// 외부에서 Window의 입력상태를 clear한다.
//-----------------------------------------------------------------------------
void Window::ClearInputState()
{
	MoveOk();
	m_u_mouse_state = MOUSE_NOSTATE;
}

//-----------------------------------------------------------------------------
// Init
//
// 
//-----------------------------------------------------------------------------
void Window::Init()
{
	InitAttributes();

	m_bl_window_move_ready = false;
	m_u_mouse_state = MOUSE_NOSTATE;
}

//-----------------------------------------------------------------------------
// GetMouseInputState
//
// 
//-----------------------------------------------------------------------------
MOUSE_STATE Window::GetMouseInputState()
{
	MOUSE_STATE state = m_u_mouse_state;
	m_u_mouse_state = MOUSE_NOSTATE;

	return state;
}

//-----------------------------------------------------------------------------
// MoveReady
//
// Window가 이동 준비 되었음을 알린다. 이것을 실행해야 Move를 할 수 있다.
//-----------------------------------------------------------------------------
void Window::MoveReady()
{
	if (m_attributes.window_move == true)
	{
		m_bl_window_move_ready = true;
	}
}

//-----------------------------------------------------------------------------
// MoveOk
//
// Window 이동이 끝났다.
//-----------------------------------------------------------------------------
void Window::MoveOk()
{
	m_bl_window_move_ready = false;
}

//-----------------------------------------------------------------------------
// SetOrigin
//
// 이동중심설정, 즉 원점의 위치를 설정한다.
// 어떤 target의 원점을 이동함으로써 그 target 전체가 이동하는 효과를 볼 수 있다.
//
// 2차원 도형의 Window에서는 임의의 (x, y)를 원점으로 설정할 수 있다.
//-----------------------------------------------------------------------------
void Window::SetOrigin(int ox, int oy)
{
	m_origin.Set(ox, oy);
}

//-----------------------------------------------------------------------------
// Move
//
// Origin의 변화량을 적용하여 Window를 이동시킨다.
// 이동하였으면 true를, 아니면 false를 반환한다.
//-----------------------------------------------------------------------------
bool Window::Move(int new_ox, int new_oy)
{
	if (m_bl_window_move_ready)
	{
		x = new_ox-m_origin.x;
		y = new_oy-m_origin.y;

		if(GetAttributes()->statch)
		{
			if(x < STATCH_VALUE && x > -STATCH_VALUE)
			{
				x = 0;
			}
			else if(x+w > g_GameRect.right-STATCH_VALUE && x+w < g_GameRect.right+STATCH_VALUE)
			{
				x = g_GameRect.right-w;
			}
			
			if(y < STATCH_VALUE && y > -STATCH_VALUE)
			{
				y = 0;
			}
			else if(y+h > g_GameRect.bottom-STATCH_VALUE && y+h < g_GameRect.bottom+STATCH_VALUE)
			{
				y = g_GameRect.bottom-h;
			}
		}
//		x += new_ox - m_origin.x;
//		y += new_oy - m_origin.y;

//		m_origin.Set(new_ox, new_oy);

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// MouseControl
//
// Window 전용 Mouse control.
// (x, y)는 항상 Window 내부에 있다.
//-----------------------------------------------------------------------------
bool Window::MouseControl(UINT message, int _x, int _y)
{
	switch (message)
	{
		case M_MOVING:
			if (Move(_x, _y) == true)
			{

				// 성공적으로 이동되었다. 따라서 WindowManager에게 이 사실을 알려야 한다.
				m_u_mouse_state = MOUSE_MOVE;
			}
			break;

		case M_LEFTBUTTON_UP:
			MoveOk();
			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// Window 전용 Keyboard control.
//-----------------------------------------------------------------------------
void Window::KeyboardControl(UINT message, UINT key, long extra)
{
	gC_ci->IME_MessageProcessor(message, key, extra);
}

//-----------------------------------------------------------------------------
// WindowManager
//
// 
//-----------------------------------------------------------------------------
WindowManager::WindowManager()
{
	m_show_list.clear();
	m_show_list_pinned_window.clear();

	m_pC_mouse_click_window = NULL;
	m_pC_topmost_window = NULL;
	m_pC_keydown_window = NULL;
	m_pC_keyboard_control_window = NULL;
	m_pC_mouse_focused_window = NULL;
	m_pC_pushed_window = NULL;
	m_old_origin_x = 0;
	m_old_origin_y = 0;
	m_res_x = 0;
	m_res_y = 0;
	m_fp_handler_acquire_mousefocus = NULL;
	m_fp_handler_unacquire_mousefocus = NULL;
	m_fp_handler_acquire_disappear = NULL;
}

//-----------------------------------------------------------------------------
// ~WindowManager
//
// 
//-----------------------------------------------------------------------------
WindowManager::~WindowManager()
{

}

//-----------------------------------------------------------------------------
// SetKeyboardControlWindow
//
// window keyboard control attr을 검색하여 m_pC_keyboard_control_window를 설정한다.
//-----------------------------------------------------------------------------
void WindowManager::SetKeyboardControlWindow(Window * p_window)
{
	if (p_window != NULL)
	{
		if (p_window->GetAttributes()->keyboard_control == true)
			m_pC_keyboard_control_window = p_window;
	}
	else
		m_pC_keyboard_control_window = NULL;
}

//-----------------------------------------------------------------------------
// SetNextKeyboardControlWindow
//
// 
//-----------------------------------------------------------------------------
void WindowManager::SetNextKeyboardControlWindow()
{
	m_pC_keyboard_control_window = NULL;

	if (m_pC_topmost_window == NULL)
	{
		List::iterator itr;

		itr = m_show_list_pinned_window.begin();
		while (itr != m_show_list_pinned_window.end())
		{
			if ((*itr)->GetAttributes()->keyboard_control == true)
			{
				m_pC_keyboard_control_window = *itr;
				return;
			}

			itr++;
		}

		itr = m_show_list.begin();
		while (itr != m_show_list.end())
		{
			if ((*itr)->GetAttributes()->keyboard_control == true)
			{
				m_pC_keyboard_control_window = *itr;
				return;
			}

			itr++;
		}
	}
}

//-----------------------------------------------------------------------------
// SetNextTopmostWindow
//
// Window topmost attr을 검색하여 m_pC_topmost_window를 설정한다.
//-----------------------------------------------------------------------------
void WindowManager::SetNextTopmostWindow()
{
	// ! topmost Window이면 pinned Window가 아니다!!!

	List::iterator itr;

	itr = m_show_list.begin();
	while (itr != m_show_list.end())
	{
		if ((*itr)->GetAttributes()->topmost == true)
		{
			m_pC_topmost_window = *itr;
			return;
		}

		itr++;
	}

	m_pC_topmost_window = NULL;
}

//-----------------------------------------------------------------------------
// CancelPushStateOfCurrentPushedWindow
//
// 현재 눌린 Window(이동하기 위해서든, ...)의 눌림상태를 해제한다.
// 이것은 외부에서 pushed state를 한 번에 clear하기 위한 것이다.
//
// 눌린 상태에 있다면 true를 반환한다.
//-----------------------------------------------------------------------------
bool WindowManager::CancelPushStateOfCurrentPushedWindow()
{
	if (m_pC_pushed_window != NULL)
	{
		m_pC_pushed_window->CancelPushState();
		m_pC_pushed_window = NULL;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// ShowedWindowSize
//
// 
//-----------------------------------------------------------------------------
int WindowManager::ShowedWindowSize() const 
{ 
	return m_show_list.size()+m_show_list_pinned_window.size(); 
}

//-----------------------------------------------------------------------------
// GetSequence
//
// p_this_window의 우선순위를 반환한다. 0에 가까울수록 우선순위가 높다.
// 실패하면, -1을 반환한다.
//-----------------------------------------------------------------------------
int WindowManager::GetSequence(Window * p_this_window)
{
	if (m_pC_topmost_window != NULL && m_pC_topmost_window == p_this_window)
		return 0;

	int sequence = 0;
	List::iterator itr;

	itr = m_show_list_pinned_window.begin();
	while (itr != m_show_list_pinned_window.end())
	{
		if (*itr == p_this_window)
		{
			return sequence;
		}

		itr++;
		sequence++;
	}
	
	itr = m_show_list.begin();
	while (itr != m_show_list.end())
	{
		if (*itr == p_this_window)
		{
			return sequence;
		}

		itr++;
		sequence++;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// SendMouseMoveMessageToMouseFocusedWindow
//
// 
//-----------------------------------------------------------------------------
void WindowManager::SendMouseMoveMessageToMouseFocusedWindow()
{
	if (m_pC_mouse_focused_window != NULL)
		m_pC_mouse_focused_window->MouseControl(M_MOVING, m_current_mouse_x, m_current_mouse_y);
}

//-----------------------------------------------------------------------------
// GetFirstPriorityWindow
//
// 최우선 Window를 반환한다.
// 보여지는 Window가 없으면 NULL을 반환한다.
//-----------------------------------------------------------------------------
Window * WindowManager::GetFirstPriorityWindow() const
{
	if (m_pC_topmost_window != NULL)
		return m_pC_topmost_window;

	if (m_show_list_pinned_window.empty() == false)
	{
		return m_show_list_pinned_window.front();
	}
	else
	{
		if (m_show_list.empty() == false)
			return m_show_list.front();
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// GetMovingWindow
//
// 현재 이동중인 Window의 pointer를 반환한다. 이것은 어떤 Window가 현재 이동하고
// 있는지 외부에서 알기위한 것이다.
//
// !Window는 한 순간에 단 하나만 이동할 수 있으며 그것은 '보이는' 것이다. 또한
// 그것은 최우선 Window이다.
//-----------------------------------------------------------------------------
Window * WindowManager::GetMovingWindow() const
{
	Window * p_window;

	if (m_show_list_pinned_window.empty() == false)
	{
		p_window = m_show_list_pinned_window.front();
		if (p_window->Moving() == true)
			return p_window;
	}

	if (m_show_list.empty() == false)
	{
		p_window = m_show_list.front();
		if (p_window->Moving() == true)
			return p_window;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// FirstPriority
//
// p_this_window를 최우선순위로 한다.
// p_this_window가 list에 존재하지 않으면 아무것도 하지 않는다.
//-----------------------------------------------------------------------------
void WindowManager::FirstPriority(Window * p_this_window)
{
	assert(p_this_window != NULL);

	// failed?
	if (p_this_window == NULL)
		 return;

	List::iterator itr;
	
	if (p_this_window->GetAttributes()->pin == true)
	{
		itr = m_show_list_pinned_window.begin();
		while (itr != m_show_list_pinned_window.end())
		{
			if (*itr == p_this_window)
			{
				m_show_list_pinned_window.erase(itr);
				m_show_list_pinned_window.push_front(p_this_window);
				break;
			}

			itr++;
		}
	}
	else
	{
		itr = m_show_list.begin();
		while (itr != m_show_list.end())
		{
			if (*itr == p_this_window)
			{
				m_show_list.erase(itr);
				m_show_list.push_front(p_this_window);
				break;
			}

			itr++;
		}	
	}
}

//-----------------------------------------------------------------------------
// Show
//
// m_show_list/m_show_list_pinned_window에 있는 Window만 Window::Show()를 실행한다.
//-----------------------------------------------------------------------------
void WindowManager::Show()
{
	List::reverse_iterator itr;

	itr = m_show_list.rbegin();
	while (itr != m_show_list.rend())
	{
		if ((*itr)->GetAttributes()->topmost == false)
			(*itr)->Show();

		itr++;
	}

	// pinned Window가 no pinned Window 보다 위에 출력한다.
	itr = m_show_list_pinned_window.rbegin();
	while (itr != m_show_list_pinned_window.rend())
	{
		Window *pWindow = *itr;
//		(*itr)->Show();
		pWindow->Show();

		itr++;
	}

	// show topmost
	itr = m_show_list.rbegin();
	while (itr != m_show_list.rend())
	{
		if ((*itr)->GetAttributes()->topmost == true)
			(*itr)->Show();

		itr++;
	}
}

//-----------------------------------------------------------------------------
// AppearWindow
//
// p_this_window를 show list에 등록한다. 이미 되어 있으면 현재 node를 delete하고
// 다시 insert한다.
//
// p_this_window가 등록되어 있지 않으면 아무것도 하지 않는다.
//-----------------------------------------------------------------------------
void WindowManager::AppearWindow(Window * p_this_window)
{
	assert(p_this_window != NULL);

	// failed?
	if (p_this_window == NULL ||
		 AlreadyRegistered(p_this_window) == false)
		 return;

	Window * p_prev_first_window = GetFirstPriorityWindow();

	List::iterator itr;

	//
	// 이미 존재하면 삭제한다.
	//

	if (p_this_window->GetAttributes()->pin == true)
	{
		itr = m_show_list_pinned_window.begin();
		while (itr != m_show_list_pinned_window.end())
		{
			if (*itr == p_this_window)
			{
				m_show_list_pinned_window.erase(itr);
				break;
			}

			itr++;
		}

		m_show_list_pinned_window.push_front(p_this_window);
	}
	else
	{
		itr = m_show_list.begin();
		while (itr != m_show_list.end())
		{
			if (*itr == p_this_window)
			{
				m_show_list.erase(itr);
				break;
			}

			itr++;
		}

		m_show_list.push_front(p_this_window);
	}

	//
	// cancel previous focused Window focus & focus new Window
	//
	Window * pC_prev_focused_window = m_pC_mouse_focused_window;
	SetMouseMoveFocusedWindow();
	if (pC_prev_focused_window != m_pC_mouse_focused_window)
	{
		if (pC_prev_focused_window != NULL)
		{
			pC_prev_focused_window->UnacquireMouseFocus();
			RunUnacquireMouseFocusHandler();
		}
		if (m_pC_mouse_focused_window != NULL)
		{
			m_pC_mouse_focused_window->AcquireMouseFocus();
			RunAcquireMouseFocusHandler();
		}
	}

	SendMouseMoveMessageToMouseFocusedWindow();

	SetNextTopmostWindow();

	// new first sequence Window detection?
	Window * p_first_window = GetFirstPriorityWindow();
	if (p_first_window != NULL && p_first_window != p_prev_first_window)
	{
		p_first_window->AcquireFirstSequence();
		SetKeyboardControlWindow(p_first_window);
	}
}

//-----------------------------------------------------------------------------
// DisappearWindow
//
// p_this_window를 show list에서 delete한다.
// p_this_window가 Disappear되었으면 true를 반환한다.
//-----------------------------------------------------------------------------
bool WindowManager::DisappearWindow(Window * p_this_window)
{
	assert(p_this_window != NULL);

	if (p_this_window == NULL)
		return false;

	if (m_pC_keydown_window==p_this_window)
	{
		m_pC_keydown_window = NULL;
	}
	if (m_pC_topmost_window == p_this_window)
	{
		m_pC_topmost_window = NULL;
	}

	Window * p_prev_first_window = GetFirstPriorityWindow();

	bool disappear_result = false;
	List::iterator itr;

	if (p_this_window->GetAttributes()->pin == true)
	{
		itr = m_show_list_pinned_window.begin();
		while (itr != m_show_list_pinned_window.end())
		{
			if (*itr == p_this_window)
			{
				p_this_window->ClearInputState();

				m_show_list_pinned_window.erase(itr);
				disappear_result = true;
				break;
			}

			itr++;
		}	
	}
	else
	{
		itr = m_show_list.begin();
		while (itr != m_show_list.end())
		{
			if (*itr == p_this_window)
			{
				p_this_window->ClearInputState();

				m_show_list.erase(itr);
				disappear_result = true;
				break;
			}

			itr++;
		}	
	}

	if (disappear_result == true)
	{
		// cancel previous focused Window focus & focus new Window
		Window * pC_prev_focused_window = m_pC_mouse_focused_window;
		SetMouseMoveFocusedWindow();
		if (pC_prev_focused_window != m_pC_mouse_focused_window)
		{
			if (pC_prev_focused_window != NULL)
			{
				pC_prev_focused_window->UnacquireMouseFocus();
				RunUnacquireMouseFocusHandler();
			}
			if (m_pC_mouse_focused_window != NULL)
			{
				m_pC_mouse_focused_window->AcquireMouseFocus();
				RunAcquireMouseFocusHandler();
			}
		}

		p_this_window->AcquireDisappear();
		RunAcquireDisappearHandler();

		SendMouseMoveMessageToMouseFocusedWindow();

		if (p_this_window == m_pC_pushed_window)
			m_pC_pushed_window = NULL;

		SetNextTopmostWindow();

		// new first sequence Window detection?
		Window * p_first_window = GetFirstPriorityWindow();
		if (p_first_window != NULL && p_first_window != p_prev_first_window)
			p_first_window->AcquireFirstSequence();

		//
		// DisappearWindow가 아닌 경우에는 확실히 SetKeyboardControlWindow로 keyboard control Window를
		// 설정할 수 있지만, Disappear 되는 경우에는 순차적으로 검색하여 그것을 찾아야 한다.
		//
		SetNextKeyboardControlWindow();
	}

	return disappear_result;
}

//-----------------------------------------------------------------------------
// SetMouseMoveFocusedWindow
//
// 최우선순위 Window부터 검사하여 mouse (x, y)가 Window에 위치하면 move focused
// Window를 설정한다.
//-----------------------------------------------------------------------------
void WindowManager::SetMouseMoveFocusedWindow()
{
	List::iterator itr, endItr;

	// topmost first
	// "topmost Window는 pinned Window가 아니다."
	itr = m_show_list.begin();
	while (itr != m_show_list.end())
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->GetAttributes()->topmost == true)
			if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
			{
				m_pC_mouse_focused_window = p_searched_window;
				return;
			}

		itr++;
	}

	itr = m_show_list_pinned_window.begin();
	endItr = m_show_list_pinned_window.end();
	while (itr != endItr)
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
		{
			m_pC_mouse_focused_window = p_searched_window;
			return;
		}

		itr++;
	}
	
	itr = m_show_list.begin();

	endItr = m_show_list.end();
	while (itr != endItr)
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->GetAttributes()->topmost == false)
		{
			if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
			{
				m_pC_mouse_focused_window = p_searched_window;
				return;
			}
		}			
		itr++;
	}

	m_pC_mouse_focused_window = NULL;
}

//-----------------------------------------------------------------------------
// GetFocusedWindow
//
// 최우선순위 Window부터 검사하여 mouse (x, y)가 Window에 위치하면 move focused
// Window를 설정한다.
//-----------------------------------------------------------------------------
Window* WindowManager::GetFocusedWindow(int x, int y)
{
	List::iterator itr;

	// topmost first
	// "topmost Window는 pinned Window가 아니다."
	itr = m_show_list.begin();
	while (itr != m_show_list.end())
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->GetAttributes()->topmost == true)
			if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
			{
				return p_searched_window;
			}

		itr++;
	}

	itr = m_show_list_pinned_window.begin();
	while (itr != m_show_list_pinned_window.end())
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
		{
			return p_searched_window;
		}

		itr++;
	}
	
	itr = m_show_list.begin();
	while (itr != m_show_list.end())
	{
		Window * p_searched_window = (*itr);
		if (p_searched_window->GetAttributes()->topmost == false)
			if (p_searched_window->IsPixel(m_current_mouse_x, m_current_mouse_y))
			{
				return p_searched_window;
			}

		itr++;
	}

	return NULL;
}
//-----------------------------------------------------------------------------
// MouseControl
//
// WindowManager::MouseControl은 App MouseControl과 Window::MouseControl의
// 중계자이다.
//
// > Mouse pointer move시 mouse pointer가 어떤 Window를 가리키면 focus 대기상태에
//   들어가게 한다. 이것은 우선순위에 상관없다. 물론 우선순위가 높은 것부터
//   검색한다.
//-----------------------------------------------------------------------------
bool WindowManager::MouseControl(UINT message, int x, int y)
{
	m_current_mouse_x = x;
	m_current_mouse_y = y;

	//
	// change move focused Window
	//
	if (message == M_MOVING)
	{
		// 이동중에는 move focused Window 변경하지 않는다.
		if (gpC_Imm && m_pC_mouse_focused_window != NULL && m_pC_mouse_focused_window->Moving())
			gpC_Imm->ForceUI(CImm::FORCE_UI_DRAG);

		if (m_pC_mouse_focused_window == NULL || m_pC_mouse_focused_window->Moving() == false)
		{
			//
			// Acquire/Unacquire mouse focus
			//
			Window * pC_prev_focused_window = m_pC_mouse_focused_window;

			SetMouseMoveFocusedWindow();

			if (pC_prev_focused_window != m_pC_mouse_focused_window)
			{
				if(gpC_Imm)
					gpC_Imm->ForceUI(CImm::FORCE_UI_WINDOW);
				if (pC_prev_focused_window != NULL)
				{
					pC_prev_focused_window->UnacquireMouseFocus();
					RunUnacquireMouseFocusHandler();
				}
				if (m_pC_mouse_focused_window != NULL)
				{
					m_pC_mouse_focused_window->AcquireMouseFocus();
					RunAcquireMouseFocusHandler();
				}
			}
		}
	}

	//
	// change Window priority
	// topmost Window가 있을 경우에는 우선순위를 변경할 수 없다.
	//
	if (message == M_LEFTBUTTON_DOWN || message == M_RIGHTBUTTON_DOWN)
	{
		if (m_pC_mouse_focused_window != NULL)
		{
			if (m_pC_topmost_window == NULL)
			{
				Window * p_prev_first_window = GetFirstPriorityWindow();

				FirstPriority(m_pC_mouse_focused_window);
				
				Window * p_first_window = GetFirstPriorityWindow();
				if (p_prev_first_window != p_first_window)
				{
					p_first_window->AcquireFirstSequence();
					SetKeyboardControlWindow(p_first_window);
				}

				// acquire mouse click
				if (m_pC_mouse_click_window != m_pC_mouse_focused_window)
				{
					m_pC_mouse_click_window = m_pC_mouse_focused_window;
					m_pC_mouse_click_window->AcquireMouseClick();
					SetKeyboardControlWindow(m_pC_mouse_click_window);
				}
			}

			// set pushed Window
			m_pC_pushed_window = m_pC_mouse_focused_window;
		}
	}

	// focused Window에게 모든 입력을 준다.
	if (m_pC_mouse_focused_window != NULL)
	{
		if (m_pC_topmost_window != NULL)
		{
			if (m_pC_topmost_window == m_pC_mouse_focused_window)
				m_pC_topmost_window->MouseControl(message, x, y);
		}
		else
		{
			m_pC_mouse_focused_window->MouseControl(message, x, y);

			//창끼리 statch
			if(message == M_MOVING && m_pC_mouse_focused_window->Moving())
			{
				if(m_pC_mouse_focused_window->GetAttributes()->statch)
				{
					List::const_iterator itr;
					
					for(int k = 0; k < 2; k++)
					{
						if(k == 0)
							itr = m_show_list_pinned_window.begin();
						else if(k == 1)
							itr = m_show_list.begin();
						
						while (k ==0 && itr != m_show_list_pinned_window.end() || k ==1 && itr != m_show_list.end())
						{
							//						m_pC_mouse_focused_window->UnStatch();
							Window * p_searched_window = (*itr);
							
							bool bl_statch = false;
							
							//세로 범위가 어느정도 포함되있을때 가로를 땡겨서 붙인다
							if(m_pC_mouse_focused_window->y < p_searched_window->y+p_searched_window->h &&
								m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y)
							{
								//왼쪽에 붙을게 있나?
								if (m_pC_mouse_focused_window->x > p_searched_window->x+p_searched_window->w-STATCH_VALUE &&
									m_pC_mouse_focused_window->x < p_searched_window->x+p_searched_window->w+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->x = p_searched_window->x+p_searched_window->w;
									bl_statch = true;
								}
								else //오른쪽에 붙을게 있나?
									if (m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w > p_searched_window->x-STATCH_VALUE &&
										m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w < p_searched_window->x+STATCH_VALUE)
									{
										m_pC_mouse_focused_window->x = p_searched_window->x-m_pC_mouse_focused_window->w;
										bl_statch = true;
									}
									
									if(bl_statch)	//어딘가 붙었다면 그 끝과 끝을 붙일수 있나 본다
									{
										//위쪽의 좌표를 같게하자
										if(m_pC_mouse_focused_window->y > p_searched_window->y-STATCH_VALUE &&
											m_pC_mouse_focused_window->y < p_searched_window->y+STATCH_VALUE)
										{
											m_pC_mouse_focused_window->y = p_searched_window->y;
										}
										else	//아래쪽의 좌표를 같게하자
											if(m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y+p_searched_window->h-STATCH_VALUE &&
												m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h < p_searched_window->y+p_searched_window->h+STATCH_VALUE)
											{
												m_pC_mouse_focused_window->y = p_searched_window->y+p_searched_window->h-m_pC_mouse_focused_window->h;
											}
									}
									
							}
							
							bl_statch = false;
							//가로 범위가 어느정도 포함되있을때 세로를 땡겨서 붙인다
							if (m_pC_mouse_focused_window->x < p_searched_window->x+p_searched_window->w &&
								m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w > p_searched_window->x)
							{
								//위쪽에 붙을게 있나?
								if (m_pC_mouse_focused_window->y > p_searched_window->y+p_searched_window->h-STATCH_VALUE &&
									m_pC_mouse_focused_window->y < p_searched_window->y+p_searched_window->h+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->y = p_searched_window->y+p_searched_window->h;
									bl_statch = true;
								}
								else //아래쪽에 붙을게 있나?
									if (m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y-STATCH_VALUE &&
										m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h < p_searched_window->y+STATCH_VALUE)
									{
										m_pC_mouse_focused_window->y = p_searched_window->y-m_pC_mouse_focused_window->h;
										bl_statch = true;
									}
							}
							if(bl_statch == true)	//어딘가 붙었다면 그 끝과 끝을 붙일수 있나 본다
							{
								//왼쪽의 좌표를 같게하자
								if(m_pC_mouse_focused_window->x > p_searched_window->x-STATCH_VALUE &&
									m_pC_mouse_focused_window->x < p_searched_window->x+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->x = p_searched_window->x;
								}
								else	//오른쪽의 좌표를 같게하자
									if(m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w > p_searched_window->x+p_searched_window->w-STATCH_VALUE &&
										m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w < p_searched_window->x+p_searched_window->w+STATCH_VALUE)
									{
										m_pC_mouse_focused_window->x = p_searched_window->x+p_searched_window->w-m_pC_mouse_focused_window->w;
									}
							}
							
							itr++;
						}
					}
				}

				// event
				m_pC_mouse_focused_window->WindowEventReceiver(Window::EVENT_WINDOW_MOVE);
				
			
			}
		}
	}

	//
	// clear mouse push state.
	//
	if (message == M_LEFTBUTTON_UP ||
		 message == M_RIGHTBUTTON_UP)
	{
		//
		// - 현재 최우선Window에서 push한 후 다른 Window 또는 background에 push up했을 경우
		//   push한 Window에 push up을 해줘야 한다.
		//
		// - pinned Window와 no pinned Window가 있으므로 pushed Window가 first priority라고
		//   장담할 수 없다.

		// mouse move focus가 Window priority를 바꾸는 모드이면 다르게 처리해야 한다.
		//if (mouse_move_focus_change_window_priority?)
		//{
		//
		//}
		//else
		if (m_pC_pushed_window != NULL)
		{
			if (m_pC_pushed_window != m_pC_mouse_focused_window)
				m_pC_pushed_window->MouseControl(message, x, y);

			m_pC_pushed_window = NULL;
		}

	}

	return (m_pC_mouse_focused_window != NULL);
}

#include "VS_UI.h"
//-----------------------------------------------------------------------------
// KeyboardControl
//
// WindowManager::KeyboardControl은 App KeyboardControl과 Window::KeyboardControl의 
// 중계자이다.
//-----------------------------------------------------------------------------
void WindowManager::KeyboardControl(UINT message, UINT key, long extra)
{
//	static Window * m_pC_keydown_window;

	if (message == WM_KEYDOWN)// || message == WM_IME_STARTCOMPOSITION)
	{
		m_pC_keydown_window = NULL;
		if (m_pC_topmost_window != NULL)// && m_pC_topmost_window->GetAttributes()->keyboard_control == true)
			m_pC_keydown_window = m_pC_topmost_window;
		else if (m_pC_keyboard_control_window != NULL)
			m_pC_keydown_window = m_pC_keyboard_control_window;
	}

	//
	// 우선순위, (1) topmost Window
	//			 	 (2) keyboard control Window
	//
	if (m_pC_topmost_window != NULL)
	{
//		if (message == WM_KEYDOWN)
//		{
//			if (m_pC_topmost_window != m_pC_keydown_window)
//				return;
//		}

		if (m_pC_topmost_window == m_pC_keydown_window)
		{

		//if(key != VK_ESCAPE && gC_vs_ui.IsGameMode() || !gC_vs_ui.IsGameMode())
		if(m_pC_topmost_window->GetAttributes()->keyboard_control == true || gC_vs_ui.IsEmptyChatting() && key == VK_RETURN || !gC_vs_ui.IsGameMode() && key == VK_ESCAPE)
		{
			m_pC_topmost_window->KeyboardControl(message, key, extra);
			return;
		}

		//게임모드가 아닐때는 아래로 내려줄 필요가 없다
		if(!gC_vs_ui.IsGameMode())
			return;
		}
	}

	// topmost window가 떠있을때도 채팅먹게 하기 위해서
	if (m_pC_keyboard_control_window != NULL && m_pC_keyboard_control_window != m_pC_topmost_window)
		m_pC_keyboard_control_window->KeyboardControl(message, key, extra);

//	if (m_pC_keydown_window != NULL)
//	{
//		if (message == WM_KEYDOWN)
//		{
//			if(m_pC_keyboard_control_window != m_pC_keydown_window && 
//				m_pC_keydown_window->GetAttributes()->keyboard_control == true ||
//				m_pC_keyboard_control_window == m_pC_topmost_window ||
//				m_pC_keyboard_control_window == NULL)
//				return;
//			if (m_pC_keyboard_control_window != m_pC_keydown_window)
//				return;
//		}

//		if(
//			m_pC_topmost_window != NULL && 
//			m_pC_topmost_window->GetAttributes()->keyboard_control == true ||
//			m_pC_keyboard_control_window == m_pC_topmost_window ||
//			m_pC_keyboard_control_window == NULL
//			)
//			return;

//		if (m_pC_keyboard_control_window != m_pC_keydown_window)
//			return;

//	}
}

//-----------------------------------------------------------------------------
// AlreadyRegistered
//
// p_window가 등록되었는가?
//-----------------------------------------------------------------------------
bool WindowManager::AlreadyRegistered(Window * p_window) const
{
	return Find(p_window);
}

//-----------------------------------------------------------------------------
// GetShowState
//
// 
//-----------------------------------------------------------------------------
bool WindowManager::GetShowState(Window * p_window) const
{
	if (p_window == NULL)
		return false;

	List::const_iterator itr;

	if (p_window->GetAttributes()->pin == true)
	{
		itr = m_show_list_pinned_window.begin();
		while (itr != m_show_list_pinned_window.end())
		{
			if (*itr == p_window)
				return true;

			itr++;
		}
	}
	else
	{
		itr = m_show_list.begin();
		while (itr != m_show_list.end())
		{
			if (*itr == p_window)
				return true;

			itr++;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Register
//
// Window를 등록한다.
// 이미 등록되었는가 검사할 필요가 없다.
//-----------------------------------------------------------------------------
void WindowManager::Register(Window * p_window)
{
	if (p_window == NULL)
		_Error(NULL_REF);

	Add(p_window);
}

//-----------------------------------------------------------------------------
// Unregister
//
// Window 등록을 해제한다.
//-----------------------------------------------------------------------------
void WindowManager::Unregister(Window * p_window)
{
	if (p_window == NULL)
		_Error(NULL_REF);

	if (Delete(p_window) == true)
		DisappearWindow(p_window); // 현재상태는 확인할 필요없이 무조건 disappear.

	if (p_window == m_pC_pushed_window)
		m_pC_pushed_window = NULL;
}

//-----------------------------------------------------------------------------
// Process
//
//
//-----------------------------------------------------------------------------
void WindowManager::Process()
{
}

//-----------------------------------------------------------------------------
// g_RegisterWindow
//
// Window를 Window Manager에 등록한다.
//-----------------------------------------------------------------------------
void g_RegisterWindow(Window * p_window)
{
	if (gpC_window_manager == NULL)
		_ErrorStr("WindowManager not initialized.");
	if (p_window == NULL)
		_Error(NULL_REF);

	gpC_window_manager->Register(p_window);
}

//-----------------------------------------------------------------------------
// g_UnregisterWindow
//
// Window Manager에서 Window를 등록해제한다.
//-----------------------------------------------------------------------------
void g_UnregisterWindow(Window * p_window)
{
	if (gpC_window_manager == NULL)
		_ErrorStr("WindowManager not initialized.");
	if (p_window == NULL)
		_Error(NULL_REF);

	gpC_window_manager->Unregister(p_window);
}	