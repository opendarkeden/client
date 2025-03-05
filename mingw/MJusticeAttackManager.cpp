//----------------------------------------------------------------------
// MJusticeAttackManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MJusticeAttackManager.h"
#include "ClientConfig.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MJusticeAttackManager* g_pJusticeAttackManager = NULL;

extern DWORD g_CurrentTime;

//----------------------------------------------------------------------
//
// constructor / destructor
// 
//----------------------------------------------------------------------
MJusticeAttackManager::MJusticeAttackManager()
{
}

MJusticeAttackManager::~MJusticeAttackManager()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MJusticeAttackManager::Release()
{
	m_Creatures.clear();
}

//----------------------------------------------------------------------
// Add Creature
//----------------------------------------------------------------------
void		
MJusticeAttackManager::AddCreature(const char* pName)
{
	std::string name = pName;

	// 이미 있거나 없거나 관계없이 시간을 설정해주면 된다.
	// 시간의 의미는 없지만.. ㅡ.ㅡ;
	m_Creatures[name] = g_CurrentTime;
}

//----------------------------------------------------------------------
// Remove Creature
//----------------------------------------------------------------------
bool		
MJusticeAttackManager::RemoveCreature(const char* pName)
{
	TIME_MAP::iterator iTime = m_Creatures.find( std::string(pName) );

	if (iTime != m_Creatures.end())
	{
		m_Creatures.erase( iTime );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Has Creature
//----------------------------------------------------------------------
bool
MJusticeAttackManager::HasCreature(const char* pName) const
{
	TIME_MAP::const_iterator iTime = m_Creatures.find( std::string(pName) );

	if (iTime != m_Creatures.end())
	{
		return true;
	}

	return false;
}
