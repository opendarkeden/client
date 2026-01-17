//////////////////////////////////////////////////////////////////////
//
// Filename    : CRConnectHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CRConnect.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "RequestServerPlayer.h"
#include "RCConnectVerify.h"


#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
	extern CMessageArray*		g_pGameMessage;
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CRConnectHandler::execute ( CRConnect * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	bool bConnected = false;

	RequestServerPlayer* pRequestServerPlayer = dynamic_cast<RequestServerPlayer*>( pPlayer );

	if (pRequestServerPlayer!=NULL
		&& g_pUserInformation!=NULL
		&& g_pGameMessage!=NULL)
	{
		// 접속을 허용시켜줄까?
		if (g_Mode==MODE_GAME)
		{
			// 제대로 접속한게 맞남..
			if (pPacket->getRequestServerName()==g_pUserInformation->CharacterID.GetString())
			{
				// 접속한 애 이름을 설정해둔다.
				pRequestServerPlayer->setName( pPacket->getRequestClientName().c_str() );

				pRequestServerPlayer->setPlayerStatus( CPS_REQUEST_SERVER_NORMAL );

				// 접속 확인 packet을 보낸다.
				RCConnectVerify _RCConnectVerify;
				_RCConnectVerify.setCode( RC_CONNECT_OK );
				pRequestServerPlayer->sendPacket( &_RCConnectVerify );

				#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
					if (g_pGameMessage!=NULL)
						g_pGameMessage->AddFormat("Connection Grant to %s", pRequestServerPlayer->getName().c_str());
				#endif
				

				bConnected = true;
			}
		}

			
		if (!bConnected)
		{
			// 접속 확인 packet을 보낸다.
			RCConnectVerify _RCConnectVerify;
			_RCConnectVerify.setCode( RC_CONNECT_REJECT );
			pRequestServerPlayer->sendPacket( &_RCConnectVerify );

			pRequestServerPlayer->setPlayerStatus( CPS_END_SESSION );

			throw DisconnectException("Reject");
			//pRequestServerPlayer->disconnect( UNDISCONNECTED );
		}
	}
	else
	{
		throw DisconnectException("Can't connect");
		//pRequestServerPlayer->disconnect( UNDISCONNECTED );
	}


#endif

	__END_CATCH
}
