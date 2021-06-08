//----------------------------------------------------------------------
// CCreatureFrameSetManager.h
//----------------------------------------------------------------------
//
// CreatureFrameSet을 생성하는 class
//
//
//----------------------------------------------------------------------

#ifndef	__CCREATUREFRAMESETMANAGER_H__
#define	__CCREATUREFRAMESETMANAGER_H__

#include "CFrameSetManager.h"
#include "CFramePack.h"

class CCreatureFrameSetManager : public CFrameSetManager {
	public :
		CCreatureFrameSetManager();
		~CCreatureFrameSetManager();

	protected :
		//--------------------------------------------------------
		// CreatureFrameSet과 관련된 SpriteID를 선택해서
		// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
		//--------------------------------------------------------
		bool	SaveSpriteSetIndex(CCreatureFramePack* pCreatureFramePack, class ofstream& setIndex, class ifstream& packIndex);
};

#endif

