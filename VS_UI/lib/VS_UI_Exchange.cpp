// VS_UI_Exchange.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_Exchange.h"
#include "VS_UI_Filepath.h"
#include "VS_UI_GameCommon.h"
#include "VS_UI_Item.h"
#include "VS_UI_Mouse_Pointer.h"
#include "VS_UI.h"
#include "UserOption.h"

#include "MGameStringTable.H"

#define	DARKBITS_NOT_TRADE_BACKGROUND		2
#define	DARKBITS_NOT_TRADE					1

static C_VS_UI_MONEY_DIALOG *	m_pC_dialog_withdraw_money;
static C_VS_UI_MONEY_DIALOG *	m_pC_dialog_exchange_money;

void ExecF_WithdrawMoneyFromExchange(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_OK:
			gpC_base->SendMessage(UI_EXCHANGE_MONEY, FALSE, m_pC_dialog_withdraw_money->GetValue());
		break;
	}

	gC_vs_ui.AcquireChatting();
}

void ExecF_ExchangeMoney(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_OK:
			gpC_base->SendMessage(UI_EXCHANGE_MONEY, TRUE, m_pC_dialog_exchange_money->GetValue());			
		break;
	}

	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE
//
//
//-----------------------------------------------------------------------------
C_VS_UI_EXCHANGE::C_VS_UI_EXCHANGE()
{
	m_pC_dialog_withdraw_money = NULL;
	m_pC_dialog_exchange_money = NULL;

	g_RegisterWindow(this);

	int w_x, w_y;
	int button_x, ok_button_y, close_button_y;
	int help_x, help_y;

	m_CheckMoneyMove = false;
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER: 
		m_image_spk.Open(SPK_EXCHANGE_SLAYER);
		w_x = 30;
		w_y = 0;
		button_x = 305;
		ok_button_y = 433;
		close_button_y = 458;
		m_my_grid_rect.Set(24+w_x, 247, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_your_grid_rect.Set(24+w_x, 23, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_money_button_point.x = 133;
		m_money_button_point.y = 433;
		m_inventory_money_button_point.x = 133;
		m_inventory_money_button_point.y = 458;
		m_your_money_button_point.x = 133;
		m_your_money_button_point.y = 209;
		m_other_ok_button_offset_x = 305;
		m_other_ok_button_offset_y = 209;
		m_your_name_x = 20;
		m_your_name_y = 209;
		m_my_name_x = 20;
		m_my_name_y = 433;
		help_x = 20;
		help_y = 458;

		Set(w_x, w_y, m_image_spk.GetWidth(EXCHANGE_WINDOW), m_image_spk.GetHeight(EXCHANGE_WINDOW));
		break;

	case RACE_VAMPIRE:
		m_image_spk.Open(SPK_VAMPIRE_INVENTORY);
		w_x = 30;
		w_y = 0;
		button_x = 292;
		ok_button_y = 427;
		close_button_y = 452;
		m_my_grid_rect.Set(17+w_x, 244, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_your_grid_rect.Set(17+w_x, 19, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_money_button_point.x = 123;
		m_money_button_point.y = 427;
		m_inventory_money_button_point.x = 123;
		m_inventory_money_button_point.y = 452;
		m_your_money_button_point.x = 123;
		m_your_money_button_point.y = 202;
		m_other_ok_button_offset_x = 292;
		m_other_ok_button_offset_y = 202;
		m_your_name_x = 17;
		m_your_name_y = 202;
		m_my_name_x = 17;
		m_my_name_y = 427;
		help_x = 17;
		help_y = 452;

		Set(w_x, w_y, m_image_spk.GetWidth(INVENTORY_WINDOW), m_image_spk.GetHeight(INVENTORY_WINDOW)*2);
		break;

	case RACE_OUSTERS:
		m_image_spk.Open(SPK_EXCHANGE_OUSTERS);
		w_x = 30;
		w_y = 0;
		button_x = 317;
		ok_button_y = 444;
		close_button_y = 469;// 36,33
		m_my_grid_rect.Set(36+w_x, 258, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_your_grid_rect.Set(36+w_x, 34, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		m_money_button_point.x = 145;
		m_money_button_point.y = 444;
		m_inventory_money_button_point.x = 145;
		m_inventory_money_button_point.y = 469;
		m_your_money_button_point.x = 145;
		m_your_money_button_point.y = 220;
		m_other_ok_button_offset_x = 317;
		m_other_ok_button_offset_y = 220;
		m_your_name_x = 32;
		m_your_name_y = 220;
		m_my_name_x = 32;
		m_my_name_y = 444;
		help_x = 32;
		help_y = 469;

		Set(w_x, w_y, m_image_spk.GetWidth(EXCHANGE_WINDOW), m_image_spk.GetHeight(EXCHANGE_WINDOW));
		break;
	}

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+button_x, y+ok_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_O), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_O), EXCHANGE_OK_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_O));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+help_x, y+help_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));

	// 단지 참조용 pointer.. by sigi
	m_pC_button_cancel = new C_VS_UI_EVENT_BUTTON(x+button_x, y+close_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), EXCHANGE_CANCEL_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X);
	m_pC_button_group->Add(m_pC_button_cancel);


	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_money_button_point.x, y+m_money_button_point.y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_MONEY));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_inventory_money_button_point.x, y+m_inventory_money_button_point.y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE_MONEY), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE_MONEY), INVENTORY_MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE_MONEY));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+m_your_money_button_point.x, y+m_your_money_button_point.y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY)+140, gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), YOUR_MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_MONEY));



