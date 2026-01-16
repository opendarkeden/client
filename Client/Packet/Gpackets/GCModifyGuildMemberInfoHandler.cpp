//////////////////////////////////////////////////////////////////////
//
// Filename    : GCModifyGuildMemberInfoHandler.cc
// Written By  : elca
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCModifyGuildMemberInfo.h"
#include "UserInformation.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCModifyGuildMemberInfoHandler::execute ( GCModifyGuildMemberInfo * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	g_pUserInformation->GuildName = pPacket->getGuildName().c_str();
	g_pUserInformation->GuildGrade = pPacket->getGuildMemberRank();
	g_pPlayer->SetGuildNumber(pPacket->getGuildID());

	UI_SetGuild(pPacket->getGuildID(), pPacket->getGuildMemberRank(), pPacket->getGuildName());
#endif

	__END_CATCH
}
