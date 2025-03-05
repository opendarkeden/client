//----------------------------------------------------------------------
// MAttachCreatureEffectGroundGenerator.h
//----------------------------------------------------------------------
// Creature에 붙어버리는 Effect
//----------------------------------------------------------------------

#ifndef	__MATTACHCREATUREEFFECTGROUNDGENERATOR_H__
#define	__MATTACHCREATUREEFFECTGROUNDGENERATOR_H__

#include "MEffectGenerator.h"

class MAttachCreatureEffectGroundGenerator : public MEffectGenerator {
	public :
		MAttachCreatureEffectGroundGenerator() {}
		~MAttachCreatureEffectGroundGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACH_CREATURE_GROUND; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttachCreatureEffectGenerator	g_AttachCreatureEffectGenerator;

#endif

