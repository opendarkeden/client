//////////////////////////////////////////////////////////////////////
//
// Filename    : GCPetInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include 
#include "Client_PCH.h"
#include "GCPetInfo.h"
#include "MFakeCreature.h"
#include "PacketFunction.h"

extern void SetPetInfo(PetInfo* pPetInfo, TYPE_OBJECTID objectID);

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCPetInfoHandler::execute ( GCPetInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	SetPetInfo(pPacket->getPetInfo(), pPacket->getObjectID());
#endif

	__END_CATCH
}
