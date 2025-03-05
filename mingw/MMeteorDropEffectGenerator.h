//----------------------------------------------------------------------
// MMeteorDropEffectGenerator.h
//----------------------------------------------------------------------
// 공중으로 솟아오르는 Effect
//----------------------------------------------------------------------

#ifndef	__MMETEORDROPEFFECTGENERATOR_H__
#define	__MMETEORDROPEFFECTGENERATOR_H__

#include "MEffectGenerator.h"

class MMeteorDropEffectGenerator : public MEffectGenerator {
	public :
		MMeteorDropEffectGenerator()	{}
		~MMeteorDropEffectGenerator() {}

		TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_METEOR_DROP; }

		// Effect생성
		bool	Generate( const EFFECTGENERATOR_INFO& egInfo );
};

//
//extern MFallingEffectGenerator	g_FallingEffectGenerator;

#endif

