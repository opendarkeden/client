//----------------------------------------------------------------------
// MAttackCreatureParabolaEffectGenerator.h
//----------------------------------------------------------------------
// 포물선으로 Creature 추적
//----------------------------------------------------------------------

#ifndef	__MATTACKCREATUREPARABOLAEFFECTGENERATOR_H__
#define	__MATTACKCREATUREPARABOLAEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttackCreatureParabolaEffectGenerator : public MEffectGenerator {
	public :
		MAttackCreatureParabolaEffectGenerator()	{}
		~MAttackCreatureParabolaEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACK_CREATURE_PARABOLA; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttackCreatureParabolaEffectGenerator	g_AttackCreatureParabolaEffectGenerator;

#endif

