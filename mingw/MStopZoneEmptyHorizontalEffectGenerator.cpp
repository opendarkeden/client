//----------------------------------------------------------------------
// MStopZoneEmptyHorizontalWallEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopZoneEmptyHorizontalWallEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "SkillDef.h"

#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MStopZoneEmptyHorizontalWallEffectGenerator	g_StopZoneWallEffectGenerator;
//---------------------------------------------
// 방향에 따라서... { 시작보정X, 시작보정Y, 변화X, 변화Y }
//---------------------------------------------
const int g_WallHorizontalDirValue[8][4] = { 
	{ 0, -1, 0, 1 },		// left
	{ -1, -1, 1, 1 },		// leftdown
	{ -1, 0, 1, 0 },		// down
	{ 1, -1, -1, 1 },		// rightdown
	{ 0, -1, 0, 1 },		// right
	{ -1, -1, 1, 1 },		// rightup
	{ -1, 0, 1, 0 },		// up
	{ -1, 1, 1, -1 }		// leftup
};

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MStopZoneEmptyHorizontalWallEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK = false;

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	int	//sX0, sY0, 
		sX1, sY1;

	//sX0 = g_pTopView->PixelToMapX(egInfo.x0);
	//sY0 = g_pTopView->PixelToMapY(egInfo.y0);

	// 지뢰인 경우
	if (egInfo.nActionInfo>=MINE_ANKLE_KILLER 
		&& egInfo.nActionInfo<=MINE_COBRA)
	{
		// 시작 위치
		sX1 = g_pTopView->PixelToMapX(egInfo.x0);
		sY1 = g_pTopView->PixelToMapY(egInfo.y0);
	}
	else
	{
		// 목표 위치
		sX1 = g_pTopView->PixelToMapX(egInfo.x1);
		sY1 = g_pTopView->PixelToMapY(egInfo.y1);
	}

	// (sX0, sY0)에서 (sX1, sY1)을 바라보는 방향을 얻어낸다.
	int lookDirection = egInfo.direction;//MTopView::GetDirectionToPosition(sX0, sY0, sX1, sY1);

	
	//---------------------------------------------
	// 시작 값
	//---------------------------------------------
	// step(개수)   stepMulti(시작값 변화)
	// 1			 0
	// 3			 1
	// 5			 2
	int stepMulti = (egInfo.step>>1);
	int sX = sX1 + g_WallHorizontalDirValue[lookDirection][0] * stepMulti;
	int sY = sY1 + g_WallHorizontalDirValue[lookDirection][1] * stepMulti;
	int cX = g_WallHorizontalDirValue[lookDirection][2];
	int cY = g_WallHorizontalDirValue[lookDirection][3];

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	int x = egInfo.x1, 
		y = egInfo.y1;

	MEffect*	pEffect;
	//---------------------------------------------
	// [1]Effect 생성
	//---------------------------------------------
	for (int i=0; i<egInfo.step; i++)
	{
		// 가운데꺼 하나를 뺀다.
		if (i!=stepMulti)
		{
			pEffect = new MEffect(bltType);

			//---------------------------------------------
			// 한번은 EffectTarget을 new할 필요가 없다.
			//---------------------------------------------
			
			pEffect->SetFrameID( frameID, maxFrame );	

			pEffect->SetPosition(sX, sY);		// Sector 좌표		
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
			bool bAdd = g_pZone->AddEffect( pEffect );

			/*
			if (bAdd)
			{
				int num = rand() % maxFrame;
					
				for (int nf=0; nf<num; nf++)
				{
					pEffect->NextFrame();
				}
			}
			*/

			if (bAdd)
			{
				if (!bOK)
				{				
					// 처음으로 추가된 effect에 대해서 link설정
					pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

					bOK = true;
				}
				else if (egInfo.pEffectTarget == NULL)
				{
					pEffect->SetLink( egInfo.nActionInfo, NULL );
				}
				else
				{
					MEffectTarget* pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
					pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
					pEffectTarget2->Set( x, y, egInfo.z0, egInfo.creatureID );
				}
			}
		}

		// 다음 좌표
		sX += cX;
		sY += cY;

		//
		x += TILE_X*cX;
		y += TILE_Y*cY;
	}

	return bOK;
}
