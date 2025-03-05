//----------------------------------------------------------------------
// MStopZoneEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopZoneEffectGenerator.h"
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
MStopZoneEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;

	TYPE_FRAMEID newFrameID = FRAMEID_NULL;

	//-----------------------------------------------------------
	// 다크니스의 경우 다양하게 찍어주기...
	// 임시 땜빵 코드.. 케케~
	//-----------------------------------------------------------
	// frameID가 아니고 effectSpriteType으로 해야되는데
	// 일단은 alphaSprite라서 수치가 맞아떨어진다 - -;;
	if (est>=EFFECTSPRITETYPE_DARKNESS_1_1
		&& est<=EFFECTSPRITETYPE_DARKNESS_3_5 ||
		est>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1 &&
		est<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5)
	{
		if (egInfo.pPreviousEffect!=NULL)
		{
			newFrameID = 5 + (egInfo.pPreviousEffect)->GetFrameID();
		}
		else
		{
			if (est>=EFFECTSPRITETYPE_DARKNESS_2_1
				&& est<=EFFECTSPRITETYPE_DARKNESS_2_5)
			{
				est = EFFECTSPRITETYPE_DARKNESS_2_1 + rand()%5;
			}
			else if (est>=EFFECTSPRITETYPE_DARKNESS_1_1
				&& est<=EFFECTSPRITETYPE_DARKNESS_1_5)
			{
				est = EFFECTSPRITETYPE_DARKNESS_1_1 + rand()%5;
			}
			else if (est>=EFFECTSPRITETYPE_DARKNESS_3_1
				&& est<=EFFECTSPRITETYPE_DARKNESS_3_5)
			{
				est = EFFECTSPRITETYPE_DARKNESS_3_1 + rand()%5;
			}
			else if (est>=EFFECTSPRITETYPE_GRAY_DARKNESS_1_1 &&
				est<=EFFECTSPRITETYPE_GRAY_DARKNESS_1_5 )
			{
				est = EFFECTSPRITETYPE_GRAY_DARKNESS_1_1 + rand()%5;
			}
			else if (est>=EFFECTSPRITETYPE_GRAY_DARKNESS_2_1 &&
				est<=EFFECTSPRITETYPE_GRAY_DARKNESS_2_5 )
			{
				est = EFFECTSPRITETYPE_GRAY_DARKNESS_2_1 + rand()%5;
			}
			else if (est>=EFFECTSPRITETYPE_GRAY_DARKNESS_3_1 &&
				est<=EFFECTSPRITETYPE_GRAY_DARKNESS_3_5 )
			{
				est = EFFECTSPRITETYPE_DARKNESS_3_1 + rand()%5;
			}
		}
	}
	else if(est == EFFECTSPRITETYPE_INFINITY_THUNDERBOLT_CENTER)
	{
		est = EFFECTSPRITETYPE_INFINITY_THUNDERBOLT_CENTER + rand()%3;
	}

	if (est >= g_pEffectSpriteTypeTable->GetSize())
	{
		return false;
	}

	
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	bool			repeatFrame	= (*g_pEffectSpriteTypeTable)[est].RepeatFrame;

	if (newFrameID != FRAMEID_NULL)
	{
		frameID = newFrameID;
	}

	int direction = egInfo.direction;


	//-----------------------------------------------------------
	// Sword Wave를 위한 임시(-_-;) 코드..
	//-----------------------------------------------------------
	if (est==EFFECTSPRITETYPE_SWORD_WAVE_1)
	{
		direction = (direction + 1) % 8;	// +1
	}
	else if (est==EFFECTSPRITETYPE_SWORD_WAVE_2)
	{
		direction = (direction + 6) % 8;	// + 8 - 2
	}
	else if (est==EFFECTSPRITETYPE_SWORD_WAVE_3)
	{
		direction = (direction + 1) % 8;
	}

	
	//-----------------------------------------------------------
	// 메테오를 위한 임시(-_-;) 코드..
	//-----------------------------------------------------------
	if(est == EFFECTSPRITETYPE_METEOR_ROCK)
	{
		MEvent event;
		event.eventID = EVENTID_METEOR_SHAKE;
		event.eventType = EVENTTYPE_ZONE;
		event.eventDelay = 63;
		event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
		event.parameter3 = 3;
		g_pEventManager->AddEvent(event);
	}

	//---------------------------------------------
	// pixel좌표를 Map의 좌표로 바꿔준다.
	//---------------------------------------------
	TYPE_SECTORPOSITION	sX, sY;
	sX = g_pTopView->PixelToMapX(egInfo.x0);
	sY = g_pTopView->PixelToMapY(egInfo.y0);

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	// Zone에 추가한다.
	
	if( egInfo.nActionInfo == RESULT_SKILL_STONE_AUGER || 
		egInfo.nActionInfo == RESULT_STEP_SKILL_STONE_AUGER_2 ||
		egInfo.nActionInfo == RESULT_STEP_SKILL_STONE_AUGER_3 )
	{
		// 십자 모양으로.
		POINT pt[] = 
		{
			{ 0, 1 },
			{ 1, 0 },
			{ 0, 0 },
			{ 0, -1 },
			{ -1, 0 }
		};

		sX = g_pTopView->PixelToMapX(egInfo.x1);
		sY = g_pTopView->PixelToMapY(egInfo.y1);

		for(int i=0;i<sizeof(pt)/sizeof(POINT);i++)
		{
			MEffect* pEffect;

			pEffect = new MEffect( bltType );

			pEffect->SetFrameID( frameID, maxFrame );

			pEffect->SetPosition(sX+ pt[i].x, sY+pt[i].y );
			pEffect->SetZ( egInfo.z0 );

			pEffect->SetStepPixel( egInfo.step );
			pEffect->SetCount( egInfo.count, egInfo.linkCount );
			pEffect->SetDirection( direction );
			pEffect->SetPower( egInfo.power);
			pEffect->SetMulti( true );

			if( g_pZone->AddEffect( pEffect ) )
			{
				if( egInfo.pEffectTarget != NULL )
				{
					if( i == 0 )
						pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget);
					else
					{
						MEffectTarget *pTarget = new MEffectTarget(*egInfo.pEffectTarget);
						pEffect->SetLink( egInfo.nActionInfo, pTarget );
					}
				}

				if (repeatFrame)
				{
					int num = rand() % maxFrame;
					
					for (int nf=0; nf<num; nf++)
					{
						pEffect->NextFrame();
					}
				}	
			}
		}
		return true;
		
	} else
	{
			MEffect*	pEffect;
			//---------------------------------------------
			// Effect 생성
			//---------------------------------------------
			pEffect = new MEffect(bltType);
			
			pEffect->SetFrameID( frameID, maxFrame );	
			if((est == EFFECTSPRITETYPE_FIRE_CRACKER_1 ||
				est == EFFECTSPRITETYPE_FIRE_CRACKER_2 ||
				est == EFFECTSPRITETYPE_FIRE_CRACKER_3 ) ||
				(est >= EFFECTSPRITETYPE_DRAGON_FIRE_CRACKER && est <= EFFECTSPRITETYPE_FIRE_CRACKER_4) ||
				est == EFFECTSPRITETYPE_PET_SLAYER_3TH_ATTACK_1 || est == EFFECTSPRITETYPE_PET_SLAYER_3TH_ATTACK_2
				)
				pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z0 );
			else
				pEffect->SetPosition(sX, sY);		// Sector 좌표		
			pEffect->SetZ(egInfo.z0);			
			pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
			pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame
			
			// 방향 설정
			pEffect->SetDirection( direction );
			
			// 위력
			pEffect->SetPower(egInfo.power);
			
			// 빛의 밝기
			//pEffect->SetLight( light );
			
			// 중복 가능한가
			if(
				(est == EFFECTSPRITETYPE_FIRE_CRACKER_1 ||
				est == EFFECTSPRITETYPE_FIRE_CRACKER_2 ||
				est == EFFECTSPRITETYPE_FIRE_CRACKER_3 ) ||
				(est >= EFFECTSPRITETYPE_DRAGON_FIRE_CRACKER && est <= EFFECTSPRITETYPE_FIRE_CRACKER_4) ||
				est == EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW ||
				est == EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW_SHADOW ||
				est == EFFECTSPRITETYPE_GREAT_RUFFIAN_1_AXE_GROUND ||
				est == EFFECTSPRITETYPE_GREAT_RUFFIAN_1_AXE_WAVE||
				est == EFFECTSPRITETYPE_NEW_PLASMA_ROCKET_LAUNCHER_BLOW 
//				|| est == EFFECTSPRITETYPE_ACID_ERUPTION_1
//				|| est == EFFECTSPRITETYPE_ACID_ERUPTION_2
//				|| est == EFFECTSPRITETYPE_ACID_ERUPTION_3
//				|| est == EFFECTSPRITETYPE_ACID_ERUPTION_4
//				|| est == EFFECTSPRITETYPE_ACID_ERUPTION_5
				)
				pEffect->SetMulti(true);
		bool bAdd = g_pZone->AddEffect( pEffect );

		if (bAdd)
		{
			// 다음 Effect 생성 정보
			pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
		
			//---------------------------------------------
			// 반복되는 frame이면..
			// 시작 frame을 다르게 한다.
			//---------------------------------------------
			if (repeatFrame)
			{
				int num = rand() % maxFrame;
				
				for (int nf=0; nf<num; nf++)
				{
					pEffect->NextFrame();
				}
			}	
		}
		return bAdd;
	}
	return false;
}
