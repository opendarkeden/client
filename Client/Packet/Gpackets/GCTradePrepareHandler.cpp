//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTradePrepareHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTradePrepare.h"
#include "ClientDef.h"
#include "MGameStringTable.h"
#include "UIDialog.h"
#include "TempInformation.h"
#include "MTradeManager.h"

#ifdef __GAME_CLIENT__

	#include "ClientPlayer.h"
	#include "CPackets/CGTradePrepare.h"
	#include "UIFunction.h"

#endif


void GCTradePrepareHandler::execute ( GCTradePrepare * pPacket , Player * pPlayer )
	 


throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	ObjectID_t targetID = pPacket->getTargetObjectID();

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);
		
	//----------------------------------------------------------------------
	//
	//			누가 나한테 거래하자고 신청하는 경우이다.
	//
	//----------------------------------------------------------------------
	// 이미 거래 중인 경우... 등등.. 뭔가 하고 있으면.. --> 거부
	//----------------------------------------------------------------------
	if (g_pPlayer->IsWaitVerify() && g_pPlayer->GetWaitVerify()!=MPlayer::WAIT_VERIFY_TRADE			// 검증 받아야 하는게 있는 경우
		|| g_pPlayer->IsRepeatAction()		// 반복 행동 중인 경우
		|| g_pUIDialog->IsLockInput()		// NPC랑 대화중..
		|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT	// 박쥐인 경우
		|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF	// 늑대인 경우
		)
	{
		// 교환 거부
		CGTradePrepare _CGTradePrepare;
		_CGTradePrepare.setTargetObjectID( targetID );
		_CGTradePrepare.setCode( CG_TRADE_PREPARE_CODE_REJECT );

		pClientPlayer->sendPacket( &_CGTradePrepare );

		return;
	}


	// 이미 다른 누가 나한테 거래하자고 해버린 경우 --> 거부 [서버에서 처리]
	switch (pPacket->getCode())
	{
		//----------------------------------------------------------------------
		//
		// 다른 사람이 나한테 교환 신청할때
		//
		//----------------------------------------------------------------------
		case GC_TRADE_PREPARE_CODE_REQUEST :
		{
			bool bLookMe = false;

			//----------------------------------------------------------------------
			// 이미 내가 다른 누구한테 거래하자고 한 경우 
			//----------------------------------------------------------------------
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE
				&& g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_REQUEST)
			{
				int code;

				//----------------------------------------------------------------------
				// 그 사람이 나한테 거래 신청한 경우 --> OK
				//----------------------------------------------------------------------
				if (g_pTempInformation->Value1==targetID)
				{
					code = CG_TRADE_PREPARE_CODE_ACCEPT;

					g_pTempInformation->SetMode(TempInformation::MODE_NULL);

					//-------------------------------------------
					// 교환창 띄운다.
					//-------------------------------------------
					UI_RunExchange( targetID );

					bLookMe = true;
				}
				//----------------------------------------------------------------------
				// 아닌 경우
				//----------------------------------------------------------------------
				else
				{
					code = CG_TRADE_PREPARE_CODE_BUSY;
				}

				CGTradePrepare _CGTradePrepare;
				_CGTradePrepare.setTargetObjectID( targetID );
				_CGTradePrepare.setCode( code );				

				pClientPlayer->sendPacket( &_CGTradePrepare );

			}
			//----------------------------------------------------------------------
			// 교환할까? Y/N
			//----------------------------------------------------------------------
			else
			{
				//-------------------------------------------
				// 교환할까?
				//-------------------------------------------
				UI_RunExchangeAsk( targetID );	
				
				bLookMe = true;
			}

			//----------------------------------------------------------------------
			// 다른 캐릭터가 나를 바라보는 경우
			//----------------------------------------------------------------------
			if (bLookMe)
			{
				MCreature* pCreature = g_pZone->GetCreature( targetID );
				
				if (pCreature!=NULL)
				{
					pCreature->SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] No Such Creature. id=%d", targetID);
				}
			}
		}
		break;

		//----------------------------------------------------------------------
		//
		// 교환 신청한 사람이 교환 취소한 경우
		//
		//----------------------------------------------------------------------
		case GC_TRADE_PREPARE_CODE_CANCEL :

			if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}		

			// 이미 내가 OK 눌러서 교환창이 떠 있는 경우
			if (UI_IsRunningExchange())
			{
				UI_CloseExchange();
			}
			else
			{
				UI_CloseExchangeAsk();
			}
		break;

		//----------------------------------------------------------------------
		//
		// 내 교환신청이 받아들여질때
		//
		//----------------------------------------------------------------------
		case GC_TRADE_PREPARE_CODE_ACCEPT :
			//----------------------------------------------------------------------
			// 이미 내가 다른 누구한테 거래하자고 한 경우 
			//----------------------------------------------------------------------
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE
				&& g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_REQUEST)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				//-------------------------------------------
				// 교환창을 띄운다.
				//-------------------------------------------
				UI_RunExchange( targetID );
			}

		break;


		//----------------------------------------------------------------------
		//
		// 내 교환신청이 거부될때
		//
		//----------------------------------------------------------------------
		case GC_TRADE_PREPARE_CODE_REJECT :
		{
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE
				&& g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_REQUEST)
			{			
				// 검증용 data제거
				g_pPlayer->SetWaitVerifyNULL();
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// 교환취소할래?를 닫는다.
				UI_CloseExchangeCancel();

				// 거래 거부 dialog
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_REJECTED].GetString());
			}
		}
		break;

		//----------------------------------------------------------------------
		//
		// 교환 신청한 사람이 현재 교환 중이다. (혹은 딴거한다..)
		//
		//----------------------------------------------------------------------
		case GC_TRADE_PREPARE_CODE_BUSY :
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE
				&& g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_REQUEST)
			{			
				// 검증용 data제거
				g_pPlayer->SetWaitVerifyNULL();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);				

				UI_CloseExchangeAsk();
				UI_CloseExchangeCancel();

				// busy라서 교환 안됨
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_BUSY].GetString());
			}
		break;

	}

#endif

	__END_CATCH
}
