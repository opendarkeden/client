#include "Client_PCH.h"
#include "Clientdef.h"

#include "MMonsterKillQuestInfo.h"

MQuestInfoManager	*g_pQuestInfoManager = NULL;

QUEST_INFO::QUEST_INFO()
{
	m_CreatureType = 0;
	m_SpriteType = 0;
	m_Chief = 0;
	m_Goal = 0;
	m_Name = "";
}

QUEST_INFO::~QUEST_INFO()
{
}
//
//void	QUEST_INFO::Set(DWORD id, DWORD mt, DWORD st, char chief, DWORD goal, DWORD time, const char* name)
//{
//	m_ID = id;
//	m_CreatureType = mt;
//	m_SpriteType = st;
//	m_Chief = chief;
//	m_Goal = goal;
//	m_TimeLimit = time;
//
//	if(name != NULL)
//		m_Name = name;
//}

void	QUEST_INFO::LoadFromFile(std::ifstream& file)
{
	MQuestInfo::LoadFromFile( file );
	file.read((char*)&m_CreatureType, sizeof( DWORD ) );
	file.read((char*)&m_SpriteType, sizeof(DWORD) );
	file.read((char*)&m_Chief, sizeof(char) );
	file.read((char*)&m_Goal, sizeof(DWORD) );	
	file.read((char*)&m_EventQuest, sizeof(BYTE) );
	file.read((char*)&m_QuestLevel, sizeof(BYTE) );
	m_Name.LoadFromFile( file );	
}

void	QUEST_INFO::SaveToFile(std::ofstream& file)
{
	MQuestInfo::SaveToFile( file );	
	file.write((const char*)&m_CreatureType, sizeof( DWORD ) );
	file.write((const char*)&m_SpriteType, sizeof(DWORD) );
	file.write((const char*)&m_Chief, sizeof(char) );
	file.write((const char*)&m_Goal, sizeof(DWORD) );
	file.write((const char*)&m_EventQuest, sizeof(BYTE) );
	file.write((const char*)&m_QuestLevel, sizeof(BYTE) );
	m_Name.SaveToFile( file );
}


QUEST_INFO*		MQuestInfoManager::GetInfo(DWORD id)
{
	return GetData(id);
//	int num = 0;
//	
//	for(int i=0;i<GetSize();i++)
//	{
//		if( m_pTypeInfo[i].GetID() == id )
//		{
//			num = i;
//			break;
//		}			
//	}
//	
//	return Get(num);
}

void					
MQuestInfoManager::SetInfo(DWORD id, DWORD goal, DWORD time, std::string str)
{	
	QUEST_INFO* mkq = GetInfo( id );
	if( NULL != mkq )
	{
		mkq->SetGoal( goal );
		mkq->SetTimeLimit( time );
		mkq->SetName(str.c_str());
	}
	else
	{
		QUEST_INFO*	pData = new QUEST_INFO;
		pData->SetID( id ); 
		pData->SetGoal( goal );
		pData->SetTimeLimit( time );
		pData->SetName(str.c_str());
		pData->SetType(QUEST_INFO_MONSTER_KILL);
		if (!AddData( id, pData ))
		{
			delete pData;
		}
		
	}
}

MQuestInfoManager::MQuestInfoManager(){}
MQuestInfoManager::~MQuestInfoManager(){}

void			
MQuestInfoManager::LoadFromFile(std::ifstream& file)
{
	//-----------------------------------------------------
	int infoSize;
	file.read((char*)&infoSize, 4);

	//-----------------------------------------------------
	// 각 info
	//-----------------------------------------------------
	for (int i=0; i<infoSize; i++)
	{		
		QUEST_INFO*	pData = new QUEST_INFO;

		pData->LoadFromFile( file );

		//-----------------------------------------------------
		// map에 추가한다.
		//-----------------------------------------------------
		if (!AddData( pData->GetID(), pData ))
		{
			delete pData;
		}
	}

}