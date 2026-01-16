//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToSelfOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToSelfOK1.h"
#include "ClientDef.h"
#include "PacketFunction2.h"
#include "SkillDef.h"
#include "MSkillManager.h"
#include "UIMessageManager.h"
#include "MEventManager.h"
#include "UIFunction.h"
#include "MGameStringTable.h"


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToSelfOK1Handler::execute ( GCSkillToSelfOK1 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		

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

	int skillID = pPacket->getSkillType();

	if( g_pActionInfoTable->GetSize() <= skillID )
	{
		DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
		SendBugReport("[ErrorGCSTSOK1H] Exceed SkillType %d", skillID );
		return;
	}

	if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
		skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);

	int resultActionInfo = skillID + (*g_pActionInfoTable).GetMinResultActionInfo();
	
	if( skillID == MAGIC_UN_TRANSFORM)
	{
		// 아우스터즈는 summon sylph 에서 내릴떄 untransform 을 날려주는데 그에대한 검증으로 이게 날라온다-_-
		// 고쳐야 하는데.. 일단 예외 처리로!
		if(g_pPlayer->IsOusters())
		{
			g_pPlayer->SetWaitVerifyNULL();
			return;
		}
		else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
		{
			return;
			//resultActionInfo = RESULT_SKILL_INSTALL_TURRET;
		}
	}
	
	if( skillID == SKILL_ETERNITY && g_pPlayer->IsSlayer() )
	{	
		MEvent event;
		event.eventID = EVENTID_RESURRECT;
		event.eventDelay = 5000;
		event.eventFlag = EVENTFLAG_SHOW_DELAY_STRING;
		event.eventType = EVENTTYPE_ZONE;
		event.m_StringsID.push_back(STRING_MESSAGE_RESURRECT_AFTER_SECONDS);
		
		g_pEventManager->AddEvent(event);
		UI_CloseRequestResurrectWindow();
	}

	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );
	if(resultActionInfo == RESULT_SKILL_CONCEALMENT)				// 이펙트에 맞게 프레임을 적당히 조정해준다.
	{
		int FrameSize = (*g_pActionInfoTable)[resultActionInfo][1].Count;
		int RemainFrame = delayFrame % FrameSize;
		if(RemainFrame < FrameSize / 2 && delayFrame > FrameSize)
		{
			delayFrame -= RemainFrame;
		} else
		{
			delayFrame += FrameSize - RemainFrame;
		}
	}

	g_pPlayer->SetEffectDelayFrame(resultActionInfo, delayFrame );

	// 소울 체인의 경우 기술 썼을때가 아니라 OK됐을때 delay세팅
	if(skillID == SKILL_SOUL_CHAIN)
	{
		if (skillID < MIN_RESULT_ACTIONINFO)
		{
			(*g_pSkillInfoTable)[skillID].SetNextAvailableTime();
		}
//		g_pUIMessageManager->Execute(UI_CLOSE_TRACE_WINDOW, 0, 0, NULL);
	}

	//------------------------------------------------------
	// Player가 기술을 성공했을때 모습..
	//------------------------------------------------------
	g_pPlayer->PacketSpecialActionResult( 
					resultActionInfo,
					g_pPlayer->GetID(),
					g_pPlayer->GetX(),
					g_pPlayer->GetY()
	);

	//------------------------------------------------------------------
	// Player가 Skill을 성공시킨 경우에 날아오는 Packet이므로
	// 결과를 반영시켜야 한다.
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

	//------------------------------------------------------------------
	// effect status를 적용시킨다.
	//------------------------------------------------------------------
	if (g_pPlayer->GetEFFECT_STAT()!=EFFECTSTATUS_NULL)
	{
		//int esDelayFrame = ConvertDurationToFrame( g_pPlayer->GetDURATION() );

		// effect를 붙인다.
		g_pPlayer->AddEffectStatus((EFFECTSTATUS)g_pPlayer->GetEFFECT_STAT(), delayFrame);	
		
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
			g_pPlayer->AddEffectStatus( es, delayFrame );
		}
	}

	if(skillID == SKILL_WILL_OF_LIFE )
	{
		g_pPlayer->CheckRegen();
		(*g_pSkillInfoTable)[skillID].SetAvailableTime( (3 + (g_pPlayer->GetLEVEL() / 10)) * 2 * 1000 );
	}

	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );


	
	//------------------------------------------------------
	//
	// skill에 결과가 있으면 적용 시킨다.
	//
	//------------------------------------------------------
	MActionResultNode* pActionResultNode = CreateActionResultNode(g_pPlayer, skillID);

	//------------------------------------------------------
	// NULL이 아니면 실행
	//------------------------------------------------------
	if (pActionResultNode!=NULL)
	{
		pActionResultNode->Execute();

		delete pActionResultNode;
	}


	__END_CATCH
}
