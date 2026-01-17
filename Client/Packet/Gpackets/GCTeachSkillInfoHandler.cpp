//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTeachSkillInfoHandler.cc
// Written By  : 김성민 
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTeachSkillInfo.h"
#include "TempInformation.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCTeachSkillInfoHandler::execute ( GCTeachSkillInfo * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	// get/set NPC's object id
	//pPacket->getObjectID();
	
	// Skill을 배우기 위해서 SkillTree를 띄운다.
	UI_RunSkillTree( (int)pPacket->getDomainType(), 100 );//pPacket->getMaxLevel() );
	
	// 임시로 정보 저장
	g_pTempInformation->SetMode(TempInformation::MODE_SKILL_LEARN);
	g_pTempInformation->Value1	=	pPacket->getDomainType();
	g_pTempInformation->Value2	=	100;//pPacket->getMaxLevel();
	

#endif

	__END_CATCH
}
