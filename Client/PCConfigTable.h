//----------------------------------------------------------------------
// PCConfigTable.h
//----------------------------------------------------------------------
// key값이 <WorldID, PlayerID>에 대한 PCConfig정보
//
// 일단은... PlayerID의 3캐릭 중에서 
//           어느 캐릭이 최근에 접속했냐?는 정보만 있으면 된다.
//
// 기술 단축키, 기타 캐릭터 마다의 옵션? 설정 정보...등?
// 뭐 나름대로 넣을 수 있겠지...
// 지금이야.. PlayerID밖에.. -_-;
//
// [ RecentCount처리 ]
//
// PlayerID data개수를 제한하기 위해서 LRU PlayerID를 제거한다.
// save할때마다 RecentCount를 1씩 증가시킨다.
// SetLastSlot(접속할때)을 하지 않고.. save만 하게 되면
// 결국 RecentCount가 가장 큰 애가.. 제일~~ 오래전에 접속한애가 된다.
// 
//----------------------------------------------------------------------

#ifndef __PC_CONFIG_TABLE_H__
#define __PC_CONFIG_TABLE_H__

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include <map>
#include <string>
class ofstream;
class ifstream;

/*
class CharacterConfig {
	public :
		PCConfig();
		~PCConfig();			
};
*/

//----------------------------------------------------------------------
// PlayerConfig
//----------------------------------------------------------------------
class PlayerConfig {
	public :
		PlayerConfig();
		~PlayerConfig();

		//--------------------------------------------------------
		// PlayerID
		//--------------------------------------------------------
		void				SetPlayerID(const std::string& playerID)	{ m_PlayerID = playerID; }
		const std::string&	GetPlayerID() const							{ return m_PlayerID; }
		
		//--------------------------------------------------------
		// slot
		//--------------------------------------------------------
		int			GetLastSlot() const				{ return m_LastSlot; }
		void		SetLastSlot(int slot);

		DWORD		GetRecentCount() const						{ return m_RecentCount; }
		
		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);

	protected :
		std::string	m_PlayerID;
		BYTE		m_LastSlot;			// 최근에 login한 캐릭터의 slot번호( 0,1,2 )
		DWORD		m_RecentCount;		// 가장 오래전에 사용된(LRU) 캐릭터 체크용.
		//CharacterConfig[3]			// 캐릭터들에 대한 정보
};

//----------------------------------------------------------------------
// PlayerConfigTable
//----------------------------------------------------------------------
// map<PlayerID, PlayerConfig*>
//----------------------------------------------------------------------
class PlayerConfigTable : public std::map<std::string, PlayerConfig*> {
	public :
		PlayerConfigTable();
		~PlayerConfigTable();

		void				Release();

		//--------------------------------------------------------
		// Add/Get PlayerConfigTable
		//--------------------------------------------------------
		void				AddPlayerConfig(PlayerConfig* pConfig);
		PlayerConfig*		GetPlayerConfig(const char* pPlayerID) const;

		//--------------------------------------------------------
		// Limit PlayerConfig
		//--------------------------------------------------------
		void				LimitPlayerConfig(int limit);

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);

};


//----------------------------------------------------------------------
// WorldPlayerConfigTable
//----------------------------------------------------------------------
// map<WorldID, PlayerConfigTable*>
//----------------------------------------------------------------------
class WorldPlayerConfigTable : public std::map<int, PlayerConfigTable*> {
	public :
		WorldPlayerConfigTable();
		~WorldPlayerConfigTable();

		void				Release();

		//--------------------------------------------------------
		// Add/Get PlayerConfigTable
		//--------------------------------------------------------
		void				AddPlayerConfigTable(int worldID, PlayerConfigTable* pTable);
		PlayerConfigTable*	GetPlayerConfigTable(int worldID) const;

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		void		SaveToFile(const char* pFilename);
		void		LoadFromFile(const char* pFilename);
};


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
extern WorldPlayerConfigTable*		g_pWorldPlayerConfigTable;

#endif

