//////////////////////////////////////////////////////////////////////
//
// Filename    : GCUseOKHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCUseOK.h"
#include "ClientDef.h"
#include "MInventory.h"
#include "MQuickSlot.h"
#include "MSlayerGear.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCUseOKHandler::execute ( GCUseOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException, Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

	// message

	// 음.. cut & paste의 걸작.. - -;;

	UseItemOK();

	//------------------------------------------------------------------
	//
	//						상태값을 바꾼다.
	//
	//------------------------------------------------------------------
	AffectModifyInfo( g_pPlayer, pPacket );


	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}

void GCUseSkillCardOKHandler::execute ( GCUseSkillCardOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException, Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

	// message

	// 음.. cut & paste의 걸작.. - -;;

	UseSkillCardOK(pPacket->getCardType());

	//------------------------------------------------------------------
	//
	//						상태값을 바꾼다.
	//
	//------------------------------------------------------------------
//	AffectModifyInfo( g_pPlayer, pPacket );


	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}