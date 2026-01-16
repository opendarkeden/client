//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToObjectOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToObjectOK1.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "PacketFunction2.h"

// [새기술2]
#include "SkillDef.h"	
#include "MFakeCreature.h"
void	SkillShadowDancing(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID);
extern void Add_RocketRuncher(MCreature* UserCreature, MCreature* TargetCreature);
extern void Add_Wild_Wolf(MCreature* UserCreature, MCreature* TargetCreature, bool IsEatCorpse);
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToObjectOK1Handler::execute ( GCSkillToObjectOK1 * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
		
	//------------------------------------------------------------------
	// Player가 기다리던 skill의 성공유무를 검증받았다.
	//------------------------------------------------------------------
	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_SKILL_SUCCESS)
	{		
		g_pPlayer->SetWaitVerifyNULL();
	}
	else
	{
		DEBUG_ADD("[Error] Player is not WaitVerifySkillSuccess");
	}

	//------------------------------------------------------------------
	// Player가 Skill을 성공시킨 경우에 날아오는 Packet이므로
	// 결과를 반영시켜야 한다.
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

		// [ TEST CODE ]
		// 진행중인 Effect이면 Result로 추가해야 한다. 
		// Result를 생성하고..
		// g_pPlayer->>AddActionResult( ... )

		// 2005, 1, 3, sobeit add start
		if(pPacket->getSkillType() == SKILL_WILD_WOLF)
		{
			if(pCreature != NULL)
				Add_Wild_Wolf(g_pPlayer, pCreature, pPacket->getGrade()?true:false);
			else
			{
				MItem* pItem = g_pZone->GetItem(pPacket->getTargetObjectID());
				if(pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_CORPSE)
				{
					pCreature = ((MCorpse*)pItem)->GetCreature();
					if(pCreature != NULL)
					{
						Add_Wild_Wolf(g_pPlayer, pCreature, pPacket->getGrade()?true:false);
					}
				}
			}
		}
		// 2005, 1, 3, sobeit add end
		// Creature에게 Damage 입힘
		if (pCreature != NULL)
		{			
			int skillID = pPacket->getSkillType();
			
			if( g_pActionInfoTable->GetSize() <= skillID )
			{
				DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
				
				SendBugReport("[Error:GCSTOOK1H] Exceed SkillType %d", skillID );
				return;
			}

			// 2004, 11, 13, sobeit add start 
			if(skillID == SKILL_PLASMA_ROCKET_LAUNCHER)
			{
				Add_RocketRuncher(g_pPlayer, pCreature);
			}
			// 2004, 11, 13, sobeit add end
//			// 2005, 1, 3, sobeit add start
			else if(skillID == SKILL_HEAL_PASS)
			{
				ExecuteActionInfoFromMainNode(SKILL_HEAL_PASS,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),pCreature->GetID()	,	
					pCreature->GetX(), pCreature->GetY(), 0, 20 , NULL, false);
				
			}
//			// 2005, 1, 3, sobeit add end
			// 현재 무기의 적용을 받는 기술이면..
			if ((*g_pActionInfoTable)[skillID].IsAffectCurrentWeaponAction())
			{
				skillID = g_pPlayer->GetBasicActionInfo();	

				if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
				{
					DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
					return;
				}
			}

			if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
				skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);
	
				
			// 스킬에 따른 예외 처리.
			switch( skillID )
			{
			case SKILL_SHADOW_DANCING :
				SkillShadowDancing(g_pPlayer, pCreature, SKILL_SINGLE_BLOW);
				break;
			// add by Coffee 2007-4-15
			case SKILL_ILLUSION_INVERSION:
				SkillIllendue( g_pPlayer, pCreature, TEMP_SKILL_ILLUSION_INVERSION);
				break;
			// end by Coffee
			case SKILL_ILLENDUE :
				// 2004, 12, 15, sobeit modify start - 힐직이 인챈 라이트볼 쓰는게 버그.
				//SkillIllendue( g_pPlayer, pCreature, SKILL_LIGHT_BALL);
				SkillIllendue( g_pPlayer, pCreature, MAGIC_CAUSE_SERIOUS_WOUNDS);
				// 2004, 12, 15, sobeit modify end
				break;
				
			case SKILL_BLAZE_WALK :
				SkillBlazeWalk( dynamic_cast<MCreature*>(g_pPlayer), pCreature, SKILL_BLAZE_WALK_ATTACK , pPacket->getGrade() );
				break;

			case SKILL_BITE_OF_DEATH :
				pCreature->SetDrainCreatureID( g_pPlayer->GetID() );
				break;
			case SKILL_SET_AFIRE:
				g_pPlayer->AddEffectStatus(EFFECTSTATUS_SET_AFIRE_ATTACK, 20);
				break;
			}
			
			MActionResult* pResult = new MActionResult;

			// 맞은 creature가 때린 player의 방향을 바라보기
			//pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

			int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			// 결과 표현
			//pCreature->PacketSpecialActionResult( pPacket->getSkillType() + g_ActionInfoTable.GetMinResultActionInfo());
			pResult->Add( new MActionResultNodeActionInfo( 
											skillID,
											g_pPlayer->GetID(), 
											pCreature->GetID(), 
											pCreature->GetX(),
											pCreature->GetY(),
											delayFrame ) 
							);	

			//------------------------------------------------------
			//
			// skill에 결과가 있으면 적용 시킨다.
			//
			//------------------------------------------------------
			MActionResultNode* pActionResultNode = CreateActionResultNode(pCreature, skillID, pPacket->getGrade() );

			//------------------------------------------------------
			// NULL이 아니면 실행
			//------------------------------------------------------
			if (pActionResultNode!=NULL)
			{
				pResult->Add( pActionResultNode );
			}


			//------------------------------------------------------
			// EffectStatus가 있다면 붙인다.
			//------------------------------------------------------
			EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
			
			if (es!=EFFECTSTATUS_NULL)
			{
				pResult->Add( new MActionResultNodeAddEffectStatus( pCreature->GetID(), 
																			es, 
																			delayFrame ) );
			}
			
			// [ TEST CODE ] 
			// 맞는 결과 damage를 표현해 준다.			
			/*
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( g_pPlayer->GetID(), g_pPlayer->GetID(), SKILL_ATTACK_MELEE ) );			

			g_pPlayer->PacketAddActionResult(pPacket->getCEffectID(), pResult);
			*/

			// Player의 현재 진행 중인 기술의 결과로 추가시킨다.
			g_pPlayer->PacketAddActionResult( pPacket->getCEffectID(), pResult );		
		}
		
	}
	

#endif

	__END_CATCH
}
