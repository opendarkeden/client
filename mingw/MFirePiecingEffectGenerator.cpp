//----------------------------------------------------------------------
// MFirePiecingEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MFirePiecingEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "MEventManager.h"
#include "SkillDef.h"

//#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MStopZoneEffectGenerator	g_StopZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MFirePiecingEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est[3] = { egInfo.effectSpriteType };
	int effectCount = 1;

	int x = egInfo.x0-TILE_X/2, y = egInfo.y0+TILE_Y/2;

	if( egInfo.pPreviousEffect != NULL )
	{
		est[0] = egInfo.pPreviousEffect->GetEst();
		
		if( est[0] > EFFECTSPRITETYPE_FIRE_16 )
			est[0] -= (rand()%2)*16;
		else
			est[0] += (rand()%2)*16;

		switch( egInfo.pPreviousEffect->GetEst() )
		{
		case EFFECTSPRITETYPE_FIRE_1:
		case EFFECTSPRITETYPE_FIRE_17:
			x = egInfo.pPreviousEffect->GetPixelX();
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y;
			break;

		case EFFECTSPRITETYPE_FIRE_2:
		case EFFECTSPRITETYPE_FIRE_18:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X/2;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y;
			break;

		case EFFECTSPRITETYPE_FIRE_3:
		case EFFECTSPRITETYPE_FIRE_19:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y;
			break;

		case EFFECTSPRITETYPE_FIRE_4:
		case EFFECTSPRITETYPE_FIRE_20:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y/2;
			break;

		case EFFECTSPRITETYPE_FIRE_5:
		case EFFECTSPRITETYPE_FIRE_21:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY();
			break;
			
		case EFFECTSPRITETYPE_FIRE_6:
		case EFFECTSPRITETYPE_FIRE_22:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y/2;
			break;
			
		case EFFECTSPRITETYPE_FIRE_7:
		case EFFECTSPRITETYPE_FIRE_23:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_8:
		case EFFECTSPRITETYPE_FIRE_24:
			x = egInfo.pPreviousEffect->GetPixelX()-TILE_X/2;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_9:
		case EFFECTSPRITETYPE_FIRE_25:
			x = egInfo.pPreviousEffect->GetPixelX();
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_10:
		case EFFECTSPRITETYPE_FIRE_26:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X/2;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_11:
		case EFFECTSPRITETYPE_FIRE_27:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_12:
		case EFFECTSPRITETYPE_FIRE_28:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()-TILE_Y/2;
			break;
			
		case EFFECTSPRITETYPE_FIRE_13:
		case EFFECTSPRITETYPE_FIRE_29:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY();
			break;
			
		case EFFECTSPRITETYPE_FIRE_14:
		case EFFECTSPRITETYPE_FIRE_30:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y/2;
			break;
			
		case EFFECTSPRITETYPE_FIRE_15:
		case EFFECTSPRITETYPE_FIRE_31:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y;
			break;
			
		case EFFECTSPRITETYPE_FIRE_16:
		case EFFECTSPRITETYPE_FIRE_32:
			x = egInfo.pPreviousEffect->GetPixelX()+TILE_X/2;
			y = egInfo.pPreviousEffect->GetPixelY()+TILE_Y;
			break;
			
		}
	}
	else
	{
		switch( egInfo.direction )
		{
		case DIRECTION_LEFT:
			x -= TILE_X/2;
			est[0] = EFFECTSPRITETYPE_FIRE_5+(rand()%2)*16;
			break;

		case DIRECTION_LEFTDOWN:
			x -= TILE_X/2;
			y += TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_3+(rand()%2)*16;
			break;
			
		case DIRECTION_DOWN:
			y += TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_1+(rand()%2)*16;
			break;
			
		case DIRECTION_RIGHTDOWN:
			x += TILE_X/2;
			y += TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_15+(rand()%2)*16;
			break;
			
		case DIRECTION_RIGHT:
			x += TILE_X/2;
			est[0] = EFFECTSPRITETYPE_FIRE_13+(rand()%2)*16;
			break;
			
		case DIRECTION_RIGHTUP:
			x += TILE_X/2;
			y -= TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_11+(rand()%2)*16;
			break;
			
		case DIRECTION_UP:
			y -= TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_9+(rand()%2)*16;
			break;
			
		case DIRECTION_LEFTUP:
			x -= TILE_X/2;
			y -= TILE_Y/2;
			est[0] = EFFECTSPRITETYPE_FIRE_7+(rand()%2)*16;
			break;
			
		}
//		est[0] = EFFECTSPRITETYPE_FIRE_1 + ((egInfo.direction+6)%8)*2 + (rand()%2)*16;
		est[1] = EFFECTSPRITETYPE_FIRE_1+(est[0]-EFFECTSPRITETYPE_FIRE_1+15)%16+(rand()%2)*16;
		est[2] = EFFECTSPRITETYPE_FIRE_1+(est[0]-EFFECTSPRITETYPE_FIRE_1+1)%16+(rand()%2)*16;
		effectCount = 3;
	}

	if (est[0] >= g_pEffectSpriteTypeTable->GetSize())
	{
		return false;
	}

	
	int direction = egInfo.direction;


	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	TYPE_SECTORPOSITION	sX, sY;
	sX = g_pTopView->PixelToMapX(egInfo.x0);
	sY = g_pTopView->PixelToMapY(egInfo.y0);

	// Zone에 추가한다.

	bool bAdd = false;
	MEffectTarget*	pEffectTarget2;

	for(int i = 0; i < effectCount; i++)
	{
		BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est[i]].BltType;
		TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est[i]].FrameID;
		bool			repeatFrame	= (*g_pEffectSpriteTypeTable)[est[i]].RepeatFrame;
		
		int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
		
		MEffect*	pEffect;
		//---------------------------------------------
		// Effect 생성
		//---------------------------------------------
		pEffect = new MEffect(bltType);
		
		pEffect->SetEst( est[i] );
		pEffect->SetFrameID( frameID, maxFrame );	
		pEffect->SetPixelPosition( x, y, egInfo.z0 );
//		pEffect->SetZ(egInfo.z0);			
		pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
		pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
//		pEffect->SetDelayFrame( i*8 );
		
		// 방향 설정
		pEffect->SetDirection( direction );
		
		// 위력
		pEffect->SetPower(egInfo.power);
		
		// 빛의 밝기
		//pEffect->SetLight( light );
		
		// 중복 가능한가
		pEffect->SetMulti(true);
		if(g_pZone->AddEffect( pEffect ))
		{
			if (!bAdd)
			{
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
				bAdd = true;
			}
			else
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
					pEffectTarget2->Set( x, y, egInfo.z0, egInfo.creatureID );
				}
			}
		}
//		if (bAdd)
//		{
//			// 다음 Effect 생성 정보
//			pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
//		
//		}
	}
	return true;
}
