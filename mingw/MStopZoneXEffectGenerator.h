//----------------------------------------------------------------------
// MStopZoneXEffectGenerator.h
//----------------------------------------------------------------------
// X屈栏肺 积己
//----------------------------------------------------------------------

#ifndef	__MSTOPZONEXEFFECTGENERATOR_H__
#define	__MSTOPZONEXEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneXEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneXEffectGenerator()	{}
		~MStopZoneXEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_X; }

		// Effect积己
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneXEffectGenerator	g_StopZoneXEffectGenerator;

#endif

