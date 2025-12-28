//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddSlayerHandler.cc
// Written By  : Reiot
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddSlayer.h"
#include "ClientDef.h"
#include "AddonDef.h"
#include "MCreatureTable.h"
#include "MTestDef.h"
#include "MGameStringTable.h"

extern void SetPetInfo(PetInfo* pPetInfo, TYPE_OBJECTID objectID);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddSlayerHandler::execute ( GCAddSlayer * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
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
		const PCSlayerInfo3 & si = pPacket->getSlayerInfo();
			
		DEBUG_ADD_FORMAT("[AddSlayer] id=%d, xy=(%d,%d), dir=%d", si.getObjectID(), si.getX(), si.getY(), si.getDir());				
		
		MCreature* pCreature = g_pZone->GetCreature(si.getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		// 복장이 있는 Creature이다.
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			MCreatureWear* pCreatureWear = new MCreatureWear;

			pCreatureWear->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, si.getAdvancementLevel() );
			pCreatureWear->SetZone( g_pZone );

			pCreature = (MCreature*)pCreatureWear;

			if (si.getCompetence()==0)
			{
				pCreatureWear->SetCompetence( 0 );
				pCreatureWear->SetCreatureType( CREATURETYPE_SLAYER_OPERATOR );
				pCreatureWear->SetMale( si.getSex()==MALE );
			}
			else
			{
				pCreatureWear->SetCreatureType( (si.getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );
			}

			pCreatureWear->SetGuildNumber( si.getGuildID() );
			pCreatureWear->SetUnionGuildID( si.getUnionID() );

			pCreatureWear->SetGroundCreature();
			pCreatureWear->SetID(si.getObjectID());
			//pCreatureWear->SetAction(ACTION_MOVE);
			pCreatureWear->SetPosition( si.getX(), si.getY() );
			pCreatureWear->SetServerPosition( si.getX(), si.getY() );
			pCreatureWear->SetDirection( si.getDir() );
			pCreatureWear->SetCurrentDirection( si.getDir() );
			pCreatureWear->SetAction( ACTION_STAND );

			// 피부색
			pCreatureWear->SetBodyColor1( si.getSkinColor() );

			pCreature->SetMasterEffectType(si.getMasterEffectColor());

			pCreatureWear->SetStatus( MODIFY_MAX_HP, si.getMaxHP() );
			pCreatureWear->SetStatus( MODIFY_CURRENT_HP, si.getCurrentHP() );
			pCreatureWear->SetStatus( MODIFY_ALIGNMENT, si.getAlignment() );
			pCreatureWear->SetStatus( MODIFY_RANK, si.getRank() );

			// 이름
			pCreatureWear->SetName( si.getName().c_str() );

			// 색깔 정보

			// 복장 정보 --> 일단 기본 옷을 입고 있게 한다.
			//----------------------------------------	
			// 복장을 착용한다.
			//----------------------------------------	
			SetAddonToSlayer( pCreatureWear, &si );
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());

			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsSlayer()== true && pCreature->IsNPC() == false)
			{
				if(!pPacket->getStoreOutlook().getSign().empty())
					pCreature->SetPersnalString((char*)pPacket->getStoreOutlook().getSign().c_str(),g_pUserOption->ChattingColor);
				else
					pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);

			}

			if (!g_pZone->AddCreature( pCreatureWear ))
			{
				delete pCreatureWear;
				pCreatureWear = NULL;
				pCreature = NULL;
			}
		}
		//--------------------------------------------------
		// 이미 있는 Creature인 경우
		//--------------------------------------------------
		else
		{
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, si.getAdvancementLevel() );
			pCreature->SetGuildNumber( si.getGuildID() );
			pCreature->SetUnionGuildID( si.getUnionID() );

			pCreature->SetGroundCreature();
			
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( si.getX(), si.getY() );
			pCreature->SetServerPosition( si.getX(), si.getY() );
			pCreature->SetDirection( si.getDir() );
			pCreature->SetCurrentDirection( si.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 피부색
			pCreature->SetBodyColor1( si.getSkinColor() );
			pCreature->SetMasterEffectType(si.getMasterEffectColor());

			pCreature->SetStatus( MODIFY_MAX_HP, si.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, si.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, si.getAlignment() );
			pCreature->SetStatus( MODIFY_RANK, si.getRank() );
			
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());
			
			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsSlayer()== true && pCreature->IsNPC() == false)
			{
				if(!pPacket->getStoreOutlook().getSign().empty())
					pCreature->SetPersnalString((char*)pPacket->getStoreOutlook().getSign().c_str(),g_pUserOption->ChattingColor);
				else
					pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);

			}

		}

		if (pCreature!=NULL)
		{
			//--------------------------------------------------
			// Effect 붙이기..
			//--------------------------------------------------
			SetEffectInfo( pCreature, pPacket->getEffectInfo() );
			
			// 펫 처리
			if(pPacket->getPetInfo() != NULL)
				SetPetInfo(pPacket->getPetInfo(), pCreature->GetID());
			NicknameInfo* _tempNick = pPacket->getNicknameInfo();
			if(_tempNick != NULL)
			{
				// 커스텀 닉네임 일때
				if(_tempNick->getNicknameType() == NicknameInfo::NICK_CUSTOM_FORCED ||
				   _tempNick->getNicknameType() == NicknameInfo::NICK_CUSTOM)
				{
					pCreature->SetNickName(_tempNick->getNicknameType(), (char*)_tempNick->getNickname().c_str());
					
				}
				else // 닉네임 인덱스가 있을 때
				{
					int TempIndex = _tempNick->getNicknameIndex();
					if(TempIndex >= g_pNickNameStringTable->GetSize())
						TempIndex = 0;
					pCreature->SetNickName(_tempNick->getNicknameType(), (char*)(*g_pNickNameStringTable)[TempIndex].GetString());
				}
			}
		}
	}

	// [도움말] Slayer가 나타날때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_CREATURE_APPEAR_SLAYER );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
