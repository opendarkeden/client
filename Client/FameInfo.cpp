#include "Client_PCH.h"
#include "SkillDef.h"

//typedef unsigned int DWORD;
#include "FameInfo.h"	


FameInfoTable *g_pFameInfoTable = NULL;

FameInfo::FameInfo()
{
	m_level = 0;
	m_domaintype = 0;
	m_fame = 0;	
}

FameInfo::~FameInfo()
{
}

DWORD FameInfo::GetFame( SKILLDOMAIN type )
{	
	if( (BYTE)(type - SKILLDOMAIN_BLADE) == m_domaintype )
		return m_fame;

	return 0;
}

void FameInfo::LoadFromFile(std::ifstream &file)
{
	file.read((char*)&m_domaintype, 1);
	file.read((char*)&m_level, 1);
	file.read((char*)&m_fame, sizeof(DWORD));
}

void FameInfo::SaveToFile(std::ofstream &file)
{
	file.write((const char*)&m_domaintype, 1);
	file.write((const char*)&m_level, 1);
	file.write((const char*)&m_fame, sizeof(DWORD));
}

int FameInfoTable::GetFameForLevel(SKILLDOMAIN type,int level)
{
	FameInfo fameInfo = Get( int(type - SKILLDOMAIN_BLADE) * (GetSize() / int(MAX_SKILLDOMAIN)) + level );

	return fameInfo.GetFame( type );
}

FameInfoTable::FameInfoTable()
{
}