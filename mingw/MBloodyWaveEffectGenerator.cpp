//----------------------------------------------------------------------
// MBloodyWaveEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MBloodyWaveEffectGenerator.h"
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
//MBloodyWaveEffectGenerator	g_BloodyWaveEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MBloodyWaveEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
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

	static int lastPhase = 0;

	int currentPhase = (egInfo.pEffectTarget==NULL? lastPhase +1 : egInfo.pEffectTarget->GetCurrentPhase());
	lastPhase = currentPhase;
	
	//---------------------------------------------
	// 좌표 변경값
	//---------------------------------------------
	std::vector<POINT> v_cp;
	POINT p;

	switch (currentPhase)
	{
		// 상, 하, 좌, 우
		case 1 :
		{
			p.x = 0; p.y = -1;
			v_cp.push_back(p);
			p.x = 0; p.y = 1;
			v_cp.push_back(p);
			p.x = 1; p.y = 0;
			v_cp.push_back(p);
			p.x = -1; p.y = 0;
			v_cp.push_back(p);
		}
		break;

		// 대각선 방향 4개
		case 2 :
		{
			p.x = 1; p.y = -1;
			v_cp.push_back(p);
			p.x = 1; p.y = 1;
			v_cp.push_back(p);
			p.x = -1; p.y = -1;
			v_cp.push_back(p);
			p.x = -1; p.y = 1;
			v_cp.push_back(p);
		}
		break;

		// 상하 좌우.. 한칸씩 건너서 4개
		case 3 :
		{
			p.x = 0; p.y = -2;
			v_cp.push_back(p);
			p.x = 0; p.y = 2;
			v_cp.push_back(p);
			p.x = 2; p.y = 0;
			v_cp.push_back(p);
			p.x = -2; p.y = 0;
			v_cp.push_back(p);
		}
		break;

		// 대각선 4방향 2개씩
		case 4 :
		{
			p.x = 1; p.y = -2;
			v_cp.push_back(p);
			p.x = 2; p.y = -1;
			v_cp.push_back(p);
			p.x = 1; p.y = 2;
			v_cp.push_back(p);
			p.x = 2; p.y = 1;
			v_cp.push_back(p);
			p.x = -1; p.y = -2;
			v_cp.push_back(p);
			p.x = -2; p.y = -1;
			v_cp.push_back(p);
			p.x = -1; p.y = 2;
			v_cp.push_back(p);
			p.x = -2; p.y = 1;
			v_cp.push_back(p);
		}
		break;

		// 상하 좌우.. 두칸씩 건너서 4개
		default:
		{
			p.x = 0; p.y = -3;
			v_cp.push_back(p);
			p.x = 0; p.y = 3;
			v_cp.push_back(p);
			p.x = 3; p.y = 0;
			v_cp.push_back(p);
			p.x = -3; p.y = 0;
			v_cp.push_back(p);
		}
		break;
	}

	TYPE_SECTORPOSITION	tX, tY;
	tX = g_pTopView->PixelToMapX(egInfo.x1);
	tY = g_pTopView->PixelToMapY(egInfo.y1);

	int sX, sY;
	int z  = egInfo.z1;

	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);	

	MEffect*	pEffect;
	
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	for (int i=0; i<v_cp.size(); i++)
	{		
		sX = tX + v_cp[i].x;
		sY = tY + v_cp[i].y;
		
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
				// 1개 이외에는 다음 Effect 생성 정보는 없다..
				// 퍼지는 effect 4개마다 다 다음 effect를 생성하면 안되기 때문이다.
				pEffect->SetLink( egInfo.nActionInfo, NULL );
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
