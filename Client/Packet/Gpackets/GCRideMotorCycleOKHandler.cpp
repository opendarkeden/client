//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRideMotorCycleOKHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRideMotorCycleOK.h"
#include "ClientDef.h"
#include "MItem.h"
#include "SoundDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRideMotorCycleOKHandler::execute ( GCRideMotorCycleOK * pPacket , Player * pPlayer )
	 
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
		MItem* pItem = g_pZone->GetItem( pPacket->getObjectID() );

		//------------------------------------------					
		// pItem은 오토바이여야 한다.
		// player에 장비 시킨다.
		//------------------------------------------					
		if (pItem != NULL)
		{
			if (pItem->GetItemClass()==ITEM_CLASS_MOTORCYCLE)
			{
				//------------------------------------------					
				// 오토바이 addon 붙임.
				//------------------------------------------					
				g_pPlayer->SetAddonItem( pItem );
				
				//------------------------------------------
				// 움직이는 방법 바꿈
				//------------------------------------------					
				//g_pPlayer->SetMoveDevice( MCreature::MOVE_DEVICE_RIDE );

				//------------------------------------------
				// Server에 접속된 경우가 아니면..
				// 바로 mouse에 item이 붙는다.
				//------------------------------------------
				//PlaySound( pItem->GetTileSoundID(),
				//			false,
				//			g_pPlayer->GetX(), g_pPlayer->GetY());

				//gC_vs_ui.PickUpItem(pItem);
				g_pZone->RemoveItem( pItem->GetID() );

				PlaySound( SOUND_WORLD_BIKE_GO );
			}
			else
			{
				DEBUG_ADD_FORMAT("[Error] id=%d is Not Motorcycle", pPacket->getObjectID());			
			}
		}
	}
	
	//------------------------------------------
	// 어쨋든 간에.. 검증이 되었다고 본다.
	//------------------------------------------
	g_pPlayer->SetWaitVerifyNULL();

	// [도움말] 오토바이 탄 경우
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_ITEM_RIDE_MOTORCYCLE );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
