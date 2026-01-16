//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTakeOffHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTakeOff.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCTakeOffHandler::execute ( GCTakeOff * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

	//------------------------------------------------------
	//
	//  Creature의 Addon을 제거한다.
	//
	//------------------------------------------------------
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

		//--------------------------------------------------
		// Creature가 없는 경우
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] Not Exist Creature. ID=%d", pPacket->getObjectID());
		}
		//--------------------------------------------------
		// 존재하는 Creature인 경우
		// 복장이 있는 creature이면 --> Addon제거
		//--------------------------------------------------
		else if (pCreature->IsWear())
		{
			MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;
			
			// 캐릭터를 정지시킨다.
			pCreatureWear->SetStop();

			#ifdef	OUTPUT_DEBUG				
				if (pCreatureWear->RemoveAddon( pPacket->getSlotID() ))
				{
					DEBUG_ADD_FORMAT("[OK] RemoveAddon. ID=%d, Slot=%d", pPacket->getObjectID(), (int)pPacket->getSlotID());
				}
				else
				{
					const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(pPacket->getSlotID());
					DEBUG_ADD_FORMAT("[Error] RemoveAddon. ID=%d, Slot=%d, AddonFrameID=%d", pPacket->getObjectID(), (int)pPacket->getSlotID(), addonInfo.FrameID);					
				}				
			#else
				pCreatureWear->RemoveAddon( pPacket->getSlotID() );
			#endif
		}
			
		//--------------------------------------------------
		// 옷을 입을 수 없는 Creature인 경우
		//--------------------------------------------------
		else 
		{
			DEBUG_ADD_FORMAT("[Error] Creature is Not CreatureWear. ID=%d", pPacket->getObjectID());
		}
	}
	

#endif

	__END_CATCH
}
