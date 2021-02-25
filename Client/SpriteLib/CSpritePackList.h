//----------------------------------------------------------------------
// CSpritePackList.h
//----------------------------------------------------------------------
// 내부적으로 stl의 list를 사용했다.
//----------------------------------------------------------------------

#ifndef	__CSPRITEPACKLIST_H__
#define	__CSPRITEPACKLIST_H__

#pragma warning(disable:4786)

#include <list>
#include "DrawTypeDef.h"
#include "CSprite.h"

typedef	std::list<CSprite*>		SPRITE_LIST;
typedef std::list<CSpritePal*>	SPRITEPAL_LIST;


class CSpritePackList {
	public :
		CSpritePackList();
		virtual ~CSpritePackList();

		//---------------------------------------------------
		// Init/Release		
		//---------------------------------------------------
		void		Release();

		//---------------------------------------------------
		// add / remove
		//---------------------------------------------------
		void		AddSprite(TYPE_SPRITEID n, CSprite* pSprite);
		void		AddSprite(CSprite* pSprite);
		void		RemoveSprite(TYPE_SPRITEID n);
		void		ChangeSprite(TYPE_SPRITEID n, TYPE_SPRITEID m);

		//---------------------------------------------------
		// get
		//---------------------------------------------------
		CSprite*	GetSprite(TYPE_SPRITEID n) const;

		//---------------------------------------------------
		// 첫번째 위치의 List Iterater를 넘겨준다.
		//---------------------------------------------------
		SPRITE_LIST::const_iterator	GetIterator() const	{ return m_listSprite.begin(); }
		SPRITE_LIST* GetListPtr() { return &m_listSprite; }

		//---------------------------------------------------
		// get size
		//---------------------------------------------------
		TYPE_SPRITEID	GetSize() const	{ return m_listSprite.size(); }

		//---------------------------------------------------
		// file I/O
		//---------------------------------------------------
		bool				SaveToFile(class ofstream& spkFile, class ofstream& indexFile);
		virtual void		LoadFromFile(class ifstream& file) = 0;

	protected :		
		SPRITE_LIST		m_listSprite;		// CSprite들을 저장해둔다.
};

class CSpritePalPackList {
	public :
		CSpritePalPackList();
		~CSpritePalPackList();

		//---------------------------------------------------
		// Init/Release		
		//---------------------------------------------------
		void		Release();

		//---------------------------------------------------
		// add / remove
		//---------------------------------------------------
		void		AddSprite(TYPE_SPRITEID n, CSpritePal* pSprite);
		void		AddSprite(CSpritePal* pSprite);
		void		RemoveSprite(TYPE_SPRITEID n);
		void		ChangeSprite(TYPE_SPRITEID n, TYPE_SPRITEID m);

		//---------------------------------------------------
		// get
		//---------------------------------------------------
		CSpritePal*	GetSprite(TYPE_SPRITEID n) const;

		//---------------------------------------------------
		// 첫번째 위치의 List Iterater를 넘겨준다.
		//---------------------------------------------------
		SPRITEPAL_LIST::const_iterator	GetIterator() const	{ return m_listSprite.begin(); }
		SPRITEPAL_LIST* GetListPtr() { return &m_listSprite; }

		//---------------------------------------------------
		// get size
		//---------------------------------------------------
		TYPE_SPRITEID	GetSize() const	{ return m_listSprite.size(); }

		//---------------------------------------------------
		// file I/O
		//---------------------------------------------------
		bool				SaveToFile(class ofstream& spkFile, class ofstream& indexFile);
		void		LoadFromFile(class ifstream& file);

	protected :		
		SPRITEPAL_LIST		m_listSprite;		// CSprite들을 저장해둔다.
};
#endif



