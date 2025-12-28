//////////////////////////////////////////////////////////////////////////////
// Filename    : CGDisplayItemHandler.cp	p
// Written By  : 
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGDisplayItem.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "PlayerCreature.h"
	#include "Store.h"
	#include "Inventory.h"
	#include "ItemUtil.h"
	#include "Zone.h"
	#include "Gpackets/GCMyStoreInfo.h"
	#include "Gpackets/GCOtherStoreInfo.h"
#endif	// __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGDisplayItemHandler::execute (CGDisplayItem* pPacket , Player* pPlayer)
	 throw (Error)
{
	__BEGIN_TRY 
		//__BEGIN_DEBUG_EX
		
#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
	Assert(pGamePlayer != NULL);

	PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pGamePlayer->getCreature());
	Assert( pPC != NULL );

	Inventory* pInventory = pPC->getInventory();
	Assert( pInventory != NULL );

	Store* pStore = pPC->getStore();
	Assert( pStore != NULL );

	if ( pPacket->getIndex() > MAX_ITEM_NUM )
	{
		filelog("Store.log", "[%s:%s] (%u) 잘못된 인덱스입니다.",
				pGamePlayer->getID().c_str(), pPC->getName().c_str(), pPacket->getIndex());
		return;
	}

	Item* pItem = pInventory->getItem( pPacket->getX(), pPacket->getY() );
	if ( pItem == NULL || pItem->getObjectID() != pPacket->getItemObjectID() )
	{
		filelog("Store.log", "[%s:%s] (%u, %u) : %u 아이템 좌표가 잘못되었거나 오브젝트 아이디가 잘못되었습니다.",
				pGamePlayer->getID().c_str(), pPC->getName().c_str(), pPacket->getX(), pPacket->getY(), pPacket->getItemObjectID() );
		return;
	}

	if ( pItem->isTimeLimitItem() || !canSell( pItem ) )
	{
		filelog("Store.log", "[%s:%s] (%s) 팔 수 없는 아이템입니다.",
				pGamePlayer->getID().c_str(), pPC->getName().c_str(), pItem->toString().c_str());
		return;
	}

	BYTE result = pStore->setStoreItem( pPacket->getIndex(), pItem, pPacket->getPrice() );
	if (  result != 0 )
	{
		filelog("Store.log", "[%s:%s] (%u) 아이템을 놓을 수 없습니다.",
				pGamePlayer->getID().c_str(), pPC->getName().c_str(), result);
		return;
	}

	GCMyStoreInfo gcInfo;
	gcInfo.setStoreInfo( &(pStore->getStoreInfo()) );
	pGamePlayer->sendPacket( &gcInfo );

	if ( pStore->isOpen() )
	{
		GCOtherStoreInfo gcOtherInfo; 
		gcOtherInfo.setObjectID(pPC->getObjectID());
		gcOtherInfo.setStoreInfo( &(pStore->getStoreInfo()) );
		pPC->getZone()->broadcastPacket( pPC->getX(), pPC->getY(), &gcOtherInfo, pPC );
	}

#endif	// __GAME_SERVER__
		
	//__END_DEBUG_EX 
		__END_CATCH
}