#ifndef _LIB
	if (g_pTradeManager!=NULL)
	{
		delete g_pTradeManager;
	}

	g_pTradeManager = new MTradeManager;
	g_pTradeManager->Init();
	g_pTradeManager->SetOtherName( "ChangeTester" );
	g_pTradeManager->GetOtherMoneyManager()->SetMoney( 42566 );
	g_pTradeManager->GetMyMoneyManager()->SetMoney( 13344 );

	MItem* pItem;
	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_PET_ITEM );
	pItem->SetID( 6120 );
	pItem->SetItemType( 2 );
	pItem->AddItemOption( 3 );
	pItem->SetCurrentDurability( 100 );		
	((MPetItem*)pItem)->SetPetCutHead(true);
	g_pTradeManager->GetOtherInventory()->AddItem( pItem,  2, 1 );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_POTION );
	pItem->SetID( 361 );
	pItem->SetItemType( 1 );
//	pItem->SetItemOptionList( 0 );
	pItem->SetCurrentDurability( 200 );	
	g_pTradeManager->GetOtherInventory()->AddItem( pItem,  4, 0 );

	//----------------------------------------------------
	// Item 생성 --> 추가
	//----------------------------------------------------
	pItem = MItem::NewItem( ITEM_CLASS_COAT );
	pItem->SetID( 642 );
	pItem->SetItemType( 0 );
	pItem->AddItemOption( 55 );
	pItem->SetCurrentDurability( 300 );	
	g_pTradeManager->GetOtherInventory()->AddItem( pItem,  7, 2 );
	
	//g_pTradeManager->AcceptOtherTrade();	// 다른 사람이 OK누름.
#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_EXCHANGE
//
//
//-----------------------------------------------------------------------------
C_VS_UI_EXCHANGE::~C_VS_UI_EXCHANGE()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_dialog_withdraw_money);
	DeleteNew(m_pC_dialog_exchange_money);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Click
