//----------------------------------------------------------------------
// MStopZoneRectEffectGenerator.h
//----------------------------------------------------------------------
// 사각형으로 생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONERECTEFFECTGENERATOR_H__
#define	__MSTOPZONERECTEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneRectEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneRectEffectGenerator()	{}
		~MStopZoneRectEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_RECT; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneRectEffectGenerator	g_StopZoneRectEffectGenerator;

#endif

