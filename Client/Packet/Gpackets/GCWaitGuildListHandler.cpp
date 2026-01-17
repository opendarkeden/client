//////////////////////////////////////////////////////////////////////
//
// Filename    : GCWaitGuildListHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCWaitGuildList.h"
#include "clientdef.h"
#include "UIFunction.h"
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCWaitGuildListHandler::execute ( GCWaitGuildList * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	//------------------------------------------------------
	// 검증
	//------------------------------------------------------
	if ( g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK )
	{
		g_pPlayer->SetWaitVerifyNULL();

		DEBUG_ADD("[Verified] NPC Ask Answer OK");
	}

	UI_ShowWaitGuildList(pPacket);
#endif

	__END_CATCH
}
