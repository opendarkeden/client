//////////////////////////////////////////////////////////////////////
//
// Filename    : GCPhoneDisconnectedHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCPhoneDisconnected.h"
#include "UserInformation.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCPhoneDisconnectedHandler::execute ( GCPhoneDisconnected * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY


	//------------------------------------------------------------------
	// 접속이 끊긴 slot 체크한다.
	//------------------------------------------------------------------
	int slot = pPacket->getSlotID();
	g_pUserInformation->OtherPCSNumber[ slot ] = 0;
	g_pUserInformation->PCSUserName[ slot ].Release();

	//------------------------------------------------------------------
	// PCS 접속 해제
	//------------------------------------------------------------------
	//UI_AcquireQuitPCSOnlineModeMessage();
//	UI_DisconnectPCS( slot );

	__END_CATCH
}
