//////////////////////////////////////////////////////////////////////
//
// Filename    : GCKnocksTargetBackOK4Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCKnocksTargetBackOK4.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCKnocksTargetBackOK4Handler::execute ( GCKnocksTargetBackOK4 * pPacket , Player * pPlayer )
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
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );

		// Creature가 knockback당함
		if (pCreature != NULL)
		{			
			unsigned short x = pPacket->getX();
			unsigned short y = pPacket->getY();

			// 물러나는방향(direction)적용..
			// 이동한 후의 좌표가 온다.
			//MCreature::GetPositionToDirection(x, y, pPacket->getDir());

			// Creature의 좌표를 바로 바꾼다.
			pCreature->MovePosition( x, y );
			pCreature->SetServerPosition( x, y );
			pCreature->SetStop();

			// 결과 바로 표현
			pCreature->PacketSpecialActionResult( 
								pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo(),
								pCreature->GetID(),
								pCreature->GetX(),
								pCreature->GetY()
			);			
		}
	}

#endif

	__END_CATCH
}
