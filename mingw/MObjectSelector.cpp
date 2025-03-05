//----------------------------------------------------------------------
// MObjectSelector.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MObjectSelector.h"
#include "MPlayer.h"
#include "MJusticeAttackManager.h"
#include "CDirectInput.h"
#include "MEventManager.h"
#include "MWarManager.h"
#include "MZone.h"
#include "GuildWarInfo.h"
#include "RaceWarInfo.h"
#include "MZoneTable.h"
#include "MGuildType.h"
#include "UserInformation.h"
//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MObjectSelector* g_pObjectSelector = NULL;

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MObjectSelector::MObjectSelector()
{
	m_SelectTarget	= SELECT_ALL;
	m_SelectBy		= SELECT_BY_RACE;
}

MObjectSelector::~MObjectSelector()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Can Select (MCreature*)
//----------------------------------------------------------------------
BOOL
MObjectSelector::CanSelect(MCreature* pCreature)
{
	// 강제공격이면 아무나 선택 가능..
	// SelectAll이면 아무나 선택가능하다.
	// 할루~걸렸을때..	
	// SelectByRace이면 SelectEnemy이면 (종족에 따라) 공격할 캐릭터만 선택한다.
	//                  SelectFriend이면 (종족에 따라) 공격하지 않을 캐릭터만 선택한다.	
	// SelectByGuild이면  SelectEnemy이면 (길드에 따라) 공격할 캐릭터만 선택한다.
	//                    SelectFriend이면 (길드에 따라) 공격하지 않을 캐릭터만 선택한다.
	// 정당방위가 성립되는 경우
	// 전쟁중일때 상대 길드인경우
	
	WORD CurZoneID = g_pZone->GetID();
	// 2004, 9, 15, sobeit add start
	bool	IsAvailablePK = true;
//	if( g_pUserInformation->IsNonPK && NULL != pCreature )
//	{
//		int spriteType = (*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteType;
//		if((*g_pCreatureSpriteTable)[spriteType].IsPlayerOnlySprite() && !pCreature->IsNPC())
//		{
//			if( g_pPlayer->GetCreatureTribe() != pCreature->GetCreatureTribe() || g_pDXInput->KeyDown(DIK_LSHIFT))
//				IsAvailablePK = false;
//		}
//	}
	// 2004, 9, 15, sobeit add end
	return	(g_pDXInput->KeyDown(DIK_LSHIFT)

			|| g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION)

			|| m_SelectTarget==SELECT_ALL

			|| (m_SelectBy==SELECT_BY_RACE 
					&& (m_SelectTarget==SELECT_ENEMY && g_pPlayer->CanAttackTribe( pCreature )
						|| m_SelectTarget==SELECT_FRIEND && !g_pPlayer->CanAttackTribe( pCreature )
						)
				)

			|| (m_SelectBy==SELECT_BY_GUILD
					&& (m_SelectTarget==SELECT_ENEMY && g_pPlayer->CanAttackGuild( pCreature )
						|| m_SelectTarget==SELECT_FRIEND && !g_pPlayer->CanAttackGuild( pCreature )
						)
				)

			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() )

			|| pCreature->IsNPC()
			|| IsWarEnemy( pCreature )
		//	|| g_pZone->GetPKType() == PK_TYPE_ALL
			|| (g_pPlayer->GetCreatureType() == CREATURETYPE_SLAYER_OPERATOR || g_pPlayer->GetCreatureType() == CREATURETYPE_VAMPIRE_OPERATOR || g_pPlayer->GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR )
			)
			&& IsAvailablePK
			
			;

}

