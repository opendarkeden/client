//////////////////////////////////////////////////////////////////////
//
// Filename    : GCNPCSayHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCNPCSay.h"
#include "ClientDef.h"
#include "MChatManager.h"
#include "MNPCScriptTable.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCNPCSayHandler::execute ( GCNPCSay * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
		// Debug Message

	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		//---------------------------------------------------------------
		// 캐릭터 머리 위에 채팅을 띄운다.
		//---------------------------------------------------------------
		MCreature*	pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		if (pCreature==NULL)
		{
			// 그런 캐릭터가 없으면 무시한다.
		}
		else
		{		
			//-------------------------------------------------
			// 정상적인 대화
			//-------------------------------------------------
			if (g_pChatManager->IsAcceptID( pCreature->GetName() ))
			{
				int scriptID = pPacket->getScriptID();
				int subjectID = pPacket->getSubjectID();

				// 채팅~~
				char str[256];
				const char* pString = g_pNPCScriptTable->GetSubject( scriptID, subjectID );

				if (pString!=NULL)
				{
					strcpy(str, pString );
				
					pCreature->SetChatString( str );
				}
			}
			//-------------------------------------------------
			// 대화 거부로 설정되어 있다면 출력안한다.
			//-------------------------------------------------
			else
			{
				pCreature->SetChatString( "......" );
			}

			// history에 추가
			//char temp[128];
			//sprintf(temp, "%s> %s", pCreature->GetName(), str);
			//UI_AddChatToHistory( temp );
		}
	}
#endif

	__END_CATCH
}
