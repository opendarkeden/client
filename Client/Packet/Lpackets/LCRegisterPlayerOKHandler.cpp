//////////////////////////////////////////////////////////////////////
//
// Filename    : LCRegisterPlayerOKHandler.cpp
// Written By  : Reiot
// Description : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "LCRegisterPlayerOK.h"
#include "UserInformation.h"
#include "ClientDef.h"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__

	#include "ClientPlayer.h"
	#include "Cpackets/CLGetPCList.h"

#endif

extern BOOL g_bNeedUpdate;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerOKHandler::execute ( LCRegisterPlayerOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	//cout << "플레이어 등록에 성공했습니당.. 추카.. " << endl;

	//throw DisconnectException("플레이어 등록에 성공했으니깐, 접속을 종료하고 다시 해랑..");
	// Debug Message

	if (!g_bNeedUpdate)
	{
		ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

		CLGetPCList clGetPCList;

		pClientPlayer->sendPacket( &clGetPCList );
		
		// 플레이어의 상태를 바꾼다.
		pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CL_GET_PC_LIST );

		//------------------------------------------------------------
		// 닫는다.
		//------------------------------------------------------------
		UI_CloseUserRegistrationWindow();

		// ID를 기억시켜둔다.
		if( g_pUserInformation->UserID != NULL && g_pUserInformation->UserID.GetLength() < 15 )
			UI_BackupLoginID( g_pUserInformation->UserID );
		else
			UI_BackupLoginID( "DarkEden" );


		//------------------------------------------------------------
		// 현재 Server 정보를 저장해둔다.
		//------------------------------------------------------------
		SetServerGroupName( pPacket->getGroupName().c_str() );
		//SetServerName( pPacket->getServerName().c_str() );

		//------------------------------------------------------------
		// Client는 PC List를 기다려야 한다.
		//------------------------------------------------------------	
		SetMode( MODE_WAIT_PCLIST );

		//------------------------------------------------------------
		// Gore Level을 바꾼다.
		//------------------------------------------------------------
		bool bGoreLevel = pPacket->isAdult() && !g_pUserOption->UseTeenVersion;

		SetGoreLevel( bGoreLevel);	
		
	}

#endif

	__END_CATCH
}
