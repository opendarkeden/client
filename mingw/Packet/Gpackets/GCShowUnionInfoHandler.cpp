//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShowUnionInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShowUnionInfo.h"
#include "ClientDef.h"
#include "UIFunction.h"


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCShowUnionInfoHandler::execute ( GCShowUnionInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY //__BEGIN_DEBUG_EX
	
#ifdef __GAME_CLIENT__
	UI_ShowUnionGuildMemberInfo(pPacket);
#endif

	__END_CATCH //__END_CATCH
}
