//////////////////////////////////////////////////////////////////////////////
// Filename    : CGMixItemHandler.cpp
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGMixItem.h"

#ifdef __GAME_SERVER__
#include "GamePlayer.h"
#include "Inventory.h"
#include "PlayerCreature.h"
#include "Item.h"
#include "ItemUtil.h"
#include "Gpackets/GCCannotUse.h"
void sendCannotUse(CGMixItem* pPacket, Player* pPlayer);
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGMixItemHandler::execute(CGMixItem* pPacket, Player* pPlayer)
	throw (ProtocolException, Error)
{
	__BEGIN_TRY 

#ifdef __GAME_SERVER__

	////cout << "CGMixItemHandler " << endl;
	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
	Assert(pGamePlayer != NULL);

	Creature* pCreature = pGamePlayer->getCreature();
	Assert(pCreature != NULL);
	Assert(pCreature->isPC());

	PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pCreature);
	Assert(pPC != NULL);

	Inventory* pInventory = pPC->getInventory();
	Zone*      pZone      = pPC->getZone();

	Assert(pInventory != NULL);
	Assert(pZone != NULL);

	CoordInven_t InvenX = pPacket->getX();
	CoordInven_t InvenY = pPacket->getY();

	//cout << "패킷날라옴 : " << pPacket->toString() << endl;

	// 인벤토리 좌표를 넘어가는 영역이라면 안 된다.
	if (InvenX >= pInventory->getWidth() || InvenY >= pInventory->getHeight())
	{
		GCCannotUse _GCCannotUse;
		_GCCannotUse.setObjectID(pPacket->getObjectID());
		pGamePlayer->sendPacket(&_GCCannotUse);
		return;
	}

	// 인벤토리에 그 아이템이 없다면 에러다.
	Item* pItem = pInventory->getItem(InvenX, InvenY);
	if (pItem == NULL)
	{
		GCCannotUse _GCCannotUse;
		_GCCannotUse.setObjectID(pPacket->getObjectID());
		pGamePlayer->sendPacket(&_GCCannotUse);
		return;
	}

	// 인벤토리에 있는 아이템의 Object를 받는다.
	ObjectID_t ItemObjectID = pItem->getObjectID();

	// OID가 일치하지 않거나, 사용할 수 없는 아이템이라면 에러다.
	if (ItemObjectID != pPacket->getObjectID() || !isUsableItem(pItem, pCreature))
	{
		//cout << "아템 사용 불가. 옵젝트 아디가 안 맞던가..." << endl;
		GCCannotUse _GCCannotUse;
		_GCCannotUse.setObjectID(pPacket->getObjectID());
		pGamePlayer->sendPacket(&_GCCannotUse);
		return;
	}

	Item *pTargetItem1 = NULL;
	Item *pTargetItem2 = NULL;

	CoordInven_t dummy;

	pTargetItem1 = pInventory->findItemOID( pPacket->getTargetObjectID(0), dummy, dummy );
	pTargetItem2 = pInventory->findItemOID( pPacket->getTargetObjectID(1), dummy, dummy );

	if ( pTargetItem1 == NULL || pTargetItem2 == NULL )
	{
		GCCannotUse _GCCannotUse;
		_GCCannotUse.setObjectID(pPacket->getObjectID());
		pGamePlayer->sendPacket(&_GCCannotUse);
		return;
	}

#endif

    __END_CATCH
}

