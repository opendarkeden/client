//----------------------------------------------------------------------
// MRippleZoneEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MRippleZoneEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MCreature.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MRippleZoneEffectGenerator	g_RippleZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MRippleZoneEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	TYPE_SECTORPOSITION	sX, sY;
	sX = g_pTopView->PixelToMapX(egInfo.x0);
	sY = g_pTopView->PixelToMapY(egInfo.y0);

	// 다음 좌표를 정한다.	
	TYPE_SECTORPOSITION x=sX, y=sY;
	MCreature::GetPositionToDirection(x,y, egInfo.direction);

	// Zone의 영역을 벗어나는 경우..
	if (x<0 || y<0 || x>=g_pZone->GetWidth() || y>=g_pZone->GetHeight())
			return false;

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MEffect*	pEffect;
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(x, y);		// Sector 좌표		
	
	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.

	// 그림에 따라서... 바닥에 추가하기도 한다. 역쉬 하드코딩 ㅋㅋ..
	if (egInfo.effectSpriteType==EFFECTSPRITETYPE_EARTHQUAKE_1
		|| egInfo.effectSpriteType==EFFECTSPRITETYPE_EARTHQUAKE_2
		|| egInfo.effectSpriteType==EFFECTSPRITETYPE_EARTHQUAKE_3
		|| egInfo.effectSpriteType==EFFECTSPRITETYPE_POWER_OF_LAND_STONE_1
		|| egInfo.effectSpriteType==EFFECTSPRITETYPE_POWER_OF_LAND_STONE_2
		|| egInfo.effectSpriteType==EFFECTSPRITETYPE_POWER_OF_LAND_STONE_3
		)
	{
		return g_pZone->AddGroundEffect( pEffect );
	}		
		
	if (g_pZone->AddEffect( pEffect ))
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

		return true;
	}

	return false;
}
