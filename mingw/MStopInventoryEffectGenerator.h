//----------------------------------------------------------------------
// MStopInventoryEffectGenerator.h
//----------------------------------------------------------------------
// Inventory의 한 지점에서  생성
//----------------------------------------------------------------------

#ifndef	__MSTOPINVENTORYEFFECTGENERATOR_H__
#define	__MSTOPINVENTORYEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MStopInventoryEffectGenerator : public MEffectGenerator {
	public :
		MStopInventoryEffectGenerator() {}
		~MStopInventoryEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_STOP_INVENTORY; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MStopInventoryEffectGenerator	g_StopInventoryEffectGenerator;

#endif

