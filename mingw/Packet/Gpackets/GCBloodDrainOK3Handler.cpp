//////////////////////////////////////////////////////////////////////
//
// Filename    : GCBloodDrainOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCBloodDrainOK3.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCBloodDrainOK3Handler::execute ( GCBloodDrainOK3 * pPacket , Player * pPlayer )
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
		MCreature* pUserCreature = g_pZone->GetCreature( pPacket->getObjectID() );
		MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );		

		// Creature에게 Damage 입힘
		if (pUserCreature != NULL && pTargetCreature != NULL)
		{
			// TargetCreature에게 UserCreature가 흡혈하구 있다구 알려준다.
			pTargetCreature->SetDrainCreatureID( pPacket->getObjectID() );

			pUserCreature->ClearStopBloodDrain();

			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 누군가(target)가 누군가가 사용한 SKill을 맞은 경우..
			// [ TEST CODE ]
			MActionResult* pResult = new MActionResult;

			//DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			pResult->Add( new MActionResultNodeActionInfo( 
											SKILL_BLOOD_DRAIN,
											pPacket->getObjectID(), 
											pPacket->getTargetObjectID(), 
											pTargetCreature->GetX(),
											pTargetCreature->GetY()
											 ) );

			// 서로 바라보기
			pUserCreature->SetDirectionToPosition(pTargetCreature->GetX(), pTargetCreature->GetY());
			//pTargetCreature->SetDirectionToPosition(pUserCreature->GetX(), pUserCreature->GetY());

			//Duration_t	m_Duration;
			pUserCreature->PacketSpecialActionToOther(
								SKILL_BLOOD_DRAIN,
								pPacket->getTargetObjectID(),
								pResult			// 결과
			);		

			int delayFrame = 691200;	// 12시간*60분*60초*16frame = 12*60*60*16;
			pTargetCreature->AddEffectStatus( EFFECTSTATUS_BLOOD_DRAIN, delayFrame );
		}
	}	

#endif

	__END_CATCH
}
