//--------------------------------------------------------------------------
//
// date 2003-09-19
// by sonee
//
//
//--------------------------------------------------------------------------

#pragma once

#pragma warning (disable:4786)

#include "CTypeTable.h"
#include <vector>

class InterfaceInformation
{
public :
	InterfaceInformation();
	~InterfaceInformation();

	const POINT&				GetPoint( int n ) const { return m_PointList[n]; }
	const RECT&					GetRect( int n ) const { return m_RectList[n]; }
	
	bool						LoadFromLinePointList( const char *szLine );
	bool						LoadFromLineRectList( const char *szLine );
	
private :
	std::vector< POINT >		m_PointList;
	std::vector< RECT >			m_RectList;
	int							m_ID;					// Interface ID
};

class SkinManager : public CTypeTable< InterfaceInformation >
{
public :
	enum
	{
		INFO = 0,
		GAME_MENU,
		OPTION,
		TITLE,
		NEW_CHAR,

		INTERFACE_MAX
	};

	SkinManager();
	~SkinManager();
	
	bool			LoadInformation(const char *szFileName);

protected :
	void			LoadPointList(int k, void *rar );
	void			LoadRectList(int k, void *rar );
};

extern SkinManager *g_pSkinManager;