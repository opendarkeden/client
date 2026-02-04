// VS_UI_GameSlayer.cpp

#include "client_PCH.h"
#include "VS_UI_GameSlayer.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Item.h"
#include "VS_UI_Mouse_pointer.h"
#include "MCreatureTable.h"
#include <algorithm>
#include "UserOption.h"
#include "VS_UI.h"
#include "ExperienceTable.h"
#include "MGameStringTable.h"
#include "UserInformation.h"
#include "MZoneTable.h"
#include "SystemAvailabilities.h"
//#include "ex\DebugInfo.h"
extern RECT g_GameRect;
#define MP_CASE_X				708
#define MP_CASE_Y				508
#define HP_PIE_X				634
#define HP_PIE_Y				511
#define MP_PIE_X				724
#define MP_PIE_Y				511
#define PIE_W					50
#define PIE_H					50
#define HP_PIE_START_X		(HP_PIE_X+15) // 12
#define HP_PIE_START_Y		HP_PIE_Y
#define HP_PIE_END_X			(HP_PIE_X+16) // 14
#define HP_PIE_END_Y			HP_PIE_Y
#define MP_PIE_START_X		(MP_PIE_X+15)
#define MP_PIE_START_Y		MP_PIE_Y
#define MP_PIE_END_X			(MP_PIE_X+16)
#define MP_PIE_END_Y			MP_PIE_Y

extern bool gbl_item_lock;
extern HWND g_hWnd;


int C_VS_UI_SLAYER_GEAR::m_slot_image[SLOT_SIZE] = {
	HELM, NECKLACE, COAT, SHIELD, SWORD, GLOVE, BELT, TROUSER, BRACELET, BRACELET, RING, RING, RING, RING, SHOES
	,-1
	,-1
	,-1
	,-1,
	PDA,SHOULDER,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE
};

//-----------------------------------------------------------------------------
// Click
//
// ���������� true��, �׷��������� false�� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------
bool C_VS_UI_SLAYER_QUICKITEM::Click()
{
	if (m_focus_slot == NOT_SELECTED || gbl_item_lock || 
		 g_pQuickSlot == NULL)
	{
		return false;
	}

	int item_x, item_y;

	if (gpC_mouse_pointer->GetPickUpItem()) // ��� �ִ°�?
	{
		const MItem* p_cur_item = g_pQuickSlot->GetItem(m_focus_slot);

		// �ѿ� źâ�� ����� �Ͱ� ���� ���� insert item�̴�.
		if (gpC_mouse_pointer->GetPickUpItem()->IsInsertToItem( p_cur_item ))
		{
			// ������ �ִ� item�� �߰��� �� �ִ� ���

			// ��� Item�� ��� �ִ�(�߰��� Item)�� Client���� �˾ƾ� �Ѵ�.
			// ��� �ִ� Item�� Client���� access�� �� �����Ƿ� ��� Item�� ������.
			gpC_base->SendMessage(UI_ITEM_INSERT_FROM_QUICKSLOT,
																  m_focus_slot,
																  0,
																  (void *)p_cur_item); // ��� Item
			return true;
		}
		else
		{
			MItem * p_old_item = NULL;

			if (g_pQuickSlot->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),
												  m_focus_slot,
												  p_old_item))
			{
				gpC_base->SendMessage(UI_ITEM_DROP_TO_QUICKSLOT, m_focus_slot);
				return true;
			}
			else
			{
				// failed.
				return false;
			}
		}
	}
	else
	{
		// ���´�.
		const MItem * p_item = g_pQuickSlot->GetItem(m_focus_slot);

		if (p_item != NULL) // Item�� �ִ�.
		{
			if(m_bl_width)
			{
				item_x = x+m_p_slot_x[m_focus_slot]+QUICKSLOT_W/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = y+QUICKSLOT_H/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
			}
			else
			{
				item_x = x+QUICKSLOT_W/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = y+m_p_slot_x[m_focus_slot]+QUICKSLOT_H/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
			}

			g_pQuickSlot->RemoveItem((BYTE)m_focus_slot);

			gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_QUICKSLOT,
																  m_focus_slot, 
																  MAKEDWORD(item_x, item_y), 
																  (MItem *)p_item);
			// SHIFT������ Ŭ�������� ������ �ڵ� �̵� ó�� -> �κ��丮
			if(g_pSDLInput->KeyDown(DIK_LSHIFT))
			{
				POINT point;
				if(p_item != NULL && g_pInventory->GetFitPosition((MItem *)p_item, point))
				{
					const MItem* p_cur_item = g_pInventory->GetItem(point.x, point.y);
					
					// �ѿ� źâ�� ����� �Ͱ� ���� ���� insert item�̴�.
					// ��ġ�� ������ ��ġ�Ұ�쿡�� �߰��Ѵ�.
					if (p_item->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == point.x && p_cur_item->GetGridY() == point.y)
					{
						// ������ �ִ� item�� �߰��� �� �ִ� ���
						
						// ��� Item�� ��� �ִ�(�߰��� Item)�� Client���� �˾ƾ� �Ѵ�.
						// ��� �ִ� Item�� Client���� access�� �� �����Ƿ� ��� Item�� ������.
						gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
							point.x, point.y,
							(void *)p_cur_item); // ��� Item
					}
					else
					{	
						// �߰��� �� ���� ���
						MItem* p_old_item  = NULL;
						
						if (g_pInventory->CanReplaceItem((MItem *)p_item,		// �߰��� item
							point.x, point.y,	// �߰��� ��ġ 
							p_old_item))								// �����ִ� item
						{
							
							gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
								point.x, point.y,
								(void *)p_item);
							
						}
						else
						{
							// failed.
							return false;
						}
					}
					
				}
			}
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Use
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::Use(int slot, bool bFunctionKey)
{
	if (slot == NOT_SELECTED || gbl_item_lock || g_pQuickSlot == NULL)
		return;

	if (gpC_mouse_pointer->GetPickUpItem())
	{

	}
	else
	{
		const MItem * p_item = g_pQuickSlot->GetItem(slot);

		if (p_item) // Item�� �ִ�.
		{
			gpC_base->SendMessage(UI_ITEM_USE_QUICKSLOT,
									  slot,
									  bFunctionKey, 
									  (MItem *)p_item);
		}
	}
}

