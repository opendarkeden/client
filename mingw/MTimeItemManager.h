//-----------------------------------------------------------------------------
// MTimeItemManager.H									- by sonee
//-----------------------------------------------------------------------------
// MTimeItemManager
//  - 시간이 필요한 아이템의 정보를 기록한다.
//  - 2003.04.04
//-----------------------------------------------------------------------------

#ifndef __TIME_ITEM_MANAGER_HEADER__
#define __TIME_ITEM_MANAGER_HEADER__

#pragma warning(disable:4786)

#include "MTypeDef.h"
#include <map>

// DWORD 는 ms 가 아니라 초단위-_-a

class MTimeItemManager : public std::map<TYPE_OBJECTID, DWORD>
{
public :
	typedef std::map<TYPE_OBJECTID, DWORD>			TIMEITEM_MAP;

	MTimeItemManager();		
	~MTimeItemManager();
	
	bool	IsExist( TYPE_OBJECTID objectID );	
	bool	IsExpired( TYPE_OBJECTID objectID );

	bool	AddTimeItem( TYPE_OBJECTID objectID, DWORD time );			// time 은 second 단위
	bool	RemoveTimeItem( TYPE_OBJECTID objectID );
	
	
	int		GetDay( TYPE_OBJECTID objectID );
	int		GetHour( TYPE_OBJECTID objectID );
	int		GetMinute( TYPE_OBJECTID objectID );
	int		GetSecond( TYPE_OBJECTID objectID );
};

extern MTimeItemManager		*g_pTimeItemManager;

#endif