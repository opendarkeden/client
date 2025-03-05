//----------------------------------------------------------------------
// MStopZoneRandomEffectGenerator.h
//----------------------------------------------------------------------
// 중심에서 사방으로 적당한(?) 위치에
// 4개의 Effect를 출력시킨다.
//----------------------------------------------------------------------

#ifndef	__MSTOPZONERANDOMEFFECTGENERATOR_H__
#define	__MSTOPZONERANDOMEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneRandomEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneRandomEffectGenerator() {}
		~MStopZoneRandomEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_RANDOM; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneRandomEffectGenerator	g_StopZoneCrossEffectGenerator;

#endif

