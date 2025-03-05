//----------------------------------------------------------------------
// MAttackCreatureHomingEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttackCreatureHomingEffectGenerator.h"
#include "MHomingEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "MathTable.h"
#include "DebugInfo.h"
#include "SkillDef.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttackCreatureHomingEffectGenerator	g_AttackCreatureHomingEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttackCreatureHomingEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;

	// creature의 좌표
	int cx, cy, cz;	

	// 목표 위치 Pixel좌표
	MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

	if (pCreature == NULL)
	{
		return false;
	}
	int currentAngle, stepAngle;

	const int angles[] =
	{
		180,		// left
		225,
		270,
		315,
		0,
		45,
		90,
		135
	};

	// 시작 각도
	currentAngle = angles[egInfo.direction];

	// 꺽는 각도
	stepAngle = 13;

	int fixZPos = TILE_Y;
	int TempEndFrame = egInfo.count;
	int TempDir = egInfo.direction;
	if(egInfo.nActionInfo == SKILL_CLIENT_HALO_ATTACK)
	{
	//	fixZPos = 0;
		stepAngle = 30;
		if(egInfo.direction == 2 || egInfo.direction == 6 )
		{
			stepAngle = 45;
			TempEndFrame = 8;
		}
		if(rand()%2)
			TempDir = (TempDir+2)%8;
		else
			TempDir = (TempDir+6)%8;
		currentAngle = angles[TempDir];
	}

	// 포물선 Effect생성

	MHomingEffect* pEffect = new MHomingEffect(bltType, currentAngle, stepAngle);	
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	pEffect->SetFrameID( frameID, maxFrame );


	
	cx = g_pTopView->MapToPixelX( pCreature->GetX() );
	cy = g_pTopView->MapToPixelY( pCreature->GetY() );
	cz = pCreature->GetZ() + fixZPos;	// 한 타일정도 위로 빼준다.


	// 발사 위치 Pixel좌표	- 한 타일정도 위로 빼준다.
	pEffect->SetPixelPosition( egInfo.x0, egInfo.y0, egInfo.z0+fixZPos );	
					
	// 방향 설정
	pEffect->SetDirection( egInfo.direction );
	pEffect->SetTraceCreatureID( egInfo.creatureID );	

	// 목표 위치 Pixel좌표
	pEffect->SetTarget( cx,cy,cz, egInfo.step );

	// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
	pEffect->SetCount( TempEndFrame, egInfo.linkCount );

	// 위력
	pEffect->SetPower(egInfo.power);
	
	pEffect->SetMulti(true);
	// 빛의 밝기
	//pEffect->SetLight( light );

	if (g_pZone->AddEffect( pEffect ))
	{
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

		return true;
	}

	return false;

}
