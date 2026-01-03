//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddBatHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddBat.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddBatHandler::execute ( GCAddBat * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	int batCreatureType = 185;

	_MinTrace("%d\n", pPacket->getBatColor() );

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

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreatureWear;

			pCreature->SetZone( g_pZone );
			
			//pCreature->SetCreatureType( 0 );
			pCreature->SetCreatureType( batCreatureType );


			pCreature->SetName( pPacket->getName().c_str() );

			// 임시로
			pCreature->SetGuildNumber( pPacket->getGuildID() );

			pCreature->SetFlyingCreature();

			pCreature->SetID(pPacket->getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );
			pCreature->SetAction( ACTION_STAND );

			
			pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );

			//pPacket->getName()
			// 색상 정보
			
			if( pPacket->getBatColor() != 0 )
				pCreature->SetBatColor( pPacket->getBatColor() );
			else
				pCreature->SetBatColor( 0xFFFF );
			

			if (!g_pZone->AddCreature( pCreature ))
			{
				delete pCreature;
			}
		}
		//--------------------------------------------------
		// 이미 있는 Creature인 경우
		//--------------------------------------------------
		else
		{
			//pCreature->SetCreatureType( batCreatureType );
			pCreature->SetFlyingCreature();
			
			// 임시로
			pCreature->SetGuildNumber( pPacket->getGuildID() );

			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );
			//pCreature->SetAction( ACTION_STAND );			

			pCreature->SetStatus( MODIFY_MAX_HP, pPacket->getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, pPacket->getCurrentHP() );

			if( pPacket->getBatColor() != 0 )
				pCreature->SetBatColor( pPacket->getBatColor() );
			else
				pCreature->SetBatColor( 0xFFFF );

			//--------------------------------------------------
			// 박쥐로 변신하는 결과
			//--------------------------------------------------
			MActionResult* pResult = new MActionResult;

			pResult->Add( new MActionResultNodeChangeCreatureType( pCreature->GetID(), batCreatureType ) );

			//--------------------------------------------------
			// 박쥐 변신 
			//--------------------------------------------------								
			ExecuteActionInfoFromMainNode(
				RESULT_MAGIC_TRANSFORM_TO_BAT,										// 사용 기술 번호
			
				pCreature->GetX(), pCreature->GetY(), 0,
				pCreature->GetDirection(),														// 사용 방향
				
				OBJECTID_NULL,												// 목표에 대한 정보
				pCreature->GetX(), pCreature->GetY(), 0, 
				
				0,													// 기술의 (남은) 지속 시간		
				
				pResult, //NULL,
				
				false);			// 기술 첨부터 시작한다.

			//pCreature->SetDelay( 1000 );
		}	
	}

	// [도움말] Vampire가 나타날때
//	__BEGIN_HELP_EVENT
//		//ExecuteHelpEvent( HE_CREATURE_APPEAR_VAMPIRE );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
