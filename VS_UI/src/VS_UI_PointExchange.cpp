// VS_UI_PointExchange.cpp
// Point-based Exchange Market UI Implementation

#include "client_PCH.h"
#define assert(e) ((void)(e))
// Disabled assert for macOS

#pragma warning(disable:4786)

#include "VS_UI_PointExchange.h"
#include "VS_UI_Filepath.h"
#include "VS_UI_GameCommon.h"
#include "VS_UI_Item.h"
#include "VS_UI_Mouse_Pointer.h"
#include "VS_UI.h"
#include "UserOption.h"
#include "ServerInfo.h"

// For packets
#include "../Client/Packet/Cpackets/CGExchangeList.h"
#include "../Client/Packet/Cpackets/CGExchangeBuy.h"
#include "../Client/Packet/Gpackets/GCExchangeList.h"
#include "../Client/Packet/SocketInputStream.h"
#include "../Client/Packet/SocketOutputStream.h"

#include "MgamestringTable.H"

//-----------------------------------------------------------------------------
// C_VS_UI_POINT_EXCHANGE
//-----------------------------------------------------------------------------

C_VS_UI_POINT_EXCHANGE::C_VS_UI_POINT_EXCHANGE()
{
	m_pC_button_group = NULL;
	m_pC_scroll = NULL;

	m_currentTab = TAB_BROWSE;
	m_currentPage = 1;
	m_pageSize = LISTINGS_PER_PAGE;
	m_totalItems = 0;

	m_filterItemClass = 0xFF;  // All classes
	m_filterItemType = 0xFFFF;  // All types
	m_minPrice = 0;
	m_maxPrice = 0;

	m_pSelectedItem = NULL;
	m_pointBalance = 0;

	g_RegisterWindow(this);

	// Initialize sprite pack based on race
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_image_spk.Open(SPK_EXCHANGE_SLAYER);
		break;

	case RACE_VAMPIRE:
		m_image_spk.Open(SPK_VAMPIRE_INVENTORY);
		break;

	case RACE_OUSTERS:
		m_image_spk.Open(SPK_EXCHANGE_OUSTERS);
		break;
	}

	// Set window dimensions
	int w_x = 30, w_y = 0;
	Set(w_x, w_y, 600, 480);

	// Initialize button group
	m_pC_button_group = new ButtonGroup(this);

	// Add close button
	int closeBtnX = w - 40, closeBtnY = 10;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(
		closeBtnX, closeBtnY,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
		BUTTON_CLOSE, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));

	// Add refresh button
	int refreshBtnX = w - 80, refreshBtnY = 10;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(
		refreshBtnX, refreshBtnY,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP),
		BUTTON_REFRESH, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));

	// Add tab buttons
	for (int i = 0; i < 4; i++)
	{
		C_VS_UI_EVENT_BUTTON* pTab = new C_VS_UI_EVENT_BUTTON(
			GetTabX(i), GetTabY(), TAB_WIDTH, TAB_HEIGHT,
			TAB_BROWSE_BTN + i, this, TAB_BROWSE_NORMAL + i * 2);
		pTab->SetClickOption(Button::RUN_WHEN_PUSH);
		m_pC_button_group->Add(pTab);
	}

	// Add page navigation buttons
	int prevPageX = LISTING_START_X, prevPageY = h - 40;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(
		prevPageX, prevPageY, 60, 25,
		BUTTON_PREV_PAGE, this, 0));

	int nextPageX = w - 80, nextPageY = h - 40;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(
		nextPageX, nextPageY, 60, 25,
		BUTTON_NEXT_PAGE, this, 0));

	// Add buy confirm button (initially hidden)
	int buyBtnX = w / 2 - 50, buyBtnY = h - 50;
	C_VS_UI_EVENT_BUTTON* pBuyBtn = new C_VS_UI_EVENT_BUTTON(
		buyBtnX, buyBtnY, 100, 30,
		BUY_CONFIRM, this, 0);
	pBuyBtn->SetClickOption(Button::RUN_WHEN_PUSHUP);
	m_pC_button_group->Add(pBuyBtn);
}

C_VS_UI_POINT_EXCHANGE::~C_VS_UI_POINT_EXCHANGE()
{
	g_UnregisterWindow(this);
	if (m_pC_button_group)
	{
		delete m_pC_button_group;
		m_pC_button_group = NULL;
	}
}

//-----------------------------------------------------------------------------
// Start
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::Start()
{
	PI_Processor::Start();

	if (m_pC_button_group)
		m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	// Request initial listing data
	RefreshList();
}

