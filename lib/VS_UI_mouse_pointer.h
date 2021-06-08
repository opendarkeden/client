/*-----------------------------------------------------------------------------

	VS_UI_mouse_pointer.h

	Mouse pointer 관련.

	2000.3.30. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_MOUSE_POINTER_H__
#define __VS_UI_MOUSE_POINTER_H__

#include "VS_UI_Util.h"
#include "VS_UI_base.h"
#include "MItem.h"
#include <limits.h>


/*-----------------------------------------------------------------------------
  Class Mouse Pointer
  `Pointer animation, Item pick up...
-----------------------------------------------------------------------------*/
class C_VS_UI_MOUSE_POINTER
{
private:
	C_SPRITE_PACK *			m_pC_mouse_pointer_spk;
	bool							m_bl_pickup_item;
	int							m_half_x; // of Mouse pointer object.
	int							m_half_y;
	int							m_mouse_x;
	int							m_mouse_y;
	bool						m_bl_pushed;
	bool						m_bl_right_pushed;
	int							m_cursor;
	bool						m_bl_description;

	//timer
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	bool						Timer();

	//
	// 현재 집은(Mouse pointer에 붙은) Item.
	//
	MItem *						m_p_current_pickup_item;

	void	PickUpItem(int item_x, int item_y);
	std::string	m_string, m_owner_string;
	int		m_portal_x, m_portal_y;

	COLORREF m_color;

	Window *m_resize_window;

public:
	C_VS_UI_MOUSE_POINTER();
	~C_VS_UI_MOUSE_POINTER();

	enum	CURSOR_SPK_INDEX
	{
		CURSOR_SLAYER_NORMAL,
		CURSOR_SLAYER_PICKUP,
		CURSOR_SLAYER_PICKUP_PUSHED,
		CURSOR_SLAYER_ATTACK,
		CURSOR_VAMPIRE_NORMAL,
		CURSOR_VAMPIRE_PICKUP,
		CURSOR_VAMPIRE_PICKUP_PUSHED,
		CURSOR_VAMPIRE_ATTACK,
		CURSOR_DESCRIPTION,
		CURSOR_NOT_MOVE,
		CURSOR_PORTAL,
		CURSOR_RESIZE,
		CURSOR_MAX,
	};

	void	SetCursor(int cursor)						{ m_cursor = cursor; }
	void	SetCursorDefault()							{ SetCursor(gbl_vampire_interface?CURSOR_VAMPIRE_NORMAL:CURSOR_SLAYER_NORMAL); }
	void	SetCursorPickUp(LPSTR str, COLORREF color)	{ SetCursor(gbl_vampire_interface?CURSOR_VAMPIRE_PICKUP:CURSOR_SLAYER_PICKUP); m_string = str; m_color = color; }
	void	SetCursorAttack()							{ SetCursor(gbl_vampire_interface?CURSOR_VAMPIRE_ATTACK:CURSOR_SLAYER_ATTACK); }
	void	CursorDescriptionToggle()					{ m_bl_description = !m_bl_description; }
	void	SetCursorPortal(int zone_id, int x = -1, int y = -1, char *owner_name = NULL);
//	void	SetCursorPortalLeft(LPSTR str)				{ SetCursor(CURSOR_PORTAL_LEFT); m_string = str; }
//	void	SetCursorPortalRight(LPSTR str)				{ SetCursor(CURSOR_PORTAL_RIGHT); m_string = str; }
//	void	SetCursorPortalUp(LPSTR str)				{ SetCursor(m_cursor = CURSOR_PORTAL_UP); m_string = str; }
//	void	SetCursorPortalDown(LPSTR str)				{ SetCursor(CURSOR_PORTAL_DOWN); m_string = str; }
	void	SetCursorNotMove()							{ SetCursor(CURSOR_NOT_MOVE); }
	void	SetCursorResize(Window *resize_window)		{ SetCursor(CURSOR_RESIZE); m_resize_window = resize_window; }

	bool	IsCursorDescription()						{ return m_bl_description; }

	void	Set(UINT message, int x, int y);
	void	Show();
	void	PickUpItem(MItem * p_item, int item_x=INT_MIN, int item_y=INT_MIN);
	void	DropItem();
	int	GetPointerW();
	int	GetPointerH();
	int	GetPointerX();
	int	GetPointerY();
	int	GetX() const { return m_mouse_x; }
	int	GetY() const { return m_mouse_y; }
	MItem * GetPickUpItem() { return m_p_current_pickup_item; }
	bool	IsPickUpItem()	{ return m_bl_pickup_item; }
	bool	MousePushed() const { return m_bl_pushed; }
	bool	RightMousePushed() const { return m_bl_right_pushed; }
};

extern C_VS_UI_MOUSE_POINTER *	gpC_mouse_pointer;

#endif