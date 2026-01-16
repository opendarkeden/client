//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSayHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSay.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "MChatManager.h"
#include "MSkillManager.h"
#include "MZoneTable.h"	
#include "MZone.h"	
#include "UIFunction.h"
#include "MGameStringTable.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCSayHandler::execute ( GCSay * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
//#ifdef __GAME_CLIENT__

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
		//DEBUG_ADD("Zone-GetCreature");

		MCreature*	pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		if (pCreature==NULL)
		{
			// 그런 캐릭터가 없으면 무시한다.
			DEBUG_ADD("No such Character");
		}
		else
		{			
			bool IsGildre	= pCreature->GetCreatureType() == 717 || 
							  pCreature->GetCreatureType() == 723 ; // 질드레 일때
			bool bMasterWords = pCreature->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
								|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR
								|| pCreature->GetCreatureType()==CREATURETYPE_OUSTERS_OPERATOR
								|| strncmp( pCreature->GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0
								|| IsGildre ;
//								|| (strstr(pCreature->GetName(), "GM")!=NULL);

			//DEBUG_ADD_FORMAT("isMasterWords=%d", bMasterWords);

			//-------------------------------------------------
			// 정상적인 대화
			//-------------------------------------------------
			if (bMasterWords 
				|| g_pChatManager->IsAcceptID( pCreature->GetName() ))
			{
				//DEBUG_ADD("can see");

				// 채팅~~
				char str[256];

				bool isPlayerVampire = pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE1
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE1
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE2
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE2
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE3
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE3
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR
										|| pCreature->GetCreatureType()==CREATURETYPE_BAT
										|| pCreature->GetCreatureType()==CREATURETYPE_WOLF
										|| pCreature->GetCreatureType()==CREATURETYPE_WER_WOLF
										// add by coffee 2006.11.24  饑쮸近蹶
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE4
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE4
										//add by viva
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE5
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE5
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE6
										|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE6;
										// add end

				bool isPlayerCharacter = (isPlayerVampire 
											|| pCreature->IsSlayer()
											|| pCreature->IsOusters()
											|| pCreature->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR);

				//DEBUG_ADD("strcpy");
				
				//---------------------------------------------------------------
				// 완성형 --> 조합형
				//---------------------------------------------------------------
				//UI_WansungToJohap( pPacket->getMessage().c_str(), str );
				strcpy( str, pPacket->getMessage().c_str() );
				bool	bTranslation = g_pSkillAvailable->IsEnableSkill(SKILL_HOLYLAND_BLOOD_BIBLE_HILLEL) || 
					!g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace ||
					g_pPlayer->HasEffectStatus( EFFECTSTATUS_TRANSLATION ) ||
					g_pPlayer->GetCreatureType() == CREATURETYPE_SLAYER_OPERATOR ||
					g_pPlayer->GetCreatureType() == CREATURETYPE_VAMPIRE_OPERATOR ||
					g_pPlayer->GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR;

				//--------------------------------------------------
				// 욕 제거
				// 운영자가 한 말도 아니고 나도 운영자가 아니면 filter한다.
				// --> 운영자의 말은 다 보이고 운영자는 다 본다.
				//--------------------------------------------------
				if (!bMasterWords && !g_pUserInformation->IsMaster 
					&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
				&& !g_bLight
#endif
					)
				{
				
					// player인 경우만 나쁜말 제거
					if (isPlayerCharacter)
					{
						DEBUG_ADD("remove curse");
						g_pChatManager->RemoveCurse( str );

						DEBUG_ADD("has effect");

						if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
						{
							//DEBUG_ADD("add mask hallu");

							g_pChatManager->AddMask(str, 50);
						}

						//DEBUG_ADD("ok");
					}					 

					#ifndef _DEBUG
						//--------------------------------------------------
						// 종족이 다른 경우
						//--------------------------------------------------
						if (g_pPlayer->GetRace() != pCreature->GetRace()
							// player인 경우만.. 못 듣게..
							//&& isPlayerVampire 
							&& isPlayerCharacter
							&& !bTranslation 
							&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
							&& !g_bLight
#endif
							)
						{
							//DEBUG_ADD("add mask1");

							// INT는 150까지이므로..  
							int percent;
//							if( g_pPlayer->IsOusters() )
//								percent = 70;
//							else
								percent = min(75, 25+g_pPlayer->GetINT()*100/(min(2, g_pPlayer->GetRace()+1)*150));
							
							g_pChatManager->AddMask(str, percent);

							//DEBUG_ADD("ok");
						}
//						else if (g_pPlayer->IsVampire() && pCreature->IsSlayer() && !bTranslation)
//						{
//							//DEBUG_ADD("add mask2");
//
//							// INT는 300까지이므로..  
//							int percent = min(75, 25+g_pPlayer->GetINT()*100/300);
//							g_pChatManager->AddMask(str, percent);
//
//							//DEBUG_ADD("ok");
//						}
						// 할루 상태는 채팅도 제대로 안 보인다.
						else if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
						{
							//DEBUG_ADD("add mask3");

							g_pChatManager->AddMask(str, 50);

							//DEBUG_ADD("ok");
						}
					#endif
				}
				
				//DEBUG_ADD("setChat");

				pCreature->SetChatString( str, pPacket->getColor() );

				//DEBUG_ADD("ok");

				// history에 추가
				char strName[256];
				//sprintf(temp, "%s> %s", pCreature->GetName(), str);
				

				// 종족이 다르면 hallu name을 읽어온다
				if(!bMasterWords && !g_pUserInformation->IsMaster && g_pPlayer->GetRace() != pCreature->GetRace() && !bTranslation)
					strcpy(strName, pCreature->GetHalluName());
				else
					strcpy(strName, pCreature->GetName());

				// player인 경우만 채팅창에 글 넣는다.
				if (isPlayerCharacter /*|| IsGildre*/) // 질드레 일경우 존 채팅으로 날린다고 해서 막음..
				{
					//DEBUG_ADD("UI add");
					UI_AddChatToHistory( str, strName, 0, pPacket->getColor() );
					//DEBUG_ADD("UI add");
				}
			}
			//-------------------------------------------------
			// 대화 거부로 설정되어 있다면 출력안한다.
			//-------------------------------------------------
			else
			{
				//DEBUG_ADD("no see");
				pCreature->SetChatString( "......", pPacket->getColor() );
			}
		}
	}
//#endif

	__END_CATCH
}
