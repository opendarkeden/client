//----------------------------------------------------------------------
// MGuildInfoMapper.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MGuildInfoMapper.h"

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
MGuildInfoMapper* g_pGuildInfoMapper = NULL;


//----------------------------------------------------------------------
//
//						GUILD_INFO
//
//----------------------------------------------------------------------
GUILD_INFO::GUILD_INFO()
{
	m_SpriteID = SPRITEID_NULL;	
}

GUILD_INFO::GUILD_INFO(const char* pGuildName, const char* pLeaderName)
{
	m_SpriteID = SPRITEID_NULL;
//	m_GuildName = pGuildName;
//	m_LeaderName = pLeaderName;
}

GUILD_INFO::~GUILD_INFO()
{
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
GUILD_INFO::SaveToFile(ofstream& file)
{
	file.write((const char*)&m_SpriteID, SIZE_SPRITEID);
//	m_GuildName.SaveToFile( file );
//	m_LeaderName.SaveToFile( file );
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void	
GUILD_INFO::LoadFromFile(ifstream& file)
{
	file.read((char*)&m_SpriteID, SIZE_SPRITEID);
//	MString tempString;
//	tempString.LoadFromFile(file);
//	tempString.LoadFromFile(file);
//	m_GuildName.LoadFromFile( file );
//	m_LeaderName.LoadFromFile( file );
}

//----------------------------------------------------------------------
// operator = 
//----------------------------------------------------------------------
void			
GUILD_INFO::operator = (const GUILD_INFO& info)
{
	m_SpriteID = info.m_SpriteID;
//	m_GuildName = info.m_GuildName;
//	m_LeaderName = info.m_GuildName;
}

//----------------------------------------------------------------------
//
//						MGuildInfoMapper
//
//----------------------------------------------------------------------
MGuildInfoMapper::MGuildInfoMapper()
{
}

MGuildInfoMapper::~MGuildInfoMapper()
{
	Release();
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
/*
void
MGuildInfoMapper::Init(int n)
{
	Release();

	//resize( n );	
}
*/

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void	
MGuildInfoMapper::Release()
{
	GUILD_INFO_MAP::iterator iInfo = begin();

	while (iInfo != end())
	{
		GUILD_INFO* pInfo = iInfo->second;

		if (pInfo!=NULL)
		{
			delete pInfo;
		}

		iInfo ++;
	}

	clear();
}

//----------------------------------------------------------------------
// operator []
//----------------------------------------------------------------------
/*
TYPE_SPRITEID&	
MGuildInfoMapper::operator [] (int n)
{
	//return at(n);
	GUILD_INFO_MAP::iterator iID = find( n );

	if (iID != end())
	{
		return iID->
	}
}

const TYPE_SPRITEID&	
MGuildInfoMapper::operator [] (int n) const
{
	//return at(n);
}
*/

//----------------------------------------------------------------------
// Get
//----------------------------------------------------------------------
GUILD_INFO*	
MGuildInfoMapper::Get(WORD guildID) const
{
	GUILD_INFO_MAP::const_iterator iID = find( guildID );

	if (iID == end())
	{
		return NULL;
	}

	return iID->second;
}

//----------------------------------------------------------------------
// Set
//----------------------------------------------------------------------
void			
MGuildInfoMapper::Set(WORD guildID, GUILD_INFO* pInfo)
{
	// 이미 있었다면 지운다.
	GUILD_INFO* pOldInfo = Get(guildID);

	if (pOldInfo!=NULL)
	{
		delete pOldInfo;
	}

	// 재 설정..
	(*this)[guildID] = pInfo;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
MGuildInfoMapper::SaveToFile(ofstream& file)
{
	int num = size();

	file.write((const char*)&num, 4);

	GUILD_INFO_MAP::iterator iID = begin();

	while (iID != end())
	{
		WORD		guildID	= iID->first;
		GUILD_INFO*	pInfo	= iID->second;

		file.write((const char*)&guildID, 2);
		pInfo->SaveToFile( file );

		iID ++;
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void	
MGuildInfoMapper::LoadFromFile(ifstream& file)
{
	int num;

	file.read((char*)&num, 4);

	Release();

	WORD guildID;
	
	for (int i=0; i<num; i++)
	{
		file.read((char*)&guildID, 2);
		
		GUILD_INFO*	pInfo	= new GUILD_INFO;
		pInfo->LoadFromFile( file );

		(*this)[guildID] = pInfo;
	}
}

//----------------------------------------------------------------------
// Change Value To File
//----------------------------------------------------------------------
// 화일에서 사바사바.. -_-;
//----------------------------------------------------------------------
/*
void	
MGuildInfoMapper::ChangeValueToFile(const char* pFilename, WORD guildID) const
{
	ofstream file(pFilename, ios::binary | ios::ate);

	file.seekp( 4 + guildID*(sizeof(WORD)+SIZE_SPRITEID), ios::beg );

	TYPE_SPRITEID spriteID = at(guildID);
	file.write((const char*)&spriteID, SIZE_SPRITEID);
}
*/

//----------------------------------------------------------------------
// Save Info To File
//----------------------------------------------------------------------
//void	
//MGuildInfoMapper::SaveInfoToFile(const char* pFilename)
//{
//	ofstream file( pFilename );
//
//	GUILD_INFO_MAP::iterator iID = begin();
//
//	char str[256];
//	int emptyGuildMarkCount = 0;
//
//	file << "|| '''길드ID''' || '''길드 이름''' || '''마스터 이름''' || '''길드마크 SpriteID''' ||" << endl;
//
//	while (iID != end())
//	{
//		WORD		guildID	= iID->first;
//		GUILD_INFO*	pInfo	= iID->second;
//
//		sprintf(str, "|| %3d || %20s || %20s ||", guildID, pInfo->GetGuildName(), pInfo->GetLeaderName());
//		if(pInfo->GetSpriteID() == 0xffff)
//		{
//			sprintf(str, "%s 길드마크 없음 ||", str);
//			emptyGuildMarkCount++;
//		}
//		else
//			sprintf(str, "%s %13d ||", str, pInfo->GetSpriteID());
//
//		file << str << endl;
//
//		iID ++;
//	}
//
//	sprintf(str, "|| %5d 개의 길드중 %5d개의 길드 마크 없음 ||", size(), emptyGuildMarkCount);
//	file << str << endl;
//
//	file.close();
// }