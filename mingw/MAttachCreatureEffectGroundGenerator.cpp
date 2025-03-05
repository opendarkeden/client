//----------------------------------------------------------------------
// MAttachCreatureEffectGroundGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachCreatureEffectGroundGenerator.h"
#include "MAttachEffect.h"
#include "MCreature.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachCreatureEffectGroundGenerator	g_AttachCreatureEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttachCreatureEffectGroundGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;

	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	int direction = egInfo.direction;

	//-----------------------------------------------------------
	// Sword Wave를 위한 임시(-_-;) 코드..
	//-----------------------------------------------------------
	if (est==EFFECTSPRITETYPE_SWORD_WAVE_1)
	{
		direction = (direction + 7) % 8;	// +8 - 1
	}
	

	MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

	// Creature가 사라졌을 경우..
	if (pCreature == NULL)
	{	
		pCreature = g_pZone->GetFakeCreature( egInfo.creatureID );

		if (pCreature==NULL)
		{
			return false;
		}
	}

	// Creature에게 붙이는 Effect를 생성해서 pointer를 넘겨받는다.
	MAttachEffect* pEffect = pCreature->CreateAttachEffect( 
											egInfo.effectSpriteType, 
											egInfo.count, 
											egInfo.linkCount, 
											TRUE );	// 바닥에 붙이는 것이다.

	if (pEffect == NULL)
	{
		return false;
	}
	

	pEffect->SetDirection( direction );

	pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );

	// 붙어야 하는 캐릭터
	//pEffect->SetAttachCreatureID( creatureID );		

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );
	
	return true;
}
