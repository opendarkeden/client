//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToObjectOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToObjectOK2.h"
#include "ClientDef.h"
#include "MSkillManager.h"
#include "PacketFunction2.h"

// [새기술2]
#include "SkillDef.h"	
void	SkillShadowDancing(MCreature* pUserCreature, MCreature* pTargetCreature, int skillID);
extern void Add_RocketRuncher(MCreature* UserCreature, MCreature* TargetCreature);
extern void Add_Wild_Wolf(MCreature* UserCreature, MCreature* TargetCreature, bool IsEatCorpse);
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToObjectOK2Handler::execute ( GCSkillToObjectOK2 * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);


	int skillID = pPacket->getSkillType();
	
	if( g_pActionInfoTable->GetSize() <= skillID )
	{
		DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
		SendBugReport("[Error:GCSTOOK2H] Exceed SkillType %d", skillID );
		return;
	}


	if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0 )
		skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);

	int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

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


		// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
		// [ TEST CODE ]
		MActionResult* pResult = new MActionResult;

		// 2005, 1, 3, sobeit add start
		if(pPacket->getSkillType() == SKILL_WILD_WOLF)
		{
			if(pCreature != NULL)
				Add_Wild_Wolf(pCreature, g_pPlayer, pPacket->getGrade()?true:false);
			else
			{
				MItem* pItem = g_pZone->GetItem(pPacket->getObjectID());
				if(pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_CORPSE)
				{
					pCreature = ((MCorpse*)pItem)->GetCreature();
					if(pCreature != NULL)
					{
						Add_Wild_Wolf(pCreature, g_pPlayer, pPacket->getGrade()?true:false);
					}
				}
			}
		}
		// 2005, 1, 3, sobeit add end
		//------------------------------------------------------------------
		// effect status를 적용시킨다.
		//------------------------------------------------------------------
		if (g_pPlayer->GetEFFECT_STAT()!=EFFECTSTATUS_NULL)
		{
			//int esDelayFrame = ConvertDurationToFrame( g_pPlayer->GetDURATION() );
			pResult->Add( new MActionResultNodeAddEffectStatus( 
										g_pPlayer->GetID(), 
										(EFFECTSTATUS)g_pPlayer->GetEFFECT_STAT(), 
										delayFrame ));

			g_pPlayer->SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );
		}
		else
		{
			//------------------------------------------------------
			// EffectStatus가 있다면 붙인다.
			//------------------------------------------------------
			EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
			
			if (es!=EFFECTSTATUS_NULL)
			{
				pResult->Add( new MActionResultNodeAddEffectStatus( g_pPlayer->GetID(),
																		es, 
																		delayFrame ) );
			}
		}

		//------------------------------------------------------
		// [새기술]
		// Typhoon 맞은 경우는 한동안 못 움직이게 한다.
		// 임시로(-_-;) 1초
		// player만 못 움직이도록 막으면 된다.
		//------------------------------------------------------
		if (skillID==SKILL_TYPHOON || skillID == SKILL_WILD_TYPHOON)
		{
			if( pPacket->getGrade() == 1 || skillID == SKILL_WILD_TYPHOON )
				g_pPlayer->SetDelay( 1000 );

		}
		// 2004, 11, 13, sobeit add start 
		if(skillID == SKILL_PLASMA_ROCKET_LAUNCHER)
		{
			Add_RocketRuncher(pCreature, g_pPlayer);
		}
		// 2004, 11, 13, sobeit add end
	
		// 현재 무기의 적용을 받는 기술이면..
		if ((*g_pActionInfoTable)[skillID].IsAffectCurrentWeaponAction()
			&& pCreature!=NULL)
		{
			skillID = pCreature->GetBasicActionInfo();

			if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
			{
				DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
				return;
			}
		}

		pResult->Add( new MActionResultNodeActionInfo( 
								skillID, 
								pPacket->getObjectID(), 
								g_pPlayer->GetID(), 
								g_pPlayer->GetX(),
								g_pPlayer->GetY(),
								delayFrame ) );	

		//------------------------------------------------------
		//
		// skill에 결과가 있으면 적용 시킨다.
		//
		//------------------------------------------------------
		MActionResultNode* pActionResultNode = CreateActionResultNode(g_pPlayer, skillID, pPacket->getGrade() );

		//------------------------------------------------------
		// NULL이 아니면 실행
		//------------------------------------------------------
		if (pActionResultNode!=NULL)
		{
			pResult->Add( pActionResultNode );
		}

		//------------------------------------------------------
		// 사용자가 없는 경우
		//------------------------------------------------------
		if (pCreature == NULL)
		{
			ExecuteActionInfoFromMainNode(
						skillID + (*g_pActionInfoTable).GetMinResultActionInfo(),										// 사용 기술 번호
					
						g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetZ(),
						g_pPlayer->GetDirection(),														// 사용 방향
						
						g_pPlayer->GetID(),												// 목표에 대한 정보
						g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetZ(),
						
						delayFrame,													// 기술의 (남은) 지속 시간		
						
						pResult,
						
						false);			// 기술 첨부터 시작한다.
		}
		//------------------------------------------------------
		// 사용자가 있는 경우
		//------------------------------------------------------
		else
		{
			if(skillID == SKILL_ABSORB_SOUL)
				pCreature->ClearStopBloodDrain();

			// [새기술2]
			// ShadowDancing인지 체크하고 임시로(-_-;) DoubleImpacet를 쓴다.

			switch( skillID )
			{
			case SKILL_SHADOW_DANCING :
				SkillShadowDancing(pCreature, g_pPlayer, SKILL_SINGLE_BLOW);
				break;
			// add by Coffee 2007-4-15
			case SKILL_ILLUSION_INVERSION:
				SkillIllendue( pCreature, g_pPlayer, TEMP_SKILL_ILLUSION_INVERSION);
				break;
			// end by Coffee	
			case SKILL_ILLENDUE :
				// 2004, 12, 15, sobeit modify start - 힐직이 인챈 라이트볼 쓰는게 버그.
				//SkillIllendue( pCreature, g_pPlayer, SKILL_LIGHT_BALL );
				SkillIllendue( pCreature, g_pPlayer, MAGIC_CAUSE_SERIOUS_WOUNDS );
				// 2004, 12, 15, sobeit modify end
				break;
				
			case SKILL_BLAZE_WALK :
				SkillBlazeWalk( pCreature , dynamic_cast<MCreature*>(g_pPlayer), SKILL_BLAZE_WALK_ATTACK, pPacket->getGrade()  );
				break;

			case SKILL_BITE_OF_DEATH :
				g_pPlayer->SetDrainCreatureID( pPacket->getObjectID() );
				break;
			case SKILL_SET_AFIRE:
				pCreature->AddEffectStatus(EFFECTSTATUS_SET_AFIRE_ATTACK, 20);
				break;
			}
			
			//------------------------------------------------------
			// Player에게 protection이 걸려있는 경우
			//------------------------------------------------------
			if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_PROTECTION_FROM_ACID))
			{
				//------------------------------------------------------
				// PROTECTION_FROM_ACID
				//------------------------------------------------------
				if ((*g_pSkillInfoTable)[skillID].GetSkillStep()==SKILL_STEP_VAMPIRE_ACID)
				{
					skillID = PROTECTION_FROM_ACID;					
				}				
			}			


			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 방향을 바라보기
			pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToOther(
								skillID, 
								g_pPlayer->GetID(),//pPacket->getObjectID(),
								pResult						// 결과
			);
		}
	}		
	
	/*
	//------------------------------------------------------
	// Player가 기술을 당했을 때의 모습..
	//------------------------------------------------------
	int resultActionInfo =  pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo();
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
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}
