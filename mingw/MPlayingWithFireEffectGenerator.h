//----------------------------------------------------------------------
// MAttachZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MPLAYINGWITHFIREEFFECTGENERATOR_H__
#define	__MPLAYINGWITHFIREEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MPlayingWithFireEffectGenerator : public MEffectGenerator {
	public :
		MPlayingWithFireEffectGenerator()	{}
		~MPlayingWithFireEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_PLAYING_WITH_FIRE; }

		int m_EffectCount;
		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttachZoneEffectGenerator	g_StopZoneEffectGenerator;

#endif

