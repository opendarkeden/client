//----------------------------------------------------------------------
// MInventory.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MItem.h"
#include "MInventory.h"

#include "MHelpManager.h"
#ifdef __GAME_CLIENT__
	#include "ClientFunction.h"
	#include "MPlayer.h"
#endif

/*
#ifdef __GAME_CLIENT__
	#include "MEffect.h"
	#include "MEffectGeneratorTable.h"
#endif
	*/

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MInventory*		g_pInventory = NULL;

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MInventory::MInventory()
{
}

MInventory::~MInventory()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Check AffectStatus
//----------------------------------------------------------------------
// 사용가능한지 체크
//----------------------------------------------------------------------
void			
MInventory::CheckAffectStatus(MItem* pItem)	// 특정 아이템
{
#ifdef __GAME_CLIENT__

	g_pPlayer->CheckAffectStatus( pItem );
		
#endif
}

//----------------------------------------------------------------------
// Add item ( pItem )
//----------------------------------------------------------------------
// Inventory에 추가될 수 있는 Item인지 보고,..
// 적절한 grid에 추가한다.
//----------------------------------------------------------------------
bool			
MInventory::AddItem(MItem* pItem)
{
	if (pItem->IsInventoryItem())
	{
		if (MGridItemManager::AddItem( pItem ))
		{
			// 제대로 추가된 경우 --> sound출력
			#ifdef __GAME_CLIENT__
				PlaySound( pItem->GetInventorySoundID() );
			#endif
		
						
			return true;
		}

		return false;
	}

	return false;
}

//----------------------------------------------------------------------
// Add item ( pItem )
//----------------------------------------------------------------------
// Inventory에 추가될 수 있는 Item인지 보고,..
// grid(x,y)에 pItem을 추가한다.
//----------------------------------------------------------------------
bool			
MInventory::AddItem(MItem* pItem, BYTE x, BYTE y)
{
	if (pItem->IsInventoryItem())
	{
		if (MGridItemManager::AddItem( pItem, x, y ))
		{
			// 제대로 추가된 경우 --> sound출력
			#ifdef __GAME_CLIENT__
				PlaySound( pItem->GetInventorySoundID() );
			#endif

			return true;
		}

		return false;
	}

	return false;
}

//----------------------------------------------------------------------
// Replace item ( pItem,  x,y,  pOldItem )
//----------------------------------------------------------------------
// Inventory에 추가될 수 있는 Item인지 보고,..
// 추가될 수 있으면 추가하는데
// 그 위치에 다른 item이 있다면 pOldItem에 담아서 넘겨준다.
//----------------------------------------------------------------------
bool			
MInventory::ReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem)
{
	if (pItem->IsInventoryItem())
	{
		if (MGridItemManager::ReplaceItem(pItem, x,y, pOldItem))
		{
			// 제대로 추가된 경우 --> sound출력
			#ifdef __GAME_CLIENT__
				PlaySound( pItem->GetInventorySoundID() );
			#endif

			return true;
		}
		
		return false;
	}

	return false;
}

//----------------------------------------------------------------------
// Get Fit Position ( pItem, point )
//----------------------------------------------------------------------
// pItem이 들어갈 수 있는 적절한 grid위치를 구한다.
//
// 일단, 쌓여서 중복될 수 있는 Item인 경우를 체크해야한다.
// 개수가 한계치를 넘어서 완전 쌓이는게 불가능할 경우는 빈 공간을
// 찾으면 된다.
//----------------------------------------------------------------------
bool			
MInventory::GetFitPosition(MItem* pItem, POINT& point)
{
	//--------------------------------------------------------
	// 쌓이는 item인 경우만 체크한다.
	//--------------------------------------------------------
	if (pItem->IsPileItem())
	{
		// 모든 Item을 체크하면서 쌓일 수 있는지를 체크한다.
		// 찾는 순서는?? -_-;;
		// ID순.. 흠.. -_-;;;
		ITEM_MAP::iterator iItem = m_mapItem.begin();

		while (iItem != m_mapItem.end())
		{
			MItem* pInventoryItem = (*iItem).second;

			//--------------------------------------------
			// 완전하게 쌓일 수 있는 조건이 되면...
			// 기존의 item의 좌표를 넘겨준다.
			//--------------------------------------------
			if (pInventoryItem->GetItemClass()==pItem->GetItemClass()
				&& pInventoryItem->GetItemType()==pItem->GetItemType()
				&& pInventoryItem->GetNumber() + pItem->GetNumber() <= pItem->GetMaxNumber()
				&& !pInventoryItem->IsQuestItem())
			{
				point.x = pInventoryItem->GetGridX();
				point.y = pInventoryItem->GetGridY();

				return true;
			}

			iItem++;
		}
	}

	return MGridItemManager::GetFitPosition( pItem, point );
}

