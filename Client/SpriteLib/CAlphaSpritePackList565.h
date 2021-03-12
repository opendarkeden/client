//----------------------------------------------------------------------
// CAlphaSpritePackList565.h
//----------------------------------------------------------------------
// 5:6:5¸¦ À§ÇÑ class
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITEPACKLIST565_H__
#define	__CALPHASPRITEPACKLIST565_H__

#include <list>
#include "DrawTypeDef.h"
#include "CAlphaSprite.h"
#include "CAlphaSpritePackList.h"

class CAlphaSpritePackList565 : public CAlphaSpritePackList {
	public :
		CAlphaSpritePackList565() {}
		~CAlphaSpritePackList565() {}

		//---------------------------------------------------
		// file I/O		
		//---------------------------------------------------
		void		LoadFromFile(ifstream& file);
};

#endif



