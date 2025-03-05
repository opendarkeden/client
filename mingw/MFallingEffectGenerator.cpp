//----------------------------------------------------------------------
// MFallingEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MFallingEffectGenerator.h"
#include "MLinearEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MFallingEffectGenerator	g_FallingEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MFallingEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MLinearEffect*	pEffect;
	int z;
	//---------------------------------------------
	// Effect 생성 - Left Up
	//---------------------------------------------
	z = 300;
	pEffect = new MLinearEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	// 발사 위치 Pixel좌표	(목표위치에서 z만큼 위에)
	// 근데 0으로 할건지. 1로 할건지.. 정할 수 있는 flag가 필요하당..
	// generator를 딴걸 만들든지... 

	//pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z1+z );	
	pEffect->SetPixelPosition( egInfo.x1, egInfo.y1, egInfo.z1+z );	

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );
					
	// 목표 위치 Pixel좌표 (목표위치)
	pEffect->SetTarget( egInfo.x1, egInfo.y1, egInfo.z1, egInfo.step );

	// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
	pEffect->SetCount( egInfo.count, egInfo.linkCount );

	// 위력
	pEffect->SetPower(egInfo.power);
	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	if (g_pZone->AddEffect( pEffect ))
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

		return true;	
	}

	return false;

}
