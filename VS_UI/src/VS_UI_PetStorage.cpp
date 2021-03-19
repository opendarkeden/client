 // VS_UI_Storage.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_PetStorage.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "VS_UI_Mouse_pointer.h"

#include "VS_UI_GameCommon.h"
#include "VS_UI_Item.h"

#include "VS_UI.h"
#include "UserOption.h"

#include "mgamestringtable.H"
#include "SystemAvailabilities.h"
static MStorage *	m_pPetStorage;
static MStorage *   m_pPersnalShop;

static int			m_pet_focused_slot;
static int			m_persnalshop_focused_slot;
static C_VS_UI_EDIT_DIALOG *	m_pC_dialog_multi_buy_confirm = NULL;
//static UINT			m_select_item_slot;
//static UINT			m_what_tab; // tab sprite id


//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
int	C_VS_UI_PETSTORAGE::GetFocusSlot()
{
	return m_Click_Slot;
}
void C_VS_UI_PETSTORAGE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();

	m_pet_focused_slot = NOT_SELECTED;
//	m_bl_money_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PETSTORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PETSTORAGE::C_VS_UI_PETSTORAGE()
{
	m_pPetStorage = NULL; // 확인용... by sigi

	g_RegisterWindow(this);

	int normal_tab_x_offset, special_tab_x_offset, mysterious_tab_x_offset;
	int close_button_x, close_button_y;
	int help_button_x, help_button_y;
	int tab_y_offset;

	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_image_spk.Open(SPK_SLAYER_SHOP_STORAGE);
		break;

	case RACE_VAMPIRE:
		m_image_spk.Open(SPK_VAMPIRE_SHOP_STORAGE);
		break;

	case RACE_OUSTERS:
		m_image_spk.Open(SPK_OUSTERS_SHOP_STORAGE);
		break;		
	}

	m_shelf_start_x_offset = 15;
	m_shelf_start_y_offset[0] = 23;
	m_shelf_start_y_offset[1] = 125;
	m_shelf_start_y_offset[2] = 227;
	m_shelf_start_y_offset[3] = 329;
	normal_tab_x_offset = 15;
	special_tab_x_offset = 119;
	mysterious_tab_x_offset = 224;
	tab_y_offset = 422;
	close_button_x = 258;
	close_button_y = 452;
	help_button_x = 193;
	help_button_y = 452;

	Set(10, 30, 332, 497);
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		w += 20;
		h += 20;
		m_shelf_start_x_offset += 10;
		m_shelf_start_y_offset[0] += 10;
		m_shelf_start_y_offset[1] += 10;
		m_shelf_start_y_offset[2] += 10;
		m_shelf_start_y_offset[3] += 10;
		normal_tab_x_offset += 10;
		special_tab_x_offset += 10;
		mysterious_tab_x_offset += 10;
		tab_y_offset += 10;
		close_button_x += 10;
		close_button_y += 10;
		help_button_x += 10;
		help_button_y += 10;
	}

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, close_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), SHOP_CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_x, help_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), SHOP_HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));
	
	m_Click_Slot = 0;
#ifndef _LIB
	// -- Test only
	MStorage*	pStorage;
	MItem*		pItem;

	//----------------------------------------------------
	// 3개의 shelf를 가진 상점
	//----------------------------------------------------
	m_pPetStorage = new MStorage;
	pStorage = m_pPetStorage;
	pStorage->Init( 1  );		

	pStorage->SetCurrent( 0 );
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	MPetItem* pPetItem;
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 0 );
	pItem->SetItemType( 1 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pPetItem = (MPetItem*)pItem;
	pPetItem->SetPetKeepedDay(100);
	if (!pStorage->SetItem( 0, pItem ))
	{
		delete pItem;
	}
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 1 );
	pItem->SetItemType( 2 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 100);	
	if (!pStorage->SetItem( 1, pItem ))
	{
		delete pItem;
	}

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 2 );
	pItem->SetItemType( 3 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 2, pItem ))
		{
		delete pItem;
		}

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 3 );
	pItem->SetItemType( 4 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 3, pItem ))
		{
		delete pItem;
		}
  	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 4 );
	pItem->SetItemType( 5 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 4, pItem ))
		{
		delete pItem;
		}


	//----------------------------------------------------
	// 돈 설정
	//----------------------------------------------------
	MMoneyManager* pMoneyManager = pStorage->GetMoneyManager();

	pMoneyManager->SetMoney( 3435 );


	//----------------------------------------------------
	// global 설정
	//----------------------------------------------------
	g_pStorage = m_pPetStorage;

	m_pPetStorage->SetCurrent(0);	

