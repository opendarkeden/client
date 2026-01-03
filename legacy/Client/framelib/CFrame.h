//----------------------------------------------------------------------
// CFrame.h
//----------------------------------------------------------------------
//
// CFrame은 하나의 Sprite에 대한 정보를 가진다.
//
//----------------------------------------------------------------------
//
// m_ID는 임의로 지정한 SpritePack/SpriteSet에서의 Sprite ID이다.
//
// m_cX, m_cY는 좌표 보정값으로서 Sprite가 출력될때 상하좌우로 
//           변화하는 값이다. 
//           (예) 캐릭터에 안경을 씌울 때,
//                캐릭터 Sprite는 m_cX=0, m_cY=0으로 한다면
//                안경 Sprite는 m_cX=20, m_cY=10으로 할 수 있겠지.
//
// m_fEffect는 출력방법에 대한 정의이다.
//        Mirror출력 가능.
//----------------------------------------------------------------------

#ifndef	__CFRAME_H__
#define	__CFRAME_H__

#include <Windows.h>
class ofstream;
class ifstream;
#include "DrawTypeDef.h"
#include "TArray.h"


//----------------------------------------------------------------------
// Frame을 출력하는데 있어서 효과를 주는 FLAG
//----------------------------------------------------------------------
//#define	FLAG_FRAME_MIRROR		0x01



class CFrame {
	public :
		CFrame(TYPE_SPRITEID spriteID=0, short cx=0, short cy=0)
		{
			Set(spriteID, cx, cy);
		}

		//~CFrame() {}

		//---------------------------------------------------------------
		// set
		//---------------------------------------------------------------
		void	Set(TYPE_SPRITEID spriteID, short cx, short cy);		

		//---------------------------------------------------------------
		// file I/O
		//---------------------------------------------------------------
		void	SaveToFile(class ofstream& file);
		void	LoadFromFile(class ifstream& file);

		//---------------------------------------------------------------
		// get		
		//---------------------------------------------------------------
		TYPE_SPRITEID	GetSpriteID()	const	{ return m_SpriteID; }
		short	GetCX()		const		{ return m_cX; }
		short	GetCY()		const		{ return m_cY; }

		//---------------------------------------------------------------
		// assign
		//---------------------------------------------------------------
		void	operator = (const CFrame& frame);


		//---------------------------------------------------------------
		// flag
		//---------------------------------------------------------------
		//void	UnSetAll()			{ m_fEffect = 0; }
		//BYTE	GetEffectFlag()		{ return m_fEffect; }

		// set flag
		//void	SetMirror()			{ m_fEffect |= FLAG_FRAME_MIRROR; }

		// unset flag
		//void	UnSetMirror()		{ m_fEffect &= ~FLAG_FRAME_MIRROR; }

		// is flag set?
		//bool	IsMirror()			{ return m_fEffect & FLAG_FRAME_MIRROR; }


	protected :
		TYPE_SPRITEID	m_SpriteID;		// SpriteSurface의 번호(0~65535)		

		// 좌표 보정값
		short	m_cX;			
		short	m_cY;

		//BYTE			m_fEffect;		// 출력방법에 대한 flag
};


//----------------------------------------------------------------------
// Effect를 위한 Frame
//----------------------------------------------------------------------
class CEffectFrame : public CFrame {
	public :
		CEffectFrame(TYPE_SPRITEID spriteID=0, short cx=0, short cy=0, char light=0, bool bBack = false)
		{
//			m_bBackground = bBack;
			Set(spriteID, cx, cy, light, bBack);
		};

		void	Set(TYPE_SPRITEID spriteID, short cx, short cy, char light, bool bBack)
		{
			CFrame::Set(spriteID, cx, cy);
			m_Light = light;
			m_bBackground = bBack;
		}

		void	SetBackground()				{ m_bBackground = true; }
		void	UnSetBackground()			{ m_bBackground = false; }

		//---------------------------------------------------------------
		// File I/O
		//---------------------------------------------------------------
		void	SaveToFile(class ofstream& file);
		void	LoadFromFile(class ifstream& file);

		// Get
		char	GetLight() const		{ return m_Light; }		
		bool	IsBackground() const	{ return m_bBackground; }

		//---------------------------------------------------------------
		// assign
		//---------------------------------------------------------------
		void	operator = (const CEffectFrame& frame);


	
	protected :
		char		m_Light;		// 시야의 크기(빛의 밝기)
		bool		m_bBackground;
};


//----------------------------------------------------------------------
//
// Frame Array  data type 정의
//
//----------------------------------------------------------------------

// FrameArray
typedef	TArray<CFrame, WORD>					FRAME_ARRAY;

// Direction FrameArray	
typedef	TArray<FRAME_ARRAY, BYTE>				DIRECTION_FRAME_ARRAY;

// Action FrameArray
typedef	TArray<DIRECTION_FRAME_ARRAY, BYTE>		ACTION_FRAME_ARRAY;


//----------------------------------------------------------------------
// Effect Frame
//----------------------------------------------------------------------
// FrameArray
typedef	TArray<CEffectFrame, WORD>					EFFECTFRAME_ARRAY;

// Direction FrameArray	
typedef	TArray<EFFECTFRAME_ARRAY, BYTE>				DIRECTION_EFFECTFRAME_ARRAY;


#endif
