//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTradeErrorHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTradeError.h"
#include "ClientDef.h"
#include "UIDialog.h"
#include "MGameStringTable.h"
#include "TempInformation.h"
#include "MTradeManager.h"
#include "UIFunction.h"

void GCTradeErrorHandler::execute ( GCTradeError * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	
	if (g_pPlayer==NULL
		|| g_pTempInformation==NULL
		|| g_pUIDialog==NULL
		|| g_pGameStringTable==NULL)
	{
		return;
	}


	int code = pPacket->getCode();

	//----------------------------------------------------------------------
	//
	//	교환이 취소되는 경우의 공통적인 처리
	//
	//----------------------------------------------------------------------
	if (code==GC_TRADE_ERROR_CODE_TARGET_NOT_EXIST
		|| code==GC_TRADE_ERROR_CODE_RACE_DIFFER
		|| code==GC_TRADE_ERROR_CODE_NOT_SAFE)
	{	
		if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
		{
			// 교환취소할래?를 닫는다.
			UI_CloseExchangeCancel();

			// 검증 제거..
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
			{
				g_pPlayer->SetWaitVerifyNULL();
			}

			g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		}
	}

	switch (code)
	{
		//----------------------------------------------------------------------
		// 교환을 요구한 대상이 존재하지 않는다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_TARGET_NOT_EXIST :						
			// 그런 사람 엄따~
			UI_CloseExchangeCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_NOBODY].GetString());
		break;
		
		
		//----------------------------------------------------------------------
		// 교환을 요구한 대상이 다른 종족이다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_RACE_DIFFER :
			// 교환할 수 엄따~
			UI_CloseExchangeCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE].GetString());
		break;

		//----------------------------------------------------------------------
		// 교환을 하려고 하는 곳이 안전 지대가 아니다.
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_NOT_SAFE :
			// 교환할 수 엄따~
			UI_CloseExchangeCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_SAFETY_ZONE_ONLY].GetString());			
		break;

		//----------------------------------------------------------------------
		// 모터사이클을 탄 채로 교환을 하려고 하고 있다.
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_MOTORCYCLE :
			// 교환할 수 엄따~
			UI_CloseExchangeCancel();

			if( g_pPlayer->IsSlayer() )
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_CANNOT_ON_MOTORCYCLE].GetString());
			else if ( g_pPlayer->IsOusters() )
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE_SUMMON_SYLPH].GetString() );
		break;
		

		//----------------------------------------------------------------------
		// 교환 중이면서 다시 교환을 하려고 한다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_ALREADY_TRADING :
			
			UI_CloseExchangeCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE_ALREADY_TRADING].GetString());
		break;

		//----------------------------------------------------------------------
		// 교환 중이 아닌데 : 교환 관련 패킷이 날아왔다.
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_NOT_TRADING :
			// - -;
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] Server is Not Trade Mode");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 교환 대상에 더하려고 하는 아이템을 가지고 있지 않다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_ADD_ITEM :
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] There is no such item to Add");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 교환 대상에서 빼려고 하는 아이템을 가지고 있지 않다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_REMOVE_ITEM :
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] There is no such item to Remove");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 교환 대상에 더하려고 하는 돈을 가지고 있지 않다.
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_INCREASE_MONEY :
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_TRADE_ERROR_CODE_INCREASE_MONEY");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 교환 대상에서 빼려고 하는 돈을 가지고 있지 않다.
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_DECREASE_MONEY :
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_TRADE_ERROR_CODE_DECREASE_MONEY");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 교환을 했는데 : 자리가 모자라서 실패했다
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_NOT_ENOUGH_SPACE :
			
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE_NO_SPACE].GetString());

			#ifdef OUTPUT_DEBUG			
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_TRADE_ERROR_CODE_NOT_ENOUGH_SPACE");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 알 수 없는 에러이다...
		//----------------------------------------------------------------------
		case GC_TRADE_ERROR_CODE_UNKNOWN :
			
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE].GetString());

			#ifdef OUTPUT_DEBUG			
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_TRADE_ERROR_CODE_UNKNOWN");
				}
			#endif
		break;
		
		case GC_TRADE_ERROR_CODE_EVENT_GIFT_BOX:
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_EVENT_GIFT_BOX].GetString());
			break;
	}

	//----------------------------------------------------------------------
	// Trade Error인 경우는 무조건 교환창을 닫는다.
	//----------------------------------------------------------------------
	if (g_pTradeManager!=NULL)
	{
		g_pTradeManager->CancelTrade();
	}

	UI_CloseExchange();

#endif

	__END_CATCH
}
