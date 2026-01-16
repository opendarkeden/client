//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSMSAddressListHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSMSAddressList.h"

#include "ClientDef.h"
#include "PacketFunction.h"
#include "UIFunction.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSMSAddressListHandler::execute ( GCSMSAddressList * pGCSMSAddressList , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY /*__BEGIN_DEBUG_EX*/
		
#ifdef __GAME_CLIENT__

		std::vector<AddressUnit*> TempList = pGCSMSAddressList->getAddresses();
		if(TempList.size())
		{
			UI_SetSMSList((void*)&TempList);
		}
	
#elif __WINDOWS__

#endif

	//__END_DEBUG_EX 
	__END_CATCH
}
