//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMineExplosionOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCMineExplosionOK1.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCMineExplosionOK1Handler::execute ( GCMineExplosionOK1 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
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
		int skillID = GetMineActionInfo( pPacket->getItemType() );

		if (skillID >= 0)
		{		
			//int resultActionInfo = skillID + (*g_pActionInfoTable).GetMinResultActionInfo();

			//------------------------------------------------------
			// 결과 생성
			//------------------------------------------------------
			MActionResult* pResult = new MActionResult;
		
			DWORD delayFrame = 16;//ConvertDurationToFrame( pPacket->getDuration() );

			int size = pPacket->getCListNum();

			for (int i=0; i<size; i++)
			{
				//pResult->Add( new MActionResultNodeActionInfo( 
				//					OBJECTID_NULL,		// 사용자 없어도 되는걸까?
				//					pPacket->popCListElement(), 
				//					skillType,
				//					delayFrame ) );
				//------------------------------------------------------
				//
				// skill에 결과가 있으면 적용 시킨다.
				//
				//------------------------------------------------------
				int targetID = pPacket->popCListElement();

				MCreature* pTargetCreature = g_pZone->GetCreature( targetID );

				if (pTargetCreature!=NULL)
				{
					//------------------------------------------------------
					// EffectStatus가 있다면 붙인다.
					//------------------------------------------------------
					EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
								
					if (es!=EFFECTSTATUS_NULL)
					{
						pResult->Add( new MActionResultNodeAddEffectStatus( targetID, es, delayFrame ) );
					}

					MActionResultNode* pActionResultNode = NULL;

					/*
					switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
					{
						//------------------------------------------------------
						// 다른 ActionInfo 실행
						//------------------------------------------------------
						case ACTIONRESULTNODE_ACTIONINFO :
							pActionResultNode =  new MActionResultNodeActionInfo( 
																(*g_pActionInfoTable)[skillID].GetActionResultValue(),
																OBJECTID_NULL, // 사용자 없다 - -;
																targetID, 
																pTargetCreature->GetX(),
																pTargetCreature->GetY(),
																delayFrame);
						break;

						//------------------------------------------------------
						// Burrow
						//------------------------------------------------------
						case ACTIONRESULTNODE_CREATURE_BURROW :
							pActionResultNode = new MActionResultNodeCreatureBurrow( targetID );
						break;
					}
					*/
					pActionResultNode =  new MActionResultNodeActionInfo( 
																skillID, //resultActionInfo,
																targetID,
																targetID, 
																pTargetCreature->GetX(),
																pTargetCreature->GetY(),
																delayFrame);

					//------------------------------------------------------
					// NULL이 아니면 같이 적용
					//------------------------------------------------------
					if (pActionResultNode!=NULL)
					{
						pResult->Add( pActionResultNode );
					}
				}
			}

			//------------------------------------------------------
			// Sector에 바로 적용
			//------------------------------------------------------
			int direction = pPacket->getDir();

			SkillToSector(skillID, 
							pPacket->getX(),
							pPacket->getY(),
							direction, 
							delayFrame,
							pResult);
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] Mine Type is Wrong. type = %d", pPacket->getItemType());
		}

		//------------------------------------------------------------------
		// Player가 당한것이므로..
		//------------------------------------------------------------------
		// 상태값을 바꾼다.
		//------------------------------------------------------------------
		AffectModifyInfo(g_pPlayer, pPacket);
	}

#endif

	__END_CATCH
}