/*-----------------------------------------------------------------------------
- C_VS_UI_SLAYER_QUICKITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_SLAYER_QUICKITEM::C_VS_UI_SLAYER_QUICKITEM()
{
	g_RegisterWindow(this);

	AttrPin(true);

	m_image_spk.Open(SPK_SLAYER_QUICKITEM);

	// ���̴� ó�� no belt�� ����̴�. belt�� �����ϸ� �ٲ��.
	// ���̴� �����ϴ�.
	Set(0, 434, m_image_spk.GetWidth(END_EDGE), m_image_spk.GetHeight(END_EDGE));

	m_pC_button_group = new ButtonGroup(this);

	int alpha_button_offset_x = 14, alpha_button_offset_y = 7;
	int pushpin_button_offset_x = 14, pushpin_button_offset_y = 23;
	int rotate_button_offset_x = 5, rotate_button_offset_y = 8;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pushpin_button_offset_x, pushpin_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(rotate_button_offset_x, rotate_button_offset_y, m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED), ROTATE_ID, this, BUTTON_ROTATE_HILIGHTED));

	m_bl_width = true;

}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_SLAYER_QUICKITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_SLAYER_QUICKITEM::~C_VS_UI_SLAYER_QUICKITEM()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetQuickItemSlotHeight(!m_bl_width);

	g_UnregisterWindow(this);
	DeleteNew(m_pC_button_group);

}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_QUICKITEM::ToggleBelt
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_QUICKITEM::ToggleBelt()
{
//	m_bl_opened = !m_bl_opened;

	// reset Window width by open/close
	Run(PUSHPIN_ID);
	ResetSize();

	PlaySound(SOUND_SLAYER_BELT);
}

void	C_VS_UI_SLAYER_QUICKITEM::Open()
{
//	m_bl_opened = true;

	AttrAutoHide(Window::ATTRIBUTES_HIDE_NOT);
	// reset Window width by open/close
	ResetSize();

	PlaySound(SOUND_SLAYER_BELT);
}

void	C_VS_UI_SLAYER_QUICKITEM::Close()
{
//	m_bl_opened = false;
	AttrAutoHide(Window::ATTRIBUTES_HIDE_NOT);
	
	Run(PUSHPIN_ID);

	// reset Window width by open/close
	ResetSize();

	PlaySound(SOUND_SLAYER_BELT);
}

//-----------------------------------------------------------------------------
// ResetSize
//
// belt�� ��ü�ϰ����� size�� �缳���ؾ� �Ѵ�.
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::ResetSize()
{
	// ���ƾƾ� �ϵ��ڵ�!!
	int slot_size = m_image_spk.GetWidth(SLOT);
	if(m_bl_width)
	{
		h = m_image_spk.GetHeight(START_EDGE);
		w = m_image_spk.GetWidth(START_EDGE_EXTEND);
		w += m_image_spk.GetWidth(END_EDGE);
		w += m_image_spk.GetWidth(SLOT)*GetPocketCount();
		if(x <= 0)
		{
			w -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);

			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(STATCH_EDGE)+3;
		}
		else if(x+w >= g_GameRect.right)
		{
			m_bl_resize = true;
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;
		}
		else
		{
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE)+3;
		}
	}
	else
	{
		w = m_image_spk.GetWidth(START_EDGE+ROTATED_OFFSET);
		h = m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
		h += m_image_spk.GetHeight(END_EDGE+ROTATED_OFFSET);
		h += m_image_spk.GetHeight(SLOT+ROTATED_OFFSET)*GetPocketCount();
		if(y <= 0)
		{
			h -= m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET);
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(STATCH_EDGE)+3;
		}
		else if(y+h >= g_GameRect.bottom)
		{
			m_bl_resize = true;
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;
		}
		else
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;

	}
	m_p_slot_x[SLOT_2] = m_p_slot_x[SLOT_1]+slot_size;
	m_p_slot_x[SLOT_3] = m_p_slot_x[SLOT_2]+slot_size;
	m_p_slot_x[SLOT_4] = m_p_slot_x[SLOT_3]+slot_size;
	m_p_slot_x[SLOT_5] = m_p_slot_x[SLOT_4]+slot_size;
	m_p_slot_x[SLOT_6] = m_p_slot_x[SLOT_5]+slot_size;
	m_p_slot_x[SLOT_7] = m_p_slot_x[SLOT_6]+slot_size;
	m_p_slot_x[SLOT_8] = m_p_slot_x[SLOT_7]+slot_size;
}

//-----------------------------------------------------------------------------
// GetPocketCount
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_SLAYER_QUICKITEM::GetPocketCount() const
{
	if (g_pQuickSlot == NULL)
		return 0;

	return g_pQuickSlot->GetPocketNumber();
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::WindowEventReceiver(id_t event)
{
	switch(event)
	{
	case EVENT_WINDOW_MOVE: 
		{
			ResetSize();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SLAYER_QUICKITEM::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT));
	if(m_bl_width == gpC_vs_ui_window_manager->IsQuickItemSlotHeight())
		Run(ROTATE_ID);
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}

	PI_Processor::Start();

	gpC_window_manager->AppearWindow(this);

	m_focus_slot = NOT_SELECTED;
	m_bl_resize = false;
	ResetSize();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::KeyboardControl(UINT message, UINT key, long extra)
{

}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_QUICKITEM::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_slot = NOT_SELECTED;
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_SLAYER_QUICKITEM::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;

	// test open/close button
	bool ret = false;
	if(m_bl_width)
	{
		if(x+w >= g_GameRect.right)
			ret = !m_pC_button_group->MouseControl(message, _x, _y);
		else
			ret = !m_pC_button_group->MouseControl(message, w-_x, _y);
	}
	else
	{
		if(y <= 0)
			ret = !m_pC_button_group->MouseControl(message, h-_y, _x);
		else
			ret = !m_pC_button_group->MouseControl(message, _y, _x);
	}

	if (g_pQuickSlot != NULL) // item selection
	{
		int i;

		const MItem * p_selected_item;

		switch (message)
		{
		case M_MOVING:
			for (i=0; i < GetPocketCount(); i++)
			{
				int m;
				int loop;
				int px, py;
				
				if (gpC_mouse_pointer->GetPickUpItem())
					loop = ITEM_REF_POINT_COUNT;
				else
					loop = 1;
				
				for (m=0; m < loop; m++)
				{
					if(m_bl_width)
					{
						if (loop == ITEM_REF_POINT_COUNT)
						{
							px = g_item_ref_point[m].x+gpC_mouse_pointer->GetPointerX();
							py = g_item_ref_point[m].y+gpC_mouse_pointer->GetPointerY()-8;
						}
						else
						{
							px = _x+x;
							py = _y+y-8;
						}
					}
					else
					{
						if (loop == ITEM_REF_POINT_COUNT)
						{
							px = g_item_ref_point[m].y+gpC_mouse_pointer->GetPointerX()-8;
							py = g_item_ref_point[m].x+gpC_mouse_pointer->GetPointerY();
						}
						else
						{
							px = _x+x-8;
							py = _y+y;
						}
					}
					
					if (m_bl_width &&
						px >= x+m_p_slot_x[i] && px < x+m_p_slot_x[i]+QUICKSLOT_W &&
						py >= y && py < y+QUICKSLOT_H ||
						!m_bl_width &&
						px >= x && px < x+QUICKSLOT_W &&
						py >= y+m_p_slot_x[i] && py < y+m_p_slot_x[i]+QUICKSLOT_H
						)
					{
						if(m_focus_slot != i)
						{
							gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
							m_focus_slot = i;
						}
						
						p_selected_item = g_pQuickSlot->GetItem(m_focus_slot);
						if (p_selected_item != NULL)
						{
							if(m_bl_width)
								g_descriptor_manager.Set(DID_ITEM, x+m_p_slot_x[m_focus_slot], y, (void *)p_selected_item);
							else
								g_descriptor_manager.Set(DID_ITEM, x, y+m_p_slot_x[m_focus_slot], (void *)p_selected_item);
						}
						
						return true;
					}
				}
			}
			m_focus_slot = NOT_SELECTED;
			break;

			case M_LEFTBUTTON_DOWN:
			case M_LB_DOUBLECLICK:
				//
				// Item�� ������ ���´�.
				//
				{
					ret |= Click();

					if (ret)
						EMPTY_MOVE;
				}
				break;

			case M_RIGHTBUTTON_DOWN:
				//
				// Item�� ����Ѵ�.
				//
				Use(m_focus_slot);
				break;

			case M_RIGHTBUTTON_UP:
				break;
		}
	}
	switch (message)
	{
		case M_MOVING:
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (gpC_mouse_pointer->GetPickUpItem() == NULL && ret == false)
			{
				MoveReady();
				SetOrigin(_x, _y);
				break;
			}
			break;

		case M_LEFTBUTTON_UP:
			if(m_bl_resize)
			{
				m_bl_resize = false;
				if(m_bl_width)
				{
					if(x+w >= g_GameRect.right)
					{
						if(x+w == g_GameRect.right)
							x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
						w -= (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					}
				}
				else
				{
					if(y+h >= g_GameRect.bottom)
					{
						if(y+h == g_GameRect.bottom)
							y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
						h -= (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					}
				}
			}
			break;
	}

	return true;
}

void C_VS_UI_SLAYER_QUICKITEM::CancelPushState()
{
	m_pC_button_group->CancelPushState();
}

void C_VS_UI_SLAYER_QUICKITEM::Run(id_t id)
{
	switch (id)
	{
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;

	case PUSHPIN_ID:
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			if(m_bl_width)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
			else
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}
		EMPTY_MOVE;
		break;

	case ROTATE_ID:
		if(m_bl_width)
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
			x += w-h;
		}
		else
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
			x -= h-w;
		}
		m_bl_width = !m_bl_width;
		ResetSize();
		EMPTY_MOVE;
		break;
	}
}

void	C_VS_UI_SLAYER_QUICKITEM::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_quickitem_button_string[6] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WIDTH].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HEIGHT].GetString(),
	};
	
	if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[3],0,0);
	if(GetAttributes()->autohide && p_button->GetID() == PUSHPIN_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[4],0,0);
	if(m_bl_width && p_button->GetID() == ROTATE_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[5],0,0);
	else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[p_button->GetID()],0,0);
}

void	C_VS_UI_SLAYER_QUICKITEM::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	// ���ο�
	if(m_bl_width)
	{
		if(x+w >= g_GameRect.right)
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(p_button->x+x-1, p_button->y+y, BUTTON_ROTATE_HILIGHTED_PUSHED);
					else
						m_image_spk.BltLocked(p_button->x+x-1, p_button->y+y, BUTTON_ROTATE_HILIGHTED);
				}
			}
		}
		else
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(x+w-p_button->x-m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), p_button->y+y, BUTTON_ROTATE_HILIGHTED_PUSHED);
					else
						m_image_spk.BltLocked(x+w-p_button->x-m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), p_button->y+y, BUTTON_ROTATE_HILIGHTED);
				}
			}
		}
	}
	// ���ο�
	else
	{
		if(y > 0)
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(p_button->y+x, p_button->x+y-1, BUTTON_ROTATE_HILIGHTED_PUSHED+ROTATED_OFFSET);
					else
						m_image_spk.BltLocked(p_button->y+x, p_button->x+y-1, BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET);
				}
			}
		}
		else
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(x+p_button->y, y+h-p_button->x-m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET), BUTTON_ROTATE_HILIGHTED_PUSHED+ROTATED_OFFSET);
					else
						m_image_spk.BltLocked(x+p_button->y, y+h-p_button->x-m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET), BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET);
				}
			}
		}
	}
	
}

/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_SLAYER_QUICKITEM::Show()
{
	static int old_size = GetPocketCount();
	if(old_size != GetPocketCount())
	{
		ResetSize();
		old_size = GetPocketCount();
	}

	int i;
	int temp_x = x, temp_y = y;

	int len = 0;
	RECT rect[8];
	int num[8];

	// �ƾ� ���� �ϵ��ڵ�-_- ���� �����-_-;;
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		bool bl_statch = false;

		if(m_bl_width)
		{
			if(x <= 0)
			{
				m_image_spk.BltLocked(temp_x, temp_y+1, STATCH_EDGE);
				temp_x += m_image_spk.GetWidth(STATCH_EDGE);
			}
			else
			{
				if(x+w >= g_GameRect.right)
				{
					bl_statch = true;
					if(Moving() && m_bl_resize)	// �հ� ���� ����� �������ε�-_-;; �̷� �ϵ� �ڵ��� �ϸ� �ȵǴµ�-_-;;
						temp_x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND);
					temp_x += m_image_spk.GetWidth(START_EDGE_EXTEND);
					if(Moving() && m_bl_resize)	// �հ� ���� ����� �������ε�-_-;; �̷� �ϵ� �ڵ��� �ϸ� �ȵǴµ�-_-;;
						x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					if(Moving() && m_bl_resize)	// �հ� ���� ����� �������ε�-_-;; �̷� �ϵ� �ڵ��� �ϸ� �ȵǴµ�-_-;;
						x -= (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
				}
				else
				{
					
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE);
					temp_x += m_image_spk.GetWidth(START_EDGE);
				}
			}
		}
		else
		{
			if(y <= 0)
			{
				bl_statch = true;
				m_image_spk.BltLocked(temp_x+1, temp_y, STATCH_EDGE+ROTATED_OFFSET);
				temp_y += m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET);
			}
			else
			{
				if(Moving() && m_bl_resize)	// �հ� ���� ����� �������ε�-_-;; �̷� �ϵ� �ڵ��� �ϸ� �ȵǴµ�-_-;;
				{
					temp_y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND+ROTATED_OFFSET);
					temp_y += m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
					y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					y -= (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
				}
				else
				{
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND+ROTATED_OFFSET);
					temp_y += m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
				}
			}
		}
		
		MItem * p_selected_item = NULL;
		if(m_focus_slot != -1)
			p_selected_item = g_pQuickSlot->GetItem(m_focus_slot);
		
		if(GetAttributes()->alpha)
		{
			if(m_bl_width)
			{
				RECT alpha_rect;
				alpha_rect.left = temp_x+3;
				alpha_rect.right = temp_x+GetPocketCount()*m_image_spk.GetWidth(SLOT_ALPHA)-4;
				alpha_rect.top = temp_y+9;
				alpha_rect.bottom = temp_y+37;

				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
			else
			{
				RECT alpha_rect;
				alpha_rect.left = temp_x+9;
				alpha_rect.right = temp_x+37;
				alpha_rect.top = temp_y+3;
				alpha_rect.bottom = temp_y+GetPocketCount()*m_image_spk.GetHeight(SLOT_ALPHA+ROTATED_OFFSET)-4;

				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
		}

		for (i=0; i < GetPocketCount(); i++)
		{
			if(GetAttributes()->alpha)
			{
				if(m_bl_width)
					m_image_spk.BltLocked(temp_x, temp_y+1, SLOT_ALPHA);
				else
					m_image_spk.BltLocked(temp_x+1, temp_y, SLOT_ALPHA+ROTATED_OFFSET);
			}
			else
			{
				if(m_bl_width)
					m_image_spk.BltLocked(temp_x, temp_y+1, SLOT);
				else
					m_image_spk.BltLocked(temp_x+1, temp_y, SLOT+ROTATED_OFFSET);
			}
			
			const MItem * p_item = g_pQuickSlot->GetItem(i);
			
			if (p_item != NULL)
			{
				// ������ ���
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int _x;
				int _y;
				if(m_bl_width)
				{
					_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+3;
					_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+8;
				}
				else
				{
					_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+8;
					_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+3;
				}

				if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
				{
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					gpC_item->BltLockedOutline(_x, _y, WHITE, frame_id);
				}
				else
				{
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					gpC_item->BltLocked(_x, _y, frame_id);
				}
				
				// ����ǥ�� AlphaBox�� ���, ���ڴ� �ڿ��� ��´�
				if(p_item->IsPileItem())
				{
					int depth, number;
					for(depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
					if(depth == 0) depth = 1;

					rect[len].right = temp_x + QUICKSLOT_W+2;
					rect[len].left = rect[len].right - 7*depth;
					rect[len].bottom = temp_y+QUICKSLOT_H+7;
					rect[len].top = rect[len].bottom - 12;
					if(!m_bl_width)
					{
						rect[len].right += 5;
						rect[len].left += 5;
						rect[len].top -= 5;
						rect[len].bottom -= 5;
					}
					
					DrawAlphaBox(&rect[len], 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
					num[len] = p_item->GetNumber();
					len++;
					
				}
				
			}
			else
				// ���� ��ġ �̸� �� �� �ֵ��� �Ѵ�.
				if (gpC_mouse_pointer->GetPickUpItem() && 
					m_focus_slot == i)
				{
					MItem * p_item = gpC_mouse_pointer->GetPickUpItem();
					
					TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
					
					int _x;
					int _y;
					if(m_bl_width)
					{
						_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+3;
						_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+8;
					}
					else
					{
						_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+8;
						_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+3;
					}
					
					MItem * p_old_item;
					if (g_pQuickSlot->CanReplaceItem(p_item, m_focus_slot, p_old_item))
					{
						if (p_old_item)
							gpC_item->BltLockedOutlineOnly(_x, _y, ga_item_blink_color_table[g_blink_value], frame_id);
						else
							gpC_item->BltLockedOutlineOnly(_x, _y, POSSIBLE_COLOR, frame_id);
					}
				}
				
				
				if(m_bl_width)
					temp_x += m_image_spk.GetWidth(SLOT);
				else
					temp_y += m_image_spk.GetHeight(SLOT+ROTATED_OFFSET);
		}
		
		if(m_bl_width)
		{
			if(bl_statch)
				m_image_spk.BltLocked(temp_x, temp_y+1, STATCH_EDGE);
			else
			{
				m_image_spk.BltLocked(temp_x, temp_y, END_EDGE_EXTEND);
			}
		}
		else
		{
			if(bl_statch)
			{
				m_image_spk.BltLocked(temp_x, temp_y, END_EDGE_EXTEND+ROTATED_OFFSET);
			}
			else
			{
				if(y+h >= g_GameRect.bottom)
					m_image_spk.BltLocked(temp_x+1, temp_y, STATCH_EDGE+ROTATED_OFFSET);
				else
					m_image_spk.BltLocked(temp_x, temp_y, END_EDGE+ROTATED_OFFSET);
			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	if(Moving() && m_bl_resize)
	{
		if(m_bl_width)
		{
			if(x+w == g_GameRect.right)
				x += m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
		else 
		{
			if(y != 0)//+h == g_GameRect.bottom)
				y += m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
	}	
	
	if(Moving() && m_bl_resize)
	{
		if(m_bl_width)
		{
			if(x+w == g_GameRect.right)
				x -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
		else 
		{
			if(y != 0)//+h == g_GameRect.bottom)
				y -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
	}

	COLORREF markColor = RGB(220, 220, 220);;//RGB(140, 140, 255);
	char sz_temp[10];
	g_FL2_GetDC();
	for(i = 0; i < len; i++)
	{
		wsprintf(sz_temp, "%d", num[i]);
		g_PrintColorStr(rect[i].left, rect[i].top, sz_temp, gpC_base->m_item_desc_pi, markColor);	
//		g_Print(rect.left, rect.top, sz_temp, &gpC_base->m_item_desc_pi);	
	}
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();
	

	// show hot-key
	char * p_hotkey_mark[] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8"};
	if (m_focus_slot != NOT_SELECTED)
	//for (i=0; i < GetPocketCount(); i++)
	{
		POINT point;
		if(m_bl_width)
		{
			point.x = x+m_p_slot_x[m_focus_slot]+1;
			point.y = y+7;
		}
		else
		{
			point.x = x+8;
			point.y = y+m_p_slot_x[m_focus_slot];
		}

		// ������ �ڽ� ���..
//		if (gpC_base->m_p_DDSurface_back->Lock())
		{
			RECT rect;

			rect.left = point.x;
			rect.top = point.y+18;
			rect.right = rect.left + 14;
			rect.bottom = rect.top + 12;

			DrawAlphaBox(&rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

//			gpC_base->m_p_DDSurface_back->Unlock();
		}

		COLORREF markColor = RGB(140, 140, 255);
		g_PrintColorStr(point.x, point.y+18, p_hotkey_mark[m_focus_slot], gpC_base->m_item_desc_pi, markColor);	
	}
}

/*-----------------------------------------------------------------------------
- C_VS_UI_SLAYER_GEAR
-
-----------------------------------------------------------------------------*/

