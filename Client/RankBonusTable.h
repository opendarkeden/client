//---------------------------------------------------------------------------------
// RankBonusTable.h
//---------------------------------------------------------------------------------
// 이미 화일이 존재하고.. 거기서 읽어온다고 가정했기 때문에.. 
// save함수는 없당..
//---------------------------------------------------------------------------------

#ifndef __RANKBONUSTABLE_H__
#define __RANKBONUSTABLE_H__

#include "CTypeTable.h"
#include "MString.h"
#include "RaceType.h"

#include <fstream>
using namespace std;


//---------------------------------------------------------------------------------
// RankBonus Info
//---------------------------------------------------------------------------------
class RankBonusInfo {
	public:
		enum RANK_BONUS_STATUS
		{
			STATUS_NULL,
			STATUS_CANNOT_LEARN,
			STATUS_LEARNED,
		};
	
	public :
		const unsigned short	GetType() const			{ return m_type; }
		const char*				GetName() const			{ return m_Name.GetString(); }
		const char				GetLevel() const		{ return m_level; }
		const bool				IsSlayerSkill() const	{ return (m_race == RACE_SLAYER); }
		const bool				IsVampireSkill() const	{ return (m_race == RACE_VAMPIRE); }
		const bool				IsOustersSkill() const	{ return (m_race == RACE_OUSTERS); }
		const unsigned short	GetSkillIconID() const	{ return m_skillIconID; }
		const RANK_BONUS_STATUS	GetStatus() const		{ return m_status; }
		void					SetStatus(RANK_BONUS_STATUS status)	{ m_status = status; }
		const int				GetPoint() const		{ return m_point; }

#ifndef __GAME_CLIENT__
		void		SetType(unsigned short type)			{ m_type = type; }
		void		SetName(const char *name)				{ m_Name = name; }
		void		SetLevel(char level)					{ m_level = level; }
		void		SetRace(Race eRace)						{ m_race = eRace; }
		void		SetSkillIconID(unsigned short id)		{ m_skillIconID = id; }
		void		SetPoint(int point)						{ m_point = point; }
#endif

	public :
		RankBonusInfo();
		void		LoadFromFile(std::ifstream& file);		

#ifndef __GAME_CLIEMT__
		void		SaveToFile(std::ofstream& file);
#endif

	private:
		unsigned short m_type;
		MString m_Name;
		char m_level;
		Race m_race;	// 0 : Slayer  1 : Vampire
		int m_point;
		unsigned short m_skillIconID;
		RANK_BONUS_STATUS m_status;
		
};

//---------------------------------------------------------------------------------
// RankBonus Table
//---------------------------------------------------------------------------------
class RankBonusTable:public CTypeTable<RankBonusInfo>
{
#ifndef __GAME_CLIENT__
public:
	RankBonusTable();
#endif
};

extern RankBonusTable *g_pRankBonusTable;
#endif