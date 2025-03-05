//----------------------------------------------------------------------
// MScreenEffect.h
//----------------------------------------------------------------------
//
// Effect중에서 화면좌표로 표현되는 Effect를 의미한다.
//
// SetScreenPosition()을 실행할때의 Basis에서의 좌표보정값을 저장해두고
//  --> m_PixelX, m_PixelY
//
// 출력할때 SetScreenBasis()를 설정해서..
// GetScreenX,Y를 사용하면 현재의 좌표를 얻을 수 있다.
//
//----------------------------------------------------------------------

#ifndef	__MSCREENEFFECT_H__
#define	__MSCREENEFFECT_H__

#include "MEffect.h"

class MScreenEffect : public MEffect {
	public :
		MScreenEffect(BYTE bltType);
		~MScreenEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_SCREEN; }

		//--------------------------------------------------------
		// 좌표 설정
		//--------------------------------------------------------
		// 화면 기준점
		static void		SetScreenBasis(int bx, int by);

		// 화면에서의 좌표
		void			SetScreenPosition(int x, int y);

		//--------------------------------------------------------
		// Get
		//--------------------------------------------------------
		int				GetScreenX()	{ return (int)m_PixelX + m_ScreenBasisX; }
		int				GetScreenY()	{ return (int)m_PixelY + m_ScreenBasisY; }

		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool	Update();

	public :
		// 화면좌표 기준점
		static	int		m_ScreenBasisX;
		static	int		m_ScreenBasisY;
};

#endif

