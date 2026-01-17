//----------------------------------------------------------------------
// ZoneFileHeader.h
//----------------------------------------------------------------------

#ifndef	__ZONEFILEHEADER_H__
#define	__ZONEFILEHEADER_H__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include "MString.h"
#include "MZoneDef.h"

#include <fstream>
using namespace std;

//----------------------------------------------------------------------
// FILEINFO_ZONE_HEADER
//----------------------------------------------------------------------
class FILEINFO_ZONE_HEADER {
public:
	enum ZONE_TYPE
	{
		NORMAL,
		SLAYER_GUILD,
		RESERVED_SLAYER_GUILD,
		NPC_VAMPIRE_LAIR,
		PC_VAMPIRE_LAIR,
		NPC_HOME,
		NPC_SHOP,
		RANDOMAP
	};

public :
	MString		ZoneVersion;	// 최종 업데이트된 날짜
	WORD		ZoneID;			//	존 아이디
	WORD		ZoneGroupID;	//	존 그룹 아이디
	MString		ZoneName;		// 존 이름


	BYTE		ZoneType;		//	존의 타입
	BYTE		ZoneLevel;		//	존에 나오는 몬스터의 레벨. 1-10 사이.
	MString		Description;	//	존에 대한 설명

public :
	FILEINFO_ZONE_HEADER()
	{
		ZoneVersion = MAP_VERSION_2000_05_10;
	}

	void		SaveToFile(std::ofstream& file);
	void		LoadFromFile(std::ifstream& file);

};

#endif

