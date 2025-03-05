//----------------------------------------------------------------------
// MAttackCreatureParabolaEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttackCreatureParabolaEffectGenerator.h"
#include "MParabolaEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttackCreatureParabolaEffectGenerator	g_AttackCreatureParabolaEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttackCreatureParabolaEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	// creature의 좌표
	int cx, cy, cz;	

	// 목표 위치 Pixel좌표
	MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

	if (pCreature == NULL)
	{
		return false;
	}
	
	// Creture의 좌표로 목표좌표를 설정한다.
	cx = g_pTopView->MapToPixelX( pCreature->GetX() );
	cy = g_pTopView->MapToPixelY( pCreature->GetY() );
	cz = pCreature->GetZ() + TILE_Y;	// 한 타일정도 위로 빼준다.

	// 포물선 Effect생성
	MParabolaEffect* pEffect = new MParabolaEffect(bltType);	
	
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	
	pEffect->SetFrameID( frameID, maxFrame );

	// 발사 위치 Pixel좌표	- 한 타일정도 위로 빼준다.
	pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z0+TILE_Y );	
					
	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 목표 위치 Pixel좌표
	pEffect->SetTarget( cx,cy,cz, egInfo.step );

	// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
	pEffect->SetCount( egInfo.count, egInfo.linkCount );

	// 위력
	pEffect->SetPower(egInfo.power);
	
	// 빛의 밝기
	//pEffect->SetLight( light );

	if (g_pZone->AddEffect( pEffect ))
	{
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

		return true;
	}

	return false;

}
