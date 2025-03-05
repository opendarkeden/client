//----------------------------------------------------------------------
// MMovingEffect.h
//----------------------------------------------------------------------
//
// Effect중에서 움직이는 Effect를 의미한다.
//
// - 현재의 Pixel좌표를 추가로 가진다.
// - 이 Pixel좌표의 값이 변할 때, 적절한 Sector좌표값을 Setting해줘야 한다.
//
//----------------------------------------------------------------------

#ifndef	__MMOVINGEFFECT_H__
#define	__MMOVINGEFFECT_H__

#include "MEffect.h"

class MMovingEffect : public MEffect {
	public :
		MMovingEffect(BYTE bltType);
		~MMovingEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_MOVING; }

		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool	Update();
};

#endif

