//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddBurrowingCreatureHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddBurrowingCreature.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddBurrowingCreatureHandler::execute ( GCAddBurrowingCreature * pPacket , Player * pPlayer )
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
		MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreature;

			pCreature->SetZone( g_pZone );

			// 어차피 안 움직일거니까 관계없다.
			pCreature->SetCreatureType( CREATURETYPE_VAMPIRE_MALE1 );	


			pCreature->SetName( pPacket->getName().c_str() );

			
			pCreature->SetUndergroundCreature();
			pCreature->SetID(pPacket->getObjectID());
			
			pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( DIRECTION_DOWN );
			pCreature->SetCurrentDirection( DIRECTION_DOWN );
			pCreature->SetAction( ACTION_STAND );

			pCreature->SetGuildNumber( -1 );

			pCreature->SetStatus( MODIFY_MAX_HP, 100 );
			pCreature->SetStatus( MODIFY_CURRENT_HP, 100 );

			//si.getName()
			// 색상 정보

			

			if (!g_pZone->AddCreature( pCreature ))
			{
				delete pCreature;
			}
		}
		else
		{
			// 이미 있다면.. Underground Creature로 만든다.
			pCreature->SetUndergroundCreature();

			// 이름 다시 설정 - -;
			pCreature->SetName( pPacket->getName().c_str() );

			// 좌표 설정
			pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );

			pCreature->SetGuildNumber( -1 );
		}
	}

#endif

	__END_CATCH
}
