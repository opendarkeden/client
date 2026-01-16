//----------------------------------------------------------------------
//
// Filename    : GCRequestPowerPointResultHandler.cpp
// Written By  :
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCRequestPowerPointResult.h"

#ifdef __GAME_CLIENT__
#include "VS_UI_GameCommon.h"
#include "UIFunction.h"
#include "MGameStringTable.h"
#include "UIDialog.h"
#endif

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void GCRequestPowerPointResultHandler::execute ( GCRequestPowerPointResult * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	switch(pPacket->getErrorCode())
	{
	case GCRequestPowerPointResult::_NO_ERROR: 
		{
			g_char_slot_ingame.m_Powerjjang_Point = pPacket->getSumPowerPoint(); 
			char TempBuffer[128];
			sprintf(TempBuffer, (*g_pGameStringTable)[UI_STRING_POWER_JJANG_REQUEST_OK].GetString(), pPacket->getRequestPowerPoint());
			g_pUIDialog->PopupFreeMessageDlg(TempBuffer);
		}
		break;
	case GCRequestPowerPointResult::SERVER_ERROR:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_SERVER_ERROR );
		break;
	case GCRequestPowerPointResult::PROCESS_ERROR:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_PROCESS_ERROR );
		break;
	case GCRequestPowerPointResult::NO_MEMBER:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_NO_MEMBER );
		break;
	case GCRequestPowerPointResult::NO_POINT:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_NO_POINT );
		break;
	case GCRequestPowerPointResult::NO_MATCHING:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_NO_MATCHING );
		break;
	case GCRequestPowerPointResult::CONNECT_ERROR:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_CONNECT );
		break;
	default:
		UI_PopupMessage( STRING_ERROR_ETC_ERROR );
		break;
	}
		
#endif

	__END_CATCH
}