C_VS_UI_SLAYER_GEAR::C_VS_UI_SLAYER_GEAR()
{
	assert(MSlayerGear::MAX_GEAR_SLAYER == SLOT_SIZE);
	 

	m_slot_rect[SN_HELM].Set(80, 15, 60, 60);
	m_slot_rect[SN_NECKLACE].Set(95, 86,30, 30);
	m_slot_rect[SN_COAT].Set(80, 120, 60, 90);
	m_slot_rect[SN_LEFTHAND].Set(145, 120, 60, 90);
	m_slot_rect[SN_RIGHTHAND].Set(10, 120, 60, 90);
	m_slot_rect[SN_GLOVE].Set(15, 215, 60, 60);
	m_slot_rect[SN_BELT].Set(145, 215, 60, 60);
	m_slot_rect[SN_TROUSER].Set(80, 215, 60, 90);
	m_slot_rect[SN_LEFTBRACELET].Set(27, 279, 30, 30);
	m_slot_rect[SN_RIGHTBRACELET].Set(162, 279, 30, 30);
	m_slot_rect[SN_RIGHTRING1].Set(8, 315, 30, 30);
	m_slot_rect[SN_RIGHTRING2].Set(43, 315, 30, 30);
	m_slot_rect[SN_LEFTRING1].Set(144, 315, 30, 30);
	m_slot_rect[SN_LEFTRING2].Set(179, 315, 30, 30);
	m_slot_rect[SN_SHOES].Set(80, 307, 60, 60);
	
	m_slot_rect[SN_COREZAP1].Set(9, 313, 30, 30);
	m_slot_rect[SN_COREZAP2].Set(44, 313, 30, 30);
	m_slot_rect[SN_COREZAP3].Set(142, 313, 30, 30);
	m_slot_rect[SN_COREZAP4].Set(177, 313, 30, 30);
	
	m_slot_rect[SN_PDA].Set(13, 55, 60, 60);
	m_slot_rect[SN_SHOULDER].Set(140, 60, 60, 60);
	
	m_slot_rect[SN_BLOODBIBLE1].Set(6, 372, 30, 30);
	m_slot_rect[SN_BLOODBIBLE2].Set(41, 372, 30, 30);
	m_slot_rect[SN_BLOODBIBLE3].Set(76, 372, 30, 30);
	m_slot_rect[SN_BLOODBIBLE4].Set(111, 372, 30, 30);
	m_slot_rect[SN_BLOODBIBLE5].Set(146, 372, 30, 30);
	m_slot_rect[SN_BLOODBIBLE6].Set(181, 372, 30, 30);
	
	
	m_slot_size = SLOT_SIZE;
	
	m_p_slot_rect = m_slot_rect;
	m_p_slot_image = m_slot_image;

}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_SLAYER_GEAR
-
-----------------------------------------------------------------------------*/
C_VS_UI_SLAYER_GEAR::~C_VS_UI_SLAYER_GEAR()
{
}


