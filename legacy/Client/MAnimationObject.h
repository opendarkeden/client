//----------------------------------------------------------------------
// MAnimationObject.h
//----------------------------------------------------------------------
//
// 하나의 그림+애니Frame에 대한 정보
// 
//----------------------------------------------------------------------
//
// MImageObject는 단순히 하나의 Sprite로만 표현된다.
// 때로는 Animation이 되는 ImageObject도 있으면 좋지 싶어서.. *_*;
//
//    SpriteID + FrameID 로써 Animation되는 Object를 표현한다.
//    여기서.. SpriteID는 기본 Frame이 되고.. 
//             FrameID는 애니 Frame이 된다.
//    
//  즉, 출력할때.. 늘 SpriteID는 출력되고.. 
//                 FrameID의 현재 SpriteID가 하나 출력된다는 것이다.
//
//----------------------------------------------------------------------

#ifndef	__MANIMATIONOBJECT_H__
#define	__MANIMATIONOBJECT_H__

#include <Windows.h>
class ofstream;
class ifstream;
#include "MTypeDef.h"
#include "MImageObject.h"
#include "CAnimationFrame.h"
#include "ShowTimeChecker.h"

//----------------------------------------------------------------------
//
// ImageObject class
//
//----------------------------------------------------------------------
class MAnimationObject : public MImageObject, public CAnimationFrame, public ShowTimeChecker {
	public :
		MAnimationObject();
		MAnimationObject(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID nSprite, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans, BYTE bltType=BLT_NORMAL);
		~MAnimationObject();


		void	Set(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID nSprite, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans, BYTE bltType=BLT_NORMAL);

		void	NextFrame();
		void	SetDirection(BYTE dir) 	{ m_Direction = dir; }
		BYTE	GetDirection() const	{ return m_Direction; }

		//-------------------------------------------------------
		// Sound
		//-------------------------------------------------------
		bool			IsSoundFrame() const			{ return m_SoundFrame==m_CurrentFrame; }
		void			SetSoundFrame(BYTE fr)			{ m_SoundFrame = fr; }
		BYTE			GetSoundFrame() const			{ return m_SoundFrame; }
		void			SetSoundID(TYPE_SOUNDID id)		{ m_SoundID = id; }
		TYPE_SOUNDID	GetSoundID() const				{ return m_SoundID; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		virtual void	SaveToFile(class ofstream& file);
		virtual void	LoadFromFile(class ifstream& file);

		static void		NextLoopFrame()					{ LoopFrameCount++; }
		
	protected :
		static DWORD	LoopFrameCount;
		BYTE			m_Direction;

		BYTE			m_SoundFrame;	// 소리가 출력되는 frame
		TYPE_SOUNDID	m_SoundID;
};


#endif
