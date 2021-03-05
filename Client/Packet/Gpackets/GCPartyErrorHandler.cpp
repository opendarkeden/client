//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyErrorHandler.cpp
// Written By  : 김성민
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyError.h"
#include "ClientDef.h"
#include "UIDialog.h"
#include "MGameStringTable.h"
#include "TempInformation.h"
#include "UIFunction.h"

void GCPartyErrorHandler::execute (GCPartyError * pPacket , Player * pPlayer)
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	if (g_pUIDialog==NULL
		|| g_pPlayer==NULL
		|| g_pTempInformation==NULL)
	{
		DEBUG_ADD("GCPartyErrorHandler Failed");
		return;
	}

	int code = pPacket->getCode();

	//----------------------------------------------------------------------
	//
	//	파티 취소되는 경우의 공통적인 처리
	//
	//----------------------------------------------------------------------
	if (code==GC_PARTY_ERROR_TARGET_NOT_EXIST
		|| code==GC_PARTY_ERROR_RACE_DIFFER
		|| code==GC_PARTY_ERROR_NOT_SAFE)
	{	
		if (g_pTempInformation->GetMode() == TempInformation::MODE_PARTY_REQUEST)
		{
			// 파티취소할래?를 닫는다.
			UI_ClosePartyCancel();

			// 검증 제거..
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY)
			{
				g_pPlayer->SetWaitVerifyNULL();
			}

			g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		}
	}

	switch (code)
	{
		//----------------------------------------------------------------------
		// 파티을 요구한 대상이 존재하지 않는다
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_TARGET_NOT_EXIST :						
			// 그런 사람 엄따~
			UI_ClosePartyCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_PARTY_NOBODY].GetString());
		break;
		
		
		//----------------------------------------------------------------------
		// 파티을 요구한 대상이 다른 종족이다
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_RACE_DIFFER :
			// 파티할 수 엄따~
			UI_ClosePartyCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_RACE_DIFFER].GetString());
		break;

		//----------------------------------------------------------------------
		// 파티을 하려고 하는 곳이 안전 지대가 아니다.
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_NOT_SAFE :
			// 파티할 수 엄따~
			UI_ClosePartyCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_PARTY_SAFETY_ZONE_ONLY].GetString());			
		break;		

		//----------------------------------------------------------------------
		// 변신 중
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_NOT_NORMAL_FORM :
			// 파티할 수 엄따~
			UI_ClosePartyCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_NOT_NORMAL_FORM].GetString());			
		break;
		

		//----------------------------------------------------------------------
		// 파티 중이면서 다시 파티을 하려고 한다
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_ALREADY_INVITING :
			
			UI_ClosePartyCancel();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_PARTY_BUSY].GetString());
		break;

		//----------------------------------------------------------------------
		// 파티 중이 아닌데 : 파티 관련 패킷이 날아왔다.
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_NOT_INVITING :
			// - -;
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] Server is Not Party Mode");
				}
			#endif
		break;	

		//----------------------------------------------------------------------
		// 파티원을 추방할 수 있는 권한이 없다.
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_NO_AUTHORITY :
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_NO_AUTHORITY].GetString());

			#ifdef OUTPUT_DEBUG			
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_PARTY_ERROR_NO_AUTHORITY");
				}
			#endif
		break;

		//----------------------------------------------------------------------
		// 알 수 없는 에러이다...
		//----------------------------------------------------------------------
		case GC_PARTY_ERROR_UNKNOWN :
			
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_ERROR_PARTY].GetString());

			#ifdef OUTPUT_DEBUG			
				if (g_pDebugMessage)
				{
					DEBUG_ADD("[Error] GC_PARTY_ERROR_UNKNOWN");
				}
			#endif
		break;
	}

#endif

	 __END_CATCH
}
