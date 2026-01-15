//--------------------------------------------------------------------------------
//
// Filename    : GCDeleteInventoryItemHandler.cpp
// Written By  : Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCDeleteInventoryItem.h"
#include "ClientDef.h"
#include "MInventory.h"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCDeleteInventoryItemHandler::execute ( GCDeleteInventoryItem * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	MItem* pItem = UI_GetMouseItem();

	// mouse에 있는 것도 함 체크해준다. by sigi. 2002.7.8
	if (pItem!=NULL
		&& pItem->GetID()==pPacket->getObjectID())
	{
		UI_DropItem();		
	}
	else
	{
		pItem = g_pInventory->RemoveItem( pPacket->getObjectID() );

		if (pItem==NULL)
		{
			DEBUG_ADD_FORMAT( "[Error] Can't remove Item. id=%d", pPacket->getObjectID());
		}		
	}

	if (pItem!=NULL)
	{

		TYPE_OBJECTID objectID = pPacket->getObjectID();

		if (g_pPlayer->IsItemCheckBufferDropToRelicTable() || g_pPlayer->IsItemCheckBufferDropToCreature())
		{
			//---------------------------------------------
			// 떨어뜨릴려는 item
			//---------------------------------------------
			MItem* pCheckItem = g_pPlayer->GetItemCheckBuffer();

			if(pCheckItem->GetID() == pItem->GetID()) 
				UI_DropItem();
			g_pPlayer->ClearItemCheckBuffer();
		}

		delete pItem;
	}

#endif

	__END_CATCH
}
