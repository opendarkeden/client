//////////////////////////////////////////////////////////////////////
//
// Filename    : GCUseBonusPointOKHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCUseBonusPointOK.h"
#include "..\cpackets\CGUseBonusPoint.h"
#include "TempInformation.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCUseBonusPointOKHandler::execute ( GCUseBonusPointOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException, Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

	//------------------------------------------------------------
	// bonus point를 사용한 경우 체크..
	//------------------------------------------------------------
	if (g_pTempInformation->GetMode()==TempInformation::MODE_BONUSPOINT_USE)
	{
		//------------------------------------------------------------
		// mode 제거
		//------------------------------------------------------------
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);

		//------------------------------------------------------------
		// 선택한 part를 올려준다.
		//------------------------------------------------------------
		/*
		int incPart = g_pTempInformation->Value1;

		switch (incPart)
		{
			case INC_INT :
				g_pPlayer->SetStatus(MODIFY_CURRENT_INT, g_pPlayer->GetINT() + 1);
			break;

			case INC_STR :
				g_pPlayer->SetStatus(MODIFY_CURRENT_STR, g_pPlayer->GetSTR() + 1);
			break;

			case INC_DEX :
				g_pPlayer->SetStatus(MODIFY_CURRENT_DEX, g_pPlayer->GetDEX() + 1);
			break;
		}
		*/

		int bonusPoint = g_pPlayer->GetBonusPoint();

		//------------------------------------------------------------
		// bonus point를 하나 빼준다.
		//------------------------------------------------------------
		if (bonusPoint-1 >= 0)
		{
			g_pPlayer->SetStatus( MODIFY_BONUS_POINT, bonusPoint-1 );
		}
		else
		{
			DEBUG_ADD("[Error] Minus BonusPoint");
		}
	}

#endif

	__END_CATCH
}
