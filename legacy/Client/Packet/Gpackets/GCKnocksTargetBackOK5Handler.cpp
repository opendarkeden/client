//////////////////////////////////////////////////////////////////////
//
// Filename    : GCKnocksTargetBackOK5Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCKnocksTargetBackOK5.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCKnocksTargetBackOK5Handler::execute ( GCKnocksTargetBackOK5 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	// message

	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
		
		return;
	}	

	//------------------------------------------------------
	// 대상이 되는 creature를 얻는다.
	//------------------------------------------------------
	MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );
	MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );
	
	if (pCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), pPacket->getSkillType());				
		
		return;
	}

	if (pTargetCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : TargetID=%d, Skill=%d", pPacket->getTargetObjectID(), pPacket->getSkillType());
		
		return;
	}


	int skillType = pPacket->getSkillType();	//pCreature->GetBasicActionInfo()	

	if( skillType >= g_pActionInfoTable->GetSize() )
	{
		DEBUG_ADD_FORMAT("[GCKnocksTargetBackOK5] SkillType Error %d", skillType );
		return;
	}

	//------------------------------------------------------
	// 물러나는방향(direction)적용..
	//------------------------------------------------------	
	unsigned short x = pPacket->getX();
	unsigned short y = pPacket->getY();

	// 이동한 후의 좌표가 온다.
	//MCreature::GetPositionToDirection(x, y, pPacket->getDir());

	//------------------------------------------------------
	// 결과(다른 캐릭터가 맞는 모습)를 설정한다.
	//------------------------------------------------------
	MActionResult* pResult = new MActionResult;
	pResult->Add( new MActionResultNodeActionInfo( 
								skillType, // 총 공격									
								pPacket->getObjectID(), 
								pPacket->getTargetObjectID(),
								pTargetCreature->GetX(),
								pTargetCreature->GetY()
								 ) 
				);

	// target object의 좌표가 바뀜
	pResult->Add( new MActionResultNodeChangePosition( 
								pPacket->getTargetObjectID(),
								x, y)
				);
								
	//------------------------------------------------------
	// 행동하는 Creature가 TargetCreature를 바라보도록 한다.
	//------------------------------------------------------
	pCreature->SetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

	//------------------------------------------------------
	// Creature가 행동을 취하도록 한다.
	//------------------------------------------------------
	pCreature->PacketSpecialActionToOther(
					// 총 공격
					skillType	, 
					pPacket->getTargetObjectID(), 
					pResult
	);

	//------------------------------------------------------
	// 동기화 문제 때문에..
	// 서버 위치는 바로 지정한다.
	//------------------------------------------------------
	pTargetCreature->SetServerPosition( x, y );

#endif

	__END_CATCH
}
