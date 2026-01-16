//////////////////////////////////////////////////////////////////////
//
// Filename    : GCPetUseSkillHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCPetUseSkill.h"
#include "MZone.h"
#include "MFakeCreature.h"
#include "DebugInfo.h"
#include "SkillDef.h"
#include "PacketFunction.h"
#include "ClientDef.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCPetUseSkillHandler::execute ( GCPetUseSkill * pGCPetUseSkill , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
	TYPE_OBJECTID attackerID = pGCPetUseSkill->getAttacker();
	TYPE_OBJECTID targetID = pGCPetUseSkill->getTarget();

	MCreature *pAttackerCreature = g_pZone->GetCreature(attackerID);
	MCreature *pTargetCreature = g_pZone->GetCreature(targetID);

	if( pAttackerCreature == NULL )
	{
		DEBUG_ADD_FORMAT("[GCPetUseSkillHandler] 어태커(%d) 없음-_-;", attackerID);
	}
	else
	{
		if( pTargetCreature == NULL )
		{
			DEBUG_ADD_FORMAT("[GCPetUseSkillHandler] 타겟(%d) 없음-_-;", targetID);
		}
		else
		{
			pTargetCreature->SetAction( ACTION_DAMAGED );
		
			MFakeCreature *pPet = (MFakeCreature *)g_pZone->GetFakeCreature(pAttackerCreature->GetPetID());
			
			if( pPet == NULL )
			{
				DEBUG_ADD_FORMAT("[GCPetUseSkillHandler] 펫(%d) 없음-_-;", pAttackerCreature->GetPetID());
			}
			else
			{
//				pPet->SetAction( ACTION_ATTACK );
//				pPet->SyncTurretDirection();

				int skillID = SKILL_CLIENT_TANK_ATTACK_3;
				if(pPet->GetPetItem() != NULL)
				{
					int petItemType = pPet->GetPetItem()->GetItemType();
					int petLevel = pPet->GetPetItem()->GetNumber();

					TYPE_SOUNDID soundID = (*g_pCreatureTable)[pPet->GetCreatureType()].GetActionSound( ACTION_ATTACK );

					if (soundID!=SOUNDID_NULL  )
					{
						PlaySound( soundID,
									false,
									pPet->GetX(), pPet->GetY());
					}

					switch(petItemType)
					{
					// 탱크-_-;
					case 3:
						{
							ExecuteActionInfoFromMainNode(
								SKILL_CLIENT_TANK_ATTACKED,										// 사용 기술 번호
								
								0, 0, 0,
								DIRECTION_DOWN, // 사용 방향
								
								pTargetCreature->GetID(),												// 목표에 대한 정보
								0, 0, 0, 
								
								0,													// 기술의 (남은) 지속 시간		
								
								NULL,
								
								false);	
						}

						if(petLevel >= 30 && petLevel < 40)
							skillID = SKILL_CLIENT_TANK_ATTACK_3;
						else if(petLevel >= 40 && petLevel < 50)
							skillID = SKILL_CLIENT_TANK_ATTACK_4;
						else if(petLevel >= 50)
							skillID = SKILL_CLIENT_TANK_ATTACK_5;
						break;
					case 4:	// stirge
						if(petLevel<20)
							skillID = SKILL_CLIENT_STIRGE_BLOOD_DRAIN_1;
						else if(petLevel >= 20 && petLevel < 30)
							skillID = SKILL_CLIENT_STIRGE_BLOOD_DRAIN_2;
						else if(petLevel >= 30 && petLevel < 40)
							skillID = SKILL_CLIENT_STIRGE_BLOOD_DRAIN_3;
						else if(petLevel >= 40 && petLevel < 50)
							skillID = SKILL_CLIENT_STIRGE_BLOOD_DRAIN_4;
						else if(petLevel >= 50)
							skillID = SKILL_CLIENT_STIRGE_BLOOD_DRAIN_5;
						
						{
							ExecuteActionInfoFromMainNode(
								skillID,										// 사용 기술 번호
								
								0, 0, 0,
								DIRECTION_DOWN, // 사용 방향
								
								pTargetCreature->GetID(),												// 목표에 대한 정보
								0, 0, 0, 
								
								0,													// 기술의 (남은) 지속 시간		
								
								NULL,
								
								true);	
						}
						break;
					case 5:	// pixie
						if(petLevel<20)
							skillID = SKILL_CLIENT_PIXIE_ABSORB_SOUL_1;
						else if(petLevel >= 20 && petLevel < 30)
							skillID = SKILL_CLIENT_PIXIE_ABSORB_SOUL_2;
						else if(petLevel >= 30 && petLevel < 40)
							skillID = SKILL_CLIENT_PIXIE_ABSORB_SOUL_3;
						else if(petLevel >= 40 && petLevel < 50)
							skillID = SKILL_CLIENT_PIXIE_ABSORB_SOUL_4;
						else if(petLevel >= 50)
							skillID = SKILL_CLIENT_PIXIE_ABSORB_SOUL_5;

						ExecuteActionInfoFromMainNode(
							skillID,										// 사용 기술 번호
							
							0, 0, 0,
							DIRECTION_DOWN, // 사용 방향
							
							pTargetCreature->GetID(),												// 목표에 대한 정보
							0, 0, 0, 
							
							0,													// 기술의 (남은) 지속 시간		
							
							NULL,
							
							false);	
						break;
					}
				}

				pPet->SetTurretDelay( 3 );
//				pPet->SetDirectionToPosition(pTargetCreature->GetX(), pTargetCreature->GetY());
				int sx = 0, sy = 0;
				BYTE petDirect = pPet->GetDirectionToPosition(pTargetCreature->GetX(), pTargetCreature->GetY());

				// 2004, 12, 21, sobeit add start - 센타우로 터렛위치 세팅
				if( pPet->GetCreatureType() == 702 || pPet->GetCreatureType() == 703 || pPet->GetCreatureType() == 704 )
					pPet->SetTurretFinalDirection(petDirect);
				// 2004, 12, 21, sobeit add end

				if( pPet->GetCreatureType() == 702 )
				{
					POINT pointGap[8] = 
					{
						{ 11, 6 },	// 좌
						{ 8, 2 },	// 좌하
						{ 0, 0 },	// 하
						{ -7, 2 },	// 우하
						{ -10, 6 },	// 우
						{ -7, 10 },	// 우상
						{ 0, 12 },	// 상
						{ 8, 10 },	// 좌상
					};
					sx = pointGap[pPet->GetDirection()].x-pointGap[petDirect].x;
					sy = pointGap[pPet->GetDirection()].y-pointGap[petDirect].y;
				}
				ExecuteActionInfoFromMainNode(
					skillID,										// 사용 기술 번호
					
					pPet->GetX(), pPet->GetY(), 0,
					petDirect, // 사용 방향
					
					pPet->GetID(),												// 목표에 대한 정보
					pPet->GetX(), pPet->GetY(), 0, 
					
					0,													// 기술의 (남은) 지속 시간		
					
					NULL,
					
					false, sx, sy);	
				
			}
		}
	}

	//cout << pGCPetUseSkill->toString() << endl;
	
#elif __WINDOWS__

#endif

	__END_CATCH
}