//-----------------------------------------------------------------------------
// Finish
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Show
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::Show()
{
	// Draw background and window frame
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x, y, w, h);

		// Draw background image
		m_image_spk.BltLocked(x, y, EXCHANGE_WINDOW);

		// Draw tab buttons
		DrawTabButtons();

		// Draw content based on current tab
		switch (m_currentTab)
		{
		case TAB_BROWSE:
			DrawBrowseList();
			break;

		case TAB_MY_LISTINGS:
			DrawMyListings();
			break;

		case TAB_MY_ORDERS:
			DrawMyOrders();
			break;

		case TAB_CLAIM:
			DrawClaimList();
			break;
		}

		// Draw page info
		DrawPageInfo();

		// Draw selected item info if any
		if (m_pSelectedItem)
		{
			DrawSelectedItemInfo();
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}

	// Draw buttons
	if (m_pC_button_group)
		m_pC_button_group->Show();
}

//-----------------------------------------------------------------------------
// Process
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::Process()
{
	// Handle UI updates and events
	if (m_pC_button_group)
		m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// IsPixel
//-----------------------------------------------------------------------------

bool C_VS_UI_POINT_EXCHANGE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget (ButtonVisual interface)
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON* p_button)
{
	// Draw button widget
	if (p_button && gpC_base->m_p_DDSurface_back->Lock())
	{
		// Use global resource to draw the button
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(
			x + p_button->x, y + p_button->y, p_button->m_image_index);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::UnacquireMouseFocus()
{
	if (m_pC_button_group)
		m_pC_button_group->UnacquireMouseFocus();

	m_pSelectedItem = NULL;
}

//-----------------------------------------------------------------------------
// Click
//-----------------------------------------------------------------------------

bool C_VS_UI_POINT_EXCHANGE::Click(int clickX, int clickY)
{
	// Check if clicked on a listing item
	int relX = clickX - x;
	int relY = clickY - y;

	for (size_t i = 0; i < m_listingItems.size(); i++)
	{
		ExchangeListingItem& item = m_listingItems[i];
		if (relX >= item.displayX && relX < item.displayX + w - LISTING_START_X * 2 &&
			relY >= item.displayY && relY < item.displayY + LISTING_HEIGHT)
		{
			SelectItem(&item);
			return true;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// MouseControl
//-----------------------------------------------------------------------------

bool C_VS_UI_POINT_EXCHANGE::MouseControl(UINT message, int _x, int _y)
{
	// TODO: Implement mouse control for item hover, etc.
	return true;
}

//-----------------------------------------------------------------------------
// Run
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::Run(id_t id)
{
	switch (id)
	{
	case BUTTON_CLOSE:
		Finish();
		break;

	case BUTTON_REFRESH:
		RefreshList();
		break;

	case BUY_CONFIRM:
		BuyItem();
		break;

	case BUTTON_PREV_PAGE:
		GoToPrevPage();
		break;

	case BUTTON_NEXT_PAGE:
		GoToNextPage();
		break;

	case TAB_BROWSE_BTN:
		SwitchTab(TAB_BROWSE);
		break;

	case TAB_MY_LISTINGS_BTN:
		SwitchTab(TAB_MY_LISTINGS);
		break;

	case TAB_MY_ORDERS_BTN:
		SwitchTab(TAB_MY_ORDERS);
		break;

	case TAB_CLAIM_BTN:
		SwitchTab(TAB_CLAIM);
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Tab operations
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::SwitchTab(int tabID)
{
	m_currentTab = tabID;
	m_currentPage = 1;
	m_pSelectedItem = NULL;

	// Update display based on selected tab
	switch (tabID)
	{
	case TAB_BROWSE:
		UpdateListings();
		break;

	case TAB_MY_LISTINGS:
		UpdateMyListings();
		break;

	case TAB_MY_ORDERS:
		UpdateMyOrders();
		break;

	case TAB_CLAIM:
		UpdateClaimList();
		break;
	}
}

void C_VS_UI_POINT_EXCHANGE::RefreshList()
{
	// Send CGExchangeList packet to server
	if (!g_pSocket) return;

	CGExchangeList* pPacket = new CGExchangeList();
	pPacket->setPage(m_currentPage);
	pPacket->setPageSize(m_pageSize);
	pPacket->setItemClass(m_filterItemClass);
	pPacket->setItemType(m_filterItemType);
	pPacket->setMinPrice(m_minPrice);
	pPacket->setMaxPrice(m_maxPrice);

	g_pSocket->sendPacket(pPacket);

	delete pPacket;
}

//-----------------------------------------------------------------------------
// Page operations
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::GoToPrevPage()
{
	if (m_currentPage > 1)
	{
		m_currentPage--;
		RefreshList();
	}
}

void C_VS_UI_POINT_EXCHANGE::GoToNextPage()
{
	int maxPage = (m_totalItems + m_pageSize - 1) / m_pageSize;
	if (m_currentPage < maxPage)
	{
		m_currentPage++;
		RefreshList();
	}
}

//-----------------------------------------------------------------------------
// Item operations
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::SelectItem(ExchangeListingItem* pItem)
{
	m_pSelectedItem = pItem;
}

void C_VS_UI_POINT_EXCHANGE::BuyItem()
{
	if (!m_pSelectedItem || !g_pSocket)
		return;

	// Send buy request to server
	CGExchangeBuy* pPacket = new CGExchangeBuy();
	pPacket->setListingID(m_pSelectedItem->listingID);

	g_pSocket->sendPacket(pPacket);

	delete pPacket;
}

void C_VS_UI_POINT_EXCHANGE::CreateListing(MItem* pItem, int price)
{
	// Send create listing request to server
	// TODO: Implement create listing packet - CGExchangeCreateListing
}

void C_VS_UI_POINT_EXCHANGE::CancelListing(ExchangeListingItem* pListing)
{
	// Send cancel listing request to server
	// TODO: Implement cancel packet - CGExchangeCancelListing
}

void C_VS_UI_POINT_EXCHANGE::ClaimItem(ExchangeListingItem* pClaim)
{
	// Send claim request to server
	// TODO: Implement claim packet - CGExchangeClaim
}

//-----------------------------------------------------------------------------
// Display updates
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::UpdateListings()
{
	// This will be called when GCExchangeList packet is received
	// The packet handler will populate m_listingItems
}

void C_VS_UI_POINT_EXCHANGE::UpdateMyListings()
{
	// TODO: Update UI with my listings
}

void C_VS_UI_POINT_EXCHANGE::UpdateMyOrders()
{
	// TODO: Update UI with my orders
}

void C_VS_UI_POINT_EXCHANGE::UpdateClaimList()
{
	// TODO: Update UI with claimable items
}

void C_VS_UI_POINT_EXCHANGE::UpdatePointBalance(int balance)
{
	m_pointBalance = balance;
}

//-----------------------------------------------------------------------------
// Layout helpers
//-----------------------------------------------------------------------------

int C_VS_UI_POINT_EXCHANGE::GetTabX(int tabIndex) const
{
	return LISTING_START_X + tabIndex * (TAB_WIDTH + 10);
}

int C_VS_UI_POINT_EXCHANGE::GetListingX(int index) const
{
	return LISTING_START_X;
}

int C_VS_UI_POINT_EXCHANGE::GetListingY(int index) const
{
	return LISTING_START_Y + index * LISTING_HEIGHT;
}

//-----------------------------------------------------------------------------
// Drawing helpers
//-----------------------------------------------------------------------------

void C_VS_UI_POINT_EXCHANGE::DrawTabButtons()
{
	// Draw tab buttons with proper highlight state
	for (int i = 0; i < 4; i++)
	{
		int spriteIndex = (i == m_currentTab) ?
			TAB_BROWSE_HIGHLIGHT + i * 2 : TAB_BROWSE_NORMAL + i * 2;

		if (gpC_base->m_p_DDSurface_back->Lock())
		{
			m_image_spk.BltLocked(x + GetTabX(i), y + GetTabY(), spriteIndex);
			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}
}

void C_VS_UI_POINT_EXCHANGE::DrawPageInfo()
{
	// Draw page information
	char pageText[64];
	sprintf(pageText, "Page %d/%d", m_currentPage,
		(m_totalItems + m_pageSize - 1) / m_pageSize);

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		// TODO: Draw text - need font interface
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_VS_UI_POINT_EXCHANGE::DrawBrowseList()
{
	// Draw listing items
	for (size_t i = 0; i < m_listingItems.size(); i++)
	{
		DrawListingItem(m_listingItems[i], i);
	}
}

void C_VS_UI_POINT_EXCHANGE::DrawMyListings()
{
	// TODO: Draw my listings tab
	DrawBrowseList(); // Reuse browse list drawing for now
}

void C_VS_UI_POINT_EXCHANGE::DrawMyOrders()
{
	// TODO: Draw my orders tab
	DrawBrowseList(); // Reuse browse list drawing for now
}

void C_VS_UI_POINT_EXCHANGE::DrawClaimList()
{
	// TODO: Draw claim items tab
	DrawBrowseList(); // Reuse browse list drawing for now
}

void C_VS_UI_POINT_EXCHANGE::DrawListingItem(const ExchangeListingItem& item, int index)
{
	int itemX = x + GetListingX(index);
	int itemY = y + GetListingY(index);
	int itemW = w - LISTING_START_X * 2;
	int itemH = LISTING_HEIGHT;

	// Draw item background
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		// Highlight if selected
		if (m_pSelectedItem && m_pSelectedItem->listingID == item.listingID)
		{
			// Draw selection highlight
			// TODO: Draw highlight rectangle
		}

		// TODO: Draw item icon, name, price, seller name
		// This requires access to item sprite and font rendering

		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

void C_VS_UI_POINT_EXCHANGE::DrawSelectedItemInfo()
{
	// Draw detailed info about selected item
	// TODO: Implement detailed item info display
}
