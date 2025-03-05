//----------------------------------------------------------------------
// MRippleZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서 진행방향으로 한 칸 움직인 곳에서 생성
//----------------------------------------------------------------------

#ifndef	__MRIPPLEZONEEFFECTGENERATOR_H__
#define	__MRIPPLEZONEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MRippleZoneEffectGenerator : public MEffectGenerator {
	public :
		MRippleZoneEffectGenerator() {}
		~MRippleZoneEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_RIPPLE_ZONE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MRippleZoneEffectGenerator	g_RippleZoneEffectGenerator;

#endif

