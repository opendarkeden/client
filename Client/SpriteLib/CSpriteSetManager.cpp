//----------------------------------------------------------------------
// CSpriteSetManager.cpp
//----------------------------------------------------------------------		
#include "client_PCH.h"
#include "CSpriteSetManager.h"
//----------------------------------------------------------------------		
//
// member functions
//
//----------------------------------------------------------------------		
CSpriteSetManager::CSpriteSetManager()
{
}

CSpriteSetManager::~CSpriteSetManager()
{
}

//----------------------------------------------------------------------		
//
// member functions
//
//----------------------------------------------------------------------		

//----------------------------------------------------------------------
// Save FilePosition
//----------------------------------------------------------------------				
// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
//
// spkIndex는 SpritePack IndexFile이고,
// SpritePack에 있는 각각의 Sprite에 대한 File Position을 저장하고 있다.
//
// setIndex는 SpriteSet IndexFile이고, m_List에서 SpriteID를 읽어서
// 이 함수에서 저장하는데 이용할 것이다.
//----------------------------------------------------------------------
bool		
CSpriteSetManager::SaveSpriteSetIndex(ofstream& setIndex, ifstream& spkIndex)
{
	// m_List에 아무것도 없으면..
	if (m_List.size() == 0)
		return false;


	size_t	count;

	//---------------------------------------------------------------
	// SpritePack IndexFile의 Sprite개수를 읽는다.
	//---------------------------------------------------------------
	spkIndex.read((char*)&count, SIZE_SPRITEID);

	// SpritePack Index를 저장해둘 memory잡기
	long* pIndex = new long [count];

	//---------------------------------------------------------------
	// 모든 SpritePack IndexFile을 Load한다.
	//---------------------------------------------------------------
	for (TYPE_SPRITEID i=0; i<count; i++)
	{
		spkIndex.read((char*)&pIndex[i], 4);
	}

	//---------------------------------------------------------------
	// m_List의 순서대로 SpritePack Index에서 
	// 해당하는 SpriteID의 File Position을 읽어서 저장한다.
	//---------------------------------------------------------------
	DATA_LIST::iterator iData = m_List.begin();

	// SpriteSet의 Sprite개수 저장
	count = m_List.size();
	setIndex.write((const char*)&count, SIZE_SPRITEID);

	// List의 모든 node에 대해서..
	while (iData != m_List.end())
	{
		// Sprite ID에 대한 SpritePack File에서의 File Position
		setIndex.write((const char*)&pIndex[(*iData)], 4);

		iData ++;
	}

	delete [] pIndex;

	return true;
}

