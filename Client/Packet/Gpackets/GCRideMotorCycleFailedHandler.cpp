//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRideMotorCycleFailedHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRideMotorCycleFailed.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRideMotorCycleFailedHandler::execute ( GCRideMotorCycleFailed * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	
	//------------------------------------------
	// 어쨋든 간에.. 검증이 되었다고 본다.
	//------------------------------------------
	g_pPlayer->SetWaitVerifyNULL();

#endif

	__END_CATCH
}
