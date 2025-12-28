#include "Client_PCH.h"
#include "MWarManager.h"
#include "MZoneTable.h"
#include "UserInformation.h"
#include "UIFunction.h"
#include "GuildWarInfo.h"
#include "RaceWarInfo.h"
#include "LevelWarInfo.h"
#include "MZone.h"
#include "MSkillManager.h"
#include "RarFile.h"
#include "Properties.h"

MWarManager *g_pWarManager = NULL;


MWarManager::MWarManager()
{
//	// 아담의 성지 존 아이디.
//	// 아담의 성지에서는 다른 성 4개의 정보가 같이 적용된다.
//	// 그러므로 WarInfo 를 얻어올때 아담의 성지 존 아이디를 체크하여 4개를 같이 넘겨주어야 한다.
//	// 좀 HardCoding 틱하지만... -_-
//
//	HolyLandZone.clear();
//
//	HolyLandZone.push_back( 71 );
//	HolyLandZone.push_back( 72 );
//	HolyLandZone.push_back( 73 );
	m_WarInfo.clear();	
}


MWarManager::~MWarManager()
{
	ClearWar();
}

// WarList 를 등록해준다. 기존에 있으면 left_time 만 Update 해준다.
void			MWarManager::SetWar(WarInfo *info)
{
	if(info == NULL)
		return;

	std::vector<WAR_INFO>::iterator itr = g_pUserInformation->WarInfo.begin();

	switch(info->getWarType())	// 종족전
	{
	case WAR_RACE:
		{
			// 편지 보내쟈
			UI_RunNotice(6, info->getStartTime());
			UI_DeleteNotice(5);
			
			RaceWarInfo *pInfo = (RaceWarInfo *)info;

			RaceWarInfo::ZoneIDList& IDList = pInfo->getCastleIDs();

			bool bUpdate = false;
			while(!IDList.IsEmpty())
			{
				ZoneID_t id = IDList.popValue();

				m_WarInfo[id] = pInfo;

				while(itr != g_pUserInformation->WarInfo.end())
				{
					if(itr->zone_id == id && itr->war_type == WAR_RACE)		// 기존에 있으면 업데이트!
					{
						itr->left_time = pInfo->getRemainTime() + (timeGetTime()/1000);
						bUpdate = true;
						break;
					}
					itr++;
				}

				if(bUpdate == false)
				{
					WAR_INFO inf;	
					inf.left_time = pInfo->getRemainTime() + (timeGetTime()/1000);
					inf.zone_id = id;
					inf.zone_name = g_pZoneTable->Get(id)->Name;
					inf.war_type = pInfo->getWarType();
			
					g_pUserInformation->WarInfo.push_back(inf);

					itr = g_pUserInformation->WarInfo.begin();
				}
				else
					bUpdate = false;
			}

			if(g_pZone != NULL )
			{
				if(g_pZoneTable->Get(g_pZone->GetID())->HolyLand)
				{
					UI_RunBloodBibleStatus();
					
					//--------------------------------------------------
					// 홀리랜드 보너스 RaceWar 시작할때 reset
					//--------------------------------------------------
					for(int i = 0; i < HOLYLAND_BONUS_MAX; i++)
					{
						g_abHolyLandBonusSkills[i] = false;
					}
					
					//--------------------------------------------------
					// 현재 사용 가능한 skill들을 다시 체크한다.
					//--------------------------------------------------
					g_pSkillAvailable->SetAvailableSkills();
				}
				else
					UI_CloseBloodBibleStatus();
			}
			
			// 2004, 10, 25, sobeit add start
			UI_RunRangerChat();
			// 2004, 10, 25, sobeit add end
		}
		break;

	case WAR_GUILD:
		{
			GuildWarInfo *pInfo = (GuildWarInfo *)info;

			m_WarInfo[pInfo->getCastleID()] = pInfo;
		
			while(itr != g_pUserInformation->WarInfo.end())
			{
				if(itr->zone_id == pInfo->getCastleID() && itr->war_type == WAR_GUILD)		// 기존에 있으면 업데이트!
				{
					itr->left_time = pInfo->getRemainTime() + (timeGetTime()/1000);
					return;
				}
				itr++;
			}

			WAR_INFO inf;	
			inf.left_time = pInfo->getRemainTime() + (timeGetTime()/1000);
			inf.zone_id = pInfo->getCastleID();
			inf.zone_name = g_pZoneTable->Get(pInfo->getCastleID())->Name;
			inf.war_type = pInfo->getWarType();
			inf.attack_guild_name = pInfo->getAttackGuildName();
			inf.defense_guild_name = pInfo->getDefenseGuildName();

			g_pUserInformation->WarInfo.push_back(inf);
		}
		break;

	case WAR_LEVEL:
		{
			LevelWarInfo *pInfo = (LevelWarInfo *)info;
			
			WAR_INFO inf;	
			inf.zone_id = g_pZone->GetID();
			inf.zone_name = g_pZoneTable->Get(inf.zone_id)->Name;
			inf.left_time = pInfo->getRemainTime() + (timeGetTime()/1000);
			inf.war_type = pInfo->getWarType();
			
			g_pUserInformation->WarInfo.push_back(inf);
		}
		break;
	}
	
}


