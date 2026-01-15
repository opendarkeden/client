//----------------------------------------------------------------------
// CFilterPack.h
//----------------------------------------------------------------------
//
// FilterPack의 Filter에 대한 File Pointer Index가 필요하다.
// FilterSet에서 읽기 위해서.
//
//----------------------------------------------------------------------

#ifndef	__CFILTERPACK_H__
#define	__CFILTERPACK_H__

#include "DrawTypeDef.h"
#include "CFilter.h"

class CFilterPack {
	public :
		CFilterPack();
		~CFilterPack();

		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void		Init(TYPE_FILTERID count);		
		void		Release();		
		
		//--------------------------------------------------------
		// file I/O
		//--------------------------------------------------------
		void		SaveToFile(std::ofstream& file);
		void		LoadFromFile(std::ifstream& file);		
		
		//--------------------------------------------------------
		// size
		//--------------------------------------------------------
		TYPE_FILTERID	GetSize() const		{ return m_nFilters; }

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		CFilter&	operator [] (TYPE_FILTERID n) { return m_pFilters[n]; }

	protected :
		TYPE_FILTERID		m_nFilters;		// CFilter의 개수
		CFilter*			m_pFilters;		// CFilter들을 저장해둔다.
};

#endif
	