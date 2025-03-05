//#define __3D_IMAGE_OBJECT__					// by sonee

//----------------------------------------------------------------------
// MTopViewDraw.cpp
//----------------------------------------------------------------------
//
// 복잡한 Draw 함수 부분만 따로 빼놓는다.
// 작업하기가 영 불편해서 
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include <math.h>
#include <stdio.h>
#include "MZone.h"
#include "MCreature.h"
#include "MFakeCreature.h"
#include "assert.h"

#include "MTopView.h"
#include "CMessageArray.h"
#include "MZoneTable.h"
#include "MEventManager.h"
#include "DebugInfo.h"
#include "Client.h"

void SweeperKeeperProc( MCreature* pCreature, int& action, int& frame, int& direction )	// 672
{
	if( action == ACTION_MOVE )
	{
		frame = (g_CurrentFrame + pCreature->GetID()) % 11;		
		
		if( (g_CurrentFrame % 14 ) == 0 )
			PlaySound( SOUND_SWEEPER_REPEAT );		
	}
	direction = 2;
}

void GildreProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if( pCreature->HasEffectStatus( EFFECTSTATUS_GDR_FLOATING )/* && action == ACTION_MOVE */)
	{
		action = ACTION_VAMPIRE_DRAIN;
		frame = 6 + (frame & 0x07);		// 흡혈 동작에서 반복 frame의 시작이 6이다.		
		
		//			m_MoveAction = ACTION_DRAINED;
		//SetAction(ACTION_DRAINED);
	}
	if(pCreature->IsDead())
	{
		if(pCreature->GetActionCount()>=pCreature->GetActionCountMax())
		{
			if(pCreature->GetCreatureType()  == 723)
				frame = 46; // 무릎꿇은 상태로..나둔당..			
		}
	}
}

void FrozenGildreProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(g_CurrentFrame&0x01)
	{
		pCreature->SetDirection((direction+1)%8);
		direction = pCreature->GetDirection();
	}
}

void FishShopLandLord( MCreature* pCreature, int& action, int& frame, int& direction )
{
	// 생선가게 주인 - 퀘스트용, 일단 누워있게..나중에 분석점 하고 수정하자..
	action = 6;
	frame = 14;
}

void HornProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(!pCreature->HasEffectStatus(EFFECTSTATUS_COMA) && !pCreature->IsDead())
	{
		action = ACTION_STAND;
	}
	else
		action = ACTION_DIE;
	
	direction = 2;
}

void TepezProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(!pCreature->HasEffectStatus(EFFECTSTATUS_COMA) && !pCreature->IsDead())
		action = ACTION_STAND;
	else
		action = ACTION_DIE;
	direction = 1;
}

void OustersNPC_675( MCreature* pCreature, int& action, int& frame, int& direction )
{
	action = ACTION_MAGIC;
	frame = 0;
	direction = 3;
}

void WayPointKindOfThree( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(pCreature->IsDead()) return;
	action = ACTION_STAND;
	frame = 0;
	direction = 2;

}

void ObstacleProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(pCreature->IsDead())
	{
		action = ACTION_DIE;
		direction = 2;
		if(pCreature->GetActionCount()>=pCreature->GetActionCountMax()) return;
	}
	else
	{
		action = ACTION_STAND;
		frame = 0;
		direction = 2;
	}
}

void MakeStandAndSouthDirectionProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	// 아래 방향에 stand 액션, 프레임을 0으로 만든다.
	action = ACTION_STAND;
	frame = 0;
	direction = 2;
}

void CraymoreProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	action = ACTION_STAND;
	frame = 0;
	direction = 1;
}

void GuildTowerProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	direction = 1;
	if(pCreature->IsAlive())
	{
		action = ACTION_STAND;
		frame = 0;
	}
}

void SummonGoreGrand( MCreature* pCreature, int& action, int& frame, int& direction )
{
	direction = 2;
	if(pCreature->IsDead())
	{
		action = ACTION_DIE;
		if(pCreature->GetActionCount()>=pCreature->GetActionCountMax()) return;		
	}
}

