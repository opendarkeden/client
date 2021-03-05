//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddMonsterHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddMonster.h"
#include "ClientDef.h"
#include "SkillDef.h"
#include "PacketFunction.h"
extern void Add_GDR_Effect(int nEffect, bool bAppearBossMonster);
extern void Add_Race_SlayerMonster(GCAddMonster * pPacket);
extern void Add_Race_OustersMonster(GCAddMonster * pPacket);
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddMonsterHandler::execute ( GCAddMonster * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	//	cout << pPacket->toString() << endl;

	//AfxMessageBox( pPacket->toString().c_str() );
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
		#ifdef __DEBUG_OUTPUT__
			DEBUG_ADD_FORMAT("[AddMonster] type=%d, id=%d, xy=(%d,%d), dir=%d", 
								pPacket->getMonsterType(),
								pPacket->getObjectID(), 
								pPacket->getX(), pPacket->getY(), 
								pPacket->getDir());
		#endif

		//AfxMessageBox( pPacket->toString().c_str() );
		WORD wMonsterType = pPacket->getMonsterType();
		// 각종족모습의 몬스터(뱀파이어 모습 몬스터는 따로 처리 안해도 되네..ㅎㅎ)

		if(wMonsterType == 735 || wMonsterType == 793 || wMonsterType == 796 || wMonsterType == 797)
		{
			Add_Race_SlayerMonster(pPacket);
			
			return;
		}
		else if(wMonsterType == 737 || wMonsterType == 795 || wMonsterType == 800)
		{
			Add_Race_OustersMonster(pPacket);
			return;
		}
		// 2004, 9, 1, sobeit add start - 공성전시 트랩일 경우 공격측에서는 보이지 않는다.
		else if( wMonsterType == 738 ||
			wMonsterType == 739 ||
			wMonsterType == 740 ||
			wMonsterType == 741 )
		{
			EffectInfo* pTempEffectInfo = pPacket->getEffectInfo();
			
			if(NULL != pTempEffectInfo)
			{
				if(pTempEffectInfo->getListNum()>0)
				{
					std::list<WORD> TemList = pTempEffectInfo->GetList();
					
					std::list<WORD>::iterator itr = TemList.begin();
					bool bCheck = true;
					while(itr != TemList.end())
					{
						if(bCheck)
						{
							EFFECTSTATUS	status		= (EFFECTSTATUS)*itr;
							
							if(status == EFFECTSTATUS_HIDE_TO_ATTACKER)
							{
								if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1) || 
									g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_2) || 
									g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_3) || 
									g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_4) || 
									g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_5) )
									return;
							}
						}
						bCheck = !bCheck;

						itr ++ ;
					}
				}			

			}
		}
		// 2004, 9, 1, sobeit add end - 공성전시 트랩일 경우 공격측에서는 보이지 않는다.
		MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());
		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreature;

			pCreature->SetZone( g_pZone );

			//pCreature->SetCreatureType( 0 );
			pCreature->SetCreatureType( pPacket->getMonsterType() );
			pCreature->SetGroundCreature();
			pCreature->SetID(pPacket->getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );
			pCreature->SetAction( ACTION_STAND );

			pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );

			// [ TEST CODE ]
			// 이름 설정
			//char str[20];
			//sprintf(str, "ID=%d", pCreature->GetID());
			//pCreature->SetName( (*g_pCreatureTable)[pPacket->getMonsterType()].Name.GetString() );
			pCreature->SetName( pPacket->getMonsterName().c_str() );
	
			// 임시로..
			pCreature->SetGuildNumber( 1 );

			// 이름
			//pCreature->SetName( pPacket->getName().toString().c_str() );

			// 색깔 정보			

			if (!g_pZone->AddCreature( pCreature ))
			{
				delete pCreature;
				pCreature = NULL;
			}
			else
			{			
				//------------------------------------------------------------
				// Load되지 않았으면 load한다.
				//------------------------------------------------------------
//				LoadCreatureType( pPacket->getMonsterType() );			
			}
		}
		else
		{
			if (pCreature->GetCreatureType()!=pPacket->getMonsterType())
			{
				pCreature->SetCreatureType(pPacket->getMonsterType());
			}

			pCreature->SetGroundCreature();
			
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );
			pCreature->SetAction( ACTION_STAND );

			pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );

			// 임시로..
			pCreature->SetGuildNumber( 1 );
		}

		if (pCreature!=NULL)
		{
			//--------------------------------------------------
			// Effect 붙이기..
			//--------------------------------------------------
			bool bShowComeFromPotal = true;
			SetEffectInfo( pCreature, pPacket->getEffectInfo() );
			// 2004, 6, 25, sobeit add start - 질드레 관련 맵 효과 
			switch(pCreature->GetCreatureType())
			{
				case 724: // 뱀파이어 소환체 1
					Add_GDR_Effect(1, true);
					break;
				case 725: // // 뱀파이어 소환체 2
					Add_GDR_Effect(2, true);
					break;
				case 739: // 트랩 - 랜드 마인
					ExecuteActionInfoFromMainNode(SKILL_CLIENT_LAND_MINE_LIGHT,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	pCreature->GetID(),	
								pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
				case 738: // 트랩 - 플레져 스테이션
					ExecuteActionInfoFromMainNode(SKILL_CLIENT_PLEASUER_STATION_LIGHT,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	pCreature->GetID(),	
								pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
				case 753:
					bShowComeFromPotal = false;
					pCreature->SetAction(ACTION_VAMPIRE_DRAIN);  
					break;
				case 765: // 그레이트 러피언 몸에 붙는 이펙트
					ExecuteActionInfoFromMainNode(SKILL_CLIENT_GREAT_RUFFIAN_BODY,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	pCreature->GetID(),	
								pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
				case 768: // miga
					bShowComeFromPotal = false;
					pCreature->AddEffectStatus((EFFECTSTATUS)(EFFECTSTATUS_SUMMON_MIGA_FIRE), 0xFFFF);
					break;
				case 786: // 장교
					pCreature->AddEffectStatus((EFFECTSTATUS)(EFFECTSTATUS_GRAND_MASTER_VAMPIRE), 0xFFFF);
					break;
				case 788: // 
					bShowComeFromPotal = false;
					pCreature->AddEffectStatus((EFFECTSTATUS)(EFFECTSTATUS_DRAGON_TORNADO), 0xFFFF);
					break;
				case 789:
					bShowComeFromPotal = false;
					pCreature->AddEffectStatus((EFFECTSTATUS)(EFFECTSTATUS_DRAGON_TORNADO_CHILD), 0xFFFF);
					break;
				case 793:
				case 794:
				case 795: // 제물들
					bShowComeFromPotal = false;
					pCreature->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 0xffff);
					ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,pCreature->GetX(), pCreature->GetY(), 0,2,	pCreature->GetID(),	
							pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
			
			}
			// 2004, 6, 25, sobeit add end - 질드레 관련 
			//--------------------------------------------------
			// 정상일 경우에는 0, 포탈을 통했을 경우에는 1
			//--------------------------------------------------
			if (pPacket->getFromFlag()==1 && bShowComeFromPotal)
			{
//				if(pCreature->GetCreatureType() == 753)
//				{
//					pCreature->SetAction(ACTION_VAMPIRE_DRAIN);  
//				}
//				else
//				{
					ComeFromPortal( pCreature );
					
					// 바닥 이펙트 붙이기
					int x			= pCreature->GetX();
					int y			= pCreature->GetY();
					int delayFrame	= ConvertDurationToFrame( 18 );
					
					ExecuteActionInfoFromMainNode(
						RESULT_MAGIC_BLOODY_TUNNEL,		// 값 자체가 RESULT_ACTIONINFO이다.
						
						x, y, 0,
						DIRECTION_DOWN,
						
						OBJECTID_NULL,
						x, y, 0,
						
						delayFrame,	
						
						NULL
						);
//				}
			}
			
			
		}
	}
	
	// [도움말] 몹이 나타날때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_CREATURE_APPEAR_MONSTER );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