//----------------------------------------------------------------------
// Find Item  (class , type)
//----------------------------------------------------------------------
// inventory에서 적절한 itemClass와 itemType을 가진 item을 하나 찾는다.
// itemType은 지정안 할 수도 있다.
// 그리고, 하나만 찾으면 되므로... 가장 먼저 발견되는걸 넘겨주면 된다.
//----------------------------------------------------------------------
MItem*	
MInventory::FindItem( ITEM_CLASS itemClass, TYPE_ITEMTYPE itemType )
{
	ITEM_MAP::iterator iItem = m_mapItem.begin();

	//------------------------------------------------------
	// itemType은 지정하지 않은 경우
	//------------------------------------------------------
	if (itemType==ITEMTYPE_NULL)
	{
		while (iItem != m_mapItem.end())
		{
			MItem* pItem = (*iItem).second;

			// class만 비교
			if (pItem->GetItemClass()==itemClass)
			{
				return pItem;
			}

			iItem++;
		}
	}
	//------------------------------------------------------
	// class와 type 모두 비교
	//------------------------------------------------------
	else
	{
		while (iItem != m_mapItem.end())
		{
			MItem* pItem = (*iItem).second;

			// class와 type 모두 비교
			if (pItem->GetItemClass()==itemClass
				&& pItem->GetItemType()==itemType)
			{
				return pItem;
			}

			iItem++;
		}
	}

	return NULL;
}

/*
#ifdef __GAME_CLIENT__
	//----------------------------------------------------------------------
	// Add Effect
	//----------------------------------------------------------------------
	bool			
	MInventory::AddEffect(MEffect* pEffect)
	{
		if (pEffect==NULL)
		{
			return false;
		}

		m_listEffect.push_back( pEffect );

		return true;
	}

	//----------------------------------------------------------------------
	// Update Effects
	//----------------------------------------------------------------------
	void			
	MInventory::UpdateEffects()
	{
		EFFECT_LIST::iterator iEffect = m_listEffect.begin();

		EFFECT_LIST::iterator iTemp;
		
		MEffect* pEffect;
		int count = m_listEffect.size();

		for (int i=0; i<count; i++)	
		{
			pEffect = *iEffect;

			//---------------------------------------
			//
			// update --> 정상적으로 된 경우
			//
			//---------------------------------------
			if (pEffect->Update())
			{
				//-----------------------------------------------
				//
				// 이 Effect가 끝나기 전에 LinkCount에 의해서
				// 다음 연결되는 Effect가 있으면 생성해야 한다.
				//
				// 현재Frame이 EndLinkFrame을 넘어간 경우
				//
				//-----------------------------------------------
				if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
					&& pEffect->GetLinkSize() != 0)
				{
					// GenerateNext에서 
					// pEffect의 EffectTarget을 NULL로 만들어주기 때문에
					// 여기서 지울 필요 없다.
					g_EffectGeneratorTable.GenerateNext( pEffect );

					// pEffect는 여전히 존재해야 하므로 지우면 안된다.
				}
				
				
				iEffect++;
			}
			//---------------------------------------
			//
			// 다 돼서 끝난 경우
			//
			//---------------------------------------
			else
			{
				//-----------------------------------------------
				// 다음 연결되는 Effect가 있으면 생성해야 한다.
				//-----------------------------------------------
				if (pEffect->GetLinkSize() != 0)
				{
					g_EffectGeneratorTable.GenerateNext( pEffect );
				}

				//-----------------------------------------------
				// 제거
				//-----------------------------------------------
				delete pEffect;

				iTemp = iEffect;
				iEffect++;

				// list에서 제거한다.
				m_listEffect.erase(iTemp);
			}
		
		}
	}
#endif
	*/

