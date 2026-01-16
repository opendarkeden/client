//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSayHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCFriendChatting.h"
#include "ClientDef.h"
#include "UIFunction.h"


//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////

void GCFriendChattingHandler::execute ( GCFriendChatting * pPacket , Player * pPlayer )
	 

throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	switch(pPacket->getCommand())
	{
	case GC_UPDATE:
		{
			UI_FriendChatting_UpdateFriendList(pPacket);
			break;
		}
	case GC_MESSAGE:
		{
			UI_FriendChatting_Message(pPacket);
			break;
		}
	case GC_ADD_FRIEND_REQUEST:
		{
			UI_RunFriendRequest(pPacket);
			break;
		}
	case GC_ADD_FRIEND_REFUSE:
		{
			UI_RunFriendRefuse(pPacket);
			break;
		}
	case GC_ADD_FRIEND_WAIT:
		{
			UI_RunFriendWait(pPacket);
			break;
		}
	case GC_ADD_FRIEND_OK:
		{
			UI_RunFriendOK(pPacket);
			break;
		}
	case GC_ADD_FRIEND_EXIST:
		{
			UI_RunFriendExist(pPacket);
			break;
		}
	case GC_ADD_FRIEND_BLACK:
		{
			UI_RunFriendBlack(pPacket);
			break;
		}
	case GC_FRIEND_DELETE:
		{
			UI_RunFriendDelete(pPacket);
			break;
		}
	default:
		break;
	}

	__END_CATCH
}
