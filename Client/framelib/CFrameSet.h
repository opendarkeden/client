//----------------------------------------------------------------------
// CFrameSet.h
//----------------------------------------------------------------------
//
// FramePack에서 특정 Frame만 Load할 수 있게 한다.
//
// FrameSet file(index용)에서 정보를 읽어서 
// FramePack에서 특정한 Frame만 읽어들인다.
//
// FrameSet IndexFile의 정보를 이용해서 FramePack에서 
// 특정 위치(File Position)의 Frame를 Load한다.
//
//----------------------------------------------------------------------

#ifndef	__CFRAMESET_H__
#define	__CFRAMESET_H__

#include "CFrame.h"
class ifstream;
class ofstream;


template <class Type>
class CFrameSet {
	public :
		CFrameSet();
		~CFrameSet();

		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void		Init(TYPE_FRAMEID count);		
		void		Release();

		//--------------------------------------------------------
		// file I/O		
		//--------------------------------------------------------
		// FramePack File에서 Frame를 Load한다.
		// indexFile = FilePointer File, packFile = FramePack File
		bool		LoadFromFile(class ifstream& indexFile, class ifstream& packFile);
		

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		Type&	operator [] (TYPE_FRAMEID n) { return m_pFrames[n]; }


	protected :
		TYPE_FRAMEID		m_nFrames;			// Frame ID의 개수
		Type*				m_pFrames;			// Type의 Set
};


//----------------------------------------------------------------------
// CFrameSet.cpp
//----------------------------------------------------------------------

//#include "CFramePack.h"
//#include "CFrameSet.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
CFrameSet<Type>::CFrameSet()
{
	m_nFrames		= 0;
	m_pFrames		= NULL;
}

template <class Type>
CFrameSet<Type>::~CFrameSet()
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
template <class Type>
void
CFrameSet<Type>::Init(TYPE_FRAMEID count)
{
	// 개수가 없을 경우 
	if (count==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_nFrames = count;

	m_pFrames = new Type [m_nFrames];
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class Type>
void
CFrameSet<Type>::Release()
{
	if (m_pFrames != NULL)
	{
		// 모든 MFrame를 지운다.
		delete [] m_pFrames;
		m_pFrames = NULL;

		m_nFrames = 0;
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// FrameSet IndexFile을 이용해서 FramePack File에서 
// 특정 위치의 Frame들을 Load한다.
//----------------------------------------------------------------------
template <class Type>
bool		
CFrameSet<Type>::LoadFromFile(class ifstream& indexFile, class ifstream& packFile)
{
	TYPE_FRAMEID	count;
	
	//------------------------------------------------------
	// FrameSet의 Frame개수를 읽어들인다.
	//------------------------------------------------------
	indexFile.read((char*)&count, SIZE_FRAMEID);


	long* pIndex = new long [count];	// file position

	//------------------------------------------------------
	// FrameSet IndexFile을 모두 읽어들인다.
	//------------------------------------------------------
	for (TYPE_FRAMEID i=0; i<count; i++)
	{
		indexFile.read((char*)&pIndex[i], 4);
	}

	// Loop를 따로 사용하는 이유는 
	// 아무래도 두개의 file을 동시에 access하면
	// 느려질 것 같아서... 정말일까? - -;;


	//------------------------------------------------------
	// Frame를 Load할 memory를 잡는다.
	//------------------------------------------------------
	Init( count );

	//------------------------------------------------------
	// Index(File Position)를 이용해서 FramePack에서
	// 특정 Frame들을 Load한다.
	//------------------------------------------------------
	for (i=0; i<count; i++)
	{
		packFile.seekg(pIndex[i], ios::beg);
		m_pFrames[i].LoadFromFile( packFile );
	}

	delete [] pIndex;

	return true;
}


//----------------------------------------------------------------------
// FrameSet을 define한다.
//----------------------------------------------------------------------
typedef CFrameSet<FRAME_ARRAY>			CThingFrameSet;
typedef	CFrameSet<ACTION_FRAME_ARRAY>	CCreatureFrameSet;


#endif

