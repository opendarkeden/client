//----------------------------------------------------------------------
// MItemManager.h
//----------------------------------------------------------------------
// MItem에 pointer를 저장하고 다루는 class이다.
// stl의 map을 이용한다.
// 
//		AddItem()로 추가하고
//
//		GetItem()으로 정보를 얻고
//
//		RemoveItem()로 제거한다.
// 
// pointer만 저장하고 있기 때문에.. new / delete 는 외부에서 해줘야 한다.
//----------------------------------------------------------------------

#ifndef	__MITEMMANAGER_H__
#define	__MITEMMANAGER_H__

#pragma warning(disable:4786)

#include "MTypeDef.h"
#include <map>
class MItem;
class MItemFinder;

//----------------------------------------------------------------------
// MItemManager
//----------------------------------------------------------------------
class MItemManager {
	public :
		typedef	std::map<TYPE_OBJECTID, MItem*>		ITEM_MAP;

	public :
		MItemManager();
		~MItemManager();

		//------------------------------------------------------
		// Release
		//------------------------------------------------------
		void			Release();

		//------------------------------------------------------
		// Add / Get / Remove
		//------------------------------------------------------
		bool			AddItem(MItem* pItem);				// Item 추가
		MItem*			GetItem(TYPE_OBJECTID id) const;	// Item 읽어오기
		MItem*			RemoveItem(TYPE_OBJECTID id);		// ItemManager에서 Item 제거

		//------------------------------------------------------
		// Iterator
		//------------------------------------------------------
		int				GetItemNum()		{ return m_mapItem.size(); }
		void			SetBegin()			{ m_iterator = m_mapItem.begin(); }
		MItem*			Get()				{ return (*m_iterator).second; }
		void			Next()				{ m_iterator++; }		
		bool			IsEnd() const		{ return m_iterator==m_mapItem.end(); }
		bool			IsNotEnd() const	{ return m_iterator!=m_mapItem.end(); }

	//protected :
		MItem*			GetItemToModify(TYPE_OBJECTID id);		
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		MItem*			GetItemToModifyAll(TYPE_OBJECTID id , MItem*& pSubInventoryItem);
	#endif
				

		//------------------------------------------------------
		// 사용가능한지 체크한다.
		//------------------------------------------------------
		void			CheckAffectStatusAll();
		virtual void	CheckAffectStatus(MItem* pItem);

		//------------------------------------------------------
		// 아이템 찾기
		//------------------------------------------------------
		MItem*			FindItem( MItemFinder& itemFinder ) const;
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		MItem*			FindItemAll( MItemFinder& itemFinder, MItem*& pSubInventoryItem) const;
	#endif
		
		
	protected :
		ITEM_MAP::const_iterator	m_iterator;
		ITEM_MAP					m_mapItem;
};

#endif

