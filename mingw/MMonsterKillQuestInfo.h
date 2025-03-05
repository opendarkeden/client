////////////////////////////////////////////////////////////////////////////////////
// 2003.4.10
// MMonsterKillQuestInfo
//
// 2003.4.21
// MQuestInfoManager
//
// Quest에 대한 정보를 관리하는 매니저 이다. 처음엔 그냥 몬스터 킬 퀘스트에만 쓰여서
// 이름을 -_- MMonsterKillQuestInfo 로 했으나, 점차 그 쓰임이 확대되어 -_- QuestInfo
// Manager 로 바뀌었다. 말그대로 퀘스트 정보를 관리하는 클래스 이다.
//
// by sonee
////////////////////////////////////////////////////////////////////////////////////

#ifndef __MONSTER_KILL_QUEST_INFO_HEADER__
#define __MONSTER_KILL_QUEST_INFO_HEADER__

#pragma warning(disable:4786)

#include "CTypeTable.h"
#include "MQuestInfo.h"
#include "MString.h"

#include <list>
#include "CTypeMap.h"



enum MINI_GAME_TYPE 
{	
	MINI_GAME_TYPE_MINE = 0,
	MINI_GAME_TYPE_NEMO,
	MINI_GAME_TYPE_PUSH,
	MINI_GAME_TYPE_ARROW,
};

class QUEST_INFO : public MQuestInfo
{
public :
	QUEST_INFO();
	~QUEST_INFO();

	DWORD			GetCreatureType()	{ return m_CreatureType; }
	DWORD			GetSpriteType()		{ return m_SpriteType;}
	DWORD			GetGoal()			{ return m_Goal; }
	
	// 아이템 찾기면..
	DWORD			GetItemClass()		{ return m_CreatureType; }
	DWORD			GetItemType()		{ return m_SpriteType; }
	const char*		GetName()			{ return m_Name.GetString(); }

	MINI_GAME_TYPE	GetGameType()		{ return (MINI_GAME_TYPE)m_CreatureType; }
	
	bool			IsChief()		{ return m_Chief == 1;}
	
	void			LoadFromFile(ifstream& file);
	void			SaveToFile(ofstream& file);

	BYTE			GetQuestLevel() { return m_QuestLevel; }
	BYTE			GetEventQuest()	{ return m_EventQuest; }

	void			SetQuestLevel(BYTE level) { m_QuestLevel = level; }
	void			SetEventQuest(BYTE event) { m_EventQuest = event; }

	// Goal 은 서버의 Packet 에 의해 바뀔 수 있다.
	void			SetGoal(DWORD goal) { m_Goal = goal; }	
	void			SetName(const char* str) { m_Name = str; }
private :
	DWORD			m_CreatureType;
	DWORD			m_SpriteType;
	char			m_Chief;
	DWORD			m_Goal;
	BYTE			m_EventQuest;
	BYTE			m_QuestLevel;
	MString			m_Name;	
};

class MQuestInfoManager : public CTypeMap<QUEST_INFO>
{
public :
	MQuestInfoManager();
	~MQuestInfoManager();

	void					Init();
	QUEST_INFO*				GetInfo(DWORD id) ;
	void					SetInfo(DWORD id, DWORD goal, DWORD time, std::string str);
	void LoadFromFile(ifstream& file);

};

extern		MQuestInfoManager	*g_pQuestInfoManager;					// 퀘스트 자체 정보

#endif