#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_PETSTORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PETSTORAGE::~C_VS_UI_PETSTORAGE()
{
	g_UnregisterWindow(this);

#ifndef _LIB
	DeleteNew(m_pPetStorage);
	g_pStorage = NULL;	// by sigi
#endif

	m_pPetStorage = NULL; // 다음 check를 위해서..

	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_PETSTORAGE::CancelPushState
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PETSTORAGE::CancelPushState()
{
	m_pC_button_group->CancelPushState();
//	m_bl_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// GetSlotItem
//
// slot에 item이 있는가?
//-----------------------------------------------------------------------------
MItem * C_VS_UI_PETSTORAGE::GetSlotItem(int slot) const
{
	if (m_pPetStorage!=NULL)	// 확인용... by sigi
	{
		return (MItem *)m_pPetStorage->GetItem( slot );
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// GetSlotX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_PETSTORAGE::GetSlotX(int slot) const
{
	if (slot != NOT_SELECTED)
	{
		return (m_shelf_start_x_offset+(slot%SLOT_X_COUNT)*SLOT_WIDTH);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// GetSlotY 
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_PETSTORAGE::GetSlotY(int slot) const
{
	if (slot != NOT_SELECTED)
	{
		return (m_shelf_start_y_offset[slot/SLOT_X_COUNT]);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// GetDialogShowX
//
// Dialog box가 출력될 곳의 x좌표를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_PETSTORAGE::GetDialogShowX() const
{
	if (m_pet_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotX(m_pet_focused_slot));
}

//-----------------------------------------------------------------------------
// GetDialogShowY
//
// Dialog box가 출력될 곳의 Y좌표를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_PETSTORAGE::GetDialogShowY() const
{
	if (m_pet_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotY(m_pet_focused_slot)+SLOT_HEIGHT);
}

int C_VS_UI_PETSTORAGE::GetEmptySlot()
{
	for (int i=0; i<STORAGE_SLOT; i++)
	{
		const MItem* p_item = m_pPetStorage->GetItem( i );
		if(NULL == p_item)
			return i;
	}
	return -1;
}
//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// SetStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::SetPetStorage(MStorage * pStorage)
{
	assert(pStorage != NULL);

	m_pPetStorage = pStorage;

	// normal을 default로 설정.. by sigi
//	m_what_tab = STORAGE_TAB1;
	
	m_pPetStorage->SetCurrent(0);	
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::Show()
{
	assert(m_pPetStorage != NULL);

	int len = 0;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x, y, w, h);
		m_image_spk.BltLocked(x+GetSlotX(0), y+GetSlotY(0), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT), y+GetSlotY(SLOT_X_COUNT), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*2), y+GetSlotY(SLOT_X_COUNT*2), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*3), y+GetSlotY(SLOT_X_COUNT*3), MAIN_WINDOW);
		
		for (int i=0; i<STORAGE_SLOT; i++)
		{
			const MItem* p_item = m_pPetStorage->GetItem( i );
			
			if (p_item != NULL)
			{
				// frame id -> sprite id
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int item_x = x+GetSlotX(i);
				
				// 바닦에 놓이게 한다.
				int item_y = y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)-4;
				
				// item을 center에 맞춘다.
				item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
				
				// identify된 아이템인 경우.. by sigi
				if (p_item->IsIdentified())
				{
					if(p_item->IsQuestItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
					else
					if(p_item->IsUniqueItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
					else
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
				}
				else
				{
					// identify 안 된 경우는 무조건 흰색
					CIndexSprite::SetUsingColorSet(378, 378);
				}
				
				
				if (m_pet_focused_slot == i)
				{
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
				
				}
				else
				{
					if (p_item->IsAffectStatus())
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
					}
				}
			}
			else
			{
				if (m_pet_focused_slot == i)
				{
					const MItem* p_mouse_item = gpC_mouse_pointer->GetPickUpItem();

					if (p_mouse_item != NULL)
					{
						// frame id -> sprite id
						TYPE_FRAMEID frame_id = p_mouse_item->GetInventoryFrameID();

						int item_x = x+GetSlotX(i);

						// 바닦에 놓이게 한다.
						int item_y = y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)-4;

						// item을 center에 맞춘다.
						item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
						
						gpC_item->BltLockedOutlineOnly(item_x, item_y, WHITE, frame_id);					
					}
				}
			}
		}
		m_pC_button_group->Show();		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	g_FL2_GetDC();

	m_pC_button_group->ShowDescription();

	g_FL2_ReleaseDC();
	
#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
/*
		for (i=0; i<STORAGE_SLOT; i++) // draw every slot rect
		{
			Rect rect(GetSlotX(i), GetSlotY(i),
						SLOT_WIDTH, SLOT_HEIGHT);

			rectangle(&surfaceinfo, &rect, WHITE);
		}
*/

		if (m_pet_focused_slot != NOT_SELECTED)
		{
			Rect focused_rect(GetSlotX(m_pet_focused_slot), GetSlotY(m_pet_focused_slot),
						SLOT_WIDTH, SLOT_HEIGHT);

//			rectangle(&surfaceinfo, &focused_rect, GREEN);
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}
#endif

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_PETSTORAGE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::Run(id_t id)
{
	switch (id)
	{
		case STORAGE_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_PETSTORAGE);
			break;

		case STORAGE_HELP_ID:
		//	gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::STORAGE);
			break;
	
	}
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char*	m_help_string[5] = {
		"",
		"",
		"",
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_STORAGE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_STORAGE_WINDOW].GetString(),
	};
	
	if (p_button->GetID() < 5)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if (p_button->GetID() == STORAGE_CLOSE_ID || p_button->GetID() == STORAGE_HELP_ID || p_button->GetID() == STORAGE_MONEY_ID)
	{
		if (p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	else if(p_button->GetID() < m_pPetStorage->GetSize())
	{
		if(p_button->GetFocusState() || m_pPetStorage != NULL && m_pPetStorage->GetCurrent() == p_button->GetID())
		{
			if(p_button->GetPressState())
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+6);
			else
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
		}
		else
			m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_PETSTORAGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x -= x; _y -= y;

	m_pC_button_group->MouseControl(message, _x, _y);

	int i;

	switch (message)
	{
		case M_MOVING:
			//
			// search shelf slot...
			//
//			m_bl_money_button_focused = false;

			for (i=0; i < STORAGE_SLOT; i++)
			{
				if (_x >= GetSlotX(i) && _x < GetSlotX(i)+SLOT_WIDTH &&
					 _y >= GetSlotY(i) && _y < GetSlotY(i)+SLOT_HEIGHT)
				{
					if(gpC_Imm && m_pet_focused_slot == NOT_SELECTED)
						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);

					m_pet_focused_slot = i;

					MItem * p_item = GetSlotItem(m_pet_focused_slot);
					if (p_item != NULL)
					{
						g_descriptor_manager.Set(DID_ITEM, x+GetSlotX(m_pet_focused_slot), y+GetSlotY(m_pet_focused_slot)+SLOT_HEIGHT-p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y, (void *)p_item);
					}
					return true;
//					break;
				}
			}

			m_pet_focused_slot = NOT_SELECTED;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (m_pet_focused_slot != NOT_SELECTED && m_pPetStorage != NULL)
			{
				if(gpC_mouse_pointer->IsCursorDescription())
				{
					if(GetSlotItem(m_pet_focused_slot))
						gC_vs_ui.RunDescDialog(DID_ITEM, (void *)GetSlotItem(m_pet_focused_slot));
				}
				else
				{
					MItem* p_item = (MItem *)m_pPetStorage->GetItem( m_pet_focused_slot );
					if(NULL != p_item)
					{
						m_Click_Slot = m_pet_focused_slot;
						gC_vs_ui.RunGetKeepPetItemDialog();
					}
//					gpC_base->SendMessage(UI_SELECT_STORAGE_SLOT, m_pPetStorage->GetCurrent(), m_pet_focused_slot);
//					// SHIFT누르고 클릭했을때 아이템 자동 이동 처리 -> 인벤토리
//					if(g_pDXInput->KeyDown(DIK_LSHIFT))
//					{
//						MItem* p_item = (MItem *)m_pPetStorage->GetItem( m_pet_focused_slot );
//						POINT point;
//						if(p_item != NULL && g_pInventory->GetFitPosition(p_item, point))
//						{
//							const MItem* p_cur_item = g_pInventory->GetItem(point.x, point.y);
//							
//							// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
//							// 위치가 완전히 일치할경우에만 추가한다.
//							if (p_item->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == point.x && p_cur_item->GetGridY() == point.y)
//							{
//								// 이전에 있던 item에 추가될 수 있는 경우
//								
//								// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
//								// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
//								gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
//									point.x, point.y,
//									(void *)p_cur_item); // 대상 Item
//							}
//							else
//							{	
//								// 추가될 수 없는 경우
//								MItem* p_old_item  = NULL;
//								
//								if (g_pInventory->CanReplaceItem(p_item,		// 추가할 item
//									point.x, point.y,	// 추가할 위치 
//									p_old_item))								// 원래있던 item
//								{
//									
//									gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
//										point.x, point.y,
//										p_item);
//									
//								}
//								else
//								{
//									// failed.
//									return false;
//								}
//							}
//							
//						}
//					}
				}
			}

			break;

		case M_LEFTBUTTON_UP:

			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	m_pet_focused_slot = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PETSTORAGE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  개인상점 
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	C_VS_UI_PERSNALSHOP::GetFocusSlot()
{
	return m_Click_Slot;
}
void C_VS_UI_PERSNALSHOP::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();

	m_pet_focused_slot = NOT_SELECTED;
//	m_bl_money_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PETSTORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PERSNALSHOP::C_VS_UI_PERSNALSHOP()
{
	m_pPersnalShop = NULL;	

	g_RegisterWindow(this);
	
	int normal_tab_x_offset, special_tab_x_offset, mysterious_tab_x_offset;
	int close_button_x, close_button_y;	
	int help_button_x, help_button_y;
	int tab_y_offset;

	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_image_spk.Open(SPK_SLAYER_SHOP_STORAGE);
		break;

	case RACE_VAMPIRE:
		m_image_spk.Open(SPK_VAMPIRE_SHOP_STORAGE);
		break;

	case RACE_OUSTERS:
		m_image_spk.Open(SPK_OUSTERS_SHOP_STORAGE);
		break;		
	}

	m_shelf_start_x_offset = 15;
	m_shelf_start_y_offset[0] = 23;
	m_shelf_start_y_offset[1] = 125;
	m_shelf_start_y_offset[2] = 227;
	m_shelf_start_y_offset[3] = 329;
	normal_tab_x_offset = 15;
	special_tab_x_offset = 119;
	mysterious_tab_x_offset = 224;
	tab_y_offset = 422;
	close_button_x = 258;
	close_button_y = 452;
	help_button_x = 193;
	help_button_y = 452;

	Set(10, 30, 332, 497);
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		w += 20;
		h += 20;
		m_shelf_start_x_offset += 10;
		m_shelf_start_y_offset[0] += 10;
		m_shelf_start_y_offset[1] += 10;
		m_shelf_start_y_offset[2] += 10;
		m_shelf_start_y_offset[3] += 10;
		normal_tab_x_offset += 10;
		special_tab_x_offset += 10;
		mysterious_tab_x_offset += 10;
		tab_y_offset += 10;
		close_button_x += 10;
		close_button_y += 10;
		help_button_x += 10;
		help_button_y += 10;
	}


	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_image_btn.Open(SPK_PERSNALSHOP_SLAYER);
		break;
		
	case RACE_VAMPIRE:
		m_image_btn.Open(SPK_PERSNALSHOP_VAMPIRE);
		break;
		
	case RACE_OUSTERS:
		m_image_btn.Open(SPK_PERSNALSHOP_OUSTERS);
		break;		
	}

	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	if(gC_vs_ui.inventory_mode != 2)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x-250, close_button_y-27, m_image_btn.GetWidth(INPUT_MESSAGE), m_image_btn.GetHeight(INPUT_MESSAGE), WRITE_MESSAGE, this, C_VS_UI_PERSNALSHOP::INPUT_MESSAGE));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x-150, close_button_y-27, m_image_btn.GetWidth(INPUT_MESSAGE), m_image_btn.GetHeight(INPUT_MESSAGE), OK_ID, this, C_VS_UI_PERSNALSHOP::SHOP_OPEN_MESSAGE));
		
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x-60, close_button_y-27, m_image_btn.GetWidth(INPUT_MESSAGE), m_image_btn.GetHeight(INPUT_MESSAGE), SHOP_CLOSE_ID, this, C_VS_UI_PERSNALSHOP::SHOP_CLOSE_MESSAGE));
	}
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, close_button_y+2, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), CANCEL_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	
	m_pC_dialog_remove_confirm = NULL;	

	m_Click_Slot = 0;
	