//----------------------------------------------------------------------
// Can Attack (MCreature*)
//----------------------------------------------------------------------
// 종족이나, 길드에 따라서.. 공격할 대상인가?
//----------------------------------------------------------------------
BOOL	
MObjectSelector::CanAttack(MCreature* pCreature)
{
	// 강제 공격이거나..
	// 할루~걸렸을때..
	// SelectByRace이면	CanAttackTribe로 체크
	// SelectByGuild이면 CanAttackGuild로 체크
	// 정당방위가 성립되는 경우
	// NPC는 언제나 선택 가능하다.
	WORD CurZoneID = g_pZone->GetID();

//	bool bFreePKZone = g_pZoneTable->Get( g_pZone->GetID() )->FreePK;
	// 2004, 9, 15, sobeit add start

	bool	IsAvailablePK = true;

	if(NULL == pCreature)return FALSE;
	if( g_pUserInformation->IsNonPK && NULL != pCreature )
	{
		int spriteType = (*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0];
		if((*g_pCreatureSpriteTable)[spriteType].IsPlayerOnlySprite()&& !pCreature->IsNPC())
			IsAvailablePK = false;
	}
	// 2004, 9, 15, sobeit add end
	
	return (
				g_pDXInput->KeyDown(DIK_LSHIFT)
				|| g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION)
				|| m_SelectBy==SELECT_BY_RACE 
						&& g_pPlayer->CanAttackTribe(pCreature)
				|| m_SelectBy==SELECT_BY_GUILD
						&& g_pPlayer->CanAttackGuild(pCreature)
				|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() )
				|| IsWarEnemy( pCreature )
			)

			&& IsAvailablePK;
		//|| g_pZone->GetPKType() == PK_TYPE_ALL;
	
}
//BOOL	
//MObjectSelector::IsAblePKbyZone(MCreature* pCreature)
//{
//	if(pCreature == NULL || g_pZone == NULL)
//		return FALSE;
//	DWORD CreatureType = pCreature->GetCreatureType();
//	if(CreatureType <4 || CreatureType == 651) // 슬레 남녀, 뱀파 남녀, 아우스터즈 일때만
//	{
//		BYTE PkType = g_pZone->GetPKType();
//		if(PkType != PK_TYPE_NULL)	// 그냥 일반 맵
//		{
//			if(PkType == PK_TYPE_DISABLE)		// 다 우리편..-_-; pk 금지
//				return FALSE;
//			else if(PkType == PK_TYPE_ALL)		// 나 빼고 다 적
//				return TRUE;
//			else if(PkType == PK_TYPE_GUILD)	// 우리 길드 빼고 다 적..
//			{
//				int myGuildID = g_pPlayer->GetGuildNumber();	
//				int OtherGuildID = pCreature->GetGuildNumber();	
//				if( myGuildID		== GUILDID_VAMPIRE_DEFAULT ||
//					myGuildID		== GUILDID_SLAYER_DEFAULT ||
//					myGuildID		== GUILDID_OUSTERS_DEFAULT ||
//					OtherGuildID	== GUILDID_VAMPIRE_DEFAULT ||
//					OtherGuildID	== GUILDID_SLAYER_DEFAULT ||
//					OtherGuildID	== GUILDID_OUSTERS_DEFAULT ) // 길드가 없는 녀석 들은..
//					return TRUE; // 혼자 싸워라..-_-;
//
//				if(myGuildID == OtherGuildID)
//					return FALSE; // 우리 길드...
//				else
//					return TRUE;
//			}
//		}
//
//	}
//	return TRUE;
//}
//--------------------------------------------------------------
// IsWarEnemy - 전쟁에 관련된 적인가?
//--------------------------------------------------------------
BOOL	
MObjectSelector::IsWarEnemy(MCreature* pCreature)
{
	BOOL bWarSelect = FALSE;

	if(pCreature == NULL || g_pZone == NULL)
		return bWarSelect;

	// 현재 존이 전쟁 존인가? 전쟁중인 존이 있을때 아담의 성지까지... 전쟁존으로 바꾸어야 한다.
	WORD CurrentZoneID = g_pZone->GetID();
	if(g_pZone!=NULL && pCreature != NULL)
	{
		if(g_pZone->GetPKType() == PK_TYPE_ALL)
			return TRUE;
		else if(g_pZone->GetPKType() == PK_TYPE_SIEGE)
		{
			// 2004, 11, 8, sobeit add start - pCreature의 무결성 확인
			if(pCreature->GetObjectType() != MObject::TYPE_CREATURE || !pCreature->IsAvailableEffectStatus())
				return FALSE;
			// 2004, 11, 8, sobeit add end

			// 수비측 공성 일때
			if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_DEFENDERL)||g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_REINFORCE))
			{
				if(!pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_DEFENDERL)&&!pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_REINFORCE))
					return TRUE;
			}
			if(g_pPlayer->GetUnionGuildID() > 0 && (g_pPlayer->GetUnionGuildID() == pCreature->GetUnionGuildID()))
			{
				return FALSE;
			}
			if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1) && !pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1))
				return TRUE;
			else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_2) && !pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_2))
				return TRUE;
			else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_3) && !pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_3))
				return TRUE;
			else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_4) && !pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_4))
				return TRUE;
			else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_5) && !pCreature->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_5))
				return TRUE;
		}
	}
