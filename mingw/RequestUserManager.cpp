//----------------------------------------------------------------------
// RequestUserManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "RequestUserManager.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
RequestUserManager*		g_pRequestUserManager = NULL;

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
RequestUserManager::RequestUserManager()
{
	InitializeCriticalSection(&m_Lock);
}

RequestUserManager::~RequestUserManager()
{
	Release();

	DeleteCriticalSection(&m_Lock);
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
RequestUserManager::Release()
{
	REQUEST_USER_MAP::iterator iUser = m_RequestUsers.begin();

	while (iUser != m_RequestUsers.end())
	{
		RequestUserInfo* pUser = iUser->second;

		delete pUser;

		iUser ++;
	}

	m_RequestUsers.clear();

	m_RequestingUsers.clear();
}

//----------------------------------------------------------------------
// Has RequestUser
//----------------------------------------------------------------------
bool
RequestUserManager::HasRequestUser(const char* pName) const
{
	REQUEST_USER_MAP::const_iterator iUser = m_RequestUsers.find( std::string(pName) );

	if (iUser!=m_RequestUsers.end())
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Add RequestUser
//----------------------------------------------------------------------
// 이미 있다면.. ip를 바꿔준다.. 
// 이미 접속중이라면 접속해제..
//----------------------------------------------------------------------
void
RequestUserManager::AddRequestUser(const char* pName, const char* pIP, int UDPPort)
{
	RequestUserInfo* pUser = GetUserInfo( pName );

	if (pUser==NULL)
	{		
		// 없다면 정보 생성.
		pUser = new RequestUserInfo;

		pUser->Name = pName;
		pUser->IP	= pIP;
		pUser->UDPPort	= UDPPort;

		m_RequestUsers[pUser->Name] = pUser;
	}
	else
	{
		// IP가 달라진 경우..
		if (pUser->IP != pIP)
		{
			pUser->IP = pIP;
			pUser->UDPPort = UDPPort;

			// 연결되어 있는 상태라면..
			// IP가 바꼈으므로 연결을 끊어준다.
			/*
			if (pUser->Connected)
			{
				g_pRequestClientPlayerManager->Disconnnect( name.c_str() );
			}
			*/
		}
	}	
}

//----------------------------------------------------------------------
// Remove RequestUser
//----------------------------------------------------------------------
bool
RequestUserManager::RemoveRequestUser(const char* pName)
{
	REQUEST_USER_MAP::iterator iUser = m_RequestUsers.find( std::string(pName) );

	if (iUser!=m_RequestUsers.end())
	{
		RequestUserInfo* pUser = iUser->second;

		delete pUser;

		m_RequestUsers.erase( iUser );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Get UserInfo
//----------------------------------------------------------------------
RequestUserInfo*
RequestUserManager::GetUserInfo(const char* pName) const
{
	REQUEST_USER_MAP::const_iterator iUser = m_RequestUsers.find( std::string(pName) );

	if (iUser!=m_RequestUsers.end())
	{
		return iUser->second;
	}

	return NULL;
}


//----------------------------------------------------------------------
// Has RequestingUser
//----------------------------------------------------------------------
bool
RequestUserManager::HasRequestingUser(const char* pName) const
{
	REQUESTING_USER_MAP::const_iterator iUser = m_RequestingUsers.find( std::string(pName) );

	if (iUser!=m_RequestingUsers.end())
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Add RequestingUser
//----------------------------------------------------------------------
void
RequestUserManager::AddRequestingUser(const char* pName, REQUESTING_FOR rf)
{
	std::string name = pName;

	m_RequestingUsers[name] = rf;	
}

//----------------------------------------------------------------------
// Remove RequestUser
//----------------------------------------------------------------------
bool
RequestUserManager::RemoveRequestingUser(const char* pName)
{
	REQUESTING_USER_MAP::iterator iUser = m_RequestingUsers.find( std::string(pName) );

	if (iUser!=m_RequestingUsers.end())
	{
		m_RequestingUsers.erase( iUser );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Get RequestingFor
//----------------------------------------------------------------------
RequestUserManager::REQUESTING_FOR			
RequestUserManager::GetRequestingFor(const char* pName) const
{
	REQUESTING_USER_MAP::const_iterator iUser = m_RequestingUsers.find( std::string(pName) );

	if (iUser!=m_RequestingUsers.end())
	{
		return iUser->second;
	}

	return REQUESTING_FOR_NULL;
}

//----------------------------------------------------------------------
// RemoveRequestUserLater 
//----------------------------------------------------------------------
void				
RequestUserManager::RemoveRequestUserLater(const char* pName)
{
	Lock();

	m_RemoveUsers.push_back( std::string(pName) );

	Unlock();
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
void
RequestUserManager::Update()
{
	if (m_RemoveUsers.empty())
	{
		return;
	}

	Lock();

	while (!m_RemoveUsers.empty())
	{
		const std::string& name = m_RemoveUsers.front();

		RemoveRequestUser( name.c_str() );

		m_RemoveUsers.pop_front();
	}

	Unlock();
}