//--------------------------------------------------------------------------------
//
// Filename    : GCMorph1Handler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCMorphSlayer2.h"
#include "ClientDef.h"
#include "MCreatureTable.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCMorphSlayer2Handler::execute ( GCMorphSlayer2 * pPacket , Player * pPlayer )
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
		const PCSlayerInfo3 & si = pPacket->getSlayerInfo();

		MCreature* pCreature = g_pZone->GetCreature(si.getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		// 복장이 있는 Creature이다.
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			MCreatureWear* pCreatureWear = new MCreatureWear;

			pCreatureWear->SetZone( g_pZone );
	
	

			pCreatureWear->SetCreatureType( (si.getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );

			pCreatureWear->SetGroundCreature();
			pCreatureWear->SetID(si.getObjectID());
			//pCreatureWear->SetAction(ACTION_MOVE);
			pCreatureWear->SetPosition( si.getX(), si.getY() );
			pCreatureWear->SetServerPosition( si.getX(), si.getY() );
			pCreatureWear->SetDirection( si.getDir() );
			pCreatureWear->SetCurrentDirection( si.getDir() );
			pCreatureWear->SetAction( ACTION_STAND );

			// 피부색
			pCreatureWear->SetBodyColor1( si.getSkinColor() );
			
			pCreatureWear->SetStatus( MODIFY_MAX_HP, si.getMaxHP() );
			pCreatureWear->SetStatus( MODIFY_CURRENT_HP, si.getCurrentHP() );

			// 이름
			pCreatureWear->SetName( si.getName().c_str() );

			// 색깔 정보

			// 복장 정보 --> 일단 기본 옷을 입고 있게 한다.
			//----------------------------------------	
			// 복장을 착용한다.
			//----------------------------------------	
			SetAddonToSlayer( pCreatureWear, &si );

			// 임시로
			pCreatureWear->SetGuildNumber( -1 );

			
			if (!g_pZone->AddCreature( pCreatureWear ))
			{
				delete pCreatureWear;
				pCreatureWear = NULL;
				pCreature = NULL;
			}
		}
		//--------------------------------------------------
		// 기존에 있던 Creature이면 정보 변경
		//--------------------------------------------------
		else
		{
			if (pCreature->GetClassType()==MCreature::CLASS_CREATUREWEAR)
			{
				MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;

				pCreatureWear->SetCreatureType( (si.getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );

				pCreatureWear->SetGroundCreature();
				//pCreatureWear->SetID(si.getObjectID());

				// 임시로
				pCreatureWear->SetGuildNumber( -1 );
				
				pCreatureWear->SetServerPosition( si.getX(), si.getY() );				
				pCreatureWear->MovePosition( si.getX(), si.getY() );
				pCreatureWear->SetDirection( si.getDir() );
				//pCreatureWear->SetCurrentDirection( si.getDir() );
				pCreatureWear->SetAction( ACTION_STAND );

				// 피부색
				pCreatureWear->SetBodyColor1( si.getSkinColor() );
			
				pCreatureWear->SetStatus( MODIFY_MAX_HP, si.getMaxHP() );
				pCreatureWear->SetStatus( MODIFY_CURRENT_HP, si.getCurrentHP() );

				// 이름
				pCreatureWear->SetName( si.getName().c_str() );

				// 색깔 정보

				//----------------------------------------	
				// 복장을 착용한다.
				//----------------------------------------	
				SetAddonToSlayer( pCreatureWear, &si );
			}
			else
			{
				DEBUG_ADD_FORMAT("[Error] the Creature is Not CreatureWear. id=%d", pCreature->GetID());
			}
		}

	}

#endif

	__END_CATCH
}