/*-----------------------------------------------------------------------------
- KeyboardControl
-
-----------------------------------------------------------------------------*/
void C_VS_UI_SLAYER::KeyboardControl(UINT message, UINT key, long extra)
{
	m_pC_chatting->KeyboardControl(message, key, extra);
}

/*-----------------------------------------------------------------------------
- C_VS_UI_SLAYER
-
-----------------------------------------------------------------------------*/
C_VS_UI_SLAYER::C_VS_UI_SLAYER():C_VS_UI_TRIBE()
{
	m_pC_gear = NULL;
//	m_pC_advence_gear = NULL;
	m_pC_quickitem  = NULL;
	m_pC_main_spk  = NULL;

//	AttrPin(true);

	m_pC_main_spk = new C_SPRITE_PACK(SPK_SLAYER_MAIN);

	Set(g_GameRect.right-m_pC_main_spk->GetWidth()+1, g_GameRect.bottom-m_pC_main_spk->GetHeight()+1, m_pC_main_spk->GetWidth()-1, m_pC_main_spk->GetHeight()-1);

	m_pC_sys_button_spk = new C_SPRITE_PACK(SPK_SLAYER_SYS_BUTTON);

	// common buttons
	int tab_x = 18, tab_y = 49-4;  //modify by viva : menu button point
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_MENU), m_pC_sys_button_spk->GetHeight(TAB_MENU), TAB_MENU_ID, this, TAB_MENU) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_MENU);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_EXP), m_pC_sys_button_spk->GetHeight(TAB_EXP), TAB_EXP_ID, this, TAB_EXP) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_EXP);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_GUILD), TAB_GUILD_ID, this, TAB_GUILD) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_GUILD);
//	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_MSG), m_pC_sys_button_spk->GetHeight(TAB_MSG), TAB_MSG_ID, this, TAB_MSG) );
//	tab_x += m_pC_sys_button_spk->GetWidth(TAB_MSG);
	// 2004, 12, 2, sobeit add start - ���
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_UTIL), TAB_UTIL_ID, this, TAB_UTIL) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_UTIL);
	// 2004, 12, 2, sobeit add end
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_HELP), m_pC_sys_button_spk->GetHeight(TAB_HELP), TAB_HELP_ID, this, TAB_HELP) );

	int system_x = w-m_pC_main_spk->GetWidth(BUTTON_SYSTEM)-5, system_y = h-m_pC_main_spk->GetHeight(BUTTON_SYSTEM);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(system_x, system_y, m_pC_main_spk->GetWidth(BUTTON_SYSTEM), m_pC_main_spk->GetHeight(BUTTON_SYSTEM), SYSTEM_ID, this, BUTTON_SYSTEM) );
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(24, system_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this,C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN ) );
	//modify by viva : buttons under the menu
	const int button_x = 25, button_y = 72-4, button_x_gap = 37, button_y_gap = 37;
	
	// menu buttons
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INVENTORY), m_pC_sys_button_spk->GetHeight(BUTTON_INVENTORY), INVENTORY_ID, this, BUTTON_INVENTORY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_GEAR), m_pC_sys_button_spk->GetHeight(BUTTON_GEAR), GEAR_ID, this, BUTTON_GEAR) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_INFO), INFO_ID, this, BUTTON_INFO) );
	//add by viva : friend button
#ifdef __FRIEND_SYSTEM_VIVA__
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_FRIEND), m_pC_sys_button_spk->GetHeight(BUTTON_FRIEND), FRIEND_ID, this, BUTTON_FRIEND) );
#endif
	//end
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_PARTY), m_pC_sys_button_spk->GetHeight(BUTTON_PARTY), PARTY_ID, this, BUTTON_PARTY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_QUEST), m_pC_sys_button_spk->GetHeight(BUTTON_QUEST), QUEST_ID, this, BUTTON_QUEST) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_MAIL), m_pC_sys_button_spk->GetHeight(BUTTON_MAIL), MAIL_ID, this, BUTTON_MAIL) );
//	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_QUEST), m_pC_sys_button_spk->GetHeight(BUTTON_QUEST), QUEST_ID, this, BUTTON_QUEST) );

	// sms ��ư 
//	if(false == g_pUserInformation->IsNetmarble)
//add by zdj 2005.5.17
	// add by Coffee 2006.11.26
		//m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_SMS), m_pC_sys_button_spk->GetHeight(BUTTON_SMS), SMS_ID, this, BUTTON_SMS) );

	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_NAMING), m_pC_sys_button_spk->GetHeight(BUTTON_NAMING), NAMING_ID, this, BUTTON_NAMING) );

	// guild buttons
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_INFO), TEAM_INFO_ID, this, BUTTON_TEAM_INFO) );
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), TEAM_MEMBER_LIST_ID, this, BUTTON_TEAM_MEMBER_LIST) );
	// 2004, 10, 12, sobeit add start - ��� ���� ������
//	if(false == g_pUserInformation->IsNetmarble) // 2005, 1, 12, sobeit modify - �ݸ��� ��� ���� ��
	{
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_GUILD_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_GUILD_LIST), TEAM_LIST_ID, this, BUTTON_GUILD_LIST) );
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_WAIT_GUILD_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_WAIT_GUILD_LIST), TEAM_WAIT_LIST_ID, this, BUTTON_WAIT_GUILD_LIST) );
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_UNION), m_pC_sys_button_spk->GetHeight(BUTTON_UNION), TEAM_UNION_ID, this, BUTTON_UNION) );
	}
	// 2004, 10, 12, sobeit add end - ��� ���� ������

	// 2004, 12, 2, sobeit add start - ���
	m_pC_util_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_STORE), m_pC_sys_button_spk->GetHeight(BUTTON_STORE), UTIL_STORE_ID, this, BUTTON_STORE) );
	if(false == g_pUserInformation->IsNetmarble && false == g_pUserInformation->IsTestServer) // ������
	{
//add by zdj 2005.5.16
// add by Coffee 2006.11.26
		m_pC_util_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_POWER_JJANG), m_pC_sys_button_spk->GetHeight(BUTTON_POWER_JJANG), UTIL_POWER_JJANG_ID, this, BUTTON_POWER_JJANG) );
	}
	// 2004, 12, 2, sobeit add end

	// help buttons
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_HELP), HELP_ID, this, BUTTON_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_CHAT_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_CHAT_HELP), CHAT_HELP_ID, this, BUTTON_CHAT_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_BATTLE_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_BATTLE_HELP), BATTLE_HELP_ID, this, BUTTON_BATTLE_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_SKILL_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_SKILL_HELP), SKILL_HELP_ID, this, BUTTON_SKILL_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), GUILD_HELP_ID, this, BUTTON_TEAM_MEMBER_LIST) );

	// instance objects 
	m_pC_quickitem = new C_VS_UI_SLAYER_QUICKITEM;

//	if(m_skill_domain > 150)
//		m_pC_advence_gear = new C_VS_UI_ADVANCEMENTSLAYER_GEAR;
//	else
	m_pC_gear = new C_VS_UI_SLAYER_GEAR;
	
	m_time = "";
	m_date = "";

#ifndef _LIB
	SetTime("12:34:05");
	SetDate("2001/06/30");
#endif
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_SLAYER
-
-----------------------------------------------------------------------------*/
C_VS_UI_SLAYER::~C_VS_UI_SLAYER()
{
	DeleteNew(m_pC_gear);
//	DeleteNew(m_pC_advence_gear);
	DeleteNew(m_pC_quickitem);

}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER::GetGearWindow
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_GEAR * C_VS_UI_SLAYER::GetGearWindow() const
{
	return m_pC_gear;
}

//C_VS_UI_ADVANCEMENTSLAYER_GEAR * C_VS_UI_SLAYER ::GetAdvenceGearWindow() const
//{
//	return m_pC_advence_gear;
//}


//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER::CloseGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER::CloseGear()
{
	m_pC_gear->Finish();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER::OpenGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER::OpenGear(bool bl_set_load)
{
	m_pC_gear->Start(bl_set_load);
}

//void	C_VS_UI_SLAYER::AdvenceCloseGear()
//{
//	m_pC_advence_gear->Finish();
//}
//
//void	C_VS_UI_SLAYER::AdvenceOpenGear(bool bl_set_load1)
//{
//	m_pC_advence_gear->Start(bl_set_load1);
//}

//-----------------------------------------------------------------------------
// DoCommonActionBeforeEventOccured
//
// C_VS_UI_TRIBE::DoCommonActionBeforeEventOccured() ���� ����.
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER::DoCommonActionBeforeEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionBeforeEventOccured();
}

