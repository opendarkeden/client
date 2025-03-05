//----------------------------------------------------------------------
// MRippleZoneWideEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MRippleZoneWideEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MCreature.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MRippleZoneWideEffectGenerator	g_RippleZoneWideEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MRippleZoneWideEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK = false;

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
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	int i;
	int n=(egInfo.power<<1) - 1;

	// 일단 한 칸 움직인다.
	TYPE_SECTORPOSITION x=sX, y=sY;
	int cX, cY;
	MCreature::GetPositionToDirection(x,y, egInfo.direction);
	
	// 방향에 따라서 확장될 좌표를 달리한다.
	switch (egInfo.direction)
	{
		case DIRECTION_LEFT : case DIRECTION_RIGHT :			
			y = y-(egInfo.power-1);
			cX	= 0;
			cY	= 1;			
		break;

		case DIRECTION_UP : case DIRECTION_DOWN :
			x = x-(egInfo.power-1);			
			cX	= 1;
			cY	= 0;
		break;

		case DIRECTION_LEFTUP : case DIRECTION_RIGHTDOWN :
			x = x-(egInfo.power-1);
			y = y+(egInfo.power-1);
			cX	= 1;
			cY	= -1;			
		break;

		case DIRECTION_LEFTDOWN : case DIRECTION_RIGHTUP :
			x = x-(egInfo.power-1);
			y = y-(egInfo.power-1);
			cX	= 1;
			cY	= 1;			
		break;
	}

	// Effec생성
	for (i=0; i<n; i++)
	{
		// Zone의 영역을 벗어나는 경우..
		if (x<0 || y<0 || x>=g_pZone->GetWidth() || y>=g_pZone->GetHeight())
			continue;

		pEffect = new MEffect(bltType);

		pEffect->SetFrameID( frameID, maxFrame );	

		// 다음 좌표를 정한다.			
		pEffect->SetPosition(x, y);		// Sector 좌표		
		
		// 방향 설정
		pEffect->SetDirection( egInfo.direction );

		pEffect->SetZ(egInfo.z0);			
		pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
		pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

		// 위력
		pEffect->SetPower(egInfo.power+1);

		// 빛의 밝기
		//pEffect->SetLight( light );

		// Zone에 추가한다.
		bool bAdd = g_pZone->AddEffect( pEffect );

		if (bAdd)
		{
			if (i==(egInfo.power-1))
			{
				bOK = true;
					
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
			}
			else
			{
				pEffect->SetLink( egInfo.nActionInfo, NULL );
			}
		}


		// 다음 좌표
		x += cX;
		y += cY;
	}


	return bOK;
}