//
// 현재 Item을 들고 있으면 놓던가 교체하고, 들고 있지 않으면 교환창에 있는
// 것을 집는다.
//
// 뭔가 했다면 true를, 그렇지않으면 false를 반환한다.
//
// grid_start_x, grid_start_y는 igrid 시작점이다. 이것을 참조하여 item
// (x, y)를 구한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_EXCHANGE::Click(int grid_start_x, int grid_start_y)
{
	if (m_focus_grid_x == NOT_SELECTED || 
		 m_focus_grid_y == NOT_SELECTED ||
		 gbl_item_lock == true)
	{
		return false;
	}

	int item_x, item_y;

	//-----------------------------------------------------------
	// 내가 OK를 누른 상태인가??
	//-----------------------------------------------------------
	BOOL bAcceptTrade = g_pTradeManager->IsAcceptMyTrade();

	//-----------------------------------------------------------
	// inventory에 있는것과 교환하거나.. 그냥 놓을때
	//-----------------------------------------------------------
	if (gpC_mouse_pointer->GetPickUpItem()) // 들고 있는가?
	{
		const MItem* p_cur_item = g_pTradeManager->GetMyInventory()->GetItem(m_focus_grid_x, m_focus_grid_y);

		//-----------------------------------------------------------
		// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
		//-----------------------------------------------------------
		if (gpC_mouse_pointer->GetPickUpItem()->IsInsertToItem( p_cur_item ))
		{
			// 이전에 있던 item에 추가될 수 있는 경우

			// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
			// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
			gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
																  m_focus_grid_x, m_focus_grid_y,
																  (void *)p_cur_item); // 대상 Item
		}
		//-----------------------------------------------------------
		// inventory의 아이템과 mouse 아이템이 바뀌는 경우
		//-----------------------------------------------------------
		else
		{	
			// 추가될 수 없는 경우
			MItem* p_old_item  = NULL;

			//-----------------------------------------------------------
			// 내가 OK누른 상태
			//-----------------------------------------------------------
			if (bAcceptTrade)
			{
				if (g_pTradeManager->GetMyInventory()->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),		// 추가할 item
															m_focus_grid_x, m_focus_grid_y,	// 추가할 위치 
															p_old_item))								// 원래있던 item
				{
					gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY,
											  m_focus_grid_x, m_focus_grid_y,
											  p_old_item); // 들고 있던 것을 보낸다.
				}
				else
				{
					// failed.
					return false;
				}
			}
			//-----------------------------------------------------------
			// OK 안 누른 상태
			//-----------------------------------------------------------
			else
			{
				if (g_pTradeManager->GetMyInventory()->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),		// 추가할 item
															m_focus_grid_x, m_focus_grid_y,	// 추가할 위치 
															p_old_item))								// 원래있던 item
				{
					if (p_old_item != NULL) // replace 되었는가?
					{
						item_x = grid_start_x+p_old_item->GetGridX()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X+(p_old_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_old_item->GetInventoryFrameID())/2;
						item_y = grid_start_y+p_old_item->GetGridY()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y+(p_old_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_old_item->GetInventoryFrameID())/2;

						gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY,
											  m_focus_grid_x, m_focus_grid_y,
											  p_old_item); // 교환될 아이템..

						// UI에서 바꿔야 한다.
						//gpC_mouse_pointer->PickUpItem((MItem *)p_old_item);
					}
					else
					{
						// not replace, just drop at empty grid.

						gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
											  m_focus_grid_x, m_focus_grid_y,
											  p_old_item);

						// 100% 놓을 수 있으니까 UI에서 drop시킨다.
						//gpC_mouse_pointer->DropItem();
					}
				}
				else
				{
					// failed.
					return false;
				}
			}
		}
	}
	//-----------------------------------------------------------
	// inventory에 있는 걸 집을 때
	//-----------------------------------------------------------
	else
	{
		// 집는다.
		const MItem * p_item = g_pTradeManager->GetMyInventory()->GetItem(m_focus_grid_x, m_focus_grid_y);

		if (p_item != NULL) // Item이 있다.
		{
			{
				//-----------------------------------------------------------
				// OK 누른 상태
				//-----------------------------------------------------------
				if (bAcceptTrade)
				{
				}
				else
				{
					g_pTradeManager->GetMyInventory()->RemoveItem(m_focus_grid_x, m_focus_grid_y);
				}

				item_x = grid_start_x+p_item->GetGridX()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = grid_start_y+p_item->GetGridY()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;

	#ifdef _LIB
				gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
													  m_focus_grid_x, m_focus_grid_y,
													  (MItem *)p_item);
	#else
				
				gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
													  MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
													  MAKEDWORD(item_x, item_y),
													  (MItem *)p_item);
	#endif
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Check
//
// 교환할려는 item을 체크한다. 
// 이미 체크되어있다면.. 취소한다.
//
// grid_start_x, grid_start_y는 igrid 시작점이다. 이것을 참조하여 item
// (x, y)를 구한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_EXCHANGE::Check(int grid_start_x, int grid_start_y)
{
	if (m_focus_grid_x == NOT_SELECTED || 
		 m_focus_grid_y == NOT_SELECTED ||
		 gbl_item_lock == true)
	{
		return false;
	}
	
	// 집는다.
	const MItem * p_item = g_pTradeManager->GetMyInventory()->GetItem(m_focus_grid_x, m_focus_grid_y);

	if (p_item != NULL && p_item->GetItemClass() != ITEM_CLASS_RELIC
		&& p_item->GetItemClass() != ITEM_CLASS_BLOOD_BIBLE
		&& p_item->GetItemClass() != ITEM_CLASS_COUPLE_RING
		&& p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING
		&& !p_item->IsQuestItem()
		) // Item이 있다. 그리고 성물이 아니다.
	{
		// 처리는 client에 맡긴다.
		gpC_base->SendMessage(UI_ITEM_SELECT_EXCHANGE,
							  m_focus_grid_x, m_focus_grid_y,
							  (MItem *)p_item);				
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::GetFocusedItemGridX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_EXCHANGE::GetFocusedItemGridX(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_my_grid_rect.x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*p_item->GetGridX();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::GetFocusedItemGridY
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_EXCHANGE::GetFocusedItemGridY(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_my_grid_rect.y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*p_item->GetGridY();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::GetFocusedOtherItemGridX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_EXCHANGE::GetFocusedOtherItemGridX(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_your_grid_rect.x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*p_item->GetGridX();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::GetFocusedOtherItemGridY
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_EXCHANGE::GetFocusedOtherItemGridY(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_your_grid_rect.y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*p_item->GetGridY();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::CancelPushState
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::CancelPushState()
{
	m_pC_button_group->CancelPushState();
//	m_bl_money_button_pushed = false;
//	m_bl_inventory_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Process
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::UnacquireMouseFocus
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
//	m_bl_money_button_focused = false;
//	m_bl_inventory_money_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_EXCHANGE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::Run(id_t id)
{
	if (g_pTradeManager==NULL)
	{
		return;
	}

	switch (id)
	{
		case EXCHANGE_OK_ID:
			if(!g_pTradeManager->IsAcceptTime())break;
			// OK한거를 취소할수 있게 한다.
			if (m_pC_button_cancel!=NULL)
			{
				m_pC_button_cancel->SetID( EXCHANGE_OK_CANCEL_ID );
			}
			
			// 이미 눌려진 상태가 아닌 경우만 메세지 보낸다.
			if (!g_pTradeManager->IsAcceptMyTrade())
			{
				gpC_base->SendMessage(UI_OK_EXCHANGE);
			}
			//m_CheckMoneyMove = true;
			break;

		// OK한거를 취소하는 경우 --> cancel은 다시 진정한(-_-;) cancel로 바뀐다.
		case EXCHANGE_OK_CANCEL_ID :
			if (m_pC_button_cancel!=NULL)
			{
				m_pC_button_cancel->SetID( EXCHANGE_CANCEL_ID );
			}

			if (g_pTradeManager->IsAcceptMyTrade())
			{
				gpC_base->SendMessage(UI_CANCEL_EXCHANGE);
			}
			
			break;

		case EXCHANGE_CANCEL_ID:
			gpC_base->SendMessage(UI_CLOSE_EXCHANGE);
			break;

		case HELP_ID:
			gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::EXCHANGE);
			break;

		case MONEY_ID:
			// money button이 눌렸다.
			// 교환할 돈에서 뺀다
			DeleteNew(m_pC_dialog_withdraw_money);
			if (false == m_CheckMoneyMove && g_pTradeManager->GetMyMoneyManager()->GetMoney() > 0)
			{
				m_pC_dialog_withdraw_money = new C_VS_UI_MONEY_DIALOG(x+m_money_button_point.x, 
					m_money_button_point.y+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 
					1, 0, ExecF_WithdrawMoneyFromExchange, DIALOG_OK|DIALOG_CANCEL, 10,
					C_VS_UI_MONEY_DIALOG::MONEY_WITHDRAW_FROM_EXCHANGE);
				m_pC_dialog_withdraw_money->Start();
				m_CheckMoneyMove = true;
			}
			break;
			
		case INVENTORY_MONEY_ID:
			// money button이 눌렸다.
			// 교환할 돈을 추가한다.
			DeleteNew(m_pC_dialog_exchange_money);
			if (false == m_CheckMoneyMove && g_pMoneyManager->GetMoney() > 0)
			{
				m_pC_dialog_exchange_money = new C_VS_UI_MONEY_DIALOG(x+m_inventory_money_button_point.x, 
					m_inventory_money_button_point.y+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE_MONEY), 
					2, 0, ExecF_ExchangeMoney, DIALOG_OK|DIALOG_CANCEL, 10, 
					C_VS_UI_MONEY_DIALOG::MONEY_EXCHANGE);																						
				m_pC_dialog_exchange_money->Start();
				m_CheckMoneyMove = true;
			}
			break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char *m_help_string[7] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CLICK_EXCHANGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANCEL_OK_BUTTON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANCEL_EXCHANGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SHOW_EXCHANGE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_BRING_MONEY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_EXCHANGE_MONEY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_EXCHANGE_YOUR_MONEY].GetString(),
	};
	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

///-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);	

	if(m_CheckMoneyMove && (p_button->GetID() == MONEY_ID || p_button->GetID() == INVENTORY_MONEY_ID))
	{
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(p_button->x, p_button->y, p_button->m_image_index,1);
		return;
	}
	if(p_button->GetID() == YOUR_MONEY_ID)
	{
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
	}
	else if(/*false == m_CheckMoneyMove &&*/ (p_button->GetID() == MONEY_ID || p_button->GetID() == INVENTORY_MONEY_ID))
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
	}
	else if(p_button->GetID()==EXCHANGE_OK_ID && !g_pTradeManager->IsAcceptTime())
	{
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedColor(p_button->x, p_button->y, p_button->m_image_index, rgb_RED);
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if((p_button->GetPressState() || p_button->GetID()==EXCHANGE_OK_ID) && g_pTradeManager->IsAcceptMyTrade())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else if(p_button->GetID() == EXCHANGE_OK_ID)
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET, 1);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
		{	
			if((p_button->GetPressState() || p_button->GetID()==EXCHANGE_OK_ID) && g_pTradeManager->IsAcceptMyTrade())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else if(p_button->GetID() == EXCHANGE_OK_ID)
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(p_button->x, p_button->y, p_button->m_image_index, 1);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		}
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_EXCHANGE::MouseControl(UINT message, int _x, int _y)
{
	if (g_pTradeManager==NULL)
	{
		return true;
	}

	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	const MItem * p_selected_item;

 	switch (message)
	{
		case M_MOVING:
//			m_bl_money_button_focused = false;
//			m_bl_inventory_money_button_focused = false;

			int i;
			int loop;
			int px, py;


			{
				static char *m_help_string[2] =
				{
					(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_MONEY_AFTER_EXCHANGE].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_WILL_EXCHANGE_MONEY].GetString(),
				};

				if(_x > m_inventory_money_button_point.x+30 && _x < m_inventory_money_button_point.x+184 && _y > m_inventory_money_button_point.y && _y < m_inventory_money_button_point.y+22)
				{
					g_descriptor_manager.Set(DID_INFO, m_inventory_money_button_point.x+30, m_inventory_money_button_point.y, (void *)m_help_string[0],0,0);
				}

				if(_x > m_money_button_point.x+30 && _x < m_money_button_point.x+184 && _y > m_money_button_point.y && _y < m_money_button_point.y+22)
				{
					
					g_descriptor_manager.Set(DID_INFO, m_money_button_point.x+30, m_money_button_point.y, (void *)m_help_string[1],0,0);
				}
			}

			if (gpC_mouse_pointer->GetPickUpItem())
				loop = ITEM_REF_POINT_COUNT;
			else
			{
				loop = 1;

			}

			for (i=0; i < loop; i++)
			{
				if (loop == ITEM_REF_POINT_COUNT)
				{
					px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
					py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
				}
				else
				{
					px = _x;
					py = _y;
				}

				// search grid
				int distance_x = px - m_my_grid_rect.x;
				int distance_y = py - m_my_grid_rect.y;

				if (distance_x >= 0 && distance_x < m_my_grid_rect.w && 
					 distance_y >= 0 && distance_y < m_my_grid_rect.h)
				{
					if(gpC_Imm && m_focus_grid_x != distance_x/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X || m_focus_grid_y != distance_y/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)
						gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
					m_focus_grid_x = distance_x/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X;
					m_focus_grid_y = distance_y/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y;

					if (loop == ITEM_REF_POINT_COUNT)
					{
						// item이 grid 영역에 어느정도 들어오면 안으로 위치시킨다.
						const MItem * p_pickup_item = gpC_mouse_pointer->GetPickUpItem();
						int a, b;
						switch (i)
						{
							case 0: // left up - first!
								a = m_focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
								b = m_focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
								if (a > 0)
									m_focus_grid_x -= a;
								if (b > 0)
									m_focus_grid_y -= b;
								break;

							case 1: // right up
								m_focus_grid_x = 0;
								b = m_focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
								if (b > 0)
									m_focus_grid_y -= b;
								break;

							case 2: // left down
								m_focus_grid_y = 0;
								a = m_focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
								if (a > 0)
									m_focus_grid_x -= a;
								break;

							case 3: // right down
								m_focus_grid_y = 0;
								if (m_focus_grid_x+1 <= p_pickup_item->GetGridHeight())
									m_focus_grid_x = 0;
						}
					}

					p_selected_item = g_pTradeManager->GetMyInventory()->GetItem(m_focus_grid_x, m_focus_grid_y);
					if (p_selected_item != NULL)
						g_descriptor_manager.Set(DID_ITEM, GetFocusedItemGridX(p_selected_item), GetFocusedItemGridY(p_selected_item), (void *)p_selected_item);

					return true;
//					break; // escape 'for'
				}

				// other grid
				distance_x = px - m_your_grid_rect.x;
				distance_y = py - m_your_grid_rect.y;

				if (distance_x >= 0 && distance_x < m_your_grid_rect.w && 
					 distance_y >= 0 && distance_y < m_your_grid_rect.h)
				{
					int focus_grid_x = distance_x/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X;
					int focus_grid_y = distance_y/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y;

					if (loop == ITEM_REF_POINT_COUNT)
					{
						// item이 grid 영역에 어느정도 들어오면 안으로 위치시킨다.
						const MItem * p_pickup_item = gpC_mouse_pointer->GetPickUpItem();
						int a, b;
						switch (i)
						{
							case 0: // left up - first!
								a = focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
								b = focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
								if (a > 0)
									focus_grid_x -= a;
								if (b > 0)
									focus_grid_y -= b;
								break;

							case 1: // right up
								focus_grid_x = 0;
								b = focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
								if (b > 0)
									focus_grid_y -= b;
								break;

							case 2: // left down
								focus_grid_y = 0;
								a = focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
								if (a > 0)
									focus_grid_x -= a;
								break;

							case 3: // right down
								focus_grid_y = 0;
								if (focus_grid_x+1 <= p_pickup_item->GetGridHeight())
									focus_grid_x = 0;
						}
					}

					p_selected_item = g_pTradeManager->GetOtherInventory()->GetItem(focus_grid_x, focus_grid_y);
					if (p_selected_item != NULL)
						g_descriptor_manager.Set(DID_ITEM, GetFocusedOtherItemGridX(p_selected_item), GetFocusedOtherItemGridY(p_selected_item), (void *)p_selected_item);

					break; // escape 'for'
				}
			}
			if(gpC_Imm && m_focus_grid_x != NOT_SELECTED || m_focus_grid_y != NOT_SELECTED)
				gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
			m_focus_grid_x = NOT_SELECTED;
			m_focus_grid_y = NOT_SELECTED;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			//
			// Item을 집던가 놓는다.
			//
			{
				bool ret = Click(m_my_grid_rect.x, m_my_grid_rect.y);

				if (ret)
					EMPTY_MOVE;
			}

			//
			// money button / close button
			//
//			if (m_bl_money_button_focused)
//				m_bl_money_button_pushed = true;			
//
//			if (m_bl_inventory_money_button_focused)
//				m_bl_inventory_money_button_pushed = true;

			break;

		case M_LEFTBUTTON_UP:
//			if (m_bl_money_button_focused && m_bl_money_button_pushed)
//			{
//				
//			}
//
//			else if (m_bl_inventory_money_button_focused && m_bl_inventory_money_button_pushed)
//			{
//				
//			}

//			m_bl_money_button_pushed = false;
//			m_bl_inventory_money_button_pushed = false;
			break;

		case M_RIGHTBUTTON_DOWN:
			{
				bool ret = Check(m_my_grid_rect.x, m_my_grid_rect.y);

				if (ret)
					EMPTY_MOVE;
			}
			break;
		}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
//	m_bl_money_button_focused = false;
//	m_bl_money_button_pushed = false;

//	m_bl_inventory_money_button_focused = false;
//	m_bl_inventory_money_button_pushed = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE::Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_EXCHANGE::Show()
{
	if (g_pTradeManager==NULL)
	{
		return;
	}

	//글자 찍을것들을 기억해뒀다가 한꺼번에 찍는다.
	std::vector<RECT>	vNumRect;
	std::vector<int>	vNum;
	
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		switch(g_eRaceInterface)
		{
		case RACE_SLAYER:
			m_image_spk.BltLocked(x, y, EXCHANGE_WINDOW);
			break;

		case RACE_VAMPIRE:
			m_image_spk.BltLocked(x, y, INVENTORY_WINDOW);
			m_image_spk.BltLocked(x, y+m_image_spk.GetHeight(INVENTORY_WINDOW)-30, INVENTORY_WINDOW);
			m_image_spk.BltLocked(x, y+m_image_spk.GetHeight(INVENTORY_WINDOW)*2-30, INVENTORY_WINDOW_BOTTOM);
			break;

		case RACE_OUSTERS:
			m_image_spk.BltLocked(x, y, EXCHANGE_WINDOW);
			break;
		}
		
		// name
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_my_name_x, y+m_my_name_y, C_GLOBAL_RESOURCE::AB_NAME_BAR);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_your_name_x, y+m_your_name_y, C_GLOBAL_RESOURCE::AB_NAME_BAR);

		// money
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_inventory_money_button_point.x+25, y+m_inventory_money_button_point.y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_money_button_point.x+25, y+m_money_button_point.y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_your_money_button_point.x+25, y+m_your_money_button_point.y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);

		//----------------------------------------------------------------
		// 상대방의 OK버튼 상태를 표시한다.
		//----------------------------------------------------------------
		if(g_pTradeManager->IsAcceptOtherTrade()&&g_pTradeManager->IsAcceptTime())
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_other_ok_button_offset_x, y+m_other_ok_button_offset_y, C_GLOBAL_RESOURCE::AB_BUTTON_O_PUSHED);
		} else			
		{			
			if(!g_pTradeManager->IsAcceptTime())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedColor(x+m_other_ok_button_offset_x, y+m_other_ok_button_offset_y, C_GLOBAL_RESOURCE::AB_BUTTON_O, rgb_RED);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(x+m_other_ok_button_offset_x, y+m_other_ok_button_offset_y, C_GLOBAL_RESOURCE::AB_BUTTON_O,1);
		}

		//----------------------------------------------------------------
		//
		// show My trade Item
		//
		//----------------------------------------------------------------
		g_pTradeManager->GetMyInventory()->SetBegin();

		const MItem * p_selected_item = g_pTradeManager->GetMyInventory()->GetItem(m_focus_grid_x, m_focus_grid_y);

		while (g_pTradeManager->GetMyInventory()->IsNotEnd())
		{
			const MItem * p_item = g_pTradeManager->GetMyInventory()->Get();

			// p_item은 NULL이 반드시 아니다. 왜냐하면 존재하는 것만 Get()하기 때문이다.
			assert(p_item);

			// frame id -> sprite id
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();

			int item_x = GetFocusedItemGridX(p_item);
			int item_y = GetFocusedItemGridY(p_item);

			int print_y = item_y + p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y - 12;
			int print_x = item_x + p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X-1;

			// Item이 놓여있는 영역 표시
			for (int j = 0; j < p_item->GetGridHeight(); j++)
				for (int i = 0; i < p_item->GetGridWidth(); i++)
				{
//					int back_color = (p_item->GetGridY()+j)*C_VS_UI_INVENTORY::GRID_X+(p_item->GetGridX()+i);

					//------------------------------------------------------------
					// 교환할려는 아이템인 경우
					//------------------------------------------------------------
					if (p_item->IsTrade())
					{
						switch(g_eRaceInterface)
						{
						case RACE_SLAYER:
							m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, EXCHANGE_ITEMBACK);
							break;

						case RACE_VAMPIRE:
							m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, INVENTORY_ITEMBACK);
							break;

						case RACE_OUSTERS:
							m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, EXCHANGE_ITEMBACK);
							break;						
						}						
					}
					//------------------------------------------------------------
					// 교환할려는 아이템이 아닌 경우
					//------------------------------------------------------------
