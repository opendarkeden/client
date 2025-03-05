//----------------------------------------------------------------------
// MStopZoneRhombusEffectGenerator.h
//----------------------------------------------------------------------
// 마름모 모양으로 생성 = 십자가 - 가운데 하나
//----------------------------------------------------------------------

#ifndef	__MSTOPZONERHOMBUSEFFECTGENERATOR_H__
#define	__MSTOPZONERHOMBUSEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneRhombusEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneRhombusEffectGenerator() {}
		~MStopZoneRhombusEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_RHOMBUS; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneRhombusEffectGenerator	g_StopZoneRhombusEffectGenerator;

#endif

