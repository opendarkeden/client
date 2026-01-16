//--------------------------------------------------------------------------------
//
// Filename    : GCAddressListVerifyHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
// include files
#include "GCAddressListVerify.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

#include "ClientDef.h"
#include "TempInformation.h"
#include "UIFunction.h"
#include "MGameStringTable.h"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddressListVerifyHandler::execute ( GCAddressListVerify * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY /*__BEGIN_DEBUG_EX*/
	/*__BEGIN_DEBUG*/
		
#ifdef __GAME_CLIENT__
	
	if(NULL == pPacket)
		return;

	switch(pPacket->getCode())
	{
	case GCAddressListVerify::ADDRESS_LIST_ADD_OK:
		if (g_pTempInformation->GetMode()==TempInformation::MODE_SMS_ADD_ADDRESS)
			UI_AddSMSList(pPacket->getParameter(), (char*)g_pTempInformation->Value1,(char*)g_pTempInformation->Value2,(char*)g_pTempInformation->Value3);
		break;
	case GCAddressListVerify::ADDRESS_LIST_DELETE_OK:
		if (g_pTempInformation->GetMode()==TempInformation::MODE_SMS_DELETE_ADDRESS)
			UI_DeleteSMSList(g_pTempInformation->Value1); 
		break;
	case GCAddressListVerify::ADDRESS_LIST_ADD_FAIL:
		if(pPacket->getParameter())
			UI_SMS_ERROR(pPacket->getParameter());
		else
			UI_PopupMessage(UI_STRING_MESSAGE_SMS_ADD_FAIL);
		break;
	case GCAddressListVerify::ADDRESS_LIST_DELETE_FAIL:
		if(pPacket->getParameter())
			UI_SMS_ERROR(pPacket->getParameter());
		else
			UI_PopupMessage(UI_STRING_MESSAGE_SMS_DELETE_FAIL);
		break;
	case GCAddressListVerify::SMS_SEND_OK:
		UI_SMS_Set_Charge(pPacket->getParameter());
		UI_PopupMessage(UI_STRING_MESSAGE_SMS_SEND_OK);
		break;
	case GCAddressListVerify::SMS_SEND_FAIL:
		if(pPacket->getParameter())
			UI_SMS_ERROR(pPacket->getParameter());
		else
			UI_PopupMessage(UI_STRING_MESSAGE_SMS_SEND_FAIL);
		break;
	case GCAddressListVerify::SMS_CHARGE_OK: 
		UI_SMS_Set_Charge(pPacket->getParameter());
		break;
	};

	g_pTempInformation->SetMode(TempInformation::MODE_NULL); 


#endif

	/*__END_DEBUG*/
	/*__END_DEBUG_EX*/ __END_CATCH
}
