// VS_UI_Mouse_pointer.cpp

#include "client_PCH.h"
#include "VS_UI_mouse_pointer.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Item.h"
#include "VS_UI_description.h"
#include "VS_UI.h"
#include "UserOption.h"
#include "MZoneTable.h"

#include "MGameStringTable.H"

/*-----------------------------------------------------------------------------
  Gloabls
-----------------------------------------------------------------------------*/
extern void g_SetItemRefPoint(int item_rect_w, int item_rect_h);
extern bool gbl_item_lock;
extern RECT g_GameRect;
C_VS_UI_MOUSE_POINTER *	gpC_mouse_pointer;

//#define MOUSE_CURSOR_SLAYER_NORMAL			0
//#define MOUSE_CURSOR_SLAYER_PICKUP			1
//#define MOUSE_CURSOR_SLAYER_PICKUP_PUSHED		2
//#define MOUSE_CURSOR_SLAYER_ATTACK			3
//#define MOUSE_CURSOR_VAMPIRE_NORMAL			4
//#define MOUSE_CURSOR_VAMPIRE_PICKUP			5
//#define MOUSE_CURSOR_VAMPIRE_PICKUP_PUSHED	6
//#define MOUSE_CURSOR_VAMPIRE_ATTACK			7
//#define MOUSE_CURSOR_DESCRIPTION			8
//#define MOUSE_CURSOR_PORTAL_RIGHT			9
//#define MOUSE_CURSOR_PORTAL_LEFT			10
//#define MOUSE_CURSOR_PORTAL_DOWN			11
//#define MOUSE_CURSOR_PORTAL_UP				12
//#define MOUSE_CURSOR_NOT_MOVE				13
//#define MOUSE_CURSOR_BAT					14
static const POINT g_mouse_point_fix[C_VS_UI_MOUSE_POINTER::CURSOR_MAX] = 
{
	{0, 0},		// slayer default
	{-9, -36},	// slayer item
	{-2, -35},	// slayer item push
	{0, 0},		// slayer attack

	{0, 0},		// vampire default
	{-9, -36},	// vampire item
	{-2, -35},	// vampire item push
	{0, 0},		// vampire attack

	{0, 0},		// ousters default
	{-8, -38},	// ousters item
	{-6, -35},	// ousters item push
	{0, 0},		// ousters attack
	
	{-5, -5},	// description

	{-10, -10},	// not move

	{-14, -22},	// portal
};


//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetPointer Width/Height
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_MOUSE_POINTER::GetPointerW()
{
	if (m_bl_pickup_item)
	{
		assert(m_p_current_pickup_item);
		TYPE_FRAMEID frame_id = m_p_current_pickup_item->GetInventoryFrameID();

		return gpC_item->GetWidth(frame_id);
	}
	else
		return m_pC_mouse_pointer_spk->GetWidth();
}

int C_VS_UI_MOUSE_POINTER::GetPointerH()
{
	if (m_bl_pickup_item)
	{
		assert(m_p_current_pickup_item);
		TYPE_FRAMEID frame_id = m_p_current_pickup_item->GetInventoryFrameID();

		return gpC_item->GetHeight(frame_id);
	}
	else
		return m_pC_mouse_pointer_spk->GetHeight();
}

//-----------------------------------------------------------------------------
// Get Mouse Pointer (x, y)
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_MOUSE_POINTER::GetPointerX()
{
	return m_mouse_x-m_half_x;
}

int C_VS_UI_MOUSE_POINTER::GetPointerY()
{
	return m_mouse_y-m_half_y;
}

//-----------------------------------------------------------------------------
// PickUpItem
//
// item lock 여부를 check한다.
//-----------------------------------------------------------------------------
void C_VS_UI_MOUSE_POINTER::PickUpItem(MItem * p_item, int item_x, int item_y)
{
	assert(p_item);

	//if (!gbl_item_lock)
	{
		m_p_current_pickup_item = p_item;
		PickUpItem(item_x, item_y);
	}
}

