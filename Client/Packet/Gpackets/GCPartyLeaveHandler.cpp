//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyLeaveHandler.cpp
// Written By  : 김성민
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyLeave.h"
#include "MGameStringTable.h"
#include "MParty.h"
#include "UIDialog.h"
#include "UserInformation.h"
#include "ClientDef.h"
#include "UIFunction.h"

extern CMessageArray*		g_pGameMessage;

void GCPartyLeaveHandler::execute (GCPartyLeave * pPacket , Player * pPlayer)
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	if (g_pZone==NULL
		|| g_pPlayer==NULL
		|| g_pUserInformation==NULL
		|| g_pParty==NULL
		|| g_pGameMessage==NULL)
	{
		DEBUG_ADD("GCPartyLeaveHandler Failed");
		return;
	}

	
	const char* pExpeller = pPacket->getExpeller().c_str();	// 추방자
	const char* pExpellee = pPacket->getExpellee().c_str(); // 추방된 자

	char str[256];
			
	bool bRemovePartyCheck = false;
	
	//-------------------------------------------------------------------
	// 검증 확인
	//-------------------------------------------------------------------
	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY_LEAVE)
	{
		g_pPlayer->SetWaitVerifyNULL();
	}

	//-------------------------------------------------------------------
	// 누가 스스로 나간 경우
	//-------------------------------------------------------------------
	if (pExpeller==NULL || pExpeller[0]==NULL)
	{
		//----------------------------------------------------------
		// 내가 나간 경우
		//----------------------------------------------------------
		if (g_pUserInformation->CharacterID==pExpellee)
		{
			g_pParty->UnSetPlayerParty();
			g_pParty->Release();	// 나의 party가 해체되는 것이당..			

			g_pGameMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_REMOVE_PARTY_MYSELF].GetString() );
			//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_REMOVE_PARTY_MYSELF].GetString() );

			UI_CloseParty();
		}
		//----------------------------------------------------------
		// 누가 나간 경우
		//----------------------------------------------------------
		else if (g_pParty->RemoveMember( pExpellee ))
		{
			sprintf(str, 
					(*g_pGameStringTable)[STRING_MESSAGE_REMOVE_PARTY_HIMSELF].GetString(), 
					pExpellee);		
			
			//g_pUIDialog->PopupFreeMessageDlg( str );
			g_pGameMessage->Add( str );

			bRemovePartyCheck = true;
		}
	}
	//-------------------------------------------------------------------
	// 누가 누구를 쫓아낸 경우
	//-------------------------------------------------------------------
	else
	{
		//-------------------------------------------------------------------
		// 내가 pExpellee를 쫓아냈다.
		//-------------------------------------------------------------------
		if (g_pUserInformation->CharacterID==pExpeller)
		{		
			if (g_pParty->RemoveMember( pExpellee ))
			{
				sprintf(str, 
						(*g_pGameStringTable)[STRING_MESSAGE_KICK_PARTY_MEMBER_OK].GetString(), 
						pExpellee);

				//g_pUIDialog->PopupFreeMessageDlg( str );
				g_pGameMessage->Add( str );
			}

			bRemovePartyCheck = true;
		}
		//-------------------------------------------------------------------
		// 내가 pExpeller에게 쫓겨났다.
		//-------------------------------------------------------------------
		else if (g_pUserInformation->CharacterID==pExpellee)
		{
			g_pParty->UnSetPlayerParty();
			g_pParty->Release();	// 나의 party가 해체되는 것이당..			

			sprintf(str, 
						(*g_pGameStringTable)[STRING_MESSAGE_KICKED_FROM_PARTY].GetString(), 
						pExpeller);		
			
			//g_pUIDialog->PopupFreeMessageDlg( str );
			g_pGameMessage->Add( str );

			bRemovePartyCheck = true;
		}
		//-------------------------------------------------------------------
		// pExpeller가 pExpeller를 쫓아냈다.
		//-------------------------------------------------------------------
		else if (g_pParty->RemoveMember( pExpellee ))
		{
			sprintf(str, 
					(*g_pGameStringTable)[STRING_MESSAGE_KICK_PARTY_MEMBER].GetString(), 
					pExpeller, pExpellee);

			//g_pUIDialog->PopupFreeMessageDlg( str );
			g_pGameMessage->Add( str );
		}
	}

	
	//--------------------------------------------------
	// 파티 해체인가?
	//--------------------------------------------------
	if (bRemovePartyCheck && g_pParty->GetSize()==0)
	{		
		UI_CloseParty();

		// 메시지만 출력해주면 되겠지..
		//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_REMOVE_PARTY].GetString() );		
		g_pGameMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_REMOVE_PARTY].GetString() );
	}

#endif

	__END_CATCH
}
