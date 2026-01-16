//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddMonsterFromTransformationHandler.cpp
// Written By  : elca@ewestsoft.com
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddMonsterFromTransformation.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddMonsterFromTransformationHandler::execute ( GCAddMonsterFromTransformation * pPacket , Player * pPlayer )
	 
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
		#ifdef __DEBUG_OUTPUT__
			DEBUG_ADD_FORMAT("[AddMonster] type=%d, id=%d, xy=(%d,%d), dir=%d", 
								pPacket->getMonsterType(),
								pPacket->getObjectID(), 
								pPacket->getX(), pPacket->getY(), 
								pPacket->getDir());
		#endif

		//AfxMessageBox( pPacket->toString().c_str() );
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
			// creature가 아니면 지운다.
			bool reAdd = false;

			if (pCreature->GetClassType()!=MCreature::CLASS_CREATURE)
			{
				g_pZone->RemoveCreature( pCreature->GetID() );

				pCreature = new MCreature;

				pCreature->SetID(pPacket->getObjectID());

				pCreature->SetZone( g_pZone );
				
				//pCreature->SetName( (*g_pCreatureTable)[pPacket->getMonsterType()].Name.GetString() );
				pCreature->SetName( pPacket->getMonsterName().c_str() );

				pCreature->SetPosition( pPacket->getX(), pPacket->getY() );				

				reAdd = true;
			}
			else
			{
				pCreature->MovePosition( pPacket->getX(), pPacket->getY() );				
			}

			pCreature->SetCreatureType( pPacket->getMonsterType() );
			pCreature->SetGroundCreature();
			
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );
			pCreature->SetAction( ACTION_STAND );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );

			// 임시로..
			pCreature->SetGuildNumber( 1 );	
			
			if (reAdd)
			{
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
//					LoadCreatureType( pPacket->getMonsterType() );			
				}
			}
		}

		if (pCreature!=NULL)
		{
			//--------------------------------------------------
			// Effect 붙이기..
			//--------------------------------------------------
			SetEffectInfo( pCreature, pPacket->getEffectInfo() );

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
