//----------------------------------------------------------------------
// MItemFinder.h
//----------------------------------------------------------------------
// 특별한 item을 찾기 위해 쓰인다.
// MItemFinder를 상속받아서 필요한걸 만들면 된다.
//
// 이제서야 필요를 느껴 만들게 되었당.. --;
// 예전 코드들은 그냥 그케 쓰도록 둬야지.. - -;;
//----------------------------------------------------------------------

#ifndef __MITEMFINDER_H__
#define __MITEMFINDER_H__

#include "MItem.h"

//----------------------------------------------------------------------
// MItemFinder
//----------------------------------------------------------------------
class MItemFinder {
	public :
		virtual bool operator () ( const MItem* pItem ) = 0;
};

//----------------------------------------------------------------------
// MItemClassFinder
//----------------------------------------------------------------------
// 같은 ItemClass의 item을 찾는다.
//----------------------------------------------------------------------
class MItemClassFinder : public MItemFinder {
	public :
		MItemClassFinder(ITEM_CLASS itemClass)		{ m_ItemClass = itemClass; }
		
		void	SetItemClass(ITEM_CLASS itemClass)	{ m_ItemClass = itemClass; }

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==m_ItemClass;
		}

	protected :
		ITEM_CLASS	m_ItemClass;	// 찾을려는 itemClass
};

//----------------------------------------------------------------------
// MItemClassTypeFinder
//----------------------------------------------------------------------
// 같은 ItemClass, ItemType의 item을 찾는다.
//----------------------------------------------------------------------
class MItemClassTypeFinder : public MItemClassFinder {
	public :
		MItemClassTypeFinder(ITEM_CLASS itemClass, TYPE_ITEMTYPE itemType) : MItemClassFinder(itemClass)
		{ 
			m_ItemType = itemType;
		}
		
		void	SetItemType(TYPE_ITEMTYPE itemType)	{ m_ItemType = itemType; }

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==m_ItemClass
					&& pItem->GetItemType()==m_ItemType;
		}

	protected :
		TYPE_ITEMTYPE	m_ItemType;	// 찾을려는 itemType
};

//----------------------------------------------------------------------
// MVampirePortalItemFinder
//----------------------------------------------------------------------
// Mark되거나 안된 VampirePortal을 찾기 위해서
//----------------------------------------------------------------------
class MVampirePortalItemFinder : public MItemFinder {
	public :
		MVampirePortalItemFinder(bool bMarked)
		{
			m_bMarked = bMarked;
		}

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==ITEM_CLASS_VAMPIRE_PORTAL_ITEM
					&& ((MVampirePortalItem*)pItem)->IsMarked()==m_bMarked;
		}

	protected :
		bool	m_bMarked;
};

//----------------------------------------------------------------------
// MSlayerPortalItemFinder
//----------------------------------------------------------------------
// 사용할 수 있는걸 찾는다.
//----------------------------------------------------------------------
class MSlayerPortalItemFinder : public MItemFinder {
	public :
		MSlayerPortalItemFinder() {}

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==ITEM_CLASS_SLAYER_PORTAL_ITEM
					&& pItem->GetNumber() > 0;
		}
};

//----------------------------------------------------------------------
// MEventStartItemFinder
//----------------------------------------------------------------------
// 특정한 class, type의 item이 몇개나 있는가..
class MItemClassTypeNumberFinder : public MItemClassTypeFinder {
	public :
		MItemClassTypeNumberFinder(ITEM_CLASS itemClass, TYPE_ITEMTYPE itemType) 
			: MItemClassTypeFinder(itemClass, itemType)	
		{
			m_TotalNumber = 0;
		}		
	
		bool	operator () ( const MItem* pItem )
		{
			// 같은 class, type이면 개수를 더한다.
			if (pItem->GetItemClass()==m_ItemClass
					&& pItem->GetItemType()==m_ItemType)
			{
				m_TotalNumber += pItem->GetNumber();
			}

			// 계속 찾게 할려는 목적이다.
			return false;
		}

		void	Clear()						{ m_TotalNumber = 0; }
		int		GetTotalNumber()	const	{ return m_TotalNumber; }


	protected :
		int				m_TotalNumber;
};

//----------------------------------------------------------------------
// MItemClassRangeTypeFinder
//----------------------------------------------------------------------
// 같은 ItemClass, min~max ItemType의 item을 찾는다.
//----------------------------------------------------------------------
class MItemClassRangeTypeFinder : public MItemClassFinder {
	public :
		MItemClassRangeTypeFinder(ITEM_CLASS itemClass, TYPE_ITEMTYPE minItemType, TYPE_ITEMTYPE maxItemType) : MItemClassFinder(itemClass)
		{ 
			m_MinItemType = minItemType;
			m_MaxItemType = maxItemType;
		}
		
		void	SetItemType(TYPE_ITEMTYPE minItemType, TYPE_ITEMTYPE maxItemType)	{ m_MinItemType = minItemType;	m_MaxItemType = maxItemType; }

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==m_ItemClass
					&& pItem->GetItemType()>=m_MinItemType
					&& pItem->GetItemType()<=m_MaxItemType;
		}

	protected :
		TYPE_ITEMTYPE	m_MinItemType;	// 찾을려는 min itemType
		TYPE_ITEMTYPE	m_MaxItemType;	// 찾을려는 max itemType
};

//----------------------------------------------------------------------
// MOustersSummonGemItemFinder 
//----------------------------------------------------------------------
//----------------------------------------------------------------------
class MOustersSummonGemItemFinder : public MItemFinder {
	public :
		MOustersSummonGemItemFinder() {}

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==ITEM_CLASS_OUSTERS_SUMMON_ITEM
					&& pItem->GetNumber() > 0;
		}
};

//----------------------------------------------------------------------
// MEventGiftBoxItemFinder 
//----------------------------------------------------------------------
//----------------------------------------------------------------------
class MEventGiftBoxItemFinder : public MItemFinder {
	public :
		MEventGiftBoxItemFinder() {}

		bool	operator () ( const MItem* pItem )
		{
			return pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX
					&& pItem->GetItemType() > 1 && pItem->IsTrade();
		}
};


#endif