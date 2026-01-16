
//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUseItemFromInventoryHandler.cpp
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGUseItemFromInventory.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "Zone.h"
	#include "PlayerCreature.h"
	#include "Slayer.h"
	#include "Vampire.h"
	#include "Inventory.h"
	#include "Item.h"
	#include "ItemInfo.h"
	#include "ItemInfoManager.h"
	#include "ItemFactoryManager.h"
	#include "ItemUtil.h"
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
void CGUseItemFromInventoryHandler::execute(CGUseItemFromInventory* pPacket, Player* pPlayer)
	throw (ProtocolException, Error)
{
	__BEGIN_TRY
//		__BEGIN_DEBUG_EX

#ifdef __GAME_SERVER__
#endif

  //  __END_DEBUG_EX 
	__END_CATCH
}

#endif