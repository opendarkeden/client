//----------------------------------------------------------------------
// MAttachZoneSelectableEffectGenerator.cpp
//----------------------------------------------------------------------
// Tile과 맞붙은 Effect들을 생성한다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachZoneSelectableEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachZoneSelectableEffectGenerator	g_StopZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttachZoneSelectableEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	/*
	int	sX, sY;
	sX = g_pTopView->PixelToMapX(egInfo.x0);
	sY = g_pTopView->PixelToMapY(egInfo.y0);

	//---------------------------------------------
	// Map좌표를 다시 pixel좌표로 바꾼다.
	//---------------------------------------------
	POINT pixelPoint;
	pixelPoint = g_pTopView->MapToPixel(sX, sY);
	*/
	// 2001.10.6
	POINT pixelPoint = { egInfo.x0, egInfo.y0 };

	
	//----------------------------------------------------------------
	// 바닥에 떨어지는 피 종류 랜덤하게.. 하드 코딩.. 에휴~
	//----------------------------------------------------------------
	if (est==EFFECTSPRITETYPE_BLOOD_GROUND_2_1)
	{
		est = EFFECTSPRITETYPE_BLOOD_GROUND_2_1 + rand()%4;

		pixelPoint.x += (rand()%g_TILE_X_HALF) - (g_TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}
	else if (est==EFFECTSPRITETYPE_BLOOD_GROUND_1_1)
	{
		est = EFFECTSPRITETYPE_BLOOD_GROUND_1_1 + rand()%5;

		pixelPoint.x += (rand()%g_TILE_X_HALF) - (g_TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;



	//---------------------------------------------
	// MaxFrame의 값을 알아온다.
	//---------------------------------------------
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MEffect*	pEffect;
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MSelectableEffect(bltType);

	
	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPixelPosition(pixelPoint.x, pixelPoint.y, egInfo.z0);		// pixel좌표		

	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.

	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	
	// Ground Effect로..
	// Zone에 추가한다.
	if (g_pZone->AddGroundEffect( pEffect ))
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

		return true;
	}

	return false;
}
