//--------------------------------------------------------------------------------
//
// Filename    : LCQueryResultCharacterNameHandler.cpp
// Written By  : Reiot
// Description : 
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCQueryResultCharacterName.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "MGameStringTable.h"
#include "UIDialog.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LCQueryResultCharacterNameHandler::execute ( LCQueryResultCharacterName * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// 캐릭 생성중이고
	// 캐릭터 아이디 맞는 경우..
	if (g_Mode==MODE_WAIT_SELECTPC
		&& g_pUserInformation->WhisperID==pPacket->getCharacterName().c_str())
	{
		if (pPacket->isExist())
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_ERROR_ALREADY_REGISTER_ID].GetString() );
		}
		else
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CAN_REGISTER_NAME].GetString() );
		}

		g_pUserInformation->WhisperID.Release();
	}
	
		
	__END_CATCH
}
