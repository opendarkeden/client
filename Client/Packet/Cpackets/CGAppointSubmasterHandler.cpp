//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAppointSubmasterHandler.cpp
// Written By  :
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGAppointSubmaster.h"

#ifdef __GAME_SERVER__
	#include "SystemAvailabilitiesManager.h"
	#include "Assert.h"
	#include "GamePlayer.h"
	#include "PlayerCreature.h"
#endif	// __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__	
void CGAppointSubmasterHandler::execute (CGAppointSubmaster* pPacket , Player* pPlayer)
	 throw (Error)
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	PlayerCreature* pPlayerCreature = dynamic_cast<PlayerCreature*>(pPlayer);
	Assert( pPlayerCreature != NULL );

	SYSTEM_ASSERT(SYSTEM_GUILD);

#endif	// __GAME_SERVER__
		
	__END_DEBUG_EX __END_CATCH
}
#endif
