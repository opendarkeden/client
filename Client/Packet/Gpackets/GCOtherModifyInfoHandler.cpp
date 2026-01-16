//////////////////////////////////////////////////////////////////////////////
// Filename    : GCOtherModifyInfoHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCOtherModifyInfo.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCOtherModifyInfoHandler::execute ( GCOtherModifyInfo * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
		
	}	
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		MCreature* pCreature = g_pZone->GetCreature(pPacket->getObjectID());

		if (pCreature==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] the Creature is Not Exist. id=%d", pPacket->getObjectID());			
		}
		else
		{
			AffectModifyInfo( pCreature, pPacket );

			if(pCreature->GetName() != NULL && UI_GetOtherInfoName() != NULL && strcmp(pCreature->GetName(), UI_GetOtherInfoName()) == 0)
			{
				if( pCreature->GetRace() == g_pPlayer->GetRace())//pCreature->IsSlayer() && g_pPlayer->IsSlayer() || pCreature->IsVampire() && g_pPlayer->IsVampire() || pCreature->IsOusters() && g_pPlayer->IsOusters()  ))
					UI_RunOtherInfo(pCreature);
			}
		}
	}


	__END_CATCH
}
