//--------------------------------------------------------------------------------
//
// Filename    : GCAddVampireCorpseHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddVampireCorpse.h"
#include "ClientDef.h"
#include "MItem.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddVampireCorpseHandler::execute ( GCAddVampireCorpse * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	// Creature를 생성해서 MCorpse에 추가해서 Zone에 넣는다.
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
		const PCVampireInfo3 & vi = pPacket->getVampireInfo();

		//----------------------------------------	
		// 이미 있는 Creature인가?
		//----------------------------------------	
		MCreature* pCreature = g_pZone->GetCreatureOnly( vi.getObjectID() );
		
		//---------------------------------------------------------
		//
		//					Zone에 없는 경우
		//
		//---------------------------------------------------------
		if (pCreature==NULL)
		{
			// 이미 시체가 있나?
			MItem* pItem = g_pZone->GetItem( vi.getObjectID() );

			//---------------------------------------------------------
			//
			// 새로운 시체를 생성
			//
			//---------------------------------------------------------
			if (pItem==NULL)
			{
				//Color_t getHairColor () const throw () { return m_Colors[VAMPIRE_COLOR_HAIR]; }
				//Color_t getSkinColor () const throw () { return m_Colors[VAMPIRE_COLOR_SKIN]; }
				//Color_t getCoatColor ( ColorType colorType = MAIN_COLOR ) const throw () { return m_Colors[VAMPIRE_COLOR_COAT1 + (int)colorType]; }

				//----------------------------------------	
				//
				// 죽은 Creature를 생성한다.
				//
				//----------------------------------------	
				DEBUG_ADD("New VampireCorpse");
				
				MCreatureWear*	pDeadCreature = new MCreatureWear;
				pDeadCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, vi.getAdvancementLevel() );

				pDeadCreature->SetZone( g_pZone );
				
				// creatureType이 Vamp남자면 2, Vamp여자면 3
				//--------------------------------------------------
				// CreatureType 설정
				//--------------------------------------------------
				Shape_t shape	= vi.getShape();
				bool bMale		= vi.getSex()==MALE;
				ItemType_t coatType = vi.getCoatType();

				if (vi.getCompetence()==0)
				{
					pDeadCreature->SetCompetence( 0 );
					pDeadCreature->SetCreatureType( CREATURETYPE_VAMPIRE_OPERATOR );
					pDeadCreature->SetMale( bMale );
				}
				else
				{
					pDeadCreature->SetCreatureType( GetVampireCreatureType( shape, bMale, coatType ) );
				}

				pDeadCreature->SetMale( bMale );				
				
				// 별 의미 없지만
				if (pDeadCreature->GetCreatureType()==CREATURETYPE_BAT)
				{
					pDeadCreature->SetFlyingCreature();
				}
				else
				{
					pDeadCreature->SetGroundCreature();
				}

				pDeadCreature->SetID( vi.getObjectID() );
				pDeadCreature->SetPosition( vi.getX(), vi.getY() );
				pDeadCreature->SetServerPosition( vi.getX(), vi.getY() );
				pDeadCreature->SetDirection( vi.getDir() );
				pDeadCreature->SetCurrentDirection( vi.getDir() );

				pDeadCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
				pDeadCreature->SetStatus( MODIFY_CURRENT_HP, 0 );
				
				// 시체로 바꾼다.
				pDeadCreature->SetCorpse();
				pDeadCreature->SetName( vi.getName().c_str() );

				// 임시로
				pDeadCreature->SetGuildNumber( vi.getGuildID() );

				pDeadCreature->SetWeaponSpeed( vi.getAttackSpeed() );
				pDeadCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );

				// 색깔
				pDeadCreature->SetBodyColor1( vi.getSkinColor() );
				pDeadCreature->SetBodyColor2( vi.getCoatColor() );
				

				//--------------------------------------------------
				// [ TEST CODE ]
				//--------------------------------------------------
				// 옷 색깔 설정하기
				//--------------------------------------------------
				/*
				if (pDeadCreature->IsMale())
				{
					pDeadCreature->SetBodyColor2( 91 );
				}
				else
				{
					pDeadCreature->SetBodyColor2( 38 );
				}
				*/

				//----------------------------------------	
				//
				// 시체item을 생성한다.
				//
				//----------------------------------------	
				MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
				pCorpse->SetID( vi.getObjectID() );
				pCorpse->SetItemType( 0 );
				pCorpse->SetCreature( pDeadCreature );
				pCorpse->SetPosition( vi.getX(), vi.getY() );		

				pCorpse->SetNumber( pPacket->getTreasureCount() );

				//----------------------------------------
				// Zone에 Item추가
				//----------------------------------------
				if (!g_pZone->AddItem( pCorpse ))
				{
					//---------------------------------------------------------
					// 추가가 안된 경우
					// 이미 있는 Item을 제거하고 다시 추가한다.
					//---------------------------------------------------------
					TYPE_OBJECTID oldItemID = g_pZone->GetItemID( vi.getX(), vi.getY() );

					DEBUG_ADD_FORMAT("Remove old Item id=%d", oldItemID);
						
					if (g_pZone->RemoveItem( oldItemID ))				
					{
						// 다시 추가한다.
						if (!g_pZone->AddItem( pCorpse ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't add Corpse to Zone, too. id=%d, xy=(%d, %d)", vi.getObjectID(), vi.getX(), vi.getY());
							
							delete pCorpse;
						}
					}
					else
					{
						// 이미 있는 item을 제거할 수 없는 경우
						DEBUG_ADD_FORMAT("[Error] Can't remove old Item. id=%d, xy=(%d, %d)", oldItemID, vi.getX(), vi.getY());
						
						delete pCorpse;
					}	
				}	
			}
			//---------------------------------------------------------
			//
			// 이미 시체가 있는 경우 
			//
			//---------------------------------------------------------
			else
			{
				DEBUG_ADD_FORMAT("[Error] Already Exist Another Item : class=%d type=%d", (int)pItem->GetItemClass(), (int)pItem->GetItemType());
				
				if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
				{
					//---------------------------------------------------------					
					// 시체가 이미 있는 경우
					// 시체에 들어있는 Item 개수 설정
					//---------------------------------------------------------
					pItem->SetNumber( pPacket->getTreasureCount() );
				}
				else
				{
					DEBUG_ADD("The Item is Not Corpse");
				}
			}
		}
		//---------------------------------------------------------
		//
		//				Zone에 이미 있는 경우
		//
		//---------------------------------------------------------		
		else
		{
			const PCVampireInfo3 & vi = pPacket->getVampireInfo();

			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, vi.getAdvancementLevel() );

			Shape_t shape	= vi.getShape();
			bool bMale		= vi.getSex()==MALE;
			ItemType_t coatType = vi.getCoatType();

			int creatureType = GetVampireCreatureType( shape, bMale, coatType );

			if (pCreature->GetCreatureType()!=creatureType)
			{
				if (vi.getCompetence()==0)
				{
					pCreature->SetCompetence( 0 );
					pCreature->SetCreatureType( CREATURETYPE_VAMPIRE_OPERATOR );
					pCreature->SetMale( bMale );
				}
				else
				{
					pCreature->SetCreatureType(creatureType);
				}

				pCreature->SetGroundCreature();
			}

			pCreature->MovePosition( vi.getX(), vi.getY() );
			pCreature->SetServerPosition( vi.getX(), vi.getY() );
			pCreature->SetDirection( vi.getDir() );
			pCreature->SetCurrentDirection( vi.getDir() );

		//	pCreature->SetStatus( MODIFY_MAX_HP, vi.getMaxHP() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, 0 );

			pCreature->SetWeaponSpeed( vi.getAttackSpeed() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, vi.getAlignment() );

			// 임시로
			pCreature->SetGuildNumber( vi.getGuildID() );

			//---------------------------------------------------------
			// Creature를 죽여야 한다.
			//---------------------------------------------------------
			if (!pCreature->IsDead())
			{
				pCreature->SetStop();			
				pCreature->SetDead();		
			}

			pCreature->SetItemCount( pPacket->getTreasureCount() );
		}
	}

#endif

	__END_CATCH
}
