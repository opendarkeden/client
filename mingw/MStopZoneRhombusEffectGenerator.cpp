//----------------------------------------------------------------------
// MStopZoneRhombusEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopZoneRhombusEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"

#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MStopZoneRhombusEffectGenerator	g_StopZoneRhombusEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MStopZoneRhombusEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK;

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	TYPE_SECTORPOSITION	sX, sY;
	sX = g_pTopView->PixelToMapX(egInfo.x0);
	sY = g_pTopView->PixelToMapY(egInfo.y0);

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MEffect*	pEffect;

	MEffectTarget*	pEffectTarget2;
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);
	
	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(sX-1, sY);		// Sector 좌표	
	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
	pEffect->SetCount( egInfo.count , egInfo.linkCount );		// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	bOK = g_pZone->AddEffect( pEffect );

	if (bOK)
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );	
	}


	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);

	
	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(sX+1, sY);		// Sector 좌표						
	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	if (g_pZone->AddEffect( pEffect ))
	{
		// 다음 Effect 생성 정보
		if (egInfo.pEffectTarget == NULL)
		{
			pEffect->SetLink( egInfo.nActionInfo, NULL );
		}
		else
		{
			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
		}

	}

	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(sX, sY-1);		// Sector 좌표						
	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
	pEffect->SetCount( egInfo.count , egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	if (g_pZone->AddEffect( pEffect ))
	{		
		// 다음 Effect 생성 정보
		if (egInfo.pEffectTarget == NULL)
		{
			pEffect->SetLink( egInfo.nActionInfo, NULL );
		}
		else
		{
			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
		}
	}

	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);


	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(sX, sY+1);		// Sector 좌표						
	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
	pEffect->SetCount( egInfo.count , egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	if (g_pZone->AddEffect( pEffect ))
	{
		// 다음 Effect 생성 정보
		if (egInfo.pEffectTarget == NULL)
		{
			pEffect->SetLink( egInfo.nActionInfo, NULL );
		}
		else
		{
			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
		}

	}

	return bOK;
}
