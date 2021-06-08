//----------------------------------------------------------------------
// CSpriteSetManager.h
//----------------------------------------------------------------------
//
// SpritePack에서 특정 Sprite만 Load할 수 있도록 정보를 만든다.
//
// <SpriteID를 저장할 List사용>
//
// SpritePack File의 일부 Sprite ID를 저장한다. 
// 
// SpriteSet에서 load할때 사용할 index file을 만들어야 하는데,
// 저장된 Sprite ID에 해당하는 Sprite의 File Position를 
// SpritePack IndexFile에서 찾아서 
// SpriteSet index file로 저장해야 한다.
//
//----------------------------------------------------------------------
//
// Sprite ID를 set하고
// File Position을 저장해야 한다.
//
//----------------------------------------------------------------------

#ifndef	__CSPRITESETMANAGER_H__
#define	__CSPRITESETMANAGER_H__


#include "DrawTypeDef.h"
#include "CSetManager.h"


class CSpriteSetManager : public CSetManager<TYPE_SPRITEID, TYPE_SPRITEID> {
	public :
		CSpriteSetManager();
		~CSpriteSetManager();

		//--------------------------------------------------------
		// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
		//--------------------------------------------------------
		bool		SaveSpriteSetIndex(class ofstream& setIndex, class ifstream& spkIndex);
		

	protected :
		
};


#endif
