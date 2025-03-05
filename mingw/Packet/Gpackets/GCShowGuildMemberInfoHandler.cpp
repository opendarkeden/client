//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShowGuildMemberInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShowGuildMemberInfo.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCShowGuildMemberInfoHandler::execute ( GCShowGuildMemberInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
		UI_ShowGuildMemberInfo(pPacket);
#endif

	__END_CATCH
}
