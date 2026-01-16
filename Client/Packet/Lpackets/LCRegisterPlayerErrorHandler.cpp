//--------------------------------------------------------------------------------
//
// Filename    : LCRegisterPlayerErrorHandler.cpp
// Written By  : Reiot
// Description : 
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCRegisterPlayerError.h"
#include "ClientDef.h"

extern void		PopupErrorMessage(ErrorID errorID);

extern BOOL		g_bNeedUpdate;

//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
void LCRegisterPlayerErrorHandler::execute ( LCRegisterPlayerError * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	// message
	DEBUG_ADD_FORMAT("[ RegisterPlayerError ] %d", (int)pPacket->getErrorID() );
	
	
	//throw DisconnectException("RegisterPlayer Failed!");
	//g_UIDialog.PopupFreeMessageDlg( pPacket->getMessage().c_str() );
	if (!g_bNeedUpdate)
	{
		PopupErrorMessage( (ErrorID)pPacket->getErrorID() );
	}

	// 다시 UI를 기다리는 상태로 바꾼다.
	g_ModeNext = MODE_NEWUSER;

#endif
		
	__END_CATCH
}
