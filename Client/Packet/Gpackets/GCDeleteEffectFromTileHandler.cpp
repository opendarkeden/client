//////////////////////////////////////////////////////////////////////
//
// Filename    : GCDeleteEffectFromTileHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCDeleteEffectFromTile.h"
#include "ClientDef.h"
#include "SkillDef.h"
#include "mintr.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCDeleteEffectFromTileHandler::execute ( GCDeleteEffectFromTile * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	
		// -_- 패배
//	switch( pPacket->getEffectID() )
//	{
//	case EFFECTSTATUS_BLOOD_BIBLE_ARMEGA :         // 199			// 타일에 붙는 이펙트
//	case EFFECTSTATUS_BLOOD_BIBLE_MIHOLE :       // 200
//	case EFFECTSTATUS_BLOOD_BIBLE_KIRO :         // 201
//	case EFFECTSTATUS_BLOOD_BIBLE_INI :         // 202
//	case EFFECTSTATUS_BLOOD_BIBLE_GREGORI :     // 203
//	case EFFECTSTATUS_BLOOD_BIBLE_CONCILIA :       // 204
//	case EFFECTSTATUS_BLOOD_BIBLE_LEGIOS :    // 205
//	case EFFECTSTATUS_BLOOD_BIBLE_HILLEL :       // 206
//	case EFFECTSTATUS_BLOOD_BIBLE_JAVE :         // 207
//	case EFFECTSTATUS_BLOOD_BIBLE_NEMA :         // 208
//	case EFFECTSTATUS_BLOOD_BIBLE_AROSA :        // 209
//	case EFFECTSTATUS_BLOOD_BIBLE_CHASPA :         // 210 210			
//		if (!g_pZone->RemoveTileEffect( pPacket->getX(), pPacket->getY(), EFFECTSTATUS_DROP_BLOOD_BIBLE, pPacket->getObjectID() ) )
//		{
//			DEBUG_ADD("[Error] Can't Remove Effect From Tile (DropBloodBible HardCoding)");
//		}
//		break;
//	}	

//	_MinTrace("Incomming GCDeleteEffectFromTileHandler\n");
	if (g_pZone!=NULL)
	{		
		if (!g_pZone->RemoveTileEffect( pPacket->getX(), pPacket->getY(), (int)pPacket->getEffectID(), pPacket->getObjectID() ))
		{
			DEBUG_ADD("[Error] Can't Remove Effect From Tile");
		} else
		{
//			_MinTrace("success delete effect\n");
		}
	}
	else
	{
		DEBUG_ADD("[Error] g_pZone is NULL");
	}
	
#endif

	__END_CATCH
}