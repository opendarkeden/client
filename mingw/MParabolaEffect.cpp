//----------------------------------------------------------------------
// MParabolaEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <math.h>
#include "MTopView.h"
#include "MLinearEffect.h"
#include "MParabolaEffect.h"
#include "MathTable.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "PacketFunction.h"
#include "SkillDef.h"
//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------

MParabolaEffect::MParabolaEffect(BYTE bltType)
: MLinearEffect(bltType)
{
	//m_EffectType	= EFFECT_PARABOLA;

	m_RadCurrent = 0;
	m_RadStep = 0;
	m_TargetTileX = 0;
	m_TargetTileY = 0;
}

MParabolaEffect::~MParabolaEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 목표를 설정한다.
//----------------------------------------------------------------------
void		
MParabolaEffect::SetTarget(int x, int y, int z, WORD speed)
{
	// 목표 설정..
	MLinearEffect::SetTarget(x, y, z, speed);

	//--------------------------------------------------
	// Grenade는 매 순간마다 Z축의 높이가 달라진다.
	//--------------------------------------------------
	int steps = (int)m_Len / speed;	// 몇번 움직여야 목표 도달인가?

	//m_RadStep = (float)PI / steps;	// 매번 움직일때마다 달라지는 theta
	m_RadStep = MathTable::FPI / (float)steps;
	m_RadCurrent = 0;
}
void
MParabolaEffect::MakeCannonadeSmoke()
{
	MEffect*	pEffect;
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_CANNONADE_SMOKE].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_CANNONADE_SMOKE].FrameID;
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MEffect(bltType);
			
	pEffect->SetFrameID( frameID, maxFrame );	

	pEffect->SetPixelPosition( m_PixelX, m_PixelY, m_PixelZ);
	pEffect->SetZ(m_PixelZ);			
//	pEffect->SetStepPixel(egInfo.step);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
	pEffect->SetCount( 9 );			// 지속되는 Frame
			
	// 방향 설정
	pEffect->SetDirection( GetDirection());
	pEffect->SetMulti(true);
	g_pZone->AddEffect( pEffect,10);
	return;
}

//----------------------------------------------------------------------
// Move
//----------------------------------------------------------------------
// 매 순간마다 StepX~Z가 달라진다.
//----------------------------------------------------------------------
bool
MParabolaEffect::Update()
{	
	if (g_CurrentFrame < m_EndFrame)
	{
		//--------------------------------
		// Pixel 좌표를 바꾼다.
		//--------------------------------
		// 각각의 방향에 대해서 Step만큼 이동해준다.
		m_PixelX += m_StepX;
		m_PixelY += m_StepY;
		m_PixelZ += m_StepZ;

		m_RadCurrent += m_RadStep;			// 현재 Radian값 변화

		m_PixelZ += ((MathTable::FCos(m_RadCurrent)*m_StepPixel)>>16);	// Z좌표 변화

		
		if(GetActionInfo() == SKILL_CANNONADE)
			MakeCannonadeSmoke();
		//------------------------------------------
		// 다 움직인 경우를 생각해봐야 한다.
		//------------------------------------------
		if (fabs(m_PixelX-m_TargetX)<m_StepPixel &&
			fabs(m_PixelY-m_TargetY)<m_StepPixel &&
			//fabs(m_PixelZ-m_TargetZ)<m_StepPixel &&
			m_RadCurrent >= MathTable::FPI			
			|| m_PixelZ < m_TargetZ	// 바닥에 떨어진 경우.
			
			)
		{
			m_PixelX = (float)m_TargetX;
			m_PixelY = (float)m_TargetY;
			m_PixelZ = (float)m_TargetZ;

			m_StepX = 0;
			m_StepY = 0;
			m_StepZ = 0;

			//------------------------------------------
			// 더 움직일 필요가 없는 경우이다.			
			//------------------------------------------
			m_EndFrame = 0;

		//	if(GetFrameID() == EFFECTSPRITETYPE_CANNONADE_BALL)
			if(GetActionInfo() == SKILL_CANNONADE)
			{
				ExecuteActionInfoFromMainNode(RESULT_SKILL_GUN_SHOT_GUIDANCE_BOMB,m_TargetTileX, m_TargetTileY, 0,0,	0,	
						m_TargetTileX, m_TargetTileY, 0, 1000, NULL, false);		

			}
			return false;
		}

		//--------------------------------
		// Sector 좌표를 맞춘다.
		//--------------------------------
		AffectPosition();

		//--------------------------------
		// Frame을 바꿔준다.
		//--------------------------------
		NextFrame();

		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}

		//--------------------------------
		// Counter를 하나 줄인다.
		//--------------------------------
		//m_Count--;

		return true;
	}

	// 끝~

	return false;

}
