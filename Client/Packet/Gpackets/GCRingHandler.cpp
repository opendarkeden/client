//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRingHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRing.h"
#include "UserInformation.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRingHandler::execute ( GCRing * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY


	int slot = pPacket->getSlotID();
	int pcsNumber = pPacket->getPhoneNumber();
	char pName[128];

	strcpy(pName, pPacket->getName().c_str());

	// 
//	UI_OnLinePCS(pName, pcsNumber);

	// PCS에 전화가 왔다고 알린다.
//	UI_PleaseConnectPCS(pName, pcsNumber, slot);

	// 번호 기억
	g_pUserInformation->OtherPCSNumber[ slot ] = pPacket->getPhoneNumber();
	g_pUserInformation->PCSUserName[ slot ] = pPacket->getName().c_str();

	__END_CATCH
}
