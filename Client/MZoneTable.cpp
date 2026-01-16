//----------------------------------------------------------------------
// MZoneTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MZoneTable.h"

//----------------------------------------------------------------------
// SOUND_SOURCE
//----------------------------------------------------------------------
/*
class SOUND_SOURCE
{
	public :
		SOUND_SOURCE();
		~SOUND_SOURCE();

	//-------------------------------------------------------
	// zone의 (x, y)에서 TimeGap
	//-------------------------------------------------------
	public :
		int				x, y;
		int				MinTimeGap;			// 최소한 MinTimeGap후에 play해야한다.
		int				MaxTimeGap;			// MaxTime안에는 play해야한다.
		int				SoundID;
		int				NextPlayTime;		// 다음에 play해도 되는 시간

	public :
		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(std::ofstream& file);		
		void			LoadFromFile(std::ifstream& file);		
};
*/

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
CZoneTable*		g_pZoneTable = NULL;

//----------------------------------------------------------------------
//
//						ZONETABLE_INFO
//
//----------------------------------------------------------------------
ZONETABLE_INFO::ZONETABLE_INFO()
{
	Property = 0;
	CreatureColorSet = 0xFFFF;	
	Safety = false;
	HolyLand = false;
	ChatMaskByRace = true;
	CannotAttackInSafe = false;
	CannotUseSpecialItem = false;
	CompetenceZone = false;
	PKType = PK_TYPE_NULL;
}

//----------------------------------------------------------------------
// Get Random SoundID
//----------------------------------------------------------------------
TYPE_SOUNDID			
ZONETABLE_INFO::GetRandomSoundID() const
{
	int numSound = SoundIDList.size();

	if (numSound == 0)
	{
		return SOUNDID_NULL;
	}

	int select = rand()%numSound;
	
	SOUNDID_LIST::const_iterator	iID = SoundIDList.begin();

	// select번째 id를 선택한다.
	for (int i=0; i<select; i++)
	{
		iID++;
	}

	return *iID;
}

//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
void			
ZONETABLE_INFO::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&ID, SIZE_ZONEID);
	Name.SaveToFile( file );
	file.write((const char*)&Property, 1);
	file.write((const char*)&MusicIDSlayer, SIZE_MUSICID);
	file.write((const char*)&MusicIDVampire, SIZE_MUSICID);
	Filename.SaveToFile( file );
	InfoFilename.SaveToFile( file );
	TeenFilename.SaveToFile( file );
	
	// sound ID들 저장
	int numSound = SoundIDList.size();

	file.write((const char*)&numSound, 4);

	SOUNDID_LIST::iterator	iID = SoundIDList.begin();
	
	while (iID != SoundIDList.end())
	{
		TYPE_SOUNDID soundID = *iID;

		file.write((const char*)&soundID, SIZE_SOUNDID);

		iID++;
	}
	
	file.write((const char*)&Safety, 1);
	file.write((const char*)&CreatureColorSet, 2);	

	file.write((char*)&MinimapSpriteID, 2);
	file.write((const char*)&HolyLand,1);
	file.write((const char*)&ChatMaskByRace,1);
	file.write((const char*)&CannotAttackInSafe,1);		
	file.write((const char*)&CannotUseSpecialItem,1);	
	file.write((const char*)&CompetenceZone,1);	
	file.write((const char*)&PKType, 1 );
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
ZONETABLE_INFO::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&ID, SIZE_ZONEID);
	Name.LoadFromFile( file );
	file.read((char*)&Property, 1);
	file.read((char*)&MusicIDSlayer, SIZE_MUSICID);
	file.read((char*)&MusicIDVampire, SIZE_MUSICID);
	Filename.LoadFromFile( file );
	InfoFilename.LoadFromFile( file );
	TeenFilename.LoadFromFile( file );

	// sound id들 loading
	SoundIDList.clear();

	int numSound;
	file.read((char*)&numSound, 4);

	TYPE_SOUNDID soundID;
	for (int i=0; i<numSound; i++)
	{
		file.read((char*)&soundID, SIZE_SOUNDID);

		SoundIDList.push_back( soundID );
	}

	file.read((char*)&Safety, 1);	
	file.read((char*)&CreatureColorSet, 2);

	file.read((char*)&MinimapSpriteID, 2);
	file.read((char*)&HolyLand,1);
	file.read((char*)&ChatMaskByRace,1);
	file.read((char*)&CannotAttackInSafe,1);	
	file.read((char*)&CannotUseSpecialItem,1);	
	file.read((char*)&CompetenceZone,1);	
	file.read((char*)&PKType, 1 );
}


