//----------------------------------------------------------------------
// MAttachCreatureOrbitEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachCreatureOrbitEffectGenerator.h"
#include "MAttachEffect.h"
#include "MCreature.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "MEffectSpriteTypeTable.h"
#include "MItem.h"
#include "DebugInfo.h"
#include "EffectSpriteTypeDef.h"
#include "MAttachOrbitEffect.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachCreatureOrbitEffectGenerator	g_AttachCreatureEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttachCreatureOrbitEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;
	int direction = egInfo.direction;


	MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

	// Creature가 사라졌을 경우..
	if (pCreature == NULL)
	{	
		pCreature = g_pZone->GetFakeCreature( egInfo.creatureID );

		if (pCreature==NULL)
		{			
			DEBUG_ADD("AttachCreatureEG - no such Creature");
			return false;		
		}
	}

	int effectPosition = rand()/(RAND_MAX/MAX_EFFECT_ORBIT_STEP);

	if( egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL_ATTACK 
		|| egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL_HEAL
		|| 
			(
			egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL
			|| egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL
			&& pCreature->GetAttachEffectSize() > 0 
			)
		)
	{
		
		MAttachOrbitEffect* pOldEffect = (MAttachOrbitEffect* )(*(pCreature->GetAttachEffectIterator()));
		if( pOldEffect != NULL )
		{
			effectPosition = pOldEffect->m_OrbitStep;
		}
		if(egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL_ATTACK 
		|| egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL_HEAL)
			pCreature->ClearAttachEffect();
	}

	// Creature에게 붙이는 Effect를 생성해서 pointer를 넘겨받는다.
	MAttachOrbitEffect* pEffect = (MAttachOrbitEffect*)pCreature->CreateAttachEffect( egInfo.effectSpriteType, 
															egInfo.count, 
															egInfo.linkCount,
															false,	// ground
															MEffect::EFFECT_ATTACH_ORBIT);

	if (pEffect == NULL)
	{
		DEBUG_ADD("can't CreateAttachEffect");
		return false;
	}
	

	pEffect->SetDirection( direction );

	pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

	// 붙어야 하는 캐릭터
	//pEffect->SetAttachCreatureID( creatureID );		

	// 위력
	pEffect->SetPower(egInfo.power);
	pEffect->m_OrbitStep = effectPosition;
	if(egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL_ATTACK 
		|| egInfo.effectSpriteType == EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL_HEAL)
	{
		pEffect->m_bRun = false;
	}


	// 빛의 밝기
	//pEffect->SetLight( light );
	
	return true;
}
