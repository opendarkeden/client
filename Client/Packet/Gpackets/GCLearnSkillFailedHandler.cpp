//////////////////////////////////////////////////////////////////////
//
// Filename    : GCLearnSkillFailed1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCLearnSkillFailed.h"
#include "ClientDef.h"
#include "MGameStringTable.h"
#include "UIDialog.h"
#include "TempInformation.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCLearnSkillFailedHandler::execute ( GCLearnSkillFailed * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__


	// pPacket->getSkillType();
	
	// pPacket->getDescription();
	g_pTempInformation->SetMode(TempInformation::MODE_NULL);

	if (pPacket->getDesc()==10)
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_SKILL_NOT_SUPPORT].GetString());
	}
	else
	{
		switch (pPacket->getDesc())
		{
			case 0 :
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_SKILL_DIFFER_DOMAIN].GetString());
			break;

			case 1 :
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_SKILL_EXCEED_LEVEL].GetString());
			break;

			default :
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_SKILL_CANNOT_LEARN].GetString());
			
		}
	}
	
#endif

	__END_CATCH
}
