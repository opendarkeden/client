//////////////////////////////////////////////////////////////////////
//
// Filename    : GCNicknameListHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCNicknameList.h"
#include "vs_ui.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCNicknameListHandler::execute ( GCNicknameList * pGCNicknameList , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 
//		__BEGIN_DEBUG_EX
		
#ifdef __GAME_CLIENT__

	gC_vs_ui.SetNickNameList((void*)&pGCNicknameList->getAddresses());
//	std::vector<NICKNAMEINFO*> g_NickNameList;
	//cout << pGCNicknameList->toString() << endl;
	
//#elif __WINDOWS__

#endif

//	__END_DEBUG_EX 
	__END_CATCH
}
