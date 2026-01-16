//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShowGuildJoinHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShowGuildJoin.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCShowGuildJoinHandler::execute ( GCShowGuildJoin * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
		UI_ShowGuildJoin(pPacket->getJoinFee(), pPacket->getGuildMemberRank(), NULL, pPacket->getGuildName().c_str(), pPacket->getGuildID());
#endif

	__END_CATCH
}
