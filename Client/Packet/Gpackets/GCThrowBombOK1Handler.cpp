//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowBombOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCThrowBombOK1.h"
#include "ClientDef.h"
#include "PacketFunction2.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowBombOK1Handler::execute ( GCThrowBombOK1 * pPacket , Player * pPlayer )
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
		int skillID = GetBombActionInfo( pPacket->getItemType() );//pPacket->getSkillType();	

		if (skillID < 0)
		{
			DEBUG_ADD_FORMAT("[Error] no match skillType. itemType=%d", pPacket->getItemType());
			return;
		}

		int useSkillID = skillID;
		
		if ((*g_pActionInfoTable)[skillID].IsAffectCurrentWeaponAction())
		{
			useSkillID = g_pPlayer->GetBasicActionInfo();

			if (useSkillID >= g_pActionInfoTable->GetMinResultActionInfo())
			{
				DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", useSkillID);
				return;
			}
		}
			
		// 의미없다.
		int delayFrame = 16;//ConvertDurationToFrame( pPacket->getDuration() );

		int size = pPacket->getCListNum();

		MActionResult* pResult = new MActionResult;

			
		//------------------------------------------------------
		// Player가 기술을 성공했을때 모습..
		//------------------------------------------------------
		//int resultActionInfo = skillID + (*g_pActionInfoTable).GetMinResultActionInfo();
		//g_pPlayer->PacketSpecialActionResult( resultActionInfo );

		int targetID = g_pPlayer->GetID();
		if ((*g_pActionInfoTable)[skillID].GetActionResultID()==ACTIONRESULTNODE_ACTIONINFO)
		{
			targetID = OBJECTID_NULL;
		}

		pResult->Add( new MActionResultNodeActionInfo( 
											skillID,
											g_pPlayer->GetID(), 
											targetID, 
											pPacket->getX(),
											pPacket->getY(),
											delayFrame ) 
							);	

		if (size!=0)
		{
			// 결과를 당하는 각각의 creature에 대해서 결과 표현
			for (int i=0; i<size; i++)
			{
				int targetID = pPacket->popCListElement();
				MCreature* pCreature = g_pZone->GetCreature( targetID );

				// Creature에게 Damage? 입힘
				if (pCreature != NULL)
				{
					//------------------------------------------------------
					// EffectStatus가 있다면 붙인다.
					//------------------------------------------------------
					EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
					
					if (es!=EFFECTSTATUS_NULL)
					{
						pResult->Add( new MActionResultNodeAddEffectStatus( targetID, es, delayFrame ) );
					}

					// 결과 바로 표현
					//pCreature->PacketSpecialActionResult( pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo());
			
					// 기술의 결과를 표현하는 resultNode를 생성한다.
					//pResult->Add( new MActionResultNodeActionInfo( 
					//							g_pPlayer->GetID(), 
					//							pCreature->GetID(), 
					//							pPacket->getSkillType(),
					//							delayFrame ) 
					//			);			
					
					//------------------------------------------------------
					//
					// skill에 결과가 있으면 적용 시킨다.
					//
					//------------------------------------------------------
					MActionResultNode* pActionResultNode = NULL;

					switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
					{
						//------------------------------------------------------
						// 다른 ActionInfo 실행
						//------------------------------------------------------
						case ACTIONRESULTNODE_ACTIONINFO :
							pActionResultNode =  new MActionResultNodeActionInfo( 
																useSkillID,//(*g_pActionInfoTable)[skillID].GetActionResultValue(),
																g_pPlayer->GetID(), 
																pCreature->GetID(), 
																pCreature->GetX(), 
																pCreature->GetY(), 
																delayFrame);
						break;

						//------------------------------------------------------
						// Burrow 등등..
						//------------------------------------------------------
						default :
							pActionResultNode = CreateActionResultNode(pCreature, useSkillID);
						
					}

					//------------------------------------------------------
					// NULL이 아니면 같이 적용
					//------------------------------------------------------
					if (pActionResultNode!=NULL)
					{
						pResult->Add( pActionResultNode );
					}
				}
			}			
		}			
		
		//------------------------------------------------------
		// range를 direction에 적용시키는 경우
		//------------------------------------------------------
		//if ((*g_pActionInfoTable)[skillID].IsOptionRangeToDirection())
		{
		//	g_pPlayer->SetDirection( pPacket->getDir() );	
		}		
	
		// Player의 현재 진행 중인 기술의 결과로 추가시킨다.		
		g_pPlayer->PacketAddActionResult( 0, pResult );

	}


#endif

	__END_CATCH
}
