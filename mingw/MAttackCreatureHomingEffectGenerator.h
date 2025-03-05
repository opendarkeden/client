//----------------------------------------------------------------------
// MAttackCreatureHomingEffectGenerator.h
//----------------------------------------------------------------------
// 곡선유도로 Creature 추적
//----------------------------------------------------------------------

#ifndef	__MATTACKCREATUREHOMINGEFFECTGENERATOR_H__
#define	__MATTACKCREATUREHOMINGEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttackCreatureHomingEffectGenerator : public MEffectGenerator {
	public :
		MAttackCreatureHomingEffectGenerator()	{}
		~MAttackCreatureHomingEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACK_CREATURE_HOMING; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttackCreatureHomingEffectGenerator	g_AttackCreatureHomingEffectGenerator;

#endif

