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
	bool						m_bl_coin;

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
		CURSOR_OUSTERS_NORMAL,
		CURSOR_OUSTERS_PICKUP,
		CURSOR_OUSTERS_PICKUP_PUSHED,
		CURSOR_OUSTERS_ATTACK,
		CURSOR_DESCRIPTION,
		CURSOR_NOT_MOVE,
		CURSOR_PORTAL,
		CURSOR_ADDFRIEND,
		CURSOR_RESIZE,		
		CURSOR_COIN,
		CURSOR_MAX,
	};

	void	SetCursor(int cursor)						{ m_cursor = cursor; }
	void	SetCursorDefault();
	void	SetCursorPickUp(LPSTR str, COLORREF color);
	void	SetCursorAttack();
	void	SetCursorAddFriend();
	void	CursorDescriptionToggle();
	void	CursorCoinToggle();
	void	SetCursorPortal(int zone_id, int x = -1, int y = -1, char *owner_name = NULL);
//	void	SetCursorPortalLeft(LPSTR str)				{ SetCursor(CURSOR_PORTAL_LEFT); m_string = str; }
//	void	SetCursorPortalRight(LPSTR str)				{ SetCursor(CURSOR_PORTAL_RIGHT); m_string = str; }
//	void	SetCursorPortalUp(LPSTR str)				{ SetCursor(m_cursor = CURSOR_PORTAL_UP); m_string = str; }
//	void	SetCursorPortalDown(LPSTR str)				{ SetCursor(CURSOR_PORTAL_DOWN); m_string = str; }
	void	SetCursorNotMove()							{ SetCursor(CURSOR_NOT_MOVE); }
	void	SetCursorResize(Window *resize_window);

	bool	IsCursorDescription() const					{ return m_bl_description; }
	bool	IsCursorCoin() const						{ return m_bl_coin; }

	void	Set(UINT message, int x, int y);
	void	Show();
	void	PickUpItem(MItem * p_item, int item_x=INT_MIN, int item_y=INT_MIN);
	void	DropItem();
	int		GetPointerW();
	int		GetPointerH();
	int		GetPointerX();
	int		GetPointerY();
	int		GetX() const { return m_mouse_x; }
	int		GetY() const { return m_mouse_y; }
	MItem * GetPickUpItem() { return m_p_current_pickup_item; }
	bool	IsPickUpItem()	{ return m_bl_pickup_item; }
	bool	MousePushed() const { return m_bl_pushed; }
	bool	RightMousePushed() const { return m_bl_right_pushed; }
	
	void	DrawTargetArrow(int TargetX, int TargetY);
};

extern C_VS_UI_MOUSE_POINTER *	gpC_mouse_pointer;

#endif