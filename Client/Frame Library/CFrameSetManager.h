//----------------------------------------------------------------------
// CFrameSetManager.h
//----------------------------------------------------------------------
//
// FramePack에서 특정 Frame만 Load할 수 있도록 정보를 만든다.
//
// <FrameID를 저장할 List사용>
//
// FramePack File의 일부 Frame ID를 저장한다. 
// 
// * FrameSet에서 load할때 사용할 index file을 만들어야 하는데,
//   저장된 Frame ID에 해당하는 Frame의 File Position를 
//   FramePack IndexFile에서 찾아서 
//   FrameSet index file로 저장해야 한다.
//
// * 정해진 Frame ID와 관련이 있는 모든 Sprite ID를 추출한다.
//   중복되면 안되므로 list에 추가할때 중복이 안되도록 한다.
//   SpriteSet IndexFile을 생성해야 하는데, 정해진 SpritePack Index를
//   이용해서 저장한다.
//
//
// (!) ThingFrameSetManager와 CreatureFramePackManager의 두가지를
//     상속받아서 사용해야 할 것이다
//----------------------------------------------------------------------
//
// Frame ID를 set하고
// FramePack의 File Position을 저장해야 한다.  --> FrameSet Index File
// SpritePack의 File Position을 저장해야 한다. --> SpriteSet Index File
//
//----------------------------------------------------------------------

#ifndef	__CFRAMESETMANAGER_H__
#define	__CFRAMESETMANAGER_H__


#include "DrawTypeDef.h"
#include "CSetManager.h"


class CFrameSetManager : public CSetManager<TYPE_FRAMEID, TYPE_FRAMEID> {
	public :
		CFrameSetManager();
		virtual ~CFrameSetManager();

		//--------------------------------------------------------
		// FramePack IndexFile로부터 FrameSet IndexFile을 생성한다.
		//--------------------------------------------------------
		bool		SaveFrameSetIndex(class ofstream& setIndex, class ifstream& packIndex);

		//--------------------------------------------------------
		// FrameSet과 관련된 SpriteID를 선택해서
		// SpritePack IndexFile로부터 SpriteSet IndexFile을 생성한다.
		//--------------------------------------------------------
		//virtual bool		SaveSpriteSetIndex(class ofstream& setIndex, class ifstream& packIndex) = 0;
		

	protected :
		
};


#endif



