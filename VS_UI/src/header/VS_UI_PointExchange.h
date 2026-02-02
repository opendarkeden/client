/*-----------------------------------------------------------------------------

	VS_UI_PointExchange.h
	  Point-based Exchange Market UI

	Exchange System

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_POINT_EXCHANGE_H__
#define __VS_UI_POINT_EXCHANGE_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"
#include "VS_UI_Dialog.h"
#include <vector>
#include <string>

// Forward declarations
class MItem;
class C_VS_UI_SCROLL;

// Exchange listing item for UI display
struct ExchangeListingItem
{
	int64_t listingID;
	int itemClass;
	int itemType;
	int itemID;
	int objectID;
	int pricePoint;
	std::string sellerName;
	std::string itemName;
	int enchantLevel;
	int optionCount;

	// Display position
	int displayX;
	int displayY;
};

//-----------------------------------------------------------------------------
// C_VS_UI_POINT_EXCHANGE - Point Exchange Market UI
//-----------------------------------------------------------------------------

class C_VS_UI_POINT_EXCHANGE : public Window, public Exec, public ButtonVisual
{
private:
	enum IMAGE_SPK_INDEX
	{
		EXCHANGE_WINDOW,
		EXCHANGE_BACKGROUND,
		TAB_BROWSE_NORMAL,
		TAB_BROWSE_HIGHLIGHT,
		TAB_MY_LISTINGS_NORMAL,
		TAB_MY_LISTINGS_HIGHLIGHT,
		TAB_MY_ORDERS_NORMAL,
		TAB_MY_ORDERS_HIGHLIGHT,
		TAB_CLAIM_NORMAL,
		TAB_CLAIM_HIGHLIGHT,
	};

	enum TAB_ID
	{
		TAB_BROWSE,          // Browse listings
		TAB_MY_LISTINGS,     // My listings
		TAB_MY_ORDERS,       // My orders
		TAB_CLAIM,           // Claim items
	};

	enum EXEC_ID
	{
		BUTTON_CLOSE,
		BUTTON_REFRESH,
		BUY_CONFIRM,         // Confirm buy button
		BUTTON_PREV_PAGE,
		BUTTON_NEXT_PAGE,
		TAB_BROWSE_BTN,
		TAB_MY_LISTINGS_BTN,
		TAB_MY_ORDERS_BTN,
		TAB_CLAIM_BTN,
	};

	enum
	{
		// Layout constants
		LISTING_START_X = 20,
		LISTING_START_Y = 80,
		LISTING_HEIGHT = 40,
		LISTINGS_PER_PAGE = 10,
		TAB_HEIGHT = 30,
		TAB_WIDTH = 120,
	};

	C_SPRITE_PACK				m_image_spk;

	ButtonGroup *				m_pC_button_group;
	C_VS_UI_SCROLL*				m_pC_scroll;

	int							m_currentTab;
	int							m_currentPage;
	int							m_pageSize;
	int							m_totalItems;

	// Filter options
	int							m_filterItemClass;
	int							m_filterItemType;
	int							m_minPrice;
	int							m_maxPrice;

	// Selected item
	ExchangeListingItem*		m_pSelectedItem;

	// Display data
	std::vector<ExchangeListingItem> m_listingItems;
	int							m_pointBalance;

	bool	Click(int x, int y);

	// ButtonVisual interface
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON* p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON* p_button) {}

	// Layout helpers
	int	GetTabX(int tabIndex) const;
	int	GetTabY() const { return 45; }
	int	GetListingX(int index) const;
	int	GetListingY(int index) const;

public:
	C_VS_UI_POINT_EXCHANGE();
	~C_VS_UI_POINT_EXCHANGE();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);

	void	Start();
	void	Finish();
	void	Show();

	// Tab operations
	void	SwitchTab(int tabID);
	void	RefreshList();

	// Page operations
	void	GoToPrevPage();
	void	GoToNextPage();

	// Item operations
	void	SelectItem(ExchangeListingItem* pItem);
	void	BuyItem();
	void	CreateListing(MItem* pItem, int price);
	void	CancelListing(ExchangeListingItem* pListing);
	void	ClaimItem(ExchangeListingItem* pClaim);

	// Display updates
	void	UpdateListings();
	void	UpdateMyListings();
	void	UpdateMyOrders();
	void	UpdateClaimList();
	void	UpdatePointBalance(int balance);

	// Drawing helpers
	void	DrawBrowseList();
	void	DrawMyListings();
	void	DrawMyOrders();
	void	DrawClaimList();
	void	DrawTabButtons();
	void	DrawPageInfo();
	void DrawListingItem(const ExchangeListingItem& item, int index);
	void DrawSelectedItemInfo();

	// Getters
	int		GetCurrentTab() const { return m_currentTab; }
	int		GetCurrentPage() const { return m_currentPage; }
	int		GetPointBalance() const { return m_pointBalance; }
	const std::vector<ExchangeListingItem>& GetListingItems() const { return m_listingItems; }
};

#endif // __VS_UI_POINT_EXCHANGE_H__
