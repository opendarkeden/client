//----------------------------------------------------------------------
// CTexturePack.h
//----------------------------------------------------------------------

#ifndef	__CTEXTUREPACK_H__
#define	__CTEXTUREPACK_H__

#include "CSpriteSurface.h"
#include "CAlphaSpritePack.h"
#include "CShadowSprite.h"
//#include "CShadowSpritePack.h"

class CTexturePack {
	public :
		CTexturePack();
		~CTexturePack();

		//--------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------
		void			Init(int count);
		bool			Init( CAlphaSpritePack& ASPK );		
		void			Release();

		//--------------------------------------------------------
		// Part
		//--------------------------------------------------------
		bool			InitPart( CShadowSpritePack& SSPK, TYPE_SPRITEID firstShadowSpriteID, TYPE_SPRITEID lastShadowSpriteID );

		//--------------------------------------------------------
		// file I/O
		//--------------------------------------------------------
		//void			SaveToFile(class ofstream& spkFile, class ofstream& indexFile);
		//void			LoadFromFile(class ifstream& file);		
		
		//--------------------------------------------------------
		// size
		//--------------------------------------------------------
		int	GetSize() const		{ return m_nTextures; }

		//--------------------------------------------------------
		// operator
		//--------------------------------------------------------
		CSpriteSurface&		operator [] (int n) { return m_pTextures[n]; }

	protected :
		int					m_nTextures;		// Texture의 개수
		CSpriteSurface*		m_pTextures;		// Texture들을 저장해둔다.		
};

#endif


