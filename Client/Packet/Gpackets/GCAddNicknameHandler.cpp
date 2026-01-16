//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddNicknameHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddNickname.h"
#include "VS_UI.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddNicknameHandler::execute ( GCAddNickname * pGCAddNickname , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY/* __BEGIN_DEBUG_EX*/
		
#ifdef __GAME_CLIENT__

		gC_vs_ui.AddNickNameList((void*)&pGCAddNickname->getNicknameInfo());
	//cout << pGCAddNickname->toString() << endl;
	
/*#elif __WINDOWS__*/

#endif

	/*__END_DEBUG_EX */__END_CATCH
}
