#include "Client_PCH.h"
#include "MTimeItemManager.H"

MTimeItemManager		*g_pTimeItemManager = NULL;

MTimeItemManager::MTimeItemManager()
{
	clear();
}

MTimeItemManager::~MTimeItemManager()
{
	clear();
}

bool	MTimeItemManager::IsExist(TYPE_OBJECTID objectID)
{
	if( empty() )
		return false;

	TIMEITEM_MAP::const_iterator c_itr = find( objectID );

	if(c_itr == end())
		return false;

	return true;
}

bool	MTimeItemManager::AddTimeItem(TYPE_OBJECTID objectID, DWORD time)
{
	RemoveTimeItem( objectID );
	
	insert( TIMEITEM_MAP::value_type( objectID, time + timeGetTime()/1000 ) );
	
	return true;
}

bool	MTimeItemManager::RemoveTimeItem(TYPE_OBJECTID objectID)
{
	if(IsExist( objectID ) )
	{
		TIMEITEM_MAP::iterator itr = find( objectID );
		erase( itr );
		return true;
	}

	return false;
}

int		MTimeItemManager::GetDay( TYPE_OBJECTID objectID )
{
	if(! IsExist( objectID ) )
		return -1;

	TIMEITEM_MAP::const_iterator c_itr = find( objectID );
	
	if(timeGetTime()/1000 > (*c_itr).second )
		return 0;

	DWORD time = (*c_itr).second - timeGetTime()/1000;
	
	return (time/60/60/24);
}

int		MTimeItemManager::GetHour( TYPE_OBJECTID objectID )
{
	if(! IsExist( objectID ) )
		return -1;

	TIMEITEM_MAP::const_iterator c_itr = find( objectID );

	if(timeGetTime()/1000 > (*c_itr).second )
		return 0;

	DWORD time = (*c_itr).second - timeGetTime()/1000;
	
	//time = time - ( GetDay() * 60 * 60 * 24 );
	
	// % ´Â ³»°¡ °¡Àå ½È¾îÇÏ´Â ¿¬»êÀÜµ¥..¤Ñ.¤Ì °è»êÇÏ±â ±ÍÂú¾Æ¼­..
	return ( (time/60/60) % 24 );
}

int		MTimeItemManager::GetMinute(TYPE_OBJECTID objectID )
{
	if(! IsExist( objectID ) )
		return -1;

	TIMEITEM_MAP::const_iterator c_itr = find( objectID );

	if(timeGetTime()/1000 > (*c_itr).second )
		return 0;

	DWORD time = (*c_itr).second - timeGetTime()/1000;
	
	// % ´Â ³»°¡ °¡Àå ½È¾îÇÏ´Â ¿¬»êÀÜµ¥..¤Ñ.¤Ì °è»êÇÏ±â ±ÍÂú¾Æ¼­..
	return ( (time/60 ) % 60 );
}

int		MTimeItemManager::GetSecond(TYPE_OBJECTID objectID )
{
	if(! IsExist ( objectID ) )
		return -1;

	TIMEITEM_MAP::const_iterator c_itr = find( objectID );
	
	if(timeGetTime()/1000 > (*c_itr).second )
		return 0;

	DWORD time = (*c_itr).second - timeGetTime()/1000;

	// % ´Â ³»°¡ °¡Àå ½È¾îÇÏ´Â ¿¬»êÀÜµ¥..¤Ñ.¤Ì °è»êÇÏ±â ±ÍÂú¾Æ¼­..
	return ( time % 60 );
}

bool	MTimeItemManager::IsExpired( TYPE_OBJECTID objectID )
{
	if( !IsExist( objectID ) )
		return true;

	if(GetDay( objectID ) == 0 && GetHour( objectID ) == 0 && GetMinute( objectID) == 0 && GetSecond( objectID ) == 0)
	{
		return true;
	}
	return false;
}
