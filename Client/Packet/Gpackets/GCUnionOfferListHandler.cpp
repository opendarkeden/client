//////////////////////////////////////////////////////////////////////
//
// Filename    : GCUnionOfferListHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCUnionOfferList.h"
#include "MGameStringTable.h"
#include "VS_UI.h"
#include "DebugInfo.h"
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCUnionOfferListHandler::execute ( GCUnionOfferList * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY// __BEGIN_DEBUG_EX
	
#ifdef __GAME_CLIENT__

		
	std::list<SingleGuildUnionOffer*>::const_iterator itr = pPacket->getUnionOfferList().begin();
	std::list<SingleGuildUnionOffer*>::const_iterator enditr = pPacket->getUnionOfferList().end();

	while(itr != enditr)
	{
		Assert( *itr != NULL );
		SingleGuildUnionOffer* offerlist = (*itr);
		SIZE windowSize;
		windowSize.cx = 500; 
		windowSize.cy = 250;
		char sztemp[2048];
		memset(sztemp,0,2048); 

		if(offerlist->getGuildType() == SingleGuildUnionOffer::JOIN)
			sprintf(sztemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_UNION_JOIN_MSG].GetString(), offerlist->getGuildName().c_str() );
		else if(offerlist->getGuildType() == SingleGuildUnionOffer::QUIT)
			sprintf(sztemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_UNION_DEPORT_MSG].GetString(), offerlist->getGuildName().c_str() );
		else
		{
			DEBUG_ADD("[GCUnionOfferListHandler] - SingleGuildUnionOffer guild type error");
			return;
		}
		
		gC_vs_ui.AddMail(0, 9, windowSize, offerlist->getGuildMaster().c_str() , sztemp , offerlist->getDate(), 0, offerlist->getGuildID(), offerlist->getGuildType());
		gC_vs_ui.AddMailContents(0, 9, sztemp ); 

		itr++;
	}
#endif

//	__END_DEBUG_EX
 __END_CATCH
}
