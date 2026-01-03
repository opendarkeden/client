//-----------------------------------------------------------------------------------------------
// MWarManager
//  Date : 2003.2
//  
//  by sonee
// 
//  전쟁 정보를 가지고 있는다.
//  전쟁 정보에 따라 UI 에서 확인 할 수 있도록 Push 해준다.
//-----------------------------------------------------------------------------------------------

#ifndef __MWARMANAGER_H__
#define __MWARMANAGER_H__

//#include "types.h"
#include "WarInfo.h"

#include <map>
#include <list>
#include <vector>

enum WAR_TYPE_ID // 안씀
{
	WAR_TYPE_HOLYLAND,					// 아담의 성지
	WAR_TYPE_GUILD,						// 길드전 
};

typedef			std::map<ZoneID_t, WarInfo*>			WarInfoMap;
typedef			WarInfoMap::iterator			WarInfoMapItr;


class MWarManager
{
private :

// 	list<ZoneID_t>		HolyLandZone;							// 아담의 성지 필드 존 ID List	
	WarInfoMap			m_WarInfo;								// 전쟁 정보 Map<ZoneID>

public :	
	MWarManager();
	~MWarManager();	

	//-------------------------------------------------------------------
	// SetData
	//-------------------------------------------------------------------
	void			SetWar(WarInfo *info);						// WarList Add
	void			RemoveWar(ZoneID_t id);						// WarList Remove
	
	void			ClearWar();									// Clear
	void			ClearRaceWar();

	//-------------------------------------------------------------------
	// Get Data
	//-------------------------------------------------------------------
	WarInfo*						GetWarInfo(ZoneID_t id);
	const WarInfoMap&				getWarInfoList() { return m_WarInfo; }
	int								getSize() { return m_WarInfo.size(); }
	
	//-------------------------------------------------------------------
	// Check
	//-------------------------------------------------------------------
	bool			IsExist(ZoneID_t id);						// 존아이디가 전쟁에 포함되어있는가
// 	bool			IsHolyLand(ZoneID_t id);					// 성이 아니라 아담의 성지 필드인가
	
	void			Update();	
};

extern MWarManager	*g_pWarManager;

#endif
