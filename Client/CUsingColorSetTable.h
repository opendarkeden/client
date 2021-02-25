//----------------------------------------------------------------------
// CUsingColorSetTable.h
//----------------------------------------------------------------------
// CIndexSprite에서 사용하는 ColorSet 중에서
// 어떤 Sprite에는 특정한 색깔들만 사용할 수 있게 하기 위해서..
// MAX_COLORSET개의 bool을 만들고
// 사용할 수 있는 COLORSET에는 BOOL을 표시한다.
//----------------------------------------------------------------------

#ifndef	__CUSINGCOLORSETTABLE_H__
#define	__CUSINGCOLORSETTABLE_H__

#include "CIndexSprite.h"
#include "CTypeTable.h"
class ofstream;
class ifstream;


class USINGCOLORSET_NODE {
	public :
		USINGCOLORSET_NODE();
		~USINGCOLORSET_NODE();

		//-------------------------------------------------
		// Enable / Disable
		//-------------------------------------------------
		void			Enable(int set);
		void			Disable(int set);

		//-------------------------------------------------
		// 일정 범위의 Enable / Disable 
		//-------------------------------------------------
		void			Enable(int start, int end);
		void			Disable(int start, int end);

		//-------------------------------------------------
		// File I/O
		//-------------------------------------------------
		void			SaveToFile(class ofstream& file);
		bool			LoadFromFile(class ifstream& file);

	protected :
		bool			m_bEnable[MAX_COLORSET];
};


typedef CTypeTable<USINGCOLORSET_NODE>		USINGCOLORSET_TABLE;
extern 	USINGCOLORSET_TABLE*	g_pUsingColorSetTable;


//----------------------------------------------------------------------
// UsingColorSet의 종류...
//----------------------------------------------------------------------
enum USINGCOLORSET
{
	USINGCOLORSET_NONE,
	USINGCOLORSET_ALL,
	USINGCOLORSET_SKIN,
	USINGCOLORSET_ITEM,
	MAX_USINGCOLORSET
};


#endif

