//----------------------------------------------------------------------
// MStopZoneMultipleEffectGenerator.h
//----------------------------------------------------------------------
// 십자가(+) 모양으로 생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONEMULTIPLEEFFECTGENERATOR_H__
#define	__MSTOPZONEMULTIPLEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneMultipleEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneMultipleEffectGenerator() {}
		~MStopZoneMultipleEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_MULTIPLE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MMultipleFallingEffectGenerator	g_StopZoneCrossEffectGenerator;

#endif

