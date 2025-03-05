//----------------------------------------------------------------------
// MChaseEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <math.h>
#include "MChaseEffect.h"
#include "MCreature.h"
#include "MZone.h"
#include "MTopView.h"

//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------

MChaseEffect::MChaseEffect(BYTE bltType)
: MGuidanceEffect(bltType)
{
	m_bChaseOver = false;
}

MChaseEffect::~MChaseEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
// ChaseEffect는 목표가 사라지기 전까지는 끝이 없다. - -;
//----------------------------------------------------------------------
bool
MChaseEffect::Update()
{	
	//if (g_CurrentFrame < m_EndFrame)
	{
		// CreatureID가 설정되어 있으면 Creature를 추적하고
		// 아니면.. 목표 좌표까지 이동하게 한다.
		if (m_CreatureID!=OBJECTID_NULL && !TraceCreature())
			return false;

		//--------------------------------
		// Frame을 바꿔준다.
		//--------------------------------
		NextFrame();

		//--------------------------------
		// Pixel 좌표를 바꾼다.
		//--------------------------------
		// 각각의 방향에 대해서 Step만큼 이동해준다.
		m_PixelX += m_StepX;
		m_PixelY += m_StepY;
		m_PixelZ += m_StepZ;

		//------------------------------------------
		// 다 움직인 경우를 생각해봐야 한다.
		//------------------------------------------
		if (fabs(m_PixelX-m_TargetX)<m_StepPixel &&
			fabs(m_PixelY-m_TargetY)<m_StepPixel &&
			fabs(m_PixelZ-m_TargetZ)<m_StepPixel)
		{
			m_PixelX = (float)m_TargetX;
			m_PixelY = (float)m_TargetY;
			m_PixelZ = (float)m_TargetZ;

			m_StepX = 0;
			m_StepY = 0;
			m_StepZ = 0;

			m_bChaseOver = true;

			return true;
		}
		else
		{
			m_bChaseOver = false;
		}

		//--------------------------------
		// Sector 좌표를 맞춘다.
		//--------------------------------
		AffectPosition();

		
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
