//----------------------------------------------------------------------
//
// Filename    : GCUsePowerPointResultHandler.cpp
// Written By  :
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCUsePowerPointResult.h"

#ifdef __GAME_CLIENT__
#include "VS_UI_GameCommon.h"
#include "UIFunction.h"
#include "MGameStringTable.h"
#include "VS_UI.h"
#endif

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void GCUsePowerPointResultHandler::execute ( GCUsePowerPointResult * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	switch(pPacket->getErrorCode())
	{
	case GCUsePowerPointResult::_NO_ERROR:
		{
			g_char_slot_ingame.m_Powerjjang_Point = pPacket->getPowerPoint();
			gC_vs_ui.PowerjjangGambleResult(pPacket->getItemCode());
		}
		break;
	case GCUsePowerPointResult::NOT_ENOUGH_INVENTORY_SPACE:
		UI_PopupMessage( UI_STRING_MESSAGE_INVENTORY_FULL_MONSTER_KILL_QUEST );
		break;
	case GCUsePowerPointResult::NOT_ENOUGH_POWER_POINT:
		UI_PopupMessage( UI_STRING_POWER_JJANG_ERROR_NO_POINT );
		break;
	default:
		UI_PopupMessage( STRING_ERROR_ETC_ERROR );
		break;
	}
#endif

	__END_CATCH
}

