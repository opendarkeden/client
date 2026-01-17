/*-----------------------------------------------------------------------------

	CDirectInput_Adapter.cpp

	DirectInput adapter using DXLibBackend.
	This file provides SDL2 backend support for CDirectInput class.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "CDirectInput.h"
#include "DXLibBackend.h"

#define MSB		0x80

/* Global instance */
CDirectInput*	g_pDXInput = NULL;

/* Keep the original key name table - it's defined in the header */

/*=============================================================================
 * SDL Backend Implementation
 *=============================================================================*/

#ifdef DXLIB_BACKEND_SDL

/* Constructor */
CDirectInput::CDirectInput()
{
	m_pDI				= NULL;
	m_pMouse			= NULL;
	m_pKeyboard		= NULL;
	m_mouse_x		= 0;
	m_mouse_y		= 0;
	m_mouse_z		= 0;	
	m_limit_x		= 0;
	m_limit_y		= 0;
	m_mouse_info[0]=0;
	m_mouse_info[1]=0;
	m_mouse_info[2]=0;

	m_fp_mouse_event_receiver = NULL;
	m_fp_keyboard_event_receiver = NULL;
	
	Clear();
}

/* Destructor */
CDirectInput::~CDirectInput()
{
	FreeDirectInput();
}

/* Clear input state */
void CDirectInput::Clear()
{
	for (int i=0; i<256; i++)
	{
		m_key[i] = FALSE;
	}
	
	m_lb_down = FALSE;
	m_rb_down = FALSE;
	m_cb_down = FALSE;
	m_lb_up = FALSE;
	m_rb_up = FALSE;
	m_cb_up = FALSE;
}

/* Initialize using SDL backend */
BOOL CDirectInput::Init(HWND hWnd, HINSTANCE hInst, E_EXCLUSIVE ex)
{
	// Initialize SDL backend
	if (dxlib_input_init(hWnd) != 0) {
		return FALSE;
	}

	// Mark as initialized (use non-null values as indicators)
	m_pDI = (IDirectInput*)0x01;
	m_pMouse = (IDirectInputDevice*)0x01;
	m_pKeyboard = (IDirectInputDevice*)0x01;

	return TRUE;
}

/* Release SDL backend */
void CDirectInput::FreeDirectInput()
{
	dxlib_input_release();

	m_pDI = NULL;
	m_pMouse = NULL;
	m_pKeyboard = NULL;
}

/* Update input using SDL backend */
void CDirectInput::UpdateInput()
{
	// Update backend
	dxlib_input_update();

	// Reset button states
	m_lb_down = FALSE;
	m_rb_down = FALSE;
	m_cb_down = FALSE;
	m_lb_up = FALSE;
	m_rb_up = FALSE;
	m_cb_up = FALSE;

	// Update keyboard state
	for (int i = 0; i < 256; i++) {
		BOOL down = dxlib_input_key_down(i) ? TRUE : FALSE;
		
		// Check for state changes and trigger events
		if (down && !m_key[i]) {
			m_key[i] = TRUE;
			if (m_fp_keyboard_event_receiver) {
				m_fp_keyboard_event_receiver(KEYDOWN, i);
			}
		} else if (!down && m_key[i]) {
			m_key[i] = FALSE;
			if (m_fp_keyboard_event_receiver) {
				m_fp_keyboard_event_receiver(KEYUP, i);
			}
		}
	}

	// Update mouse position
	int old_x = m_mouse_x;
	int old_y = m_mouse_y;
	dxlib_input_get_mouse_pos(&m_mouse_x, &m_mouse_y);

	// Check for mouse movement
	if (old_x != m_mouse_x || old_y != m_mouse_y) {
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(MOVE, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}

	// Update mouse wheel
	int old_z = m_mouse_z;
	m_mouse_z = dxlib_input_get_mouse_wheel();
	
	// Check for wheel movement
	if (old_z != m_mouse_z) {
		if (m_fp_mouse_event_receiver) {
			if (m_mouse_z > old_z) {
				m_fp_mouse_event_receiver(WHEELUP, m_mouse_x, m_mouse_y, m_mouse_z);
			} else {
				m_fp_mouse_event_receiver(WHEELDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
			}
		}
	}

	// Update mouse buttons
	int left, right, center;
	dxlib_input_get_mouse_buttons(&left, &right, &center);

	// Check for button state changes
	if (left && !m_lb_down) {
		m_lb_down = TRUE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(LEFTDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}
	if (!left && m_lb_down) {
		m_lb_up = TRUE;
		m_lb_down = FALSE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(LEFTUP, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}

	if (right && !m_rb_down) {
		m_rb_down = TRUE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(RIGHTDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}
	if (!right && m_rb_down) {
		m_rb_up = TRUE;
		m_rb_down = FALSE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(RIGHTUP, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}

	if (center && !m_cb_down) {
		m_cb_down = TRUE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(CENTERDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}
	if (!center && m_cb_down) {
		m_cb_up = TRUE;
		m_cb_down = FALSE;
		if (m_fp_mouse_event_receiver) {
			m_fp_mouse_event_receiver(CENTERUP, m_mouse_x, m_mouse_y, m_mouse_z);
		}
	}
}

/* Set acquire (SDL backend - no-op) */
HRESULT CDirectInput::SetAcquire(bool active_app)
{
	if (!m_pMouse || !m_pKeyboard)
		return S_FALSE;
	return S_OK;
}

/* Set mouse position (SDL backend) */
void CDirectInput::SetMousePosition(int x, int y)
{
	m_mouse_x = x;
	m_mouse_y = y;
	dxlib_input_set_mouse_pos(x, y);
}

/* Set mouse speed (SDL backend - stub) */
void CDirectInput::SetMouseSpeed()
{
	// SDL backend uses system mouse settings
}

/* Get mouse acceleration (not applicable for SDL) */
int CDirectInput::GetMouseAcceleration(int value)
{
	return value;
}

/* Set mouse move limit */
void CDirectInput::SetMouseMoveLimit(int x, int y)
{
	m_mouse_x = 0;
	m_mouse_y = 0;
	m_mouse_z = 0;

	m_limit_x = x;
	m_limit_y = y;
}

/* Set event receivers */
void CDirectInput::SetMouseEventReceiver(void (*fp_receiver)(E_MOUSE_EVENT, int, int, int))
{
	m_fp_mouse_event_receiver = fp_receiver;
}

void CDirectInput::SetKeyboardEventReceiver(void (*fp_receiver)(E_KEYBOARD_EVENT, DWORD))
{
	m_fp_keyboard_event_receiver = fp_receiver;
}

/* Stub implementations for unused methods */
void CDirectInput::OnMouseInput() { /* Handled in UpdateInput */ }
void CDirectInput::OnKeyboardInput() { /* Handled in UpdateInput */ }
HRESULT CDirectInput::InitDI(HWND hWnd, HINSTANCE hInst, E_EXCLUSIVE ex) { 
	return Init(hWnd, hInst, ex) ? S_OK : S_FALSE; 
}

#endif /* DXLIB_BACKEND_SDL */
