//----------------------------------------------------------------------
// MAttackZoneEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttackZoneEffectGenerator.h"
#include "MLinearEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include <math.h>
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "SkillDef.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttackZoneEffectGenerator	g_AttackZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttackZoneEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;

	MLinearEffect* pEffect = new MLinearEffect(bltType);	
	
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	// 시작 좌표
	int sx = egInfo.x0;
	int sy = egInfo.y0;
	int sz = egInfo.z0;

	// 목표 좌표
	int tx = egInfo.x1; 
	int ty = egInfo.y1;
	int tz = egInfo.z1;

	if(egInfo.nActionInfo == SKILL_HALO)
	{
		if(NULL != g_pTopView)
		{
			int sTileX = g_pTopView->PixelToMapX(sx);
			int sTileY = g_pTopView->PixelToMapY(sy);
			int eTileX = g_pTopView->PixelToMapX(tx);
			int eTileY = g_pTopView->PixelToMapY(ty);

			int TempDir = g_pTopView->GetDirectionToPosition(sTileX, sTileY, eTileX, eTileY);
			switch (TempDir)
			{
				case DIRECTION_LEFTDOWN		: eTileX-=3;	eTileY+=3;	break;
				case DIRECTION_RIGHTUP		: eTileX+=3;	eTileY-=3;	break;
				case DIRECTION_LEFTUP		: eTileX-=3;	eTileY-=3;	break;
				case DIRECTION_RIGHTDOWN	: eTileX+=3;	eTileY+=3;	break;
				case DIRECTION_LEFT			: eTileX-=3;				break;
				case DIRECTION_DOWN			:			eTileY+=3;	break;
				case DIRECTION_UP			:			eTileY-=3;	break;
				case DIRECTION_RIGHT		: eTileX+=3;				break;
			}
			pEffect->SetMulti(true);

			tx = g_pTopView->MapToPixelX(eTileX);
			ty = g_pTopView->MapToPixelY(eTileY);
		}
	}
	//------------------------------------------------------------
	// 하드코딩.. ㅋㅋ
	//------------------------------------------------------------
	if (est==EFFECTSPRITETYPE_WIND_DIVIDER_1
		|| est==EFFECTSPRITETYPE_WIND_DIVIDER_2
		|| est==EFFECTSPRITETYPE_WIND_DIVIDER_3)
	{
		// 목표까지가는게 아니라.. 일정한 pixel수 만큼 가야한다?
		int movePixel = egInfo.step * egInfo.count;

		int cx = sx-tx;
		int cy = sy-ty;

		if (cx==0 || cy==0)
		{
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
			MCreature::GetPositionToDirection(x,y, egInfo.direction);

			//---------------------------------------------
			// (x,y)를 다시 pixel좌표로 바꾼다.
			//---------------------------------------------
			tx = g_pTopView->MapToPixelX( x );
			ty = g_pTopView->MapToPixelY( y );

			// 다시 계산..
			cx = sx - tx;
			cy = sy - ty;
		}

		int currentPixel = sqrt(cx*cx + cy*cy);

		//float basis = ((cx==0)? 0 : (float)cy / (float)cx);

		if (currentPixel==0)
		{			
		}
		else
		{
			tx = sx - (cx * movePixel / currentPixel);
			ty = sy - (cy * movePixel / currentPixel);

			MEffectTarget* pTarget = egInfo.pEffectTarget;
			if (pTarget!=NULL)
			{
				int tx2 = sx - (cx * movePixel);
				int ty2 = sy - (cy * movePixel);

				pTarget->Set(tx2, ty2, tz, pTarget->GetID());
			}
		}
	}



	pEffect->SetFrameID( frameID, maxFrame );		// 0번 Effect, Max 3 Frame					

	// 발사 위치 Pixel좌표	
	pEffect->SetPixelPosition( sx, sy, sz );	

	// 목표 위치 Pixel좌표
	pEffect->SetTarget( tx, ty, tz, egInfo.step );	

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );					
	
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
