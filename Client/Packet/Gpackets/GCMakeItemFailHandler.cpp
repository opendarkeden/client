//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMakeItemFailHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCMakeItemFail.h"
#include "TempInformation.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCMakeItemFailHandler::execute ( GCMakeItemFail * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	//------------------------------------------------------------------
	// Temp Mode 체크
	//------------------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_SKILL_MAKE_ITEM)
	{
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		//g_pTempInformation->Value1 = itemClass;
		//g_pTempInformation->Value2 = itemType;

		// 못 움직이게 했던거 풀기
		UI_UnlockItem();
	}
	else
	{
		DEBUG_ADD( "[Error] Temp Mode is Not MODE_SKILL_MAKE_ITEM" );
	}

#endif

	__END_CATCH
}