//					else
//					{
//						m_pC_backcolor_spk->BltLockedDarkness(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, 
//												   item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j,
//													back_color, DARKBITS_NOT_TRADE_BACKGROUND);
//					}
				}

			item_x += (p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			item_y += (p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
			
			if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
			{
				if(p_item->IsQuestItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
				else
				if(p_item->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
				else				
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
				gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
				if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
				{
					ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
					if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
						eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
						eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
						)
					{
						// 2004, 3, 9 sobeit - x,y 변경
						//gpC_global_resource->m_pC_info_spk->BltLockedOutline(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, RGB_WHITE, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
						gpC_global_resource->m_pC_info_spk->BltLockedOutline(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), RGB_WHITE, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);

					}
				}
			}
			else
			{
				if(p_item->IsQuestItem() )
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
				else					
				if(p_item->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
				else				
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);

				//------------------------------------------------------------
				// 교환할려는 아이템인 경우
				//------------------------------------------------------------
				if (p_item->IsTrade())
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
								// 2004, 3, 9 sobeit - x,y 변경
								//gpC_global_resource->m_pC_info_spk->BltLocked(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
								gpC_global_resource->m_pC_info_spk->BltLocked(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
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
								// 2004, 3, 9 sobeit - x,y 변경
								//gpC_global_resource->m_pC_info_spk->BltLockedColor(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
								gpC_global_resource->m_pC_info_spk->BltLockedColor(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
							}
						}
					}
				}
				//------------------------------------------------------------
				// 교환할려는 아이템이 아닌 경우
				//------------------------------------------------------------
				else
				{
					gpC_item->BltLockedDarkness(item_x, item_y, frame_id, DARKBITS_NOT_TRADE);				
					if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
					{
						ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
						if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
							)
						{
							// 2004, 3, 9 sobeit - x,y 변경
							//gpC_global_resource->m_pC_info_spk->BltLockedDarkness(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, DARKBITS_NOT_TRADE);
							gpC_global_resource->m_pC_info_spk->BltLockedDarkness(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, DARKBITS_NOT_TRADE);
						}
					}
				}
			}

			//아이템 개수 찍을 alphabox 숫자는 찍지 않는다 저장해 뒀다가 나중에 몰아서 한꺼번에 찍는다
			if(p_item->IsPileItem() || p_item->IsChargeItem())
			{
				RECT rt;
				rt.right = print_x;
				for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
				if(depth == 0) depth = 1;
				rt.left = print_x - 7*depth;
				rt.top = print_y;
				rt.bottom = rt.top+12;
				
				rt.left = max(0, rt.left);
				rt.top = max(0, rt.top);
				
				if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

				vNumRect.push_back(rt);
				vNum.push_back(p_item->GetNumber());
				
			}
			g_pTradeManager->GetMyInventory()->Next();
		}

		//----------------------------------------------------------------
		//
		// Item을 들고 있으면 grid 위치를 미리 알 수 있도록 한다.
		//
		//----------------------------------------------------------------
		if (gpC_mouse_pointer->GetPickUpItem() && 
			 m_focus_grid_x != NOT_SELECTED && 
			 m_focus_grid_y != NOT_SELECTED)
		{
			MItem * p_item = gpC_mouse_pointer->GetPickUpItem();

			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
			int item_x = m_my_grid_rect.x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*m_focus_grid_x+(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			int item_y = m_my_grid_rect.y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*m_focus_grid_y+(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;

			MItem * p_old_item;
			if (g_pTradeManager->GetMyInventory()->CanReplaceItem(p_item, m_focus_grid_x, m_focus_grid_y, p_old_item))
			{
				if (p_old_item)
					gpC_item->BltLockedOutlineOnly(item_x, item_y, ga_item_blink_color_table[g_blink_value], frame_id);
				else
					
					gpC_item->BltLockedOutlineOnly(item_x, item_y, POSSIBLE_COLOR, frame_id);
			}
			else
			{
				gpC_item->BltLockedOutlineOnly(item_x, item_y, IMPOSSIBLE_COLOR, frame_id);
			}
		}

		//
		// show Your trade item
		//
		g_pTradeManager->GetOtherInventory()->SetBegin();

		while (g_pTradeManager->GetOtherInventory()->IsNotEnd())
		{
			const MItem * p_item = g_pTradeManager->GetOtherInventory()->Get();

			// p_item은 NULL이 반드시 아니다. 왜냐하면 존재하는 것만 Get()하기 때문이다.
			assert(p_item);

			// frame id -> sprite id
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();

			int item_x = m_your_grid_rect.x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*p_item->GetGridX();
			int item_y = m_your_grid_rect.y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*p_item->GetGridY();

			int print_y = item_y + p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y - 12;
			int print_x = item_x + p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X-1;
			
			// Item이 놓여있는 영역 표시
			for (int j = 0; j < p_item->GetGridHeight(); j++)
				for (int i = 0; i < p_item->GetGridWidth(); i++)
				{
					switch(g_eRaceInterface)
					{
					case RACE_SLAYER:
						m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, EXCHANGE_ITEMBACK);
						break;

					case RACE_VAMPIRE:
						m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, INVENTORY_ITEMBACK);
						break;

					case RACE_OUSTERS:
						m_image_spk.BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, EXCHANGE_ITEMBACK);
						break;
					}						
				}

			item_x += (p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			item_y += (p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
			
			if(p_item->IsQuestItem())
				CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
			else
			if(p_item->IsUniqueItem())
				CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
			else			
				CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
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
						// 2004, 3, 9 sobeit - x,y 변경
						//gpC_global_resource->m_pC_info_spk->BltLocked(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
						gpC_global_resource->m_pC_info_spk->BltLocked(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
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
						// 2004, 3, 9 sobeit - x,y 변경
						//gpC_global_resource->m_pC_info_spk->BltLockedColor(item_x-(p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2, item_y-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
						gpC_global_resource->m_pC_info_spk->BltLockedColor(GetFocusedItemGridX(p_item), GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
					}
				}
			}

			//아이템 개수 찍을 alphabox 숫자는 찍지 않는다 저장해 뒀다가 나중에 몰아서 한꺼번에 찍는다
			if(p_item->IsPileItem() || p_item->IsChargeItem())
			{
				RECT rt;
				rt.right = print_x;
				for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
				if(depth == 0) depth = 1;
				rt.left = print_x - 7*depth;
				rt.top = print_y;
				rt.bottom = rt.top+12;
				
				rt.left = max(0, rt.left);
				rt.top = max(0, rt.top);
				
				if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

				vNumRect.push_back(rt);
				vNum.push_back(p_item->GetNumber());
				
			}
			g_pTradeManager->GetOtherInventory()->Next();
		}

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	

