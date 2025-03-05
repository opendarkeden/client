//----------------------------------------------------------------------
// MParabolaEffect.h
//----------------------------------------------------------------------
// 포물선을 그리는 Effect
//----------------------------------------------------------------------

#ifndef	__MPARABOLAEFFECT_H__
#define	__MPARABOLAEFFECT_H__


#include "MLinearEffect.h"

#define	PI	3.14159265


class MParabolaEffect : public MLinearEffect {
	public :
		MParabolaEffect(BYTE bltType);
		~MParabolaEffect(); 

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_PARABOLA; }

		//--------------------------------------------------------
		// 새로운 목표 설정
		//--------------------------------------------------------
		virtual void		SetTarget(int x, int y, int z, WORD stepPixel);
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool		Update();

		void				MakeCannonadeSmoke();
		void				SetTargetTile(int x, int y) {	m_TargetTileX = x; m_TargetTileY = y;	}
	protected :	
		int			m_RadCurrent;	// 현재 radian값
		int			m_RadStep;		// 매 turn마다 달라지는 radian값
		TYPE_SECTORPOSITION m_TargetTileX;
		TYPE_SECTORPOSITION m_TargetTileY;
};


#endif

