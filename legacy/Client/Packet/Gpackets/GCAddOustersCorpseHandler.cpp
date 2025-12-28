//--------------------------------------------------------------------------------
//
// Filename    : GCAddOustersCorpseHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddOustersCorpse.h"
#include "ClientDef.h"
#include "MItem.h"


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddOustersCorpseHandler::execute ( GCAddOustersCorpse * pPacket , Player * pPlayer )
throw ( Error )
{
	__BEGIN_TRY
	
	// Creature를 생성해서 MCorpse에 추가해서 Zone에 넣는다.
	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] GCAddOusterCorpseHandler. Zone 이 NULL 이예요.");
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{	
		const PCOustersInfo3 & oi = pPacket->getOustersInfo();
		
		//----------------------------------------	
		// 이미 있는 Creature인가?
		//----------------------------------------	
		MCreature* pCreature = g_pZone->GetCreatureOnly( oi.getObjectID() );
		
		//---------------------------------------------------------
		//					Zone에 없는 경우
		//---------------------------------------------------------
		if (pCreature==NULL)
		{
			// 이미 시체가 있나?
			MItem* pItem = g_pZone->GetItem( oi.getObjectID() );
			
			//---------------------------------------------------------
			// 새로운 시체를 생성
			//---------------------------------------------------------
			if (pItem==NULL)
			{
				//----------------------------------------	
				// 죽은 Creature를 생성한다.
				//----------------------------------------	
				DEBUG_ADD("[GCAddOustersCorpseHandler] New OustersCorpse");				
				MCreatureWear*	pDeadCreature = new MCreatureWear;				
				//add by zdj 2005.6.24
				pDeadCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, oi.getAdvancementLevel() );

				//pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, oi.getAdvancementLevel() );
				pDeadCreature->SetZone( g_pZone );
				
				ItemType_t		coatType	= oi.getCoatType();

				if (oi.getCompetence()==0)
				{
					pDeadCreature->SetCompetence( 0 );
					pDeadCreature->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
				}
				else
				{
					pDeadCreature->SetCreatureType( GetOustersCreatureType( coatType ));
				}
				
				pDeadCreature->SetID( oi.getObjectID() );
				pDeadCreature->SetPosition( oi.getX(), oi.getY() );
				pDeadCreature->SetServerPosition( oi.getX(), oi.getY() );
				pDeadCreature->SetDirection( oi.getDir() );
				pDeadCreature->SetCurrentDirection( oi.getDir() );				
				pDeadCreature->SetStatus( MODIFY_MAX_HP, oi.getMaxHP() );
				pDeadCreature->SetStatus( MODIFY_CURRENT_HP, 0 );				
				// 시체로 바꾼다.
				pDeadCreature->SetCorpse();
				pDeadCreature->SetName( oi.getName().c_str() );				
				// 임시로
				pDeadCreature->SetGuildNumber( oi.getGuildID() );				
				
				pDeadCreature->SetWeaponSpeed( oi.getAttackSpeed() );
				pDeadCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );				
				// 색깔
				pDeadCreature->SetBodyColor1( oi.getHairColor() );
				pDeadCreature->SetBodyColor2( oi.getCoatColor() );

				SetAddonToOusters( pDeadCreature, &oi );				
				
				//----------------------------------------	
				// 시체item을 생성한다.
				//----------------------------------------	
				MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
				
				pCorpse->SetID( oi.getObjectID() );
				pCorpse->SetItemType( 0 );
				pCorpse->SetCreature( pDeadCreature );
				pCorpse->SetPosition( oi.getX(), oi.getY() );				
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
					TYPE_OBJECTID oldItemID = g_pZone->GetItemID( oi.getX(), oi.getY() );
					
					DEBUG_ADD_FORMAT("Remove old Item id=%d", oldItemID);
					
					if (g_pZone->RemoveItem( oldItemID ))				
					{
						// 다시 추가한다.
						if (!g_pZone->AddItem( pCorpse ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't add Corpse to Zone, too. id=%d, xy=(%d, %d)", oi.getObjectID(), oi.getX(), oi.getY());
							
							delete pCorpse;
						}
					}
					else
					{
						// 이미 있는 item을 제거할 수 없는 경우
						DEBUG_ADD_FORMAT("[Error] Can't remove old Item. id=%d, xy=(%d, %d)", oldItemID, oi.getX(), oi.getY());
						
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
			const PCOustersInfo3 & oi = pPacket->getOustersInfo();
		
			ItemType_t coatType = oi.getCoatType();
			
			int creatureType = GetOustersCreatureType( coatType );
			
			if (pCreature->GetCreatureType()!=creatureType)
			{
				if (oi.getCompetence()==0)
				{
					pCreature->SetCompetence( 0 );
					pCreature->SetCreatureType( CREATURETYPE_OUSTERS_OPERATOR );
				}
				else
				{
					pCreature->SetCreatureType(creatureType);
				}
				
				pCreature->SetGroundCreature();
			}			
			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, oi.getAdvancementLevel() );
			pCreature->MovePosition( oi.getX(), oi.getY() );
			pCreature->SetServerPosition( oi.getX(), oi.getY() );
			pCreature->SetDirection( oi.getDir() );
			pCreature->SetCurrentDirection( oi.getDir() );
			pCreature->SetStatus( MODIFY_CURRENT_HP, 0 );
			
			pCreature->SetWeaponSpeed( oi.getAttackSpeed() );
			pCreature->SetStatus( MODIFY_ALIGNMENT, oi.getAlignment() );

			pCreature->SetGuildNumber( oi.getGuildID() );
			
			//---------------------------------------------------------
			// Creature를 죽여야 한다.
			//---------------------------------------------------------
			if (!pCreature->IsDead())
			{
				pCreature->SetStop();			
				pCreature->SetDead();		
			}			
			pCreature->SetItemCount( pPacket->getTreasureCount() );
			SetAddonToOusters( dynamic_cast<MCreatureWear*>(pCreature), &oi );			
		}
	}		
	__END_CATCH
}
