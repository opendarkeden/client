//----------------------------------------------------------------------
// MFallingEffectGenerator.h
//----------------------------------------------------------------------
// 공중으로 솟아오르는 Effect
//----------------------------------------------------------------------

#ifndef	__MFALLINGEFFECTGENERATOR_H__
#define	__MFALLINGEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MFallingEffectGenerator : public MEffectGenerator {
	public :
		MFallingEffectGenerator()	{}
		~MFallingEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_FALLING; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MFallingEffectGenerator	g_FallingEffectGenerator;

#endif

