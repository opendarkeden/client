// u_button.cpp

#include "client_PCH.h"

#include "u_button.h"

#ifndef _LIB
EventButton *g_EventButton = NULL;
#endif

//-----------------------------------------------------------------------------
// EventButton
//
// 
//-----------------------------------------------------------------------------
EventButton::EventButton(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, Button::ClickOption co) :
										Button(_x, _y, _w, _h, id, pC_exec_handler, co)
{
	m_bl_prev_focus = false;
}

EventButton::~EventButton()
{
}

//-----------------------------------------------------------------------------
// InitState
//
// 
//-----------------------------------------------------------------------------
void EventButton::InitState()
{ 
	ButtonBase::InitState();

	m_bl_prev_focus = false; 
}

//-----------------------------------------------------------------------------
// EventButton::MouseControl
//
// 입력이벤트를 정의된 method를 실행하므로써 알린다.
//-----------------------------------------------------------------------------
bool EventButton::MouseControl(UINT message, int _x, int _y)
{
	switch (message)
	{
		//
		// M_MOVING
		//
		// focus of/off
		//
		case M_MOVING:
			if (IsInRect(_x, _y))
			{
				if (!m_bl_prev_focus)
				{
					m_bl_prev_focus = true;
					EventFocusOn();
				}
			}
			else if (m_bl_prev_focus)
			{
				m_bl_prev_focus = false;
				EventFocusOff();
			}
			break;

		//
		// M_LEFTBUTTON_DOWN, M_LB_DOUBLECLICK
		//
		// button press down.
		//
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (IsInRect(_x, _y)) 
			{
				m_bl_prev_focus = true; // move focus 없이 될 경우를 생각해서.
				EventPressDown();
			}
			break;

		//
		// M_LEFTBUTTON_UP
		//
		// button press up.
		//
		case M_LEFTBUTTON_UP:
			if (GetFocusState()) // focus: yes
			{
				if (GetPressState()) // press: yes
				{
					EventPressUp();
				}
			}
			break;

#ifndef _LIB
		case M_RIGHTBUTTON_UP:
			if (GetFocusState()) // focus: yes
				g_EventButton = this;
			break;
#endif
	}

	return Button::MouseControl(message, _x, _y); // left up시 GetPressState()를 확인할 수 있음.
}

ButtonBase::ButtonBase()
{
	InitState();
}

ButtonBase::~ButtonBase()
{
#ifndef _LIB
	if(g_EventButton == this)g_EventButton = NULL;
#endif

}

void ButtonBase::InitState()
{
	m_bl_focused = false;
	m_bl_pressed = false;
}

void ButtonBase::FocusOn()
{
	m_bl_focused = true;
}

void ButtonBase::FocusOff()
{
	m_bl_focused = false;
}

void ButtonBase::PressDown()
{
	m_bl_pressed = true;
}

void ButtonBase::PressUp()
{
	m_bl_pressed = false;
}

bool ButtonBase::GetFocusState() const
{
	return m_bl_focused;
}

bool ButtonBase::GetPressState() const
{
	return m_bl_pressed;
}

//-----------------------------------------------------------------------------
// Button (Constructor)
//
// 
//-----------------------------------------------------------------------------
Button::Button(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, ClickOption co)
{
	Set(_x, _y, _w, _h);
	m_id = id;
	m_pC_exec_handler = pC_exec_handler;
	m_click_option = co;
}

Button::Button()
{

}

//-----------------------------------------------------------------------------
// ~Button (Destructor)
//
// 
//-----------------------------------------------------------------------------
Button::~Button()
{

}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void Button::Run()
{
	if (m_pC_exec_handler) // set handler?
		m_pC_exec_handler->Run(m_id);
}

void Button::SetExecHandler(Exec * pC_exec_handler)
{
	m_pC_exec_handler = pC_exec_handler;
}

void Button::SetID(id_t id)
{
	m_id = id;
}

//-----------------------------------------------------------------------------
// MouseControl
//
// Button input interface.
// 
// focus, press 상태는 내부적으로 설정한다. 이것은 ButtonBase에서 참조할 수
// 있다. button down이면 false를 반환하고 그렇지 않으면 true를 반환한다.
//
// Example:
//				if (button->MouseControl()) // false이면 이후의 입력은 처리하지 않는다.
//					if (button->MouseControl())
//						...
//
//
// message는 i_signal.h에 정의되어 있는 것이다.
//
// History:
//				[2000.8.5. 3:15pm]
//
//				rect 범위에 있는 입력이면 true를 반환하고, 그렇지않으면 false를 반환
//				하도록 하였는데, focus 문제가 발생하였다. 그것은 다음과 같은 형태를
//				취해서 불필요한 처리를 없애기 위한 것이었다.
//
//				if (m_p_button_ok->MouseControl(message, x, y) == false)
//					if (m_p_button_cancel->MouseControl(message, x, y) == false)
//						...
//
//				그런데 이렇게 했을 때, cancel이 focus된 후 빠르게 ok가 focus되면,
//				cancel의 focus가 취소되지 않는다. 물론 Button::MouseControl()에서
//				그 문제에 대비할 수도 있겠지만, 그러한 모든 코드가 결국 불필요하다는
//				것을 짐작하였다.
//
//				위험한 것은, 만약 button이 다른 것과 겹칠 경우 모든 button이 실행
//				된다는 것이다. 따라서 겹치지 않도록 하여야한다. 겹치는 것은 잘못된
//				것이므로 Button은 이것에 대해 몰라도 된다.
//
//
//				[2000.8.5. 3:35pm]
//
//				Window 안에 button이 있고 그 Window는 mouse로 끌어서 이동할 수 있는
//				것이라면, 반드시 button의 입력상태를 알 수 있어야한다. 그렇지않으면
//				button을 누른상태에서 Window를 이동할 수 있게 된다.
//				이것을 해결하기 위해 button down일 경우에는 false를 반환하도록 한다.
//				false를 반환하는 이유는 MouseControl()을 처리하는 부분을 간소화하기
//				위해서다.
//-----------------------------------------------------------------------------
bool Button::MouseControl(UINT message, int _x, int _y)
{
	switch (message)
	{
		//
		// M_MOVING
		//
		// focus of/off
		//
		case M_MOVING:
			if (IsInRect(_x, _y))
			{
				FocusOn();
			}
			else
			{
				FocusOff();
			}
			break;

		//
		// M_LEFTBUTTON_DOWN, M_LB_DOUBLECLICK
		//
		// button press down.
		//
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (IsInRect(_x, _y))
			{
				PressDown();

				if (m_click_option == RUN_WHEN_PUSH)
					Run();

				return false;
			}
			break;

		//
		// M_LEFTBUTTON_UP
		//
		// button press up -> execution!
		//
		case M_LEFTBUTTON_UP:
			if (GetFocusState()) // focus: yes
			{
				if (GetPressState()) // press: yes
				{
					PressUp();

					if (m_click_option == RUN_WHEN_PUSHUP)
						Run();
				}
				else // press: no
				{
					// no code!
				}
			}
			else // focus: no
			{
				if (GetPressState()) // press: yes
				{
					PressUp();
				}
				else // press: no
				{
					// no code!
				}
			}
	}

	return true;
}