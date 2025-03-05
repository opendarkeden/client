//----------------------------------------------------------------------
// MAttackZoneBombEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttackZoneBombEffectGenerator.h"
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
//MAttackZoneBombEffectGenerator	g_AttackZoneParabolaEffectGenerator;


//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttackZoneBombEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	MParabolaEffect* pEffect = new MParabolaEffect(bltType);	
	
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	
	pEffect->SetFrameID( frameID, maxFrame );		// 0번 Effect, Max 3 Frame					

	
	// 방향으로 바뀌는 값
	// 그 방향으로 한 타일 더 안 간다. - -;
	//POINT cxy = MTopView::GetChangeValueToDirection( egInfo.direction );

	int tx = egInfo.x1; // + cxy.x * TILE_X;
	int ty = egInfo.y1; // + cxy.y * TILE_Y;

	// 발사 위치 Pixel좌표	
	//pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z0+TILE_Y );	
	pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z0+(TILE_Y<<1) );	

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );
					
	// 목표 위치 Pixel좌표
	pEffect->SetTarget( tx, ty, egInfo.z1, egInfo.step );	

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
