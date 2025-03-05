//----------------------------------------------------------------------
// MSpreadOutEffectGenerator.cpp
//----------------------------------------------------------------------
// (일단은) 8방향으로 뻗어가는 Effect
//
// step씩 count번 움직인다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MSpreadOutEffectGenerator.h"
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
//MSpreadOutEffectGenerator	g_SpreadOutEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MSpreadOutEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK = false;

	int est = egInfo.effectSpriteType;

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

	MEffectTarget* pTarget = egInfo.pEffectTarget;

	// 시작 좌표
	int sx = egInfo.x0;
	int sy = egInfo.y0;
	int sz = 0;//egInfo.z0;

	int cx, cy;
	int tx, ty, tz=sz;	// 목표 좌표
	

	MLinearEffect*	pEffect;
		
	//------------------------------------------------------------
	// 목표까지가는게 아니라.. 일정한 pixel수 만큼 가야한다?
	//------------------------------------------------------------
	// 한 단계 이동..
	for (int d=0; d<8; d++)
	{
		int movePixel = egInfo.step;// * egInfo.count;
	
		//---------------------------------------------
		// pixel좌표를 Map의 좌표로 바꿔준다.
		//---------------------------------------------
		TYPE_SECTORPOSITION	sX, sY;
		sX = g_pTopView->PixelToMapX( sx );
		sY = g_pTopView->PixelToMapY( sy );

		//---------------------------------------------
		// 나가는 방향으로 다음 좌표를 정한다.	
		//---------------------------------------------
		TYPE_SECTORPOSITION x=sX, y=sY;
		MCreature::GetPositionToDirection(x,y, d);

		//---------------------------------------------
		// (x,y)를 다시 pixel좌표로 바꾼다.
		//---------------------------------------------
		tx = g_pTopView->MapToPixelX( x );
		ty = g_pTopView->MapToPixelY( y );

		cx = sx - tx;
		cy = sy - ty;		

		int currentPixel = sqrt(cx*cx + cy*cy);

		float basis = ((cx==0)? 0 : (float)cy / (float)cx);

		if (currentPixel==0)
		{
		}
		else
		{
			movePixel = (float)movePixel * (1.0f - fabs((float)cy / (float)(2.0f*currentPixel)));
		
			/*
			if (pTarget!=NULL && pTarget->GetCurrentPhase()==1)
			{
				movePixel *= 3;
			}
			*/

			int movePixelStep = movePixel * egInfo.count;
			
			tx = sx - (cx * movePixelStep / currentPixel);
			ty = sy - (cy * movePixelStep / currentPixel);
		}

		//---------------------------------------------
		// Effect 생성
		//---------------------------------------------
		pEffect = new MLinearEffect(bltType);


		pEffect->SetFrameID( frameID, maxFrame );	

		pEffect->SetPixelPosition( sx, sy, sz );
		pEffect->SetTarget( tx, ty, tz, egInfo.step );

		pEffect->SetStepPixel( egInfo.step );		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
		pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

		// 방향 설정
		pEffect->SetDirection( d );

		// 위력
		pEffect->SetPower(egInfo.power);

		// 빛의 밝기
		//pEffect->SetLight( light );

		// Zone에 추가한다.
		if (d==0)
		{
			bOK = g_pZone->AddEffect( pEffect );

			
			if (bOK)
			{
				// 다음 Effect 생성 정보
				if (pTarget == NULL)
				{
					pEffect->SetLink( egInfo.nActionInfo, NULL );
				}
				else
				{
					// 다음 Effect 생성 정보
					pEffect->SetLink( egInfo.nActionInfo, pTarget );			
					pTarget->Set( tx, ty, tz, egInfo.creatureID );
				}
			}			
		}
		else
		{
			if (g_pZone->AddEffect( pEffect ))
			{
				if (pTarget==NULL)
				{
					pEffect->SetLink( egInfo.nActionInfo, NULL );
				}
				else
				{
					MEffectTarget* pEffectTarget2 = new MEffectTarget( *pTarget );
					pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
					pEffectTarget2->Set( tx, ty, tz, egInfo.creatureID );
				}
			}			
		}
	}


	return bOK;
}