//-----------------------------------------------------------------------------
// PickUpItem
//
// Item을 집었다.
// `Item을 집으면 Mouse pointer가 Item으로 변한다.
//
// item_x, item_y는 item이 출력되고 있는 위치이다. 이 값이 -1이면, 출력되는
// 상태를 무시한 것 즉, Mouse pointer를 item 중심에 위치시킨다. 실제로는 Mouse
// pointer는 가만히 있고 item이 움직여지는 것이다. 따라서 이렇게 하면 갑자기
// item이 이동하는 현상이 일어난다. 이것을 해결하기 위해서 item_x, item_y 값을
// 준다. 그러면 이 값을 참조하여 그러한 현상이 일어나지 않도록 한다.
//-----------------------------------------------------------------------------
void C_VS_UI_MOUSE_POINTER::PickUpItem(int item_x, int item_y)
{
	m_bl_pickup_item = true;	// get item width/height

	assert(m_p_current_pickup_item);
	TYPE_FRAMEID frame_id = m_p_current_pickup_item->GetInventoryFrameID();

	// !Item SPK는 중복되지 않으며, 역시 그것에 1:1 대응하는 Inventory FRR 또한 
	// 중복되지 않는다. 따라서 frame id는 곧 sprite id가 된다.

	if (item_x == INT_MIN || item_y == INT_MIN)
	{
		m_half_x = gpC_item->GetWidth(frame_id)/2;
		m_half_y = gpC_item->GetHeight(frame_id)/2;
	}
	else
	{
		//
		// 여기서 m_half는 (item_x, item_y)에서 Mouse pointer까지의 거리이다.
		//
		// Mouse pointer는 grid에 위치하고 item_x는 grid 내부에 위치하므로,
		// 음수가 나올 수 있다.
		//
		m_half_x = m_mouse_x - item_x;
		m_half_y = m_mouse_y - item_y;
	}

	g_SetItemRefPoint(gpC_item->GetWidth(frame_id), gpC_item->GetHeight(frame_id));
}

//-----------------------------------------------------------------------------
// DropItem
//
// Item을 놓았다.
//-----------------------------------------------------------------------------
void C_VS_UI_MOUSE_POINTER::DropItem()
{
	m_p_current_pickup_item = NULL;
	m_bl_pickup_item = false;
	m_half_x = 0;
	m_half_y = 0;
}

