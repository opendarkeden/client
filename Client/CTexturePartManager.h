//----------------------------------------------------------------------
// CTexturePartManager.h
//----------------------------------------------------------------------
// 특정File(ASPK)에 대해서
// FileIndex(ASPKI)를 참조해서
// 일부의 AlphaSprite를 Load해서 사용하게 하는 class
//----------------------------------------------------------------------

#ifndef	__CTEXTUREPARTMANAGER_H__
#define	__CTEXTUREPARTMANAGER_H__

#include "CPartManager.h"
#include "CSpriteSurface.h"
#include <list>
#include <map>
//std::ifstream;
//class CFileIndexTable;
//class CAlphaSpritePack;
#include "CAlphaSpritePal.h"
#include "MPalettePack.h"

class CTexturePartManager : public CPartManager<WORD, WORD, CSpriteSurface*>
{
	public :
		typedef std::list<CSpriteSurface*>	SPRITESURFACE_LIST;
		typedef std::map<int, int>			SPRITE_PALETTE;			// 사용한 스프라이트 아이디가 마지막에 사용한 파레트 번호

	public :
		//CTexturePartManager(CAlphaSpritePack* pASPK, WORD partSize=100);
		CTexturePartManager(const char* aspkFilename, WORD partSize=100);
		~CTexturePartManager();

		//---------------------------------------------------
		// Init / Release
		//---------------------------------------------------
		//void				Init(CAlphaSpritePack* pASPK, WORD partSize);
		void				Init(const char* aspkFilename, WORD partSize);
		void				Release();
	
		// data내용을 초기화 시킨다.
		void				Clear();

		//---------------------------------------------------
		// aspk의 id번째 AlphaSprite를 넘겨받는다.
		//---------------------------------------------------
		CSpriteSurface*		GetTexture(TYPE_SPRITEID id, int index);

		// AlphaSprite의 원래 크기를 넘겨준다.
		int					GetWidth(TYPE_SPRITEID id)	{ return m_pWidth[id]; }
		int					GetHeight(TYPE_SPRITEID id)	{ return m_pHeight[id]; }

		int					GetSpriteWidth(TYPE_SPRITEID id)	{ return m_ASPK[id].GetWidth(); }
		int					GetSpriteHeight(TYPE_SPRITEID id)	{ return m_ASPK[id].GetHeight(); }

		//---------------------------------------------------
		// 지워야 할 것들
		//---------------------------------------------------
		void				DeleteRemoved();
		int					GetUsedPalette(int spriteID);

	protected :
//		bool				m_bOpen;		// ASPK File을 Open했는가?
//		std::ifstream		m_ASPKFile;		// ASPK File
//		CFileIndexTable		m_ASPKIndex;	// ASPK Index 정보
		CAlphaSpritePalPack	m_ASPK;			// ASPK

		//CAlphaSpritePack	*m_pASPK;			// ASPK
		
		// Texture 출력할때의 크기
		int					*m_pWidth;
		int					*m_pHeight;

		// 지워야할 것들..
		SPRITESURFACE_LIST	m_listRemoved;
		SPRITE_PALETTE		m_historySprPal;
		MPalettePack		m_EffectAlphaPPK;
};


#endif

