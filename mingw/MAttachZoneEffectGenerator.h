//----------------------------------------------------------------------
// MAttachZoneEffectGenerator.h
//----------------------------------------------------------------------
// Zone의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MATTACHZONEEFFECTGENERATOR_H__
#define	__MATTACHZONEEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttachZoneEffectGenerator : public MEffectGenerator {
	public :
		MAttachZoneEffectGenerator()	{}
		~MAttachZoneEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_ATTACH_ZONE; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MAttachZoneEffectGenerator	g_StopZoneEffectGenerator;

#endif

