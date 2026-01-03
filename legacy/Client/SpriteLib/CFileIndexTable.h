//----------------------------------------------------------------------
// CFileIndexTable.h
//----------------------------------------------------------------------
//
// Index File을 Load한다.
//
//----------------------------------------------------------------------

#ifndef	__CFILEINDEXTABLE_H__
#define	__CFILEINDEXTABLE_H__

#include <Windows.h>

class CFileIndexTable {
	public :
		CFileIndexTable();
		~CFileIndexTable();		

		//--------------------------------------------------------
		// file I/O		
		//--------------------------------------------------------		
		bool			LoadFromFile(class ifstream& indexFile);		

		WORD				GetSize()					{ return m_Size; }

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		const long&		operator [] (WORD n) { return m_pIndex[n]; }

		//--------------------------------------------------------
		// Release
		//--------------------------------------------------------
		void			Release();


	protected :
		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void			Init(WORD count);		
		

	protected :
		WORD			m_Size;				// 개수
		long*			m_pIndex;			// File position
};

#endif