//-----------------------------------------------------------------------------
// DoCommonActionAfterEventOccured
//
// C_VS_UI_TRIBE::DoCommonActionAfterEventOccured() ���� ����.
//
// ��� Window�� �ݴ´�.
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER::DoCommonActionAfterEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionAfterEventOccured();
}

//-----------------------------------------------------------------------------
// ResetSlayerQuickItemSize
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER::ResetSlayerQuickItemSize()
{
	m_pC_quickitem->ResetSize();
}

//void C_VS_UI_SLAYER::HotKey_Extend()		// ctrl + e
//{
//	Run(EXTEND);
//}

void C_VS_UI_SLAYER::HotKey_F1()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_1);
}

void C_VS_UI_SLAYER::HotKey_QuickItemSlot()
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->ToggleBelt();
}

void C_VS_UI_SLAYER::HotKey_F2()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_2);
}

void C_VS_UI_SLAYER::HotKey_F3()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_3);
}

void C_VS_UI_SLAYER::HotKey_F4()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_4);
}

void C_VS_UI_SLAYER::HotKey_F5()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_5);
}

void C_VS_UI_SLAYER::HotKey_F6()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_6);
}

void C_VS_UI_SLAYER::HotKey_F7()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_7);
}

void C_VS_UI_SLAYER::HotKey_F8()				
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Use(C_VS_UI_SLAYER_QUICKITEM::SLOT_8);
}

void C_VS_UI_SLAYER::RunQuickItemSlot()
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Open();
}

void C_VS_UI_SLAYER::CloseQuickItemSlot()
{
	assert(m_pC_quickitem != NULL);

	m_pC_quickitem->Close();
}

bool C_VS_UI_SLAYER::IsRunningQuickItemSlot()
{
	assert(m_pC_quickitem != NULL);

//	return m_pC_quickitem->IsOpen();
	return true;
}


//-----------------------------------------------------------------------------
// MouseControl
//
// Item�� ��� ���� ��� Window �̵��̳� Main interface���� �Է��� �� �� ����.
//-----------------------------------------------------------------------------
/*bool C_VS_UI_SLAYER::MouseControl(UINT message, int _x, int _y)
{
//	if(m_pC_chat->GetCurrentMode() == WHISPER)
//		m_pC_chat->MouseControl(message, _x, _y);

	//
	// Main interface.
	//
//	int i;

	if (gpC_mouse_pointer->GetPickUpItem() == NULL) // Item�� ��� ������ �Է��� �� ����.
	{
//		if (m_pC_pds->GetPDS() == NO_PDS_MENU)// || m_pC_pds->PCSOnline() == true)
		if(m_bl_mark && m_pC_main_spk->IsPixel(_x-m_mark_x, _y-m_mark_y, MARK_WINDOW))
		{
			switch(message)
			{
				case M_LEFTBUTTON_DOWN:
				case M_LB_DOUBLECLICK:
				{
					if(_x > m_mark_x+9 && _x < m_mark_x + 9*14+9 && _y > m_mark_y+9 && _y < m_mark_y + 14*5+9)
					{
						m_mark_col = (_y - m_mark_y-9)/14;
						m_mark_row = (_x - m_mark_x-9)/14;
						KeyboardControl(WM_KEYDOWN, VK_RETURN, 0);
					}
				}
				break;

				case M_WHEEL_UP:
					Run(MARK_UP_ID);
					break;

				case M_WHEEL_DOWN:
					Run(MARK_DOWN_ID);
					break;

			}
			m_pC_mark_button_group->MouseControl(message, _x, _y);
		}
		else
		{
			m_pC_chatting->MouseControl(message, _x, _y);
			m_pC_button_group->MouseControl(message, _x, _y);
		}
	}

	return true;

}
*/

/*-----------------------------------------------------------------------------
- KeyboardControl
-
-----------------------------------------------------------------------------*/
//void C_VS_UI_SLAYER::KeyboardControl(UINT message, UINT key, long extra)
//{

//}

int qsort_SortExp(const void *arg1,const void *arg2)
{
	return ((*g_pSkillManager)[*(int*)arg1].GetDomainLevel() < (*g_pSkillManager)[*(int*)arg2].GetDomainLevel());
}

