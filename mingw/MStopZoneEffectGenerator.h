//----------------------------------------------------------------------
// MStopZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONEEFFECTGENERATOR_H__
#define	__MSTOPZONEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneEffectGenerator() {}
		~MStopZoneEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneEffectGenerator	g_StopZoneEffectGenerator;

#endif

