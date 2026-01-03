//--------------------------------------------------------------------------------
//
// Filename    : GCFastMoveHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCFastMove.h"
#include "Player.h"
#include "ClientDef.h"
#include "SkillDef.h"
#include "MTopView.h"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCFastMoveHandler::execute ( GCFastMove * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	int objectID = pPacket->getObjectID();

	// (ox, oy) --> (nx, ny)
	int ox = pPacket->getFromX();
	int oy = pPacket->getFromY();
	int nx = pPacket->getToX();
	int ny = pPacket->getToY();
	ACTIONINFO skillType = (ACTIONINFO)pPacket->getSkillType();

	MCreature* pCreature = NULL;
	
	//------------------------------------------------------------------
	// Player인 경우
	//------------------------------------------------------------------
	if (objectID==g_pPlayer->GetID())
	{
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

		pCreature = g_pPlayer;
	}
	else
	{
		pCreature = g_pZone->GetCreature( objectID );
	}

	if (pCreature == NULL)
	{
		// 캐릭터 없다.
		DEBUG_ADD_FORMAT("[Error] There is no such Creature. id=%d", objectID);
	}
	// Creature있을 때.. 		
	else
	{
		switch(skillType)
		{
		case SKILL_CHARGING_ATTACK:
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_CHARGING_ATTACK,										// 사용 기술 번호
						
							ox, oy, 0,
							pCreature->GetDirection(),														// 사용 방향
							
							pCreature->GetID(),												// 목표에 대한 정보
							nx, ny, 0,
							
							17, //5*16, 
							
							NULL,
							
							false,
							
							0, 0);	

				if(pCreature == g_pPlayer)
				{
					g_pPlayer->SetDelay(1000);
				}
				pCreature->FastMovePosition( nx, ny, true );
				pCreature->SetGhost(3, 16);
			break;
			
		case SKILL_TELEPORT:
			{
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_TELEPORT,										// 사용 기술 번호
						
							ox, oy, 0,
							pCreature->GetDirection(),														// 사용 방향
							
							pCreature->GetID(),												// 목표에 대한 정보
							ox, oy, 0,
							
							20, //5*16, 
							
							NULL,
							
							false,
							
							0, 0);	

				//pCreature->SetPosition(nx, ny);
				pCreature->FastMovePosition( nx, ny, true );
				if(pCreature == g_pPlayer)
				{
					g_pPlayer->SetDelay(1250);
				}
				pCreature->SetGhost(0xFF, 18);

				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_TELEPORT2,										// 사용 기술 번호
						
							nx, ny, 0,
							pCreature->GetDirection(),														// 사용 방향
							
							pCreature->GetID(),												// 목표에 대한 정보
							nx, ny, 0,
							
							20, //5*16, 
							
							NULL,
							
							false,
							
							0, 0);	

			}
			break;

		case SKILL_DUCKING_WALLOP:
			{
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_DUCKING_WALLOP,										// 사용 기술 번호
						
							ox, oy, 0,
							pCreature->GetDirectionToPosition(nx, ny),														// 사용 방향
							
							pCreature->GetID(),												// 목표에 대한 정보
							ox, oy, 0,
							
							10, //5*16, 
							
							NULL,
							
							false,
							
							0, 0);	

				pCreature->FastMovePosition( nx, ny, true );
				pCreature->SetGhost(0xFF, 9);
				if(pCreature == g_pPlayer)
				{
					g_pPlayer->SetDelay(565);
				}
			}
			break;
		case SKILL_BIKE_CRASH:
			{
				MItem*	pItem = MItem::NewItem( ITEM_CLASS_MOTORCYCLE );
				if(pItem != NULL)
				{
					pCreature->AddEffectStatus(EFFECTSTATUS_BIKE_CRASH, 0xffff);
					pItem->SetItemType( 6 );
					((MCreatureWear*)pCreature)->SetAddonItem( pItem );
					delete pItem;
					int TempDir = MTopView::GetDirectionToPosition(pCreature->GetX(), pCreature->GetY(), nx, ny);
					pCreature->SetInstallTurretDirect(TempDir);
					pCreature->SetDirection(TempDir);
					pCreature->SetCurrentDirection(TempDir);
					pCreature->SetInstallTurretCount(0);
					if(pCreature->GetID() == g_pPlayer->GetID())
						g_pPlayer->SetDelay(3000);
				}
				pCreature->FastMovePosition( nx, ny, true );
			}
			break;
		default:
			{
//							
				pCreature->FastMovePosition( nx, ny, true );
				
			}
		}
	}


#endif

	__END_CATCH
}
