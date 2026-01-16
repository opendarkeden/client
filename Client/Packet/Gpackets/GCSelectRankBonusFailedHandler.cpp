//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSelectRankBonusFailed1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSelectRankBonusFailed.h"
#include "ClientDef.h"
#include "MGameStringTable.h"
#include "TempInformation.h"
#include "UIDialog.h"

extern bool gbl_global_empty_move;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSelectRankBonusFailedHandler::execute ( GCSelectRankBonusFailed * pGCSelectRankBonusFailed , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY

		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_SKILL_NOT_SUPPORT].GetString());
		gbl_global_empty_move = true;
		
	__END_CATCH
}
