//----------------------------------------------------------------------
// CAlphaSpritePack.cpp
//----------------------------------------------------------------------

#include "CSpriteSurface.h"
#include "CAlphaSprite555.h"
#include "CAlphaSprite565.h"
#include "CAlphaSpritePack.h"
#include <fstream.h>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CAlphaSpritePack::CAlphaSpritePack()
{
	m_nSprites = 0;
	m_pSprites = NULL;
}

CAlphaSpritePack::~CAlphaSpritePack()
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
CAlphaSpritePack::Init(TYPE_SPRITEID count, bool b565)
{
	// 개수가 없을 경우 
	if (count==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_nSprites = count;

	if (b565)
	{
		m_pSprites = new CAlphaSprite565 [m_nSprites];
	}
	else
	{
		m_pSprites = new CAlphaSprite555 [m_nSprites];
	}
}


//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CAlphaSpritePack::Release()
{
	if (m_pSprites != NULL)
	{
		// 모든 CAlphaSprite를 지운다.
		delete [] m_pSprites;
		m_pSprites = NULL;
		
		m_nSprites = 0;
	}
}

//----------------------------------------------------------------------
// Release Part
//----------------------------------------------------------------------
// firstSpriteID ~ lastSpriteID까지만 메모리에서 제거한다.
//----------------------------------------------------------------------
void			
CAlphaSpritePack::ReleasePart(TYPE_SPRITEID firstSpriteID, TYPE_SPRITEID lastSpriteID)
{
	// SpritePack의 memory가 잡혀있지 않으면 그냥 return한다.	
	if (m_pSprites==NULL
		|| firstSpriteID >= m_nSprites)
		return;

	int last = min(lastSpriteID, m_nSprites-1);

	for (int id=firstSpriteID; id<=last; id++)
	{
		m_pSprites[id].Release();
	}
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// map 전체를 따라가면서 file에 저장해야한다.
//----------------------------------------------------------------------
bool		
CAlphaSpritePack::SaveToFile(ofstream& spkFile, ofstream& indexFile)
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
		// CAlphaSprite 내부적으로 길이만 저장하므로 
		// 다음에 Load할 때 문제가 없을 것이다.

		m_pSprites[i].SaveToFile(spkFile);		// CAlphaSprite저장	
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
CAlphaSpritePack::SaveToFileSpriteOnly(ofstream& spkFile, long &filePosition)
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
		// CSprite 내부적으로 길이만 저장하므로 
		// 다음에 Load할 때 문제가 없을 것이다.

		m_pSprites[i].SaveToFile(spkFile);		// CSprite저장	
	}

	return true;
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// file에서 ID와 Sprite를 읽어와서 map에 하나씩 저장한다.
//----------------------------------------------------------------------
void
CAlphaSpritePack::LoadFromFile(ifstream& file)
{
	// memory에서 map제거
	Release();

	// file에서 sprite 개수를 읽어온다.	
	file.read((char*)&m_nSprites, SIZE_SPRITEID);

	// memory잡는다.
	Init(m_nSprites, CDirectDraw::Is565());

	// file에 있는 Sprite들을 Load	
	for (TYPE_SPRITEID i=0; i<m_nSprites; i++)
	{			
		m_pSprites[i].LoadFromFile(file);	// Sprite 읽어오기
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
CAlphaSpritePack::LoadFromFilePart(ifstream& file, long filePosition,
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
		// 아직 Load되지 않은 경우에만 Load한다.
		//if (m_pSprites[id].IsInit())
		//	continue;
		// file position을 이동시켜줘야 하므로 무조건 loading해야 한다.

		m_pSprites[id].LoadFromFile( file );
	}
}

//----------------------------------------------------------------------
// LoadFromFile Sprite
//----------------------------------------------------------------------
// indexFile을 이용해서 spkFile에서 spriteID번째 sprite를 읽어온다.
//----------------------------------------------------------------------
bool
CAlphaSpritePack::LoadFromFileSprite(int spriteID, int fileSpriteID, std::ifstream& spkFile, std::ifstream& indexFile)
{
	if (spriteID < 0 || spriteID >= m_nSprites)
	{
		return false;
	}

	//-------------------------------------------------------------------
	// index의 개수를 체크한다. fileSpriteID가 있는지..?
	//-------------------------------------------------------------------
	TYPE_SPRITEID num;
	indexFile.read((char*)&num, SIZE_SPRITEID);

	if (fileSpriteID >= num)
	{
		return false;
	}

	//-------------------------------------------------------------------
	// load할 sprite의 file pointer를 읽는다.
	//-------------------------------------------------------------------
	long fp;	
	indexFile.seekg( 2 + fileSpriteID*4 );		// 2(num) + spriteID * (4 bytes)
	indexFile.read((char*)&fp, 4);

	//-------------------------------------------------------------------
	// minimap sprite loading
	//-------------------------------------------------------------------
	spkFile.seekg( fp );	

	m_pSprites[spriteID].LoadFromFile( spkFile );

	return true;
}

//----------------------------------------------------------------------
// LoadFromFile Sprite
//----------------------------------------------------------------------
// indexFile을 이용해서 spkFile에서 spriteID번째 sprite를 읽어온다.
//----------------------------------------------------------------------
bool
CAlphaSpritePack::LoadFromFileSprite(int spriteID, int fileSpriteID, const char* spkFilename, const char* indexFilename)
{
	if (spriteID < 0 || spriteID >= m_nSprites)
	{
		return false;
	}

	std::ifstream spkFile(spkFilename, ios::binary | );

	if (!spkFile.is_open())
	{
		return false;
	}

	std::ifstream indexFile(indexFilename, ios::binary | );

	if (!indexFile.is_open())
	{
		return false;
	}

	return LoadFromFileSprite( spriteID, fileSpriteID, spkFile, indexFile );
}