/*-----------------------------------------------------------------------------
- Show
- 
-----------------------------------------------------------------------------*/
void C_VS_UI_SLAYER::Show()
{

	if(gpC_base->m_p_DDSurface_back->Lock())
	{

		m_pC_main_spk->BltLocked(x, y, MAIN_WINDOW);
		int skill_id = m_pC_skill->GetSelectedSkillID();
		if (skill_id != NOT_SELECTED)
		{
			int max_exp = (*g_pSkillInfoTable)[skill_id].maxExp;
			int exp = (*g_pSkillInfoTable)[skill_id].GetSkillExp();
			int exp_percent;
			if(max_exp > 0)
				exp_percent = (exp % (max_exp/100))*100/(max_exp/100);
			else exp_percent = 0;
			
			if(exp == max_exp)exp_percent = 100;
			Rect rect;
			rect.Set(0, m_pC_main_spk->GetHeight(SKILL_EXP_BAR)-m_pC_main_spk->GetHeight(SKILL_EXP_BAR)*exp_percent/100, m_pC_main_spk->GetWidth(SKILL_EXP_BAR), m_pC_main_spk->GetHeight(SKILL_EXP_BAR)*exp_percent/100);
			m_pC_main_spk->BltLockedClip(x+79, y+6, rect, SKILL_EXP_BAR);
		}
		
		int sec = 0, min = 0, hour = 0;
		char sz_temp[20];
		strcpy(sz_temp, m_time.c_str());
		sec = atoi(sz_temp+strlen(sz_temp)-2);
		sz_temp[strlen(sz_temp)-3] = '\0';
		min = atoi(sz_temp+strlen(sz_temp)-2);
		sz_temp[strlen(sz_temp)-3] = '\0';
		hour = atoi(sz_temp);
		
		const int icon_x = 88, icon_y = 29-2;	//modify by viva : icon_y
		if(hour >= 8 && hour < 16)	// ���̴�
			m_pC_main_spk->BltLocked(x+icon_x, y+icon_y, ICON_SUN);
		else if(hour >= 20 || hour < 4)	// ���̴�
			m_pC_main_spk->BltLocked(x+icon_x+3, y+icon_y, ICON_MOON);
		else	// ���� ��ü�� �ñ�-_- �ñ�-_-?;;
		{
			if(hour >=4 && hour < 8)	// �ضߴ� �ð�
			{
				int time = (hour-4)*60+min;
				// �ض� ���̶� ����� �޶� ������-_-;
				Rect rect(0, 0, m_pC_main_spk->GetWidth(ICON_MOON), m_pC_main_spk->GetHeight(ICON_MOON)*(240-time)/240);
				rect.y = m_pC_main_spk->GetHeight(ICON_MOON)-rect.h;
				m_pC_main_spk->BltLockedClip(x+icon_x+3, y+icon_y-rect.y, rect, ICON_MOON);
				rect.w = m_pC_main_spk->GetWidth(ICON_SUN);
				rect.h = m_pC_main_spk->GetHeight(ICON_SUN)*(time)/240;
				rect.y = 0;
				m_pC_main_spk->BltLockedClip(x+icon_x, y+icon_y+m_pC_main_spk->GetHeight(ICON_SUN)-rect.h, rect, ICON_SUN);
			}
			else						// �ƴϸ� ������ �ð��̰���-_-
			{
				int time = (hour-16)*60+min;
				// �ض� ���̶� ����� �޶� ������-_-;
				Rect rect(0, 0, m_pC_main_spk->GetWidth(ICON_SUN), m_pC_main_spk->GetHeight(ICON_SUN)*(240-time)/240);
				rect.y = m_pC_main_spk->GetHeight(ICON_SUN)-rect.h;
				m_pC_main_spk->BltLockedClip(x+icon_x, y+icon_y-rect.y, rect, ICON_SUN);
				rect.w = m_pC_main_spk->GetWidth(ICON_MOON);
				rect.h = m_pC_main_spk->GetHeight(ICON_MOON)*(time)/240;
				rect.y = 0;
				m_pC_main_spk->BltLockedClip(x+icon_x+3, y+icon_y+m_pC_main_spk->GetHeight(ICON_MOON)-rect.h, rect, ICON_MOON);
			}
		}

		m_pC_common_button_group->Show();
		
		switch(m_selected_tab)
		{
		case TAB_MENU_ID:
			m_pC_menu_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			m_pC_menu_button_group->ShowDescription();
			break;
			
		case TAB_EXP_ID:
			// EXP �� ��ư�� ����
			{//modify by viva : bar_y
				const int bar_x = 98, bar_y = 74-3, str_x = 27, num_x = 76, bar_gap = 12;
				char sz_temp[10];
				
				int domain[3];
				domain[0] = -1;
				domain[1] = -1;
				domain[2] = -1;
				
				for(int i = 0; i < 3; i++)
				{
					int high_level = -1;
					
					for(int d = SKILLDOMAIN_BLADE ; d <= SKILLDOMAIN_ENCHANT; d++)
					{
						const int domain_level = (*g_pSkillManager)[d].GetDomainLevel();
						if(d != domain[0] && d != domain[1] && d != domain[2] && domain_level > high_level)
						{
							domain[i] = d;
							high_level = domain_level;
						}
					}
				}
				
				if((*g_pSkillManager)[domain[0]].GetDomainLevel() == 0 &&
					(*g_pSkillManager)[domain[1]].GetDomainLevel() == 0 &&
					(*g_pSkillManager)[domain[2]].GetDomainLevel() == 0)
				{
					int domain2[5] = { SKILLDOMAIN_BLADE, SKILLDOMAIN_SWORD, SKILLDOMAIN_GUN, SKILLDOMAIN_HEAL, SKILLDOMAIN_ENCHANT };

					qsort((void*)domain2,5,sizeof(int),qsort_SortExp);
					
					memcpy(domain,domain2,sizeof(int) * 3);
				}
				
				char *domain_string[5] =
				{
					(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_BLADE].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_SWORD].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_GUN].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_HEAL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_ENCHANT].GetString(),
				};
				
				//			if(gpC_base->m_p_DDSurface_back->Lock())
				{
					Rect rect;
					int exp_remain = g_char_slot_ingame.STR_EXP_REMAIN;
					int goal_exp = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).GoalExp;
					if(goal_exp)
					{
						m_pC_main_spk->BltLocked(x+bar_x, y+bar_y+bar_gap*0, EXP_BACK);
						rect.Set(0, 0, m_pC_main_spk->GetWidth(EXP_BAR)*(goal_exp - exp_remain)/(max(1, goal_exp)), m_pC_main_spk->GetHeight(EXP_BAR));
						m_pC_main_spk->BltLockedClip(x+bar_x+3, y+bar_y+bar_gap*0+3, rect, EXP_BAR);
					}
					exp_remain = g_char_slot_ingame.DEX_EXP_REMAIN;
					goal_exp = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).GoalExp;
					if(goal_exp)
					{
						m_pC_main_spk->BltLocked(x+bar_x, y+bar_y+bar_gap*1, EXP_BACK);
						rect.Set(0, 0, m_pC_main_spk->GetWidth(EXP_BAR)*(goal_exp - exp_remain)/(max(1, goal_exp)), m_pC_main_spk->GetHeight(EXP_BAR));
						m_pC_main_spk->BltLockedClip(x+bar_x+3, y+bar_y+bar_gap*1+3, rect, EXP_BAR);
					}
					exp_remain = g_char_slot_ingame.INT_EXP_REMAIN;
					goal_exp = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).GoalExp;
					if(goal_exp)
					{
						m_pC_main_spk->BltLocked(x+bar_x, y+bar_y+bar_gap*2, EXP_BACK);
						rect.Set(0, 0, m_pC_main_spk->GetWidth(EXP_BAR)*(goal_exp - exp_remain)/(max(1, goal_exp)), m_pC_main_spk->GetHeight(EXP_BAR));
						m_pC_main_spk->BltLockedClip(x+bar_x+3, y+bar_y+bar_gap*2+3, rect, EXP_BAR);
					}

					for(int i = 0; i < 3; i++)
					{
						const int domain_level = (*g_pSkillManager)[domain[i]].GetDomainLevel();
						const int exp_remain = (*g_pSkillManager)[domain[i]].GetDomainExpRemain();
						const int goal_exp = (*g_pSkillManager)[domain[i]].GetExpInfo(domain_level).GoalExp;
						//������ ����ġ ��
						m_pC_main_spk->BltLocked(x+bar_x, y+bar_y+bar_gap*(3+i), EXP_BACK);
						rect.Set(0, 0, m_pC_main_spk->GetWidth(EXP_BAR)*(goal_exp - exp_remain)/(goal_exp), m_pC_main_spk->GetHeight(EXP_BAR));
						m_pC_main_spk->BltLockedClip(x+bar_x+3, y+bar_y+bar_gap*(3+i)+3, rect, EXP_BAR);
					}
					
					gpC_base->m_p_DDSurface_back->Unlock();
				}
				
				g_FL2_GetDC();
				
				g_PrintColorStrShadow(x+str_x, y+bar_y+bar_gap*0, (*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_STR].GetString(), gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				wsprintf(sz_temp, "%d", g_char_slot_ingame.STR_CUR);
				g_PrintColorStrShadow(x+num_x, y+bar_y+bar_gap*0, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				
				g_PrintColorStrShadow(x+str_x, y+bar_y+bar_gap*1, (*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DEX].GetString(), gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				wsprintf(sz_temp, "%d", g_char_slot_ingame.DEX_CUR);
				g_PrintColorStrShadow(x+num_x, y+bar_y+bar_gap*1, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				
				g_PrintColorStrShadow(x+str_x, y+bar_y+bar_gap*2, (*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_INT].GetString(), gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				wsprintf(sz_temp, "%d", g_char_slot_ingame.INT_CUR);
				g_PrintColorStrShadow(x+num_x, y+bar_y+bar_gap*2, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);

				for(int i = 0; i < 3; i++)
				{
					g_PrintColorStrShadow(x+str_x, y+bar_y+bar_gap*(3+i), domain_string[domain[i]-SKILLDOMAIN_BLADE], gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
					const int domain_level = (*g_pSkillManager)[domain[i]].GetDomainLevel();
					wsprintf(sz_temp, "%d", domain_level);
					g_PrintColorStrShadow(x+num_x, y+bar_y+bar_gap*(3+i), sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				}				
			}
			break;
			
		case TAB_GUILD_ID:
			m_pC_guild_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			m_pC_guild_button_group->ShowDescription();
			break;
			
		case TAB_MSG_ID:
			m_pC_msg_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			m_pC_guild_button_group->ShowDescription();
			break;
			
		case TAB_UTIL_ID:
			m_pC_util_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			m_pC_util_button_group->ShowDescription();
			break;

		case TAB_HELP_ID:
			m_pC_help_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			m_pC_help_button_group->ShowDescription();
			break;
			
		default:
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			break;
		}
	}
	m_pC_common_button_group->ShowDescription();
	
	g_PrintColorStr(x-5 +128 - g_GetStringWidth(m_date.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+10+3, m_date.c_str() ,gpC_base->m_chatting_pi, RGB_WHITE);	
	g_PrintColorStr(x-5 + 6+128 - g_GetStringWidth(m_time.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+31-3, m_time.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);	
	g_FL2_ReleaseDC();

	m_pC_skill->Show2();
	SHOW_WINDOW_ATTR;

}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER::Start()
{
	C_VS_UI_TRIBE::Start();

	m_pC_quickitem->Start();	
	m_pC_skill->Start(); 
	m_pC_hpbar->Start();
	m_pC_effect_status->Start();
	m_pC_minimap->Start();

	// add by Coffee 2007-3-6 ���������ͼ
//	m_pC_worldmap->Start();
	// end 

	WindowEventReceiver(EVENT_WINDOW_MOVE);

}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER::Process()
{
	C_VS_UI_TRIBE::Process();
}

C_VS_UI_SLAYER_PORTAL::C_VS_UI_SLAYER_PORTAL()
{
	g_RegisterWindow(this);

	AttrTopmost(true);

	m_etc_spk.Open(SPK_SLAYER_PORTAL_ETC);
	m_map_spk.Open(SPK_SLAYER_PORTAL_MAP);

	Set(g_GameRect.right/2-m_map_spk.GetWidth(0)/2, g_GameRect.bottom/2 - m_map_spk.GetHeight(0)/2, m_map_spk.GetWidth(0), m_map_spk.GetHeight(0));

	m_left_x = -12, m_left_y = 0;
	m_right_x = 313, m_right_y = m_left_y;
	m_close_x = 255, m_close_y = 105;

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + m_left_x, y + m_left_y, m_etc_spk.GetWidth(LEFT), m_etc_spk.GetHeight(LEFT), LEFT, this, LEFT));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + m_right_x, y + m_right_y, m_etc_spk.GetWidth(RIGHT), m_etc_spk.GetHeight(RIGHT), RIGHT, this, RIGHT));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + m_close_x, y + m_close_y, m_etc_spk.GetWidth(CLOSE), m_etc_spk.GetHeight(CLOSE), CLOSE, this, CLOSE));


	// ���� ��
	MAP_SPK_INDEX map_convert[MAP_MAX] = { ESLANIAN, LIMBO, DROBETA, RODIN, TIMORE, PERONA };

	int map = gC_vs_ui.GetZoneID()/10-1;
	if(map < 0 || map >= MAP_MAX)
		m_map = 0;
	else
		m_map = map_convert[map];

