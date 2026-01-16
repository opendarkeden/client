#include "Client_PCH.h"
#include "MQuestInfo.h"

void	MQuestInfo::LoadFromFile(std::ifstream& file)
{
	BYTE Type;
	file.read((char*)&Type, sizeof( BYTE ) );
	file.read((char*)&m_ID, sizeof( DWORD ) );
	file.read((char*)&m_TimeLimit, sizeof( DWORD ) );

	m_Type = (QUEST_INFO_TYPE) Type;
}

void	MQuestInfo::SaveToFile(std::ofstream& file)
{
	BYTE Type = (int)GetType();

	file.write((const char*)&Type, sizeof( BYTE ) );
	file.write((const char*)&m_ID, sizeof( DWORD ) );	
	file.write((const char*)&m_TimeLimit, sizeof( DWORD ) );
}
	
MQuestInfo::MQuestInfo()
{
	m_ID = 0;
	m_Type = QUEST_INFO_TYPE_NULL;
}

MQuestInfo::~MQuestInfo()
{
	m_ID = 0;
	m_Type = QUEST_INFO_TYPE_NULL;
}