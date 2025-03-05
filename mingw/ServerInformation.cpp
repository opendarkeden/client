//----------------------------------------------------------------------
// ServerInformation.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "ServerInformation.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
ServerInformation*	g_pServerInformation = NULL;

//----------------------------------------------------------------------
//
// ServerGroup
// 
//----------------------------------------------------------------------
ServerGroup::ServerGroup()
{
	m_GroupStatus = 0;
}

//----------------------------------------------------------------------
//
// ServerInformation
//
//----------------------------------------------------------------------
ServerInformation::ServerInformation()
{
}

ServerInformation::~ServerInformation()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void			
ServerInformation::Release()
{
	SERVER_GROUP_MAP::Release();

	m_ServerGroupID = 0;
	m_ServerGroupStatus = 0;
	m_ServerID = 0;

	// 이름을 지워준다.
	m_ServerGroupName.Release();
	m_ServerName.Release();
}

//----------------------------------------------------------------------
// Set Server GroupID
//----------------------------------------------------------------------
bool			
ServerInformation::SetServerGroupID(unsigned int id)	
{ 
	//-----------------------------------------------------------
	// Group의 이름을 기억해둔다.
	//-----------------------------------------------------------
	ServerGroup* pGroup = GetData( id );

	if (pGroup==NULL)
	{
		m_ServerGroupName.Release();

		return false;
	}
	
	m_ServerGroupID = id; 
	m_ServerGroupName = pGroup->GetGroupName();	
	m_ServerGroupStatus = pGroup->GetGroupStatus();

	return true;
}

//----------------------------------------------------------------------
// Set ServerID
//----------------------------------------------------------------------
bool			
ServerInformation::SetServerID(unsigned int id)		
{ 
	//-----------------------------------------------------------
	// Group찾기
	//-----------------------------------------------------------
	ServerGroup* pGroup = GetData( m_ServerGroupID );

	if (pGroup==NULL)
	{
		return false;
	}

	//-----------------------------------------------------------
	// Server 이름 찾기
	//-----------------------------------------------------------
	SERVER_INFO* pInfo = pGroup->GetData( id );

	if (pInfo==NULL)
	{
		return false;
	}
		
	m_ServerID = id; 
	m_ServerName = pInfo->ServerName;
	m_ServerStatus = pInfo->ServerStatus;

	return true;
}
