//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToInventoryOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToInventoryOK2.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToInventoryOK2Handler::execute ( GCSkillToInventoryOK2 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__



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
			
		// Creature가 casting동작 취함..
		if (pCreature != NULL)
		{
			int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			//------------------------------------------------------------
			// Delay Frame 설정
			//------------------------------------------------------------
			pCreature->SetEffectDelayFrame( pPacket->getSkillType(), delayFrame );


			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToInventoryItem(
								pPacket->getSkillType()								
			);
		}
	}	
	

#endif

	__END_CATCH
}
