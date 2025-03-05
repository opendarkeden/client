//----------------------------------------------------------------------
// CSpriteSet.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSpriteSurface.h"
#include "CSprite565.h"
#include "CSprite555.h"
#include "CSpritePack.h"
#include "CSpriteSet.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CSpriteSet::CSpriteSet()
{
	m_nSprites		= 0;
	m_pSprites		= NULL;
}

CSpriteSet::~CSpriteSet()
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
CSpriteSet::Init(TYPE_SPRITEID count)
{
	// 개수가 없을 경우 
	if (count==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_nSprites = count;

	if (CDirectDraw::Is565())
	{
		m_pSprites = new CSprite565 [m_nSprites];
	}
	else
	{
		m_pSprites = new CSprite555 [m_nSprites];
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CSpriteSet::Release()
{
	if (m_pSprites != NULL)
	{
		// 모든 MSprite를 지운다.
		delete [] m_pSprites;
		m_pSprites = NULL;

		m_nSprites = 0;
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// SpriteSet IndexFile을 이용해서 SpritePack File에서 
// 특정 위치의 Sprite들을 Load한다.
//----------------------------------------------------------------------
bool		
CSpriteSet::LoadFromFile(ifstream& indexFile, ifstream& packFile)
{
	TYPE_SPRITEID	count;
	
	//------------------------------------------------------
	// SpriteSet의 Sprite개수를 읽어들인다.
	//------------------------------------------------------
	indexFile.read((char*)&count, SIZE_SPRITEID);


	long* pIndex = new long [count];	// file position

	//------------------------------------------------------
	// SpriteSet IndexFile을 모두 읽어들인다.
	//------------------------------------------------------
	for (TYPE_SPRITEID i=0; i<count; i++)
	{		
		indexFile.read((char*)&pIndex[i], 4);
	}

	// Loop를 따로 사용하는 이유는 
	// 아무래도 두개의 file을 동시에 access하면
	// 느려질 것 같아서... 정말일까? - -;;


	//------------------------------------------------------
	// Sprite를 Load할 memory를 잡는다.
	//------------------------------------------------------
	Init( count );

	//------------------------------------------------------
	// Index(File Position)를 이용해서 SpritePack에서
	// 특정 Sprite들을 Load한다.
	//------------------------------------------------------
	for (int i=0; i<count; i++)
	{
		packFile.seekg(pIndex[i], ios::beg);
		m_pSprites[i].LoadFromFile( packFile );
	}

	delete [] pIndex;

	return true;
}