/*-----------------------------------------------------------------------------
- Show
- Mouse pointer image를 그린다.

  `Mouse pointer 좌표는 속도를 위해 내부에서 읽는다.
-----------------------------------------------------------------------------*/
void	C_VS_UI_MOUSE_POINTER::DrawTargetArrow(int TargetX, int TargetY)
{

	m_pC_mouse_pointer_spk->BltLocked(TargetX, TargetY, CURSOR_PORTAL);
	//m_pC_mouse_pointer_spk->BltLockedAlpha(TargetX, TargetY, CURSOR_PORTAL, 16);
}
void C_VS_UI_MOUSE_POINTER::Show()
{
	static int frame = 0;

//	if(m_cursor == CURSOR_SLAYER_PORTAL || m_cursor == CURSOR_VAMPIRE_PORTAL)
//	{
//		if(Timer())frame = (frame+1)%10;
//	}
//	else frame = 0;
		

	//
	// Item을 집었으면, C_VS_UI::m_current_pickup_item을 Mouse Pointer로써 출력한다.
	//
	if (m_bl_pickup_item)
	{
		TYPE_FRAMEID frame_id = m_p_current_pickup_item->GetInventoryFrameID();

		if(m_p_current_pickup_item->IsQuestItem())
			CIndexSprite::SetUsingColorSet(m_p_current_pickup_item->GetQuestItemColorset() ,0 );
		else
		if(m_p_current_pickup_item->IsUniqueItem())
			CIndexSprite::SetUsingColorSet(m_p_current_pickup_item->GetUniqueItemColorset(), 0);
		else
			CIndexSprite::SetUsingColorSet(m_p_current_pickup_item->GetItemOptionColorSet(), 0);

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			gpC_item->BltLocked(m_mouse_x-m_half_x,
											m_mouse_y-m_half_y,
											frame_id);

			if(m_p_current_pickup_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET
				&& g_eRaceInterface == RACE_OUSTERS )
			{
				ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[m_p_current_pickup_item->GetItemClass()][m_p_current_pickup_item->GetItemType()].ElementalType;
				if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
					eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
					eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
					)
				{
					// 2004, 3, 9 sobeit - x,y 변경
					//gpC_global_resource->m_pC_info_spk->BltLocked(m_mouse_x-(m_p_current_pickup_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, m_mouse_y-(m_p_current_pickup_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
					gpC_global_resource->m_pC_info_spk->BltLocked(m_mouse_x-m_half_x-6, m_mouse_y-m_half_y-14, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
				}
			}

			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}
	else
	{
//		if (m_bl_pushed && m_cursor == CURSOR)
//			m_pC_mouse_pointer_spk->Blt(m_mouse_x, m_mouse_y, 1);
		//else
//		int mouse_x = m_mouse_x + g_mouse_point_fix[m_cursor==CURSOR_SLAYER_PORTAL?CURSOR_VAMPIRE_PORTAL:m_cursor].x;
//		int mouse_y = m_mouse_y + g_mouse_point_fix[m_cursor==CURSOR_SLAYER_PORTAL?CURSOR_VAMPIRE_PORTAL:m_cursor].y;
		
		if(m_cursor == CURSOR_RESIZE && gpC_window_manager->GetMouseFocusedWindow() != m_resize_window)
		{
			//if(!IsCursorDescription())
				SetCursorDefault();
		}

		if(m_cursor == CURSOR_RESIZE && gpC_window_manager->GetMouseFocusedWindow() == m_resize_window)
		{
			int temp_cursor;
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				temp_cursor = CURSOR_SLAYER_PICKUP;
				break;

			case RACE_VAMPIRE:
				temp_cursor = CURSOR_VAMPIRE_PICKUP;
				break;

			case RACE_OUSTERS:
				temp_cursor = CURSOR_OUSTERS_PICKUP;
				break;
			}
			if(MousePushed())
				m_pC_mouse_pointer_spk->Blt(m_mouse_x + g_mouse_point_fix[temp_cursor+1].x, m_mouse_y + g_mouse_point_fix[temp_cursor+1].y, temp_cursor + frame +1);
			else
				m_pC_mouse_pointer_spk->Blt(m_mouse_x + g_mouse_point_fix[temp_cursor].x, m_mouse_y + g_mouse_point_fix[temp_cursor].y, temp_cursor + frame);
		}
		//UI위에 있거나 미니맵 위에 있으면 커서 default로
		else if(gpC_window_manager->GetMouseFocusedWindow())
		{
			extern Window* g_desc_dialog_window_id;
			if(m_bl_description && gpC_window_manager->GetMouseFocusedWindow() != g_desc_dialog_window_id)
				m_pC_mouse_pointer_spk->Blt(m_mouse_x + g_mouse_point_fix[CURSOR_DESCRIPTION].x, m_mouse_y + g_mouse_point_fix[CURSOR_DESCRIPTION].y, CURSOR_DESCRIPTION);
			else
			{
				int temp_cursor;
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					temp_cursor = CURSOR_SLAYER_NORMAL;
					break;
					
				case RACE_VAMPIRE:
					temp_cursor = CURSOR_VAMPIRE_NORMAL;
					break;
					
				case RACE_OUSTERS:
					temp_cursor = CURSOR_OUSTERS_NORMAL;
					break;
				}
				m_pC_mouse_pointer_spk->Blt(m_mouse_x, m_mouse_y, temp_cursor);
			}
		}
		else
		{
			if((m_cursor == CURSOR_SLAYER_PICKUP || m_cursor == CURSOR_VAMPIRE_PICKUP || m_cursor == CURSOR_OUSTERS_PICKUP) && MousePushed())
				m_pC_mouse_pointer_spk->Blt(m_mouse_x + g_mouse_point_fix[m_cursor+1].x, m_mouse_y + g_mouse_point_fix[m_cursor+1].y, m_cursor + frame +1);
			else
				m_pC_mouse_pointer_spk->Blt(m_mouse_x + g_mouse_point_fix[m_cursor].x, m_mouse_y + g_mouse_point_fix[m_cursor].y, m_cursor + frame);

			if((m_cursor == CURSOR_SLAYER_PICKUP || m_cursor == CURSOR_PORTAL || m_cursor == CURSOR_OUSTERS_PICKUP ||
				m_cursor == CURSOR_VAMPIRE_PICKUP) &&
				m_string.size() != 0)// || m_cursor == CURSOR_VAMPIRE_PORTAL)
			{
				RECT rect;
				if(m_cursor == CURSOR_PORTAL)// || m_cursor == CURSOR_VAMPIRE_PORTAL)
					SetRect(&rect, m_mouse_x - g_GetStringWidth(m_string.c_str(), gpC_base->m_info_pi.hfont)/2-5, m_mouse_y + g_mouse_point_fix[CURSOR_PORTAL].y - (g_GetStringHeight(m_string.c_str(), gpC_base->m_info_pi.hfont))*((m_portal_x==-1?1:2)+(m_owner_string.size()?1:0))-10, m_mouse_x + g_GetStringWidth(m_string.c_str(), gpC_base->m_info_pi.hfont)/2+5, m_mouse_y+g_mouse_point_fix[CURSOR_PORTAL].y);
				else
					SetRect(&rect, m_mouse_x-5, m_mouse_y-5-60, m_mouse_x+5 + g_GetStringWidth(m_string.c_str(), gpC_base->m_item_pi.hfont), m_mouse_y-60 + g_GetStringHeight(m_string.c_str(), gpC_base->m_item_name_pi.hfont));

				if(rect.top < 0){ rect.bottom += -rect.top; rect.top = 0; }
				else if(rect.bottom > g_GameRect.bottom){ rect.top -= rect.bottom - g_GameRect.bottom; rect.bottom = g_GameRect.bottom -1; }
				if(rect.left < 0){ rect.right += -rect.left; rect.left = 0; }
				else if(rect.right >= g_GameRect.right){ rect.left -= rect.right - g_GameRect.right; rect.right = g_GameRect.right -1; }

//				gpC_base->m_p_DDSurface_back->ChangeBrightnessBit(&rect, 1);
				DrawAlphaBox(&rect, 100, 100, 100, g_pUserOption->ALPHA_DEPTH);
				g_FL2_GetDC();
				if(m_cursor == CURSOR_PORTAL)// || m_cursor == CURSOR_VAMPIRE_PORTAL)
				{
					g_PrintColorStr(rect.left +5, rect.top +6+(g_GetStringHeight(m_string.c_str(), gpC_base->m_info_pi.hfont))*(m_owner_string.size()?1:0), m_string.c_str(), gpC_base->m_info_pi, RGB_WHITE);
					if(m_portal_x != -1)
					{
						char sz_temp[50];
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), m_portal_x, m_portal_y);
						g_PrintColorStr(rect.left +5, rect.top +6+(g_GetStringHeight(sz_temp, gpC_base->m_info_pi.hfont))*(m_owner_string.size()?2:1), sz_temp, gpC_base->m_info_pi, RGB(255,200,255));
					}

					if(m_owner_string.size())
					{
						g_PrintColorStr(rect.left +5, rect.top +6-1, m_owner_string.c_str(), gpC_base->m_info_pi, RGB_YELLOW);
					}
				}
				else
					// 아이템 이름
					g_PrintColorStr(rect.left +5, rect.top +4, m_string.c_str(), gpC_base->m_item_pi, m_color);
				g_FL2_ReleaseDC();
			}
