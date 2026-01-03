//----------------------------------------------------------------------
// MStopZoneXEffectGenerator.h
//----------------------------------------------------------------------
// X형으로 생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONEXEFFECTGENERATOR_H__
#define	__MSTOPZONEXEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneXEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneXEffectGenerator()	{}
		~MStopZoneXEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_X; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneXEffectGenerator	g_StopZoneXEffectGenerator;

#endif

