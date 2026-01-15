//----------------------------------------------------------------------
// CShadowPartManager.h
//----------------------------------------------------------------------
// 특정File(SSPK)에 대해서
// FileIndex(SSPKI)를 참조해서
// 일부의 ShadowSprite를 Load해서 사용하게 하는 class
//----------------------------------------------------------------------
// 그림자 Texture의 기본 크기는 64*64이다.
//----------------------------------------------------------------------
// 이거랑 TexturePartManager랑 거의 같다..
// template으로 바꿔야 되는데.. - -;; 아 귀찮아... 나중에.. - -;
//----------------------------------------------------------------------

#ifndef	__CSHADOWPARTMANAGER_H__
#define	__CSHADOWPARTMANAGER_H__

#include "CPartManager.h"
#include "CSpriteSurface.h"
#include "DrawTypeDef.h"
#include <list>
#include <fstream>
#include "CShadowSprite.h"
class CFileIndexTable;

using namespace std;

// <전체개수, 사용개수, 사용Data>
class CShadowPartManager : public CPartManager<WORD, WORD, CSpriteSurface*>
{
	public :
		typedef std::list<CSpriteSurface*>	SPRITESURFACE_LIST;

	public :
		//CShadowPartManager(CShadowSpritePack* pSSPK, WORD partSize=100);
		CShadowPartManager(const char* SSPKFilename, WORD partSize=100);
		~CShadowPartManager();

		//---------------------------------------------------
		// Init / Release
		//---------------------------------------------------
		//void				Init(CShadowSpritePack* pSSPK, WORD partSize);
		void				Init(const char* SSPKFilename, WORD partSize);
		void				Release();
	
		// data내용을 초기화 시킨다.
		void				Clear();

		//---------------------------------------------------
		// sspk의 id번째 AlphaSprite를 넘겨받는다.
		//---------------------------------------------------
		CSpriteSurface*		GetTexture(TYPE_SPRITEID id);

		// AlphaSprite의 원래 크기를 넘겨준다.
		int					GetWidth(TYPE_SPRITEID id)	{ return m_pWidth[id]; }
		int					GetHeight(TYPE_SPRITEID id)	{ return m_pHeight[id]; }

		// Sprite를 넘겨준다.
		CShadowSprite&		GetSprite(TYPE_SPRITEID id);

		//---------------------------------------------------
		// 지워야 할 것들
		//---------------------------------------------------
		void				DeleteRemoved();


	protected :
//		bool				m_bOpen;		// SSPK File을 Open했는가?
//		class ifstream		m_SSPKFile;		// SSPK File
//		CFileIndexTable		m_SSPKIndex;	// SSPK Index 정보
		CShadowSpritePack	m_SSPK;			// SSPK

		//CShadowSpritePack	*m_pSSPK;			// SSPK
		

		// Texture 출력할때의 크기
		int					*m_pWidth;
		int					*m_pHeight;

		// 지워야할 것들..
		SPRITESURFACE_LIST	m_listRemoved;
};


#endif

