//----------------------------------------------------------------------
// MPlayerGear.cpp
//----------------------------------------------------------------------
// Player의 Gear에 대한 class
//----------------------------------------------------------------------

#ifndef __MPLAYERGEAR_H__
#define __MPLAYERGEAR_H__

#include "MSlotItemManager.h"

class MPlayerGear : public MSlotItemManager {
	public :
		enum ITEM_STATUS
		{
			ITEM_STATUS_OK,					// 좋은 상태
			ITEM_STATUS_SOMEWHAT_BROKEN,		// 약간 부서짐 - -;
			ITEM_STATUS_ALMOST_BROKEN		// 많이 부서짐
		};

	public :
		MPlayerGear();
		~MPlayerGear();

		//------------------------------------------------------
		// Init / Release
		//------------------------------------------------------
		void			Init(BYTE size);
		void			Release();

		//------------------------------------------------------
		// Add / Get / Remove
		//------------------------------------------------------		
		bool			AddItem(MItem* pItem, BYTE n);		// slot(n)에 pItem을 추가한다.

		MItem*			GetItem(BYTE n) const;				// slot(n)의 item을 읽어온다.
		
		MItem*			RemoveItem(BYTE n);					// slot(n)의 item을 제거한다.
		MItem*			RemoveItem(TYPE_OBJECTID id);		// id가 같은 item을 제거한다.

		bool			ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);	// (n)위치의 Item교환

		//------------------------------------------------------
		// Item 변화
		//------------------------------------------------------
		bool			ModifyDurability(BYTE n, int changeValue);

		//------------------------------------------------------
		// 부서질려는 아이템이 있는가?
		//------------------------------------------------------
		ITEM_STATUS		GetItemStatus(int slot) const	{ return m_pItemStatus[slot]; }
		BOOL			HasBrokenItem() const			{ return m_nBrokenItem!=0; }
		void			CheckItemStatusAll();

	protected :
		//virtual void	CheckItemStatusAll() = 0;	// 필요 없을 듯..
		void			SetItemStatusOK(int slot);	// slot번째는 굿~으로 만든다.
		void			CheckItemStatus(const MItem* pItem, int slot);
		
	protected :
		int				m_nBrokenItem;	// 부서진 item의 개수
		ITEM_STATUS*	m_pItemStatus;
};

#endif

