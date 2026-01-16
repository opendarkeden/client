//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowBombOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCThrowBombOK2.h"
#include "ClientDef.h"
#include "MEffectStatusDef.h"
#include "PacketFunction2.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowBombOK2Handler::execute ( GCThrowBombOK2 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

		
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

	// 의미없다.
	int delayFrame = 16;//ConvertDurationToFrame( pPacket->getDuration() );

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
		//------------------------------------------------------
		// Player가 사용한 기술이라고 packet이 날아온 경우
		// --> Error다
		//------------------------------------------------------
		if (pPacket->getObjectID()==g_pPlayer->GetID())
		{
			DEBUG_ADD("[Error] the User of GCSkillToBombOK2 is Player!");			
		}
		else
		{
			//------------------------------------------------------
			// 사용자
			//------------------------------------------------------
			int creatureID = pPacket->getObjectID();
			MCreature* pCreature = g_pZone->GetCreature( creatureID );

			int skillID = GetBombActionInfo( pPacket->getItemType() );//pPacket->getSkillType();	

			if (skillID < 0)
			{
				DEBUG_ADD_FORMAT("[Error] no match skillType. itemType=%d", pPacket->getItemType());
				return;
			}

			int useSkillID = skillID;

			DEBUG_ADD("CurWA");

			if ((*g_pActionInfoTable)[skillID].IsAffectCurrentWeaponAction()
				&& pCreature!=NULL)
			{
				DEBUG_ADD("SetBA");

				useSkillID = pCreature->GetBasicActionInfo();

				if (useSkillID >= g_pActionInfoTable->GetMinResultActionInfo())
				{
					DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", useSkillID);
					return;
				}
			}

			//------------------------------------------------------
			// BombOK2로 인한 결과 
			//------------------------------------------------------				
			int size = pPacket->getCListNum();

			MActionResult* pResult = new MActionResult;

			//------------------------------------------------------------------
			// effect status를 적용시킨다.
			//------------------------------------------------------------------
			if (g_pPlayer->GetEFFECT_STAT()!=EFFECTSTATUS_NULL)
			{
				DEBUG_ADD("AddES");

				pResult->Add( new MActionResultNodeAddEffectStatus( 
											g_pPlayer->GetID(), 
											(EFFECTSTATUS)g_pPlayer->GetEFFECT_STAT(), 
											delayFrame ));

				g_pPlayer->SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );
			}

			DEBUG_ADD("AResultID?");

			int targetID = creatureID;
			if ((*g_pActionInfoTable)[skillID].GetActionResultID()==ACTIONRESULTNODE_ACTIONINFO)
			{
				targetID = OBJECTID_NULL;
			}

			DEBUG_ADD("AddAI");

			pResult->Add( new MActionResultNodeActionInfo( 
												skillID,
												creatureID, 
												targetID,
												pPacket->getX(),
												pPacket->getY(),
												delayFrame ) 
								);	
		
			if (size!=0)
			{
				// 결과를 당하는 각각의 creature에 대해서 결과 표현
				for (int i=0; i<size; i++)
				{
					//MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getCListElement() );
						
					// Creature에게 Damage 입힘
					//if (pTargetCreature != NULL)
					//{
						// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
						// [ TEST CODE ]
						
					//	pResult->Add( new MActionResultNodeActionInfo( 
					//								pCreature->GetID(), 
					//								pPacket->popCListElement(), 
					//								pPacket->getSkillType(),
					//								delayFrame ) 
					//				);
					//}
					int targetID = pPacket->popCListElement();

					DEBUG_ADD("getTC");

					MCreature* pTargetCreature = g_pZone->GetCreature( targetID );

					if (pTargetCreature!=NULL)
					{
						DEBUG_ADD("TCNotNULL");

						//------------------------------------------------------
						// EffectStatus가 있다면 붙인다.
						//------------------------------------------------------
						EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
						
						if (es!=EFFECTSTATUS_NULL)
						{
							pResult->Add( new MActionResultNodeAddEffectStatus( targetID, es, delayFrame ) );
						}

						//------------------------------------------------------
						//
						// skill에 결과가 있으면 적용 시킨다.
						//
						//------------------------------------------------------
						MActionResultNode* pActionResultNode = NULL;

						switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
						{
							//------------------------------------------------------
							// 다른 ActionInfo 실행
							//------------------------------------------------------
							case ACTIONRESULTNODE_ACTIONINFO :
								
								DEBUG_ADD("ANodeAI");

								pActionResultNode =  new MActionResultNodeActionInfo( 
																	useSkillID, //(*g_pActionInfoTable)[skillID].GetActionResultValue(),
																	creatureID, 
																	targetID, 
																	pTargetCreature->GetX(),
																	pTargetCreature->GetY(),
																	delayFrame);

								DEBUG_ADD("ANodeAI-ok");
							break;

							//------------------------------------------------------
							// Burrow 등등.. 
							//------------------------------------------------------
							default :
								DEBUG_ADD("default");
								pActionResultNode = CreateActionResultNode(pTargetCreature, useSkillID);
						}

						//------------------------------------------------------
						// NULL이 아니면 같이 적용
						//------------------------------------------------------
						if (pActionResultNode!=NULL)
						{
							DEBUG_ADD("add");
							pResult->Add( pActionResultNode );
							DEBUG_ADD("ok");
						}
					}
					else
					{
						DEBUG_ADD("TCNull");
					}
				}
			}


			DEBUG_ADD("pCreture?");

			//------------------------------------------------------
			// 사용한 캐릭터가 없는 경우 --> 결과 바로 표현
			//------------------------------------------------------
			if (pCreature==NULL)
			{
				DEBUG_ADD_FORMAT("[Error] The Creature is not Exist. id=%d", pPacket->getObjectID());
				
				int direction = pPacket->getDir();
				
				ExecuteActionInfoFromMainNode(
							skillID,										// 사용 기술 번호
						
							pPacket->getX(), pPacket->getY(), 0,
							direction,														// 사용 방향
							
							OBJECTID_NULL,		// 2002.3.6에 바꿈. 원래는 pPacket->getObjectID()였다.
							pPacket->getX(), pPacket->getY(), 0,
							
							delayFrame,													// 기술의 (남은) 지속 시간		
							
							pResult,
							
							false);			// 기술 첨부터 시작한다.);
			}
			//------------------------------------------------------
			// 정상
			//------------------------------------------------------
			else
			{				
				DEBUG_ADD("C-OK");

				//------------------------------------------------------
				// 방향을 바라보기
				//------------------------------------------------------
				pCreature->SetDirectionToPosition(pPacket->getX(), pPacket->getY());		

				//------------------------------------------------------
				// range를 direction에 적용시키는 경우
				//------------------------------------------------------
				pCreature->SetDirection( pPacket->getDir() );				

				//------------------------------------------------------
				// BombOK2로 인한 결과 추가
				//------------------------------------------------------
				//Duration_t	m_Duration;
				DEBUG_ADD("AtoSector");

				pCreature->PacketSpecialActionToSector(
									skillID, 
									pPacket->getX(), pPacket->getY(),
									pResult						// 결과
				);

				DEBUG_ADD("AtoSectorOK");
			}
			
		}
	}

	
	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	//g_pPlayer->SetEffectDelayFrame( resultActionInfo, delayFrame );




	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );
	
	DEBUG_ADD("BombOK2End");

#endif

	__END_CATCH
}
