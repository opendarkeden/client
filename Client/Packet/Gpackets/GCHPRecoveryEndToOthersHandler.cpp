//////////////////////////////////////////////////////////////////////
//
// Filename    : GCHPRecoveryEndToOthersHandler.cpp
// Written By  : elca@ewestsoft.com
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCHPRecoveryEndToOthers.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCHPRecoveryEndToOthersHandler::execute ( GCHPRecoveryEndToOthers * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	if (g_pZone!=NULL)
	{
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		if (pCreature!=NULL)
		{
			pCreature->StopRecoveryHP();

			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );
		}
	}


#endif

	__END_CATCH
}
