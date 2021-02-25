//----------------------------------------------------------------------
// CFilter.h
//----------------------------------------------------------------------
//
// Sprite를 출력할때 효과를 주기위해 사용하는 Filter class
//
// 한 pixel에 대한 어떠한 변화를 나타내는 값들의 2D array
//
//----------------------------------------------------------------------

#ifndef	__CFILTER_H__
#define	__CFILTER_H__

#include <Windows.h>
#include "CTypePack.h"
class ifstream;
class ofstream;

class CFilter {
	public :
		CFilter();
		~CFilter();

		bool		IsNotInit() const	{ return m_ppFilter==NULL; }
		bool		IsInit() const		{ return m_ppFilter==NULL; }

		//---------------------------------------------------------
		// Init / Release
		//---------------------------------------------------------
		void		Init(WORD width, WORD height);
		void		Release();

		//---------------------------------------------------------
		// Set Filter Value
		//---------------------------------------------------------
		// CDirectDrawSurface에서 값을 읽어온다.
		void		SetFilter(WORD* pSurface, WORD pitch, WORD width, WORD height);
		
		// 다른 Filter를 대입한다.
		void		SetFilter(const CFilter& filter);

		// 다른 Filter를 어둡게 해서.. 대입한다.
		void		SetFilterDarkness(const CFilter& filter, BYTE DarkBits);

		// 직접 Filter의 (x,y)좌표에 값을 대입한다.
		void		SetFilter(WORD x, WORD y, BYTE value)	{ m_ppFilter[y][x]=value; }

		//---------------------------------------------------------
		// Blt
		//---------------------------------------------------------
		void		BltFilter(int x, int y, const CFilter& filter);
		void		BltFilterAdd(int x, int y, const CFilter& filter);
		void		BltFilterSub(int x, int y, const CFilter& filter);
		void		Blt4444(WORD* pSurface, WORD pitch);
		void		Blt4444Color(WORD* pSurface, WORD pitch, WORD color);

		//---------------------------------------------------------
		// File I/O
		//---------------------------------------------------------
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);

		//---------------------------------------------------------
		// Get
		//---------------------------------------------------------
		WORD		GetWidth() const		{ return m_Width; }
		WORD		GetHeight() const		{ return m_Height; }
		BYTE*		GetFilter(WORD y) const	{ return m_ppFilter[y]; }


		

	protected :
		WORD		m_Width;
		WORD		m_Height;
		BYTE**		m_ppFilter;
};

typedef CTypePack<CFilter>			CFilterPack;

#endif

