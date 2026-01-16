//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowItemOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCThrowItemOK2.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowItemOK2Handler::execute ( GCThrowItemOK2 * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	int skillType = MAGIC_THROW_HOLY_WATER;

	//int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

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
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );
			
		// Creature에게 Damage 입힘
		if (pCreature == NULL)
		{
			
		}
		else
		{
			// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
			// [ TEST CODE ]
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( 
									skillType, 
									pCreature->GetID(), 
									g_pPlayer->GetID(), 
									g_pPlayer->GetX(),
									g_pPlayer->GetY()
									)
						);
									//delayFrame ) );		

			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 방향을 바라보기
			pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToOther(
								skillType, 
								g_pPlayer->GetID(),		// player가 맞는다.
								pResult						// 결과
			);
		}
	}	
	
	/*
	//------------------------------------------------------
	// Player가 기술을 당했을 때의 모습..
	//------------------------------------------------------
	int resultActionInfo =  pPacket->getSkillType() + g_ActionInfoTable.GetMinResultActionInfo();
	g_pPlayer->PacketSpecialActionResult( 
						resultActionInfo,
						g_pPlayer->GetID(),
						g_pPlayer->GetX(),
						g_pPlayer->GetY()
	);

	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	g_pPlayer->SetEffectDelayFrame( resultActionInfo, delayFrame );
	*/

	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}
