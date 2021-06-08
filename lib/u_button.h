/*-----------------------------------------------------------------------------

	u_button.h

	Button Widget.

	2000.6.7. KJTINC
	2000.8.4.

-----------------------------------------------------------------------------*/

#ifndef __U_BUTTON_H__
#define __U_BUTTON_H__

#include "Basics.h"

//----------------------------------------------------------------------------
// Widget Button
//
// Design:
//				Button은 상태를 갖는 rect이다.
//
//				Button은 다음과 같은 event를 발생한다.
//
//				(1) button영역에 mouse move가 들어올 때/나갈 때.
//				(2) button영역을 누룰 때/뗄 때.
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Class ButtonBase
//
// 버튼 그 자체 곧 Single Button Object.
//----------------------------------------------------------------------------
class ButtonBase : public Rect
{
private:
	//---------------------------------------
	// Private data
	//---------------------------------------
	bool		m_bl_focused; // button focus state
	bool		m_bl_pressed; // button press state

public:
	ButtonBase();
	virtual ~ButtonBase();

	//---------------------------------------
	// Set
	//---------------------------------------
	void	FocusOn();
	void	FocusOff();
	void	PressDown();
	void	PressUp();
	virtual void	InitState();

	//---------------------------------------
	// Get
	//---------------------------------------
	bool	GetFocusState() const;
	bool	GetPressState() const;
};

//----------------------------------------------------------------------------
// Exec (abstract class)
//
// Button execution interface.
//
// Button을 사용하는 class는 이것을 상속받아야 한다. 그러면 Button을 정의하면서
// this 포인터를 주어 Button에서도 같은 interface를 통해 exec를 할 수 있다.
//----------------------------------------------------------------------------
class Exec
{
public:
	virtual void Run(id_t id) = 0;
};

//----------------------------------------------------------------------------
// Class Button (abstract class)
//
// Button condition change & execution.
//
// 실제적인 Button Widget.
// 이것은 일반적인 Button의 내용을 정의한다. 이것은 출력에 대해서는 정의하지
// 않지만, 순수가상으로써 Show()를 갖는다.
//
// [2000.9.25.]
//
// VS UI Project에서 Button을 Button group으로 넣어 한번에 처리할 수 있도록 하고
// 또한 출력도 마찬가지로 어느때나 그 자신의 출력부분을 갖을 수 있도록 하기 위해서
// pure virtual Show가 불필요하였다. 그래서 그것이 꼭 필요하면 출력메소드를
// 별도로 추가하는 방식으로 사용하기로 하고, Button이 instance화 될 수 있도록 하기 
// 위해서 pure virtual은 넣지 않는다.
//----------------------------------------------------------------------------
class Button : public ButtonBase
{
public:
	enum ClickOption // Run()을 언제 실행하는가?
	{
		RUN_WHEN_PUSHUP,
		RUN_WHEN_PUSH,
	};

private:
	Exec *						m_pC_exec_handler;	// exec handler
	id_t							m_id;						// own exec id
	ClickOption					m_click_option;

	void	Run();

public:
	//
	// 두 가지 방법으로 초기화할 수 있다.
	//
	// (1) Button * p = new Button(x, y, w, h, id, this);
	// (2) Button p;
	//		 p.Set(x, y, w, h);
	//		 p.SetID(id);
	//		 p.SetExecHandler(this);
	//		 p.SetClickOption(RUN_WHEN_PUSH);
	//
	Button(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, ClickOption co=Button::RUN_WHEN_PUSHUP);
	Button();
	virtual ~Button();

	// rect에 XY(), Set()이 있다.
	// 또한 ButtonBase의 InitState()는 상태를 초기화한다.

	void	SetID(id_t id);
	void	SetExecHandler(Exec * pC_exec_handler);
	void	SetClickOption(ClickOption co) { m_click_option = co; }

	//
	// Button을 상속할 경우에 현재의 MouseControl()에 Event call 속성과 같은
	// 새로운 기능을 정의할 수 있다. 따라서 virtual로 한다.
	//
	virtual bool	MouseControl(UINT message, int _x, int _y);
	//virtual void	Show() = 0;

	id_t	GetID() const { return m_id; }
};

//-----------------------------------------------------------------------------
// EventButton (abstract class)
//
// NormalButton과 기본은 같지만, Mouse입력마다 명시적인 event를 발생시켜 쉽게
// 그것을 알 수 있도록 한다. 이것의 진정한 목적은 mouse pointer를 갖다대면
// animation되는 animation button을 위한 것이다.
//-----------------------------------------------------------------------------
class EventButton : public Button
{
protected:
	bool			m_bl_prev_focus; // static으로 하지 않는다.

public:
	EventButton(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, Button::ClickOption co=Button::RUN_WHEN_PUSHUP);
	virtual ~EventButton();

	virtual void	EventFocusOn() {}
	virtual void	EventFocusOff() {}
	virtual void	EventPressDown() {}
	virtual void	EventPressUp() {}

	bool	MouseControl(UINT message, int _x, int _y);
	void	InitState();
};

#endif