#ifndef __MATTACKZONERECTEFFECTGENERATOR_H__
#define __MATTACKZONERECTEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MAttackZoneRectEffectGenerator : public MEffectGenerator 
{
public :
	MAttackZoneRectEffectGenerator(){}
	~MAttackZoneRectEffectGenerator(){}
	
	TYPE_EFFECTGENERATORID			GetID()			{ return EFFECTGENERATORID_ATTACK_ZONE_RECT; }
	
	bool		Generate(const EFFECTGENERATOR_INFO& egInfo );
};

#endif