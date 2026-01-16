//--------------------------------------------------------------------------------
//
// Filename    : GCCreateItemHandler.cpp
// Written By  : elca
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCCreateItem.h"
#include "ClientDef.h"
#include "MInventory.h"
#include "UIFunction.h"
#include "VS_UI_GameCommon.h"
extern int g_C2G;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCCreateItemHandler::execute ( GCCreateItem * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	MItem* pItem;

	
	//---------------------------------------------------------------------
	// inventory에서 item몇개를 드는 경우..
	//---------------------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferPickupSomeFromInventory())
	{
		MItem* pItem = g_pPlayer->GetItemCheckBuffer();
		g_pPlayer->ClearItemCheckBuffer();		

		if (pItem!=NULL)
		{
			// 들어야 하는 item의 개수는
			// ( 원래 개수 - 바닥에 남은 개수 ) 이다.
			int numPickup = pItem->GetNumber() - pPacket->getItemNum();

			// inventory에서 제거한다.
			MItem* pRemovedItem = g_pInventory->RemoveItem( pItem->GetGridX(), pItem->GetGridY() );

			if (pRemovedItem==pItem)
			{
				// 개수를 다시 설정해준다.
				pItem->SetNumber( numPickup );

				// mouse에 든다.
				UI_PickUpItem( pItem );
			}
			else
			{
				DEBUG_ADD("[Error] PickupPart item Wrong");
			}
		}
		else
		{
			DEBUG_ADD("[Error] PickupPart item NULL");
		}
	}

	//---------------------------------------------------------------------
	// Item을 생성한다.
	//---------------------------------------------------------------------
	pItem = MItem::NewItem( (enum ITEM_CLASS)pPacket->getItemClass() );
	pItem->SetItemType( pPacket->getItemType() );
	pItem->SetItemOptionList( pPacket->getOptionType() );

	pItem->SetID( pPacket->getObjectID() );		

	pItem->SetCurrentDurability( pPacket->getDurability() );

	//pItem->SetNumber( pPacket->getItemNum() );

	pItem->SetGrade( pPacket->getGrade() );
	pItem->SetEnchantLevel( pPacket->getEnchantLevel() );

	//------------------------------------------
	// 개수
	//------------------------------------------
	// 총인 경우
	//------------------------------------------
	if (pItem->IsGunItem())
	{
		MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

		// 의미 없음 - -;
		pMagazine->SetID( 0 );

		// 이거는 총에 맞춰서 해줘야된다.
		for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
		{
			pMagazine->SetItemType(	j );

			if (pMagazine->IsInsertToItem( pItem ))
			{
				break;
			}
		}

		// 의미 없음
		pMagazine->ClearItemOption();
	
		// 탄창 개수
		pMagazine->SetNumber( pPacket->getItemNum() );

		//------------------------------------
		// 탄창 설정
		//------------------------------------
		MGunItem* pGunItem = (MGunItem*)pItem;
		pGunItem->SetMagazine( pMagazine );
	}		
	//------------------------------------------
	// 총이 아닌 경우
	//------------------------------------------
	else
	{
		pItem->SetNumber( pPacket->getItemNum() );
		//pItem->SetSilver( item.silver );
	}

	// 크리스마스 트리 합체
	if(pItem->GetItemClass() == ITEM_CLASS_EVENT_TREE && (pItem->GetItemType() == 12
		|| pItem->GetItemType() == 25 || pItem->GetItemType() == 41))
	{
		if(g_pPlayer->GetItemCheckBufferStatus() == MPlayer::ITEM_CHECK_BUFFER_TREE_MERGE)
		{
			for(int y = 0; y < 4; y++)
			{
				for(int x = 0; x < 3; x++)
				{
					MItem *pPartOfTree = g_pInventory->GetItem(x+pPacket->getInvenX(), y+pPacket->getInvenY());
					if(pPartOfTree != NULL && 
						pPartOfTree->GetItemClass() == ITEM_CLASS_EVENT_TREE &&
						( pPartOfTree->GetItemType() == y*3+x ||
						pPartOfTree->GetItemType() == y*3+x + 13 ||
						pPartOfTree->GetItemType() == y*3+x + 29))
					{
						g_pInventory->RemoveItem(x+pPacket->getInvenX(), y+pPacket->getInvenY());
						delete pPartOfTree;
					}
				}
			}
			g_pPlayer->ClearItemCheckBuffer();
		}
		else
		{
			DEBUG_ADD("[GCCreateItemHandler] ItemCheckBuffer != ITEM_CHECK_BUFFER_TREE_MERGE");
		}
	}

	//---------------------------------------------------------------------
	// inventory에 추가한다.
	//---------------------------------------------------------------------
	if (g_pInventory->AddItem( pItem, pPacket->getInvenX(), pPacket->getInvenY() ))
	{
		if( g_C2G > 0 )
		{
			C_VS_UI_INVENTORY::AutoMove( pPacket->getInvenX(), pPacket->getInvenY() );
			gpC_base->DispatchMessage();
			gpC_base->DispatchMessage();
			gpC_base->DispatchMessage();
			gpC_base->DispatchMessage();
			gpC_base->DispatchMessage();
			gpC_base->DispatchMessage();
			g_C2G--;
		}
		// 제대로 추가된 경우..
	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] Cannot Create Item to inventory: id=%d, tp=%d, xy=(%d,%d)", (int)pItem->GetID(), (int)pItem->GetItemType(), (int)pPacket->getInvenX(), (int)pPacket->getInvenY());
		
		delete pItem;
	}


#endif

	__END_CATCH
}
