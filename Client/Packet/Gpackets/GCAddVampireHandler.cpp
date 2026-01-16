//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAddVampireHandler.cc
// Written By  : Reiot
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddVampire.h"
#include "ClientDef.h"
#include "MCreatureTable.h"
#include "MTestDef.h"
#include "MGameStringTable.h"
extern void SetPetInfo(PetInfo* pPetInfo, TYPE_OBJECTID objectID);
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAddVampireHandler::execute ( GCAddVampire * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
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
		const PCVampireInfo3 & vi = pPacket->getVampireInfo ();

		MCreature* pCreature = g_pZone->GetCreature(vi.getObjectID());

		//--------------------------------------------------
		// 새로운 Creature이면 추가
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			pCreature = new MCreatureWear;
			
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, vi.getAdvancementLevel() );
			pCreature->SetZone( g_pZone );

			pCreature->SetName( vi.getName().c_str() );

			//--------------------------------------------------
			// CreatureType 설정
			//--------------------------------------------------
			Shape_t shape	= vi.getShape();
			bool bMale		= vi.getSex()==MALE;
			ItemType_t coatType = vi.getCoatType();

			if (vi.getCompetence()==0)
			{
				pCreature->SetCompetence( 0 );
				pCreature->SetCreatureType( CREATURETYPE_VAMPIRE_OPERATOR );
				pCreature->SetMale( bMale );
			}
			else
			{
				pCreature->SetCreatureType( GetVampireCreatureType( shape, bMale, coatType ) );
			}

			pCreature->SetMale( bMale );

			if (pCreature->GetCreatureType()==CREATURETYPE_BAT)
			{
				pCreature->SetFlyingCreature();
			}
			else
			{
				pCreature->SetGroundCreature();
			}

			pCreature->SetID(vi.getObjectID());
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->SetPosition( vi.getX(), vi.getY() );
			pCreature->SetServerPosition( vi.getX(), vi.getY() );
			pCreature->SetDirection( vi.getDir() );
			pCreature->SetCurrentDirection( vi.getDir() );
			pCreature->SetAction( ACTION_STAND );
 
			// 색깔
			
			if( pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF )
			{
				pCreature->SetBodyColor1( vi.getCoatColor() );
				pCreature->SetBodyColor2( vi.getCoatColor() );
			} else
			{
				pCreature->SetBodyColor1( vi.getSkinColor() );
				pCreature->SetBodyColor2( vi.getCoatColor() );
			}
			// add by sonic 2006.10.29
			pCreature->SetMasterEffectType( vi.getMasterEffectColor() );
			// 2006.10.29 sonic add end
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

			pCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, vi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );
			pCreature->SetStatus( MODIFY_RANK, vi.getRank() );

			//vi.getName()
			// 색상 정보

			pCreature->SetWeaponSpeed( vi.getAttackSpeed() );
			
			// 임시로
			pCreature->SetGuildNumber( vi.getGuildID() );
			pCreature->SetUnionGuildID( vi.getUnionID() );
			if( vi.getBatColor() != 0 )
				pCreature->SetBatColor( vi.getBatColor() );
			else
				pCreature->SetBatColor( 0xFFFF );
			
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());
			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsVampire()== true && pCreature->IsNPC() == false)
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
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, vi.getAdvancementLevel() );
			//--------------------------------------------------
			// CreatureType 설정
			//--------------------------------------------------
			Shape_t shape	= vi.getShape();
			bool bMale		= vi.getSex()==MALE;
			ItemType_t coatType = vi.getCoatType();

			if (vi.getCompetence()==0)
			{
				pCreature->SetCompetence( 0 );
				pCreature->SetCreatureType( CREATURETYPE_VAMPIRE_OPERATOR );
				pCreature->SetMale( bMale );
			}
			else
			{
				pCreature->SetCreatureType( GetVampireCreatureType( shape, bMale, coatType ) );
			}

			pCreature->SetMale( bMale );

			if (pCreature->GetCreatureType()==CREATURETYPE_BAT)
			{
				pCreature->SetFlyingCreature();
			}
			else
			{
				pCreature->SetGroundCreature();
			}
			//pCreature->SetGroundCreature();

			// 임시로
			pCreature->SetGuildNumber( vi.getGuildID() );
			pCreature->SetUnionGuildID( vi.getUnionID() );
			
			//pCreature->SetAction(ACTION_MOVE);
			pCreature->MovePosition( vi.getX(), vi.getY() );
			pCreature->SetServerPosition( vi.getX(), vi.getY() );			
			pCreature->SetDirection( vi.getDir() );
			pCreature->SetCurrentDirection( vi.getDir() );
			pCreature->SetAction( ACTION_STAND );

			// 색깔
			if( pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF )
			{
				pCreature->SetBodyColor1( vi.getCoatColor() );
				pCreature->SetBodyColor2( vi.getCoatColor() );
			} else
			{
				pCreature->SetBodyColor1( vi.getSkinColor() );
				pCreature->SetBodyColor2( vi.getCoatColor() );
			}

			// add by sonic 2006.10.29
			pCreature->SetMasterEffectType( vi.getMasterEffectColor() );
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

			pCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, vi.getCurrentHP() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );
			pCreature->SetStatus( MODIFY_RANK, vi.getRank() );
			
			pCreature->SetWeaponSpeed( vi.getAttackSpeed() );
			if( vi.getBatColor() != 0 )
				pCreature->SetBatColor( vi.getBatColor() );
			else
				pCreature->SetBatColor( 0xFFFF );
			if(pCreature->IsNPC() == false)
				pCreature->SetPersnalShop(pPacket->getStoreOutlook().isOpen());
			
			if(pPacket->getStoreOutlook().isOpen()&& g_pPlayer->IsVampire()== true && pCreature->IsNPC() == false)
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
			
			//--------------------------------------------------
			// 정상일 경우에는 0, 포탈을 통했을 경우에는 1
			//--------------------------------------------------
			if (pPacket->getFromFlag()==1)
			{
				ComeFromPortal( pCreature );
			}
			
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

	// [도움말] Vampire가 나타날때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_CREATURE_APPEAR_VAMPIRE );
//	__END_HELP_EVENT

	__END_CATCH
}