//#define __FLAG_SET_MODE
#ifdef  __FLAG_SET_MODE

	// FLAG ����
	PORTAL_FLAG temp_flag;

	// ������
	temp_flag.zone_id = 21;
	temp_flag.x = 292;
	temp_flag.y = 27;
	temp_flag.portal_x = 227;
	temp_flag.portal_y = 59;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 21;
	temp_flag.x = 211;
	temp_flag.y = 38;
	temp_flag.portal_x = 54;
	temp_flag.portal_y = 111;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 22;
	temp_flag.x = 115;
	temp_flag.y = 58;
	temp_flag.portal_x = 120;
	temp_flag.portal_y = 214;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 22;
	temp_flag.x = 121;
	temp_flag.y = 24;
	temp_flag.portal_x = 127;
	temp_flag.portal_y = 12;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 23;
	temp_flag.x = 290;
	temp_flag.y = 81;
	temp_flag.portal_x = 185;
	temp_flag.portal_y = 57;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 23;
	temp_flag.x = 230;
	temp_flag.y = 112;
	temp_flag.portal_x = 74;
	temp_flag.portal_y = 192;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 24;
	temp_flag.x = 145;
	temp_flag.y = 118;
	temp_flag.portal_x = 187;
	temp_flag.portal_y = 206;
	m_flag[LIMBO].push_back(temp_flag);

	temp_flag.zone_id = 24;
	temp_flag.x = 81;
	temp_flag.y = 98;
	temp_flag.portal_x = 76;
	temp_flag.portal_y = 100;
	m_flag[LIMBO].push_back(temp_flag);


	// ������Ͼ�
	temp_flag.zone_id = 11;
	temp_flag.x = 206;
	temp_flag.y = 41;
	temp_flag.portal_x = 48;
	temp_flag.portal_y = 131;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 11;
	temp_flag.x = 292;
	temp_flag.y = 31;
	temp_flag.portal_x = 223;
	temp_flag.portal_y = 75;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 12;
	temp_flag.x = 67;
	temp_flag.y = 36;
	temp_flag.portal_x = 16;
	temp_flag.portal_y = 99;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 12;
	temp_flag.x = 157;
	temp_flag.y = 45;
	temp_flag.portal_x = 211;
	temp_flag.portal_y = 149;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 13;
	temp_flag.x = 259;
	temp_flag.y = 105;
	temp_flag.portal_x = 127;
	temp_flag.portal_y = 163;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 13;
	temp_flag.x = 260;
	temp_flag.y = 84;
	temp_flag.portal_x = 140;
	temp_flag.portal_y = 74;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 14;
	temp_flag.x = 72;
	temp_flag.y = 84;
	temp_flag.portal_x = 50;
	temp_flag.portal_y = 77;
	m_flag[ESLANIAN].push_back(temp_flag);

	temp_flag.zone_id = 14;
	temp_flag.x = 174;
	temp_flag.y = 79;
	temp_flag.portal_x = 233;
	temp_flag.portal_y = 55;
	m_flag[ESLANIAN].push_back(temp_flag);


	// �ε��
	temp_flag.zone_id = 41;
	temp_flag.x = 287;
	temp_flag.y = 44;
	temp_flag.portal_x = 207;
	temp_flag.portal_y = 142;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 41;
	temp_flag.x = 211;
	temp_flag.y = 42;
	temp_flag.portal_x = 55;
	temp_flag.portal_y = 131;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 42;
	temp_flag.x = 121;
	temp_flag.y = 28;
	temp_flag.portal_x = 124;
	temp_flag.portal_y = 53;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 42;
	temp_flag.x = 113;
	temp_flag.y = 61;
	temp_flag.portal_x = 119;
	temp_flag.portal_y = 223;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 43;
	temp_flag.x = 210;
	temp_flag.y = 90;
	temp_flag.portal_x = 45;
	temp_flag.portal_y = 99;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 43;
	temp_flag.x = 308;
	temp_flag.y = 108;
	temp_flag.portal_x = 217;
	temp_flag.portal_y = 184;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 44;
	temp_flag.x = 89;
	temp_flag.y = 91;
	temp_flag.portal_x = 82;
	temp_flag.portal_y = 103;
	m_flag[RODIN].push_back(temp_flag);

	temp_flag.zone_id = 44;
	temp_flag.x = 149;
	temp_flag.y = 106;
	temp_flag.portal_x = 194;
	temp_flag.portal_y = 163;
	m_flag[RODIN].push_back(temp_flag);


	// ��κ�Ÿ
	temp_flag.zone_id = 31;
	temp_flag.x = 198;
	temp_flag.y = 29;
	temp_flag.portal_x = 27;
	temp_flag.portal_y = 58;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 31;
	temp_flag.x = 252;
	temp_flag.y = 58;
	temp_flag.portal_x = 132;
	temp_flag.portal_y = 211;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 32;
	temp_flag.x = 153;
	temp_flag.y = 39;
	temp_flag.portal_x = 192;
	temp_flag.portal_y = 114;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 32;
	temp_flag.x = 84;
	temp_flag.y = 30;
	temp_flag.portal_x = 47;
	temp_flag.portal_y = 63;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 33;
	temp_flag.x = 198;
	temp_flag.y = 95;
	temp_flag.portal_x = 47;
	temp_flag.portal_y = 63;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 33;
	temp_flag.x = 291;
	temp_flag.y = 114;
	temp_flag.portal_x = 192;
	temp_flag.portal_y = 114;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 34;
	temp_flag.x = 138;
	temp_flag.y = 103;
	temp_flag.portal_x = 175;
	temp_flag.portal_y = 153;
	m_flag[DROBETA].push_back(temp_flag);

	temp_flag.zone_id = 34;
	temp_flag.x = 62;
	temp_flag.y = 94;
	temp_flag.portal_x = 38;
	temp_flag.portal_y = 117;
	m_flag[DROBETA].push_back(temp_flag);

	// Ƽ��
	temp_flag.zone_id = 51;
	temp_flag.x = 201;
	temp_flag.y = 36;
	temp_flag.portal_x = 42;
	temp_flag.portal_y = 90;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 51;
	temp_flag.x = 275;
	temp_flag.y = 59;
	temp_flag.portal_x = 188;
	temp_flag.portal_y = 220;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 52;
	temp_flag.x = 66;
	temp_flag.y = 27;
	temp_flag.portal_x = 40;
	temp_flag.portal_y = 48;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 52;
	temp_flag.x = 160;
	temp_flag.y = 51;
	temp_flag.portal_x = 216;
	temp_flag.portal_y = 171;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 53;
	temp_flag.x = 188;
	temp_flag.y = 91;
	temp_flag.portal_x = 17;
	temp_flag.portal_y = 104;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 53;
	temp_flag.x = 293;
	temp_flag.y = 95;
	temp_flag.portal_x = 199;
	temp_flag.portal_y = 110;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 54;
	temp_flag.x = 77;
	temp_flag.y = 83;
	temp_flag.portal_x = 70;
	temp_flag.portal_y = 62;
	m_flag[TIMORE].push_back(temp_flag);

	temp_flag.zone_id = 54;
	temp_flag.x = 135;
	temp_flag.y = 125;
	temp_flag.portal_x = 178;
	temp_flag.portal_y = 237;
	m_flag[TIMORE].push_back(temp_flag);

	// ��γ�
	temp_flag.zone_id = 61;
	temp_flag.x = 141;
	temp_flag.y = 44;
	temp_flag.portal_x = 44;
	temp_flag.portal_y = 139;
	m_flag[PERONA].push_back(temp_flag);

	temp_flag.zone_id = 62;
	temp_flag.x = 104;
	temp_flag.y = 35;
	temp_flag.portal_x = 93;
	temp_flag.portal_y = 89;
	m_flag[PERONA].push_back(temp_flag);

	temp_flag.zone_id = 63;
	temp_flag.x = 142;
	temp_flag.y = 90;
	temp_flag.portal_x = 57;
	temp_flag.portal_y = 102;
	m_flag[PERONA].push_back(temp_flag);

	temp_flag.zone_id = 64;
	temp_flag.x = 49;
	temp_flag.y = 128;
	temp_flag.portal_x = 40;
	temp_flag.portal_y = 243;
	m_flag[PERONA].push_back(temp_flag);

	ofstream flag_file(INF_SLAYER_PORTAL, ios::binary);
	int map_max = MAP_MAX;
	flag_file.write((const char *)&map_max, sizeof(int));
	for(int j = 0; j < MAP_MAX; j++)
	{
		int size = m_flag[j].size();
		flag_file.write((const char *)&size, sizeof(int));
		for(int i = 0; i < size; i++)
		{
			flag_file.write((const char *)&m_flag[j][i].zone_id, sizeof(int));
			flag_file.write((const char *)&m_flag[j][i].x, sizeof(int));
			flag_file.write((const char *)&m_flag[j][i].y, sizeof(int));
			flag_file.write((const char *)&m_flag[j][i].portal_x, sizeof(int));
			flag_file.write((const char *)&m_flag[j][i].portal_y, sizeof(int));
		}
	}
	flag_file.close();
#else
	UI_PORTAL_FLAG temp_flag;
	CRarFile flag_file;
	flag_file.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);

	flag_file.Open(INF_SLAYER_PORTAL);
	int map_max = *(int *)flag_file.Read(sizeof(int));
	assert(map_max == MAP_MAX);
	for(int j = 0; j < MAP_MAX; j++)
	{
		int size;
		size = *((int *)(flag_file.Read(sizeof(int))));
		assert(size != 0);
		for(int i = 0; i < size; i++)
		{
			temp_flag.zone_id = *((int *)flag_file.Read(sizeof(int)));
			temp_flag.x = *((int *)flag_file.Read(sizeof(int)));
			temp_flag.y = *((int *)flag_file.Read(sizeof(int)));
			temp_flag.portal_x = *((int *)flag_file.Read(sizeof(int)));
			temp_flag.portal_y = *((int *)flag_file.Read(sizeof(int)));
			assert(temp_flag.zone_id >= 0);
			assert(temp_flag.x >= 0);
			assert(temp_flag.x >= 0);
			assert(temp_flag.portal_x >= 0 && temp_flag.portal_x < 256);
			assert(temp_flag.portal_y >= 0 && temp_flag.portal_y < 256);

			if( g_pSystemAvailableManager->ZoneFiltering( temp_flag.zone_id ) )
				m_flag[j].push_back(temp_flag);
			
			bool bExist=false;
			
			std::vector<int>::iterator itr = m_zoneidList[j].begin();
			std::vector<int>::iterator enditr = m_zoneidList[j].end();

			while( itr != enditr )
			{
				if( *itr == temp_flag.zone_id )
					bExist = true;
				itr++;
			}

			if( !bExist )
				m_zoneidList[j].push_back( temp_flag.zone_id );
		}
	}
