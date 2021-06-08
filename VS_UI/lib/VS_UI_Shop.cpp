// VS_UI_Shop.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_Shop.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "VS_UI_Mouse_pointer.h"

#include "VS_UI_GameCommon.h"
#include "VS_UI_Item.h"

#include "VS_UI.h"

#include "MGameStringTABLE.h"
#include "SystemAvailabilities.h"

// dialog exec func에서 사용하므로 여기다..
static MShop *		m_pShop;
static int			m_focused_slot;
static UINT			m_select_item_slot;
static C_VS_UI_EDIT_DIALOG *	m_pC_dialog_multi_buy_confirm = NULL;
static UINT			m_what_tab; // tab sprite id


bool					C_VS_UI_SHOP::m_bl_mysterious_tab;
MShop::SHOP_TYPE		C_VS_UI_SHOP::m_shop_type;

//-----------------------------------------------------------------------------
// Exec functions
//-----------------------------------------------------------------------------
void ExecF_BuyConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_OK:
			//
			// 살 개수를 함께 넘겨준다. (Normal을 제외한 것은 한 개씩만 있다)
			//
			assert(m_select_item_slot != NOT_SELECTED);

			int item_count = 1;
			if (m_pC_dialog_multi_buy_confirm != NULL)
			{
				item_count = m_pC_dialog_multi_buy_confirm->GetValue();
			}
			
			gpC_base->SendMessage(UI_BUY_ITEM, m_select_item_slot, item_count, m_pShop);
			break;
	}

	m_select_item_slot = NOT_SELECTED;

	// re-acquire chatting
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();

	m_focused_slot = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SHOP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SHOP::C_VS_UI_SHOP()
{
	m_pShop = NULL; // 확인용... by sigi

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
	
	m_question_spk.Open(SPK_QUESTION);

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

	C_VS_UI_EVENT_BUTTON * p_tab = new C_VS_UI_EVENT_BUTTON(normal_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, NORMAL_TAB_ID, this, NORMAL_TAB);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	p_tab = new C_VS_UI_EVENT_BUTTON(special_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, SPECIAL_TAB_ID, this, SPECIAL_TAB);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	p_tab = new C_VS_UI_EVENT_BUTTON(mysterious_tab_x_offset, tab_y_offset, TAB_WIDTH, TAB_HEIGHT, MYSTERIOUS_TAB_ID, this, MYSTERIOUS_TAB);
	p_tab->SetClickOption(Button::RUN_WHEN_PUSH);
	m_pC_button_group->Add(p_tab);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, close_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), SHOP_CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_x, help_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), SHOP_HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));

	m_pC_dialog_buy_confirm = NULL;
	m_pC_dialog_multi_buy_confirm = NULL;

#ifndef _LIB
	// -- Test only
	MShop*		pShop;
	MShopShelf*	pShelf;
	MItem*		pItem;

	//----------------------------------------------------
	// 3개의 shelf를 가진 상점
	//----------------------------------------------------
	m_pShop = new MShop;
	pShop = m_pShop;
	pShop->Init( 3 );	
