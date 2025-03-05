//----------------------------------------------------------------------
// MChaseEffect.h
//----------------------------------------------------------------------
// ChaseEffect는 목표가 사라지기 전까지는 끝이 없다. - -;
//----------------------------------------------------------------------

#ifndef	__MCHASEEFFECT_H__
#define	__MCHASEEFFECT_H__

#include "MGuidanceEffect.h"
#include "MTypeDef.h"

class MChaseEffect : public MGuidanceEffect {
	public :
		MChaseEffect(BYTE bltType);
		~MChaseEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_CHASE; }		
		
		//--------------------------------------------------------
		// 추적 상황?
		//--------------------------------------------------------
		bool				IsChaseOver() const		{ return m_bChaseOver; }

		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool		Update();

	protected :
		//--------------------------------------------------------
		// 추적해서 같은 위치에 있는가?
		//--------------------------------------------------------
		bool				m_bChaseOver;
};

#endif