//----------------------------------------------------------------------
// MGuildInfoMapper.h
//----------------------------------------------------------------------

#ifndef __MGUILD_INFO_MAPPER_H__
#define __MGUILD_INFO_MAPPER_H__

#pragma warning(disable:4786)

#include "CSprite.h"
#include "MString.h"

#include <fstream>
#include <map>
using namespace std;

//----------------------------------------------------------------------
// GUILD_INFO
//----------------------------------------------------------------------
class GUILD_INFO {
	public :
		GUILD_INFO();
		GUILD_INFO(const char* pGuildName, const char* pLeaderName);
		~GUILD_INFO();

		//----------------------------------------------------------
		// Get / Set
		//----------------------------------------------------------
		void			SetSpriteID(TYPE_SPRITEID id)		{ m_SpriteID = id; }
//		void			SetGuildName(const char* pName)		{ m_GuildName = pName; }
//		void			SetLeaderName(const char* pName)	{ m_LeaderName = pName; }

		TYPE_SPRITEID	GetSpriteID() const					{ return m_SpriteID; }
//		const char*		GetGuildName() const				{ return m_GuildName.GetString(); }
//		const char*		GetLeaderName() const				{ return m_LeaderName.GetString(); }

		//----------------------------------------------------------
		// operator =
		//----------------------------------------------------------
		void			operator = (const GUILD_INFO& info);

		//----------------------------------------------------------
		// File I/O
		//----------------------------------------------------------
		void	SaveToFile(std::ofstream& file);
		void	LoadFromFile(std::ifstream& file);

	protected :
		TYPE_SPRITEID	m_SpriteID;
//		MString			m_GuildName;
//		MString			m_LeaderName;
};

//----------------------------------------------------------------------
// MGuildInfoMapper <길드마크, SpriteID>
//----------------------------------------------------------------------
class MGuildInfoMapper : public std::map<WORD, GUILD_INFO*> {
	public :
		typedef std::map<WORD, GUILD_INFO*> GUILD_INFO_MAP;

	public :
		MGuildInfoMapper();
		~MGuildInfoMapper();

		//----------------------------------------------------------
		// Init / Release
		//----------------------------------------------------------
		//void	Init(int n);
		void	Release();

		//----------------------------------------------------------
		// operator []
		//----------------------------------------------------------
		//TYPE_SPRITEID&			operator [] (int n);
		//const TYPE_SPRITEID&	operator [] (int n) const;
		GUILD_INFO*		Get(WORD guildID) const;
		void			Set(WORD guildID, GUILD_INFO* pInfo);

		//----------------------------------------------------------
		// File I/O
		//----------------------------------------------------------
		void	SaveToFile(std::ofstream& file);
		void	LoadFromFile(std::ifstream& file);
//		void	SaveInfoToFile(const char* pFilename);
		//void	ChangeValueToFile(const char* pFilename, WORD guildID) const;

		bool	IsExistGuildName( WORD guild ) { return m_GuildNameMap.find( guild ) != m_GuildNameMap.end(); }
		const char*		GetGuildName( WORD guild ) { return m_GuildNameMap[guild].c_str(); }
		void	SetGuildName( WORD guild, std::string& name ) { m_GuildNameMap[guild] = name; }
		void	ClearGuildName() { m_GuildNameMap.clear(); }
		
	private :
		std::map<WORD, std::string>			m_GuildNameMap;
};

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
extern MGuildInfoMapper* g_pGuildInfoMapper;

#endif

