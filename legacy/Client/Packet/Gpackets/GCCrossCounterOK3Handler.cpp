//////////////////////////////////////////////////////////////////////
//
// Filename    : GCCrossCounterOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCCrossCounterOK3.h"
#include "ClientDef.h"
#include "MFakeCreature.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCCrossCounterOK3Handler::execute ( GCCrossCounterOK3 * pPacket , Player * pPlayer )
	 throw ( Error )
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
		MCreature* pUserCreature = g_pZone->GetCreature( pPacket->getObjectID() );
		MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );

		SkillCrossCounter( pUserCreature, pTargetCreature, pPacket->getSkillType() );
	}	

#endif

	__END_CATCH
}
