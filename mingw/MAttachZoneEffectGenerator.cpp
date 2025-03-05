 //----------------------------------------------------------------------
// MAttachZoneEffectGenerator.cpp
//----------------------------------------------------------------------
// Tile과 맞붙은 Effect들을 생성한다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachZoneEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "MEventManager.h"
//#define	new			DEBUG_NEW

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachZoneEffectGenerator	g_StopZoneEffectGenerator;

bool	MercyGroundEffect( const EFFECTGENERATOR_INFO& egInfo )
{	
	POINT pt ={egInfo.x0, egInfo.y0};//g_pTopView->PixelToMap( egInfo.x0, egInfo.y0 );

	int est = egInfo.effectSpriteType;		
	
	for(int j=-1;j<=1;j++)
	{
		for(int i=-1;i<=1;i++)
		{
			BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
			TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
			int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
			
			MEffect*	pEffect;
			pEffect = new MEffect(bltType);
			
			pEffect->SetFrameID( frameID, maxFrame );				
			pEffect->SetPixelPosition(
				pt.x , 
				pt.y , 
				egInfo.z0);		// pixel좌표
			
			pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
			pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
			pEffect->SetDirection( egInfo.direction );			
			pEffect->SetPower(egInfo.power);			
			if (g_pZone->AddGroundEffect( pEffect ) )
			{
				if( egInfo.effectSpriteType == est )
					pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );				
				else
				{
					MEffectTarget *pTarget = egInfo.pEffectTarget;
					
					if( pTarget != NULL )
					{
						MEffectTarget *pEffectTarget = new MEffectTarget( *pTarget );
						pEffect->SetLink( egInfo.nActionInfo, pEffectTarget );						
					}
				}
			}		
			est++;
		}
	}

	return true;
}

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttachZoneEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	if( est == EFFECTSPRITETYPE_MERCY_GROUND_1 )
	{
		return MercyGroundEffect( egInfo );
	}
	
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
	if (est==EFFECTSPRITETYPE_GREEN_BLOOD_GROUND_2_1)
	{
		est = EFFECTSPRITETYPE_GREEN_BLOOD_GROUND_2_1 + rand()%4;
		
		pixelPoint.x += (rand()%TILE_X_HALF) - (TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}
	else if (est==EFFECTSPRITETYPE_GREEN_BLOOD_GROUND_1_1)
	{
		est = EFFECTSPRITETYPE_GREEN_BLOOD_GROUND_1_1 + rand()%5;
		
		pixelPoint.x += (rand()%TILE_X_HALF) - (TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}
	if (est==EFFECTSPRITETYPE_BLOOD_GROUND_2_1)
	{
		est = EFFECTSPRITETYPE_BLOOD_GROUND_2_1 + rand()%4;
		
		pixelPoint.x += (rand()%TILE_X_HALF) - (TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}
	else if (est==EFFECTSPRITETYPE_BLOOD_GROUND_1_1)
	{
		est = EFFECTSPRITETYPE_BLOOD_GROUND_1_1 + rand()%5;
		
		pixelPoint.x += (rand()%TILE_X_HALF) - (TILE_X_HALF>>1);
		pixelPoint.y += (rand()%g_TILE_Y_HALF) - (g_TILE_Y_HALF>>1);
	}
	else if( est == EFFECTSPRITETYPE_GREAT_RUFFIAN_1_AXE_GROUND)
	{
		MEvent event;
		event.eventID = EVENTID_METEOR_SHAKE;
		event.eventType = EVENTTYPE_ZONE;
		event.eventDelay = 500;
		event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
		event.parameter3 = 3;
		g_pEventManager->AddEvent(event);
	}
	else if(est == EFFECTSPRITETYPE_QUEST_MAGIC_ZONE)
	{
		pixelPoint.x -= 26;
		pixelPoint.y -= 14;
	}
	
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	
	//---------------------------------------------
	// MaxFrame의 값을 알아온다.
	//---------------------------------------------
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	int currentPhase = egInfo.pEffectTarget != NULL ?egInfo.pEffectTarget->GetCurrentPhase() : -1;
	
	// 으흑흑..ㅡ.ㅜ
	if( egInfo.temp1 == 0 )				// UseActionGrade
	{
		MEffect*	pEffect;
		//---------------------------------------------
		// Effect 생성
		//---------------------------------------------
		pEffect = new MEffect(bltType);
		
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
	} else
	{
		// ActionGrade 를 사용한다.
		std::vector<POINT> effectlist;
		POINT pt = g_pTopView->PixelToMap( pixelPoint.x, pixelPoint.y );
		
		if( egInfo.temp1 == 1 )
		{
			for( int i=-1;i<=1;i++)
			{
				POINT point = pt;
				point.x += i;
				effectlist.push_back( point );
			}
		} 
		else if( egInfo.temp1 == 2 )
		{
			for( int i=-1;i<=1;i++)
			{
				if( i == 0)
					continue;
				POINT point = pt;
				point.x += i;				
				effectlist.push_back( point );
				point.x -= i;
				point.y += i;
				effectlist.push_back( point );
			}
			effectlist.push_back( pt );
		} 
		else if( egInfo.temp1 == 3 )
		{
			for(int i=-1;i<=1;i++)
			{
				for(int j=-1;j<=1;j++)
				{
					POINT point = pt;
					point.x += i;
					point.y += j;
					effectlist.push_back( point );
				}
			}
		}		
		for(int i=0;i<effectlist.size();i++)
		{
			MEffect*	pEffect;
			pEffect = new MEffect(bltType);
			
			pEffect->SetFrameID( frameID, maxFrame );				
			pEffect->SetPixelPosition(g_pTopView->MapToPixelX( effectlist[i].x), g_pTopView->MapToPixelY(effectlist[i].y), egInfo.z0);		// pixel좌표
			pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
			pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
			pEffect->SetDirection( egInfo.direction );			
			pEffect->SetPower(egInfo.power);			
			if (g_pZone->AddGroundEffect( pEffect ) && i == 0)
			{
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );				
			}  else
			{
				MEffectTarget *pTarget = egInfo.pEffectTarget;
				
				if( pTarget != NULL )
				{
					MEffectTarget *pEffectTarget = new MEffectTarget( *pTarget );
					pEffect->SetLink( egInfo.nActionInfo, pEffectTarget );						
				}
			}
		}
		//		egInfo.temp1= 0;
		return true;
	}
	return false;	
}