//	pShop->SetShopType(MShop::SHOP_EVENT_STAR);
//	m_shop_type = pShop->GetShopType();
	//----------------------------------------------------
	//
	// [0] Fixed Shelf
	//
	//----------------------------------------------------
	pShelf = new MShopFixedShelf;
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 0 );
	pItem->SetItemType( 0 );
	//pItem->AddItemOption( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 0, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_MAGAZINE );
	pItem->SetID( 1 );
	pItem->SetItemType( 1 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 1, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_POTION );
	pItem->SetID( 2 );
	pItem->SetItemType( 2 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 2, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 3 );
	pItem->SetItemType( 3 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 3, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 4 );
	pItem->SetItemType( 4 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 4, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_POTION );
	pItem->SetID( 5 );
	pItem->SetItemType( 5 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 5, pItem );

	if (!pShop->SetShelf(0, pShelf))
	{
		// 추가가 안 된 경우는 외부에서 delete해줘야 한다.
		// shelf 번호가 상점 전체 개수보다 큰 경우에 추가 실패당..
		delete pShelf;
	}
  

	//----------------------------------------------------
	//
	// [1] Special Shelf
	//
	//----------------------------------------------------
	pShelf = new MShopSpecialShelf;
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 100 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 1 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 0, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 101 );
	pItem->SetItemType( 1 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 1, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 102 );
	pItem->SetItemType( 2 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 2, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 103 );
	pItem->SetItemType( 3 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 3, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 104 );
	pItem->SetItemType( 4 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 4, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 105 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 5, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 106 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 6, pItem );

	if (!pShop->SetShelf(1, pShelf))
	{
		delete pShelf;
	}


	//----------------------------------------------------
	// [2] Unknown Shelf
	//----------------------------------------------------
	pShelf = new MShopUnknownShelf;
	
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_BRACELET );
	pItem->SetID( 100 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 1 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 0, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pItem->SetID( 101 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 1, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_RING );
	pItem->SetID( 102 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 2, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_BLADE );
	pItem->SetID( 103 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 3, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_COAT );
	pItem->SetID( 104 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 4, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_TROUSER );
	pItem->SetID( 105 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pItem->UnSetIdentified();
	pShelf->SetItem( 5, pItem );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_SHIELD );
	pItem->SetID( 106 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 6, pItem );
	pItem->UnSetIdentified();
	pShelf->SetEnable();

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_BELT );
	pItem->SetID( 107 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 1 );	
	pShelf->SetItem( 6, pItem );
	pItem->UnSetIdentified();
	pShelf->SetEnable();

	if (!pShop->SetShelf(2, pShelf))
	{
		delete pShelf;
	}
