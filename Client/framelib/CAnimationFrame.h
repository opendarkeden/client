//----------------------------------------------------------------------
// CAnimationFrame.h
//----------------------------------------------------------------------
// Frame이 바뀌면서 Animation이 되는 것에 대한 class
//----------------------------------------------------------------------
//
// 어떤 FramePack에서의 Frame ID와 
// CurrentFrame/MaxFrame의 정보를 갖는다.
//
//----------------------------------------------------------------------
//
// [ File I/O ]
//
// Frame ID만 저장된다. MaxFrame은 바뀔 수 있으므로 실행 시에 값을 결정한다.
// CurrentFrame은 늘 0부터 시작된다.
// Frame ID만 있으면 MaxFrame에 대한 정보를 알 수 있다.
// 그러나, 어떤 CAnimationFrame과 관련된 FramePack이 뭔지를 알 수가 없다.
// 어쨋거나.. 외부에서 그 정보를 결정해준다고 하자.. - -;;
//
//----------------------------------------------------------------------

#ifndef	__CANIMATIONFRAME_H__
#define	__CANIMATIONFRAME_H__

#include <Windows.h>
#include "DrawTypeDef.h"
class ofstream;
class ifstream;


class CAnimationFrame {
	public :
		CAnimationFrame(BYTE bltType=BLT_NORMAL);
		~CAnimationFrame();

		//--------------------------------------------------------
		// 기본 Frame
		//--------------------------------------------------------
		void			SetFrameID(TYPE_FRAMEID FrameID, BYTE max)	{ m_FrameID = FrameID; m_MaxFrame=max; m_CurrentFrame=0; }
		TYPE_FRAMEID	GetFrameID() const		{ return m_FrameID; }
		BYTE			GetFrame() const		{ return m_CurrentFrame; }
		BYTE			GetMaxFrame() const		{ return m_MaxFrame; }

		// 
		void			NextFrame()		 		{ if (++m_CurrentFrame==m_MaxFrame) m_CurrentFrame=0; }

		//--------------------------------------------------------
		// file I/O
		//--------------------------------------------------------
		void	SaveToFile(class ofstream& file);
		void	LoadFromFile(class ifstream& file);

		//-------------------------------------------------------
		// 출력 방식
		//-------------------------------------------------------
		void	SetBltType(BYTE bltType)	{ m_BltType = bltType; }
		BYTE	GetBltType() const			{ return m_BltType; }
		bool	IsBltTypeNormal() const		{ return m_BltType==BLT_NORMAL; }
		bool	IsBltTypeEffect() const		{ return m_BltType==BLT_EFFECT; }
		bool	IsBltTypeShadow() const		{ return m_BltType==BLT_SHADOW; }
		bool	IsBltTypeScreen() const		{ return m_BltType==BLT_SCREEN; }


	protected :		
		// 현재 frame에 대한 정보 : CThingFramePack에 대한 것이다.	
		TYPE_FRAMEID		m_FrameID;

		// Animation Frame을 위한 정보
		BYTE				m_CurrentFrame;	// Current Frame
		BYTE				m_MaxFrame;		// Max Frame
	
		// 출력 방식
		BYTE				m_BltType;
};

#endif

