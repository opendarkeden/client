//////////////////////////////////////////////////////////////////////
//
// Filename    : GCExecuteElementHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCExecuteElement.h"

#include "Client.h"
#include "vs_ui.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCExecuteElementHandler::execute ( GCExecuteElement * pGCExecuteElement , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 
//		__BEGIN_DEBUG_EX
		
#ifdef __GAME_CLIENT__

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
#ifdef __DEBUG_OUTPUT__
		DEBUG_ADD_FORMAT("[GCExecuteElementHandler] QuestID=%d, bCondition=%d, index = %d", pGCExecuteElement->getQuestID(), pGCExecuteElement->getCondition(), pGCExecuteElement->getIndex());
#endif
		gC_vs_ui.PushGQuestExcuteElement(pGCExecuteElement->getQuestID(), pGCExecuteElement->getCondition(), pGCExecuteElement->getIndex());

	}
	
//#elif __WINDOWS__

#endif

//	__END_DEBUG_EX 
	__END_CATCH
}
