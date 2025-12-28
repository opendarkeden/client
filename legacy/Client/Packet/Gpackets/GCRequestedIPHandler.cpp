//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRequestedIPHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRequestedIP.h"
//#include "MParty.h"
//#include "RequestFunction.h"

#include "RequestClientPlayerManager.h"
#include "RequestUserManager.h"
#include "WhisperManager.h"
#include "ClientDef.h"
#include "UserInformation.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRequestedIPHandler::execute ( GCRequestedIP * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
	
	if( g_pUserInformation->bKorean == false || 1 )
		return;
		
#ifdef __GAME_CLIENT__

	if (g_Mode==MODE_GAME
		&& g_pRequestUserManager!=NULL
		&& g_pRequestClientPlayerManager!=NULL)
	{
		const char* pName = pPacket->getName().c_str();

		struct in_addr sa;
		sa.S_un.S_addr = pPacket->getIP();

		const char* pIP = inet_ntoa( sa );

		// 정보 추가
		g_pRequestUserManager->AddRequestUser( pName, pIP, pPacket->getPort() );	


		switch (g_pRequestUserManager->GetRequestingFor(pName))
		{
			//--------------------------------------------------------
			// REQUESTING_FOR_WHISPER
			//--------------------------------------------------------
			// 귓속말을 보내기 위해 IP를 요청한 경우
			//--------------------------------------------------------
			case RequestUserManager::REQUESTING_FOR_WHISPER :
				if (g_pWhisperManager->HasWhisperMessage( pName ))
				{
					g_pRequestClientPlayerManager->Connect( pIP, 
															pName, 
															REQUEST_CLIENT_MODE_WHISPER );
				}
			break;

			//--------------------------------------------------------
			// REQUESTING_FOR_PROFILE
			//--------------------------------------------------------
			// Profile을 받기 위해 IP를 요청한 경우
			//--------------------------------------------------------
			case RequestUserManager::REQUESTING_FOR_PROFILE :
				g_pRequestClientPlayerManager->Connect( pIP, 
														pName, 
														REQUEST_CLIENT_MODE_PROFILE );
			break;
		}


		// 요청중인거 해제
		g_pRequestUserManager->RemoveRequestingUser( pName );	
	}

	//---------------------------------------------------------------
	// 일단은.. 파티를 위한 IP이므로.. 파티원만 체크를 해본다.	
	//---------------------------------------------------------------
	/*
	int num = g_pParty->GetSize();

	for (int i=0; i<num; i++)
	{
		PARTY_INFO*	pInfo = g_pParty->GetMemberInfo(i);

		if (pInfo!=NULL)
		{
			// 같은 이름이라면..
			if (pInfo->Name==pPacket->getName().c_str())				
			{
				// 일단 IP 저장
				pInfo->IP = pPacket->getIP().c_str();

				// 접속이 없다면 접속시도를 한다.
				if (!g_pRequestClientPlayerManager->HasConnection( pInfo->Name.GetString() ))
				{
					RequestConnect( pInfo->IP.c_str(), pInfo->Name.GetString() );
				}

				break;
			}
		}
	}	
	*/

#endif

	__END_CATCH
}
