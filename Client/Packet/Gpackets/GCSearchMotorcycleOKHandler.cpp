//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSearchMotorcycleOKHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSearchMotorcycleOK.h"
#include "ClientDef.h"
#include "UIDialog.h"
#include "MZoneTable.h"
#include "MGameStringTable.h"

void GCSearchMotorcycleOKHandler::execute ( GCSearchMotorcycleOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	

	ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( pPacket->getZoneID() );

	if (pZoneInfo==NULL)
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_FIND_MOTOR_NO_WHERE].GetString());
	}
	else
	{
		char str[128];

		sprintf(str, (*g_pGameStringTable)[STRING_MESSAGE_FIND_MOTOR_OK].GetString(), 
						pZoneInfo->Name.GetString(),
						pPacket->getX(),
						pPacket->getY());

		g_pUIDialog->PopupFreeMessageDlg( str );
	}

#endif

	__END_CATCH
}