// 해당 존의 전쟁 정보를 지워준다.
void			MWarManager::RemoveWar(ZoneID_t id)
{
	WarInfoMapItr	itr = m_WarInfo.find(id);
	
	if(itr != m_WarInfo.end())
	{
		RaceWarInfo *pInfo = dynamic_cast<RaceWarInfo*>(itr->second);
		if( pInfo != NULL )
		{
			delete pInfo;
			pInfo = NULL;
		}

		m_WarInfo.erase(itr);
	}

	std::vector<WAR_INFO>::iterator itrv = g_pUserInformation->WarInfo.begin();

	while(itrv != g_pUserInformation->WarInfo.end() )
	{
		if( itrv->zone_id == id)
		{
			g_pUserInformation->WarInfo.erase(itrv);
			break;
		}
		itrv++;
	}
}

WarInfo*		MWarManager::GetWarInfo(ZoneID_t id)
{
	switch(id)
	{
	case 1211:
	case 1212:
		id = 1201;
		break;

	case 1221:
	case 1222:
		id = 1202;
		break;

	case 1231:
	case 1232:
		id = 1203;
		break;

	case 1241:
	case 1242:
		id = 1204;
		break;

	case 1251 :
	case 1252 :
		id = 1205;
		break;

	case 1261 :
	case 1262 :
		id = 1206;
		break;

	}

	WarInfoMapItr	itr = m_WarInfo.find(id);

	if(itr != m_WarInfo.end())
		return (*itr).second;

	return NULL;
}

void			MWarManager::ClearWar()
{
	WarInfoMapItr	itr = m_WarInfo.begin();
	std::map<WarInfo*, bool>		deleteHistory;

	while( itr != m_WarInfo.end() )
	{
		WarInfo *pInfo = itr->second;
		
		if( pInfo != NULL && deleteHistory[pInfo] == false )
		{
			deleteHistory[pInfo] = true;
			delete pInfo;			
		}		
		
		itr++;
	}
	m_WarInfo.clear();
	if(g_pUserInformation != NULL)
		g_pUserInformation->WarInfo.clear();
}

void			MWarManager::ClearRaceWar()
{
	WarInfoMapItr	itr = m_WarInfo.begin();

	while( itr != m_WarInfo.end() )
	{
		if( (*itr).second != NULL && (*itr).second->getWarType() == WAR_RACE)
		{
			WarInfoMapItr deleteItr = itr;
			itr++;

			m_WarInfo.erase(deleteItr);
		}
		else
			itr++;
	}
//	m_WarInfo.clear();

	std::vector<WAR_INFO>::iterator itr2 = g_pUserInformation->WarInfo.begin();
	while( itr2 != g_pUserInformation->WarInfo.end())
	{
		if(itr2->war_type == WAR_RACE)
		{
			g_pUserInformation->WarInfo.erase(itr2);
			itr2 = g_pUserInformation->WarInfo.begin();
		}
		else
			itr2++;
//		g_pUserInformation->WarInfo.clear();
	}

	UI_SetBloodBibleStatusTimer(30);
//	UI_CloseBloodBibleStatus();
	// 2004, 10, 25, sobeit add start
	UI_CloseRangerChat();
	// 2004, 10, 25, sobeit add end
}

bool			MWarManager::IsExist(ZoneID_t id)
{
	switch(id)
	{
	case 1211:
	case 1212:
		id = 1201;
		break;

	case 1221:
	case 1222:
		id = 1202;
		break;

	case 1231:
	case 1232:
		id = 1203;
		break;

	case 1241:
	case 1242:
		id = 1204;
		break;

	case 1251 :
	case 1252 :
		id = 1205;
		break;

	case 1261 :
	case 1262 :
		id = 1206;
		break;

	}

	WarInfoMapItr	itr = m_WarInfo.find(id);

	if(itr == NULL || itr == m_WarInfo.end())
	{
//		if(!m_WarInfo.empty())
//			return IsHolyLand( id );
//		
		return false;
	}

	return true;
}

//// 아담의 성지 필드맵인가?
//bool			MWarManager::IsHolyLand(ZoneID_t id)
//{
//	list<ZoneID_t>::const_iterator itr = HolyLandZone.begin();
//	
//	while(itr != HolyLandZone.end() )
//	{
//		if( *itr == id )
//			return true;
//		itr++;
//	}	
//
//	return false;
//}
