//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddEffectToTileHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddEffectToTile.h"
#include "ClientDef.h"
#include "MEffectStatusTable.h"
#include "SkillDef.h"
#include "MTopView.h"

extern void SetDragonTorando(int Type, DWORD ObjectID, int TileX, int TileY);
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddEffectToTileHandler::execute ( GCAddEffectToTile * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
		
	// ¿¹¿Ü Ã³¸®
	if( pPacket->getEffectID() == EFFECTSTATUS_TRAP_INSTALLED && !g_pPlayer->IsSlayer() )
		return;

	EFFECTSTATUS EffectStatusType = (EFFECTSTATUS)pPacket->getEffectID();
	int skillType	= (*g_pEffectStatusTable)[ EffectStatusType ].ActionInfo;
	
	// 2004, 9, 3, sobeit add start
	if(EffectStatusType == EFFECTSTATUS_TURRET_LASER && g_pPlayer) // °ø¼ºÀü Æ®·¦Áß..°ø°ÝÃø¿¡ ¾Èº¸ÀÌ´Â ±¤¼±
	{
		if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1) || 
			g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_2) || 
			g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_3) || 
			g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_4) || 
			g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_5) )
			return;
	}
	// 2006.10.29  sonic add ÐÞÕýµ¶¶þ×ª¼¼ÄÜÎÞÐ§¹û
	if(EffectStatusType == EFFECTSTATUS_DRAGON_TORNADO || EffectStatusType == EFFECTSTATUS_DRAGON_TORNADO_CHILD)
	{
		SetDragonTorando(EffectStatusType, pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
		return;
	}
	// 2004, 9, 3, sobeit add end
	//------------------------------------------------------------
	// Effect¿Í °ü·ÃµÈ ±â¼úÀÌ ÀÖÀ»¶§..
	//------------------------------------------------------------
	if (skillType!=ACTIONINFO_NULL)
	{
		int x			= pPacket->getX();
		int y			= pPacket->getY();
		int delayFrame	= ConvertDurationToFrame( pPacket->getDuration() );
		
//		switch(skillType)
//		{
//		case SKILL_CLIENT_TRAP_ICICLE_DROP:
//			delayFrame = 16;
//			break;
//		case SKILL_CLIENT_TRAP_ICICLE_AUGER:
//			delayFrame = 14;
//			break;
//		case SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER:
//			delayFrame = 12;
//			break;
//		}
		ExecuteActionInfoFromMainNode(
				skillType,		// °ª ÀÚÃ¼°¡ RESULT_ACTIONINFOÀÌ´Ù.
			
				x, y, 0,
				DIRECTION_DOWN,
				
				OBJECTID_NULL,
				x, y, 0,
				
				delayFrame,	
				
				NULL
		);
	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] No ActionInfo with EffectStatusID=%d", pPacket->getEffectID());
	}
	
	switch( EffectStatusType )
	{
	case EFFECTSTATUS_TRYING :
		ExecuteActionInfoFromMainNode(
			REGEN_TOWER_TRING_POSITION_LIGHT,
			pPacket->getX(),
			pPacket->getY(),
			0,
			DIRECTION_DOWN,
			OBJECTID_NULL,
			pPacket->getX(),
			pPacket->getY(),
			0,
			ConvertDurationToFrame( pPacket->getDuration() ),
			NULL
			);
		break;
		
	case EFFECTSTATUS_GROUND_ELEMENTAL_AURA:
		if( g_pZone != NULL )
		{
			const_cast<MSector &>(g_pZone->GetSector(pPacket->getX(), pPacket->getY())).SetGroundElemental();
		}
		break;

	case EFFECTSTATUS_DARKNESS_FORBIDDEN:
		if( g_pZone != NULL )
		{
			const_cast<MSector &>(g_pZone->GetSector(pPacket->getX(), pPacket->getY())).SetDarknessForbidden();
		}
		break;
	case EFFECTSTATUS_LUCKY:
		ExecuteActionInfoFromMainNode(
			RESULT_LUCKY_AURA,
			pPacket->getX(),
			pPacket->getY(),
			0,
			DIRECTION_DOWN,
			OBJECTID_NULL,
			pPacket->getX(),
			pPacket->getY(),
			0,
			ConvertDurationToFrame( pPacket->getDuration() ),
			NULL
			);
		break;
	case EFFECTSTATUS_MISFORTUNE:
		ExecuteActionInfoFromMainNode(
			RESULT_MISFORTUNE_AURA,
			pPacket->getX(),
			pPacket->getY(),
			0,
			DIRECTION_DOWN,
			OBJECTID_NULL,
			pPacket->getX(),
			pPacket->getY(),
			0,
			ConvertDurationToFrame( pPacket->getDuration() ),
			NULL
			);
		break;
	case EFFECTSTATUS_FURY_OF_GNOME:
		if( g_pZone != NULL )
		{
			const_cast<MSector &>(g_pZone->GetSector(pPacket->getX(), pPacket->getY())).SetFuryOfGnome();
		}
		break;

	case EFFECTSTATUS_HALO:
		if( g_pZone != NULL )
		{
			MCreature * pTargetCreature = g_pZone->GetCreature(pPacket->getObjectID());
			if(pTargetCreature != NULL && g_pTopView != NULL)
			{
				int TempDir = g_pTopView->GetDirectionToPosition(pPacket->getX(), pPacket->getY(), pTargetCreature->GetX(), pTargetCreature->GetY());
//				if(rand()%2)
//					TempDir = (TempDir+2)%8;
//				else
//					TempDir = (TempDir+6)%8;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_HALO_ATTACK, pPacket->getX(), pPacket->getY(), 0,TempDir,	pTargetCreature->GetID(),	
					pTargetCreature->GetX(), pTargetCreature->GetY(), 0, 12, NULL, true);	
			}
		}
		break;
	}

#endif

	__END_CATCH
}
