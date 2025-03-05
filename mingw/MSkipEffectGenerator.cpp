 //----------------------------------------------------------------------
// MSkipEffectGenerator.cpp
//----------------------------------------------------------------------
// Tile과 맞붙은 깜빡이는 Effect들을 생성한다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MSkipEffectGenerator.h"
#include "MSkipEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
//#define	new			DEBUG_NEW

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachZoneEffectGenerator	g_StopZoneEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MSkipEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	POINT pixelPoint = { egInfo.x0, egInfo.y0 };
	
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
		pEffect = new MSkipEffect(bltType);
		
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
			pEffect = new MSkipEffect(bltType);
			
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
