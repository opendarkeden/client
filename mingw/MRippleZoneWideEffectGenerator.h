//----------------------------------------------------------------------
// MRippleZoneWideEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서 진행방향으로 한 칸 움직인 곳에서 생성
//----------------------------------------------------------------------

#ifndef	__MRIPPLEZONEWIDEEFFECTGENERATOR_H__
#define	__MRIPPLEZONEWIDEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MRippleZoneWideEffectGenerator : public MEffectGenerator {
	public :
		MRippleZoneWideEffectGenerator() {}
		~MRippleZoneWideEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_RIPPLE_ZONE_WIDE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MRippleZoneWideEffectGenerator	g_RippleZoneWideEffectGenerator;

#endif

