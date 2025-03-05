#ifndef __MATTACHZONEAROUNDEFFECTGENERATOR_H__
#define __MATTACHZONEAROUNDEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttachZoneAroundEffectGenerator : public MEffectGenerator 
{
public :
	MAttachZoneAroundEffectGenerator(){}
	~MAttachZoneAroundEffectGenerator(){}
	
	TYPE_EFFECTGENERATORID			GetID()			{ return EFFECTGENERATORID_ATTACH_ZONE_AROUND; }
	
	bool		Generate(const EFFECTGENERATOR_INFO& egInfo );
};

#endif