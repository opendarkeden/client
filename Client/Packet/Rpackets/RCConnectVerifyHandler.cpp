//////////////////////////////////////////////////////////////////////
//
// Filename    : RCConnectVerifyHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "RCConnectVerify.h"
#include "RequestClientPlayer.h"
//#include "CRRequest.h"
//#include "MZone.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void RCConnectVerifyHandler::execute ( RCConnectVerify * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	RequestClientPlayer* pRequestClientPlayer = dynamic_cast<RequestClientPlayer*>( pPlayer );

	if (pRequestClientPlayer!=NULL)
	{
		switch (pPacket->getCode())
		{
			//---------------------------------------------------------------
			// 접속이 허용된 경우
			//---------------------------------------------------------------
			case RC_CONNECT_OK :
				pRequestClientPlayer->setPlayerStatus( CPS_REQUEST_CLIENT_NORMAL );

				// 현재 player의 시야에 있는지 없는지를 체크해서 
				// 없다면 좌표 요청을 한다.
				/*
				if (OBJECTID_NULL != g_pZone->GetCreatureID( pRequestClientPlayer->getRequestServerName().c_str() ))
				{
					// 있는 경우					
				}
				else
				{
					// 없는 경우
					// 지속적인 좌표 요청을 한다.
					CRRequest _CRRequest;
					_CRRequest.setCode( CR_REQUEST_POSITION_REPEATLY );

					pRequestClientPlayer->sendPacket( &_CRRequest );
				}
				*/

			break;

			//---------------------------------------------------------------
			// 접속이 안되거나 거부된 경우
			//---------------------------------------------------------------
			case RC_CONNECT_REJECT :
			case RC_CONNECT_WRONG :
				pRequestClientPlayer->setPlayerStatus( CPS_END_SESSION );
				//pRequestClientPlayer->disconnect(UNDISCONNECTED);
				throw DisconnectException("Reject or Wrong");
			break;
		}
	}

#endif

	__END_CATCH
}