#endif
}

C_VS_UI_SLAYER_PORTAL::~C_VS_UI_SLAYER_PORTAL()
{
	g_UnregisterWindow(this);

	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	PlaySound(SOUND_WORLD_MAP);

}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}


//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_PORTAL::Show()
{

	static int flag_animation = 0;
	const int flag_frame = 3;
	
	flag_animation++;
	if(flag_animation >= 7*flag_frame)
		flag_animation%=7*flag_frame;

	if(gpC_base->m_p_DDSurface_back->Lock())
	{		
		m_map_spk.BltLocked(x, y, m_map);
		
		for(int i = 0; i < m_flag[m_map].size(); i++)
		{
			int _x = gpC_mouse_pointer->GetX(), _y = gpC_mouse_pointer->GetY();
			
			if(_x >= x+m_flag[m_map][i].x -10 && _x <= x+m_flag[m_map][i].x +10 &&
				_y >= y+m_flag[m_map][i].y -20 && _y <= y+m_flag[m_map][i].y +10)
				m_etc_spk.BltLocked(x+m_flag[m_map][i].x - m_etc_spk.GetWidth(FLAG_HILIGHT)/2-1, y+m_flag[m_map][i].y - m_etc_spk.GetHeight(FLAG_HILIGHT)+3, FLAG_HILIGHT+flag_animation/flag_frame);
			else
				m_etc_spk.BltLocked(x+m_flag[m_map][i].x - m_etc_spk.GetWidth(FLAG)/2-1, y+m_flag[m_map][i].y - m_etc_spk.GetHeight(FLAG)+3, FLAG+flag_animation/flag_frame);
		}		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_button_group->ShowDescription();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::ShowDescription
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_SLAYER_PORTAL::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char *m_help_string[MAP_MAX+1] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_RODIN].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DROBETA].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERONA].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIMORE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(),		
	};

	// �ݸ�����
	int map_max = MAP_MAX;
//	if(g_pUserInformation->IsNetmarble)
//		map_max = 4;

	if(m_etc_spk.IsPixel(gpC_mouse_pointer->GetX()-p_button->x, gpC_mouse_pointer->GetY()-p_button->y, p_button->GetID()))
	{
		id_t ButtonID=p_button->GetID();
		if(ButtonID==CLOSE)
			g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[MAP_MAX],0,0);
		else if(ButtonID==LEFT)
		{
			int map = (m_map-1+map_max)%map_max;
			
			std::vector<int>::iterator itr = m_zoneidList[map].begin();
			std::vector<int>::iterator enditr =m_zoneidList[map].end();
			
			bool bCheck = false;
			while( itr != enditr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( *itr ) )
				{
					bCheck = true;
					break;
				}
				itr++;
			}
			if( bCheck )
				map = GetNext( map, true );
			g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[map],0,0);
		}
		else if(ButtonID==RIGHT)
		{
			int map = (m_map+1)%map_max;
			std::vector<int>::iterator itr = m_zoneidList[map].begin();
			std::vector<int>::iterator enditr =m_zoneidList[map].end();
			bool bCheck = false;
			
			while( itr != enditr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( *itr ) )
				{
					bCheck = true;
					break;
				}
				itr++;
			}
			if( bCheck )
				map = GetNext(map, false );
			g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[map],0,0);
		}
	}	
}


//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);

//	if (p_button->GetFocusState())
//		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()-1],0,0);

	if(p_button->GetFocusState() && m_etc_spk.IsPixel(gpC_mouse_pointer->GetX()-p_button->x, gpC_mouse_pointer->GetY()-p_button->y, p_button->GetID()))
	{
		if(p_button->GetPressState())
		{
			switch(p_button->GetID())
			{
			case CLOSE:				
				m_etc_spk.BltLocked(p_button->x, p_button->y, p_button->GetID()+2);
				break;
				
			case LEFT:
					m_etc_spk.BltLocked(p_button->x+2, p_button->y+1, p_button->GetID()+2);
				EMPTY_MOVE;
				break;
				
			case RIGHT:
					m_etc_spk.BltLocked(p_button->x, p_button->y+1, p_button->GetID()+2);
				EMPTY_MOVE;
				break;
			}
		} else
			m_etc_spk.BltLocked(p_button->x, p_button->y, p_button->GetID()+1);
	}
	else
		m_etc_spk.BltLocked(p_button->x, p_button->y, p_button->GetID());

//	if(p_button->GetFocusState() && p_button->GetPressState())m_image_spk.Blt(p_button->x, p_button->y, p_button->m_image_index);
//	else if(m_iLevel == p_button->GetID())
//		m_image_spk.Blt(p_button->x-1, p_button->y-1, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
				gpC_base->SendMessage(UI_CLOSE_SLAYER_PORTAL);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SLAYER_PORTAL::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	int flag = -1;
	for(int i = 0; i < m_flag[m_map].size(); i++)
	{
		if(_x >= x+m_flag[m_map][i].x -10 && _x <= x+m_flag[m_map][i].x +10 &&
			_y >= y+m_flag[m_map][i].y -20 && _y <= y+m_flag[m_map][i].y +10)
			flag = i;
	}

	if(flag != -1)
	{
		switch(message)
		{
		case M_MOVING:
			{
				static S_DEFAULT_HELP_STRING flag_string;
				static char flag_temp[50];

				flag_string.sz_main_str = g_pZoneTable->Get(m_flag[m_map][flag].zone_id)->Name.GetString();
				flag_string.sz_sub_str = flag_temp;
				wsprintf(flag_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), m_flag[m_map][flag].portal_x, m_flag[m_map][flag].portal_y);

				g_descriptor_manager.Set(DID_HELP, m_flag[m_map][flag].x+x, m_flag[m_map][flag].y+y-m_etc_spk.GetHeight(FLAG)-3, (void *)&flag_string);
			}
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			{
				int left = m_flag[m_map][flag].zone_id;
				int right = ((m_flag[m_map][flag].portal_x)<<16) + m_flag[m_map][flag].portal_y;
				gpC_base->SendMessage(UI_SLAYER_PORTAL, left, right);
			}
			break;
		}
	}

	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SLAYER_PORTAL::IsPixel(int _x, int _y)
{
	bool re = m_map_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));

	if(re == false)
	{
		if(m_etc_spk.IsPixel(_x - m_left_x-x, _y - m_left_y-y, LEFT) ||
			m_etc_spk.IsPixel(_x - m_right_x-x, _y - m_right_y-y, RIGHT) ||
			m_etc_spk.IsPixel(_x - m_close_x-x, _y - m_close_y-y, CLOSE))
			re = true;
	}

	return re;
}

int		C_VS_UI_SLAYER_PORTAL::GetNext(int map, bool bLeft)
{
	for(int i = 1; i < MAP_MAX; i++ )
	{
		int nmap;
		
		if( bLeft )
			nmap = (map-i+MAP_MAX)%MAP_MAX;
		else
			nmap = (map+i)%MAP_MAX;
		
		bool bCheck = true;
		std::vector<int>::iterator itr = m_zoneidList[nmap].begin();
		std::vector<int>::iterator enditr =m_zoneidList[nmap].end();
		
		while( itr != enditr )
		{
			if( !g_pSystemAvailableManager->ZoneFiltering( *itr ) )
			{
				bCheck = false;
				break;
			}
			itr++;
		}
		if( bCheck )
			return nmap;
		
	}
	if( bLeft )
		return (map+1)&MAP_MAX;
	
	return (map-1+MAP_MAX)%MAP_MAX;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SLAYER_PORTAL::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SLAYER_PORTAL::Run(id_t id)
{
	// �ݸ�����
	int map_max = MAP_MAX;
//	if(g_pUserInformation->IsNetmarble)
//		map_max = 4;

	C_VS_UI_EVENT_BUTTON *p_button;
	m_pC_button_group->Data(id/3, p_button);
	if(m_etc_spk.IsPixel(gpC_mouse_pointer->GetX()-p_button->x, gpC_mouse_pointer->GetY()-p_button->y, id))
	switch (id)
	{
	case LEFT:
		PlaySound(SOUND_WORLD_MAP);
		m_map = (m_map-1+map_max)%map_max;
		{
			bool bCannot = false;
			std::vector<int>::iterator itr = m_zoneidList[m_map].begin();
			std::vector<int>::iterator enditr =m_zoneidList[m_map].end();
			
			while( itr != enditr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( *itr ) )
				{
					bCannot = true;
					break;
				}
				itr++;
			}
			
			if( bCannot )
				m_map = GetNext( m_map, true );
		}
		break;

	case RIGHT:
		PlaySound(SOUND_WORLD_MAP);
		m_map = (m_map+1)%map_max;
		
		{
			bool bCannot = false;
			std::vector<int>::iterator itr = m_zoneidList[m_map].begin();
			std::vector<int>::iterator enditr =m_zoneidList[m_map].end();
			
			while( itr != enditr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( *itr ) )
				{
					bCannot = true;
					break;
				}
				itr++;
			}
			if( bCannot )
				m_map = GetNext( m_map, false);
		}
		break;

	case CLOSE:
		gpC_base->SendMessage(UI_CLOSE_SLAYER_PORTAL);
		break;
	}
}


