//----------------------------------------------------------------------
// MAroundZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MAROUNDZONEEFFECTGENERATOR_H__
#define	__MAROUNDZONEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAroundZoneEffectGenerator : public MEffectGenerator {
	public :
		MAroundZoneEffectGenerator()	{}
		~MAroundZoneEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_AROUND_ZONE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAroundZoneEffectGenerator	g_StopZoneEffectGenerator;

#endif

