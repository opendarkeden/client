//----------------------------------------------------------------------
// MAttackZoneParabolaEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점까지 이동하는 Effect생성
//----------------------------------------------------------------------

#ifndef	__MATTACKZONEPARABOLAEFFECTGENERATOR_H__
#define	__MATTACKZONEPARABOLAEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttackZoneParabolaEffectGenerator : public MEffectGenerator {
	public :
		MAttackZoneParabolaEffectGenerator() {}
		~MAttackZoneParabolaEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACK_ZONE_PARABOLA; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttackZoneParabolaEffectGenerator	g_AttackZoneParabolaEffectGenerator;

#endif

