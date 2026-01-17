//----------------------------------------------------------------------
// CFilterPack.cpp
//----------------------------------------------------------------------

#include "CFilter.h"
#include "CFilterPack.h"
#include <fstream>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CFilterPack::CFilterPack()
{
	m_nFilters = 0;
	m_pFilters = NULL;
}

CFilterPack::~CFilterPack()
{
	// array를 메모리에서 제거한다.
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
CFilterPack::Init(TYPE_FILTERID count)
{
	// 개수가 없을 경우 
	if (count==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_nFilters = count;

	m_pFilters = new CFilter [m_nFilters];
}


//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CFilterPack::Release()
{
	if (m_pFilters != NULL)
	{
		// 모든 CFilter를 지운다.
		delete [] m_pFilters;
		m_pFilters = NULL;
		
		m_nFilters = 0;
	}
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void
CFilterPack::SaveToFile(ofstream& file)
{
	// 개수 저장
	file.write((const char*)&m_nFilters, SIZE_FILTERID);

	// 저장된 것이 없으면 return
	if (m_nFilters==0 || m_pFilters==NULL)
		return;

	// 각각의 Filter를 File에 저장한다.
	for (TYPE_FILTERID i=0; i<m_nFilters; i++)
	{
		m_pFilters[i].SaveToFile( file );
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void		
CFilterPack::LoadFromFile(ifstream& file)
{
	// 개수 읽어오기
	file.read((char*)&m_nFilters, SIZE_FILTERID);

	// 없으면 return
	if (m_nFilters==0)
		return;

	// memory잡기
	Init( m_nFilters );

	// 각각의 Filter를 Load해 온다.
	for (TYPE_FILTERID i=0; i<m_nFilters; i++)
	{
		m_pFilters[i].LoadFromFile( file );
	}
}
