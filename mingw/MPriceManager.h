//-----------------------------------------------------------------------------
// MPriceManager.h
//-----------------------------------------------------------------------------

#ifndef __MPRICEMANAGER_H__
#define __MPRICEMANAGER_H__

class MItem;

// 가격은 type의 별이 number개.
struct STAR_ITEM_PRICE {
	int type;
	int number;
};

class MPriceManager {
	public :
		enum TRADE_TYPE
		{
			NPC_TO_PC,		// player가 물건 살때, m_MarketCondSell
			PC_TO_NPC,		// player가 물건 팔때, m_MarketCondBuy
			REPAIR,			// player가 물건 repair할 때,
			SILVERING,		// player가 물건에 실버코팅할 때,
		};

	public :
		MPriceManager(); 	
		~MPriceManager();

		//-------------------------------------------------------		
		// Get Item Price
		//-------------------------------------------------------		
		int			GetItemPrice(MItem* pItem, TRADE_TYPE type, bool bMysterious=false);
		void		GetItemPrice(MItem* pItem, STAR_ITEM_PRICE& price);
		int			GetMysteriousPrice(MItem* pItem) const;

		//-------------------------------------------------------		
		// 시세
		//-------------------------------------------------------		
		// NPC 기준으로 buy, sell
		void		SetMarketCondBuy(int buy)			{ m_MarketCondBuy = buy; }
		void		SetMarketCondSell(int sell)			{ m_MarketCondSell = sell; }

		int			GetMarketCondBuy() const			{ return m_MarketCondBuy; }
		int			GetMarketCondSell() const			{ return m_MarketCondSell; }

		void		SetEventItemPrice(int Price)		{ m_EventFixPrice = Price; }

	protected :
		
		// 시세
		int					m_MarketCondBuy;		// NPC가 살때(25)
		int					m_MarketCondSell;		// NPC가 팔때(100)
		int					m_EventFixPrice;		// 서버에서 정해주는 이벤트 아이템 가격
};

extern MPriceManager*		g_pPriceManager;

#endif

