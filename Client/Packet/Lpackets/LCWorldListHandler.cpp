//----------------------------------------------------------------------
//
// Filename    : LCWorldListHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCWorldList.h"
#include "ClientDef.h"
#include "ServerInformation.h"
#include "DebugInfo.h"
#include "UIFunction.h"

//----------------------------------------------------------------------
// 서버로부터 캐릭터 리스트를 받았다. 
// 이제 캐릭터 관리 인터페이스의 적절한 곳에 전송받은 값을 집어 넣어서
// 출력하자.
//----------------------------------------------------------------------
void LCWorldListHandler::execute ( LCWorldList * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	//-----------------------------------------------------------
	// Server Information 초기화
	//-----------------------------------------------------------
	if (g_pServerInformation==NULL)
	{
		g_pServerInformation = new ServerInformation;
	}
	else
	{
		g_pServerInformation->Release();
	}

	int currentID = pPacket->getCurrentWorldID();
	
	bool bExistDefault = false;

	//-----------------------------------------------------------
	// Server정보 생성
	//-----------------------------------------------------------
	int groupNum = pPacket->getListNum();
	int firstID = 0;

	for (int i=0; i<groupNum; i++)
	{
		WorldInfo* pWorldInfo = pPacket->popFrontListElement();

		if (pWorldInfo!=NULL)
		{
			if (i==0)
			{
				firstID = pWorldInfo->getID();
			}

			if (pWorldInfo->getID()==currentID)
			{
				bExistDefault = true;
			}

			//--------------------------------------------------------------
			// 새로운 ServerGroup의 정보 생성
			//--------------------------------------------------------------
			ServerGroup* pNewGroup = g_pServerInformation->GetData( pWorldInfo->getID() );
			
			if (pNewGroup==NULL)
			{
				pNewGroup = new ServerGroup;
				g_pServerInformation->AddData( pWorldInfo->getID(), pNewGroup );
			}

			// Group의 정보 설정			
			pNewGroup->SetGroupName( pWorldInfo->getName().c_str() );
			pNewGroup->SetGroupStatus( (int)pWorldInfo->getStat() );
			
			//--------------------------------------------------------------
			// 각각의 Server에 대한 정보 생성
			//--------------------------------------------------------------
			/*
			int serverNum = pWorldInfo->getListNum();

			for (int j=0; j<serverNum; j++)
			{
				SubServerInfo* pServerInfo = pWorldInfo->popFrontListElement();
				
				if (pServerInfo!=NULL)
				{
					//--------------------------------------------------------------
					// ServerGroup에 Server 추가
					//--------------------------------------------------------------
					SERVER_INFO* pNewServer = new SERVER_INFO;

					pNewServer->ServerName		= pServerInfo->getServerName().c_str();
					pNewServer->ServerStatus	= (SERVER_INFO::SERVER_STATUS)pServerInfo->getServerStat();
					
					pNewGroup->AddData( pServerInfo->getServerID(), pNewServer );					

					delete pServerInfo;
				}
				else
				{
					DEBUG_ADD( "[Error] SubServerGroupInfo is NULL" );
				}
			}
			*/

			delete pWorldInfo;
		}
		else
		{
			DEBUG_ADD( "[Error] ServerGroupInfo is NULL" );
		}	
	}

	// default 선택
	if (currentID==0 || !bExistDefault)
	{
		g_pServerInformation->SetServerGroupID( firstID );
	}
	else
	{
		g_pServerInformation->SetServerGroupID( currentID );
	}

	UI_SetWorldList();

	SetMode( MODE_WAIT_SELECT_WORLD );

#endif
		
	__END_CATCH
}
