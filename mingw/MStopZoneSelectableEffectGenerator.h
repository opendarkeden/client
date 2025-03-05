//----------------------------------------------------------------------
// MStopZoneSelectableEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONESELECTABLEEFFECTGENERATOR_H__
#define	__MSTOPZONESELECTABLEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneSelectableEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneSelectableEffectGenerator() {}
		~MStopZoneSelectableEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_SELECTABLE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneSelectableEffectGenerator	g_StopZoneEffectGenerator;

#endif