#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_SHOP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SHOP::~C_VS_UI_SHOP()
{
	g_UnregisterWindow(this);
	
#ifndef _LIB
	DeleteNew(m_pShop);
#endif

	m_pShop = NULL; // 다음 check를 위해서..

	if(m_pC_dialog_multi_buy_confirm != NULL)
		DeleteNew(m_pC_dialog_multi_buy_confirm);
	if(m_pC_dialog_buy_confirm != NULL)
		DeleteNew(m_pC_dialog_buy_confirm);
	if(m_pC_button_group != NULL)
		DeleteNew(m_pC_button_group);

	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SHOP::CancelPushState
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SHOP::CancelPushState()
{
	m_pC_button_group->CancelPushState();
}

//-----------------------------------------------------------------------------
// GetSlotItem
//
// slot에 item이 있는가?
//-----------------------------------------------------------------------------
MItem * C_VS_UI_SHOP::GetSlotItem(int slot) const
{
	if (m_pShop!=NULL)	// 확인용... by sigi
	{
		MShopShelf* pShopShelf = m_pShop->GetCurrentShelf();

		if (pShopShelf != NULL)
		{
			MItem* pItem = pShopShelf->GetItem(	slot );

			return pItem;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// GetSlotX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_SHOP::GetSlotX(int slot) const
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
int C_VS_UI_SHOP::GetSlotY(int slot) const
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
int C_VS_UI_SHOP::GetDialogShowX() const
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
int C_VS_UI_SHOP::GetDialogShowY() const
{
	if (m_focused_slot == NOT_SELECTED)
		return 0;

	return (GetSlotY(m_focused_slot)+SLOT_HEIGHT);
}

//-----------------------------------------------------------------------------
// StartBuyConfirmDialog
//
// dialog 내부에서 좌표를 변경할 수 없기도하고...
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::StartBuyConfirmDialog(int _x, int _y, int num)
{
	DeleteNew(m_pC_dialog_buy_confirm);
	DeleteNew(m_pC_dialog_multi_buy_confirm);

	MItem *selected_item = NULL;
	if (m_select_item_slot != NOT_SELECTED)
		selected_item = GetSlotItem(m_select_item_slot);

	if (selected_item != NULL)
	{
		// by sigi - 살려고할 때 decription이 가려저서리..
		g_descriptor_manager.Unset();	// by sigi

		if (selected_item->IsPileItem())
		{
			if(m_pC_dialog_multi_buy_confirm != NULL)
				DeleteNew(m_pC_dialog_multi_buy_confirm);

			m_pC_dialog_multi_buy_confirm = new C_VS_UI_EDIT_DIALOG(_x, _y, 3, 0, ExecF_BuyConfirm, DIALOG_OK|DIALOG_CANCEL, num, selected_item->GetMaxNumber());
			m_pC_dialog_multi_buy_confirm->Start();
		}
		else
		{
			m_pC_dialog_buy_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_BuyConfirm, DIALOG_OK|DIALOG_CANCEL);
			static char * pp_dmsg_buy_confirm[] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_BUY_ITEM].GetString()};
			m_pC_dialog_buy_confirm->SetMessage(pp_dmsg_buy_confirm, 1, SMO_NOFIT);

			m_pC_dialog_buy_confirm->Start();
		}
	}
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// SetShop
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::SetShop(MShop* pShop)
{
	assert(pShop != NULL);

	m_pShop = pShop;

	// normal을 default로 설정.. by sigi
	m_bl_mysterious_tab = false;
	m_shop_type = pShop->GetShopType();

	m_what_tab = NORMAL_TAB_ID;	
	m_pShop->SetCurrent(MShopShelf::SHELF_FIXED);	

	if(!m_pShop->GetShelf(NORMAL_TAB_ID)->IsEnable())
	{
		if(!m_pShop->GetShelf(SPECIAL_TAB_ID)->IsEnable())
			Run(MYSTERIOUS_TAB_ID);
		else
			Run(SPECIAL_TAB_ID);
	}

	if(m_shop_type == MShop::SHOP_EVENT_STAR)
		Run(SPECIAL_TAB_ID);

}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::Show()
{
	assert(m_pShop != NULL);

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x, y, w, h);
		m_image_spk.BltLocked(x+GetSlotX(0), y+GetSlotY(0), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT), y+GetSlotY(SLOT_X_COUNT), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*2), y+GetSlotY(SLOT_X_COUNT*2), MAIN_WINDOW);
		m_image_spk.BltLocked(x+GetSlotX(SLOT_X_COUNT*3), y+GetSlotY(SLOT_X_COUNT*3), MAIN_WINDOW);

		//
		// Shop의 Item을 출력시킨다.
		//
		// 현재 사용중인 shelf
		//
		MShopShelf* pShopShelf = m_pShop->GetCurrentShelf();
		
		if (pShopShelf!=NULL)	// 확인.. by sigi
		{
			for (int i=0; i<SHOP_SHELF_SLOT; i++)
			{
				MItem* p_item = pShopShelf->GetItem( i );
				
				if (p_item != NULL)
				{
					if( p_item->GetItemClass() == ITEM_CLASS_EVENT_STAR &&
						!g_pSystemAvailableManager->IsAvailableEnchantSystem() )
						continue;

					// frame id -> sprite id
					TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
					
					int item_x = x+GetSlotX(i);
					
					// 바닦에 놓이게 한다.
					int item_y = y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)-4;
					
					// item을 center에 맞춘다.
					item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
					
					if(m_bl_mysterious_tab)
					{
						if (m_focused_slot == i)
							gpC_item->BltLockedOutlineOnly(item_x, item_y, WHITE, frame_id);
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, 392);
						static int sprite_id = 0;
						static int sprite_plus = 1;
						if(Timer())
						{
							sprite_id += sprite_plus;
							if(sprite_id == 3 || sprite_id == 0)sprite_plus *=-1;
						}

						m_question_spk.BltLocked(x+GetSlotX(i)+SLOT_WIDTH/2-m_question_spk.GetWidth(sprite_id)/2, y+GetSlotY(i)+SLOT_HEIGHT-gpC_item->GetHeight(frame_id)/2+m_question_spk.GetHeight(0)/2-m_question_spk.GetHeight(sprite_id)-4, sprite_id);
					}
					else
					{
						// identify된 아이템인 경우.. by sigi
						if (p_item->IsIdentified())
						{
							if(p_item->IsQuestItem())
								CIndexSprite::SetUsingColorSet(p_item->GetQuestItemColorset() ,0 );
							else
							if(p_item->IsUniqueItem())
								CIndexSprite::SetUsingColorSet(p_item->GetUniqueItemColorset(), 0);							
							else								
								CIndexSprite::SetUsingColorSet(p_item->GetItemOptionColorSet(), 0);
						}
						else
						{
							// identify 안 된 경우는 무조건 흰색
//							CIndexSprite::SetUsingColorSet(378, 378);
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
					}
				}
			}
			
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	m_pC_button_group->ShowDescription();


