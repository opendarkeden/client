//----------------------------------------------------------------------
// MInventory.h
//----------------------------------------------------------------------
/*
	//--------------------------------------------------------
	// 가로 * 세로 = (5 * 4) 인 GridInventory 생성
	//--------------------------------------------------------
	g_Inventory.Init(5, 4);
	MItem* pNewItem;

	//--------------------------------------------------------
	// 테스트용 Item 생성
	//--------------------------------------------------------
	pNewItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pNewItem->SetID( 2 );
	pNewItem->SetItemType( 1 );
	g_Inventory.AddItem( pNewItem, 1, 0 );

	
	pNewItem = MItem::NewItem( ITEM_CLASS_POTION );
	pNewItem->SetID( 3 );
	pNewItem->SetItemType( 0 );
	g_Inventory.AddItem( pNewItem, 3, 1 );
	
	
	pNewItem = MItem::NewItem( ITEM_CLASS_SHOES );
	pNewItem->SetID( 1 );
	pNewItem->SetItemType( 0 );
	g_Inventory.AddItem( pNewItem );


	//--------------------------------------------------------
	// 심심해서 remove 해 봄... - -;;
	//--------------------------------------------------------
	g_Inventory.RemoveItem( 0, 0 );
	g_Inventory.RemoveItem( 1, 3 );
	g_Inventory.RemoveItem( 4, 2 );

	
	//--------------------------------------------------------
	// 출력방법1
	//--------------------------------------------------------
	// Item을 한번씩 출력
	//--------------------------------------------------------
	g_Inventory.SetBegin();
	while (g_Inventory.IsNotEnd())
	{
		const MItem* pItem = g_Inventory.Get();

		DEBUG_ADD_FORMAT("[ID=%d] size=(%d, %d) xy=(%d, %d) ", 
			pItem->GetID(),
			pItem->GetGridWidth(), pItem->GetGridHeight(),
			pItem->GetGridX(), pItem->GetGridY()
			);

		g_Inventory.Next();
	}


	//--------------------------------------------------------
	// 출력방법2
	//--------------------------------------------------------
	// 각 grid에 있는 Item들을 읽어서 출력
	//--------------------------------------------------------
	for (int i=0; i<g_Inventory.GetHeight(); i++)
	{
		for (int j=0; j<g_Inventory.GetWidth(); j++)
		{
			const MItem* pItem = g_Inventory.GetItem( j, i );

			if (pItem!=NULL)
			{
				DEBUG_ADD_FORMAT("[ID=%d] size=(%d, %d) xy=(%d, %d) ", 
											pItem->GetID(),
											pItem->GetGridWidth(), pItem->GetGridHeight(),
											pItem->GetGridX(), pItem->GetGridY()
											);
			}
		}
	}
*/
//----------------------------------------------------------------------

#ifndef	__MINVENTORY_H__
#define	__MINVENTORY_H__

#include "MGridItemManager.h"
#include "MItem.h"
#include <list>

/*
#ifdef __GAME_CLIENT__
	class MEffect;
#endif
	*/

class MInventory : public MGridItemManager {
	public :
		MInventory();
		~MInventory();

		//------------------------------------------------------
		// Add
		//------------------------------------------------------		
		bool			AddItem(MItem* pItem);					// 적절한 grid에 추가한다.
		bool			AddItem(MItem* pItem, BYTE x, BYTE y);	// grid(x,y)에 pItem을 추가한다.
	
		bool			ReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem);	// (x,y)위치의 Item교환
		
		//------------------------------------------------------		
		// Get FitPosition
		//------------------------------------------------------		
		bool			GetFitPosition(MItem* pItem, POINT& point);	// pItem이 들어갈 수 있는 적절한 grid위치를 구한다.

		//------------------------------------------------------
		// Find Item
		//------------------------------------------------------		
		MItem*			FindItem( ITEM_CLASS itemClass, TYPE_ITEMTYPE itemType=ITEMTYPE_NULL);

		//------------------------------------------------------
		// 현재 사용가능한건가?
		//------------------------------------------------------
		void			CheckAffectStatus();				// 모든 아이템
		void			CheckAffectStatus(MItem* pItem);	// 특정 아이템

		/*
	#ifdef __GAME_CLIENT__
		public :
			typedef std::list<MEffect*>						EFFECT_LIST;

		public :
			//------------------------------------------------------		
			// Effect
			//------------------------------------------------------
			bool			AddEffect(MEffect* pEffect);
			void			UpdateEffects();
			int				GetEffectCount()				{ return m_listEffect.size(); }
			EFFECT_LIST::const_iterator GetEffects()		{ return m_listEffect.begin(); }

		protected :
			EFFECT_LIST		m_listEffect;
	#endif
	*/

};

extern MInventory*		g_pInventory;



#endif