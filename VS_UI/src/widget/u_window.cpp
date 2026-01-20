// u_window.cpp
#include "Client_PCH.h"
#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#include "u_window.h"
#include <math.h>
#include "vs_ui.h"
#ifdef _WIN32
#include "CImm.h"
#endif
#include "UserOption.h"

#define STATCH_VALUE 10
#define HIDE_GAP	4
#define HIDE_SPEED	10
extern RECT g_GameRect;

//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------
WindowManager *	gpC_window_manager; // App���� �Ҵ��Ѵ�.

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
	// constructor/destructor������ pure virtual�� �Ұ����ϴ�. �ֱ׷�������
	// �𸣰ڴ�. �Ƹ� �׷����ϸ� instatance�� �Ұ����ϴٴ� ����... ��...
	// �׷��� �� event�� �� ������δ� �Ұ����ϴ�.
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
// Window ���� ���̴� Widget���� ����Ѵ�.
//
// !�̰��� �ܺ��� Show() ��ƾ ���������� ����Ǿ� �Ѵ�.
//-----------------------------------------------------------------------------
void Window::ShowWidget() const
{
	LineEditorVisual * data;
	for (int i=0; i < m_sdl_lev.Size(); i++)
		if (m_sdl_lev.Data(i, data))
			data->Show(); // ���ο��� dc�� Get/Release �Ѵ�.
}

