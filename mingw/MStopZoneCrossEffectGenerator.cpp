//----------------------------------------------------------------------
// MStopZoneCrossEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopZoneCrossEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "SkillDef.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MStopZoneCrossEffectGenerator	g_StopZoneCrossEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MStopZoneCrossEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	bool bOK, bAdd;

	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;
	BYTE			power = egInfo.power;

	if(egInfo.nActionInfo == SAND_CROSS)
		power = 3;

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
	pEffect = new MEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPosition(sX, sY);		// Sector 좌표		
	pEffect->SetZ(egInfo.z0);			
	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( egInfo.direction );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Zone에 추가한다.
	bAdd = g_pZone->AddEffect( pEffect );

	if (bAdd)
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );	

		bOK = true;
	}	

	int sX1 = sX-power;
	int sY1 = sY-power;
	int sX2 = sX+power;
	int sY2 = sY+power;

	//------------------------------------------------------
	// Zone의 영역이 아닌 경우에 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{					
		sX1 = 0;	
	}

	if (sX2 >= g_pZone->GetWidth())
	{
		sX2 = g_pZone->GetWidth()-1;
	}

	if (sY1 < 0)
	{
		sY1 = 0;	
	}

	if (sY2 >= g_pZone->GetHeight())
	{
		sY2 = g_pZone->GetHeight()-1;
	}


	// Tile마다 하나씩 생성
	MEffectTarget*	pEffectTarget2;
	
	int x, y;
	for (y=sY1; y<=sY2; y++)
	{
		for (x=sX1; x<=sX2; x++)
		{
			if (x==sX && y==sY || !(x == sX || y == sY))
				continue;


			pEffect = new MEffect(bltType);
	
		

			pEffect->SetFrameID( frameID, maxFrame );	

			pEffect->SetPosition(x, y);		// Sector 좌표	
			pEffect->SetZ(egInfo.z0);			
			pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
			pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

			// 방향 설정
			pEffect->SetDirection( egInfo.direction );

			// 위력
			pEffect->SetPower(power);

			// 빛의 밝기
			//pEffect->SetLight( light );

			// Zone에 추가한다.
			bAdd = g_pZone->AddEffect( pEffect );

			if (bAdd)
			{
				// parameter로 받은 effectTarget을 설정해야 하는 경우
				if (!bOK)
				{
					pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

					bOK = true;
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
						pEffectTarget2->Set( egInfo.x1+TILE_X*(x-sX1-1), 
												egInfo.y1+TILE_Y*(y-sY1-1), 
												egInfo.z0, 
												egInfo.creatureID );
					}
				}
			}

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

			//bOK = bOK || bAdd;
		}
	}

//	// Test 4 
//	MEffectTarget*	pEffectTarget2;
//	//---------------------------------------------
//	// Effect 생성
//	//---------------------------------------------
//	pEffect = new MEffect(bltType);
//	
//	
//	pEffect->SetFrameID( frameID, maxFrame );	
//
//	pEffect->SetPosition(sX-1, sY);		// Sector 좌표	
//	pEffect->SetZ(egInfo.z0);			
//	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
//	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
//
//	// 방향 설정
//	pEffect->SetDirection( egInfo.direction );
//
//	// 위력
//	pEffect->SetPower(egInfo.power);
//
//	// 빛의 밝기
//	//pEffect->SetLight( light );
//
//	// Zone에 추가한다.
//	if (g_pZone->AddEffect( pEffect ))
//	{
//		// 다음 Effect 생성 정보
//		if (egInfo.pEffectTarget == NULL)
//		{
//			pEffect->SetLink( egInfo.nActionInfo, NULL );
//		}
//		else
//		{
//			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
//			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
//			pEffectTarget2->Set( egInfo.x0-TILE_X, egInfo.y0, egInfo.z0, egInfo.creatureID );
//		}
//	}
//
//
//	//---------------------------------------------
//	// Effect 생성
//	//---------------------------------------------
//	pEffect = new MEffect(bltType);
//
//
//	pEffect->SetFrameID( frameID, maxFrame );	
//
//	pEffect->SetPosition(sX+1, sY);		// Sector 좌표						
//	pEffect->SetZ(egInfo.z0);			
//	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
//	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
//
//	// 방향 설정
//	pEffect->SetDirection( egInfo.direction );
//
//	// 위력
//	pEffect->SetPower(egInfo.power);
//
//	// 빛의 밝기
//	//pEffect->SetLight( light );
//
//	// Zone에 추가한다.
//	if (g_pZone->AddEffect( pEffect ))
//	{
//		// 다음 Effect 생성 정보
//		if (egInfo.pEffectTarget == NULL)
//		{
//			pEffect->SetLink( egInfo.nActionInfo, NULL );
//		}
//		else
//		{
//			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
//			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
//			pEffectTarget2->Set( egInfo.x0+TILE_X, egInfo.y0, egInfo.z0, egInfo.creatureID );
//		}
//	}
//
//	//---------------------------------------------
//	// Effect 생성
//	//---------------------------------------------
//	pEffect = new MEffect(bltType);
//
//
//	pEffect->SetFrameID( frameID, maxFrame );	
//
//	pEffect->SetPosition(sX, sY-1);		// Sector 좌표						
//	pEffect->SetZ(egInfo.z0);			
//	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
//	pEffect->SetCount( egInfo.count , egInfo.linkCount );			// 지속되는 Frame
//
//	// 방향 설정
//	pEffect->SetDirection( egInfo.direction );
//
//	// 위력
//	pEffect->SetPower(egInfo.power);
//
//	// 빛의 밝기
//	//pEffect->SetLight( light );
//
//	// Zone에 추가한다.
//	if (g_pZone->AddEffect( pEffect ))
//	{
//		// 다음 Effect 생성 정보
//		if (egInfo.pEffectTarget == NULL)
//		{
//			pEffect->SetLink( egInfo.nActionInfo, NULL );
//		}
//		else
//		{
//			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
//			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
//			pEffectTarget2->Set( egInfo.x0, egInfo.y0-TILE_Y, egInfo.z0, egInfo.creatureID );
//		}
//	}
//
//	//---------------------------------------------
//	// Effect 생성
//	//---------------------------------------------
//	pEffect = new MEffect(bltType);
//
//	pEffect->SetFrameID( frameID, maxFrame );	
//
//	pEffect->SetPosition(sX, sY+1);		// Sector 좌표						
//	pEffect->SetZ(egInfo.z0);			
//	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.	
//	pEffect->SetCount( egInfo.count , egInfo.linkCount );			// 지속되는 Frame
//
//	// 방향 설정
//	pEffect->SetDirection( egInfo.direction );
//
//	// 위력
//	pEffect->SetPower(egInfo.power);
//
//	// 빛의 밝기
//	//pEffect->SetLight( light );
//
//	// Zone에 추가한다.
//	if (g_pZone->AddEffect( pEffect ))
//	{		
//		// 다음 Effect 생성 정보
//		if (egInfo.pEffectTarget == NULL)
//		{
//			pEffect->SetLink( egInfo.nActionInfo, NULL );
//		}
//		else
//		{
//			pEffectTarget2 = new MEffectTarget(*egInfo.pEffectTarget);
//			pEffect->SetLink( egInfo.nActionInfo, pEffectTarget2 );
//			pEffectTarget2->Set( egInfo.x0, egInfo.y0+TILE_Y, egInfo.z0, egInfo.creatureID );
//		}
//	}
	//*/


	return bOK;
}
