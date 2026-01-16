//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopVersionHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopVersion.h"
#include "ClientDef.h"
#include "MNPC.h"
#include "MShopShelf.h"
#include "ServerInfo.h"
#include "UIFunction.h"
#include "MPriceManager.h"
void GCShopVersionHandler::execute ( GCShopVersion * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
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
			DEBUG_ADD_FORMAT("[Error] There is no such Creature id=%d", pPacket->getObjectID());
		}
		//------------------------------------------------------
		// NPC인 경우
		//------------------------------------------------------
		else if (pCreature->GetClassType()==MCreature::CLASS_NPC)
		{
			MNPC* pNPC = (MNPC*)pCreature;

			//------------------------------------------------------
			// NPC의 상점을 얻는다.
			//------------------------------------------------------
			MShop* pShop = pNPC->GetShop();

			if (pShop==NULL)
			{
				// 상점이 없는 경우 --> 생성.
				pShop = new MShop;
				pShop->Init( MShopShelf::MAX_SHELF );
				
				// NPC에 상점 설정..
				pNPC->SetShop( pShop );				
			}

			//------------------------------------------------------
			// default로 normal 상점에 접근하게 한다.
			//------------------------------------------------------
			pShop->SetCurrent( 0 );

			//------------------------------------------------------
			// normal item 선반을 생성한다.
			//------------------------------------------------------
			pNPC->CreateFixedShelf();
			pNPC->CreateFixedShelf(true);	// mysterious -_-;


			//------------------------------------------------------
			// 각 shelf의 version 비교..
			//------------------------------------------------------
			BOOL bSameAll = TRUE;
			for (ShopRackType_t i=0; i<SHOP_RACK_TYPE_MAX; i++)
			{
				//------------------------------------------------------
				// normal item인 경우는 체크할 필요없다. 
				// --> client에 이미 정보가 있으므로
				//------------------------------------------------------
				if (i!=SHOP_RACK_SPECIAL)
				{
					continue;
				}

				MShopShelf* pShopShelf = pShop->GetShelf( i );

				//------------------------------------------------------
				// 선반이 없는 경우 --> 생성
				//------------------------------------------------------
				if (pShopShelf==NULL)
				{
					pShopShelf = MShopShelf::NewShelf( (MShopShelf::SHELF_TYPE)i );

					pShop->SetShelf( (MShopShelf::SHELF_TYPE)i, pShopShelf );
				}

				unsigned int serverVersion = pPacket->getVersion( i );
				unsigned int clientVersion = pShopShelf->GetVersion();

				//------------------------------------------------------
				// version이 다르면 item std::list를 요청한다.
				//------------------------------------------------------
				if (serverVersion!=clientVersion)
				{
					// version이 다른 것이 있다고 체크
					bSameAll = FALSE;

						// item std::list 요청 packet
						CGShopRequestList	_CGShopRequestList;
						_CGShopRequestList.setObjectID( pNPC->GetID() );
						_CGShopRequestList.setRackType( i );

						g_pSocket->sendPacket( &_CGShopRequestList );						
				}
			}

			// 2004, 10, 25, sobeit add start - 세율 조절
			g_pPriceManager->SetMarketCondSell( pPacket->getMarketCondSell() );
			// 2004, 10, 25, sobeit add end
			
			//------------------------------------------------------
			// 모든 shelf의 version이 같으면..
			// 바로 상점을 띄운다.
			//------------------------------------------------------
			if (bSameAll)
			{
				//------------------------------------------------------
				// 정상적으로 된 경우
				// --> 상점을 실행한다.
				//------------------------------------------------------
				UI_RunShop();
				UI_SetShop( pShop );		// shop 설정				
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
