//----------------------------------------------------------------------
//
// Filename    : RCSayHandler.cpp
// Written By  : Reiot
// Description :
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "RCSay.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "MChatManager.h"
#include "UIFunction.h"
#include "MGameStringTable.h"

//----------------------------------------------------------------------
// 
// RCSayHander::execute()
// 
//----------------------------------------------------------------------
void RCSayHandler::execute ( RCSay * pPacket )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	if ((g_Mode==MODE_GAME
			|| g_Mode==MODE_WAIT_UPDATEINFO			// 로딩 중이 아니거나..
			|| g_Mode==MODE_WAIT_SETPOSITION		// 좌표 기다리는 경우
			)
		&& g_pUserInformation!=NULL
		&& g_pChatManager!=NULL)
	{
		//g_pClientCommunicationManager->sendPacket( pPacket->getHost() , pPacket->getPort() , &glIncomingConnectionOK );
		char str[128];
		char strName[128];
		strcpy(str, pPacket->getMessage().c_str());
		strcpy(strName, pPacket->getName().c_str());

		//bool bMasterWords = (strstr(strName, "GM")!=NULL);
		bool bMasterWords = strncmp( strName, (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0;

		if (bMasterWords 
			|| g_pChatManager->IsAcceptID( strName ))
		{
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
				g_pChatManager->RemoveCurse( str );

				/*
				// RCSay에는 mask를 씌우지 않는다.
				#ifndef _DEBUG
					//--------------------------------------------------
					// 종족이 다른 경우
					//--------------------------------------------------
					bool bVampireSay = pPacket->getRace();
					if (g_pPlayer->IsSlayer() && bVampireSay)
					{
						// INT는 150까지이므로..  
						int percent = min(75, 25+g_pPlayer->GetINT()*100/150);
						g_pChatManager->AddMask(str, percent);
					}
					else if (g_pPlayer->IsVampire() && !bVampireSay)
					{
						// INT는 300까지이므로..  
						int percent = min(75, 25+g_pPlayer->GetINT()*100/300);
						g_pChatManager->AddMask(str, percent);
					}
				#endif
				*/
			}

			//sprintf(str, "<%s> %s", pPacket->getName().c_str(), pPacket->getMessage().c_str());
			// party = 3
			UI_AddChatToHistory( str, strName, 3, pPacket->getColor() );

			// 귓속말 대상 설정 ID+' '
			//char strWhisperID[128];
			//sprintf(strWhisperID, "%s ", pPacket->getName().c_str());
			//g_pUserInformation->WhisperID = strWhisperID;

			// [도움말] 귓속말 받을 때
//			__BEGIN_HELP_EVENT
//
//				//ExecuteHelpEvent( HE_CHAT_WHISPERED );	
//			__END_HELP_EVENT
		}
	}
		
	__END_CATCH
}
