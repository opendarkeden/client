//////////////////////////////////////////////////////////////////////////////
// Filename    : GCAddVampirePortalHandler.cpp
// Written By  : excel96
// Description :
// 뱀파이어 포탈은 현재 이펙트의 일종으로서 구현되는데, 이 패킷은
// 클라이언트에게 바닥에다 뱀파이어 포탈 이펙트를 붙이라는 패킷이다.
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCAddVampirePortal.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void GCAddVampirePortalHandler::execute ( GCAddVampirePortal * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 
		
#ifdef __GAME_CLIENT__


	int serverID = pPacket->getObjectID();

	const char* ownerName = pPacket->getOwnerID().c_str();
	TYPE_SECTORPOSITION sX = pPacket->getX();
	TYPE_SECTORPOSITION sY = pPacket->getY();

	DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

	int zoneID = pPacket->getTargetZoneID();
	TYPE_SECTORPOSITION zoneX = pPacket->getTargetX();
	TYPE_SECTORPOSITION zoneY = pPacket->getTargetY();

	// 오래된거면 mainNode부터 실행해야되지..
	bool bStartFromMainNode = (pPacket->getCreateFlag()==0);

	AddVampirePortal( serverID, 
						ownerName, 
						sX, sY,	
						delayFrame,	
						zoneID,	zoneX, zoneY,
						bStartFromMainNode );

#endif

	__END_CATCH
}