//글자 출력용 dc를 잡는다.
	g_FL2_GetDC();
	m_pC_button_group->ShowDescription();

	char sz_num[10];
	COLORREF markColor = RGB(220, 220, 220);//RGB(140, 140, 255);
	for(int i = 0; i < vNum.size(); i++)
	{
		wsprintf(sz_num, "%d", vNum[i]);
		g_PrintColorStr(vNumRect[i].left, vNumRect[i].top, sz_num, gpC_base->m_item_desc_pi, markColor);
	}

	//----------------------------------------------------------------
	// 내가 가진 돈	
	//----------------------------------------------------------------
	// 2004, 12, 14, sobeit modify start
	char money_buf[512];
	if(NULL != g_pMoneyManager)
	{
		if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
		{
			std::string sstr = g_GetStringByMoney(g_pMoneyManager->GetMoney());
			g_Print(x+m_inventory_money_button_point.x+150, y+m_inventory_money_button_point.y+2, sstr.c_str(), &gpC_base->m_money2_pi);
		}
		else
		{
			wsprintf(money_buf, "%d", g_pMoneyManager->GetMoney());
			std::string sstr = money_buf;
			for(i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(money_buf, "$%s", sstr.c_str());
			g_Print(x+m_inventory_money_button_point.x+150, y+m_inventory_money_button_point.y+2, money_buf, &gpC_base->m_money2_pi);
		}
	}
	// 2004, 12, 14, sobeit modify end
	//----------------------------------------------------------------
	// 교환할 내 돈
	//----------------------------------------------------------------
	// 2004, 12, 14, sobeit modify start
//	wsprintf(money_buf, "%d", g_pTradeManager->GetMyMoneyManager()->GetMoney());
//	sstr = money_buf;
//	for(i = 3; i <= 13; i += 4)
//		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
//	sprintf(money_buf, "$%s", sstr.c_str());
//	g_Print(x+m_money_button_point.x+30, y+m_money_button_point.y+2, money_buf, &gpC_base->m_money_pi);
	if(NULL != g_pTradeManager && NULL != g_pTradeManager->GetMyMoneyManager())
	{
		if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
		{
			std::string sstr = g_GetStringByMoney(g_pTradeManager->GetMyMoneyManager()->GetMoney());
			g_Print(x+m_money_button_point.x+150, y+m_money_button_point.y+2, sstr.c_str(), &gpC_base->m_money2_pi);
		}
		else
		{
			wsprintf(money_buf, "%d", g_pTradeManager->GetMyMoneyManager()->GetMoney());
			std::string sstr = money_buf;
			for(i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(money_buf, "$%s", sstr.c_str());
			g_Print(x+m_money_button_point.x+150, y+m_money_button_point.y+2, money_buf, &gpC_base->m_money2_pi);
		}
	}
	// 2004, 12, 14, sobeit modify end
	//----------------------------------------------------------------
	// 남 돈 - -;
	//----------------------------------------------------------------
	// 2004, 12, 14, sobeit modify start
//	wsprintf(money_buf, "%d", g_pTradeManager->GetOtherMoneyManager()->GetMoney());
//	sstr = money_buf;
//	for(i = 3; i <= 13; i += 4)
//		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
//	sprintf(money_buf, "$%s", sstr.c_str());
//	g_Print(x+m_your_money_button_point.x+30, y+m_your_money_button_point.y+2, money_buf, &gpC_base->m_money_pi);
	if(NULL != g_pTradeManager && NULL != g_pTradeManager->GetOtherMoneyManager())
	{
		if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
		{
			std::string sstr = g_GetStringByMoney(g_pTradeManager->GetOtherMoneyManager()->GetMoney());
			g_Print(x+m_your_money_button_point.x+150, y+m_your_money_button_point.y+2, sstr.c_str(), &gpC_base->m_money2_pi);
		}
		else
		{
			wsprintf(money_buf, "%d", g_pTradeManager->GetOtherMoneyManager()->GetMoney());
			std::string sstr = money_buf;
			for(i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(money_buf, "$%s", sstr.c_str());
			g_Print(x+m_your_money_button_point.x+150, y+m_your_money_button_point.y+2, money_buf, &gpC_base->m_money2_pi);
		}
	}
	// 2004, 12, 14, sobeit modify end
	// show my name on trade interace
	g_Print(x+m_my_name_x+6, y+m_my_name_y+2, g_char_slot_ingame.sz_name.c_str(), &gpC_base->m_char_value_pi);

	// show your name on trade interface
	g_Print(x+m_your_name_x+6, y+m_your_name_y+2, g_pTradeManager->GetOtherName(), &gpC_base->m_char_value_pi);

//글자 출력용 dc를 풀어준다.
	g_FL2_ReleaseDC();

	SHOW_WINDOW_ATTR;
}