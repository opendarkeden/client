//--------------------------------------------------------------------------------
//
// Filename    : GCAddMonsterCorpseHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCAddMonsterCorpse.h"
#include "ClientDef.h"
#include "MItem.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddMonsterCorpseHandler::execute ( GCAddMonsterCorpse * pPacket , Player * pPlayer )
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
		//----------------------------------------	
		// 이미 있는 Creature인가?
		//----------------------------------------	
		MCreature* pCreature = g_pZone->GetCreatureOnly( pPacket->getObjectID() );
		
		static bool IsIKilled = false;
		
		// 2005, 1, 18 , sobeit add start - 슬레, 뱀파, 아우 모습의 몬스터들은 시체 무시(뱀파는 상관 없는데..암튼..)
		// gcaddslayercorps나 gcaddousterscorps가 날라와야 안 팅기는데...암튼...이런 저런 이유로 무시..
		int MonsterType = pPacket->getMonsterType();
		if(MonsterType == 735 || MonsterType == 736 ||MonsterType == 737 || (MonsterType>= 792 && MonsterType<=800))
			return ;
		// 2005, 1, 18 , sobeit add end
		//---------------------------------------------------------
		//
		//					Zone에 없는 경우
		//
		//---------------------------------------------------------
		if (pCreature==NULL)
		{
			// 이미 시체가 있나?
			MItem* pItem = g_pZone->GetItem( pPacket->getObjectID() );

			//---------------------------------------------------------
			//
			// 새로운 시체를 생성
			//
			//---------------------------------------------------------
			if (pItem==NULL)
			{
				DEBUG_ADD("New MonsterCorpse");
				
				//----------------------------------------	
				//
				// 죽은 Creature를 생성한다.
				//
				//----------------------------------------	
				MCreature*	pCreature = new MCreature;

				pCreature->SetZone( g_pZone );
				
				pCreature->SetCreatureType( pPacket->getMonsterType() );
				pCreature->SetGroundCreature();
				pCreature->SetID( pPacket->getObjectID() );
				pCreature->SetPosition( pPacket->getX(), pPacket->getY() );
				pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
				pCreature->SetDirection( pPacket->getDir() );
				pCreature->SetCurrentDirection( pPacket->getDir() );
				
				pCreature->SetStatus( MODIFY_MAX_HP, 100 );
				pCreature->SetStatus( MODIFY_CURRENT_HP, 0 );

				// 시체로 바꾼다.
				pCreature->SetCorpse();
				//pCreature->SetName( (*g_pCreatureTable)[pPacket->getMonsterType()].Name.GetString() );
				pCreature->SetName( pPacket->getMonsterName().c_str() );
				pCreature->SetDrainCreatureID( pPacket->getLastKiller() );

				// 2004, 5, 7 sobeit add start - 내가 죽인 몬스터 일 때 - 흡혈 도움말
				if(pPacket->getLastKiller() == g_pPlayer->GetID() && !IsIKilled)
				{
					ExecuteHelpEvent(HELP_EVENT_KILL);
					IsIKilled = true;
				}
				// 2004, 5, 7 sobeit add end

				// 임시로..
				pCreature->SetGuildNumber( 1 );				

				
				// 머리가 없으면 -_-;
				if (!pPacket->gethasHead())
				{
					pCreature->RemoveHead();
				}

				//----------------------------------------	
				//
				// 시체item을 생성한다.
				//
				//----------------------------------------	
				MCorpse* pCorpse = (MCorpse*)MItem::NewItem( ITEM_CLASS_CORPSE );
				pCorpse->SetID( pPacket->getObjectID() );
				pCorpse->SetItemType( 0 );
				pCorpse->SetCreature( pCreature );
				pCorpse->SetPosition( pPacket->getX(), pPacket->getY() );		

				//---------------------------------------------------------
				// 시체에 들어있는 Item 개수 설정
				//---------------------------------------------------------
				pCorpse->SetNumber( pPacket->getTreasureCount() );


				//----------------------------------------
				// Zone에 Item추가
				//----------------------------------------
				if (g_pZone->AddItem( pCorpse ))
				{
					//------------------------------------------------------------
					// Load되지 않았으면 load한다.
					//------------------------------------------------------------
//					LoadCreatureType( pPacket->getMonsterType() );			
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] Can't add Corpse to Zone. id=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
					
					//---------------------------------------------------------
					// 추가가 안된 경우
					// 이미 있는 Item을 제거하고 다시 추가한다.
					//---------------------------------------------------------
					TYPE_OBJECTID oldItemID = g_pZone->GetItemID( pPacket->getX(), pPacket->getY() );

					DEBUG_ADD_FORMAT("Remove old Item id=%d", oldItemID);
						
					if (g_pZone->RemoveItem( oldItemID ))				
					{
						// 다시 추가한다.
						if (!g_pZone->AddItem( pCorpse ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't add Corpse to Zone, too. id=%d, xy=(%d, %d)", pPacket->getObjectID(), pPacket->getX(), pPacket->getY());
							
							delete pCorpse;
						}
					}
					else
					{
						// 이미 있는 item을 제거할 수 없는 경우
						DEBUG_ADD_FORMAT("[Error] Can't remove old Item. id=%d, xy=(%d, %d)", oldItemID, pPacket->getX(), pPacket->getY());
						
						delete pCorpse;
					}								
				}
				
				// 크리스 마스 트리는 이펙트 붙인다.
				if(pCreature->GetCreatureType() == 482 || pCreature->GetCreatureType() == 650)
				{
					if(pCreature->GetCreatureType() == 482)
						pCreature->AddEffectStatus(EFFECTSTATUS_CLIENTONLY_XMAS_TREE, 0xFFFF);
					pCreature->SetChatString((char *)pCreature->GetName());
				}
			}
			//---------------------------------------------------------
			//
			// 이미 시체가 있는 경우 
			//
			//---------------------------------------------------------
			else
			{
				#ifdef	OUTPUT_DEBUG
					if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
					{
						// 시체가 이미 있는 경우
						DEBUG_ADD("[Collide] Already Exist the Corpse");
					}					
				#endif								

				//---------------------------------------------------------
				// 시체에 들어있는 Item 개수 설정
				//---------------------------------------------------------
				pItem->SetNumber( pPacket->getTreasureCount() );
			}

		}
		//---------------------------------------------------------
		//
		//				Zone에 이미 있는 경우
		//
		//---------------------------------------------------------		
		else
		{
			DEBUG_ADD("[Collide] exist Creature");

			if (pCreature->GetCreatureType()!=pPacket->getMonsterType())
			{
				pCreature->SetCreatureType(pPacket->getMonsterType());
				pCreature->SetGroundCreature();
			}

			pCreature->MovePosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetServerPosition( pPacket->getX(), pPacket->getY() );
			pCreature->SetDirection( pPacket->getDir() );
			pCreature->SetCurrentDirection( pPacket->getDir() );

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

			// 임시로..
			pCreature->SetGuildNumber( 1 );
			pCreature->SetDrainCreatureID( pPacket->getLastKiller() );

			// 2004, 5, 7 sobeit add start - 내가 죽인 몬스터 일 때 - 흡혈 도움말
			if(pPacket->getLastKiller() == g_pPlayer->GetID() && !IsIKilled)
			{
				ExecuteHelpEvent(HELP_EVENT_KILL);
				IsIKilled = true;
			}
			// 2004, 5, 7 sobeit add end
			// 머리가 없으면 -_-;
			if (!pPacket->gethasHead())
			{
				pCreature->RemoveHead();
			}
		}

	}

	// [도움말] 시체 생길 때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_ITEM_APPEAR_CORPSE );	
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
