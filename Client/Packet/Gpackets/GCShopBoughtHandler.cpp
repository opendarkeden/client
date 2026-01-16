//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopBoughtHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopBought.h"
#include "ClientDef.h"
#include "MNPC.h"
#include "MShop.h"
#include "MShopShelf.h"

void GCShopBoughtHandler::execute ( GCShopBought * pPacket , Player * pPlayer )
	 

throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	// shop에 아이템 등장
	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		//------------------------------------------------------
		// 그런 creature가 없는 경우
		//------------------------------------------------------
		if (pCreature==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] There is no such Creature id=%d", pPacket->getObjectID());
		}
		//------------------------------------------------------
		// NPC인 경우
		//------------------------------------------------------
		else if (pCreature->GetClassType()==MCreature::CLASS_NPC)
		{
			MNPC* pNPC = (MNPC*)pCreature;

			MShop* pShop = pNPC->GetShop();

			if (pShop==NULL)
			{
				//------------------------------------------------------
				// shop이 없다면..
				// item을 추가 시킬 필요가 없다.
				//------------------------------------------------------
			}
			else
			{
				//------------------------------------------------------
				// 새로운 아이템을 생성해서 추가해야 한다.
				//------------------------------------------------------
				MShopShelf* pShopShelf = pShop->GetShelf( pPacket->getShopType() );

				if (pShopShelf==NULL)
				{
					DEBUG_ADD_FORMAT("[Error] There is no Shelf type=%d", (int)pPacket->getShopType());
				}
				else
				{
					// version 수정
					pShopShelf->SetVersion( pPacket->getShopVersion() );

					// 새로운 item 생성
					MItem* pItem = MItem::NewItem( (ITEM_CLASS)pPacket->getItemClass() );

					pItem->SetID( pPacket->getItemObjectID() );
					pItem->SetItemType( pPacket->getItemType() );
					pItem->SetItemOptionList( pPacket->getOptionType() );
					pItem->SetCurrentDurability( pPacket->getDurability() );
					pItem->SetSilver( pPacket->getSilver() );
					pItem->SetGrade( pPacket->getGrade() );
					pItem->SetEnchantLevel( pPacket->getEnchantLevel() );

					// 생성된 item을 shop에 추가
					if (!pShopShelf->SetItem( pPacket->getShopIndex(), pItem ))
					{
						DEBUG_ADD("[Error] Cannot Add Item to ShopShelf");
						
						delete pItem;
					}
				}
			}

		}
		//------------------------------------------------------
		// NPC가 아닌 경우
		//------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[Error] The Creature is Not NPC. id=%d", pPacket->getObjectID());
		}
	}
	


#endif

	__END_CATCH
}
