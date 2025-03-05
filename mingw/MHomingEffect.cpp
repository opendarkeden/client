//----------------------------------------------------------------------
// MHomingEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <math.h>
#include "MHomingEffect.h"
#include "MCreature.h"
#include "MathTable.h"
#include "MZone.h"
#include "MTopView.h"
#include "SkillDef.h"
//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------

MHomingEffect::MHomingEffect(BYTE bltType, int currentAngle, int turnAngle)
: MGuidanceEffect(bltType)
{		
	m_RadCurrent = MathTable::GetAngle360( currentAngle );		// 현재 이동하는 방향각도

	m_RadStep = MathTable::GetAngle360( turnAngle );			// 매 turn마다 달라지는 radian값
}

MHomingEffect::~MHomingEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Target
//----------------------------------------------------------------------
void		
MHomingEffect::SetTarget(int x, int y, int z, WORD speed)
{
	// 목표 설정..
	//MLinearEffect::SetTarget(x, y, z, speed);
	m_TargetX = x;
	m_TargetY = y;
	m_TargetZ = z;
	m_StepPixel = speed;

	// 임시로 - -;	
	m_StepZ = (m_TargetZ - m_PixelZ) / 16.0f;
	
	CalculateAngle();
}

//----------------------------------------------------------------------
// TraceCreature
//----------------------------------------------------------------------
bool
MHomingEffect::TraceCreature()
{
	MCreature* pCreature = g_pZone->GetCreature( m_CreatureID );

	// Creature가 사라졌을 경우..
	if (pCreature == NULL)
	{
		m_CreatureID = OBJECTID_NULL;
		m_EndFrame = 0;
		return false;
	}

	// 현재의 좌표를 읽어온다.
	m_TargetX = pCreature->GetPixelX();
	m_TargetY = pCreature->GetPixelY();
	
	return true;
}

//----------------------------------------------------------------------
// CalculateAngle
//----------------------------------------------------------------------
void
MHomingEffect::CalculateAngle()
{
	//--------------------------------------------------
	// 현재 방향에서 목표의 방향을 향한 
	// 각도 변환값(m_RadStep)을 알아낸다.
	//--------------------------------------------------
	int targetAngle = MathTable::GetAngleToTarget(m_PixelX, m_PixelY, m_TargetX, m_TargetY);

	int dir = MathTable::GetAngleDir( m_RadCurrent, targetAngle );

	m_RadStep = dir * abs(m_RadStep);		// +-speed	
}

//----------------------------------------------------------------------
// SetDirectionByAngle
//----------------------------------------------------------------------
void
MHomingEffect::SetDirectionByAngle()
{
	if (m_RadCurrent < MathTable::ANGLE_180)
	{
		if (m_RadCurrent < MathTable::ANGLE_90)
		{
			if (m_RadCurrent < MathTable::ANGLE_30)
			{
				m_Direction = DIRECTION_RIGHT;
			}
			else if (m_RadCurrent < MathTable::ANGLE_60)
			{
				m_Direction = DIRECTION_RIGHTUP;
			}
			else
			{
				m_Direction = DIRECTION_UP;
			}
		}
		else
		{
			if (m_RadCurrent < MathTable::ANGLE_120)
			{
				m_Direction = DIRECTION_UP;
			}
			else if (m_RadCurrent < MathTable::ANGLE_150)
			{
				m_Direction = DIRECTION_LEFTUP;
			}
			else
			{
				m_Direction = DIRECTION_LEFT;
			}
		}
	}
	else
	{
		if (m_RadCurrent < MathTable::ANGLE_270)
		{
			if (m_RadCurrent < MathTable::ANGLE_210)
			{
				m_Direction = DIRECTION_LEFT;
			}
			else if (m_RadCurrent < MathTable::ANGLE_240)
			{
				m_Direction = DIRECTION_LEFTDOWN;
			}
			else
			{
				m_Direction = DIRECTION_DOWN;
			}
		}
		else
		{
			if (m_RadCurrent < MathTable::ANGLE_300)
			{
				m_Direction = DIRECTION_DOWN;
			}
			else if (m_RadCurrent < MathTable::ANGLE_330)
			{
				m_Direction = DIRECTION_RIGHTDOWN;
			}
			else
			{
				m_Direction = DIRECTION_RIGHT;
			}
		}
	}
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
bool
MHomingEffect::Update()
{	
	if (g_CurrentFrame < m_EndFrame)
	{
		if (GetActionInfo() != SKILL_CLIENT_HALO_ATTACK )
		{ 
			if(!TraceCreature())
				return false;

			CalculateAngle();
		}

		

		//--------------------------------
		// Pixel 좌표를 바꾼다.
		//--------------------------------
		// 각각의 방향에 대해서 Step만큼 이동해준다.
		m_RadCurrent += m_RadStep;
		m_RadCurrent &= MathTable::MAX_ANGLE_1;		
		
		m_PixelX += ((MathTable::FCos(m_RadCurrent)*m_StepPixel)>>16);
		m_PixelY -= ((MathTable::FSin(m_RadCurrent)*m_StepPixel)>>16);
		m_PixelZ += m_StepZ;

		if (fabs(m_PixelZ-m_TargetZ) < m_StepZ)
		{
			m_PixelZ = m_TargetZ;
			m_StepZ = 0;
		}

		//------------------------------------------
		// 다 움직인 경우를 생각해봐야 한다.
		//------------------------------------------
		if (fabs(m_PixelX-m_TargetX)<m_StepPixel &&
			fabs(m_PixelY-m_TargetY)<m_StepPixel &&
			GetActionInfo() != SKILL_CLIENT_HALO_ATTACK 
			)		// z는 무시 - -;
		{
			m_PixelX = (float)m_TargetX;
			m_PixelY = (float)m_TargetY;
			m_PixelZ = (float)m_TargetZ;

			m_StepX = 0;
			m_StepY = 0;
			m_StepZ = 0;

			m_RadStep = 0;

			//------------------------------------------
			// 더 움직일 필요가 없는 경우이다.			
			//------------------------------------------
			m_EndFrame = 0;

			return false;
		}
		else
		{
			//--------------------------------
			// 방향을 다시 설정해준다.
			//--------------------------------
		//	SetDirectionByAngle();
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