#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
/*
		for (i=0; i<SHOP_SHELF_SLOT; i++) // draw every slot rect
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
void C_VS_UI_SHOP::WindowEventReceiver(id_t event)
{

}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SHOP::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::Run(id_t id)
{
	switch (id)
	{
		case NORMAL_TAB_ID:
			if (!m_pShop->GetShelf(NORMAL_TAB_ID)->IsEnable())
				break;
			m_bl_mysterious_tab = false;
			m_what_tab = NORMAL_TAB_ID;
			if (m_pShop!=NULL)	// 확인용 by sigi
			{
				m_pShop->SetCurrent(MShopShelf::SHELF_FIXED);
			}
			break;

		case SPECIAL_TAB_ID:
			if (!m_pShop->GetShelf(SPECIAL_TAB_ID)->IsEnable())
				break;
			m_bl_mysterious_tab = false;
			m_what_tab = SPECIAL_TAB_ID;
			if (m_pShop!=NULL)	// 확인용 by sigi
			{
				m_pShop->SetCurrent(MShopShelf::SHELF_SPECIAL);
			}
			break;

		case MYSTERIOUS_TAB_ID:
			if (!m_pShop->GetShelf(MYSTERIOUS_TAB_ID)->IsEnable() ||
				!g_pSystemAvailableManager->IsAvailableGambleSystem() )
				break;

			m_bl_mysterious_tab = true;
			m_what_tab = MYSTERIOUS_TAB_ID;	
			if (m_pShop!=NULL)	// 확인용 by sigi
			{
				m_pShop->SetCurrent(MShopShelf::SHELF_UNKNOWN);
			}
			break;

		case SHOP_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_SHOP);
			break;

		case SHOP_HELP_ID:
			gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::SHOP);
			break;
	}
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char*	m_help_string[5] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_TYPE_NORMAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_TYPE_SPECIAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_TYPE_MISTERIOUS].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_SHOP_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_SHOP_WINDOW].GetString(),
	};

	if (p_button->GetID() == MYSTERIOUS_TAB_ID && (!m_pShop->GetShelf(MYSTERIOUS_TAB_ID)->IsEnable()
		|| !g_pSystemAvailableManager->IsAvailableGambleSystem() ) ||
		p_button->GetID() == NORMAL_TAB_ID && !m_pShop->GetShelf(NORMAL_TAB_ID)->IsEnable() ||
		p_button->GetID() == SPECIAL_TAB_ID && !m_pShop->GetShelf(SPECIAL_TAB_ID)->IsEnable())
		return;

	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	if (p_button->GetID() == MYSTERIOUS_TAB_ID && (!m_pShop->GetShelf(MYSTERIOUS_TAB_ID)->IsEnable()
		|| !g_pSystemAvailableManager->IsAvailableGambleSystem() ) ||
		p_button->GetID() == NORMAL_TAB_ID && !m_pShop->GetShelf(NORMAL_TAB_ID)->IsEnable() ||
		p_button->GetID() == SPECIAL_TAB_ID && !m_pShop->GetShelf(SPECIAL_TAB_ID)->IsEnable())
		return;
	
	if (p_button->GetID() == SHOP_CLOSE_ID || p_button->GetID() == SHOP_HELP_ID)
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
	else
	{
		if(p_button->GetFocusState() || m_what_tab == p_button->GetID())
		{
			if(p_button->GetPressState())
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+6);
			else
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
		}
		else
			m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}

	/*
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		Rect rect(p_button->x, p_button->y, p_button->w, p_button->h);

		FillRect(&surfaceinfo, &rect, WHITE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}*/
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SHOP::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	m_pC_button_group->MouseControl(message, _x, _y);

	int i;

	switch (message)
	{
		case M_MOVING:
			//
			// search shelf slot...
			//
			for (i=0; i < SHOP_SHELF_SLOT; i++)
			{
				if (_x >= GetSlotX(i) && _x < GetSlotX(i)+SLOT_WIDTH &&
					 _y >= GetSlotY(i) && _y < GetSlotY(i)+SLOT_HEIGHT)
				{
					if(gpC_Imm && m_focused_slot == NOT_SELECTED)
						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);

					m_focused_slot = i;

					// by sigi - 살려고할 때 decription이 가려저서리.. 근데 딴 방법이 있지 않나?? 싶은데 - -;
					if (m_pC_dialog_multi_buy_confirm==NULL && m_pC_dialog_buy_confirm==NULL
						|| m_pC_dialog_multi_buy_confirm!=NULL && !m_pC_dialog_multi_buy_confirm->Running()
						|| m_pC_dialog_buy_confirm!=NULL && !m_pC_dialog_buy_confirm->Running()) 
					{						
						MItem * p_item = GetSlotItem(m_focused_slot);

						if (p_item != NULL)
						{
							if( p_item->GetItemClass() == ITEM_CLASS_EVENT_STAR &&
								g_pSystemAvailableManager->IsAvailableEnchantSystem() ||
								p_item->GetItemClass() != ITEM_CLASS_EVENT_STAR )
								g_descriptor_manager.Set(DID_ITEM, x+GetSlotX(m_focused_slot), y+GetSlotY(m_focused_slot)+SLOT_HEIGHT-p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y, (void *)p_item, true);
						}
					}

					return true;
				}
			}
			m_focused_slot = NOT_SELECTED;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			//
			// Item을 선택한다.
			//
			if (m_focused_slot != NOT_SELECTED)
			{
				MItem *p_item = GetSlotItem(m_focused_slot);
				if (p_item != NULL)
				{
					if( p_item->GetItemClass() == ITEM_CLASS_EVENT_STAR &&
						!g_pSystemAvailableManager->IsAvailableEnchantSystem() )
						break;

					if(gpC_mouse_pointer->IsCursorDescription())
					{
						if(m_what_tab != MYSTERIOUS_TAB_ID)
							gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);
					}

					else if (gbl_item_trade_lock == false)
					{
						m_select_item_slot = m_focused_slot;
						StartBuyConfirmDialog(GetDialogShowX(), GetDialogShowY(), 1);
					}
				}
			}
			break;

		case M_LEFTBUTTON_UP:
			break;

		case M_RIGHTBUTTON_DOWN:
			//
			// Item을 선택한다.
			//
			if (m_focused_slot != NOT_SELECTED)
			{
				MItem *p_item = GetSlotItem(m_focused_slot);
				if (p_item != NULL)
				{
					if(gpC_mouse_pointer->IsCursorDescription())
						gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);

					else if (gbl_item_trade_lock == false)
					{
						m_select_item_slot = m_focused_slot;
						StartBuyConfirmDialog(GetDialogShowX(), GetDialogShowY(), p_item->GetMaxNumber());
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
void C_VS_UI_SHOP::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	m_bl_mysterious_tab = false;
	m_what_tab = NORMAL_TAB_ID;
	m_focused_slot = NOT_SELECTED;
	m_select_item_slot = NOT_SELECTED;

	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 100;
}

bool C_VS_UI_SHOP::Timer()
{
	if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_dw_prev_tickcount = GetTickCount();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SHOP::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);
}