//----------------------------------------------------------------------
// MStopZoneEmptyVerticalWallEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MSTOPZONEEMPTYVERTICALWALLEEFFECTGENERATOR_H__
#define	__MSTOPZONEEMPTYVERTICALWALLEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopZoneEmptyVerticalWallEffectGenerator : public MEffectGenerator {
	public :
		MStopZoneEmptyVerticalWallEffectGenerator() {}
		~MStopZoneEmptyVerticalWallEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_ZONE_EMPTY_VERTICAL_WALL; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

#endif

