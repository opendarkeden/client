//--------------------------------------------------------------------------------
//
// Filename    : GCLightningHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCLightning.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

#include "ClientDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCLightningHandler::execute ( GCLightning * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	#ifdef __EXPO_CLIENT__
		return;
	#endif


	// server : 10 = 1초
	// client : 1000 = 1초
	// 그래서.. *100.. 음하하..
	SetLightning( pPacket->getDelay()*100 );

#endif

	__END_CATCH
}
