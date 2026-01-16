//--------------------------------------------------------------------------------
//
// Filename    : GCGuildResponseHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
// include files
#include "GCGuildResponse.h"
#include "ClientDef.h"
#include "UIFunction.h"
#include "MGameStringTable.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCGuildResponseHandler::execute ( GCGuildResponse * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__BEGIN_DEBUG
		
#ifdef __GAME_CLIENT__
	switch(pPacket->getCode())
	{
	case OK:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_MESSAGE_OK );
		break;
	case ALREADY_IN_UNION:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_ALREADY_IN_UNION );
		break;
	case ALREADY_OFFER_SOMETHING:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_ALREADY_OFFER_SOMETHING );
		break;
	case TARGET_IS_NOT_MASTER:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_TARGET_IS_NOT_MASTER );
		break;
	case NOT_IN_UNION:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_NOT_IN_UNION );
		break;
	case MASTER_CANNOT_QUIT:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_MASTER_CANNOT_QUIT );
		break;
	case NO_TARGET_UNION: 
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_NO_TARGET_UNION );
		break;
	case NOT_YOUR_UNION:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_NOT_YOUR_UNION );
		break;
	case SOURCE_IS_NOT_MASTER:
			UI_PopupMessage( UI_STRING_MESSAGE_TOTAL_GUILD_SOURCE_IS_NOT_MASTER );	
		break;
	case YOU_HAVE_PENALTY:
		UI_PopupMessage (UI_STRING_MESSAGE_TOTAL_UNION_PENALTY );
		break;
	case NOT_ENOUGH_SLOT:
		UI_PopupMessage (UI_STRING_MESSAGE_UNION_ERROR_NO_SLOT );
		break;
	case TOO_MANY_MEMBER:
		UI_PopupMessage (UI_STRING_MESSAGE_TOO_MANY_MEMBER );
		break;
	}
	

#endif

	__END_DEBUG
	 __END_CATCH
}
