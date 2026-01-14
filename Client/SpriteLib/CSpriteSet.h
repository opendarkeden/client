//----------------------------------------------------------------------
// CSpriteSet.h
//----------------------------------------------------------------------
//
// SpritePack에서 특정 Sprite만 Load할 수 있게 한다.
//
// SpriteSet file(index용)에서 정보를 읽어서 
// SpritePack에서 특정한 Sprite만 읽어들인다.
//
// SpriteSet IndexFile의 정보를 이용해서 SpritePack에서 
// 특정 위치(File Position)의 Sprite를 Load한다.
//
//----------------------------------------------------------------------

#ifndef	__CSPRITESET_H__
#define	__CSPRITESET_H__


#include "DrawTypeDef.h"
#include "CSpritePack.h"


class CSpriteSet {
	public :
		CSpriteSet();
		~CSpriteSet();

		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void		Init(TYPE_SPRITEID count);		
		void		Release();

		//--------------------------------------------------------
		// file I/O		
		//--------------------------------------------------------
		// SpritePack File에서 Sprite를 Load한다.
		// spkFile = SpritePack File, indexFile = FilePointer File
		bool		LoadFromFile(std::ifstream& indexFile, std::ifstream& packFile);
		

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		CSprite&	operator [] (TYPE_SPRITEID n) { return m_pSprites[n]; }


	protected :
		TYPE_SPRITEID		m_nSprites;			// Sprite ID의 개수
		CSprite*			m_pSprites;			// MSpritePack의 CSprite들을 Load한다.
};

#endif

