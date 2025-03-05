//----------------------------------------------------------------------
// MParty.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MParty.h"

#ifdef __GAME_CLIENT__
	#include "MZone.h"
	#include "MCreature.h"
	#include "ClientConfig.h"

	extern DWORD	g_CurrentTime;
#endif

#define	MAX_PARTY_MEMBER	6

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MParty*	g_pParty = NULL;

//----------------------------------------------------------------------
// PARTY_INFO
//----------------------------------------------------------------------
PARTY_INFO::PARTY_INFO()	
{ 
	ID = OBJECTID_NULL; 
	bMale = true;
	hairStyle = 0;
	guildID = 0;
	
	zoneID = 0;
	zoneX = 0;
	zoneY = 0;

	MaxHP = HP = 100;
	
	bInSight = false;
}

//----------------------------------------------------------------------
//
// MParty
//
//----------------------------------------------------------------------
MParty::MParty()
{
	m_pInfo.reserve( MAX_PARTY_MEMBER );

	m_bAccept = true;
	m_JoinTime = 0xFFFFFFFF;
}

MParty::~MParty()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MParty::Release()
{
#ifdef __GAME_CLIENT__
	PARTY_VECTOR::iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL)
		{
			delete pInfo;
		}

		iInfo ++;
	}

	m_pInfo.clear();

	//m_JoinTime = 0xFFFFFFFF;
#endif
}

//----------------------------------------------------------------------
// UnSetPlayerParty
//----------------------------------------------------------------------
void
MParty::UnSetPlayerParty() const
{
#ifdef __GAME_CLIENT__
	PARTY_VECTOR::const_iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL)
		{
			// party설정을 없앤다.
			if (g_pZone!=NULL)
			{
				MCreature* pCreature = g_pZone->GetCreature( g_pZone->GetCreatureID( pInfo->Name.GetString(), 1 ) );

				if (pCreature!=NULL)
				{
					pCreature->UnSetPlayerParty();
				}
			}
		}

		iInfo ++;
	}
#endif
}

//----------------------------------------------------------------------
// Add Member
//----------------------------------------------------------------------
bool		
MParty::AddMember(PARTY_INFO* pInfo)
{
	// 값이 제대로 설정 안된 경우
	if (pInfo==NULL
		|| pInfo->Name.GetString()==NULL && pInfo->ID==OBJECTID_NULL)
	{
		return false;
	}

	// party 꽉 찼당..
	if (GetSize() >= m_pInfo.capacity())
	{
		return false;
	}

	#ifdef __GAME_CLIENT__
		
		// 이름이나 ID가 값이 설정 안된 경우.. 값 넣어주기
		if (pInfo->Name.GetString()==NULL)
		{		
			MCreature* pCreature = g_pZone->GetCreature( pInfo->ID );

			if (pCreature==NULL)
			{
				return false;
			}

			pInfo->Name = pCreature->GetName();	
			pCreature->SetPlayerParty();
		}
		else if (pInfo->ID==OBJECTID_NULL)
		{			
			pInfo->ID = g_pZone->GetCreatureID( pInfo->Name.GetString(), 1 );

			MCreature* pCreature = g_pZone->GetCreature( pInfo->ID );

			if (pCreature!=NULL)
			{
				pCreature->SetPlayerParty();
			}			
		}
		
	#endif

	m_pInfo.push_back( pInfo );

	return true;
}

//----------------------------------------------------------------------
// Remove Member
//----------------------------------------------------------------------
bool		
MParty::RemoveMember(const char* pName)
{
#ifdef __GAME_CLIENT__
	PARTY_VECTOR::iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL
			&& pInfo->Name==pName)
		{
			MCreature* pCreature = g_pZone->GetCreature( g_pZone->GetCreatureID( pName, 1 ) );

			if (pCreature!=NULL)
			{
				pCreature->UnSetPlayerParty();
			}

			delete pInfo;
			pInfo = NULL;
			
			m_pInfo.erase( iInfo );

			return true;
		}	
		
		iInfo ++;
	}
#endif
	return false;
}

//----------------------------------------------------------------------
// Remove Member
//----------------------------------------------------------------------
bool		
MParty::RemoveMember(int creatureID)
{
#ifdef __GAME_CLIENT__
	PARTY_VECTOR::iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL 
			&& pInfo->ID==creatureID)
		{
			MCreature* pCreature = g_pZone->GetCreature( creatureID );

			if (pCreature!=NULL)
			{
				pCreature->UnSetPlayerParty();
			}

			delete pInfo;
			pInfo = NULL;

			m_pInfo.erase( iInfo );

			return true;
		}

		iInfo ++;
	}
#endif
	return false;
}

//----------------------------------------------------------------------
// Get MemberInfo
//----------------------------------------------------------------------
PARTY_INFO*	
MParty::GetMemberInfo(int n) const
{
	if (n>=0 && n < m_pInfo.capacity())
	{
		return m_pInfo[n];
	}

	return NULL;
}

//----------------------------------------------------------------------
// Get Member
//----------------------------------------------------------------------
PARTY_INFO*	
MParty::GetMemberInfo(const char* pName) const
{
	if (pName==NULL || pName[0]==NULL)
	{
		return NULL;
	}

	PARTY_VECTOR::const_iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL && pInfo->Name==pName)
		{
			return pInfo;
		}

		iInfo ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Get MemberInfoByIP
//----------------------------------------------------------------------
PARTY_INFO*	
MParty::GetMemberInfoByIP(const char* pIP) const
{
	if (pIP==NULL || pIP[0]==NULL)
	{
		return NULL;
	}

	PARTY_VECTOR::const_iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL && pInfo->IP==pIP)
		{
			return pInfo;
		}

		iInfo ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Has Member
//----------------------------------------------------------------------
bool		
MParty::HasMember(const char* pName) const
{
	if (pName==NULL || pName[0]==NULL)
	{
		return false;
	}

	PARTY_VECTOR::const_iterator iInfo = m_pInfo.begin();

	while (iInfo != m_pInfo.end())
	{
		PARTY_INFO* pInfo = *iInfo;

		if (pInfo!=NULL && pInfo->Name==pName)
		{
			return true;
		}

		iInfo ++;
	}

	return false;
}

//----------------------------------------------------------------------
// Set JoinTime
//----------------------------------------------------------------------
void		
MParty::SetJoinTime()
{
#ifdef __GAME_CLIENT__
	m_JoinTime = g_CurrentTime;
#endif
}

//----------------------------------------------------------------------
// Is Kick AvailableTime
//----------------------------------------------------------------------
bool		
MParty::IsKickAvailableTime() const
{
#ifdef __GAME_CLIENT__
	return m_JoinTime + g_pClientConfig->AFTER_PARTY_KICK_DELAY < g_CurrentTime;
#else
	return true;
#endif
}