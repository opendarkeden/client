//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShowWaitGuildInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShowWaitGuildInfo.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCShowWaitGuildInfoHandler::execute ( GCShowWaitGuildInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
		UI_ShowWaitGuildInfo(pPacket);
#endif

	__END_CATCH
}
