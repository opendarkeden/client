//////////////////////////////////////////////////////////////////////
//
// Filename    : GCModifyNicknameHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCModifyNickname.h"
#include "ClientDef.h"
#include "VS_UI.h"
#include "MGameStringTable.h"
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCModifyNicknameHandler::execute ( GCModifyNickname * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY /*__BEGIN_DEBUG_EX*/
	
#ifdef __GAME_CLIENT__

		int CreatureID = pPacket->getObjectID();
		
		if (g_pZone==NULL)
		{
			// message
			DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
			return;
		}
		
		MCreature* pCreature = g_pZone->GetCreature(CreatureID);

		if (pCreature != NULL)
		{
			NicknameInfo TempNick	= pPacket->getNicknameInfo();
			const char* szNickName;
			if(TempNick.getNicknameType() == NicknameInfo::NICK_CUSTOM_FORCED ||
				TempNick.getNicknameType() == NicknameInfo::NICK_CUSTOM)
			{
				szNickName = (TempNick.getNickname()).c_str();
			}
			else // 닉네임 인덱스가 있을 때
			{
				DWORD TempIndex = TempNick.getNicknameIndex();
				if(TempIndex >= g_pNickNameStringTable->GetSize())
					TempIndex = 0;
				szNickName = (*g_pNickNameStringTable)[TempIndex].GetString();
			}

			pCreature->SetNickName(TempNick.getNicknameType(), (char*)szNickName);
//			if(pCreature->GetID() == g_pPlayer->GetID())
//			{
//				g_char_slot_ingame.m_NickNameType = TempNick.getNicknameType();
//				g_char_slot_ingame.m_NickName = szNickName;
//			}

		}

#endif

	/*__END_DEBUG_EX */__END_CATCH
}
