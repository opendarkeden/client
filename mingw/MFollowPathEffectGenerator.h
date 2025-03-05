//----------------------------------------------------------------------
// MFollowPathEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MFOLLOWPATHEFFECTGENERATOR_H__
#define	__MFOLLOWPATHEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MFollowPathEffectGenerator : public MEffectGenerator {
	public :
		MFollowPathEffectGenerator()	{}
		~MFollowPathEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_FOLLOW_PATH; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

#endif

