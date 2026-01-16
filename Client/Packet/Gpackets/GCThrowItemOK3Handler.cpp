//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowItemOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCThrowItemOK3.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowItemOK3Handler::execute ( GCThrowItemOK3 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	int skillType = MAGIC_THROW_HOLY_WATER;

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
			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 누군가(target)가 누군가가 사용한 SKill을 맞은 경우..
			// [ TEST CODE ]
			MActionResult* pResult = new MActionResult;

			//DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			pResult->Add( new MActionResultNodeActionInfo( 
										skillType,//pPacket->getSkillType(), 
										pPacket->getObjectID(), 
										pPacket->getTargetObjectID(), 
										pTargetCreature->GetX(),
										pTargetCreature->GetY()
									)
						);
										//delayFrame ) );

			// 서로 바라보기
			pUserCreature->SetDirectionToPosition(pTargetCreature->GetX(), pTargetCreature->GetY());
			//pTargetCreature->SetDirectionToPosition(pUserCreature->GetX(), pUserCreature->GetY());

			//Duration_t	m_Duration;
			pUserCreature->PacketSpecialActionToOther(
								skillType,
								pPacket->getTargetObjectID(),
								pResult			// 결과
			);		
		}
	}		

#endif

	__END_CATCH
}
