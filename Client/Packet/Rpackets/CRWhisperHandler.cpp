//////////////////////////////////////////////////////////////////////
//
// Filename    : CRWhisperHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CRWhisper.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "MChatManager.h"
#include "RequestServerPlayer.h"
#include "RequestUserManager.h"
#include "Rpackets\RCRequestVerify.h"
#include "UIFunction.h"
#include "MGameStringTable.h"

extern CMessageArray*		g_pNoticeMessage;

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void CRWhisperHandler::execute ( CRWhisper * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	
	RequestServerPlayer* pRequestServerPlayer = dynamic_cast<RequestServerPlayer*>( pPlayer );
		
	//-------------------------------------------------------------------
	// 나한테 오는게 맞는지 확인한다.
	//-------------------------------------------------------------------
	if (g_Mode!=MODE_GAME						// 게임 중이 아니거나
		&& g_Mode!=MODE_WAIT_UPDATEINFO			// 로딩 중이 아니거나..
		&& g_Mode!=MODE_WAIT_SETPOSITION		// 좌표 기다리는 경우
		|| g_pPlayer==NULL
		|| g_pChatManager==NULL
		|| g_pUserInformation==NULL 
		|| g_pUserInformation->CharacterID.GetString()==NULL
		|| pPacket->getTargetName() != g_pUserInformation->CharacterID.GetString()
		|| pPacket->getWorldID() != g_pUserInformation->WorldID)
	{		
		// 아니면 ...
		RCRequestVerify _RCRequestVerify;
		_RCRequestVerify.setCode( REQUEST_VERIFY_WHISPER_FAILED );
		pRequestServerPlayer->sendPacket( &_RCRequestVerify );

		// 보내고
		try {
			pRequestServerPlayer->processOutput();
		} catch (Throwable& t)	{
			DEBUG_ADD_ERR(t.toString().c_str());
		}	
	}
	else
	{
		//-------------------------------------------------------------------
		// 귓말 보낸 애의 IP를 기억해두자.
		//-------------------------------------------------------------------
		// 사용자 정보 다시 설정

		if (!g_pRequestUserManager->HasRequestUser(pPacket->getName().c_str()))
		{
			const char* pIP = pPlayer->getSocket()->getHost().c_str();

			g_pRequestUserManager->AddRequestUser( pPacket->getName().c_str(), pIP );
		}

		// IP, Port 다시 설정.
		//RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pPacket->getName().c_str() );

		//if (pUserInfo!=NULL)
		{
		//	Socket* pSocket = pPlayer->getSocket();
		//	pUserInfo->IP = pSocket->getHost().c_str();
			//pUserInfo->TCPPort = pSocket->getPort();
		}


		int numMessage = pPacket->getMessageSize();

		char str[128];
		char strName[128];	
		strcpy(strName, pPacket->getName().c_str());

		//bool bMasterWords = (strstr(strName, "GM")!=NULL);
		bool bMasterWords = strncmp( strName, (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0;
		
		if (bMasterWords 
			|| g_pChatManager->IsAcceptID( strName ))
		{
			//--------------------------------------------------
			// message가 여러개일 수 있당..
			//--------------------------------------------------
			for (int i=0; i<numMessage; i++)
			{
				WHISPER_MESSAGE* pString = pPacket->popMessage();

				if (pString==NULL)
				{
					break;
				}

				strcpy(str, pString->msg.c_str());

				//--------------------------------------------------
				// 욕 제거
				// 운영자가 한 말도 아니고 나도 운영자가 아니면 filter한다.
				// --> 운영자의 말은 다 보이고 운영자는 다 본다.
				//--------------------------------------------------
				if (!bMasterWords && !g_pUserInformation->IsMaster 
					&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
				&& !g_bLight
#endif
					)
				{
					g_pChatManager->RemoveCurse( str, true );

					#ifndef _DEBUG
						//--------------------------------------------------
						// 종족이 다른 경우
						//--------------------------------------------------
						Race race = (Race)pPacket->getRace();
						if (g_pPlayer->GetRace() != race)
						{
							// INT는 150까지이므로..  
							int percent = min(75, 25+g_pPlayer->GetINT()*100/(min(2, g_pPlayer->GetRace()+1)*150));
							g_pChatManager->AddMask(str, percent);
						}
//						else if (g_pPlayer->IsVampire() && !bVampireSay)
//						{
//							// INT는 300까지이므로..  
//							int percent = min(75, 25+g_pPlayer->GetINT()*100/300);
//							g_pChatManager->AddMask(str, percent);
//						}
					#endif
				}

				//sprintf(str, "<%s> %s", pPacket->getName().c_str(), pPacket->getMessage().c_str());
				// whisper = 2
				UI_AddChatToHistory( str, strName, 2, pString->color );
				
				if ( bMasterWords )
				{
					// 운영자일경우 시스템 메시지로도 찍어준다.
					g_pNoticeMessage->AddFormat("%s>%s",strName,str);
				}

				// 귓속말 대상 설정 ID+' '
				char strWhisperID[128];
				sprintf(strWhisperID, "%s ", pPacket->getName().c_str());
				g_pUserInformation->WhisperID = strWhisperID;

				// [도움말] 귓속말 받을 때
//				__BEGIN_HELP_EVENT
//					ExecuteHelpEvent( HE_CHAT_WHISPERED );	
//				__END_HELP_EVENT


				delete pString;
			}
		}
	}

	//-------------------------------------------------------------------
	// 끊는다.
	//-------------------------------------------------------------------
	throw DisconnectException("Disconnect after Whisper");
	/*
	try {

		pRequestServerPlayer->disconnect( UNDISCONNECTED );

	} catch (Throwable& t)	{
		DEBUG_ADD(t.toString().c_str());
	}
	*/

#endif

	__END_CATCH
}
