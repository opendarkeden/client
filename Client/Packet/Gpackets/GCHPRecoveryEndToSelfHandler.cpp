//////////////////////////////////////////////////////////////////////
//
// Filename    : GCHPRecoveryEndToSelfHandler.cpp
// Written By  : elca@ewestsoft.com
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCHPRecoveryEndToSelf.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCHPRecoveryEndToSelfHandler::execute ( GCHPRecoveryEndToSelf * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__



	if (g_pPlayer!=NULL)
	{
		g_pPlayer->StopRecoveryHP();

		g_pPlayer->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );
	}	

#endif

	__END_CATCH
}
