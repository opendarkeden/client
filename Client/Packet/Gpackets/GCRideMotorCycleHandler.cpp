//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRideMotorCycleHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRideMotorCycle.h"
#include "ClientDef.h"
#include "MItem.h"
#include "SoundDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRideMotorCycleHandler::execute ( GCRideMotorCycle * pPacket , Player * pPlayer )
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
	//
	// Creature가 pItem(오토바이)을 장착한다.
	//------------------------------------------------------
	else
	{
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		//-------------------------------------------------------------
		// 캐릭터가 있는 경우
		//-------------------------------------------------------------
		if (pCreature!=NULL)
		{
			// 장착가능한 creature인 경우
			if (pCreature->IsWear())
			{
				MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;

				MItem* pItem = g_pZone->GetItem( pPacket->getTargetObjectID() );

				//------------------------------------------					
				// pItem은 오토바이여야 한다.
				// creature에 장비 시킨다.
				//------------------------------------------					
				if (pItem != NULL)
				{
					if (pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
					{
						//------------------------------------------					
						// 남자 / 여자에 따라서..
						// 오토바이 addon 붙임.
						//------------------------------------------
						pCreatureWear->SetAddonItem( pItem );						
						
						//------------------------------------------
						// 움직이는 방법 바꿈
						//------------------------------------------					
						//pCreatureWear->SetMoveDevice( MCreature::MOVE_DEVICE_RIDE );

						//------------------------------------------
						// Server에 접속된 경우가 아니면..
						// 바로 mouse에 item이 붙는다.
						//------------------------------------------
						//PlaySound( pItem->GetTileSoundID(),
						//			false,
						//			pCreatureWear->GetX(), pCreatureWear->GetY());

						//gC_vs_ui.PickUpItem(pItem);
						g_pZone->RemoveItem( pItem->GetID() );

						// 오토바이 타는 소리 출력
						PlaySound( SOUND_WORLD_BIKE_GO,
									false,
									g_pPlayer->GetX(),
									g_pPlayer->GetY());
					}
					else
					{
						DEBUG_ADD_FORMAT("[Error] id=%d is Not Motorcycle", pPacket->getTargetObjectID());
					}
				}
			}
		}
		//-------------------------------------------------------------
		// 캐릭터가 없는 경우
		//-------------------------------------------------------------
		else
		{
			MItem* pItem = g_pZone->GetItem( pPacket->getTargetObjectID() );

			//------------------------------------------					
			// pItem은 오토바이여야 한다.
			// 걍 없앤다.
			//------------------------------------------					
			if (pItem != NULL)
			{
				if (pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
				{
					g_pZone->RemoveItem( pItem->GetID() );
				}
			}
		}
	}
	
#endif

	__END_CATCH
}
