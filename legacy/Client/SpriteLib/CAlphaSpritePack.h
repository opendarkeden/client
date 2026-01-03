//----------------------------------------------------------------------
// CAlphaSpritePack.h
//----------------------------------------------------------------------
//
// SpritePack의 Sprite에 대한 File Pointer Index가 필요하다.
// SpriteSet에서 읽기 위해서.
//
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITEPACK_H__
#define	__CALPHASPRITEPACK_H__

#include "DrawTypeDef.h"
#include "CAlphaSprite565.h"
#include "CAlphaSprite555.h"

class CAlphaSpritePack {
	public :
		CAlphaSpritePack();
		~CAlphaSpritePack();

		//------------------------------------------------------------
		// Init/Release
		//------------------------------------------------------------
		void		Init(TYPE_SPRITEID count, bool b565);		
		void		Release();
		void		ReleasePart(TYPE_SPRITEID firstSpriteID, TYPE_SPRITEID lastSpriteID);


		//------------------------------------------------------------
		// file I/O
		//------------------------------------------------------------
		bool		SaveToFile(class ofstream& spkFile, class ofstream& indexFile);
		bool		SaveToFileSpriteOnly(class ofstream& spkFile, long &filePosition);
		void		LoadFromFile(class ifstream& file);		
		void		LoadFromFilePart(class ifstream& file, long filePosition,
									 TYPE_SPRITEID firstSpriteID, TYPE_SPRITEID lastSpriteID);

		bool		LoadFromFileSprite(int spriteID, int fileSpriteID, class ifstream& spkFile, class ifstream& indexFile);
		bool		LoadFromFileSprite(int spriteID, int fileSpriteID, const char* spkFilename, const char* indexFilename);

		//--------------------------------------------------------
		// size
		//--------------------------------------------------------
		TYPE_SPRITEID	GetSize() const		{ return m_nSprites; }

		//------------------------------------------------------------
		// operator
		//------------------------------------------------------------
		CAlphaSprite&	operator [] (TYPE_SPRITEID n) { return m_pSprites[n]; }

	protected :
		TYPE_SPRITEID	m_nSprites;		// CAlphaSprite의 개수
		CAlphaSprite*	m_pSprites;		// CAlphaSprite들을 저장해둔다.
};

#endif


