//////////////////////////////////////////////////////////////////////
//
// Filename    : GCStashSellHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCStashSell.h"
#include "ClientDef.h"
#include "MGameStringTable.h"
#include "TempInformation.h"
#include "UIDialog.h"
#include "UIFunction.h"


void GCStashSellHandler::execute ( GCStashSell * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__BEGIN_DEBUG
	
#ifdef __GAME_CLIENT__


	if ( g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK )
	{
		g_pPlayer->SetWaitVerifyNULL();
	
		DEBUG_ADD("[Verified] NPC Ask Answer OK");
	
		//------------------------------------------------------
		// 보관함을 더 살 수 없는 경우는 가격이 0으로 날아온다.
		//------------------------------------------------------
		if (pPacket->getPrice()==0)
		{
			// 다시 뭔가를 선택할 수 있게한다..
			g_pUIDialog->ShowPCTalkDlg();

			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_MORE].GetString());
		}
		//------------------------------------------------------
		// price에 보관함을 살까? 말까?
		//------------------------------------------------------
		else
		{
			//------------------------------------------------------
			// price에 보관함을 살까? 말까?
			//------------------------------------------------------
			UI_RunStorageBuy( pPacket->getPrice() );

			//------------------------------------------------------
			// 가격 저장
			//------------------------------------------------------
			g_pTempInformation->SetMode(TempInformation::MODE_STORAGE_BUY);
			g_pTempInformation->Value1	= pPacket->getPrice();
		}
	}

#endif

	__END_DEBUG
	__END_CATCH
}
