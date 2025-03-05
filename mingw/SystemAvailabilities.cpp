#include "Client_PCH.h"
#include "SystemAvailabilities.h"
#include "MinTR.H"
#include "RarFile.h"
#include "packet/Properties.h"

SystemAvailabilitiesManager *g_pSystemAvailableManager = NULL;

SystemAvailabilitiesManager::SystemAvailabilitiesManager()
{
	// Default 로 모두 가능 상태로 되어있다.
	for(int i = 0; i < m_Flag.size(); i++ )
		m_Flag.set(i);

	m_OpenDegree = 0xFF;
	m_LimitSkillLevel = 0xFF;
}

SystemAvailabilitiesManager::~SystemAvailabilitiesManager()
{
}

bool	SystemAvailabilitiesManager::ScriptFiltering(int scriptid, int answerid)
{
	for(int i = 0; i < SYSTEM_MAX; i++ )
	{
		if( m_Flag[i] )
			continue;

		const std::list<FilterScript> &Filter = m_ScriptFilter[(SystemKind)i];

		if( Filter.empty() )
			continue;

		if( CheckScript( Filter, scriptid, answerid ) == false )		// 해당 스크립트를 사용할 수 없으면 false 를 리턴
			return false;
	}	

	std::list<FilterScriptByDegree>::const_iterator itr = m_DegreeScriptFilter.begin();
	std::list<FilterScriptByDegree>::const_iterator enditr = m_DegreeScriptFilter.end();
	
	while( itr != enditr )
	{
		const FilterScriptByDegree &scr = *itr;

		if( scriptid == scr.scriptID && answerid == scr.answerID )
		{
			if( !ZoneFiltering( scr.zoneID ) )
				return false;
		}
		
		itr++;
	}
	return true;
}

bool	SystemAvailabilitiesManager::ZoneFiltering( int zoneID ) const
{
	if( m_OpenDegree >= MAX_OPEN_DEGREE )
		return true;

	for(int i = m_OpenDegree; i >= 0; i-- )
	{
		std::list<int>::const_iterator itr = m_ZoneFilter[i].begin();
		std::list<int>::const_iterator endItr = m_ZoneFilter[i].end();
		
		while( itr != endItr )
		{			
			int AllowZoneID = *itr;
			itr++;

			if( AllowZoneID == 90909 )
				return true;

			if( zoneID == AllowZoneID )
				return true;
		}
	}
	return false;
}

bool	SystemAvailabilitiesManager::CheckScript( const std::list<FilterScript>& List, int &scriptID, int& answerID ) const
{
	std::list<FilterScript>::const_iterator itr = List.begin();	
	std::list<FilterScript>::const_iterator endItr = List.end();

	while( itr != endItr )
	{
		const FilterScript *Script = &(*itr);

		if( Script->scriptID == scriptID &&
			Script->answerID == answerID )
			return false;

		itr++;
	}
	return true;			// 리스트에 없으면 사용 가능
}

bool	SystemAvailabilitiesManager::LoadFromFile(const char *szFileName)
{
	CRarFile rarfile;
	rarfile.SetRAR(g_pFileDef->getProperty("FILE_INFO_DATA").c_str(),"darkeden");
	rarfile.Open(szFileName);

	if( !rarfile.IsSet() )
		return false;
	char szLine[512];

	int key=-1, count=-1;

	std::list<FilterScript>			ScriptList;
	std::list<FilterScriptByDegree> ScriptListByDegree;
	std::list<int>					ZoneList;

	enum KIND_PARSE
	{
		SCRIPT_PARSE,
		ZONE_PARSE,
		DEGREE_SCRIPT_PARSE,

		PARSE_MAX
	};

	KIND_PARSE Kind = PARSE_MAX;

	while( rarfile.GetString( szLine, 512 ) )
	{
		// * 는 key, ; 는 주석
		if( strlen( szLine ) <= 0 )
			continue;
		
		if( szLine[0] == ';' )
			continue;

		if( szLine[0] == '*' )
		{
			sscanf(szLine+1,"%d %d",&key,&count);		// key 는 enum(SystemKind) 값.
			ScriptList.clear();
			count--;
			Kind = SCRIPT_PARSE;
			continue;
		}

		if( szLine[0] == 'Z' )
		{
			sscanf(szLine+1,"%d",&key);					// key 는 회차
			ZoneList.clear();
			Kind = ZONE_PARSE;
			continue;
		}
		
		if( szLine[0] == 'S' )
		{
			sscanf(szLine+1,"%d %d",&key,&count);		// key 는 무효-_-
			ScriptListByDegree.clear();
			count--;
			Kind = DEGREE_SCRIPT_PARSE;
			continue;
		}

		if( key != -1 )
		{
			switch( Kind )
			{
			case SCRIPT_PARSE :
				{
					FilterScript SCR;
					sscanf(szLine,"%d %d",&SCR.scriptID,&SCR.answerID);
					SCR.answerID--;
					ScriptList.push_back( SCR );
					count--;

					if( count < 0 )
					{
						m_ScriptFilter[(SystemKind)key] = ScriptList;
						ScriptList.clear();
						key = -1;
						Kind = PARSE_MAX;
					}
				}
				break;
			
			case ZONE_PARSE :
				{
					int FilterZoneID;
					sscanf(szLine,"%d",&FilterZoneID);
					if( FilterZoneID != 99999 )
						ZoneList.push_back( FilterZoneID );

					if( FilterZoneID == 99999 )
					{
						if( key >= 0 && key < MAX_OPEN_DEGREE )
							m_ZoneFilter[key] = ZoneList;
						ZoneList.clear();
						key = -1;
						Kind = PARSE_MAX;						
					}
				}
				break;
			case DEGREE_SCRIPT_PARSE :
				{
					FilterScriptByDegree SCR;
					sscanf(szLine,"%d %d %d",&SCR.scriptID,&SCR.answerID,&SCR.zoneID);
					SCR.answerID--;
					ScriptListByDegree.push_back( SCR );
					count--;

					if( count < 0 )
					{
						m_DegreeScriptFilter = ScriptListByDegree;
						ScriptListByDegree.clear();
						key = -1;
						Kind = PARSE_MAX;
					}
				}
				break;
			}			
		}
	}
	rarfile.Release();
	return true;
}
