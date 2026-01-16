//--------------------------------------------------------------------------------
//
// Filename    : GCMorphVampire2Handler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCMorphVampire2.h"
#include "ClientDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCMorphVampire2Handler::execute ( GCMorphVampire2 * pPacket , Player * pPlayer )
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
		const PCVampireInfo3& vi = pPacket->getSlayerInfo();

		MCreature* pCreature = g_pZone->GetCreature(vi.getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreatureWear;

			pCreature->SetZone( g_pZone );			

			pCreature->SetName( vi.getName().c_str() );

			//pCreature->SetCreatureType( 0 );
			Shape_t shape	= vi.getShape();
			bool bMale		= vi.getSex()==MALE;
			ItemType_t coatType = vi.getCoatType();
			pCreature->SetCreatureType( GetVampireCreatureType( shape, bMale, coatType ) );

			pCreature->SetMale( bMale );

			pCreature->SetGroundCreature();
			pCreature->SetID(vi.getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetPosition( vi.getX(), vi.getY() );
			pCreature->SetServerPosition( vi.getX(), vi.getY() );
			pCreature->SetDirection( vi.getDir() );
			pCreature->SetCurrentDirection( vi.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 색깔
			pCreature->SetBodyColor1( vi.getSkinColor() );
			pCreature->SetBodyColor2( vi.getCoatColor() );

			// speed
			pCreature->SetWeaponSpeed( vi.getAttackSpeed() );
			//--------------------------------------------------
			// [ TEST CODE ]
			//--------------------------------------------------
			// 옷 색깔 설정하기
			//--------------------------------------------------
			/*
			if (pCreature->IsMale())
			{
				pCreature->SetBodyColor2( 91 );
			}
			else
			{
				pCreature->SetBodyColor2( 38 );
			}
			*/


			pCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, vi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );

			//si.getName()
			// 색상 정보
			// 임시로
			pCreature->SetGuildNumber( 2 );

			if (!g_pZone->AddCreature( pCreature ))
			{
				delete pCreature;
				pCreature = NULL;
			}
		}
		//--------------------------------------------------
		// 이미 있던 Creature이면 정보 변경
		//--------------------------------------------------
		else
		{
			pCreature->SetName( vi.getName().c_str() );

			// 임시로
			pCreature->SetGuildNumber( 2 );

			//pCreature->SetCreatureType( 0 );
			Shape_t shape	= vi.getShape();
			bool bMale		= vi.getSex()==MALE;
			ItemType_t coatType = vi.getCoatType();
			pCreature->SetCreatureType( GetVampireCreatureType( shape, bMale, coatType ) );

			pCreature->SetMale( bMale );

			pCreature->SetGroundCreature();
			//pCreature->SetID(vi.getObjectID());
			
			pCreature->MovePosition( vi.getX(), vi.getY() );
			pCreature->SetServerPosition( vi.getX(), vi.getY() );
			pCreature->SetDirection( vi.getDir() );
			//pCreature->SetCurrentDirection( vi.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 색깔
			pCreature->SetBodyColor1( vi.getSkinColor() );
			pCreature->SetBodyColor2( vi.getCoatColor() );

			// speed
			pCreature->SetWeaponSpeed( vi.getAttackSpeed() );

			//--------------------------------------------------
			// [ TEST CODE ]
			//--------------------------------------------------
			// 옷 색깔 설정하기
			//--------------------------------------------------
			/*
			if (pCreature->IsMale())
			{
				pCreature->SetBodyColor2( 91 );
			}
			else
			{
				pCreature->SetBodyColor2( 38 );
			}
			*/

			pCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, vi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );			
			
		}
	}

#endif

	__END_CATCH
}
