//----------------------------------------------------------------------
//
// Filename    : RCPositionInfoHandler.cpp
// Written By  : Reiot
// Description :
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "RCPositionInfo.h"
#include "MParty.h"
#include "RequestUserManager.h"
#include "ClientDef.h"

#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
	extern CMessageArray*		g_pGameMessage;
#endif

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;


//----------------------------------------------------------------------
// 
// RCPositionInfoHander::execute()
// 
//----------------------------------------------------------------------
void RCPositionInfoHandler::execute ( RCPositionInfo * pPacket )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	//g_pClientCommunicationManager->sendPacket( pPacket->getHost() , pPacket->getPort() , &glIncomingConnectionOK );
	if ((g_Mode==MODE_GAME
			|| g_Mode==MODE_WAIT_UPDATEINFO			// 로딩 중이 아니거나..
			|| g_Mode==MODE_WAIT_SETPOSITION		// 좌표 기다리는 경우
			)
		&& g_pPlayer!=NULL
		&& g_pParty!=NULL
		&& g_pGameMessage!=NULL
		&& g_pRequestUserManager!=NULL)
	{
		// 정보 다시 설정
		RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pPacket->getName().c_str() );

		if (pUserInfo!=NULL)
		{
			pUserInfo->IP = pPacket->getHost().c_str();
			pUserInfo->UDPPort = pPacket->getPort();
		}

		PARTY_INFO*	pInfo = NULL;

		// 이름이 없는 경우엔 IP로 찾아본다.
		if (pPacket->getName().size()==0)
		{
			pInfo = g_pParty->GetMemberInfoByIP( pPacket->getHost().c_str() );
		}
		else
		{
			pInfo = g_pParty->GetMemberInfo( pPacket->getName().c_str() );
		}
			
		// 좌표를 수정해준다.
		if (pInfo!=NULL)
		{
			pInfo->zoneID = pPacket->getZoneID();
			pInfo->zoneX = pPacket->getZoneX();
			pInfo->zoneY = pPacket->getZoneY();

			int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

			int sight15 = g_pPlayer->GetSight() + (g_pPlayer->GetSight()>>1);

			if (pInfo->zoneID == pInfo->zoneID
				&&	
					(abs(g_pPlayer->GetX()-pInfo->zoneX) + abs(g_pPlayer->GetY()-pInfo->zoneY))
						<= sight15
				)
			{
				pInfo->bInSight = true;
			}
			else
			{
				pInfo->bInSight = false;
			}
			
			#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
				if (g_pGameMessage!=NULL)
				{
					g_pGameMessage->AddFormat("[RCMove] %s to zone:%d, (%d, %d)",
												pInfo->Name.GetString(),
												pInfo->zoneID,
												pInfo->zoneX,
												pInfo->zoneY);
				}
			#endif
		}
	}
		
	__END_CATCH
}

