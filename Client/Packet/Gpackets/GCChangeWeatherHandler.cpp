//--------------------------------------------------------------------------------
//
// Filename    : GCChangeWeatherHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCChangeWeather.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

#include "ClientDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCChangeWeatherHandler::execute ( GCChangeWeather * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	#ifdef __EXPO_CLIENT__
		return;
	#endif
	
	
	SetWeather( pPacket->getWeather(), pPacket->getWeatherLevel() );	

#endif

	__END_CATCH
}
