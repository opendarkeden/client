//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSystemMessageHandler.cc
// Written By  : elca
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSystemMessage.h"
#include "ClientDef.h"
#include "UIFunction.h"
#include "MGameStringTable.h"
#include "Client.h"
//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCSystemMessageHandler::execute ( GCSystemMessage * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	static char previous1[128] = { NULL, };
	switch(pPacket->getType())
	{ 
		case SYSTEM_MESSAGE_HOLY_LAND :		// 아담의 성지 관련
			if(g_pUserOption->DoNotShowHolyLandMsg)
				return;
			break;

		case SYSTEM_MESSAGE_NORMAL:
			break;

		case SYSTEM_MESSAGE_OPERATOR:	// 운영자 말씀
			break;
	
		case SYSTEM_MESSAGE_MASTER_LAIR:	// 마스터 레어 관련
			if(g_pUserOption->DoNotShowLairMsg)
				return;
			break;

		case SYSTEM_MESSAGE_COMBAT:		// 전쟁 관련
			if(g_pUserOption->DoNotShowWarMsg)
				return;
			break;
	
		case SYSTEM_MESSAGE_INFO: 		// 특정한 정보 관련
			break;
			
		case SYSTEM_MESSAGE_RANGER_CHAT:
			{
				char* message = (char*)pPacket->getMessage().c_str();
				if(NULL != message)
				{
					UI_SetRangerChatString(message);
				}
			}
			return;
	
		case SYSTEM_MESSAGE_PLAYER:	    // add by Coffee 2007-8-2 藤속鯤소랙箇무멩
			char* message = (char*)pPacket->getMessage().c_str();

			if (NULL != message)
			{
				message = (char*)pPacket->getMessage().c_str();
// 				if (strcmp(previous1, message)==0)
// 				{
// 					BOOL bExist = FALSE;
// 
// 					//--------------------------------------------------------------------
// 					// 이미 있는 메세지인지 검사한다.
// 					//--------------------------------------------------------------------
// 					for (int i=0; i<g_pPlayerMessage->GetSize(); i++)
// 					{
// 						if (strcmp((*g_pPlayerMessage)[i], message)==0)
// 						{
// 							bExist = TRUE;
// 						}
// 					}
// 
// 					//--------------------------------------------------------------------
// 					// 없는거면 추가한다.		
// 					//--------------------------------------------------------------------
// 					if (!bExist)
// 					{
// 						g_pPlayerMessage->Add( message );
// 					}
// 				}
// 				//--------------------------------------------------------------------
// 				// 새로운 메세지이면 추가한다.
// 				//--------------------------------------------------------------------
// 				else
// 				{
					g_pPlayerMessage->Add( message );

					strcpy( previous1, pPacket->getMessage().c_str() );
//				}
			}
			return;

			
	}

	static char previous[128] = { NULL, };

	const char* message = pPacket->getMessage().c_str();
	// add by Coffee 2007-8-2 藤속溝固斤口혐깎
		char *pMsg = NULL;
		if (message!=NULL && pPacket->getType() != SYSTEM_MESSAGE_PLAYER )
		{
			pMsg = new char[strlen(message)+20];
			sprintf(pMsg,(*g_pGameStringTable)[UI_STRING_MESSAGE_SYSTEM].GetString(),message);
			pPacket->setMessage(pMsg);
			SAFE_DELETE_ARRAY( pMsg );
		}
		message = pPacket->getMessage().c_str();
	// add end by Coffee 2007-8-2
	//--------------------------------------------------------------------
	// system message에 출력
	//--------------------------------------------------------------------
	if (strcmp(previous, message)==0)
	{
		BOOL bExist = FALSE;

		//--------------------------------------------------------------------
		// 이미 있는 메세지인지 검사한다.
		//--------------------------------------------------------------------
		for (int i=0; i<g_pSystemMessage->GetSize(); i++)
		{
			if (strcmp((*g_pSystemMessage)[i], message)==0)
			{
				bExist = TRUE;
			}
		}

		//--------------------------------------------------------------------
		// 없는거면 추가한다.		
		//--------------------------------------------------------------------
		if (!bExist)
		{
			g_pSystemMessage->Add( message );
		}
	}
	//--------------------------------------------------------------------
	// 새로운 메세지이면 추가한다.
	//--------------------------------------------------------------------
	else
	{
		g_pSystemMessage->Add( message );

		strcpy( previous, pPacket->getMessage().c_str() );
	}

#endif

	__END_CATCH
}
