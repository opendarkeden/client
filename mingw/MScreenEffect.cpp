//----------------------------------------------------------------------
// MScreenEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffect.h"
#include "MScreenEffect.h"
#include "MTopView.h"

//----------------------------------------------------------------------
// static
//----------------------------------------------------------------------
int		MScreenEffect::m_ScreenBasisX	= 0;
int		MScreenEffect::m_ScreenBasisY	= 0;


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

MScreenEffect::MScreenEffect(BYTE bltType)
: MEffect(bltType)
{
	//m_ObjectType	= TYPE_EFFECT;

	//m_EffectType	= EFFECT_SCREEN;

}

MScreenEffect::~MScreenEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Set Screen Basis
//----------------------------------------------------------------------
// 화면 기준점
//----------------------------------------------------------------------
void		
MScreenEffect::SetScreenBasis(int bx, int by)
{
	m_ScreenBasisX = bx;
	m_ScreenBasisY = by;
}

//----------------------------------------------------------------------
// Set Screen Position
//----------------------------------------------------------------------
// 화면에서의 좌표
//----------------------------------------------------------------------
void		
MScreenEffect::SetScreenPosition(int x, int y)
{
	// 좌표 보정값을 저장한다.
	m_PixelX = x - m_ScreenBasisX;
	m_PixelY = y - m_ScreenBasisY;
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
// m_Count가 0일때까지 -1 해주면서 Frame을 바꾼다.
//----------------------------------------------------------------------
bool
MScreenEffect::Update()
{
	if (g_CurrentFrame < m_EndFrame)
	{
		// Frame을 바꿔준다.
		NextFrame();
		
		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}
		
		return true;
	}
	
	return false;
}