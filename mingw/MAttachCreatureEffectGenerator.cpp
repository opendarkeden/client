//----------------------------------------------------------------------
// MAttachCreatureEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachCreatureEffectGenerator.h"
#include "MAttachEffect.h"
#include "MCreature.h"
#include "MZone.h"
#include "MEffectSpriteTypeTable.h"
#include "MItem.h"
#include "DebugInfo.h"
#include "EffectSpriteTypeDef.h"
#include "MPlayer.h"
#include "SkillDef.h"
#include "MTopView.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MAttachCreatureEffectGenerator	g_AttachCreatureEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MAttachCreatureEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[egInfo.effectSpriteType].FrameID;
	int direction = egInfo.direction;

	//-----------------------------------------------------------
	// Sword Wave를 위한 임시(-_-;) 코드..
	//-----------------------------------------------------------
	/*
	if (frameID==EFFECTSPRITETYPE_SWORD_WAVE_1)
	{
		direction = (direction + 1) % 8;	// +1
	}
	*/
	int			plusZ=0;
	
	if( egInfo.nActionInfo == SKILL_EMISSION_WATER || egInfo.nActionInfo == STEP_SKILL_EMISSION_WATER_2 || egInfo.nActionInfo == STEP_SKILL_EMISSION_WATER_3 )
	{
		plusZ = egInfo.step;		
	}


	MCreature* pCreature = g_pZone->GetCreature( egInfo.creatureID );

	// Creature가 사라졌을 경우..
	if (pCreature == NULL)
	{	
		pCreature = g_pZone->GetFakeCreature( egInfo.creatureID );

		if (pCreature==NULL)
		{
			MItem* pItem = g_pZone->GetItem( egInfo.creatureID );

			if (pItem==NULL)
			{
				DEBUG_ADD("AttachCreatureEG - no such Creature");
				return false;
			}

			if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
			{
				pCreature = ((MCorpse*)pItem)->GetCreature();
			}
		}
	}
	TYPE_EFFECTSPRITETYPE EffectType = egInfo.effectSpriteType;
	
	switch( EffectType )
	{
	case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_3 :
		if( pCreature != NULL && ( pCreature->GetCreatureType() == 104 ||
			pCreature->GetCreatureType() >= 111 && pCreature->GetCreatureType() <= 119 ||
			pCreature->GetCreatureType() == 358 ||
			pCreature->GetCreatureType() == 359 ||
			pCreature->GetCreatureType() == 467 ||
			pCreature->GetCreatureType() == 582 ) )
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_FRONT_GOLEMER;
		else if( pCreature != NULL && (pCreature->GetCreatureType() == 106 ||
			pCreature->GetCreatureType() >= 129 && pCreature->GetCreatureType() <= 137 ) )
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_FRONT_ALCAN;
		
		break;
	case EFFECTSPRITETYPE_JABBING_VEIN_REAR_3 :
		if( pCreature != NULL && ( pCreature->GetCreatureType() == 104 ||
			pCreature->GetCreatureType() >= 111 && pCreature->GetCreatureType() <= 119 ||
			pCreature->GetCreatureType() == 358 ||
			pCreature->GetCreatureType() == 359 ||
			pCreature->GetCreatureType() == 467 ||
			pCreature->GetCreatureType() == 582 ) )
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_REAR_GOLEMER;
		else if( pCreature != NULL && (pCreature->GetCreatureType() == 106 ||
			pCreature->GetCreatureType() >= 129 && pCreature->GetCreatureType() <= 137 ) )
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_REAR_ALCAN;
		break;
	case EFFECTSPRITETYPE_TRIDENT_BOMB_1 :
		EffectType = EFFECTSPRITETYPE_TRIDENT_BOMB_1 + (rand()%10);
		break;
	case EFFECTSPRITETYPE_PIERCING_FRONT :
		// ToT 방향은..ㅡ.ㅜ 반대~
		direction = (direction+4)%8;
		break;
	case EFFECTSPRITETYPE_QUICK_FIRE_BOMB_1 :
		EffectType = EFFECTSPRITETYPE_QUICK_FIRE_BOMB_1 + (rand()%5);
		break;
	case EFFECTSPRITETYPE_STONE_SKIN_DAMAGED_1 :
		EffectType = EFFECTSPRITETYPE_STONE_SKIN_DAMAGED_1 + rand()%3;
		break;
		
	case EFFECTSPRITETYPE_DISTANCE_BLITZ_12:
		{
			int sx = g_pTopView->PixelToMapX(egInfo.x0);
			int sy = g_pTopView->PixelToMapY(egInfo.y0);
			int dx = g_pTopView->PixelToMapX(egInfo.x1);
			int dy = g_pTopView->PixelToMapY(egInfo.y1);
			EffectType += min(6, max(2, max(abs(dx-sx), abs(dy-sy))))-2;
		}
		break;
	}

	if(pCreature!= NULL && pCreature->GetCreatureType() == 185 &&
		EffectType >=EFFECTSPRITETYPE_JABBING_VEIN_FRONT_1 && EffectType <= EFFECTSPRITETYPE_JABBING_VEIN_FRONT_BAT)
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_FRONT_BAT;

	if(pCreature!= NULL && pCreature->GetCreatureType() == 185 &&
		EffectType >=EFFECTSPRITETYPE_JABBING_VEIN_REAR_1 && EffectType <= EFFECTSPRITETYPE_JABBING_VEIN_REAR_BAT)
			EffectType = EFFECTSPRITETYPE_JABBING_VEIN_REAR_BAT;
	
	// Creature에게 붙이는 Effect를 생성해서 pointer를 넘겨받는다.
	MAttachEffect* pEffect = pCreature->CreateAttachEffect( EffectType, egInfo.count, egInfo.linkCount );

	if (pEffect == NULL)
	{
		DEBUG_ADD("can't CreateAttachEffect");
		return false;
	}
	
	pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
	pEffect->SetZ( plusZ );

	// 붙어야 하는 캐릭터
	//pEffect->SetAttachCreatureID( creatureID );		

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );
	pEffect->SetDirection( direction );
	
	return true;
}
