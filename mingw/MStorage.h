//----------------------------------------------------------------------
// MStorage.h
//----------------------------------------------------------------------
// 
// 보관함이다.
//
// current를 설정해서 현재에 사용중인 함~을 설정할 수 있다.
//----------------------------------------------------------------------
/*

	= 보관함이 3개가 있다고 가정하고... 

	MStorage*	pStorage = ....;


	[ 보관함 바꿀 때 ]

		pStorage->SetCurrent( n );		// n번째 보관함에 접근한다.


	[ 현재 접근중인 보관함번호 알아내기 ]

		int n = pStorage->GetCurrent();


	[ 접근중인 보관함의 slot에 있는 item 읽어오기 ]
	
		const MItem* pItem = pStorage->GetItem( slot );

*/
//----------------------------------------------------------------------

#ifndef __MSTORAGE_H__
#define	__MSTORAGE_H__

#include "MItem.h"
#include "MGameDef.h"
#include "MMoneyManager.h"

class MStorage {
	public :
		MStorage();
		~MStorage();

		//-------------------------------------------------------
		// Init / Release
		//-------------------------------------------------------
		void				Init(unsigned int size);
		void				Release();

		//-------------------------------------------------------		
		// Active - 보관함이 떠있는 상태인가?
		//-------------------------------------------------------		
		bool				IsActive() const	{ return m_bActive; }
		void				SetActive()			{ m_bActive = true; }
		void				UnSetActive()		{ m_bActive = false; }
	
		//-------------------------------------------------------		
		// Size
		//-------------------------------------------------------		
		unsigned int		GetSize() const		{ return m_Size; }

		//-------------------------------------------------------
		// Item
		//-------------------------------------------------------		
		bool				SetItem(unsigned int slot, MItem* pItem);
		MItem*				RemoveItem(unsigned int slot);
		const MItem*		GetItem(unsigned int slot) const;

		//-------------------------------------------------------		
		// 현재 접근 중인 shelf
		//-------------------------------------------------------		
		void				SetCurrent(unsigned int n);
		unsigned int		GetCurrent() const			{ return m_CurrentStorage; }		

		void				SetPersnalMessage(char * str) { m_pPersnalMsg = str;}
		char *				GetPersnalMessage() { return m_pPersnalMsg; }		

		void				SetCuropenid(DWORD id) { m_curopenid = id;}
		DWORD				GetCuropenid() { return m_curopenid; }		


		//-------------------------------------------------------		
		// Money
		//-------------------------------------------------------		
		MMoneyManager*		GetMoneyManager() const		{ return m_pMoneyManager; }

	protected :
		bool				m_bActive;		// 보관함 작동중인가?

		unsigned int		m_Size;			// 전체 보관함 개수

		MItem***			m_pStorage;		// item보관함

		// 현재 사용중인 storage
		unsigned int		m_CurrentStorage;

		// 돈
		MMoneyManager*		m_pMoneyManager;
		char		*	    m_pPersnalMsg;
		DWORD				m_curopenid;
};

extern MStorage*		g_pStorage;
extern MStorage*		g_pStorage2;
extern MStorage*		g_pStorageTemp;

#endif


