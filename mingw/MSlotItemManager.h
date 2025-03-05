//----------------------------------------------------------------------
// MSlotItemManager.h
//----------------------------------------------------------------------
// 특정한 Slot 여러개~~에 MItem을 장착~해서 사용할 수 있게 한다.
//
// 한 Slot에는 하나의 Item이 들어갈 수 있다.
//----------------------------------------------------------------------

#ifndef	__MSLOTITEMMANAGER_H__
#define	__MSLOTITEMMANAGER_H__


#include "MItemManager.h"


class MSlotItemManager : public MItemManager {
	public :
		MSlotItemManager();
		~MSlotItemManager();

		//------------------------------------------------------
		// Init / Release
		//------------------------------------------------------
		void		Init(BYTE size);
		void		Release();

		//------------------------------------------------------
		// get
		//------------------------------------------------------
		BYTE		GetSize() const	{ return m_Size; }
	
		//------------------------------------------------------
		// Add / Get / Remove
		//------------------------------------------------------		
		bool			AddItem(MItem* pItem, BYTE n);		// slot(n)에 pItem을 추가한다.

		MItem*			GetItem(BYTE n) const;				// slot(n)의 item을 읽어온다.
		
		MItem*			RemoveItem(BYTE n);					// slot(n)의 item을 제거한다.
		MItem*			RemoveItem(TYPE_OBJECTID id);		// id가 같은 item을 제거한다.

		bool			ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);	// (n)위치의 Item교환

		bool			CanReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);


	protected :
		BYTE			m_Size;
		MItem**			m_ItemSlot;
};

#endif


