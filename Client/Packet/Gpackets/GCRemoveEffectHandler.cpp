//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRemoveEffectHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRemoveEffect.h"
#include "ClientDef.h"
#include "MEffectStatusDef.h"
#include "MEventManager.h"
#include "MSkillManager.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRemoveEffectHandler::execute ( GCRemoveEffect * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		int CreatureID = pPacket->getObjectID();
		
		MCreature* pCreature = g_pZone->GetCreature(CreatureID);

		// 성물보관대 얻어오기 위해서
		if (pCreature == NULL)
		{
			MItem *selectedItem = g_pZone->GetItem(CreatureID);
			if(selectedItem != NULL)
			{
				if(selectedItem->GetItemClass() == ITEM_CLASS_CORPSE)
				{
					MCorpse *pCorpse = (MCorpse *)selectedItem;
					pCreature = pCorpse->GetCreature();
				}
			}
		}

		if (pCreature==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] There is no such Creature. id=%d", pPacket->getObjectID());
		}
		else
		{	
			int size = pPacket->getListNum();

			for (int i=0; i<size; i++)
			{
				WORD effectStatus = pPacket->popFrontListElement();

				switch( effectStatus )
				{
					case EFFECTSTATUS_KEEP_SWEEPER :                      // 304
					case EFFECTSTATUS_KEEP_SWEEPER_2 :                    // 305
					case EFFECTSTATUS_KEEP_SWEEPER_3 :                    // 306
					case EFFECTSTATUS_KEEP_SWEEPER_4 :                    // 307
					case EFFECTSTATUS_KEEP_SWEEPER_5 :                    // 308
					case EFFECTSTATUS_KEEP_SWEEPER_6 :                    // 309
					case EFFECTSTATUS_KEEP_SWEEPER_7 :                    // 310
					case EFFECTSTATUS_KEEP_SWEEPER_8 :                    // 311
					case EFFECTSTATUS_KEEP_SWEEPER_9 :                    // 312
					case EFFECTSTATUS_KEEP_SWEEPER_10 :                   // 313
					case EFFECTSTATUS_KEEP_SWEEPER_11 :                   // 314
					case EFFECTSTATUS_KEEP_SWEEPER_12 :                   // 315
						PlaySound( SOUND_SWEEPER_END );
						break;
				}

				if (pCreature->RemoveEffectStatus( (EFFECTSTATUS)effectStatus ),0)
				{					
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] Failed to Remove EffectStatus %d", effectStatus);
				}
				if( g_pPlayer->GetID() == pCreature->GetID() ) // -_- 내 자신이면
				{
					ACTIONINFO actionInfo = (ACTIONINFO) (*g_pEffectStatusTable)[effectStatus].OriginalActionInfo;
					switch(actionInfo)
					{
						//						case MAGIC_SACRIFICE :
					case MAGIC_AURA_SHIELD :
						(*g_pSkillInfoTable)[actionInfo].SetAvailableTime();
						break;						
					}
				}

				if( effectStatus == EFFECTSTATUS_FADE_OUT) 
					pCreature->SetVisibleSoon();
				
//				if( effectStatus == EFFECTSTATUS_REDIANCE && pCreature->IsSlayer() )
//				{
//					MCreatureWear* pCreatureWear = dynamic_cast<MCreatureWear*>( pCreature );
////					_MinTrace("Basic ActionInfo : %d %s\n",pCreatureWear->GetID(), pCreatureWear->GetName() );
//					
//					const MCreatureWear::ADDON_INFO &addonInfo = pCreatureWear->GetAddonInfo( ADDON_RIGHTHAND );
//					
//					if( addonInfo.bAddon )
//					{
////						_MinTrace("Basic ActionInfo : %d %d %d\n",addonInfo.ItemClass,addonInfo.ItemType);
//						if( addonInfo.ItemClass < g_pItemTable->GetSize() &&
//							addonInfo.ItemType < g_pItemTable[addonInfo.ItemClass].GetSize() &&
//							(*g_pItemTable)[addonInfo.ItemClass][addonInfo.ItemType].UseActionInfo != ACTIONINFO_NULL )
//							pCreature->SetBasicActionInfo( (*g_pItemTable)[addonInfo.ItemClass][addonInfo.ItemType].UseActionInfo );
//						else
//							pCreature->SetBasicActionInfo( SKILL_ATTACK_MELEE );
//					} else
//						pCreature->SetBasicActionInfo( SKILL_ATTACK_MELEE );
//				}
			}
		}
	}

#endif

	__END_CATCH
}
