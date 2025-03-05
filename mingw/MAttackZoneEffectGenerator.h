//----------------------------------------------------------------------
// MAttackZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점까지 이동하는 Effect생성
//----------------------------------------------------------------------

#ifndef	__MATTACKZONEEFFECTGENERATOR_H__
#define	__MATTACKZONEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttackZoneEffectGenerator : public MEffectGenerator {
	public :
		MAttackZoneEffectGenerator() {}
		~MAttackZoneEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACK_ZONE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttackZoneEffectGenerator	g_AttackZoneEffectGenerator;

#endif

