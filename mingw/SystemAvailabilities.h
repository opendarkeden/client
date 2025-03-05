//---------------------------------------------------------------------------
//
// 기능별 제한-_-
// 시간이 없다보니 날림으로~
// 
// date : 2003-09-16
// by sonee
//
//---------------------------------------------------------------------------

#pragma once

#pragma warning(disable:4786)

#include <map>
#include <list>
#include <bitset>

#define MAX_OPEN_DEGREE		9

class SystemAvailabilitiesManager
{
protected :
	struct FilterScript
	{
		int scriptID;
		int answerID;
	};

	struct FilterScriptByDegree : FilterScript
	{
		int zoneID;
	};

	enum SystemKind
	{
		SYSTEM_PARTY = 0,
		SYSTEM_GAMBLE,
		SYSTEM_RANK_BONUS,
		SYSTEM_ENCHANT,
		SYSTEM_GUILD,
		SYSTEM_MASTER_LAIR,
		SYSTEM_PK_ZONE,
		SYSTEM_MARKET,
		SYSTEM_GRAND_MASTER_EFFECT,
		SYSTEM_COUPLE,
		SYSTEM_HOLY_LAND_WAR,
		SYSTEM_GUILD_WAR,
		SYSTEM_RACE_WAR,
		SYSTEM_FLAG_WAR,
		
		SYSTEM_MAX
	};
	
public :

	SystemAvailabilitiesManager();
	~SystemAvailabilitiesManager();
	
	void SetFlag( DWORD Flag ) { m_Flag = std::bitset< SYSTEM_MAX > (Flag); }
	void SetOpenDegree( BYTE grade ) { m_OpenDegree = grade; }
	void SetLimitSkillLevel( BYTE level ) { m_LimitSkillLevel = level; }
	
	// Guild Avaliable

	bool	IsAvailableGuildSystem() const { return m_Flag[SYSTEM_GUILD]; }
	bool	IsAvailablePartySystem() const { return m_Flag[SYSTEM_PARTY]; }
	bool	IsAvailableGambleSystem() const { return m_Flag[SYSTEM_GAMBLE]; }
	bool	IsAvailableCoupleSystem() const { return m_Flag[SYSTEM_COUPLE]; }
	bool	IsAvailableGrandMasterEffectSystem() const { return m_Flag[SYSTEM_GRAND_MASTER_EFFECT]; }
	bool	IsAvailableRaceWarSystem() const { return m_Flag[SYSTEM_RACE_WAR]; }
	bool	IsAvailableGuildWarSystem() const { return m_Flag[SYSTEM_GUILD_WAR]; }
	bool	IsAvailableMarketSystem() const { return m_Flag[SYSTEM_MARKET]; }
	bool	IsAvailableEnchantSystem() const { return m_Flag[SYSTEM_ENCHANT]; }
	bool	IsAvailableRankBonusSystem() const { return m_Flag[SYSTEM_RANK_BONUS]; }
	bool	IsAvailablePKZoneSystem() const { return m_Flag[SYSTEM_PK_ZONE]; }
	bool	IsAvailableMasterLairSystem() const { return m_Flag[SYSTEM_MASTER_LAIR]; }
	bool	IsAvailableCTFSystem() const { return m_Flag[SYSTEM_FLAG_WAR]; }

	BYTE	GetLimitLearnSkillLevel() const { return m_LimitSkillLevel; }

	bool	ScriptFiltering( int scriptID, int answerID );
	bool	ZoneFiltering( int zoneID ) const;

	bool	LoadFromFile(const char *szFileName);
	
private :
	bool	CheckScript( const std::list<FilterScript>& List, int &scriptID, int& answerID ) const;

	std::bitset< SYSTEM_MAX >							m_Flag;
	std::map< SystemKind, std::list<FilterScript> >		m_ScriptFilter;
	std::list< int >									m_ZoneFilter[ MAX_OPEN_DEGREE ];
	std::list< FilterScriptByDegree >					m_DegreeScriptFilter;

	BYTE												m_OpenDegree;
	BYTE												m_LimitSkillLevel;
	
};

extern SystemAvailabilitiesManager *g_pSystemAvailableManager;
