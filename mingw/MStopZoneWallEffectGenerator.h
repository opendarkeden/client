//----------------------------------------------------------------------
// MStopZoneWallEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONWALLEEFFECTGENERATOR_H__
#define	__MSTOPZONWALLEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneWallEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneWallEffectGenerator() {}
		~MStopZoneWallEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_WALL; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

#endif

