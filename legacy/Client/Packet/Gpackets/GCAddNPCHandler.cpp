//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddNPCHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddNPC.h"
#include "ClientDef.h"
#include "MNPC.h"
#include "MGuildType.h"
#include "EffectSpriteTypeDef.h"
#include "MINtr.h"

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddNPCHandler::execute ( GCAddNPC * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	// message

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
		MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{			
			int npcID = pPacket->getNPCID();

			MNPC* pNPC = new MNPC;
			
			pNPC->SetZone( g_pZone );

			int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

			// EVE 길드 마크
			int creatureType = npcID;
			int spriteType = (*g_pCreatureTable)[creatureType].SpriteTypes[0];			
			
			CREATURESPRITETABLE_INFO& spriteInfo = (*g_pCreatureSpriteTable)[spriteType];

			if (spriteInfo.IsNPCSprite()
				&& spriteInfo.IsSlayerSprite()
				|| spriteInfo.IsSlayerSprite())
			{
				// slayer NPC
				pNPC->SetGuildNumber( GUILDID_EVE );
			} else
			if( spriteInfo.IsOustersSprite() )
			{
				pNPC->SetGuildNumber( GUILDID_OUSTERS );
			}
			else if (zoneID==1007 || zoneID==1114 || zoneID==1115)
			{
				// 테페즈
				pNPC->SetGuildNumber( GUILDID_TEPEZ );
			}
			else
			{
				// 바토리
				pNPC->SetGuildNumber( GUILDID_BATHORY );
			}

			//pNPC->SetCreatureType( 0 );
			pNPC->SetCreatureType( npcID );
			pNPC->SetGroundCreature();
			pNPC->SetID(pPacket->getObjectID());
			//pNPC->SetAction(ACTION_MOVE);
			pNPC->SetPosition( pPacket->getX(), pPacket->getY() );
			pNPC->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pNPC->SetDirection( pPacket->getDir() );
			pNPC->SetCurrentDirection( pPacket->getDir() );
			pNPC->SetAction( ACTION_STAND );

			if(zoneID>= 1500 && zoneID<=1506)
				pNPC->SetName( pPacket->getName().c_str() );
			else
				pNPC->SetName( (*g_pCreatureTable)[creatureType].Name );

			pNPC->SetBodyColor1( pPacket->getMainColor() );
			pNPC->SetBodyColor2( pPacket->getSubColor() );

			// NPC ID
			pNPC->SetNPCID( pPacket->getNPCID() );
			
			//
			pNPC->SetStatus( MODIFY_MAX_HP, 100 );

//			_MinTrace("AddNPC : %s Dir:%d Pos(%d,%d) CreatureType : %d\n",pNPC->GetName(), pPacket->getDir(), pPacket->getX(), pPacket->getY(), pPacket->getNPCID() );
			//------------------------------------------------------------
			// 바토리인 경우.. 하드코딩(-_-);
			//------------------------------------------------------------
			if (npcID==217)
			{
				pNPC->SetStatus( MODIFY_CURRENT_HP, 10 );
				pNPC->SetStatus( MODIFY_ALIGNMENT, -10000 );	// 별 의미없다
			}
			else
			{
				pNPC->SetStatus( MODIFY_CURRENT_HP, 100 );
			}

			if (!g_pZone->AddCreature( pNPC ))
			{
				delete pNPC;
				pNPC = NULL;
			}
			else
			{			
				//------------------------------------------------------------
				// Slayer NPC인 경우 복장을 입힌다.
				//------------------------------------------------------------
				SetAddonToSlayer( (MNPC*)pNPC, npcID );

				//------------------------------------------------------------
				// Load되지 않았으면 load한다.
				//------------------------------------------------------------
//				LoadCreatureType( pPacket->getNPCID() );

				MAttachEffect* pEffect = NULL;

				if( pNPC->GetCreatureType() == 638 || pNPC->GetCreatureType() == 657)					
					pEffect = pNPC->CreateAttachEffect( EFFECTSPRITETYPE_AMATA, 0xffff,0);
				else
				if( pNPC->GetCreatureType() == 639 || pNPC->GetCreatureType() == 654)
					pEffect = pNPC->CreateAttachEffect( EFFECTSPRITETYPE_RIPATY, 0xffff,0);

				if(pEffect != NULL)
				{
					pEffect->SetDirection( pNPC->GetDirection() );					
					pEffect->SetLink( ACTIONINFO_NULL, NULL );
					pEffect->SetAttachCreatureID( pNPC->GetID() );
				}
			}
		}
		//--------------------------------------------------
		// 이미 있던 Creature인 경우
		//--------------------------------------------------
		else
		{
			if (pCreature->GetClassType()==MCreature::CLASS_NPC)
			{
				// 이미 있다..
				pCreature->SetGroundCreature();
				
				//pCreature->SetAction(ACTION_MOVE);
				pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );				
				pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
				pCreature->SetDirection( pPacket->getDir() );
				pCreature->SetCurrentDirection( pPacket->getDir() );
				pCreature->SetAction( ACTION_STAND );

				pCreature->SetBodyColor1( pPacket->getMainColor() );
				pCreature->SetBodyColor2( pPacket->getSubColor() );

				//
				//pCreature->SetStatus( MODIFY_MAX_HP, 100 );
				//pCreature->SetStatus( MODIFY_CURRENT_HP, 100 );
			}
			else
			{
				DEBUG_ADD_FORMAT("[Error] The Creature is Not NPC. id=%d", pPacket->getObjectID());
			}
		}
	}

	// [도움말] NPC가 나타날때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_CREATURE_APPEAR_NPC );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
