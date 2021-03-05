//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyInviteHandler.cpp
// Written By  : 김성민
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyInvite.h"
#include "CPackets\CGPartyInvite.h"
#include "ClientPlayer.h"
#include "MGameStringTable.h"
#include "UIDialog.h"
#include "TempInformation.h"
#include "MParty.h"
#include "ClientDef.h"
#include "UIFunction.h"
#include "MHelpDef.h"
extern CMessageArray*		g_pGameMessage;

void GCPartyInviteHandler::execute (GCPartyInvite * pPacket , Player * pPlayer)
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	if (g_pZone==NULL
		|| g_pTempInformation==NULL
		|| g_pUIDialog==NULL
		|| g_pPlayer==NULL
		|| g_pParty==NULL
		|| g_pGameMessage==NULL)
	{
		DEBUG_ADD("GCPartyInviteHandler Failed");
		return;
	}

	int targetID = pPacket->getTargetObjectID();

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);
			
	
	bool bCancelAll = false;
	bool bMessage = false;
	int cancelMessage = 0;
	bool bJoinedSomeone = false;

	switch (pPacket->getCode())
	{
		//--------------------------------------------------------------------
		//
		// 누가 나를 초대한다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_REQUEST :
			//----------------------------------------------------------------------
			// 이미 내가 다른 누구한테 거래하자고 한 경우 
			//----------------------------------------------------------------------
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY
				&& g_pTempInformation->GetMode()==TempInformation::MODE_PARTY_REQUEST)
			{
				int code;

				//----------------------------------------------------------------------
				// 그 사람이 나한테 파티 신청한 경우 --> OK
				//----------------------------------------------------------------------
				if (g_pTempInformation->Value1==targetID)
				{
					code = CG_PARTY_INVITE_ACCEPT;

					g_pTempInformation->SetMode(TempInformation::MODE_NULL);

					//-------------------------------------------
					// 파티 성공
					//-------------------------------------------
					// UI_RunParty();
					// 파티 성공 패킷을 기다린다.
					UI_ClosePartyCancel();

					bJoinedSomeone = true;
					
					//2004, 5, 6 sobeit add start
					ExecuteHelpEvent(HELP_EVENT_PARTY);
					//2004, 5, 6 sobeit add end
				}
				//----------------------------------------------------------------------
				// 아닌 경우
				//----------------------------------------------------------------------
				else
				{
					code = CG_PARTY_INVITE_BUSY;
				}

				CGPartyInvite _CGPartyInvite;
				_CGPartyInvite.setTargetObjectID( targetID );
				_CGPartyInvite.setCode( code );				

				pClientPlayer->sendPacket( &_CGPartyInvite );

			}
			//----------------------------------------------------------------------
			// 파티할까? Y/N
			//----------------------------------------------------------------------
			else
			{
				//-------------------------------------------
				// 파티에 들어오남?
				//-------------------------------------------
				if (g_pPlayer->IsWaitVerify() && g_pPlayer->GetWaitVerify()!=MPlayer::WAIT_VERIFY_PARTY			// 검증 받아야 하는게 있는 경우
					//|| g_pPlayer->IsRepeatAction()		// 반복 행동 중인 경우
					|| g_pUIDialog->IsLockInput()		// NPC랑 대화중..
					|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT	// 박쥐인 경우
					|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF	// 늑대인 경우
					|| !g_pParty->IsAcceptInvite()
					)
				{
					// 파티 거부
					CGPartyInvite _CGPartyInvite;
					_CGPartyInvite.setTargetObjectID( targetID );
					_CGPartyInvite.setCode( CG_PARTY_INVITE_REJECT );

					pClientPlayer->sendPacket( &_CGPartyInvite );
				}
				else
				{
					UI_RunPartyRequest( targetID );
				}
			}
		break;

		//--------------------------------------------------------------------
		//
		// 나를 초대할려던 애가 초대를 취소했다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_CANCEL :
			if (g_pTempInformation->GetMode() == TempInformation::MODE_PARTY_REQUEST)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}		

			UI_ClosePartyRequest();
			UI_ClosePartyAsk();			
		break;

		//--------------------------------------------------------------------
		//
		// 내가 초대한 애가 파티에 들어왔다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_ACCEPT :
			//----------------------------------------------------------------------
			// 이미 내가 다른 누구한테 파티하자고 한 경우 
			//----------------------------------------------------------------------
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY
				&& g_pTempInformation->GetMode()==TempInformation::MODE_PARTY_REQUEST)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
			
			bJoinedSomeone = true;

			UI_ClosePartyCancel();			

			//2004, 5, 6 sobeit add start
			ExecuteHelpEvent(HELP_EVENT_PARTY);
			//2004, 5, 6 sobeit add end	
		break;

		//--------------------------------------------------------------------
		//
		// 내가 초대한 애가 파티에 들어오길 거부했다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_REJECT :
			bCancelAll = true;
			bMessage = true;
			cancelMessage = STRING_MESSAGE_PARTY_REJECTED;			
		break;

		//--------------------------------------------------------------------
		//
		// 내가 초대한 애가 이미 다른 사람의 초대에 응답하는 중이다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_BUSY :
			bCancelAll = true;
			bMessage = true;
			cancelMessage = STRING_MESSAGE_PARTY_BUSY;
		break;

		//--------------------------------------------------------------------
		//
		// 내가 초대한 애가 이미 다른 파티에 속해있다.
		//
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_ANOTHER_PARTY :
		{
			MCreature* pCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );
			
			if ( pCreature!=NULL )
			{
				// player의 파티가 아닌 경우에만 메시지를 출력해준다.
				if (pCreature->IsPlayerParty())
				{
					bCancelAll = true;
					bMessage = false;
				}
				else
				{
					bCancelAll = true;
					bMessage = true;
					cancelMessage = STRING_MESSAGE_IN_ANOTHER_PARTY;					
				}
			}
		}
		break;

		//--------------------------------------------------------------------
		//
		// 꽉 찼당.
		// 
		//--------------------------------------------------------------------
		case GC_PARTY_INVITE_MEMBER_FULL :
			bCancelAll = true;
			bMessage = true;
			cancelMessage = STRING_MESSAGE_PARTY_FULL;
		break;
	}

	//-------------------------------------------
	// 파티 확인
	//-------------------------------------------
	if (bJoinedSomeone)
	{
		MCreature* pCreature = g_pZone->GetCreature( g_pTempInformation->PartyInviter );

		if (pCreature!=NULL)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_SOMEONE_JOINED_PARTY].GetString(), pCreature->GetName());
		}
	}

	//--------------------------------------------------------------------
	// 다 닫고 message 띄우는 경우
	//--------------------------------------------------------------------
	if (bCancelAll)
	{
		if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY
			&& g_pTempInformation->GetMode()==TempInformation::MODE_PARTY_REQUEST)
		{			
			// 검증용 data제거
			g_pPlayer->SetWaitVerifyNULL();
			g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		}

		// 파티취소할래?를 닫는다.
		UI_ClosePartyCancel();

		UI_ClosePartyAsk();
		UI_ClosePartyRequest();

		if (bMessage)
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[cancelMessage].GetString());		
		}
	}

#endif

	__END_CATCH
}