//	// 2004, 5, 14 sobeit add start
//	DWORD CreatureType = pCreature->GetCreatureType();
//	if(CreatureType <4 || CreatureType == 651) // 슬레 남녀, 뱀파 남녀, 아우스터즈 일때만
//	{
//		BYTE PkType = g_pZone->GetPKType();
//		if(PkType != PK_TYPE_NULL)	// 그냥 일반 맵
//		{
//			if(PkType == PK_TYPE_DISABLE)		// 다 우리편..-_-; pk 금지
//				return FALSE;
//			else if(PkType == PK_TYPE_ALL)		// 나 빼고 다 적
//				return TRUE;
//			else if(PkType == PK_TYPE_GUILD)	// 우리 길드 빼고 다 적..
//			{
//				int myGuildID = g_pPlayer->GetGuildNumber();	
//				int OtherGuildID = pCreature->GetGuildNumber();	
//				if( myGuildID		== GUILDID_VAMPIRE_DEFAULT ||
//					myGuildID		== GUILDID_SLAYER_DEFAULT ||
//					myGuildID		== GUILDID_OUSTERS_DEFAULT ||
//					OtherGuildID	== GUILDID_VAMPIRE_DEFAULT ||
//					OtherGuildID	== GUILDID_SLAYER_DEFAULT ||
//					OtherGuildID	== GUILDID_OUSTERS_DEFAULT ) // 길드가 없는 녀석 들은..
//					return TRUE; // 혼자 싸워라..-_-;
//
//				if(myGuildID == OtherGuildID)
//					return FALSE; // 우리 길드...
//				else
//					return TRUE;
//			}
//		}
//	}
//	// 2004, 5, 14 sobeit add end

	if( g_pWarManager->IsExist( CurrentZoneID ) && g_pWarManager->getSize() > 0)		// 전쟁중이면(아담의성지까지 체크함)
	{
//		if(g_pWarManager->IsHolyLand( CurrentZoneID ) )									// 성내부가 아닌 아담의 성지인경우
//		{																				// 모든 성에 대해서 검색한다.
//			const WarInfoMap&			info = g_pWarManager->getWarInfoList();
//			WarInfoMap::const_iterator	itr = info.begin();
//			bool						creatureGuildCheck = false;						// 포함되어 있지 않을경우 크리쳐길드가 전쟁길드에 포함되어있는지 여부
//
//			while(itr != info.end() && bWarSelect == FALSE)
//			{
//				WarInfo *info = (*itr).second;
//				if(info->getWarType() == WAR_GUILD)												// 길드-_- 끼리...만.. 좀 enum 으로 해서 넘겨주지..
//				{
//					GuildWarInfo *pInfo = (GuildWarInfo *)info;
//
//					int myGuildID = g_pPlayer->GetGuildNumber();			
//					GuildWarInfo::GuildIDList joinGuild = pInfo->getJoinGuilds();
//					list<GuildID_t>::const_iterator	itr = joinGuild.Begin();					
//					
//					for(int i=0;i<joinGuild.getSize();i++,itr++)						// 내 길드가 전쟁 길드에 포함되어 있으면 내길드 외에 다른 길드는 모두 적
//					{				
//						if( *itr == myGuildID )
//						{
//							if( pCreature->GetGuildNumber() != myGuildID )
//								bWarSelect = TRUE;
//							break;
//						}
//						if( *itr == pCreature->GetGuildNumber() )						// 해당 크리쳐 길드아이디가 전쟁 아이디에 속해있는가
//							creatureGuildCheck = true;
//					}					
//				}
//				itr++;
//			}
//			if(bWarSelect == FALSE && creatureGuildCheck == true)				// 길드에 포함되어 있지 않으면 전쟁 길드에 포함된 길드만 적.
//				bWarSelect = TRUE;
//		} else
		{								// 해당 성에 대해서만 검색한다.				
			WarInfo *info = g_pWarManager->GetWarInfo( CurrentZoneID );
			
			if(info == NULL)
				return FALSE;

			GuildWarInfo *pInfo = (GuildWarInfo *)info;

			if(info->getWarType() == WAR_GUILD)// || info->getWarType() == 2)				// 길드-_- 끼리...만.. 좀 enum 으로 해서 넘겨주지..
			{																	// 2번은 나중에... 길드전 기능 추가할때를 대비해서..
				int myGuildID = g_pPlayer->GetGuildNumber();			
				GuildWarInfo::GuildIDList joinGuild = pInfo->getJoinGuilds();
				std::list<GuildID_t>::const_iterator	itr = joinGuild.Begin();
				bool	creatureGuildCheck = false;								// 포함되어 있지 않을경우 크리쳐길드가 전쟁길드에 포함되어있는지 여부
				
				for(int i=0;i<joinGuild.getSize();i++,itr++)					// 내 길드가 전쟁 길드에 포함되어 있으면 내길드 외에 다른 길드는 모두 적
				{				
					if( *itr == myGuildID )
					{
						if( pCreature->GetGuildNumber() != myGuildID )
							bWarSelect = TRUE;
						break;
					}
					if( *itr == pCreature->GetGuildNumber() )
						creatureGuildCheck = true;
				}
				if(bWarSelect == FALSE && creatureGuildCheck == true)				// 길드에 포함되어 있지 않으면 전쟁 길드에 포함된 길드만 적.
					bWarSelect = TRUE;
			}			
		}		
	}
	
	return bWarSelect;
}