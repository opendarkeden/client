//--------------------------------------------------------------------------------
//
// Filename    : GCMoveHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCMove.h"
#include "Player.h"
#include "ClientDef.h"

#ifdef __GAME_CLIENT__
	#include "Cpackets/CGMove.h"
	#include "ClientPlayer.h"
#endif


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCMoveHandler::execute ( GCMove * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

		// ID가 getObjectID()인 Creature를 움직여준다.
		// 없으면 Zone에 새로 추가
		// 있으면 움직임..

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
				// message
				DEBUG_ADD("[Error] Not Exist");			 
				
					/*
				pCreature = new MCreature;
				pCreature->SetZone( g_pZone );

				pCreature->SetCreatureType( 0 );
				pCreature->SetGroundCreature();
				pCreature->SetID(pPacket->getObjectID());
				//pCreature->SetAction(ACTION_MOVE);
				pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
				pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
				pCreature->SetDirection(pPacket->getDir());
				pCreature->SetAction( ACTION_STAND );

				g_pZone->AddCreature( pCreature );			
				*/
//				_MinTrace("afsaf");
//				_MinTrace("[GCMove] Not Exist Creature. (MyX:%d,MyY:%d,CreatureID:%d,MoveX:%d,MoveY:%d,Direction:%d)\n",
//					g_pPlayer->GetX(), g_pPlayer->GetY(), pPacket->getObjectID(), pPacket->getX(), pPacket->getY(),
//					pPacket->getDir() );
			}
			//--------------------------------------------------
			// 이미 존재하는 Creature이면 이동
			//--------------------------------------------------
			else
			{
				pCreature->PacketMove(pPacket->getX(), pPacket->getY(), pPacket->getDir());		
			}
		}
	

//	#endif

#endif

	__END_CATCH
}
