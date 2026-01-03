//----------------------------------------------------------------------
//
// Filename    : LCVersionCheckErrorHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCVersionCheckError.h"
#include "ClientDef.h"
#include "UIDialog.h"
#include "MGameStringTable.h"

extern void		PopupErrorMessage(ErrorID errorID);

extern BOOL		g_bNeedUpdate;

//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
void LCVersionCheckErrorHandler::execute ( LCVersionCheckError * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	
	// client를 update해야한다.
	//g_bNeedUpdate = TRUE;
	// edit by Coffee
	//g_ModeNext = MODE_QUIT;
	g_ModeNext = MODE_LOGIN_WRONG;
	//PopupErrorMessage((ErrorID)21); // 쇱꿎경굶댄轎
	g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_CHECK_VERSION_ERROR].GetString(), -1,-1,UI_DIALOG_OK, true);
	// end

#endif
		
	__END_CATCH
}