#ifndef _LIB
	// -- Test only
	MStorage*	pStorage;

	m_pPersnalShop = new MStorage;
	pStorage = m_pPersnalShop;
	pStorage->Init( 1  );		

	pStorage->SetCurrent( 0 );
	
//	//----------------------------------------------------
//	// Item 생성 --> 추가
//	//----------------------------------------------------
//	MPetItem* pPetItem;
//	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
//	pItem->SetID( 0 );
//	pItem->SetItemType( 1 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	pPetItem = (MPetItem*)pItem;
//	pPetItem->SetPetKeepedDay(100);
//	if (!pStorage->SetItem( 0, pItem ))
//	{
//		delete pItem;
//	}
//	
//	//----------------------------------------------------
//	// Item 생성 --> 추가
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
//	pItem->SetID( 1 );
//	pItem->SetItemType( 2 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 100);	
//	if (!pStorage->SetItem( 1, pItem ))
//	{
//		delete pItem;
//	}
//
//	//----------------------------------------------------
//	// Item 생성 --> 추가
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
//	pItem->SetID( 2 );
//	pItem->SetItemType( 3 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	if (!pStorage->SetItem( 2, pItem ))
//		{
//		delete pItem;
//		}
//
//	//----------------------------------------------------
//	// Item 생성 --> 추가
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
//	pItem->SetID( 3 );
//	pItem->SetItemType( 4 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	if (!pStorage->SetItem( 3, pItem ))
//		{
//		delete pItem;
//		}
//  	//----------------------------------------------------
//	// Item 생성 --> 추가
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
//	pItem->SetID( 4 );
//	pItem->SetItemType( 5 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	if (!pStorage->SetItem( 4, pItem ))
//		{
//		delete pItem;
//		}
//
//
//	//----------------------------------------------------
//	// 돈 설정
//	//----------------------------------------------------
//	MMoneyManager* pMoneyManager = pStorage->GetMoneyManager();
//
//	pMoneyManager->SetMoney( 3435 );


	//----------------------------------------------------
	// global 설정
	//----------------------------------------------------
	g_pStorage2 = m_pPersnalShop;


	

