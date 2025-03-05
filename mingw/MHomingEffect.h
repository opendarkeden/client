//----------------------------------------------------------------------
// MHomingEffect.h
//----------------------------------------------------------------------
// 곡선을 그리며 목표에 다가가는 effect
//
//             90도
//              |
//              |
//  180도-------+--------0도
//              |
//              |
//             270도
//
//----------------------------------------------------------------------

#ifndef	__MHOMINGEEFFECT_H__
#define	__MHOMINGEEFFECT_H__

#include "MGuidanceEffect.h"
#include "MTypeDef.h"

class MHomingEffect : public MGuidanceEffect {
	public :
		MHomingEffect(BYTE bltType, int currentAngle, int turnAngle);
		~MHomingEffect();
		
		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_HOMING; }
		
		//--------------------------------------------------------
		// Set Target
		//--------------------------------------------------------
		virtual void		SetTarget(int x, int y, int z, WORD stepPixel);

		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool		Update();
		void	SetRadStep(int step);
		void	SetRadCurrent(int step);

	protected :
		void			CalculateAngle();
		void			SetDirectionByAngle();
		virtual bool	TraceCreature();
	
	protected :	
		int			m_RadCurrent;	// 현재 이동하는 방향각도
		int			m_RadStep;		// 매 turn마다 달라지는 radian값
};

#endif