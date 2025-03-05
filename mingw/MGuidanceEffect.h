//----------------------------------------------------------------------
// MGuidanceEffect.h
//----------------------------------------------------------------------

#ifndef	__MGUIDANCEEFFECT_H__
#define	__MGUIDANCEEFFECT_H__

#include "MLinearEffect.h"
#include "MTypeDef.h"

class MGuidanceEffect : public MLinearEffect {
	public :
		MGuidanceEffect(BYTE bltType);
		~MGuidanceEffect();
		
		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_GUIDANCE; }

		//--------------------------------------------------------
		// 목표 설정
		//--------------------------------------------------------
		void				SetTraceCreatureID(TYPE_OBJECTID id);
		TYPE_OBJECTID		GetTraceCreatureID()					{ return m_CreatureID; }
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool		Update();

	protected :
		virtual bool		TraceCreature();		// 추적 좌표 설정

	protected :
		TYPE_OBJECTID	m_CreatureID;
};

#endif