#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_PETSTORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PERSNALSHOP::~C_VS_UI_PERSNALSHOP()
{
	g_UnregisterWindow(this);
	gC_vs_ui.setinventory_mode(0);
	gC_vs_ui.SetPersnalShoptoUI(0);
#ifndef _LIB
	DeleteNew(m_pPersnalShop);
	g_pStorage2 = NULL;
#endif

	m_pPersnalShop = NULL;
	if(m_pC_dialog_multi_buy_confirm != NULL)
		DeleteNew(m_pC_dialog_multi_buy_confirm);
	if(m_pC_dialog_remove_confirm != NULL)
		DeleteNew(m_pC_dialog_remove_confirm);

	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_PETSTORAGE::CancelPushState
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PERSNALSHOP::CancelPushState()
{
	m_pC_button_group->CancelPushState();
//	m_bl_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// GetSlotItem
//
// slot에 item이 있는가?
//-----------------------------------------------------------------------------
MItem * C_VS_UI_PERSNALSHOP::GetSlotItem(int slot) const
{
	if (m_pPersnalShop!=NULL)	// 확인용... by sigi
	{
		return (MItem *)m_pPersnalShop->GetItem( slot );
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// GetSlotX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_PERSNALSHOP::GetSlotX(int slot) const
{
	if (slot != NOT_SELECTED)
	{
		return (m_shelf_start_x_offset+(slot%SLOT_X_COUNT)*SLOT_WIDTH);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// GetSlotY 
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_PERSNALSHOP::GetSlotY(int slot) const
{
	if (slot != NOT_SELECTED)
	{
		return (m_shelf_start_y_offset[slot/SLOT_X_COUNT]);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// GetDialogShowX
//
// Dialog box가 출력될 곳의 x좌표를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_PERSNALSHOP::GetDialogShowX() const
{
	if (m_persnalshop_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotX(m_persnalshop_focused_slot));
}

//-----------------------------------------------------------------------------
// GetDialogShowY
//
// Dialog box가 출력될 곳의 Y좌표를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_PERSNALSHOP::GetDialogShowY() const
{
	if (m_persnalshop_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotY(m_persnalshop_focused_slot)+SLOT_HEIGHT);
}

int C_VS_UI_PERSNALSHOP::GetEmptySlot()
{
	for (int i=0; i<STORAGE_SLOT; i++)
	{
		const MItem* p_item = m_pPersnalShop->GetItem( i );
		if(NULL == p_item)
			return i;
	}
	return -1;
}
//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// SetStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::SetPersnalShop(MStorage * pStorage)
{
	assert(pStorage != NULL);

	m_pPersnalShop = pStorage;

	// normal을 default로 설정.. by sigi
//	m_what_tab = STORAGE_TAB1;
	
	m_pPersnalShop->SetCurrent(0);	
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------

void C_VS_UI_PERSNALSHOP::Show()
{
	assert(m_pPersnalShop != NULL);

	RECT rect[20];
	int num[20];
	int len = 0;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x, y, w, h);
		m_image_spk.BltLocked(x+GetSlotX(0), y+GetSlotY(0), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT), y+GetSlotY(SLOT_X_COUNT), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*2), y+GetSlotY(SLOT_X_COUNT*2), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*3), y+GetSlotY(SLOT_X_COUNT*3), MAIN_WINDOW);
		
//		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_money_button_offset_x+25, y+m_money_button_offset_y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);
		m_pPersnalShop->SetCurrent(0);
		for (int i=0; i<STORAGE_SLOT; i++)
		{
			const MItem* p_item = m_pPersnalShop->GetItem( i );
			
			if (p_item != NULL)
			{
				// frame id -> sprite id
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int item_x = x+GetSlotX(i);
				
				// 바닦에 놓이게 한다.
				int item_y = y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)-4;
				
				// item을 center에 맞춘다.
				item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
				
				// identify된 아이템인 경우.. by sigi
				if (p_item->IsIdentified())
				{
					if(p_item->IsQuestItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
					else
					if(p_item->IsUniqueItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
					else
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
				}
				else
				{
					// identify 안 된 경우는 무조건 흰색
					CIndexSprite::SetUsingColorSet(378, 378);
				}
				
				
				if (m_persnalshop_focused_slot == i)
				{
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
					if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
					{
						ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
						if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
							)
						{
							gpC_global_resource->m_pC_info_spk->BltLockedOutline(x+GetSlotX(i), item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, RGB_WHITE, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
						}	
					}
				}
				else
				{
					if (p_item->IsAffectStatus())
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLocked(x+GetSlotX(i), item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
							}
						}
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLockedColor(x+GetSlotX(i), item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
							}
						}
					}
				}

				// 아이템 개수표시
				if(p_item->IsPileItem() || p_item->IsChargeItem())
				{
					RECT rt;
					rt.right = x+GetSlotX(i)+SLOT_WIDTH;
					int depth, number;
					for(depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
					if(depth == 0) depth = 1;
					rt.left = rt.right - 7*depth;
					rt.bottom = y+GetSlotY(i)+SLOT_HEIGHT;
					rt.top = rt.bottom - 12;
					
					rect[len] = rt;
					rt.left = max(0, rt.left);
					rt.top = max(0, rt.top);
					
					if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
					num[len] = p_item->GetNumber();
					len++;
				}

			}
			else
			{
				if (m_persnalshop_focused_slot == i)
				{
					const MItem* p_mouse_item = gpC_mouse_pointer->GetPickUpItem();

					if (p_mouse_item != NULL)
					{
						// frame id -> sprite id
						TYPE_FRAMEID frame_id = p_mouse_item->GetInventoryFrameID();

						int item_x = x+GetSlotX(i);

						// 바닦에 놓이게 한다.
						int item_y = y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)-4;

						// item을 center에 맞춘다.
						item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
						
						gpC_item->BltLockedOutlineOnly(item_x, item_y, WHITE, frame_id);					
					}
				}
			}
		}
		m_pC_button_group->Show();		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	char sz_temp[10];
	g_FL2_GetDC();

	COLORREF markColor = RGB(220, 220, 220);//RGB(140, 140, 255);
	for(int i = 0; i < len; i++)
	{
		wsprintf(sz_temp, "%d", num[i]);
		g_PrintColorStr(rect[i].left, rect[i].top, sz_temp, gpC_base->m_item_desc_pi, markColor);	
	}

	m_pC_button_group->ShowDescription();

	g_FL2_ReleaseDC();
	
#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
/*
		for (i=0; i<STORAGE_SLOT; i++) // draw every slot rect
		{
			Rect rect(GetSlotX(i), GetSlotY(i),
						SLOT_WIDTH, SLOT_HEIGHT);

			rectangle(&surfaceinfo, &rect, WHITE);
		}
*/

		if (m_persnalshop_focused_slot != NOT_SELECTED)
		{
			Rect focused_rect(GetSlotX(m_persnalshop_focused_slot), GetSlotY(m_persnalshop_focused_slot),
						SLOT_WIDTH, SLOT_HEIGHT);

//			rectangle(&surfaceinfo, &focused_rect, GREEN);
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}
#endif

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_PERSNALSHOP::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::Run(id_t id)
{
	switch (id)
	{
		case CANCEL_ID:
			gC_vs_ui.setinventory_mode(0);
			gpC_base->SendMessage(UI_CLOSE_PERSNALSHOP);
			/**/
			break;
		
		case OK_ID:

				gpC_base->SendMessage(UI_STORE_OPEN,0,0,NULL);
			break;
		case SHOP_CLOSE_ID:	
				gpC_base->SendMessage(UI_STORE_CLOSE);
			break;
		case WRITE_MESSAGE:
			gC_vs_ui.RunPersnalShopMessage();
			break;
	
	}
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char*	m_help_string[5] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNALSHOP_OK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNALSHOP_WRITE_MESSAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNALSHOP_CANCEL].GetString(),
		"",
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_STORAGE_WINDOW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_STORAGE_WINDOW].GetString(),
	};
	
	if (p_button->GetID() < 5)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{

	if(gC_vs_ui.inventory_mode != 2)
	{
		
		if( p_button->GetID() == WRITE_MESSAGE || p_button->GetID() == OK_ID || p_button->GetID() == SHOP_CLOSE_ID)
		{
			if (p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_image_btn.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
				else
					m_image_btn.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
			}
			else
				m_image_btn.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
			//gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}



	if ( p_button->GetID() == CANCEL_ID )
	{
		if (p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
//	else if(p_button->GetID() < m_pPersnalShop->GetSize())
//	{
//		if(p_button->GetFocusState() || m_pPersnalShop != NULL && m_pPersnalShop->GetCurrent() == p_button->GetID())
//		{
//			if(p_button->GetPressState())
//				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+6);
//			else
//				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
//		}
//		else
//			m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
//	}
}


void ExecF_sellConfirm2(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_OK:
			
			//assert(m_select_item_slot != NOT_SELECTED);

			int item_count = 1;  
			if (m_pC_dialog_multi_buy_confirm != NULL)
			{
				item_count = m_pC_dialog_multi_buy_confirm->GetValue();
			}
			 
			//gpC_base->SendMessage(UI_SELECT_PERSNALSHOP_SLOT,3, 3, NULL);
			gpC_base->SendMessage(UI_SELECT_PERSNALSHOP_SLOT, m_pPersnalShop->GetCurrent(), m_persnalshop_focused_slot);
			break;
	}

	//m_select_item_slot = NOT_SELECTED;

	// re-acquire chatting
	gC_vs_ui.AcquireChatting();
}


void C_VS_UI_PERSNALSHOP::StartBuyConfirmDialog(int _x, int _y, int num)
{
//	DeleteNew(m_pC_dialog_remove_confirm);
//
//
//	MItem *selected_item = NULL;
//	//if (m_select_item_slot != NOT_SELECTED)
//	//selected_item = GetSlotItem(m_select_item_slot);
////		selected_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
//
//    selected_item = gpC_mouse_pointer->GetPickUpItem();
//
//
//	if (selected_item != NULL)
//	{
//		// by sigi - 살려고할 때 decription이 가려저서리..
//		g_descriptor_manager.Unset();	// by sigi
//
//		if (selected_item->IsPileItem())
//		{
//			if(m_pC_dialog_multi_buy_confirm != NULL)
//				DeleteNew(m_pC_dialog_multi_buy_confirm);
//
//			m_pC_dialog_multi_buy_confirm = new C_VS_UI_EDIT_DIALOG(_x, _y, 3, 0, ExecF_sellConfirm2, DIALOG_OK|DIALOG_CANCEL, num, selected_item->GetMaxNumber());
//			m_pC_dialog_multi_buy_confirm->Start();
//		}
//		else
//		{
//			m_pC_dialog_sell_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_sellConfirm2, DIALOG_OK|DIALOG_CANCEL);
//			static char * pp_dmsg_buy_confirm[] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_SELL_CONFIRM].GetString()};
//			m_pC_dialog_sell_confirm->SetMessage(pp_dmsg_buy_confirm, 1, SMO_NOFIT);
//
//			m_pC_dialog_sell_confirm->Start();
//		}
//	}
}

void ExecF_Remove_Persnalshop(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
//	MItem * p_item = NULL;
//	MItem * p_olditem = NULL;
//	MItem * p_Newitem = NULL;
//
	switch( id )
	{
	case DIALOG_EXECID_OK :
		
			if(gC_vs_ui.inventory_mode == 2 )
			{
				gpC_base->SendMessage(UI_BUY_STORE_ITEM,  m_persnalshop_focused_slot,m_pPersnalShop->GetCurrent(),(MItem*)m_pPersnalShop->GetItem(m_persnalshop_focused_slot));
			}
			else if(gC_vs_ui.inventory_mode == 1 )
			{
				if(g_pStorage2 != NULL)
				{
					g_pStorage2->SetCurrent(0);
					g_pInventory->SetBegin();
					while (g_pInventory->IsNotEnd())
					{
						MItem * p_olditem = g_pInventory->Get();
						MItem * p_item = (MItem*)m_pPersnalShop->GetItem(m_persnalshop_focused_slot );
						if(p_olditem != NULL && p_item!=NULL )
						{
							if(p_olditem->GetID() == p_item->GetID())
							{
								//p_olditem = g_pInventory->GetItem(g_pInventory->(),g_pInventory->GetGridY());
								MItem * p_Newitem = p_olditem;
								p_Newitem->SetPersnal(false);
								g_pInventory->ReplaceItem(p_Newitem,p_olditem->GetGridX(),p_olditem->GetGridY(),p_olditem);
								break;
							}
						}
						g_pInventory->Next();
					}
				}
				gpC_base->SendMessage(UI_UNDISPLAY_ITEM,  m_persnalshop_focused_slot,m_pPersnalShop->GetCurrent(),(MItem*)m_pPersnalShop->GetItem(m_persnalshop_focused_slot));
				
			}
	
			

		break;
	case DIALOG_CANCEL:

		break;
		
	}
}


//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_PERSNALSHOP::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x -= x; _y -= y;

	m_pC_button_group->MouseControl(message, _x, _y);

	int i;

	switch (message)
	{
		case M_MOVING:
			//
			// search shelf slot...
			//
//			m_bl_money_button_focused = false;

			for (i=0; i < STORAGE_SLOT; i++)
			{
				if (_x >= GetSlotX(i) && _x < GetSlotX(i)+SLOT_WIDTH &&
					 _y >= GetSlotY(i) && _y < GetSlotY(i)+SLOT_HEIGHT)
				{
					if(gpC_Imm && m_persnalshop_focused_slot == NOT_SELECTED)
						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);

					m_persnalshop_focused_slot = i;

					MItem * p_item = GetSlotItem(m_persnalshop_focused_slot);
					if (p_item != NULL)
					{ 

							g_descriptor_manager.Set(DID_ITEM, x+GetSlotX(m_persnalshop_focused_slot), y+GetSlotY(m_persnalshop_focused_slot)+SLOT_HEIGHT-p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y, (void *)p_item);

					}
					return true;
//					break;
				}
			}
			
			m_persnalshop_focused_slot = NOT_SELECTED;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:

			
			
			if(gC_vs_ui.inventory_mode == 2 && m_pPersnalShop != NULL)
			{
				MItem* p_item = (MItem *)m_pPersnalShop->GetItem( m_persnalshop_focused_slot );
				if(p_item != NULL)
				{
					
					DeleteNew( m_pC_dialog_remove_confirm );
					m_pC_dialog_remove_confirm = new C_VS_UI_DIALOG(_x,_y, 2, 0, ExecF_Remove_Persnalshop, DIALOG_OK|DIALOG_CANCEL);
					char * pp_dmsg[1] = 
					{
						(*g_pGameStringTable)[UI_STRING_MESSAGE_BUY_ITEM].GetString(),
					};
					m_pC_dialog_remove_confirm->SetMessage( pp_dmsg, 1, SMO_NOFIT );
					m_pC_dialog_remove_confirm->Start();
				}
			}
			break;

		case M_RIGHTBUTTON_DOWN:

			if (m_persnalshop_focused_slot != NOT_SELECTED && m_pPersnalShop != NULL && gC_vs_ui.inventory_mode != 2)
			{
				MItem* p_item = (MItem *)m_pPersnalShop->GetItem( m_persnalshop_focused_slot );
				if(p_item != NULL)
				{
					DeleteNew( m_pC_dialog_remove_confirm );
					m_pC_dialog_remove_confirm = new C_VS_UI_DIALOG(_x,_y, 2, 0, ExecF_Remove_Persnalshop, DIALOG_OK|DIALOG_CANCEL);
					char * pp_dmsg[1] = {
						(*g_pGameStringTable)[UI_STRING_MESSAGE_QUESTION_REMOVE_OPTION].GetString(),
					};
					m_pC_dialog_remove_confirm->SetMessage( pp_dmsg, 1, SMO_NOFIT );
					m_pC_dialog_remove_confirm->Start();
				}

			} 
			break;

	}

	return true;
}


//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	m_pet_focused_slot = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PERSNALSHOP::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
	gC_vs_ui.setinventory_mode(0);
}

