//----------------------------------------------------------------------
// MMovingEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffect.h"
#include "MathTable.h"
#include "MSkipEffect.h"
#include "MTopView.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

MSkipEffect::MSkipEffect(BYTE bltType)
: MEffect(bltType)
{
	m_nSkipValue = 3;
}

MSkipEffect::~MSkipEffect()
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
MSkipEffect::Update()
{
	if (g_CurrentFrame < m_EndFrame-4)
	{
		if((rand()%m_nSkipValue))
			SetDrawSkip(true);
		else
			SetDrawSkip(false);
		
		NextFrame();
	
		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}

		return true;
	}
	
	return false;
}