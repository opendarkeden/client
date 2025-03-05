//----------------------------------------------------------------------
// MRippleZonePixelEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서 진행방향으로 한 칸 움직인 곳에서 생성
//----------------------------------------------------------------------

#ifndef	__MRIPPLEZONEPIXELEFFECTGENERATOR_H__
#define	__MRIPPLEZONEPIXELEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MRippleZonePixelEffectGenerator : public MEffectGenerator {
	public :
		MRippleZonePixelEffectGenerator() {}
		~MRippleZonePixelEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_RIPPLE_ZONE_PIXEL; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MRippleZoneEffectGenerator	g_RippleZoneEffectGenerator;

#endif

