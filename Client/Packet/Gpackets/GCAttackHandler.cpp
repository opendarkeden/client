//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttack.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackHandler::execute ( GCAttack * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	// message

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

		if (pCreature != NULL)
		{
			// 방향 바라보기
			pCreature->SetDirectionToPosition(pPacket->getX(), pPacket->getY());

			pCreature->PacketAttackNormal(	pPacket->getX(),
											pPacket->getY(),
											pPacket->getDir());
		}
	}	

#endif

	__END_CATCH
}
