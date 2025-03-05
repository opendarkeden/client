//----------------------------------------------------------------------
// MFollowPathEffectGenerator.cpp
// 미리 좌표 세팅을 해놓으면 그 좌표로 따라간다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MFollowPathEffectGenerator.h"
#include "MLinearEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include <math.h>
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "MPlayer.h"
#include "SkillDef.h"

typedef std::vector<POINT> FOLLOW_PATH;

FOLLOW_PATH			FollowPath[8];

// 하드코딩-_-각도와 횟수로 하면.... 딴데서 많이 못쓸듯..
void	MakePathWildTyphoon()
{
	int i;
	POINT pt;		

	for(i=0;i<8;i++)
		FollowPath[i].clear();

	// 먼저 상하좌우 세팅
	for(i=0;i<8;i+=2)
	{
		if( i == 2 || i == 6 )
		{
			pt.x = i == 6 ? 1 : -1;
			pt.y = 0;
		} else
		{
			pt.x = 0;
			pt.y = i == 0 ? -1 : 1;
		}
		FollowPath[i].push_back( pt );

		if( i == 2 || i == 6 )
		{
			pt.x = 0;
			pt.y = i == 6 ? -2 : 2;
		} else
		{
			pt.y =0;
			pt.x = i == 0 ? -2 : 2;
		}
		FollowPath[i].push_back( pt );
		
		if( i == 2 || i == 6 )
		{
			pt.x = i == 6 ? -2 : 2;
			pt.y = 0;
		} else
		{
			pt.x = 0;
			pt.y = i == 0 ? 2 : -2;
		}
		FollowPath[i].push_back( pt );

		if( i == 2 || i == 6 )
		{
			pt.x  =0;
			pt.y = i == 6 ? 2 : -2;
		} else
		{
			pt.x = i == 0 ? 2 : -2;
			pt.y = 0;
		}
		FollowPath[i].push_back( pt );

		if( i == 2 || i == 6 )
		{
			pt.x = i == 6 ? 1 : -1;
			pt.y = i == 6 ? -1 : 1;
		} else
		{
			pt.x = i == 0 ? -1 : 1;
			pt.y = i == 0 ? -1 : 1;
		}
		FollowPath[i].push_back( pt );
	}
	
	// 대각선 방향 세팅
	for(i=1;i<8;i+=2)
	{
		if( i == 1 || i == 5 ){
			pt.x = 0;
			pt.y = i == 1 ? 2 : -2;
		} else {
			pt.x = i == 3 ? 2 : -2;
			pt.y = 0;
		}
		FollowPath[i].push_back( pt );

		if(i == 1 || i == 5 ) {
			pt.x = i == 1 ? -2 : 2;
			pt.y = 0;
		} else {
			pt.x = 0;
			pt.y = i == 3 ? 2 : -2;
		}
		FollowPath[i].push_back( pt );

		if(i == 1 || i == 5 ) {
			pt.x = 0;
			pt.y = i == 1 ? -2 : 2;
		} else {
			pt.x = i == 3 ? -2 : 2;
			pt.y = 0;
		}
		FollowPath[i].push_back( pt );

		if(i == 1 || i == 5 ) {
			pt.x = i == 1 ? 1 : -1;
			pt.y = 0;
		} else {
			pt.x = 0;
			pt.y = i == 3 ? -1 : 1;
		}
		FollowPath[i].push_back( pt );

		if( i == 1 || i == 5 ) {
			pt.x = 0;
			pt.y = i == 1 ? 1 : -1;
		} else {
			pt.x = i == 3 ? 1 : -1;
			pt.y = 0;
		}
		FollowPath[i].push_back( pt );
	}
}

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MFollowPathEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	int currentPhase = egInfo.pEffectTarget != NULL ?egInfo.pEffectTarget->GetCurrentPhase() : -1;
	MEffectTarget*	pTarget = egInfo.pEffectTarget;
	
	if( egInfo.nActionInfo == SKILL_WILD_TYPHOON )
		MakePathWildTyphoon();

	BYTE Dir = egInfo.direction;
	
	if( currentPhase-2 >= FollowPath[Dir].size() || currentPhase < 0 )
		return false;

	MLinearEffect* pEffect = new MLinearEffect(bltType);
	
	// 시작 좌표
	TYPE_SECTORPOSITION tX,tY;
	tX = g_pTopView->PixelToMapX( egInfo.x0 );
	tY = g_pTopView->PixelToMapY( egInfo.y0 );

	int sx,sy,sz;
	sx = egInfo.x0;
	sy = egInfo.y0;
	sz = egInfo.z0;

	// 목표 좌표
	int tx = g_pTopView->MapToPixelX(tX+FollowPath[Dir][currentPhase-2].x); 
	int ty = g_pTopView->MapToPixelY(tY+FollowPath[Dir][currentPhase-2].y);
	int tz = egInfo.z1;

	pEffect->SetFrameID( frameID, maxFrame );		// 0번 Effect, Max 3 Frame
	// 발사 위치 Pixel좌표	
	pEffect->SetPixelPosition( sx, sy, sz );	

	// 목표 위치 Pixel좌표
	pEffect->SetTarget( tx, ty, tz, egInfo.step );	
	// 방향 설정
	pEffect->SetDirection( Dir );	
	// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
	pEffect->SetCount( egInfo.count, egInfo.linkCount );
	// 위력
	pEffect->SetPower(egInfo.power);
	// 빛의 밝기
	//pEffect->SetLight( light );

	if (g_pZone->AddEffect( pEffect ))
	{
		if( pTarget != NULL)
		{
			pEffect->SetLink( egInfo.nActionInfo, pTarget );
			pTarget->Set( tx, ty, tz, egInfo.creatureID );			
		}
		return true;
	}

	return false;

}
