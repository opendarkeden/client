//----------------------------------------------------------------------
// CAlphaSpritePackList.h
//----------------------------------------------------------------------
// 내부적으로 stl의 list를 사용했다.
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITEPACKLIST_H__
#define	__CALPHASPRITEPACKLIST_H__

#pragma warning(disable:4786)

#include <list>
#include "DrawTypeDef.h"
#include "CAlphaSprite.h"

typedef	std::list<CAlphaSprite*>		ALPHASPRITE_LIST;


class CAlphaSpritePackList {
	public :
		CAlphaSpritePackList();
		~CAlphaSpritePackList();

		//---------------------------------------------------
		// Init/Release		
		//---------------------------------------------------
		void		Release();

		//---------------------------------------------------
		// add / remove
		//---------------------------------------------------
		void		AddSprite(TYPE_SPRITEID n, CAlphaSprite* pSprite);
		void		AddSprite(CAlphaSprite* pSprite);
		void		RemoveSprite(TYPE_SPRITEID n);
		void		ChangeSprite(TYPE_SPRITEID n, TYPE_SPRITEID m);

		//---------------------------------------------------
		// get
		//---------------------------------------------------
		CAlphaSprite*	GetSprite(TYPE_SPRITEID n) const;

		//---------------------------------------------------
		// 첫번째 위치의 List Iterater를 넘겨준다.
		//---------------------------------------------------
		ALPHASPRITE_LIST::const_iterator	GetIterator() const	{ return m_listSprite.begin(); }
		ALPHASPRITE_LIST* GetListPtr() { return &m_listSprite; }

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
		ALPHASPRITE_LIST		m_listSprite;		// CAlphaSprite들을 저장해둔다.
};

#endif



