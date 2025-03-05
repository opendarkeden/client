//----------------------------------------------------------------------
// MRippleZonePixelEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MRippleZonePixelEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MCreature.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "UserInformation.h"

//#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MRippleZonePixelEffectGenerator	g_RippleZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MRippleZonePixelEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	//------------------------------------------------------------
	// 이번에는 시작좌표(sx,sy)에 Effect를 출력하고
	// 다음에는 (tx,ty)에 Effect를 출력하게 된다.
	// MEffectTarget을 수정하면 가능하다.
	//------------------------------------------------------------
	MEffectTarget* pTarget = egInfo.pEffectTarget;

	// 시작 좌표
	int sx = egInfo.x0;
	int sy = egInfo.y0;
	int sz = 0;//egInfo.z0;

	// 목표 좌표
	int tx = egInfo.x1; 
	int ty = egInfo.y1;
	int tz = 0;//egInfo.z1;

	int tx2 = tx;
	int ty2 = ty;

	int est = egInfo.effectSpriteType;

	//------------------------------------------------------------
	// 하드코딩.. ㅋㅋ
	//------------------------------------------------------------
	bool bPixelStepMove = false;
	bool bGroundEffect = false;
	bool bResetTarget = true;
	int startMoveStep = 0;

	//------------------------------------------------------------
	// Power of Land
	//------------------------------------------------------------
	if (est==EFFECTSPRITETYPE_POWER_OF_LAND_STONE_1)
	{
		// 1~3사이에 하나를 선택한다.
		est = EFFECTSPRITETYPE_POWER_OF_LAND_STONE_2 + rand()%3;
		
		bGroundEffect = true;
		bPixelStepMove = true;
		startMoveStep = 1;
		bResetTarget = false;
	}
	//------------------------------------------------------------
	// Earthquake
	//------------------------------------------------------------
	else if (est==EFFECTSPRITETYPE_EARTHQUAKE_1)
	{
		// 1~3사이에 하나를 선택한다.
		est = EFFECTSPRITETYPE_EARTHQUAKE_1 + rand()%3;
		
		bGroundEffect = true;
		bPixelStepMove = true;
		startMoveStep = 3;
	}
	//------------------------------------------------------------
	// Transfusion
	//------------------------------------------------------------
	else if (est==EFFECTSPRITETYPE_TRANSFUSION_1 || est==EFFECTSPRITETYPE_GREEN_TRANSFUSION_1)
	{
		MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

		int targetX, targetY;

		// 이펙트(egInfo.xy0 + size)가  
		// 캐릭터(pt + size)와 충돌하면
		// 이펙트 그만 그린다..
		POINT pt;

 		if (pCreature==NULL)
		{
			targetX = egInfo.x1;
			targetY = egInfo.y1;

//			pt.x = targetX;
//			pt.y = targetY;
		}
		else
		{
			targetX = pCreature->GetX();
			targetY = pCreature->GetY();

		}
		pt = MTopView::MapToPixel( targetX, targetY );

		RECT rectEffect = 
		{
			egInfo.x0 + (g_TILE_X_HALF>>1),
			egInfo.y0 + (g_TILE_Y_HALF>>1),
			egInfo.x0 + g_TILE_X_HALF,
			egInfo.y0 + g_TILE_Y_HALF,
		};
		
		RECT rectChar =
		{
			pt.x,
			pt.y,
			pt.x + g_TILE_X_HALF,
			pt.y + g_TILE_Y_HALF
		};

		if (rectChar.left <= rectEffect.right
			&& rectChar.right >= rectEffect.left
			&& rectChar.top <= rectEffect.bottom
			&& rectChar.bottom >= rectEffect.top)
		{
			return false;
		}

		// 1~3사이에 하나를 선택한다.
		if(g_pUserInformation->GoreLevel == false)
			est = EFFECTSPRITETYPE_GREEN_TRANSFUSION_1 + rand()%3;
		else
			est = EFFECTSPRITETYPE_TRANSFUSION_1 + rand()%3;

		bGroundEffect = true;
		bPixelStepMove = true;
		startMoveStep = 1;
	}

	if (bPixelStepMove)
	{
		// 목표까지가는게 아니라.. 일정한 pixel수 만큼 가야한다?
		// 한 단계 이동..
		int movePixel = egInfo.step;// * egInfo.count;

		int cx = sx-tx;
		int cy = sy-ty;

		if (bResetTarget && (cx==0 || cy==0))
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

		float basis = ((cx==0)? 0 : (float)cy / (float)cx);

		if (currentPixel==0)
		{			
		}
		else
		{
			movePixel = (float)movePixel * (1.0f - fabs((float)cy / (float)(2.0f*currentPixel)));
		
			if (pTarget!=NULL && pTarget->GetCurrentPhase()==1)
			{
				movePixel *= startMoveStep;
			}

			int movePixel2 = movePixel<<1;
			
			tx = sx - (cx * movePixel / currentPixel);
			ty = sy - (cy * movePixel / currentPixel);

			tx2 = sx - (cx * movePixel2 / currentPixel);
			ty2 = sy - (cy * movePixel2 / currentPixel);
		}
	}

	//-----------------------------------------------------
	// MEffectTarget에 목표 위치를 다시 설정해야한다.
	//-----------------------------------------------------	
	if (pTarget!=NULL && bResetTarget)
	{
		pTarget->Set(tx2, ty2, tz, pTarget->GetID());		
	}


	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MEffect*	pEffect;
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	//pEffect->SetPosition(x, y);		// Sector 좌표(의미 없을 듯)

	pEffect->SetPixelPosition(tx, ty, tz);		// pixel 좌표		
	
	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	//pEffect->SetZ( sz );			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	bool bAdd = false;
	// Zone에 추가한다.		
	if (bGroundEffect)
	{
		bAdd = g_pZone->AddGroundEffect( pEffect );
	}	
	else
	{
		bAdd = g_pZone->AddEffect( pEffect );
	}

	if (bAdd)
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );	
	}

	return bAdd;
}
