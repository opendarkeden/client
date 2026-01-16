//////////////////////////////////////////////////////////////////////
//
// Filename    : GCNPCAskDynamicHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCNPCAskDynamic.h"
#include "ClientDef.h"
#include "TalkBox.h"
#include "UIDialog.h"
#include "SystemAvailabilities.h"


//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCNPCAskDynamicHandler::execute ( GCNPCAskDynamic * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

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
		if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK)
		{
			g_pPlayer->SetWaitVerifyNULL();

			int size = pPacket->getContentsCount();

			if (size==0)
			{
				DEBUG_ADD_FORMAT("[Error] No Answer String.. ScriptID=%d", (int)pPacket->getScriptID());			
			}
			else
			{
				// NPC
				MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

				if (pCreature==NULL)
				{
					// 그런 캐릭터가 없으면 무시한다.
				}
				else
				{
					//---------------------------------------------------
					// g_PCTalkBox에 추가하면 된다.
					//---------------------------------------------------
					// 기존에 있던것 제거
					g_pPCTalkBox->Release();

					//---------------------------------------------------
					// normal
					//---------------------------------------------------
					g_pPCTalkBox->SetType( PCTalkBox::NORMAL );

					//---------------------------------------------------
					// PC Talk Box의 정보 설정
					//---------------------------------------------------
					g_pPCTalkBox->SetContent( pPacket->getSubject().c_str() );
					g_pPCTalkBox->SetNPCID( pPacket->getObjectID() );
					g_pPCTalkBox->SetCreatureType( pCreature->GetCreatureType() );
					g_pPCTalkBox->SetScriptID( pPacket->getScriptID() );

					//---------------------------------------------------
					// 각 std::string 추가
					//---------------------------------------------------

					g_pPCTalkBox->m_AnswerIDMap.clear();
					for (int i=0; i<size; i++)
					{
						// g_PCTalkBox에 추가
						if( g_pSystemAvailableManager->ScriptFiltering( pPacket->getScriptID(), i ) )
						{
							g_pPCTalkBox->AddString( pPacket->popContent().c_str() );
							g_pPCTalkBox->m_AnswerIDMap.push_back( i );
						}
					}
					
					g_pUIDialog->PopupPCTalkDlg();
				}

				//---------------------------------------------------
				// Dialog를 띄운다.
				//---------------------------------------------------
				//POINT point = ConvertPositionMapToScreen(pCreature->GetX(), pCreature->GetY());

				//point.y -= pCreature->GetHeight();

				//g_pUIDialog->PopupPCTalkDlg(point.x, point.y - size*20-100);				
			}
		}
		else
		{
			DEBUG_ADD("[Error] WaitVerifyMode is Not WAIT_VERIFY_NPC_ASK");
		}
	}

#endif

	__END_CATCH
}
