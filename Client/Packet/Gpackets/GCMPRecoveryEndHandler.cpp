//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMPRecoveryEndHandler.cpp
// Written By  : elca@ewestsoft.com
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCMPRecoveryEnd.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCMPRecoveryEndHandler::execute ( GCMPRecoveryEnd * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__



	g_pPlayer->StopRecoveryMP();

	g_pPlayer->SetStatus( MODIFY_CURRENT_MP, pPacket->getCurrentMP() );

#endif

	__END_CATCH
}