void Ctype_764_765_Proc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(action == ACTION_DIE && pCreature->GetCreatureType() == 765)
		direction = 2;
	if(action != ACTION_MAGIC)
		pCreature->SetShadowCount(0);
	if(action == ACTION_MOVE)
	{
		if(frame == 1 || frame == 8) // 실제로는 3,10프레임이 발 딛는 프레임인데..두프레임정도 앞서면 싱크가 대충 맞지 않을까...ㅎㅎ
		{
			MEvent event;
			event.eventID = EVENTID_METEOR_SHAKE;
			event.eventType = EVENTTYPE_ZONE;
			event.eventDelay = 300;
			event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
			event.parameter3 = 1;
			g_pEventManager->AddEvent(event);
		}
	}
	else if(action == ACTION_DRAINED)
	{
		action = ACTION_ATTACK;
		if(frame>4)
		{
			frame = 4;
			
			if(g_CurrentFrame&0x01)
			{
				direction = ((direction - 1 ) + 8)%8;
				pCreature->SetCurrentDirection(direction);
			}
		}
	}
}
// 2004, 12, 11, sobeit add start
void GuildWarGateProc( MCreature* pCreature, int& action, int& frame, int& direction )
{
	action = ACTION_STAND;
	direction = 1;
	if(pCreature->IsAlive())
	{
		int currentHP		= max(0,int(pCreature->GetHP()));
		frame = 4 -(currentHP*5/pCreature->GetMAX_HP());
		if(frame<0) frame = 0;
	}
	else
		frame = 4;
}
// 2004, 12, 11, sobeit add end
// 2005, 1, 17, sobeit add start - 이성의 봉인
void Ctype_792( MCreature* pCreature, int& action, int& frame, int& direction )
{
	if(action != ACTION_DIE)
		action = ACTION_STAND;
	direction = 3;
}
// 2005, 1, 17, sobeit add end
void MakeDirectionToSouth( MCreature* pCreature, int& action, int& frame, int& direction )
{
	direction = 2;
}

void	MTopView::InitMapingExceptionProc()
{
	int i;
	for( i = 371; i <= 376; ++i )
		RegistExceptionProcFunction( i , MakeDirectionToSouth );

	RegistExceptionProcFunction( 433 , TepezProc );
	RegistExceptionProcFunction( 482 , MakeDirectionToSouth );

	for( i = 526; i <= 549; ++i )
		RegistExceptionProcFunction( i , MakeDirectionToSouth );
	for( i = 560; i <= 563; ++i )
		RegistExceptionProcFunction( i , MakeDirectionToSouth );
	
	RegistExceptionProcFunction( 636 , FishShopLandLord );
	RegistExceptionProcFunction( 650 , MakeDirectionToSouth );
	for( i = 660; i <= 670; ++i )
		RegistExceptionProcFunction( i , MakeDirectionToSouth );
	
	RegistExceptionProcFunction( 672 , SweeperKeeperProc );
	RegistExceptionProcFunction( 673 , MakeDirectionToSouth );
	RegistExceptionProcFunction( 675 , OustersNPC_675 );
	RegistExceptionProcFunction( 698 , HornProc );
	
	RegistExceptionProcFunction( 717 , GildreProc );
	RegistExceptionProcFunction( 721 , GildreProc );
	RegistExceptionProcFunction( 722 , FrozenGildreProc );
	RegistExceptionProcFunction( 723 , GildreProc );
	RegistExceptionProcFunction( 730 , WayPointKindOfThree );
	RegistExceptionProcFunction( 731 , WayPointKindOfThree );
	RegistExceptionProcFunction( 732 , WayPointKindOfThree );
	RegistExceptionProcFunction( 734 , GuildTowerProc );
	RegistExceptionProcFunction( 738 , MakeStandAndSouthDirectionProc );
	RegistExceptionProcFunction( 739 , MakeStandAndSouthDirectionProc );
	RegistExceptionProcFunction( 740 , CraymoreProc );
	RegistExceptionProcFunction( 741 , MakeStandAndSouthDirectionProc );
	
	for( i = 743; i <= 748; ++i )
		RegistExceptionProcFunction( i, ObstacleProc );
	
	RegistExceptionProcFunction( 753 , SummonGoreGrand );
	RegistExceptionProcFunction( 764 , Ctype_764_765_Proc );
	RegistExceptionProcFunction( 765 , Ctype_764_765_Proc );

	// 2004, 12, 11, sobeit add start
	RegistExceptionProcFunction( 726 , GuildWarGateProc );
	RegistExceptionProcFunction( 727 , GuildWarGateProc );
	RegistExceptionProcFunction( 728 , GuildWarGateProc );
	RegistExceptionProcFunction( 729 , GuildWarGateProc );
	// 2004, 12, 11, sobeit add end
	
	// 2005, 1, 17, sobeit add start - 이성의 봉인
	RegistExceptionProcFunction( 792 , Ctype_792 );
	// 2005, 1, 17, sobeit add end

}

void	MTopView::RegistExceptionProcFunction( DWORD objectID, DrawCreatureExceptionProc proc )
{
	Assert( m_ExceptionProcMap.find( objectID ) == m_ExceptionProcMap.end() );

	m_ExceptionProcMap[ objectID ] = proc;
}

