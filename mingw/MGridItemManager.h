//----------------------------------------------------------------------
// MGridItemManager.h
//----------------------------------------------------------------------
// width * height 만큼의 grid를 가진다.
//
// 각 grid는 MItem*로 이루어져 있다.
//
// MItem의 크기(GridWidth()*GridHeight)에 따라서
// 적절한 개수의 grid를 차지하게 된다.
//
// Item마다 MItemManager의 map에도 추가하고
// grid의 크기만큼 m_ItemGrid에도 각각의 grid에 표시해준다.
//----------------------------------------------------------------------
/*
 = Init(9, 5)

	   012345678
	  +---------+
	0 |         |
	1 |         |
	2 |         |
	3 |         |
	4 |         |
	  +---------+


 = AddItem( 크기가 2x2인것, (1,1)에 )

  	   012345678
	  +---------+
	0 |         |
	1 | AA      |
	2 | AA      |
	3 |         |
	4 |         |
	  +---------+


 = AddItem( 크기가 3x2인 것 ) - 적절한 위치에 추가

   	   012345678
	  +---------+
	0 |   BBB   |
	1 | AABBB   |
	2 | AA      |
	3 |         |
	4 |         |
	  +---------+


 = GetItem( 2,2 ) --> A
 = GetItem( 4,1 ) --> B



*/
//----------------------------------------------------------------------

#ifndef	__MGridItemManager_H__
#define	__MGridItemManager_H__

#include "MItemManager.h"


class MGridItemManager : public MItemManager {
	public :
		MGridItemManager();
		~MGridItemManager();

		//------------------------------------------------------
		// Init / Release
		//------------------------------------------------------
		void		Init(BYTE width, BYTE height);
		void		Release();

		//------------------------------------------------------
		// get
		//------------------------------------------------------
		BYTE		GetWidth() const	{ return m_Width; }
		BYTE		GetHeight() const	{ return m_Height; }
		
		//------------------------------------------------------
		// Add / Get / Remove
		//------------------------------------------------------		
		bool			AddItem(MItem* pItem);					// 적절한 grid에 추가한다.
		bool			AddItem(MItem* pItem, BYTE x, BYTE y);	// grid(x,y)에 pItem을 추가한다.

		MItem*			GetItem(BYTE x, BYTE y) const;			// grid(x,y)의 item을 읽어온다.
		MItem*			GetItem(TYPE_OBJECTID id) const;
		
		MItem*			RemoveItem(BYTE x, BYTE y);				// grid(x,y)의 item을 제거한다.
		MItem*			RemoveItem(TYPE_OBJECTID id);			// id가 같은 item을 제거한다.

		bool			ReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);	// (x,y)위치의 Item교환

		//------------------------------------------------------		
		// Can Replace Item (실제는 Add or Replace를 check한다.)
		//------------------------------------------------------		
		bool			CanReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);

		//------------------------------------------------------		
		// Get FitPosition
		//------------------------------------------------------		
		bool			GetFitPosition(MItem* pItem, POINT& point);	// pItem이 들어갈 수 있는 적절한 grid위치를 구한다.

		//------------------------------------------------------		
		// FindItemGridOrder
		//------------------------------------------------------		
		MItem*			FindItemGridOrder( MItemFinder& itemFinder ) const;

	protected :
		BYTE			m_Width;
		BYTE			m_Height;
		MItem***		m_ItemGrid;
};

#endif

