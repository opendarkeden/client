//----------------------------------------------------------------------
// MEffectGeneratorTable.h
//----------------------------------------------------------------------
//
//  = EffectGeneratorTable은 EffectGenerator의 Array이다.
//
//  = EffectGeneratorTableID로 EffectGenerator를 구분한다.
//
//----------------------------------------------------------------------

#ifndef	__MEFFECTGENERATORTABLE_H__
#define	__MEFFECTGENERATORTABLE_H__

#include "MEffectGenerator.h"
#include "DrawTypeDef.h"
#include "MTypeDef.h"
#include "MActionInfoTable.h"
#include "MEffectTarget.h"
class MActionResult;
class MEffect;

class MEffectGeneratorTable {
	public :
		MEffectGeneratorTable();
		~MEffectGeneratorTable();

		//----------------------------------------------------
		// Init / Release
		//----------------------------------------------------
		void	Init();	
		void	Release();

		//----------------------------------------------------
		// 최초 Effect생성 
		//----------------------------------------------------
		void	Generate(	int x0, int y0, int z0,
							BYTE direction,																
							BYTE power,
							TYPE_ACTIONINFO nActionInfo,
							MEffectTarget* pEffectTarget, BYTE temp1 = 0, BYTE temp2 = 0);
		//----------------------------------------------------
		// 연결된 다음 Effect 생성
		//----------------------------------------------------
		void	GenerateNext( MEffect* pEffect );

	protected :		
		void	AddEffectGenerator(MEffectGenerator* pEffectGenerator);

	protected :
		MEffectGenerator*	m_pEffectGenerator[MAX_EFFECTGENERATORID];
		
};

extern MEffectGeneratorTable*	g_pEffectGeneratorTable;

#endif
