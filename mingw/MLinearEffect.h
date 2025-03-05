//----------------------------------------------------------------------
// MLinearEffect.h
//----------------------------------------------------------------------

#ifndef	__MLINEAREFFECT_H__
#define	__MLINEAREFFECT_H__

#include "MMovingEffect.h"

class MLinearEffect : public MMovingEffect {
	public :
		MLinearEffect(BYTE bltType);
		~MLinearEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_LINEAR; }

		//--------------------------------------------------------
		// 새로운 목표 설정
		//--------------------------------------------------------
		virtual void		SetTarget(int x, int y, int z, WORD stepPixel);
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool		Update();
	

	protected :
		// 목표 좌표
		int			m_TargetX;
		int			m_TargetY;
		int			m_TargetZ;

		// 한 번에 이동하는 pixel량
		float		m_StepX;
		float		m_StepY;
		float		m_StepZ;		
	
		// 이동해야하는 전체 거리(pixel)
		float		m_Len;	
};

#endif

