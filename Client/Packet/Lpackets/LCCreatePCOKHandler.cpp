//----------------------------------------------------------------------
//
// Filename    : LCCreatePCOKHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCCreatePCOK.h"
#include "ClientDef.h"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
	#include "Cpackets/CLGetPCList.h"
#endif

//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
void LCCreatePCOKHandler::execute ( LCCreatePCOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	UI_NewCharacterCreateOk();

	// 다시 PC LIST 를 받아와야 한다.
	CLGetPCList clGetPCList;
	pClientPlayer->sendPacket( &clGetPCList );

	pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CL_GET_PC_LIST );
	
#endif

	__END_CATCH
}