//----------------------------------------------------------------------
//
//							CZoneTable
//
//----------------------------------------------------------------------
CZoneTable::CZoneTable()
{
}

CZoneTable::~CZoneTable()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void				
CZoneTable::Release()
{
	ZONEINFO_MAP::iterator iInfo = m_mapZoneInfo.begin();

	// 모든 ZoneInfo를 메모리에서 지운다.
	while (iInfo != m_mapZoneInfo.end())
	{
		ZONETABLE_INFO*	pInfo = (*iInfo).second;
		
		if (pInfo!=NULL)
		{
			delete pInfo;
		}

		iInfo++;
	}

	m_mapZoneInfo.clear();
}

//----------------------------------------------------------------------
// Add ZoneInfo
//----------------------------------------------------------------------
// ZoneInfo를 추가한다.
//----------------------------------------------------------------------
bool				
CZoneTable::Add(ZONETABLE_INFO* pZoneInfo)
{
	if (pZoneInfo==NULL)
	{
		return false;
	}

	ZONEINFO_MAP::iterator iInfo = m_mapZoneInfo.find( pZoneInfo->ID );

	// 이미 있으면.. 지운다.
	if (iInfo!=m_mapZoneInfo.end())
	{
		delete pZoneInfo;

		return false;
	}

	// 추가한다.
	m_mapZoneInfo.insert( ZONEINFO_MAP::value_type(pZoneInfo->ID, pZoneInfo) );

	return true;
}

//----------------------------------------------------------------------
// Get ZoneInfo
//----------------------------------------------------------------------
// zoneID가 id인 zoneInfo를 얻는다.
//----------------------------------------------------------------------
ZONETABLE_INFO*		
CZoneTable::Get(TYPE_ZONEID id)
{
	ZONEINFO_MAP::iterator iInfo = m_mapZoneInfo.find( id );

	// 없으면..
	if (iInfo == m_mapZoneInfo.end())
	{
		return NULL;
	}

	return (*iInfo).second;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// size , 각각의 ZoneInfo
//----------------------------------------------------------------------
void				
CZoneTable::SaveToFile(std::ofstream& file)
{
	//---------------------------------------------
	// 전체 개수 저장
	//---------------------------------------------
	int size = m_mapZoneInfo.size();

	file.write((const char*)&size , 4);

	//---------------------------------------------
	// 각각의 zoneInfo를 저장한다.
	//---------------------------------------------
	ZONEINFO_MAP::iterator iInfo = m_mapZoneInfo.begin();

	while (iInfo != m_mapZoneInfo.end())
	{
		ZONETABLE_INFO*	pInfo = (*iInfo).second;
		
		if (pInfo!=NULL)
		{
			pInfo->SaveToFile( file );
		}

		iInfo++;
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// size , 각각의 ZoneInfo
//----------------------------------------------------------------------
void				
CZoneTable::LoadFromFile(std::ifstream& file)
{
	//---------------------------------------------
	// 기존에 있던 메모리 삭제
	//---------------------------------------------
	Release();

	//---------------------------------------------
	// size 읽어오기
	//---------------------------------------------
	int size;

	file.read((char*)&size, 4);

	//---------------------------------------------
	// 각각의 ZoneInfo를 Load한다.
	//---------------------------------------------
	for (int i=0; i<size; i++)
	{
		ZONETABLE_INFO* pInfo = new ZONETABLE_INFO;

		pInfo->LoadFromFile( file );

		Add( pInfo );
	}
}
