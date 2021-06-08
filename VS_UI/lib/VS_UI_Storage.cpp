// VS_UI_Storage.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_Storage.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "VS_UI_Mouse_pointer.h"

#include "VS_UI_GameCommon.h"
#include "VS_UI_Item.h"

#include "VS_UI.h"
#include "UserOption.h"

#include "mgamestringtable.H"
static MStorage *	m_pStorage;
static int			m_focused_slot;
//static UINT			m_select_item_slot;
//static UINT			m_what_tab; // tab sprite id


static C_VS_UI_MONEY_DIALOG *	m_pC_dialog_withdraw_money;		// by sigi

// by sigi
void ExecF_WithdrawMoney(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_OK:
			gpC_base->SendMessage(UI_WITHDRAW_MONEY, m_pC_dialog_withdraw_money->GetValue());
		break;
	}

	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();

	m_focused_slot = NOT_SELECTED;
//	m_bl_money_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_STORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_STORAGE::C_VS_UI_STORAGE()
{
	m_pStorage = NULL; // 확인용... by sigi

	g_RegisterWindow(this);

	m_pC_dialog_withdraw_money = NULL;	// by sigi

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
	m_money_button_offset_x = 15;
	m_money_button_offset_y = 457;

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
		m_money_button_offset_x += 10;
		m_money_button_offset_y += 8;
	}

	// set button
	m_pC_button_group = new ButtonGroup(this);

	C_VS_UI_EVENT_BUTTON * p_tab = new C_VS_UI_EVENT_BUTTON(normal_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, STORAGE_TAB1_ID, this, STORAGE_TAB1);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	p_tab = new C_VS_UI_EVENT_BUTTON(special_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, STORAGE_TAB2_ID, this, STORAGE_TAB2);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	p_tab = new C_VS_UI_EVENT_BUTTON(mysterious_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, STORAGE_TAB3_ID, this, STORAGE_TAB3);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_money_button_offset_x, m_money_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), STORAGE_MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_MONEY));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, close_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), SHOP_CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_x, help_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), SHOP_HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));
	

