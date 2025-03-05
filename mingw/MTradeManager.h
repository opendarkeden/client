//-----------------------------------------------------------------------------
// MTradeManager.h
//-----------------------------------------------------------------------------
// 교환 담당~
//
// Init하면 한 쌍의 (inventory와 moneymanager)를 생성한다.
// 
// 하나는 내꺼.. 하나는 남꺼..
//
// * Get .... () 해서 외부에서 처리하고..
//
// item이 add/remove되거나.. money가 바뀌면 .. AcceptTrade를 제거해야한다.
//
// MyAcceptTrade && OtherAcceptTrade인 경우만 trade 가능하다.
//
// CanTrade()로 체크해보고 trade가능하면 Trade()한다.
//-----------------------------------------------------------------------------

#ifndef __MTRADEMANAGER_H__
#define __MTRADEMANAGER_H__

#include "MInventory.h"
#include "MMoneyManager.h"

class MTradeManager {
	public :
		MTradeManager();
		~MTradeManager();

		//-------------------------------------------------------
		// Init / Release
		//-------------------------------------------------------
		void				Init();
		void				Release();

		//-------------------------------------------------------
		// inventory 얻기
		//-------------------------------------------------------
		MInventory*			GetMyInventory() const			{ return m_pMyInventory; }
		MInventory*			GetOtherInventory() const		{ return m_pOtherInventory; }

		//-------------------------------------------------------
		// money manager 얻기
		//-------------------------------------------------------
		MMoneyManager*		GetMyMoneyManager() const		{ return m_pMyMoney; }
		MMoneyManager*		GetOtherMoneyManager() const	{ return m_pOtherMoney; }

		//-------------------------------------------------------
		// Trade OK ?
		//-------------------------------------------------------
		bool				IsAcceptTime() const;
		void				SetNextAcceptTime();		// 다음 교환 가능한 시간 설정

		bool				IsAcceptMyTrade() const				{ return m_bAcceptMyTrade; }
		bool				IsAcceptOtherTrade() const			{ return m_bAcceptOtherTrade; }

		void				AcceptMyTrade()						{ m_bAcceptMyTrade = true; }
		void				AcceptOtherTrade()					{ m_bAcceptOtherTrade = true; }

		void				RefuseMyTrade();
		void				RefuseOtherTrade();

		//-------------------------------------------------------
		// Trade
		//-------------------------------------------------------
		bool				CanTrade() const;		// trade 가능한가?
		bool				Trade();				// 교환!
		bool				CancelTrade();			// 교환 거절

		//-------------------------------------------------------
		// 교환할려는 사람의 정보
		//-------------------------------------------------------
		void				SetOtherID(TYPE_OBJECTID otherID)	{ m_OtherID = otherID; }
		TYPE_OBJECTID		GetOtherID() const					{ return m_OtherID; }
		void				SetOtherName(const char* pName)		{ m_OtherName = pName; }
		const char*			GetOtherName() const				{ return m_OtherName.GetString(); }

	protected :
		TYPE_OBJECTID		m_OtherID;				// 다른 사람 ID
		MString				m_OtherName;			// 다른 사람 이름

		MInventory*			m_pMyInventory;			// 내꺼
		MInventory*			m_pOtherInventory;		// 남꺼

		MMoneyManager*		m_pMyMoney;				// 내 돈
		MMoneyManager*		m_pOtherMoney;			// 남 돈

		bool				m_bAcceptMyTrade;			// 나의 교환확인
		bool				m_bAcceptOtherTrade;		// 남의 교한확인

		DWORD				m_NextAcceptTime;		// OK 누를 수 있는 시간
};

extern MTradeManager*		g_pTradeManager;

#endif

