//--------------------------------------------------------------------------------
//
// Filename    : GCAddGearToInventoryHandler.cpp
// Written By  : crazydog 
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddGearToInventory.h"
#include "ClientDef.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "MInventory.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddGearToInventoryHandler::execute ( GCAddGearToInventory * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__



	//----------------------------------------------------------------------
	// Slayer인 경우	
	//----------------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
		case RACE_SLAYER:
			{
				const MItem* pItem = g_pSlayerGear->GetItem( (MSlayerGear::GEAR_SLAYER)pPacket->getSlotID() );
				
				//----------------------------------------------------------------------
				// 그런 id를 가진 item이 없는 경우
				//----------------------------------------------------------------------
				if (pItem==NULL)
				{
					DEBUG_ADD_FORMAT("[Error] Item does not exist in SlayerGear! slot=%d", (int)pPacket->getSlotID());
				}
				//----------------------------------------------------------------------
				// item이 있는 경우 --> Gear에서 제거해서 Inventory에 추가한다.
				//----------------------------------------------------------------------
				else
				{
					MItem* pRemovedItem = g_pSlayerGear->RemoveItem( pItem->GetID() );
					
					// Inventory(x,y)에 pItem을 추가한다.
					if (!g_pInventory->AddItem( pRemovedItem, pPacket->getInvenX(), pPacket->getInvenY() ))
					{
						// 추가 실패한 경우
						DEBUG_ADD_FORMAT("[Error] Cannot Add Item to Inventory(%d,%d), slot=%d", (int)pPacket->getInvenX(), (int)pPacket->getInvenY(), (int)pPacket->getSlotID());
					}
				}
			}
			break;

		case RACE_VAMPIRE:
			{
				const MItem* pItem = g_pVampireGear->GetItem( (MVampireGear::GEAR_VAMPIRE)pPacket->getSlotID() );
				
				//----------------------------------------------------------------------
				// 그런 id를 가진 item이 없는 경우
				//----------------------------------------------------------------------
				if (pItem==NULL)
				{
					DEBUG_ADD_FORMAT("[Error] Item does not exist in VampireGear! slot=%d", (int)pPacket->getSlotID());
				}
				//----------------------------------------------------------------------
				// item이 있는 경우 --> Gear에서 제거해서 Inventory에 추가한다.
				//----------------------------------------------------------------------
				else
				{
					MItem* pRemovedItem = g_pVampireGear->RemoveItem( pItem->GetID() );
					
					// Inventory(x,y)에 pItem을 추가한다.
					if (!g_pInventory->AddItem( pRemovedItem, pPacket->getInvenX(), pPacket->getInvenY() ))
					{
						// 추가 실패한 경우
						DEBUG_ADD_FORMAT("[Error] Cannot Add Item to Inventory(%d,%d), slot=%d", (int)pPacket->getInvenX(), (int)pPacket->getInvenY(), (int)pPacket->getSlotID());
					}
				}
			}
			break;

		case RACE_OUSTERS:
			{
				const MItem* pItem = g_pOustersGear->GetItem( (MOustersGear::GEAR_OUSTERS)pPacket->getSlotID() );
				
				//----------------------------------------------------------------------
				// 그런 id를 가진 item이 없는 경우
				//----------------------------------------------------------------------
				if (pItem==NULL)
				{
					DEBUG_ADD_FORMAT("[Error] Item does not exist in OustersGear! slot=%d", (int)pPacket->getSlotID());
				}
				//----------------------------------------------------------------------
				// item이 있는 경우 --> Gear에서 제거해서 Inventory에 추가한다.
				//----------------------------------------------------------------------
				else
				{
					MItem* pRemovedItem = g_pOustersGear->RemoveItem( pItem->GetID() );
					
					// Inventory(x,y)에 pItem을 추가한다.
					if (!g_pInventory->AddItem( pRemovedItem, pPacket->getInvenX(), pPacket->getInvenY() ))
					{
						// 추가 실패한 경우
						DEBUG_ADD_FORMAT("[Error] Cannot Add Item to Inventory(%d,%d), slot=%d", (int)pPacket->getInvenX(), (int)pPacket->getInvenY(), (int)pPacket->getSlotID());
					}
				}
			}
			break;

	}




#endif

	__END_CATCH
}
