//----------------------------------------------------------------------
// CAlphaSpritePackList555.h
//----------------------------------------------------------------------
// 5:5:5를 위한 class
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITEPACKLIST555_H__
#define	__CALPHASPRITEPACKLIST555_H__

#include "DrawTypeDef.h"
#include "CAlphaSprite.h"
#include "CAlphaSpritePackList.h"

class CAlphaSpritePackList555 : public CAlphaSpritePackList {
	public :
		CAlphaSpritePackList555() {}
		~CAlphaSpritePackList555() {}

		//---------------------------------------------------
		// file I/O		
		//---------------------------------------------------
		void		LoadFromFile(std::ifstream& file);
};

#endif

