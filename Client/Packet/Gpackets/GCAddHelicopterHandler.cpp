//////////////////////////////////////////////////////////////////////////////
// Filename    : GCAddHelicopterHandler.cpp
// Written By  : excel96
// Description :
// 슬레이어가 무전기를 사용해, 헬기를 호출했을 경우에 날아가는 패킷
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCAddHelicopter.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void GCAddHelicopterHandler::execute ( GCAddHelicopter * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	if (g_pZone!=NULL)
	{
		TYPE_OBJECTID creatureID = pPacket->getObjectID();

		if (pPacket->getCode()==0)
		{
			g_pZone->AddHelicopter( creatureID, 0, 0 );		
		}
		else
		{
			g_pZone->RemoveHelicopter( creatureID );
		}
	}

#endif

	__END_CATCH
}