//			else if(m_cursor == CURSOR_PORTAL_LEFT || m_cursor == CURSOR_PORTAL_RIGHT || m_cursor == CURSOR_PORTAL_UP || m_cursor == CURSOR_PORTAL_DOWN)
//			{
//				POINT portal_fix[4] = 
//				{
//					{159-91, 14-8},
//					{90,14-9},
//					{69-1,46-42},
//					{69-1,26},
//				};
//				
//				g_PrintColorStr(mouse_x + portal_fix[m_cursor - CURSOR_PORTAL_LEFT].x - g_GetStringWidth(m_string.c_str(), gpC_base->m_info_pi.hfont)/2, mouse_y + portal_fix[m_cursor - CURSOR_PORTAL_LEFT].y, m_string.c_str(), gpC_base->m_info_pi, RGB_WHITE);
//			}
		}

	}
}

/*-----------------------------------------------------------------------------
- C_VS_UI_MOUSE_POINTER
- Constructor.
-----------------------------------------------------------------------------*/
C_VS_UI_MOUSE_POINTER::C_VS_UI_MOUSE_POINTER()
{
	m_bl_pushed = false;
	m_bl_right_pushed = false;
	m_bl_pickup_item = false;
	m_half_x = 0;
	m_half_y = 0;
	m_mouse_x = 0;
	m_mouse_y = 0;
	m_p_current_pickup_item = NULL;
	m_cursor = 0;
	m_bl_description = false;
	m_resize_window = NULL;

	m_color = RGB_WHITE;

	// TIMER
	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 300;

	m_pC_mouse_pointer_spk = new C_SPRITE_PACK(SPK_MOUSE_POINTER);

	m_portal_x = -1; m_portal_y = -1;
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_MOUSE_POINTER
- Destructor.
-----------------------------------------------------------------------------*/
C_VS_UI_MOUSE_POINTER::~C_VS_UI_MOUSE_POINTER()
{
	DeleteNew(m_pC_mouse_pointer_spk);
}

//-----------------------------------------------------------------------------
// C_VS_UI_MOUSE_POINTER::Set
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MOUSE_POINTER::Set(UINT message, int x, int y) 
{ 
	m_mouse_x = x; 
	m_mouse_y = y; 

	switch (message)
	{
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
//			if(m_cursor == CURSOR_SLAYER_PICKUP || m_cursor == CURSOR_VAMPIRE_PICKUP)
//				m_cursor++;
			m_bl_pushed = true;
			break;

		case M_LEFTBUTTON_UP:
//			if(m_cursor == CURSOR_SLAYER_PICKUP_PUSHED || m_cursor == CURSOR_VAMPIRE_PICKUP_PUSHED)
//				m_cursor--;
			m_bl_pushed = false;
			break;

		case M_RIGHTBUTTON_DOWN:
			if(IsCursorDescription())CursorDescriptionToggle();
			m_bl_right_pushed = true;
			break;

		case M_RIGHTBUTTON_UP:
			m_bl_right_pushed = false;
			break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BRIEFING::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_MOUSE_POINTER::Timer()
{
	if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_dw_prev_tickcount = GetTickCount();
		
		return true;
	}

	return false;
}

void	C_VS_UI_MOUSE_POINTER::SetCursorPortal(int zone_id, int x, int y, char *owner_name)
{
	SetCursor(CURSOR_PORTAL);
	if(zone_id == 1500)
	{
		switch(x)
		{
		case 200:
			m_string = (*g_pGameStringTable)[STRING_MESSAGE_SIEGE_POTAL_200].GetString();
			break;
		case 201:
			m_string = (*g_pGameStringTable)[STRING_MESSAGE_SIEGE_POTAL_201].GetString();
			break;
		case 202:
			m_string = (*g_pGameStringTable)[STRING_MESSAGE_SIEGE_POTAL_202].GetString();
			break;
		case 203:
			m_string = (*g_pGameStringTable)[STRING_MESSAGE_SIEGE_POTAL_203].GetString();
			break;
		}
		m_portal_x = -1; m_portal_y = -1;
	}
	else
	{
		m_string = g_pZoneTable->Get(zone_id)->Name;
		m_portal_x = x; m_portal_y = y;
	}
	if(owner_name)
		m_owner_string = owner_name;
	else m_owner_string = "";
}

void	C_VS_UI_MOUSE_POINTER::SetCursorResize(Window *resize_window)
{
	SetCursor(CURSOR_RESIZE);
//	switch(g_eRaceInterface)
//	{
//	case RACE_SLAYER:
//		SetCursor(CURSOR_SLAYER_PICKUP);
//		break;
//
//	case RACE_VAMPIRE:
//		SetCursor(CURSOR_VAMPIRE_PICKUP);
//		break;
//
//	case RACE_OUSTERS:
//		SetCursor(CURSOR_OUSTERS_PICKUP);
//		break;
//	}
	m_resize_window = resize_window;
}

void	C_VS_UI_MOUSE_POINTER::SetCursorDefault()
{
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		SetCursor(CURSOR_SLAYER_NORMAL);
		break;
		
	case RACE_VAMPIRE:
		SetCursor(CURSOR_VAMPIRE_NORMAL);
		break;
		
	case RACE_OUSTERS:
		SetCursor(CURSOR_OUSTERS_NORMAL);
		break;
	}
}

void	C_VS_UI_MOUSE_POINTER::CursorDescriptionToggle()
{
	m_bl_description = !m_bl_description; 
}

void	C_VS_UI_MOUSE_POINTER::SetCursorPickUp(LPSTR str, COLORREF color)
{
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		SetCursor(CURSOR_SLAYER_PICKUP);
		break;
		
	case RACE_VAMPIRE:
		SetCursor(CURSOR_VAMPIRE_PICKUP);
		break;
		
	case RACE_OUSTERS:
		SetCursor(CURSOR_OUSTERS_PICKUP);
		break;
	}

	m_string = str;
	m_color = color; 
}

void	C_VS_UI_MOUSE_POINTER::SetCursorAttack()
{
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		SetCursor(CURSOR_SLAYER_ATTACK);
		break;
		
	case RACE_VAMPIRE:
		SetCursor(CURSOR_VAMPIRE_ATTACK);
		break;
		
	case RACE_OUSTERS:
		SetCursor(CURSOR_OUSTERS_ATTACK);
		break;
	}
}
