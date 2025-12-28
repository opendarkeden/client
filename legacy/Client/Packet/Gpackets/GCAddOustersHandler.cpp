//--------------------------------------------------------------------------------
//
// Filename    : GCAddOustersHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddOusters.h"
#include "ClientDef.h"
#include "MCreatureTable.h"
#include "MTestDef.h"
#include "MGameStringTable.h"

extern void SetPetInfo(PetInfo* pPetInfo, TYPE_OBJECTID objectID);
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddOustersHandler::execute ( GCAddOusters * pPacket , Player * pPlayer )
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
		const PCOustersInfo3 & oi = pPacket->getOustersInfo ();

		MCreature* pCreature = g_pZone->GetCreature(oi.getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreatureWear;
			
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, oi.getAdvancementLevel() );

			pCreature->SetZone( g_pZone );

			pCreature->SetName( oi.getName().c_str() );

			//--------------------------------------------------
			// CreatureType 설정
			//--------------------------------------------------
			ItemType_t coatType = oi.getCoatType();

			if (oi.getCompetence()==0)
			{
				pCreature->SetCompetence( 0 );
				pCreature->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
			}
			else
			{
				pCreature->SetCreatureType( GetOustersCreatureType( coatType ) );
			}

			pCreature->SetID(oi.getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetPosition( oi.getX(), oi.getY() );
			pCreature->SetServerPosition( oi.getX(), oi.getY() );
			pCreature->SetDirection( oi.getDir() );
			pCreature->SetCurrentDirection( oi.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 색깔
			pCreature->SetBodyColor1( oi.getHairColor() );
			pCreature->SetBodyColor2( oi.getCoatColor() );
	
			// add by sonic 2006.10.29
			pCreature->SetMasterEffectType( oi.getMasterEffectColor() );
			// end
			//--------------------------------------------------
			// [ TEST CODE ]
			//--------------------------------------------------
			// 옷 색깔 설정하기
			//--------------------------------------------------
			/*
			if (pCreature->IsMale())
			{
				pCreature->SetBodyColor2( 91 );
			}
			else
			{
				pCreature->SetBodyColor2( 38 );
			}
			*/

			pCreature->SetStatus( MODIFY_MAX_HP, oi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, oi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );
			pCreature->SetStatus( MODIFY_RANK, oi.getRank() );

			//oi.getName()
			// 색상 정보

			pCreature->SetWeaponSpeed( oi.getAttackSpeed() );
			
			// 임시로
			pCreature->SetGuildNumber( oi.getGuildID() );
			pCreature->SetUnionGuildID( oi.getUnionID() );
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());
			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsOusters()== true && pCreature->IsNPC() == false)
			{
				if(!pPacket->getStoreOutlook().getSign().empty())
					pCreature->SetPersnalString((char*)pPacket->getStoreOutlook().getSign().c_str(),g_pUserOption->ChattingColor);
				else
					pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);
			}


			if (!g_pZone->AddCreature( pCreature ))
			{
				delete pCreature;
				pCreature = NULL;
			}
			
		}
		//--------------------------------------------------
		// 이미 있는 Creature인 경우
		//--------------------------------------------------
		else
		{
			//--------------------------------------------------
			// CreatureType 설정
			//--------------------------------------------------
			ItemType_t coatType = oi.getCoatType();

			if (oi.getCompetence()==0)
			{
				pCreature->SetCompetence( 0 );
				pCreature->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
			}
			else
			{
				pCreature->SetCreatureType( GetOustersCreatureType( coatType ) );
			}

			// 임시로
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, oi.getAdvancementLevel() );
			pCreature->SetGuildNumber( oi.getGuildID() );
			pCreature->SetUnionGuildID( oi.getUnionID() );
			
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( oi.getX(), oi.getY() );
			pCreature->SetServerPosition( oi.getX(), oi.getY() );			
			pCreature->SetDirection( oi.getDir() );
			pCreature->SetCurrentDirection( oi.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 색깔
			pCreature->SetBodyColor1( oi.getHairColor() );
			pCreature->SetBodyColor2( oi.getCoatColor() );
			pCreature->SetMasterEffectType( oi.getMasterEffectColor() );

	
			//--------------------------------------------------
			// [ TEST CODE ]
			//--------------------------------------------------
			// 옷 색깔 설정하기
			//--------------------------------------------------
			/*
			if (pCreature->IsMale())
			{
				pCreature->SetBodyColor2( 91 );
			}
			else
			{
				pCreature->SetBodyColor2( 38 );
			}
			*/

			pCreature->SetStatus( MODIFY_MAX_HP, oi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, oi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );
			pCreature->SetStatus( MODIFY_RANK, oi.getRank() );

			pCreature->SetWeaponSpeed( oi.getAttackSpeed() );
			
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());

			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsOusters()== true && pCreature->IsNPC() == false)
			{
				if(!pPacket->getStoreOutlook().getSign().empty())
					pCreature->SetPersnalString((char*)pPacket->getStoreOutlook().getSign().c_str(),g_pUserOption->ChattingColor);
				else
					pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);
 
//				pCreature->SetPersnalString((char *)pPacket->getStoreOutlook().getSign().c_str());
//				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());
			}

			
		}

		if (pCreature!=NULL)
		{
			//--------------------------------------------------
			// Effect 붙이기..
			//--------------------------------------------------
			SetEffectInfo( pCreature, pPacket->getEffectInfo() );
			
			SetAddonToOusters( (MCreatureWear*)pCreature, &oi );			

			// 펫 처리
			if(pPacket->getPetInfo() != NULL)
				SetPetInfo(pPacket->getPetInfo(), pCreature->GetID());

			DEBUG_ADD_FORMAT("[GCADDOUSTERS] ID = %d", pCreature->GetName());	
			
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

#endif

	__END_CATCH
}
