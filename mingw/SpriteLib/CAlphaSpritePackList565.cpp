//----------------------------------------------------------------------
// CAlphaSpritePackList565.cpp
//----------------------------------------------------------------------

#include "CAlphaSprite565.h"
#include "CAlphaSpritePackList.h"
#include "CAlphaSpritePackList565.h"
#include <fstream.h>

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
// file에서 ID와 Sprite를 읽어와서 list 하나씩 저장한다.
//----------------------------------------------------------------------
void		
CAlphaSpritePackList565::LoadFromFile(ifstream& file)
{
	// memory에서 list제거
	Release();

	// file에서 sprite 개수를 읽어온다.	
	TYPE_SPRITEID size;
	file.read((char*)&size, SIZE_SPRITEID);


	CAlphaSprite* pSprite;

	// file에 있는 Sprite들을 Load	
	for (TYPE_SPRITEID i=0; i<size; i++)	
	{			
		// 메모리 잡기
		pSprite = new CAlphaSprite565;

		pSprite->LoadFromFile(file);	// Sprite 읽어오기

		// List에 추가
		AddSprite(pSprite);
	}
}

