//----------------------------------------------------------------------
//
// Filename    : LCCreatePCErrorHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCCreatePCError.h"
#include "ClientDef.h"

extern void		PopupErrorMessage(ErrorID errorID);

//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
void LCCreatePCErrorHandler::execute ( LCCreatePCError * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	//cout << "Fail to create PC... (" << pPacket->toString() << ")" << endl;
	//throw ProtocolException("CreatePC Failed!");

	//UI_NewCharacterCreateFailed( 0 );

	PopupErrorMessage( (ErrorID)pPacket->getErrorID() );


	g_ModeNext = MODE_WAIT_SELECTPC;

#endif
		
	__END_CATCH
}
