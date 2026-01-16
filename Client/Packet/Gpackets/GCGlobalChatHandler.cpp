//////////////////////////////////////////////////////////////////////
//
// Filename    : GCGlobalChatHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCGlobalChat.h"
#include "ClientDef.h"
#include "MChatManager.h"
#include "UserInformation.h"
#include "MSkillManager.h"
#include "MZone.h"
#include "MZoneTable.H"
#include "UIFunction.h"
#include "MGameStringTable.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCGlobalChatHandler::execute ( GCGlobalChat * pPacket , Player * pPlayer )
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
		char str[256];
		char strName[256];

		//---------------------------------------------------------------
		// 완성형 --> 조합형
		//---------------------------------------------------------------
		//UI_WansungToJohap( pPacket->getMessage().c_str(), str );
		strcpy( str, pPacket->getMessage().c_str() );

		//UI_AddChatToHistory( str );
		char* pLB = strchr( str, ' ' );

		if (pLB!=NULL)
		{
			if (*(pLB+1)!=NULL)
			{
				// ' '를 NULL로
				*pLB = NULL;

				strcpy(strName, str);
 
//				bool bMasterWords = (strstr(strName, "GM")!=NULL);
				bool bMasterWords = strncmp( strName, (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0 ;

				//--------------------------------------------------
				// 나에게 보이는 글인가? (운영자의 말은 무조건 보인다)
				//--------------------------------------------------
				if (bMasterWords 
					|| g_pChatManager->IsAcceptID( strName ))
				{
					//--------------------------------------------------
					// 욕 제거
					// 운영자가 한 말도 아니고 나도 운영자가 아니면 filter한다.
					// --> 운영자의 말은 다 보이고 운영자는 다 본다.
					//--------------------------------------------------			

					if (!bMasterWords && 
						!g_pUserInformation->IsMaster 
						&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
				&& !g_bLight
#endif
					)
					{
						//--------------------------------------------------
						// 욕 제거
						//--------------------------------------------------
						g_pChatManager->RemoveCurse( pLB+1 );

						#ifndef _DEBUG
						
						ZONETABLE_INFO *pZoneInfo = g_pZoneTable->Get( g_pZone->GetID() );
						if( pZoneInfo->ChatMaskByRace == true && 
							!g_pSkillAvailable->IsEnableSkill(SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL) &&
							!g_pPlayer->HasEffectStatus( EFFECTSTATUS_TRANSLATION ))
						{
							//--------------------------------------------------
							// 종족이 다른 경우
							//--------------------------------------------------
							Race race = (Race)pPacket->getRace();
							if (g_pPlayer->GetRace() != race)
							{
								// INT는 150까지이므로..  
								int percent = min(75, 25+g_pPlayer->GetINT()*100/(min(2, g_pPlayer->GetRace()+1)*150));
//								if(g_pPlayer->GetRace() == RACE_OUSTERS || race == RACE_OUSTERS)
//									percent = 70;
								
								g_pChatManager->AddMask(str, percent);
							}
//							if (g_pPlayer->IsSlayer() && race != RACE_SLAYER)
//							{
//								// INT는 150까지이므로..  
//								int percent = min(75, 25+g_pPlayer->GetINT()*100/150);
//								if(race == RACE_OUSTERS)
//									percent = 70;
//								g_pChatManager->AddMask(pLB+1, percent);
//							}
//							else if (g_pPlayer->IsVampire() && race != RACE_VAMPIRE)
//							{
//								// INT는 300까지이므로..  
//								int percent = min(75, 25+g_pPlayer->GetINT()*100/300);
//								if(race == RACE_OUSTERS)
//									percent = 70;
//								g_pChatManager->AddMask(pLB+1, percent);
//							}
//							else if (g_pPlayer->IsOusters() && race != RACE_OUSTERS)
//							{
//								// INT는 300까지이므로..  
//								//int percent = min(75, 25+g_pPlayer->GetINT()*100/300);
//								int percent = 70;
//								g_pChatManager->AddMask(pLB+1, percent);
//							}
						}

						#endif
					}

					// ZONECHAT = 1
					UI_AddChatToHistory( pLB+1, strName, 1, pPacket->getColor() );

					// [도움말] 외치기 할 때
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_CHAT_SHOUTED );	
//					__END_HELP_EVENT
				}
			}
		}
	}

#endif

	__END_CATCH
}
