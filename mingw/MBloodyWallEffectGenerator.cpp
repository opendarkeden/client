//----------------------------------------------------------------------
// MBloodyWallEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MBloodyWallEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"

#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MBloodyWallEffectGenerator	g_BloodyWallEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MBloodyWallEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK = false;

	int est = egInfo.effectSpriteType;

	// 하드 하드~
	if (est>=EFFECTSPRITETYPE_BLOODY_WALL_1
		&& est<=EFFECTSPRITETYPE_BLOODY_WALL_3)
	{
		est = EFFECTSPRITETYPE_BLOODY_WALL_1 + rand()%3;
	}


	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	bool			repeatFrame	= (*g_pEffectSpriteTypeTable)[est].RepeatFrame;

	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	int	tx, ty;
	
	tx = egInfo.x1;
	ty = egInfo.y1;

	// (sX0, sY0)에서 (sX1, sY1)을 바라보는 방향을 얻어낸다.
	int lookDirection = egInfo.direction;//MTopView::GetDirectionToPosition(sX0, sY0, sX1, sY1);

	//---------------------------------------------
	// 방향에 따라서... { 시작보정X, 시작보정Y, 변화pixelX, 변화pixelY }
	//---------------------------------------------
	// 5개 출력
	/*
	const int dirValue[8][4] = { 
		{		0,	-12*2,		0,		12 },		// left에서 up
		{	-24*2,	-12*2,		24,		12 },		// leftdown
		{	-24*2,		0,		24,		0 },		// down
		{	24*2,	-12*2,		-24,	12 },		// rightdown
		{		0,	-24*2,		0,		24 },		// right
		{	-24*2,	-12*2,		24,		12 },		// rightup
		{	-24*2,		0,		24,		0 },		// up
		{	-24*2,	12*2,		24,		-12 }		// leftup
	};	

	//---------------------------------------------
	// 시작 값
	//---------------------------------------------
	int sx = tx + dirValue[lookDirection][0];
	int sy = ty + dirValue[lookDirection][1];
	int cx = dirValue[lookDirection][2];
	int cy = dirValue[lookDirection][3];
	int z  = egInfo.z0;
	*/

	const POINT dirValue[8][5] =
	{
		{ { 0, -2 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },	// left
		{ { -1, -1 }, { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } },		// leftdown
		{ { -2, 0 }, { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },			// down
		{ { 1, -1 }, { 1, 0 }, { 0, 0 }, { 0, 1 }, { -1, 1 } },			// rightdown
		{ { 0, -2 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },			// right
		{ { 1, 1 }, { 1, 0 }, { 0, 0 }, { 0, -1 }, { -1, -1 } },		// rightup
		{ { -2, 0 }, { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },	// up
		{ { 1, -1 }, { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } },	// leftup
	};

	TYPE_SECTORPOSITION	tX, tY;
	tX = g_pTopView->PixelToMapX(egInfo.x0);
	tY = g_pTopView->PixelToMapY(egInfo.y0);

	int sX, sY;
	int sx, sy;
	int z  = egInfo.z0;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);	

	MEffect*	pEffect;
	
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	for (int i=0; i<5; i++)
	{		
		sX = tX + dirValue[lookDirection][i].x;
		sY = tY + dirValue[lookDirection][i].y;
		sx = tx + dirValue[lookDirection][i].x * TILE_X;
		sy = ty + dirValue[lookDirection][i].y * TILE_Y;

		pEffect = new MEffect(bltType);
		
		pEffect->SetFrameID( frameID, maxFrame );	

		//pEffect->SetPixelPosition(sx, sy, z);		// Sector 좌표		
		pEffect->SetPosition( sX, sY );
		pEffect->SetZ( z );			
		pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
		pEffect->SetCount( egInfo.count , egInfo.linkCount );			// 지속되는 Frame

		// 방향 설정
		pEffect->SetDirection( egInfo.direction );

		// 위력
		pEffect->SetPower(egInfo.power);

		// Zone에 추가한다.
		bool bAdd = g_pZone->AddEffect( pEffect );

		if (bAdd)
		{
			// 처음으로 추가된 effect에 대해서 link설정
			if (!bOK)
			{
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

				bOK = true;
			}
		
			// 처음으로 추가된거는 아니지만 제대로 들어간 경우
			else
			{
				// 다음 Effect 생성 정보
				if (egInfo.pEffectTarget == NULL)
				{
					pEffect->SetLink( egInfo.nActionInfo, NULL );
				}
				else
				{
					MEffectTarget* pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
					pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
					pEffectTarget2->Set( sx, sy, z, egInfo.creatureID );
				}			
			}
		}

		//---------------------------------------------
		// 반복되는 frame이면..
		// 시작 frame을 다르게 한다.
		//---------------------------------------------
		if (bAdd && repeatFrame)
		{
			int num = rand() % maxFrame;
			
			for (int nf=0; nf<num; nf++)
			{
				pEffect->NextFrame();
			}
		}

		//
		//sx += cx;
		//sy += cy;

		// 다음 그림
		if (est>=EFFECTSPRITETYPE_BLOODY_WALL_1
			&& est<=EFFECTSPRITETYPE_BLOODY_WALL_3)
		{
			if (++est > EFFECTSPRITETYPE_BLOODY_WALL_3)
			{
				est = EFFECTSPRITETYPE_BLOODY_WALL_1;
			}
		}
		frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
		maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);	
	}

	return bOK;
}
