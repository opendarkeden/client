//----------------------------------------------------------------------
// MMovingEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffect.h"
#include "MMovingEffect.h"
#include "MTopView.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

MMovingEffect::MMovingEffect(BYTE bltType)
: MEffect(bltType)
{
	//m_ObjectType	= TYPE_EFFECT;

	//m_EffectType	= EFFECT_MOVING;

}

MMovingEffect::~MMovingEffect()
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
// m_Count가 0일때까지 -1 해주면서 Frame을 바꾼다.
//----------------------------------------------------------------------
bool
MMovingEffect::Update()
{
	if (g_CurrentFrame < m_EndFrame)
	{
		// Frame을 바꿔준다.
		NextFrame();
		
		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}

		// Sector 좌표 설정
		AffectPosition();
		
		return true;
	}
	
	return false;
}