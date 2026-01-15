//////////////////////////////////////////////////////////////////////////////
// Filename    : GCOtherGuildName.cc
// Written By  : elca@ewestsoft.com
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCOtherGuildName.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCOtherGuildNameHandler::execute(GCOtherGuildName* pPacket, Player* pPlayer) 
	 throw ( Error )
{
	__BEGIN_TRY
	// Other Info 검색후에-_- 파티 매니저도 검색해서 길드이름을 넣어준다.

	UI_SetGuildName(pPacket->getGuildID(), pPacket->getGuildName());
	
	__END_CATCH
}
