//----------------------------------------------------------------------
//
// Filename    : LCLoginOKHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCLoginOK.h"

#ifdef __GAME_CLIENT__

	#include "ClientPlayer.h"
	//#include "Cpackets/CLGetPCList.h"
	#include "Cpackets/CLGetWorldList.h"
	#include "UserInformation.h"
	#include "nProtect.h"
	#include "MGameStringTable.h"
	#include "UIDialog.h"
#endif

#include "ClientDef.h"
#include "UIFunction.h"
//#include "ServerInformation.h"

extern BOOL		g_bNeedUpdate;
extern int		g_LeftPremiumDays;
extern bool		g_bFamily;

bool	ChecknProtectCRC(HINSTANCE dll, char* FilePath);

//----------------------------------------------------------------------
//
// 로그인 서버로부터 아이디와 패스워드의 인증을 받았다는 소리다.
//
// 이제 로그인 서버에게 PC 의 리스트를 요청하는 패킷을 전송하면 된다.
//
//----------------------------------------------------------------------
void LCLoginOKHandler::execute ( LCLoginOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	if( g_pUserInformation->UserID.GetLength() >= 15 )
	{
		UI_BackupLoginID( "DarkEden" );
	} 
	else 
		UI_BackupLoginID( g_pUserInformation->UserID );

	if (!g_bNeedUpdate)
	{
		// 2004, 07, 20 sobeit add start
		// 블럭 처리 된건 그전 꺼
//			ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);
//
//			CLGetWorldList clGetWorldList;
//
//			pClientPlayer->sendPacket( &clGetWorldList );
//
//			SetMode( MODE_WAIT_WORLD_LIST );

		g_pUserInformation->pLogInClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);
		// 2006.11.07 edit Coffee Start
			CLGetWorldList clGetWorldList;

			ClientPlayer * pClientPlayer = (ClientPlayer *)(g_pUserInformation->pLogInClientPlayer);
			pClientPlayer->sendPacket( &clGetWorldList );

			SetMode( MODE_WAIT_WORLD_LIST );
/*
		if(false == g_pUserInformation->IsAutoLogIn) 
		{
			CLGetWorldList clGetWorldList;

			ClientPlayer * pClientPlayer = (ClientPlayer *)(g_pUserInformation->pLogInClientPlayer);
			pClientPlayer->sendPacket( &clGetWorldList );

			SetMode( MODE_WAIT_WORLD_LIST );
		}	

  */
		// 2006.11.07  Coffee edit end
		//else
		//	SetMode( MODE_MAINMENU );
		// 2004, 07, 20 sobeit add end


		/*
		// 예전꺼
		ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

		CLGetPCList clGetPCList;

		pClientPlayer->sendPacket( &clGetPCList );
		
		// 플레이어의 상태를 바꾼다.
		pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CL_GET_PC_LIST );

		//------------------------------------------------------------
		// 현재 Server 정보를 저장해둔다.
		//------------------------------------------------------------
		SetServerGroupName( pPacket->getGroupName().c_str() );
		SetServerGroupStatus( (int)pPacket->getStat() );
		//SetServerName( pPacket->getServerName().c_str() );

		//------------------------------------------------------------
		// Client는 PC List를 기다려야 한다.
		//------------------------------------------------------------	
		SetMode( MODE_WAIT_PCLIST );
		*/

		// 넷마블용
		//------------------------------------------------------------
		// Gore Level을 바꾼다.
		//------------------------------------------------------------
		bool bGoreLevel;
		
//		if(g_pUserInformation->IsNetmarble)
//			bGoreLevel = g_pUserInformation->bNetmarbleGoreLevel && !g_pUserOption->UseTeenVersion;
//		else
//		By tiancaiamao:
//		bGoreLevel = pPacket->isAdult() && !g_pUserOption->UseTeenVersion;
		bGoreLevel = !g_pUserOption->UseTeenVersion;

		g_pUserInformation->GoreLevel = bGoreLevel;

		SetGoreLevel( bGoreLevel );			
	}

#endif

//#if !defined(OUTPUT_DEBUG) && !defined(_DEBUG) && !defined(__DEBUG_OUTPUT__)
//	if( g_nProtectInstance == NULL )
//	{
//		SendMessage(g_hWnd, WM_NPROTECT_FORCEEXIT, 0, 0 );
//		return;
//	}
//	
//	char szBuffer[256] = {0,};
//	if( GetSystemDirectory( szBuffer, 256) == 0)
//	{
//		SendMessage( g_hWnd, WM_NPROTECT_FORCEEXIT, 0, 0 );
//		return;
//
//	}
//		
//	strcat( szBuffer, "\\NPMON.EXE");	
//	
//	bool	Result;
//	Result  = ChecknProtectCRC(g_nProtectInstance, "FindHack.exe");
//	Result |= ChecknProtectCRC(g_nProtectInstance, szBuffer );
//	
//	if(!Result)
//		SendMessage(g_hWnd, WM_NPROTECT_FORCEEXIT, 0, 0 );
//#endif

	g_bFamily = pPacket->isFamily();

	if( g_pUserInformation != NULL && !g_pUserInformation->IsNetmarble )
	{
		// 2005, 1, 24, sobeit add start
		if(0xFFFD == pPacket->getLastDays())
		{
			g_LeftPremiumDays = 7; // 휴면 계정 아이템 주는 이벤트 시 첫 접속일때 fffd가 날라온다. -> 무료 프리미엄 7일 시작
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_MESSAGE_GET_EVENT_ITEM_NOTICE].GetString() );
		}
		else
		// 2005, 1, 24, sobeit add end
			g_LeftPremiumDays = pPacket->getLastDays();
	}
	else
		g_LeftPremiumDays = 0xFFFF;
	
	DEBUG_ADD_FORMAT("[LCLoginOK] g_LeftPremiumDays : %x pPacket->getLastDays() : %x g_bFamily : %s", g_LeftPremiumDays, pPacket->getLastDays(), g_bFamily?"true":"false");

#if !defined(__DEBUG_OUTPUT__) && !defined(OUTPUT_DEBUG)
	if( g_pUserInformation!=NULL && !g_pUserInformation->bChinese && !g_pUserInformation->IsNetmarble)
		SendUserIDToGameMon(g_pUserInformation->UserID.GetString());
#endif
	__END_CATCH
}
