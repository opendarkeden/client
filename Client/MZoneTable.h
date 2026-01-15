//----------------------------------------------------------------------
// MZoneTable.h
//----------------------------------------------------------------------
// Zone Loading시에 Zone마다 필요한 정보를 저장하는 class
//----------------------------------------------------------------------
//
// [ Zone별로 Loadind에 필요한 정보 ] 
//
// - ZoneID
// - Zone크기Type (Large or Small)
// - file이름
// 
//----------------------------------------------------------------------

#ifndef	__MZONETABLE_H__
#define	__MZONETABLE_H__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include "MTypeDef.h"
#include "MString.h"
#include <map>
#include <list>

#define	FLAG_ZONESIZE_SMALL		0x01


// 2004, 5, 14 sobeit add start - pk zone 관련
#define PK_TYPE_NULL			0			// 그냥 default map
#define PK_TYPE_DISABLE			0x01		// 다 우리편..-_-; pk 금지
#define PK_TYPE_ALL				0x02		// 나 빼고 다 적
#define PK_TYPE_GUILD			0x03		// 우리 길드 빼고 다 적..
#define PK_TYPE_SIEGE			0x04		// Siege 맵 // effect status에따라 적
// 2004, 5, 14 sobeit add end - pk zone 관련
//----------------------------------------------------------------------
//
// 한 종류의 Zone에 대한 정보
//
//----------------------------------------------------------------------
class ZONETABLE_INFO {
	public :
		typedef std::list<TYPE_SOUNDID>			SOUNDID_LIST;

	
	public :
		TYPE_ZONEID				ID;						// Zone ID
		MString					Name;					// Zone 이름
		BYTE					Property;				// Zone의 속성
		MString					Filename;				// Zone File이름
		MString					InfoFilename;			// ZoneInfo File이름
		MString					TeenFilename;			// Minimap File이름
		TYPE_MUSICID			MusicIDSlayer;			// Music종류		
		TYPE_MUSICID			MusicIDVampire;			// Music종류		
		SOUNDID_LIST			SoundIDList;			// random하게 나는 소리들..
		WORD					CreatureColorSet;		// Zone 전체의 몹 색깔 바꾸기
		bool					Safety;
		unsigned short			MinimapSpriteID;		// 미니맵의 스프라이트 아이디
		bool					HolyLand;				// 아담의 성지인가
		bool					ChatMaskByRace;			// 종족에 따라 채팅내용에 마스크를 씌우는가
		bool					CannotAttackInSafe;		// 안전지대에서 공격할 수 없는가
		bool					CannotUseSpecialItem;	// 변신이나 소환 금지 여부
		bool					CompetenceZone;
		BYTE					PKType;					// 자유 PK 지역인가?
		
	public :
		ZONETABLE_INFO();

		TYPE_SOUNDID			GetRandomSoundID() const;

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(class ofstream& file);		
		void			LoadFromFile(class ifstream& file);		
};

//----------------------------------------------------------------------
//
// 전체 Zone에 대한 정보
//
//----------------------------------------------------------------------
class CZoneTable {
	public :
		typedef	std::map<TYPE_ZONEID, ZONETABLE_INFO*>	ZONEINFO_MAP;

	public :
		CZoneTable();
		~CZoneTable();

		//-------------------------------------------------------
		// Release
		//-------------------------------------------------------
		void				Release();

		//-------------------------------------------------------
		// zoneID가 id인 zone의 정보를 얻는다.
		//-------------------------------------------------------
		bool				Add(ZONETABLE_INFO* pZoneInfo);
		ZONETABLE_INFO*		Get(TYPE_ZONEID id);

		const ZONEINFO_MAP&	GetZoneMap() const	{ return m_mapZoneInfo; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void				SaveToFile(class ofstream& file);
		void				LoadFromFile(class ifstream& file);
	
	protected :
		ZONEINFO_MAP		m_mapZoneInfo;		// zone정보 map
};

extern CZoneTable*		g_pZoneTable;

#endif