//-----------------------------------------------------------------------------
// Attach
//
// p_lev�� Window�� ���δ�.
//-----------------------------------------------------------------------------
void Window::Attach(LineEditorVisual * p_lev)
{
	if (p_lev == NULL)
		_Error(NULL_REF);

	m_sdl_lev.Add(p_lev); // �̹� �����ϸ� �����̴�.
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
// topmost �Ӽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrTopmost(bool state)
{
	m_attributes.topmost = state;
}

//-----------------------------------------------------------------------------
// AttrKeyboardControl
//
// keyboard control �Ӽ��� �����Ѵ�.
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
// Window �̵� �Ӽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrWindowMove(bool new_state)
{ 
	m_attributes.window_move = new_state;

	// move ready�� true�� �� �� window_move attr�� false�� �� �� �ִ�.
	m_bl_window_move_ready = false;
}

//-----------------------------------------------------------------------------
// AttrAlpha
//
// Window �̵� �Ӽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrAlpha(bool new_state)
{ 
	m_attributes.alpha = new_state;
}

//-----------------------------------------------------------------------------
// AttrStatch
//
// Window �̵� �Ӽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrStatch(bool new_state)
{ 
	m_attributes.statch = new_state;
}

//-----------------------------------------------------------------------------
// AttrAutoHide
//
// Window �̵� �Ӽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrAutoHide(ATTRIBUTES_HIDE new_state)
{ 
	m_attributes.autohide = new_state;
}

//-----------------------------------------------------------------------------
// ProcessHide
//
// Window �̵� �Ӽ��� �����Ѵ�.
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
	// ���콺�� ���� ������ ������.
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
	// �ƴϸ� �����簡-_-;
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
			// ������
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
			// ������
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
// Window�� pin�ȱ� ���θ� �����Ѵ�.
// pin�� ������ pin�� ���� ���� Window(WindowManager::m_show_list)���� ���� 
// �켱������ ���δ�. pin�� ���� Window(WindowManager::m_show_list_pinned_window)�� 
// �׵鳢�� �켱������ �����Ѵ�.
//-----------------------------------------------------------------------------
void Window::AttrPin(bool new_state)
{
	if (gpC_window_manager->GetShowState(this) == true)
	{
		// Window�� �������� �ִ� �����̸� ���� ������ show_list�� �����ϰ� new_state��
		// show_list�� �����Ѵ�.
		gpC_window_manager->DisappearWindow(this);

		m_attributes.pin = new_state;
		gpC_window_manager->AppearWindow(this);
	}
	else
	{
		// �������� ���� ���� ���¸� �׳� attr�� �ٲ۴�. �̰��� ���� AppearWindow��
		// ����Ǹ鼭 show_list�� ���������� ����� ���̴�.
		m_attributes.pin = new_state;
	}
}

//-----------------------------------------------------------------------------
// ClearInputState
//
// �ܺο��� Window�� �Է»��¸� clear�Ѵ�.
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
// Window�� �̵� �غ� �Ǿ����� �˸���. �̰��� �����ؾ� Move�� �� �� �ִ�.
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
// Window �̵��� ������.
//-----------------------------------------------------------------------------
void Window::MoveOk()
{
	m_bl_window_move_ready = false;
}

//-----------------------------------------------------------------------------
// SetOrigin
//
// �̵��߽ɼ���, �� ������ ��ġ�� �����Ѵ�.
// � target�� ������ �̵������ν� �� target ��ü�� �̵��ϴ� ȿ���� �� �� �ִ�.
//
// 2���� ������ Window������ ������ (x, y)�� �������� ������ �� �ִ�.
//-----------------------------------------------------------------------------
void Window::SetOrigin(int ox, int oy)
{
	m_origin.Set(ox, oy);
}

//-----------------------------------------------------------------------------
// Move
//
// Origin�� ��ȭ���� �����Ͽ� Window�� �̵���Ų��.
// �̵��Ͽ����� true��, �ƴϸ� false�� ��ȯ�Ѵ�.
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
// Window ���� Mouse control.
// (x, y)�� �׻� Window ���ο� �ִ�.
//-----------------------------------------------------------------------------
bool Window::MouseControl(UINT message, int _x, int _y)
{
	switch (message)
	{
		case M_MOVING:
			if (Move(_x, _y) == true)
			{

				// ���������� �̵��Ǿ���. ���� WindowManager���� �� ����� �˷��� �Ѵ�.
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
// Window ���� Keyboard control.
//-----------------------------------------------------------------------------
void Window::KeyboardControl(UINT message, UINT key, long extra)
{
#ifdef PLATFORM_WINDOWS
	gC_ci->IME_MessageProcessor(message, key, extra);
#else
	// On macOS/SDL2, the CI::IME_MessageProcessor is a stub
	// Windows that need keyboard input should override this method
	(void)message;
	(void)key;
	(void)extra;
#endif
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
// window keyboard control attr�� �˻��Ͽ� m_pC_keyboard_control_window�� �����Ѵ�.
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
// Window topmost attr�� �˻��Ͽ� m_pC_topmost_window�� �����Ѵ�.
//-----------------------------------------------------------------------------
void WindowManager::SetNextTopmostWindow()
{
	// ! topmost Window�̸� pinned Window�� �ƴϴ�!!!

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
// ���� ���� Window(�̵��ϱ� ���ؼ���, ...)�� �������¸� �����Ѵ�.
// �̰��� �ܺο��� pushed state�� �� ���� clear�ϱ� ���� ���̴�.
//
// ���� ���¿� �ִٸ� true�� ��ȯ�Ѵ�.
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
// p_this_window�� �켱������ ��ȯ�Ѵ�. 0�� �������� �켱������ ����.
// �����ϸ�, -1�� ��ȯ�Ѵ�.
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
// �ֿ켱 Window�� ��ȯ�Ѵ�.
// �������� Window�� ������ NULL�� ��ȯ�Ѵ�.
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
// ���� �̵����� Window�� pointer�� ��ȯ�Ѵ�. �̰��� � Window�� ���� �̵��ϰ�
// �ִ��� �ܺο��� �˱����� ���̴�.
//
// !Window�� �� ������ �� �ϳ��� �̵��� �� ������ �װ��� '���̴�' ���̴�. ����
// �װ��� �ֿ켱 Window�̴�.
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
// p_this_window�� �ֿ켱������ �Ѵ�.
// p_this_window�� list�� �������� ������ �ƹ��͵� ���� �ʴ´�.
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
// m_show_list/m_show_list_pinned_window�� �ִ� Window�� Window::Show()�� �����Ѵ�.
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

	// pinned Window�� no pinned Window ���� ���� ����Ѵ�.
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
// p_this_window�� show list�� ����Ѵ�. �̹� �Ǿ� ������ ���� node�� delete�ϰ�
// �ٽ� insert�Ѵ�.
//
// p_this_window�� ��ϵǾ� ���� ������ �ƹ��͵� ���� �ʴ´�.
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
	// �̹� �����ϸ� �����Ѵ�.
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
// p_this_window�� show list���� delete�Ѵ�.
// p_this_window�� Disappear�Ǿ����� true�� ��ȯ�Ѵ�.
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
		// DisappearWindow�� �ƴ� ��쿡�� Ȯ���� SetKeyboardControlWindow�� keyboard control Window��
		// ������ �� ������, Disappear �Ǵ� ��쿡�� ���������� �˻��Ͽ� �װ��� ã�ƾ� �Ѵ�.
		//
		SetNextKeyboardControlWindow();
	}

	return disappear_result;
}

//-----------------------------------------------------------------------------
// SetMouseMoveFocusedWindow
//
// �ֿ켱���� Window���� �˻��Ͽ� mouse (x, y)�� Window�� ��ġ�ϸ� move focused
// Window�� �����Ѵ�.
//-----------------------------------------------------------------------------
void WindowManager::SetMouseMoveFocusedWindow()
{
	List::iterator itr, endItr;

	// topmost first
	// "topmost Window�� pinned Window�� �ƴϴ�."
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
// �ֿ켱���� Window���� �˻��Ͽ� mouse (x, y)�� Window�� ��ġ�ϸ� move focused
// Window�� �����Ѵ�.
//-----------------------------------------------------------------------------
Window* WindowManager::GetFocusedWindow(int x, int y)
{
	List::iterator itr;

	// topmost first
	// "topmost Window�� pinned Window�� �ƴϴ�."
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
// WindowManager::MouseControl�� App MouseControl�� Window::MouseControl��
// �߰����̴�.
//
// > Mouse pointer move�� mouse pointer�� � Window�� ����Ű�� focus �����¿�
//   ���� �Ѵ�. �̰��� �켱������ �������. ���� �켱������ ���� �ͺ���
//   �˻��Ѵ�.
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
		// 이동중이면 move focused Window을 바꾸지 않는다.
#ifdef _WIN32
		if (gpC_Imm && m_pC_mouse_focused_window != NULL && m_pC_mouse_focused_window->Moving())
			gpC_Imm->ForceUI(CImm::FORCE_UI_DRAG);
#endif

		if (m_pC_mouse_focused_window == NULL || m_pC_mouse_focused_window->Moving() == false)
		{
			//
			// Acquire/Unacquire mouse focus
			//
			Window * pC_prev_focused_window = m_pC_mouse_focused_window;

			SetMouseMoveFocusedWindow();

			if (pC_prev_focused_window != m_pC_mouse_focused_window)
			{
#ifdef _WIN32
				if(gpC_Imm)
					gpC_Imm->ForceUI(CImm::FORCE_UI_WINDOW);
#endif
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
	// topmost Window�� ���� ��쿡�� �켱������ ������ �� ����.
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

	// focused Window���� ��� �Է��� �ش�.
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

			//â���� statch
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
							
							//���� ������ ������� ���Ե������� ���θ� ���ܼ� ���δ�
							if(m_pC_mouse_focused_window->y < p_searched_window->y+p_searched_window->h &&
								m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y)
							{
								//���ʿ� ������ �ֳ�?
								if (m_pC_mouse_focused_window->x > p_searched_window->x+p_searched_window->w-STATCH_VALUE &&
									m_pC_mouse_focused_window->x < p_searched_window->x+p_searched_window->w+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->x = p_searched_window->x+p_searched_window->w;
									bl_statch = true;
								}
								else //�����ʿ� ������ �ֳ�?
									if (m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w > p_searched_window->x-STATCH_VALUE &&
										m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w < p_searched_window->x+STATCH_VALUE)
									{
										m_pC_mouse_focused_window->x = p_searched_window->x-m_pC_mouse_focused_window->w;
										bl_statch = true;
									}
									
									if(bl_statch)	//��� �پ��ٸ� �� ���� ���� ���ϼ� �ֳ� ����
									{
										//������ ��ǥ�� ��������
										if(m_pC_mouse_focused_window->y > p_searched_window->y-STATCH_VALUE &&
											m_pC_mouse_focused_window->y < p_searched_window->y+STATCH_VALUE)
										{
											m_pC_mouse_focused_window->y = p_searched_window->y;
										}
										else	//�Ʒ����� ��ǥ�� ��������
											if(m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y+p_searched_window->h-STATCH_VALUE &&
												m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h < p_searched_window->y+p_searched_window->h+STATCH_VALUE)
											{
												m_pC_mouse_focused_window->y = p_searched_window->y+p_searched_window->h-m_pC_mouse_focused_window->h;
											}
									}
									
							}
							
							bl_statch = false;
							//���� ������ ������� ���Ե������� ���θ� ���ܼ� ���δ�
							if (m_pC_mouse_focused_window->x < p_searched_window->x+p_searched_window->w &&
								m_pC_mouse_focused_window->x+m_pC_mouse_focused_window->w > p_searched_window->x)
							{
								//���ʿ� ������ �ֳ�?
								if (m_pC_mouse_focused_window->y > p_searched_window->y+p_searched_window->h-STATCH_VALUE &&
									m_pC_mouse_focused_window->y < p_searched_window->y+p_searched_window->h+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->y = p_searched_window->y+p_searched_window->h;
									bl_statch = true;
								}
								else //�Ʒ��ʿ� ������ �ֳ�?
									if (m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h > p_searched_window->y-STATCH_VALUE &&
										m_pC_mouse_focused_window->y+m_pC_mouse_focused_window->h < p_searched_window->y+STATCH_VALUE)
									{
										m_pC_mouse_focused_window->y = p_searched_window->y-m_pC_mouse_focused_window->h;
										bl_statch = true;
									}
							}
							if(bl_statch == true)	//��� �پ��ٸ� �� ���� ���� ���ϼ� �ֳ� ����
							{
								//������ ��ǥ�� ��������
								if(m_pC_mouse_focused_window->x > p_searched_window->x-STATCH_VALUE &&
									m_pC_mouse_focused_window->x < p_searched_window->x+STATCH_VALUE)
								{
									m_pC_mouse_focused_window->x = p_searched_window->x;
								}
								else	//�������� ��ǥ�� ��������
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
		// - ���� �ֿ켱Window���� push�� �� �ٸ� Window �Ǵ� background�� push up���� ���
		//   push�� Window�� push up�� ����� �Ѵ�.
		//
		// - pinned Window�� no pinned Window�� �����Ƿ� pushed Window�� first priority���
		//   ����� �� ����.

		// mouse move focus�� Window priority�� �ٲٴ� ����̸� �ٸ��� ó���ؾ� �Ѵ�.
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
// WindowManager::KeyboardControl�� App KeyboardControl�� Window::KeyboardControl�� 
// �߰����̴�.
//-----------------------------------------------------------------------------
void WindowManager::KeyboardControl(UINT message, UINT key, long extra)
{
//	static Window * m_pC_keydown_window;

	if (message == WM_KEYDOWN || message == WM_CHAR ||
	    message == WM_TEXTINPUT || message == WM_TEXTEDITING)// || message == WM_IME_STARTCOMPOSITION)
	{
		m_pC_keydown_window = NULL;
		if (m_pC_topmost_window != NULL)// && m_pC_topmost_window->GetAttributes()->keyboard_control == true)
			m_pC_keydown_window = m_pC_topmost_window;
		else if (m_pC_keyboard_control_window != NULL)
			m_pC_keydown_window = m_pC_keyboard_control_window;
	}

	//
	// �켱����, (1) topmost Window
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

		//���Ӹ�尡 �ƴҶ��� �Ʒ��� ������ �ʿ䰡 ����
		if(!gC_vs_ui.IsGameMode())
			return;
		}
	}

	// topmost window�� ���������� ä�ø԰� �ϱ� ���ؼ�
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
// p_window�� ��ϵǾ��°�?
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
// Window�� ����Ѵ�.
// �̹� ��ϵǾ��°� �˻��� �ʿ䰡 ����.
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
// Window ����� �����Ѵ�.
//-----------------------------------------------------------------------------
void WindowManager::Unregister(Window * p_window)
{
	if (p_window == NULL)
		_Error(NULL_REF);

	if (Delete(p_window) == true)
		DisappearWindow(p_window); // ������´� Ȯ���� �ʿ���� ������ disappear.

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
// Window�� Window Manager�� ����Ѵ�.
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
// Window Manager���� Window�� ��������Ѵ�.
//-----------------------------------------------------------------------------
void g_UnregisterWindow(Window * p_window)
{
	if (gpC_window_manager == NULL)
		_ErrorStr("WindowManager not initialized.");
	if (p_window == NULL)
		_Error(NULL_REF);

	gpC_window_manager->Unregister(p_window);
}	
