//----------------------------------------------------------------------
// CSpritePackList565.h
//----------------------------------------------------------------------
// 5:6:5를 위한 class
//----------------------------------------------------------------------

#ifndef	__CSPRITEPACKLIST565_H__
#define	__CSPRITEPACKLIST565_H__

#pragma warning(disable:4786)

#include <list>
#include "DrawTypeDef.h"
#include "CSprite.h"
#include "CSpritePackList.h"

typedef	std::list<CSprite*>		SPRITE_LIST;


class CSpritePackList565 : public CSpritePackList {
	public :
		CSpritePackList565() {}
		~CSpritePackList565() {}

		//---------------------------------------------------
		// file I/O		
		//---------------------------------------------------
		void		LoadFromFile(class ifstream& file);
};

#endif



