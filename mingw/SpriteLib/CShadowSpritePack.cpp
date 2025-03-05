//----------------------------------------------------------------------
// CShadowSpritePack.cpp
//----------------------------------------------------------------------

#include "CSpriteSurface.h"
#include "CShadowSprite.h"
#include "CShadowSpritePack.h"
#include <fstream>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CShadowSpritePack::CShadowSpritePack()
{
	m_nSprites = 0;
	m_pSprites = NULL;
}

CShadowSpritePack::~CShadowSpritePack()
{
	// array를 메모리에서 제거한다.
	Release();
	
	m_listLoad.clear();
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
CShadowSpritePack::Init(TYPE_SPRITEID count)
{
	// 개수가 없을 경우 
	if (count==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_nSprites = count;

	m_pSprites = new CShadowSprite [m_nSprites];
}


//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CShadowSpritePack::Release()
{
	if (m_pSprites != NULL)
	{
		// 모든 CShadowSprite를 지운다.
		delete [] m_pSprites;
		m_pSprites = NULL;
		
		m_nSprites = 0;

		m_listLoad.clear();
	}
}

//----------------------------------------------------------------------
// Release Part
//----------------------------------------------------------------------
// firstSpriteID ~ lastSpriteID까지만 메모리에서 제거한다.
//----------------------------------------------------------------------
void			
CShadowSpritePack::ReleasePart(TYPE_SPRITEID firstSpriteID, TYPE_SPRITEID lastSpriteID)
{
	// SpritePack의 memory가 잡혀있지 않으면 그냥 return한다.	
	if (m_pSprites==NULL
		|| firstSpriteID >= m_nSprites)
		return;

	int last = min(lastSpriteID, m_nSprites-1);

	for (TYPE_SPRITEID id=firstSpriteID; id<=last; id++)
	{
		m_pSprites[id].Release();

		// m_listLoad에서도 지워야 하는데,
		// 느릴 수 있어서... 뺏다. - -;;		
	}
}

//----------------------------------------------------------------------
// Release Loaded
//----------------------------------------------------------------------
// m_listLoad에는 현재 Load된 SpriteID가 있다.
// 이거를 모두 Release해줘야 한다.
//----------------------------------------------------------------------
void
CShadowSpritePack::ReleaseLoaded()
{
	INT_LIST::iterator iID = m_listLoad.begin();

	while (iID != m_listLoad.end())
	{
		m_pSprites[*iID].Release();

		iID ++;
	}

	m_listLoad.clear();
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// map 전체를 따라가면서 file에 저장해야한다.
//----------------------------------------------------------------------
bool		
CShadowSpritePack::SaveToFile(ofstream& spkFile, ofstream& indexFile)
{
	// 초기화되지 않았으면 
	if (m_nSprites==0 || m_pSprites==NULL)
		return false;
	
	//--------------------------------------------------
	// index file을 생성하기 위한 정보
	//--------------------------------------------------
	long*	pIndex = new long [m_nSprites];


	//--------------------------------------------------
	// Size 저장
	//--------------------------------------------------
	spkFile.write((const char *)&m_nSprites, SIZE_SPRITEID); 
	indexFile.write((const char *)&m_nSprites, SIZE_SPRITEID); 

	//--------------------------------------------------
	//
	// SpritePack에 Array의 모든 Sprite를 저장한다.
	//
	//--------------------------------------------------
	for (TYPE_SPRITEID i=0; i<m_nSprites; i++)
	{
		// SpritePack file에 쓰여지는 index를 저장
		pIndex[i] = spkFile.tellp();

		// m_pSprites[i]에 저장된 pixel이 없어도
		// CShadowSprite 내부적으로 길이만 저장하므로 
		// 다음에 Load할 때 문제가 없을 것이다.

		m_pSprites[i].SaveToFile(spkFile);		// CShadowSprite저장	
	}

	//--------------------------------------------------
	// index 저장
	//--------------------------------------------------
	for (i=0; i<m_nSprites; i++)
	{
		indexFile.write((const char*)&pIndex[i], 4);
	}

	delete [] pIndex;

	return true;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// map 전체를 따라가면서 file에 저장해야한다.
//----------------------------------------------------------------------
bool
CShadowSpritePack::SaveToFileSpriteOnly(ofstream& spkFile, long &filePosition)
{
	// 초기화되지 않았으면 
	if (m_nSprites==0 || m_pSprites==NULL)
		return false;

	// SpritePack file에 쓰여지는 index를 저장
	filePosition = spkFile.tellp();
	
	//--------------------------------------------------
	//
	// SpritePack에 Array의 모든 Sprite를 저장한다.
	//
	//--------------------------------------------------
	for (TYPE_SPRITEID i=0; i<m_nSprites; i++)
	{
		// m_pSprites[i]에 저장된 pixel이 없어도
		// CShadowSprite 내부적으로 길이만 저장하므로 
		// 다음에 Load할 때 문제가 없을 것이다.

		m_pSprites[i].SaveToFile(spkFile);		// CShadowSprite저장	
	}

	return true;
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// file에서 ID와 Sprite를 읽어와서 하나씩 저장한다.
//----------------------------------------------------------------------
void
CShadowSpritePack::LoadFromFile(ifstream& file)
{
	// memory에서 map제거
	Release();

	// file에서 sprite 개수를 읽어온다.	
	file.read((char*)&m_nSprites, SIZE_SPRITEID);

	// memory잡는다.
	Init(m_nSprites);

	// file에 있는 Sprite들을 Load	
	for (TYPE_SPRITEID i=0; i<m_nSprites; i++)
	{			
		m_pSprites[i].LoadFromFile(file);	// Sprite 읽어오기

		// load한 것들을 list에 유지한다. (이거는 필요없을거도 같은데..)
		//m_listLoad.push_back( id );
	}
}

//----------------------------------------------------------------------
// Load From File Part
//----------------------------------------------------------------------
// file에서 일부의 Sprite들만 읽어들인다.
// 
// file의 filePosition에서부터 읽어들이고..
// FirstSpriteID부터 SpriteSize개만큼만 읽어들인다.
//----------------------------------------------------------------------
void			
CShadowSpritePack::LoadFromFilePart(ifstream& file, long filePosition,
							  TYPE_SPRITEID firstSpriteID, TYPE_SPRITEID lastSpriteID)
{
	if (firstSpriteID==SPRITEID_NULL || lastSpriteID==SPRITEID_NULL)
		return;

	// SpritePack의 memory가 잡혀있지 않으면 그냥 return한다.	
	if (m_pSprites==NULL)
		return;

	// Load할려는 위치까지 FilePosition을 이동한다.
	file.seekg( filePosition, ios::beg );

	// firstSpriteID ~ lastSpriteID까지의 Sprite를 Load한다.
	for (TYPE_SPRITEID id=firstSpriteID; id<=lastSpriteID; id++)
	{
		m_pSprites[id].LoadFromFile( file );

		// load된 sprite list에 등록.
		m_listLoad.push_back( id );
	}
}

//----------------------------------------------------------------------
// Load From File Part
//----------------------------------------------------------------------
// file에서 일부의 Sprite들만 읽어들인다.
// 
// fpArray의 (SpriteID, FilePosition)을 이용해서
// SpriteID의 Sprite를 FilePosition에서 읽어들인다.
//
//----------------------------------------------------------------------
void			
CShadowSpritePack::LoadFromFilePart(ifstream& spkFile, const CSpriteFilePositionArray& fpArray)
{
	// SpritePack의 memory가 잡혀있지 않으면 그냥 return한다.	
	if (m_pSprites==NULL)
		return;

	for (int i=0; i<fpArray.GetSize(); i++)
	{
		// Load할려는 위치까지 FilePosition을 이동한다.
		spkFile.seekg( fpArray[i].FilePosition, ios::beg );

		// Sprite를 Load한다.
		m_pSprites[fpArray[i].SpriteID].LoadFromFile( spkFile );

		// load된 sprite list에 등록.
		m_listLoad.push_back( fpArray[i].SpriteID );
	}
}

//----------------------------------------------------------------------
// Init( IndexSpritepack )
//----------------------------------------------------------------------
// IndexSpritePack --> ShadowSpritePack
//----------------------------------------------------------------------
void			
CShadowSpritePack::InitPart(CIndexSpritePack& ISPK, TYPE_SPRITEID firstShadowSpriteID, TYPE_SPRITEID lastShadowSpriteID)
{
	// 모든 IndexSprite를 이용해서 ShadowSprite를 만든다.
	for (int i=firstShadowSpriteID; i<lastShadowSpriteID; i++)
	{
		m_pSprites[i].SetPixel( ISPK[i] );
	}
}
