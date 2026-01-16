//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddMonsterFromBurrowingHandler.cpp
// Written By  : elca@ewestsoft.com
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddMonsterFromBurrowing.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddMonsterFromBurrowingHandler::execute ( GCAddMonsterFromBurrowing * pPacket , Player * pPlayer )
	 throw ( Error )
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
		//AfxMessageBox( pPacket->toString().c_str() );
		MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());

		// 2004, 9, 1, sobeit add start - 공성전시 트랩일 경우 공격측에서는 보이지 않는다.
		WORD wMonsterType = pPacket->getMonsterType();
		if( wMonsterType == 738 ||
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
		//--------------------------------------------------
		// 이미 있는 Creature인 경우
		//--------------------------------------------------
		else
		{
			//pCreature->SetCreatureType( 0 );
			pCreature->SetCreatureType( pPacket->getMonsterType() );
			pCreature->SetGroundCreature();
			pCreature->SetID(pPacket->getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
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

			//------------------------------------------------------------
			// Load되지 않았으면 load한다.
			//------------------------------------------------------------
//			LoadCreatureType( pPacket->getMonsterType() );			
		}

		//--------------------------------------------------
		// Effect 붙이기..
		//--------------------------------------------------
		if (pCreature!=NULL)
		{
			SetEffectInfo( pCreature, pPacket->getEffectInfo() );		
			// 2004, 8, 31, sobeit add start - 트랩에 이펙트 붙임
			switch(pCreature->GetCreatureType())
			{
				case 739: // 트랩 - 랜드 마인
					ExecuteActionInfoFromMainNode(SKILL_CLIENT_LAND_MINE_LIGHT,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	pCreature->GetID(),	
								pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
				case 738: // 트랩 - 플레져 스테이션
					ExecuteActionInfoFromMainNode(SKILL_CLIENT_PLEASUER_STATION_LIGHT,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	pCreature->GetID(),	
								pCreature->GetX(), pCreature->GetY(), 0, 0xffff, NULL, false);			
					break;
			}
			// 2004, 8, 31, sobeit add end -
			//--------------------------------------------------
			// Burrow에서 빠져나오는 Effect == Burrowing하는 Effect
			//--------------------------------------------------		
			ExecuteActionInfoFromMainNode(
				RESULT_MAGIC_HIDE,										// 사용 기술 번호
			
				pCreature->GetX(), pCreature->GetY(), 0,
				pCreature->GetDirection(),														// 사용 방향
				
				OBJECTID_NULL,												// 목표에 대한 정보
				pCreature->GetX(), pCreature->GetY(), 0, 
				
				0,													// 기술의 (남은) 지속 시간		
				
				NULL,
				
				false);			// 기술 첨부터 시작한다.
		}
	}

	// [도움말] 몹이 나타날때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_CREATURE_APPEAR_MONSTER );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
