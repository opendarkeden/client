//--------------------------------------------------------------------------------
//
// Filename    : GCAddSlayerCorpseHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddSlayerCorpse.h"
#include "ClientDef.h"
#include "MItem.h"
#include "AddonDef.h"
#include "DrawTypeDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddSlayerCorpseHandler::execute ( GCAddSlayerCorpse * pPacket , Player * pPlayer )
	 throw ( Error )
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
		const PCSlayerInfo3 & si = pPacket->getSlayerInfo();	
			
		//----------------------------------------	
		// 이미 있는 Creature인가?
		//----------------------------------------	
		MCreature* pCreature = g_pZone->GetCreatureOnly( si.getObjectID() );
		
		//---------------------------------------------------------
		//
		//					Zone에 없는 경우
		//
		//---------------------------------------------------------
		if (pCreature==NULL)
		{
			// 이미 시체가 있나?
			MItem* pItem = g_pZone->GetItem( si.getObjectID() );

			//---------------------------------------------------------
			//
			// 새로운 시체를 생성
			//
			//---------------------------------------------------------
			if (pItem==NULL)
			{
				DEBUG_ADD("New SlayerCorpse");
				
				//----------------------------------------	
				//
				// 죽은 Creature를 생성한다.
				//
				//----------------------------------------	
				MCreatureWear*	pDeadCreature = new MCreatureWear;
				
				pDeadCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, si.getAdvancementLevel() );

				pDeadCreature->SetZone( g_pZone );
				
				//----------------------------------------	
				// 복장을 착용한다.
				//----------------------------------------					
				if (si.getCompetence()==0)
				{
					pDeadCreature->SetCompetence( 0 );
					pDeadCreature->SetCreatureType( CREATURETYPE_SLAYER_OPERATOR );
					pDeadCreature->SetMale( si.getSex()==MALE );
				}
				else
				{
					pDeadCreature->SetCreatureType( (si.getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );
				}

				pDeadCreature->SetGuildNumber( si.getGuildID() );

				// creatureType이 Slayer남자면 0, Slayer여자면 1 
				pDeadCreature->SetGroundCreature();
				pDeadCreature->SetID( si.getObjectID() );
				pDeadCreature->SetPosition( si.getX(), si.getY() );
				pDeadCreature->SetServerPosition( si.getX(), si.getY() );
				pDeadCreature->SetDirection( si.getDir() );
				pDeadCreature->SetCurrentDirection( si.getDir() );

				// 피부색
				pDeadCreature->SetBodyColor1( si.getSkinColor() );

				pDeadCreature->SetStatus( MODIFY_MAX_HP, si.getMaxHP() );
				pDeadCreature->SetStatus( MODIFY_CURRENT_HP, 0 );			
				
				SetAddonToSlayer( pDeadCreature, &si );	

				// 시체로 바꾼다.
				pDeadCreature->SetCorpse();
				pDeadCreature->SetName( si.getName().c_str() );				

				//----------------------------------------	
				//
				// 시체item을 생성한다.
				//
				//----------------------------------------	
				MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
				pCorpse->SetID( si.getObjectID() );
				pCorpse->SetItemType( 0 );
				pCorpse->SetCreature( pDeadCreature );
				pCorpse->SetPosition( si.getX(), si.getY() );	
				
				pCorpse->SetNumber( pPacket->getTreasureCount() );

				//----------------------------------------
				// Zone에 Item추가
				//----------------------------------------
				if (!g_pZone->AddItem( pCorpse ))
				{
					DEBUG_ADD("[Error] Can't Add to Zone.");
					
					//---------------------------------------------------------
					// 추가가 안된 경우
					// 이미 있는 Item을 제거하고 다시 추가한다.
					//---------------------------------------------------------
					TYPE_OBJECTID oldItemID = g_pZone->GetItemID( si.getX(), si.getY() );

					DEBUG_ADD_FORMAT("Remove old Item id=%d", oldItemID);
						
					if (g_pZone->RemoveItem( oldItemID ))				
					{
						// 다시 추가한다.
						if (!g_pZone->AddItem( pCorpse ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't add Corpse to Zone, too. id=%d, xy=(%d, %d)", si.getObjectID(), si.getX(), si.getY());
							
							delete pCorpse;
						}
					}
					else
					{
						// 이미 있는 item을 제거할 수 없는 경우
						DEBUG_ADD_FORMAT("[Error] Can't remove old Item. id=%d, xy=(%d, %d)", oldItemID, si.getX(), si.getY());
						
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
			DEBUG_ADD_FORMAT("Already Exist Creature. Set Dead id=%d", si.getObjectID());

			const PCSlayerInfo3 & si = pPacket->getSlayerInfo();

			pCreature->SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, si.getAdvancementLevel() );

			int creatureType = ((si.getSex()==MALE)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE);

			if (pCreature->GetCreatureType()!=creatureType)
			{
				if (si.getCompetence()==0)
				{
					pCreature->SetCompetence( 0 );
					pCreature->SetCreatureType( CREATURETYPE_SLAYER_OPERATOR );
					pCreature->SetMale( si.getSex()==MALE );
				}
				else
				{
					pCreature->SetCreatureType(creatureType);
				}

				pCreature->SetGroundCreature();				
			}

			pCreature->SetGuildNumber( si.getGuildID() );
			
			pCreature->MovePosition( si.getX(), si.getY() );
			pCreature->SetServerPosition( si.getX(), si.getY() );
			pCreature->SetDirection( si.getDir() );
			pCreature->SetCurrentDirection( si.getDir() );			
			
			//pCreature->SetStatus( MODIFY_MAX_HP, 100 );
			pCreature->SetStatus( MODIFY_CURRENT_HP, 0 );

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
