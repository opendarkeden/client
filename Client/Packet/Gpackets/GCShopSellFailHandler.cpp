//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopSellFailHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopSellFail.h"
#include "ClientDef.h"
#include "UIDialog.h"
#include "TempInformation.h"
#include "MGameStringTable.h"
#include "UIFunction.h"

void GCShopSellFailHandler::execute ( GCShopSellFail * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	// mode를 없앤다.
	g_pTempInformation->SetMode(TempInformation::MODE_NULL);

	// 거래를 다시 활성화한다.
	UI_UnlockItemTrade();

	// 2005, 1, 3, sobeit add start - 이궁 대충. TempInformation에 모드 추가 해야 하지만.. 
	if(UI_IsRunningSwapAdvancementItem())
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_MESSAGE_SWAP_ADVANCEMENT_ITEM_ERROR].GetString() );
	else
	// 2005, 1, 3, sobeit add end
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SELL].GetString() );

#endif

	__END_CATCH
}
