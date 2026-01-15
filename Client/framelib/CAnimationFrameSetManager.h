//----------------------------------------------------------------------
// CThingFrameSetManager.h
//----------------------------------------------------------------------
//
// AnimationFrameSet을 생성하는 class
//
//
//----------------------------------------------------------------------

#ifndef	__CANIMATIONFRAMESETMANAGER_H__
#define	__CANIMATIONFRAMESETMANAGER_H__

#include "CFrameSetManager.h"
#include "CFramePack.h"

class CAnimationFrameSetManager : public CFrameSetManager {
	public :
		CAnimationFrameSetManager();
		~CAnimationFrameSetManager();

	protected :
		//--------------------------------------------------------
		// AnimationFrameSet과 관련된 SpriteID를 선택해서
		// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
		//--------------------------------------------------------
		bool	SaveSpriteSetIndex(CAnimationFramePack* pAnimationFramePack, std::ofstream& setIndex, std::ifstream& packIndex);
};

#endif

