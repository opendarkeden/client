//----------------------------------------------------------------------
// MAttachCreatureOrbitEffectGenerator.h
//----------------------------------------------------------------------
// Creature추적
//----------------------------------------------------------------------

#ifndef	__MATTACHCREATUREORBITEFFECTGENERATOR_H__
#define	__MATTACHCREATUREORBITEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttachCreatureOrbitEffectGenerator : public MEffectGenerator {
	public :
		MAttachCreatureOrbitEffectGenerator()	{}
		~MAttachCreatureOrbitEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACH_CREATURE_ORBIT; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttachCreatureOrbitEffectGenerator	g_AttackCreatureEffectGenerator;

#endif

