//----------------------------------------------------------------------
// MAttachZoneSelectableEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MATTACHZONESELECTABLEEFFECTGENERATOR_H__
#define	__MATTACHZONESELECTABLEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttachZoneSelectableEffectGenerator : public MEffectGenerator {
	public :
		MAttachZoneSelectableEffectGenerator()	{}
		~MAttachZoneSelectableEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACH_ZONE_SELECTABLE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttachZoneSelectableEffectGenerator	g_StopZoneEffectGenerator;

#endif

