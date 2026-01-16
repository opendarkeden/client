//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopListHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopList.h"
#include "ClientDef.h"
#include "MShopShelf.h"
#include "MNPC.h"
#include "MPriceManager.h"
#include "UIFunction.h"

void GCShopListHandler::execute ( GCShopList * pPacket , Player * pPlayer )
	 

throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	DEBUG_ADD("[GCShopListHandler::execute] run in execute function OK [0].");	
		
#ifdef __GAME_CLIENT__

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
			DEBUG_ADD("[Error] OK 111");
		}
		//------------------------------------------------------
		// NPC인 경우
		//------------------------------------------------------
		else if (pCreature->GetClassType()==MCreature::CLASS_NPC)
		{

			DEBUG_ADD("[GCShopListHandler::execute] OK [1]\n");


			MNPC* pNPC = (MNPC*)pCreature;

			//------------------------------------------------------
			// 새로운 Shelf를 생성한다.
			//------------------------------------------------------\

			ShopRackType_t shopType = pPacket->getShopType();
			DEBUG_ADD_FORMAT("[GCShopListHandler::execute] OK [1.0]  %d\n", shopType);
			if (shopType >= MShopShelf::MAX_SHELF) {
				DEBUG_ADD("[GCShopListHandler::execute] SHELF_TYPE Wrong!");
			}

			MShopShelf* pShelf = MShopShelf::NewShelf( (MShopShelf::SHELF_TYPE) shopType);


			DEBUG_ADD("[GCShopListHandler::execute] OK [1.1]\n");

			pShelf->SetVersion( pPacket->getShopVersion() );
			pShelf->SetEnable();

			DEBUG_ADD("[GCShopListHandler::execute] OK [2]\n");
			//------------------------------------------------------
			// 아이템들 추가
			//------------------------------------------------------
			for (int i=0; i<SHOP_RACK_INDEX_MAX; i++)
			{

				DEBUG_ADD_FORMAT("[GCShopListHandler::execute] shop item %d befre", i);

				const SHOPLISTITEM& item = pPacket->getShopItem( i );

				DEBUG_ADD_FORMAT("[GCShopListHandler::execute] shop item %d after", i);

				if (item.bExist)
				{
					// item 생성
					MItem* pItem = MItem::NewItem( (ITEM_CLASS)item.itemClass );

					pItem->SetID( item.objectID );
					pItem->SetItemType( item.itemType );
					pItem->SetItemOptionList( item.optionType );
					pItem->SetCurrentDurability( item.durability );
					pItem->SetSilver( item.silver );
					pItem->SetGrade( item.grade );
					pItem->SetEnchantLevel( item.enchantLevel );

					// Shelf에 item추가
					pShelf->SetItem( i, pItem );
				}
			}

			DEBUG_ADD("[GCShopListHandler::execute] OK [3]");

			//------------------------------------------------------
			//
			// NPC의 상점에 shelf를 추가한다.
			//
			//------------------------------------------------------
			MShop* pShop = pNPC->GetShop();

			if (pShop==NULL)
			{
				// 상점이 없었으면 생성한다.
				pShop = new MShop;
				pShop->Init( MShopShelf::MAX_SHELF );

				// NPC에 상점 설정
				pNPC->SetShop( pShop );

				// normal item 선반을 생성한다.
				pNPC->CreateFixedShelf();
				pNPC->CreateFixedShelf(true);	// mysterious -_-;
			}

			//------------------------------------------------------
			// 상점의 종류 설정
			//------------------------------------------------------
			pShop->SetShopType( (MShop::SHOP_TYPE)pPacket->getNPCShopType() );

			//------------------------------------------------------
			// default로 normal 상점에 접근하게 한다.
			//------------------------------------------------------
			if (pShop->GetShopType()==MShop::SHOP_EVENT_STAR)
			{
				pShop->SetCurrent( 1 );
			}
			else
			{			
				pShop->SetCurrent( 0 );
			}

			//------------------------------------------------------
			// 선반의 가격 비율 
			//------------------------------------------------------
			g_pPriceManager->SetMarketCondBuy( pPacket->getMarketCondBuy() );
			g_pPriceManager->SetMarketCondSell( pPacket->getMarketCondSell() );			
			
			//------------------------------------------------------
			// 상점에 선반 설정
			//------------------------------------------------------
			pShop->SetShelf( pShelf->GetShelfType(), pShelf );

			//------------------------------------------------------
			// 정상적으로 된 경우
			// --> 상점을 실행한다.
			//------------------------------------------------------
			UI_SetShop( pShop );		// shop 설정
			UI_RunShop();
			UI_SetShop( pShop );		// shop 설정
		}
		//------------------------------------------------------
		// NPC가 아닌 경우
		//------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[Error] The Creature is Not NPC. id=%d", pPacket->getObjectID());
		}
	}
//	__BEGIN_HELP_EVENT
		ExecuteHelpEvent( HELP_EVENT_USE_SHOP );
//	__END_HELP_EVENT
#endif

	__END_CATCH
}
