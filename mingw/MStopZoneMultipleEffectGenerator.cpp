//----------------------------------------------------------------------
// MStopZoneMultipleEffectGenerator.cpp
//----------------------------------------------------------------------
// 4개의 Effect가 공중에서 떨어진다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopZoneMultipleEffectGenerator.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "SkillDef.h"
#include "MEventManager.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MMultipleFallingEffectGenerator	g_StopZoneCrossEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MStopZoneMultipleEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	bool bOK = false;

	MEffect*	pEffect;
	int x, y;


	MEffectTarget*	pEffectTarget2;
	
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	int numEffectPhase = 4;

	int randY = 50;
	int randX = TILE_X;
	const int numEffect = 4;

	if(egInfo.nActionInfo == SKILL_ACID_STORM_WIDE || egInfo.nActionInfo == SKILL_POISON_STORM_WIDE)
	{
		randY = 100;
		randX = TILE_X*2;
		numEffectPhase = 6;
	}

	MEvent event;
	event.eventID = EVENTID_METEOR_SHAKE;
	event.eventType = EVENTTYPE_ZONE;
	event.eventDelay = 2300;
	event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
	event.parameter3 = 1;
	g_pEventManager->AddEvent(event);

	int ex[numEffect];
	int ey[numEffect];

	const int phaseUpper = 150;	// 한 단계에서 위로 더 올라가는 좌표 보정

	int effectCount = 0;

	// numEffectPhase * numEffect 개의 effect를 생성한다.
	for (int i=0; i<numEffectPhase; i++)
	{
		int n = 0;
	
		ex[n] = egInfo.x0 - rand()%randX - g_TILE_X_HALF;
		ey[n] = egInfo.y0 - rand()%randY;

		n++;
		ex[n] = egInfo.x0 - rand()%randX - g_TILE_X_HALF;
		ey[n] = egInfo.y0 + rand()%randY;

		n++;
		ex[n] = egInfo.x0 + rand()%randX + g_TILE_X_HALF;
		ey[n] = egInfo.y0 - rand()%randY;

		n++;
		ex[n] = egInfo.x0 + rand()%randX + g_TILE_X_HALF;
		ey[n] = egInfo.y0 + rand()%randY;

		for (int j=0; j<numEffect; j++)
		{
			x = ex[j];
			y = ey[j];

			pEffect = new MEffect(bltType);
			pEffect->SetDelayFrame(effectCount);
			
			pEffect->SetFrameID( frameID, maxFrame );	

			// 발사 위치 Pixel좌표	
			pEffect->SetPixelPosition( x, y, egInfo.z0 );	

			// 방향 설정
			pEffect->SetDirection( egInfo.direction );
							
//			// 목표 위치 Pixel좌표
//			pEffect->SetTarget( x, y, zt, egInfo.step );

			// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
			pEffect->SetCount( egInfo.count+effectCount, egInfo.linkCount );
			pEffect->SetMulti( true );
			
			effectCount += 2;

			// 위력
			pEffect->SetPower(egInfo.power);

			// Zone에 추가한다.
			if (g_pZone->AddEffect( pEffect ))
			{
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
						pEffectTarget2->Set( x, y, egInfo.z0, egInfo.creatureID );
					}
				}
			}
		}
	}



	return bOK;
}