#ifndef _LIB
	// -- Test only
	MStorage*	pStorage;
	MItem*		pItem;

	//----------------------------------------------------
	// 3개의 shelf를 가진 상점
	//----------------------------------------------------
	m_pStorage = new MStorage;
	pStorage = m_pStorage;
	pStorage->Init( /*rand()%3*/ 2  );		

	pStorage->SetCurrent( 0 );
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_POTION );
	pItem->SetID( 0 );
	pItem->SetItemType( 0 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetNumber(10);
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 0, pItem ))
	{
		delete pItem;
	}

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 1 );
	pItem->SetItemType( 1 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 1, pItem ))
	{
		delete pItem;
	}
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 2 );
	pItem->SetItemType( 2 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 2, pItem ))
	{
		delete pItem;
	}

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 3 );
	pItem->SetItemType( 3 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 3, pItem ))
		{
		delete pItem;
		}

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 4 );
	pItem->SetItemType( 4 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	if (!pStorage->SetItem( 4, pItem ))
		{
		delete pItem;
		}
  

	//----------------------------------------------------
	//
	// [1] Special Shelf
	//
	//----------------------------------------------------
	if (pStorage->GetSize()>=2)
	{
		pStorage->SetCurrent( 1 );

		//----------------------------------------------------
		// Item 생성 --> 추가
		//----------------------------------------------------
		pItem = MItem::NewItem( ITEM_CLASS_SWORD );
		pItem->SetID( 100 );
		pItem->SetItemType( 0 );
		pItem->AddItemOption( 1 );
		pItem->SetCurrentDurability( 1 );	
		if (!pStorage->SetItem( 0, pItem ))
			{
			delete pItem;
			}

		//----------------------------------------------------
		// Item 생성 --> 추가
		//----------------------------------------------------
		pItem = MItem::NewItem( ITEM_CLASS_SWORD );
		pItem->SetID( 101 );
		pItem->SetItemType( 1 );
		pItem->AddItemOption( 3 );
		pItem->SetCurrentDurability( 1 );	
		if (!pStorage->SetItem( 1, pItem ))
			{
			delete pItem;
			}
	}


	//----------------------------------------------------
	// [2] Unknown Shelf
	//----------------------------------------------------
	if (pStorage->GetSize()>=3)
	{
		pStorage->SetCurrent( 2 );
		
		//----------------------------------------------------
		// Item 생성 --> 추가
		//----------------------------------------------------
		pItem = MItem::NewItem( ITEM_CLASS_BLADE );
		pItem->SetID( 202 );
		pItem->SetItemType( 0 );
		pItem->AddItemOption( 4 );
		pItem->SetCurrentDurability( 1 );	
		pItem->UnSetIdentified();
		if (!pStorage->SetItem( 0, pItem ))
			{
			delete pItem;
			}

		//----------------------------------------------------
		// Item 생성 --> 추가
		//----------------------------------------------------
		pItem = MItem::NewItem( ITEM_CLASS_BLADE );
		pItem->SetID( 205 );
		pItem->SetItemType( 1 );
		pItem->AddItemOption( 2 );
		pItem->SetCurrentDurability( 1 );
		pItem->UnSetIdentified();
		if (!pStorage->SetItem( 1, pItem ))
			{
			delete pItem;
			}
	}

	//----------------------------------------------------
	// 돈 설정
	//----------------------------------------------------
	MMoneyManager* pMoneyManager = pStorage->GetMoneyManager();

	pMoneyManager->SetMoney( 3435 );


	//----------------------------------------------------
	// global 설정
	//----------------------------------------------------
	g_pStorage = m_pStorage;

	m_pStorage->SetCurrent(0);	

#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_STORAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_STORAGE::~C_VS_UI_STORAGE()
{
	g_UnregisterWindow(this);

#ifndef _LIB
	DeleteNew(m_pStorage);
	g_pStorage = NULL;	// by sigi
#endif

	m_pStorage = NULL; // 다음 check를 위해서..

	DeleteNew(m_pC_dialog_withdraw_money);// by sigi

	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_STORAGE::CancelPushState
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STORAGE::CancelPushState()
{
	m_pC_button_group->CancelPushState();
//	m_bl_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// GetSlotItem
//
// slot에 item이 있는가?
//-----------------------------------------------------------------------------
MItem * C_VS_UI_STORAGE::GetSlotItem(int slot) const
{
	if (m_pStorage!=NULL)	// 확인용... by sigi
	{
		return (MItem *)m_pStorage->GetItem( slot );
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// GetSlotX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_STORAGE::GetSlotX(int slot) const
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
int C_VS_UI_STORAGE::GetSlotY(int slot) const
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
int C_VS_UI_STORAGE::GetDialogShowX() const
{
	if (m_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotX(m_focused_slot));
}

//-----------------------------------------------------------------------------
// GetDialogShowY
//
// Dialog box가 출력될 곳의 Y좌표를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_STORAGE::GetDialogShowY() const
{
	if (m_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotY(m_focused_slot)+SLOT_HEIGHT);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// SetStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::SetStorage(MStorage * pStorage)
{
	assert(pStorage != NULL);

	m_pStorage = pStorage;

	// normal을 default로 설정.. by sigi
//	m_what_tab = STORAGE_TAB1;
	
	m_pStorage->SetCurrent(0);	
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::Show()
{
	assert(m_pStorage != NULL);

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
		
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_money_button_offset_x+25, y+m_money_button_offset_y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);

		for (int i=0; i<STORAGE_SLOT; i++)
		{
			const MItem* p_item = m_pStorage->GetItem( i );
			
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
				
				
				if (m_focused_slot == i)
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
					for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
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
				if (m_focused_slot == i)
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

	// 보관된 액수를 출력한다.
	// 2004, 12, 14, sobeit modify start
	if(NULL != m_pStorage && NULL != m_pStorage->GetMoneyManager())
	{
		if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
		{
			std::string sstr = g_GetStringByMoney(m_pStorage->GetMoneyManager()->GetMoney());
			g_Print(x+m_money_button_offset_x+147, y+m_money_button_offset_y+4, sstr.c_str(), &gpC_base->m_money2_pi);
		}
		else
		{
			char money_buf[512];
			wsprintf(money_buf, "%d", m_pStorage->GetMoneyManager()->GetMoney());
			std::string sstr = money_buf;
			for(i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(money_buf, "%s", sstr.c_str());
			g_Print(x+m_money_button_offset_x+147, y+m_money_button_offset_y+4, money_buf, &gpC_base->m_money2_pi);
		}
	}
	
	// 2004, 12, 14, sobeit modify end
		
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

		if (m_focused_slot != NOT_SELECTED)
		{
			Rect focused_rect(GetSlotX(m_focused_slot), GetSlotY(m_focused_slot),
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
void C_VS_UI_STORAGE::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_STORAGE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::Run(id_t id)
{
	switch (id)
	{
		case STORAGE_TAB1_ID:
// 			m_what_tab = STORAGE_TAB1_ID;

			if (m_pStorage!=NULL)	// 확인용 by sigi
			{
				m_pStorage->SetCurrent(0);
			}
			break;

		case STORAGE_TAB2_ID:
// 			m_what_tab = STORAGE_TAB2_ID;

			if (m_pStorage!=NULL && m_pStorage->GetSize()>=2)	// 확인용 by sigi
			{
				m_pStorage->SetCurrent(1);
			}
			break;

		case STORAGE_TAB3_ID:
// 			m_what_tab = STORAGE_TAB3_ID; 

			if (m_pStorage!=NULL && m_pStorage->GetSize()>=3)	// 확인용 by sigi
			{
				m_pStorage->SetCurrent(2);
			}
			break;

		case STORAGE_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_STORAGE);
			break;

		case STORAGE_HELP_ID:
			gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::STORAGE);
			break;
			
		case STORAGE_MONEY_ID:
			DeleteNew(m_pC_dialog_withdraw_money);
			if (m_pStorage->GetMoneyManager()->GetMoney() > 0)
			{
				m_pC_dialog_withdraw_money = new C_VS_UI_MONEY_DIALOG(x+m_money_button_offset_x, y+m_money_button_offset_y-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_MONEY_BAR), 1, 0, ExecF_WithdrawMoney, DIALOG_OK|DIALOG_CANCEL, 10,
					C_VS_UI_MONEY_DIALOG::MONEY_WITHDRAW);	// by sigi
				m_pC_dialog_withdraw_money->Start();
			}
			break;


	}
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char*	m_help_string[6] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_STORAGE_FIRST].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_STORAGE_SECOND].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_STORAGE_THIRD].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_STORAGE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_STORAGE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_BRING_MONEY_FROM_STORAGE].GetString(),
	};
	
	if (p_button->GetID() < m_pStorage->GetSize())
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
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
	else if(p_button->GetID() < m_pStorage->GetSize())
	{
		if(p_button->GetFocusState() || m_pStorage != NULL && m_pStorage->GetCurrent() == p_button->GetID())
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
bool C_VS_UI_STORAGE::MouseControl(UINT message, int _x, int _y)
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
					if(gpC_Imm && m_focused_slot == NOT_SELECTED)
						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);

					m_focused_slot = i;

					MItem * p_item = GetSlotItem(m_focused_slot);
					if (p_item != NULL)
					{
							g_descriptor_manager.Set(DID_ITEM, x+GetSlotX(m_focused_slot), y+GetSlotY(m_focused_slot)+SLOT_HEIGHT-p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y, (void *)p_item);
						 
					}
					return true;
//					break;
				}
			}

			m_focused_slot = NOT_SELECTED;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			//
			// Item을 선택한다.
			//
			if (m_focused_slot != NOT_SELECTED && m_pStorage != NULL)
			{
				//if (GetSlotItem(m_focused_slot) != NULL)
				//{
				//	if (gbl_item_trade_lock == false)
				//	{
				//		m_select_item_slot = m_focused_slot;
				//	}
				//}
				if(gpC_mouse_pointer->IsCursorDescription())
				{
					if(GetSlotItem(m_focused_slot))
						gC_vs_ui.RunDescDialog(DID_ITEM, (void *)GetSlotItem(m_focused_slot));
				}
				else
				{
					gpC_base->SendMessage(UI_SELECT_STORAGE_SLOT, m_pStorage->GetCurrent(), m_focused_slot);
					// SHIFT누르고 클릭했을때 아이템 자동 이동 처리 -> 인벤토리
					if(g_pDXInput->KeyDown(DIK_LSHIFT))
					{
						MItem* p_item = (MItem *)m_pStorage->GetItem( m_focused_slot );
						POINT point;
						if(p_item != NULL && g_pInventory->GetFitPosition(p_item, point))
						{
							const MItem* p_cur_item = g_pInventory->GetItem(point.x, point.y);
							
							// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
							// 위치가 완전히 일치할경우에만 추가한다.
							if (p_item->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == point.x && p_cur_item->GetGridY() == point.y)
							{
								// 이전에 있던 item에 추가될 수 있는 경우
								
								// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
								// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
								gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
									point.x, point.y,
									(void *)p_cur_item); // 대상 Item
							}
							else
							{	
								// 추가될 수 없는 경우
								MItem* p_old_item  = NULL;
								
								if (g_pInventory->CanReplaceItem(p_item,		// 추가할 item
									point.x, point.y,	// 추가할 위치 
									p_old_item))								// 원래있던 item
								{
										
									gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
										point.x, point.y,
										p_item);
									
								}
								else
								{
									// failed.
									return false;
								}
							}
							
						}
					}
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
void C_VS_UI_STORAGE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	// 원래는 g_pStorage에서 읽는거다.
//	m_what_tab = STORAGE_TAB1_ID;

	m_focused_slot = NOT_SELECTED;
	//m_select_item_slot = NOT_SELECTED;
//	m_bl_money_button_focused = false;
//	m_bl_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_STORAGE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}
