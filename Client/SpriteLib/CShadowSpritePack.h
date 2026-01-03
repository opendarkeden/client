//----------------------------------------------------------------------
// CShadowSpritePack.h
//----------------------------------------------------------------------
// 여러개(많이~~)의 ShadowSprite Image들을 저장해두고 필요한 것을
// ShadowSpriteID를 통해서 읽어서 사용하게 된다.
// 내부적으로 std::map을 사용했...으나
//
//----------------------------------------------------------------------
// [ Test결과 ]
//
// CShadowSpritePack에 170개의 CShadowSprite가 있을 때,
//
// map  사용 시  : 43 Frame
// Array 사용 시 : 49 Frame
//
// 그래서, map의 속도 문제도 인해서 array를 사용하기로 했다. - -;
//
//----------------------------------------------------------------------
// 
// [ 사용법 ]
//
// CShadowSpritePack sp;
//
// sp.Init( 3 );
//
// sp[0].SetPixel( ... );
// sp[1].SetPixel( ... );
// sp[2].SetPixel( ... );
//
// Surface->CopyShadowSprite( sp[1], ... )
//
//----------------------------------------------------------------------
//
// ShadowSpritePack의 ShadowSprite에 대한 File Pointer Index가 필요하다.
// ShadowSpriteSet에서 읽기 위해서.
//
//----------------------------------------------------------------------

#ifndef	__CSHADOWSPRITEPACK_H__
#define	__CSHADOWSPRITEPACK_H__

#include "DrawTypeDef.h"
#include "CShadowSprite.h"
#include "CIndexSpritePack.h"
#include <list>
class ofstream;
class ifstream;

class CShadowSpritePack {
	public :
		CShadowSpritePack();
		~CShadowSpritePack();

		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void			Init(TYPE_SPRITEID count);				
		void			Release();
		void			ReleasePart(TYPE_SPRITEID firstShadowSpriteID, TYPE_SPRITEID lastShadowSpriteID);
		void			ReleaseLoaded();

		//--------------------------------------------------------
		// file I/O
		//--------------------------------------------------------
		bool			SaveToFile(class ofstream& spkFile, class ofstream& indexFile);
		bool			SaveToFileSpriteOnly(class ofstream& spkFile, long &filePosition);
		void			LoadFromFile(class ifstream& file);		
		void			LoadFromFilePart(class ifstream& file, long filePosition,
										 TYPE_SPRITEID firstShadowSpriteID, TYPE_SPRITEID lastShadowSpriteID);
		void			LoadFromFilePart(class ifstream& spkFile, const CSpriteFilePositionArray& fpArray);

		//--------------------------------------------------------
		// Convert
		//--------------------------------------------------------
		void			InitPart(CIndexSpritePack& ISPK, TYPE_SPRITEID firstShadowSpriteID, TYPE_SPRITEID lastShadowSpriteID);

		//--------------------------------------------------------
		// size
		//--------------------------------------------------------
		TYPE_SPRITEID	GetSize() const		{ return m_nSprites; }

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		CShadowSprite&		operator [] (TYPE_SPRITEID n) { return m_pSprites[n]; }

	protected :
		TYPE_SPRITEID		m_nSprites;		// CShadowSprite의 개수
		CShadowSprite*		m_pSprites;		// CShadowSprite들을 저장해둔다.

		INT_LIST		m_listLoad;		// Load된 ShadowSprite들에 대한 ID
		
};

#endif


