//-----------------------------------------------------------------------------------------------
// MWarManager
//  Date : 2003.2
//  
//  by sonee
// 
//  ���� ������ ������ �ִ´�.
//  ���� ������ ���� UI ���� Ȯ�� �� �� �ֵ��� Push ���ش�.
//-----------------------------------------------------------------------------------------------

#ifndef __MWARMANAGER_H__
#define __MWARMANAGER_H__

//#include "types.h"
#include "WarInfo.h"

#include <map>
#include <list>
#include <vector>

enum WAR_TYPE_ID // �Ⱦ�
{
	WAR_TYPE_HOLYLAND,					// �ƴ��� ����
	WAR_TYPE_GUILD,						// ����� 
};

typedef			std::map<ZoneID_t, WarInfo*>			WarInfoMap;
typedef			WarInfoMap::iterator			WarInfoMapItr;


class MWarManager
{
private :

// 	list<ZoneID_t>		HolyLandZone;							// �ƴ��� ���� �ʵ� �� ID List	
	WarInfoMap			m_WarInfo;								// ���� ���� Map<ZoneID>

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
	bool			IsExist(ZoneID_t id);						// �����̵� ���￡ ���ԵǾ��ִ°�
// 	bool			IsHolyLand(ZoneID_t id);					// ���� �ƴ϶� �ƴ��� ���� �ʵ��ΰ�
	
	void			Update();	
};

extern MWarManager	*g_pWarManager;

#endif