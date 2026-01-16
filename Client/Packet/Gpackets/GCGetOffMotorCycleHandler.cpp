//////////////////////////////////////////////////////////////////////
//
// Filename    : GCGetOffMotorCycleHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCGetOffMotorCycle.h"
#include "ClientDef.h"
#include "SoundDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCGetOffMotorCycleHandler::execute ( GCGetOffMotorCycle * pPacket , Player * pPlayer )
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
	// Creature가 오토바이를 장착해제 한다.
	//------------------------------------------------------
	else
	{
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		if (pCreature!=NULL)
		{
			// 장착가능한 creature인 경우
			if (pCreature->IsWear())
			{
				MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;
			
				//------------------------------------------
				// 오토바이 그림을 없애준다.
				//------------------------------------------
				pCreatureWear->RemoveAddon( ADDON_MOTOR );

				//------------------------------------------
				// 움직이는 방법 바꿈
				//------------------------------------------
				pCreatureWear->SetMoveDevice( MCreature::MOVE_DEVICE_WALK );

				//------------------------------------------
				// 내리는 소리 출력
				//------------------------------------------
				PlaySound( SOUND_WORLD_BIKE_STOP,
							false,
							g_pPlayer->GetX(),
							g_pPlayer->GetY());
			}
			else
			{
				DEBUG_ADD_FORMAT("[Error] Creature is not WearCreature id=%d", pPacket->getObjectID());
			}
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] Not Exist Creature id=%d", pPacket->getObjectID());
		}
	}

#endif

	__END_CATCH
}
