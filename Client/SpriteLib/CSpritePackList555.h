//----------------------------------------------------------------------
// CSpritePackList555.h
//----------------------------------------------------------------------
// 5:5:5¸¦ À§ÇÑ class
//----------------------------------------------------------------------

#ifndef	__CSPRITEPACKLIST555_H__
#define	__CSPRITEPACKLIST555_H__

#pragma warning(disable:4786)

#include <list>
#include "DrawTypeDef.h"
#include "CSprite.h"
#include "CSpritePackList.h"

typedef	std::list<CSprite*>		SPRITE_LIST;


class CSpritePackList555 : public CSpritePackList {
	public :
		CSpritePackList555() {}
		~CSpritePackList555() {}

		//---------------------------------------------------
		// file I/O		
		//---------------------------------------------------
		void		LoadFromFile(std::ifstream& file);
};

#endif

