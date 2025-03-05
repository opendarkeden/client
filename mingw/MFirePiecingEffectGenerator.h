//----------------------------------------------------------------------
// MFirePiecingEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MFIREPIECINGEFFECTGENERATOR_H__
#define	__MFIREPIECINGEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MFirePiecingEffectGenerator : public MEffectGenerator {
	public :
		MFirePiecingEffectGenerator() {}
		~MFirePiecingEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_FIRE_PIECING; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopZoneEffectGenerator	g_StopZoneEffectGenerator;

#endif

