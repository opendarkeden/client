//----------------------------------------------------------------------
// CFrameSetManager.cpp
//----------------------------------------------------------------------
#include "Frame_PCH.h"
#include "CFrameSetManager.h"
//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CFrameSetManager::CFrameSetManager()
{
}

CFrameSetManager::~CFrameSetManager()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Save FrameSet Index
//----------------------------------------------------------------------
// FramePack IndexFile로부터 FrameSet IndexFile을 생성한다.
//----------------------------------------------------------------------
bool		
CFrameSetManager::SaveFrameSetIndex(ofstream& setIndex, ifstream& packIndex)
{
	// m_List에 아무것도 없으면..
	if (m_List.size() == 0)
		return false;


	TYPE_FRAMEID	count;

	//---------------------------------------------------------------
	// FramePack IndexFile의 Frame개수를 읽는다.
	//---------------------------------------------------------------
	packIndex.read((char*)&count, SIZE_FRAMEID);

	// FramePack Index를 저장해둘 memory잡기
	long* pIndex = new long [count];

	//---------------------------------------------------------------
	// 모든 FramePack IndexFile을 Load한다.
	//---------------------------------------------------------------
	for (TYPE_FRAMEID i=0; i<count; i++)
	{
		packIndex.read((char*)&pIndex[i], 4);
	}

	//---------------------------------------------------------------
	// m_List의 순서대로 FramePack Index에서 
	// 해당하는 FrameID의 File Position을 읽어서 저장한다.
	//---------------------------------------------------------------
	DATA_LIST::iterator iData = m_List.begin();

	// FrameSet의 Frame개수 저장
	count = m_List.size();
	setIndex.write((const char*)&count, SIZE_FRAMEID);

	// List의 모든 node에 대해서..
	while (iData != m_List.end())
	{
		// Frame ID에 대한 FramePack File에서의 File Position
		setIndex.write((const char*)&pIndex[(*iData)], 4);

		iData ++;
	}

	delete [] pIndex;

	return true;
}

