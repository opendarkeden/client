//----------------------------------------------------------------------
// MZoneInfo.h
//----------------------------------------------------------------------
// 안전지대 표시.. 등등..
// 자주 바뀔거 같아서.. map화일과 분리한다.
//----------------------------------------------------------------------


#ifndef	__MZONEINFO_H__
#define	__MZONEINFO_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include "MSectorInfo.h"


class MZoneInfo {
	public :
		MZoneInfo();
		~MZoneInfo();

		//------------------------------------------------------------------
		// Init / Release
		//------------------------------------------------------------------
		void		Init(int width, int height);
		void		Release();

		int			GetWidth() const	{ return m_Width; }
		int			GetHeight() const	{ return m_Height; }

		//------------------------------------------------------------------
		//
		//                  File I/O
		//
		//------------------------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);

		//------------------------------------------------------------------
		// Safety
		//------------------------------------------------------------------
		BOOL		IsSafeSector(int sX, int sY) const;
		void		SetSafeSector(int sX, int sY);
		void		UnSetSafeSector(int sX, int sY);

	protected :
		int					m_Width;
		int					m_Height;
		MSectorInfo**		m_ppInfo;
};

#endif

