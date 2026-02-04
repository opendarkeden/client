//----------------------------------------------------------------------
// CSpriteTexturePartManager.h
//----------------------------------------------------------------------
// 특정File(SPK)에 대해서
// FileIndex(SPKI)를 참조해서
// 일부의 Sprite를 Load해서 사용하게 하는 class
//----------------------------------------------------------------------

#ifndef	__CSPRITETEXTUREPARTMANAGER_H__
#define	__CSPRITETEXTUREPARTMANAGER_H__

#include "CPartManager.h"
#include "SpriteLib/CSpriteSurface.h"
#include <list>
#include <map>
//std::ifstream;
//class CFileIndexTable;
#include "SpriteLib/CSpritePack.h"
#include "SpriteLib/CSpritePal.h"
#include "SpriteLib/MPalettePack.h"

class CSpriteTexturePartManager : public CPartManager<WORD, WORD, CSpriteSurface*>
{
	public :
		typedef std::list<CSpriteSurface*>	SPRITESURFACE_LIST;
		typedef std::map<int, int>			SPRITE_PALETTE;			// 사용한 스프라이트 아이디가 마지막에 사용한 파레트 번호

	public :
		//CTexturePartManager(CAlphaSpritePack* pSPK, WORD partSize=100);
		CSpriteTexturePartManager(const char* spkFilename, WORD partSize=100);
		~CSpriteTexturePartManager();

		//---------------------------------------------------
		// Init / Release
		//---------------------------------------------------
		//void				Init(CAlphaSpritePack* pSPK, WORD partSize);
		void				Init(const char* spkFilename, WORD partSize);
		void				Release();
	
		// data내용을 초기화 시킨다.
		void				Clear();

		//---------------------------------------------------
		// SPK의 id번째 AlphaSprite를 넘겨받는다.
		//---------------------------------------------------
		CSpriteSurface*		GetTexture(TYPE_SPRITEID id, int index);

		// AlphaSprite의 원래 크기를 넘겨준다.
		int					GetWidth(TYPE_SPRITEID id)	{ return m_pWidth[id]; }
		int					GetHeight(TYPE_SPRITEID id)	{ return m_pHeight[id]; }

		int					GetSpriteWidth(TYPE_SPRITEID id)	{ return m_SPK[id].GetWidth(); }
		int					GetSpriteHeight(TYPE_SPRITEID id)	{ return m_SPK[id].GetHeight(); }

		//---------------------------------------------------
		// 지워야 할 것들
		//---------------------------------------------------
		void				DeleteRemoved();

		int					GetUsedPalette(int spriteID);

	protected :
//		bool				m_bOpen;		// SPK File을 Open했는가?
//		std::ifstream		m_SPKFile;		// SPK File
//		CFileIndexTable		m_SPKIndex;		// SPK Index 정보
		CSpritePalPack			m_SPK;			// SPK

		//CAlphaSpritePack	*m_pSPK;			// SPK
		
		// Texture 출력할때의 크기
		int					*m_pWidth;
		int					*m_pHeight;

		// 지워야할 것들..
		SPRITESURFACE_LIST	m_listRemoved;
		SPRITE_PALETTE		m_historySprPal;
		MPalettePack		m_EffectScreenPPK;
};

class CNormalSpriteTexturePartManager : public CPartManager<WORD, WORD, CSpriteSurface*>
{
	public :
		typedef std::list<CSpriteSurface*>	SPRITESURFACE_LIST;
		typedef std::map<int, int>			SPRITE_PALETTE;			// 사용한 스프라이트 아이디가 마지막에 사용한 파레트 번호

	public :
		CNormalSpriteTexturePartManager(CSpritePack* pSPK, WORD partSize=100);
//		CNormalSpriteTexturePartManager(const char* spkFilename, WORD partSize=100);
		~CNormalSpriteTexturePartManager();

		//---------------------------------------------------
		// Init / Release
		//---------------------------------------------------
		//void				Init(CAlphaSpritePack* pSPK, WORD partSize);
		void				Init(CSpritePack *pSPK, WORD partSize);
		void				Release();
	
		// data내용을 초기화 시킨다.
		void				Clear();

		//---------------------------------------------------
		// SPK의 id번째 AlphaSprite를 넘겨받는다.
		//---------------------------------------------------
		CSpriteSurface*		GetTexture(TYPE_SPRITEID id);

		// AlphaSprite의 원래 크기를 넘겨준다.
		int					GetWidth(TYPE_SPRITEID id)	{ return m_pWidth[id]; }
		int					GetHeight(TYPE_SPRITEID id)	{ return m_pHeight[id]; }

		int					GetSpriteWidth(TYPE_SPRITEID id)	{ return (*m_pSPK)[id].GetWidth(); }
		int					GetSpriteHeight(TYPE_SPRITEID id)	{ return (*m_pSPK)[id].GetHeight(); }

		//---------------------------------------------------
		// 지워야 할 것들
		//---------------------------------------------------
		void				DeleteRemoved();

	protected :
//		bool				m_bOpen;		// SPK File을 Open했는가?
//		std::ifstream		m_SPKFile;		// SPK File
//		CFileIndexTable		m_SPKIndex;		// SPK Index 정보
		CSpritePack			*m_pSPK;			// SPK
		
		// Texture 출력할때의 크기
		int					*m_pWidth;
		int					*m_pHeight;

		// 지워야할 것들..
		SPRITESURFACE_LIST	m_listRemoved;		
};


#endif

