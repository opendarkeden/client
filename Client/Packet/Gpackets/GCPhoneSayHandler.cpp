//////////////////////////////////////////////////////////////////////
//
// Filename    : GCPhoneSayHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCPhoneSay.h"
#include "UserInformation.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCPhoneSayHandler::execute ( GCPhoneSay * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	

	int slot = pPacket->getSlotID();

	char message[128];
	sprintf(message, "[%s] %s", 
						g_pUserInformation->PCSUserName[ slot ].GetString(), 
						pPacket->getMessage().c_str());

	//--------------------------------------------------
	// 욕 제거
	//--------------------------------------------------
	//g_pChatManager->RemoveCurse( message );

	//------------------------------------------------------
	// 특정 slot에 message를 추가시켜야 한다.
	//------------------------------------------------------
	//UI_AddChatToHistory( message );


#endif

	__END_CATCH
}
