//////////////////////////////////////////////////////////////////////
//
// Filename    : GCDownSkillFailed1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCDownSkillFailed.h"
#include "ClientDef.h"
#include "UIFunction.h"
#include "mgamestringtable.h"

#define DOMAIN_DIFFER 0
#define NOT_OUSTERS 1
#define TOO_LOW 2
#define TOO_HIGH 3
#define NOT_ENOUGH_MONEY 4
#define INVALID_SKILL 5
#define HAVE_NOT_SKILL 6
#define CANNOT_DROP_SKILL 7
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCDownSkillFailedHandler::execute ( GCDownSkillFailed * pGCDownSkillFailed , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 

	switch( pGCDownSkillFailed->getDesc() )
	{
	//	case DOMAIN_DIFFER :
	//		break;
		case NOT_OUSTERS :
			UI_PopupMessage( STRING_MESSAGE_NOT_OUSTERS );
			break;
		case TOO_LOW :
			UI_PopupMessage( STRING_MESSAGE_TOO_LOW_SKILL_LEVEL );
			break;
		case TOO_HIGH :
			UI_PopupMessage( STRING_MESSAGE_TOO_HIGH_SKILL_LEVEL );
			break;
		case NOT_ENOUGH_MONEY :
			UI_PopupMessage( STRING_MESSAGE_NOT_ENOUGH_MONEY_FOR_DOWN_SKILL );
			break;
		case INVALID_SKILL :
			UI_PopupMessage( STRING_MESSAGE_INVALID_SKILL );
			break;
		case HAVE_NOT_SKILL :
			UI_PopupMessage( STRING_MESSAGE_NOT_LEARNED_SKILL );
			break;
		case CANNOT_DROP_SKILL :
			UI_PopupMessage( STRING_MESSAGE_CANNOT_SKILLTREE_DELETE );
			break;
		default :
			UI_PopupMessage( STRING_ERROR_ETC_ERROR );
			break;
	}

	__END_CATCH
}
