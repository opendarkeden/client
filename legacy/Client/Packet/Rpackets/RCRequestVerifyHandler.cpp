//////////////////////////////////////////////////////////////////////
//
// Filename    : RCRequestVerifyHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "RCRequestVerify.h"
#include "MGameStringTable.h"
#include "RequestClientPlayer.h"
#include "RequestUserManager.h"
#include "ProfileManager.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void RCRequestVerifyHandler::execute ( RCRequestVerify * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY

	RequestClientPlayer* pRequestClientPlayer = dynamic_cast<RequestClientPlayer*>( pPlayer );

	if (pRequestClientPlayer!=NULL
		&& g_pRequestUserManager!=NULL
		&& g_pGameStringTable!=NULL)
	{
		switch (pPacket->getCode())
		{
			//-----------------------------------------------------------------
			// 귓속말이 전달되지 않은 경우
			//-----------------------------------------------------------------
			case REQUEST_VERIFY_WHISPER_FAILED :
			{
				// info = 5
				UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_WHISPER_FAILED].GetString(), NULL, 5, RGB(255, 255, 255) );

				// 잘못된 IP거나..
				// 그 IP의 사람이 다른 사용자로 접속했거나 해서...
				// 더 이상 쓸모없는 정보이므로 제거한다.
				g_pRequestUserManager->RemoveRequestUser( pRequestClientPlayer->getRequestServerName().c_str() );
			}
			break;

			//-----------------------------------------------------------------
			// 잘못된 사용자에게 Profile을 요청한 경우
			//-----------------------------------------------------------------
			case REQUEST_VERIFY_PROFILE_WRONG_USER :
			{
				// 잘못된 IP거나..
				// 그 IP의 사람이 다른 사용자로 접속했거나 해서...
				// 더 이상 쓸모없는 정보이므로 제거한다.
				g_pRequestUserManager->RemoveRequestUser( pRequestClientPlayer->getRequestServerName().c_str() );
			}
			break;

			//-----------------------------------------------------------------
			// Profile이 아예 없는 경우
			//-----------------------------------------------------------------
			case REQUEST_VERIFY_PROFILE_NOT_EXIST :
			{
				g_pProfileManager->AddProfileNULL( pRequestClientPlayer->getRequestServerName().c_str() );
			}
			break;

			//-----------------------------------------------------------------
			// Profile 다 보냈다고 날아온 경우
			//-----------------------------------------------------------------
			case REQUEST_VERIFY_PROFILE_DONE :
			{
				//pRequestClientPlayer->disconnect( UNDISCONNECTED );
				throw DisconnectException("Profile Done");
			}
			break;
		}	
	}

	__END_CATCH
}
