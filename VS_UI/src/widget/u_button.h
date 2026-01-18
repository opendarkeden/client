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

//

//


//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Class ButtonBase
//

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



//
// [2000.9.25.]
//





//----------------------------------------------------------------------------
class Button : public ButtonBase
{
public:
	enum ClickOption
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




	void	SetID(id_t id);
	void	SetExecHandler(Exec * pC_exec_handler);
	void	SetClickOption(ClickOption co) { m_click_option = co; }

	//


	//
	virtual bool	MouseControl(UINT message, int _x, int _y);
	//virtual void	Show() = 0;

	id_t	GetID() const { return m_id; }
};

//-----------------------------------------------------------------------------
// EventButton (abstract class)
//



//-----------------------------------------------------------------------------
class EventButton : public Button
{
protected:
	bool			m_bl_prev_focus;

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