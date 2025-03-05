//----------------------------------------------------------------------
// MMultipleFallingEffectGenerator.h
//----------------------------------------------------------------------
// 십자가(+) 모양으로 생성
//----------------------------------------------------------------------

#ifndef	__MMULTIPLEFALLINGEFFECTGENERATOR_H__
#define	__MMULTIPLEFALLINGEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MMultipleFallingEffectGenerator : public MEffectGenerator {
	public :
		MMultipleFallingEffectGenerator() {}
		~MMultipleFallingEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_MULTIPLE_FALLING; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MMultipleFallingEffectGenerator	g_StopZoneCrossEffectGenerator;

#endif

