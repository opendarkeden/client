//----------------------------------------------------------------------
// CStorageSurface.h
//----------------------------------------------------------------------
// Surface의 일부분을 기억하고 있다가 
// 필요한때에 다시 기억된 내용은 복구시켜서 사용할 수 있게 한다.
//----------------------------------------------------------------------

#ifndef	__CSTORAGESURFACE_H__
#define	__CSTORAGESURFACE_H__


#pragma warning(disable:4786)


#include <Windows.h>
class CDirectDrawSurface;


class CStorageSurface {
	public :
		CStorageSurface();
		~CStorageSurface();

		//--------------------------------------------------------
		// Init / Release
		//--------------------------------------------------------
		// (Surface개수, width, height)
		void					Init(int size, int width, int height);
		void					Release();

		//--------------------------------------------------------
		// Store / Restore		
		//--------------------------------------------------------
		void					Store(int i, CDirectDrawSurface* pSurface, POINT* pPoint);
		void					Restore(int i, CDirectDrawSurface* pSurface, POINT* pPoint=NULL) const;

		//--------------------------------------------------------
		// Get
		//--------------------------------------------------------
		int						GetSize() const		{ return m_Size; }

	protected :
		int						m_Size;				// surface개수
		POINT*					m_pPoint;			// surface에 기억된 위치
		CDirectDrawSurface*		m_pStorageSurface;	// 기억된 surface내용
};


#endif
