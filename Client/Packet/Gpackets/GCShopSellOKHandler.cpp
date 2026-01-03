//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopSellOKHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopSellOK.h"
#include "ClientDef.h"
#include "MInventory.h"
#include "TempInformation.h"
#include "MMoneyManager.h"
#include "UIFunction.h"
#include "MStorage.h"
#include "Client.h"
// PacketFunction.cpp에 있다. compile 시간 관계상..
void	CheckItemForSkillIcon(const MItem* pItem);

void GCShopSellOKHandler::execute ( GCShopSellOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	//--------------------------------------------------------------
	//
	// Item 파는 packet을 받는게 맞나?
	//
	//--------------------------------------------------------------

	if(pPacket->getShopVersion() == -1 && g_pStorage2!=NULL)
	{
		int current_storage = g_pStorage2->GetCurrent();
		const int storage_size = g_pStorage2->GetSize();
		const int storage_max = STORAGE_SLOT;
		
		for(int j = 0; j < storage_size; j++)
		{
			g_pStorage2->SetCurrent(0);
			 
			for(int i = 0; i < storage_max; i++)
			{
				const MItem *p_slot_item = g_pStorage2->GetItem(i);
				
				// 슬랏이 비었으면 걍~ 넣는다
				if(NULL != p_slot_item)
				{
					if(p_slot_item->GetID() == pPacket->getObjectID())
					{ 
						MItem* pRemovedItem = g_pStorage2->RemoveItem( i );
						SAFE_DELETE(pRemovedItem);
						break;
					}
				}
			}
			current_storage ++;
			if(current_storage >= storage_size)
				current_storage = 0;
		}

		// inventory에서 제거
		MItem *TempItem = g_pInventory->RemoveItem( (TYPE_OBJECTID)pPacket->getItemObjectID() );
		// item정보 제거
		UI_RemoveDescriptor( (void*)TempItem );
		// memory에서 제거
		SAFE_DELETE(TempItem);

		g_pMoneyManager->AddMoney(pPacket->getPrice());
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		UI_UnlockItemTrade();
		return;
	
	}
	if(pPacket->getShopVersion() == -1 && g_pStorage2==NULL)
	{
		MItem *TempItem = g_pInventory->RemoveItem( (TYPE_OBJECTID)pPacket->getItemObjectID() );
		UI_RemoveDescriptor( (void*)TempItem );
		// memory에서 제거
		SAFE_DELETE(TempItem);

		g_pMoneyManager->AddMoney(pPacket->getPrice());
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		UI_UnlockItemTrade();
		return;

	}

	if (g_pTempInformation->GetMode() == TempInformation::MODE_SHOP_SELL)
	{
		MItem* pCheckItem = (MItem*)g_pTempInformation->pValue;		
		
		const MItem* pItem = g_pInventory->GetItem( pCheckItem->GetGridX(), pCheckItem->GetGridY() );

		// ID 검증을 한다.
		if (pCheckItem->GetID()==pItem->GetID())
		{
			// ShopVersion_t getShopVersion(); -_-;;

			MItem* pRemovedItem = g_pInventory->RemoveItem( (TYPE_OBJECTID)pItem->GetID() );

			if (pRemovedItem!=NULL)
			{
				PlaySound( pRemovedItem->GetInventorySoundID() );

				CheckItemForSkillIcon( pRemovedItem );

				// item정보 제거
				UI_RemoveDescriptor( (void*)pRemovedItem );

				delete pRemovedItem;
			}
			else
			{
				DEBUG_ADD("[Error] Item is NULL");
			}

			//--------------------------------------------------------------
			// 돈을 바꿔준다.
			//--------------------------------------------------------------
			if (!g_pMoneyManager->SetMoney( pPacket->getPrice() ))
			{
				DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), pPacket->getPrice());
			}
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] Different ID. Packet(%d)!=ClientTemp(%d)", pPacket->getItemObjectID(), pItem->GetID());
		}

		// mode를 없앤다.
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		
		// 거래를 다시 활성화한다.
		UI_UnlockItemTrade();
	}
	//--------------------------------------------------------------
	//
	// 모든 해골을 다 파는 경우
	//
	//--------------------------------------------------------------
	else if (g_pTempInformation->GetMode() == TempInformation::MODE_SHOP_SELL_ALL_SKULL)
	{
		MItem* pSkull = g_pInventory->FindItem( ITEM_CLASS_SKULL );

		//--------------------------------------------------------------
		// 소리 한번만 내준다.
		//--------------------------------------------------------------
		if (pSkull!=NULL)
		{
			PlaySound( pSkull->GetInventorySoundID() );
		}				

		//--------------------------------------------------------------
		// 모든 해골을 다 없애준다.
		//--------------------------------------------------------------
		while (pSkull!=NULL)
		{
			MItem* pRemovedItem = g_pInventory->RemoveItem( (TYPE_OBJECTID)pSkull->GetID() );

			if (pRemovedItem!=NULL)
			{
				// item정보 제거
				UI_RemoveDescriptor( (void*)pRemovedItem );

				delete pRemovedItem;
			}

			//--------------------------------------------------------------
			// 해골이 또 있는지 검사..
			//--------------------------------------------------------------
			pSkull = g_pInventory->FindItem( ITEM_CLASS_SKULL );
		}

		//--------------------------------------------------------------
		// 돈을 바꿔준다.
		//--------------------------------------------------------------
		if (!g_pMoneyManager->SetMoney( pPacket->getPrice() ))
		{
			DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), pPacket->getPrice());
		}

		// mode를 없앤다.
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);

		// 거래를 다시 활성화한다.
		UI_UnlockItemTrade();
	}	
	//--------------------------------------------------------------
	//
	// 뭔가 잘못된 경우
	//
	//--------------------------------------------------------------
	else
	{
		DEBUG_ADD("[Error] Sell? -_-; No Temp Information!");
	}
#endif

	__END_CATCH
}
