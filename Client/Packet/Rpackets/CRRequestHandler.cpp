//////////////////////////////////////////////////////////////////////
//
// Filename    : CRRequestHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CRRequest.h"
#include "RequestServerPlayer.h"
#include "UserInformation.h"
#include "ProfileManager.h"
#include "RequestFileManager.h"
#include "RequestUserManager.h"
#include "Rpackets/RCRequestVerify.h"
#include "DebugInfo.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CRRequestHandler::execute ( CRRequest * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	DEBUG_ADD("CRRequestHandler");

	RequestServerPlayer* pRequestServerPlayer = dynamic_cast<RequestServerPlayer*>( pPlayer );

	// 이름이 없다면 .. IP로 설정한다.
	if (pRequestServerPlayer->getName().c_str()==NULL
		|| pRequestServerPlayer->getName().size()==0)
	{
		DEBUG_ADD("setName");
		pRequestServerPlayer->setName( pPlayer->getSocket()->getHost().c_str() );		
		DEBUG_ADD("setName OK");
	}
	
	const std::string& otherName =	pRequestServerPlayer->getName();


	if (pRequestServerPlayer!=NULL
		&& g_pUserInformation!=NULL
		&& g_pProfileManager!=NULL
		&& g_pRequestFileManager!=NULL
		&& g_pRequestUserManager!=NULL)
	{			
		DEBUG_ADD("Not NULL");

		// 사용자 정보 다시 설정
		//RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( otherName.c_str() );

		//if (pUserInfo!=NULL)
		{
		//	Socket* pSocket = pPlayer->getSocket();
		//	pUserInfo->IP = pSocket->getHost().c_str();
		//	pUserInfo->TCPPort = pSocket->getPort();
		}

		switch (pPacket->getCode())
		{
			//------------------------------------------------------------------
			//
			//			Profile을 요청했을 때,
			//
			//------------------------------------------------------------------
			case CR_REQUEST_FILE_PROFILE :
			{
				DEBUG_ADD("Request Profile");

				const char* pMyName = g_pUserInformation->CharacterID.GetString();

				if (pPacket->getRequestName()==pMyName)
				{
					const char* pFilename = g_pProfileManager->GetFilename( pMyName );

					//-------------------------------------------------------------
					// profile이 있는 경우
					//-------------------------------------------------------------
					if (pFilename != NULL)
					{
						DEBUG_ADD("profile not NULL");

						// 이미 요청한 내용이 없는 경우에.. 
						if (!g_pRequestFileManager->HasOtherRequest(otherName))
						{
							DEBUG_ADD("no Request");

							std::string filenameIndex = pFilename;
							filenameIndex += "i";

							RequestSendInfo* pInfo = new RequestSendInfo(otherName.c_str());							
							
							pInfo->AddSendFileInfo( new SendFileInfo(filenameIndex.c_str(), REQUEST_FILE_PROFILE_INDEX) );
							pInfo->AddSendFileInfo( new SendFileInfo(pFilename, REQUEST_FILE_PROFILE) );							

							DEBUG_ADD("add OtherReq");

							if (g_pRequestFileManager->AddOtherRequest(pInfo))
							{
								DEBUG_ADD("add otherReq OK");

								pRequestServerPlayer->setRequestMode(REQUEST_CLIENT_MODE_PROFILE);

								pRequestServerPlayer->setPlayerStatus(CPS_REQUEST_SERVER_NORMAL);
							}
							else
							{
								DEBUG_ADD("not add otherReq");

								//pRequestServerPlayer->disconnect( UNDISCONNECTED );

								DEBUG_ADD("disconnectExp");
								throw DisconnectException("Already Requested");
							}
						}					
					}
					//-------------------------------------------------------------
					// profile이 아예 없는 경우
					//-------------------------------------------------------------
					else
					{
						DEBUG_ADD("send RCRequestVerify");

						RCRequestVerify rcRequestVerify;
						rcRequestVerify.setCode( REQUEST_VERIFY_PROFILE_NOT_EXIST );

						pRequestServerPlayer->sendPacket( &rcRequestVerify );

						pRequestServerPlayer->setPlayerStatus( CPS_END_SESSION );
						//pRequestServerPlayer->disconnect( UNDISCONNECTED );

						DEBUG_ADD("disconnectExp");
						throw DisconnectException("No Profile");
					}
				}
				//-------------------------------------------------------------
				// 다른 사람의 profile을 요청하는 경우
				//-------------------------------------------------------------
				else
				{
					RCRequestVerify rcRequestVerify;
					rcRequestVerify.setCode( REQUEST_VERIFY_PROFILE_WRONG_USER );

					pRequestServerPlayer->sendPacket( &rcRequestVerify );

					pRequestServerPlayer->setPlayerStatus( CPS_END_SESSION );
					//pRequestServerPlayer->disconnect( UNDISCONNECTED );

					DEBUG_ADD("disconnectExp");
					throw DisconnectException("Wrong Profile requested");
				}
			}
			break;
		
			//------------------------------------------------------------------
			//
			//			길드 마크를 요청했을 때,
			//
			//------------------------------------------------------------------
			case CR_REQUEST_FILE_GUILD_MARK :

				DEBUG_ADD("Request GuildMark");

				pRequestServerPlayer->setRequestMode(REQUEST_CLIENT_MODE_GUILDMARK);
				pRequestServerPlayer->setPlayerStatus(CPS_REQUEST_SERVER_NORMAL);
			break;
		}
	}

	DEBUG_ADD("CRRequestHandler End");

#endif

	__END_CATCH
}
