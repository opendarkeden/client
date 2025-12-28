//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAddItemToCodeSheetHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//////////////////////////////////////////////////////////////////////////////
/*

// 한 아이템을 들어서 다른 아이템에 넣는다.
 [Client]                                               [Server]
           -- CGAddItemToCodeSheet
              (mouseItem_ObjectID, invenX, invenY) 
			  =(마우스에 들고있는 별, upgrade할 item XY) -->

            <-- GCAddItemToCodeSheetVerify(결과코드, 관련값) ---
   
//-------------+---------------------------------------------
// 사용 packet | client 처리
//-------------+---------------------------------------------
// 불가 packet | 불가 message 출력
// 실패 packet | 실패 message 출력, 별 삭제, [아이템 제거]
// 성공 packet | 별 삭제, option변경
//-------------+---------------------------------------------
*/

#include "Client_PCH.h"
#include "CGAddItemToCodeSheet.h"

#ifdef __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGAddItemToCodeSheetHandler::execute (CGAddItemToCodeSheet* pPacket , Player* pPlayer)
	 throw (Error)
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#ifdef __GAME_SERVER__

#endif	// __GAME_SERVER__
		
	__END_DEBUG_EX __END_CATCH
}

#endif	// __GAME_SERVER__
