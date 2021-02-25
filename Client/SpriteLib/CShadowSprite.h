//----------------------------------------------------------------------
// CShadowSprite.h
//----------------------------------------------------------------------

#ifndef	__CSHADOWSPRITE_H__
#define	__CSHADOWSPRITE_H__

#include <Windows.h>
#include "CSpriteDef.h"
#include "CIndexSprite.h"
#include "CSprite.h"
class ofstream;
class ifstream;


//----------------------------------------------------------------------
// width * height만큼의 pixel을 저장해둔다.
//----------------------------------------------------------------------
// 한 pixel을 저장하는 방법은 다음과 같다.
//
//    [1] 5:5:5 --> 2 bytes
//    [2] 5:6:5 --> 2 bytes
//    [3] R,G,B --> 3 bytes
//
// [3]은 용량이 커서 안좋으므로 [2]번 방법을 사용해서
// 5:5:5로 같은 방식으로 저장되도록 한다.
//
// Memory는 5:5:5나 5:6:5로 사용하지만
// disk에는 5:6:5로 저장된다.
//
// 즉, 5:5:5를 사용하는 system에서는 
//    File(5:6:5) ---(5:5:5로 변환)--> Memory(5:5:5)
//    File(5:6:5) <--(5:6:5로 변환)--- Memory(5:5:5)  이렇게 해야된다.
//
//----------------------------------------------------------------------
//
// = 한 줄의 정보
//
// (반복회수) (투명수, shadow수) (투명수, shadow수) (투명수, shadow수) ......
//
//
//----------------------------------------------------------------------

class CShadowSprite
{
	public :
		CShadowSprite();
		~CShadowSprite();

		bool		IsNotInit() const	{ return !m_bInit; }
		bool		IsInit() const		{ return m_bInit; }

		void		operator = (const CShadowSprite& Sprite);

		//---------------------------------------------------------
		// m_Pixels의 memory를 Release한다.		
		//---------------------------------------------------------
		void		Release();

		//---------------------------------------------------------
		// 투명 Color 
		//---------------------------------------------------------
		static void	SetColorkey(WORD color)			{ s_Colorkey = color; }
		static WORD	GetColorkey() 					{ return s_Colorkey; }

		//---------------------------------------------------------
		// fstream에서 save/load를 한다.
		//---------------------------------------------------------
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);
	
		//---------------------------------------------------------
		// CDirectDrawSurface의 영역을 읽어서 m_Pixels에 저장한다.
		//---------------------------------------------------------
		void		SetPixel(WORD* pSource, WORD sourcePitch, 							 
							 WORD width, WORD height);

		//---------------------------------------------------------
		// Sprite --> ShadowSprite
		//---------------------------------------------------------
		void		SetPixel(CIndexSprite& ispr);
		void		SetPixel(CSprite& spr);

		//---------------------------------------------------------
		// (x,y)는 sprite의 색깔 부분인가?
		//---------------------------------------------------------
		bool		IsColorPixel(short x, short y);

		//---------------------------------------------------------
		// get functions		
		//---------------------------------------------------------
		WORD		GetWidth()		const			{ return m_Width; }
		WORD		GetHeight()		const			{ return m_Height; }
		WORD		GetPixel(WORD x, WORD y)const	{ return m_Pixels[y][x]; }
		WORD*		GetPixelLine(WORD y)	const	{ return m_Pixels[y]; }	

		//---------------------------------------------------------
		//
		// Blt
		//
		//---------------------------------------------------------

		//---------------------------------------------------------
		// 정상적인 Blt
		//---------------------------------------------------------
		void		Blt(WORD *pDest, WORD pitch);				
		void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// 실제 크기보다 작게 출력하기
		// Test Code~~
		//---------------------------------------------------------
		void		BltSmall(WORD *pDest, WORD pitch, BYTE shift);
		void		BltSmallClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE shift);
		void		BltSmallClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE shift);
		void		BltSmallClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE shift);
		void		BltSmallClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE shift);

		//---------------------------------------------------------
		// 정상적인 Blt
		//---------------------------------------------------------
		//void		BltRotate(WORD *pDest, WORD pitch, short deg);
		//void		BltRotateClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltRotateClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltRotateClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltRotateClipHeight(WORD *pDest, WORD pitch, RECT* pRect);
		

		//---------------------------------------------------------
		// 좌우 바뀜
		//---------------------------------------------------------
	

		//---------------------------------------------------------
		// 반투명
		//---------------------------------------------------------
		//void		BltHalf(WORD *pDest, WORD pitch);		
		//void		BltHalfClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltHalfClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltHalfClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Alpha Blending
		//---------------------------------------------------------
		//void		BltAlpha(WORD *pDest, WORD pitch);
		//void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// RGB값 바뀜
		//---------------------------------------------------------
		//void		BltColor(WORD *pDest, WORD pitch, BYTE rgb);		
		//void		BltColorClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		//void		BltColorClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		//void		BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);

		//---------------------------------------------------------
		// 어둡게 하기
		//---------------------------------------------------------
		void		BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		void		BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// Texture용으로 4444 Blt
		//---------------------------------------------------------
		void		Blt4444(WORD *pDest, WORD pitch, WORD pixel);				
		void		Blt4444ClipLeft(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel);
		void		Blt4444ClipRight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel);
		void		Blt4444ClipWidth(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel);
		void		Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel);

		//---------------------------------------------------------
		// Texture용으로 작게 출력하는 4444 Blt
		//---------------------------------------------------------
		void		BltSmall4444(WORD *pDest, WORD pitch, WORD pixel, BYTE shift);				
		void		BltSmall4444ClipLeft(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift);
		void		BltSmall4444ClipRight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift);
		void		BltSmall4444ClipWidth(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift);
		void		BltSmall4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift);


		//---------------------------------------------------------
		// Effect
		//---------------------------------------------------------
		//void		BltEffect(WORD *pDest, WORD pitch);				
		//void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// ShadowChannel Filter를 이용한 출력
		//---------------------------------------------------------
		//void		BltShadowFilter(WORD *pDest, WORD pitch, CFilter* pFilter);		

		//---------------------------------------------------------
		// Utility Functions
		//---------------------------------------------------------
		void		memcpyShadowDarkness(WORD* pDest, WORD pixels);
		inline void		memcpyShadow4444(WORD* pDest, WORD pixels);
		//void			memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels);
		//void		memcpyShadow(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyColor(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels);

		//void			memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels);
		//void		memcpyDarknessFilter(WORD* pDest, WORD* pSource, WORD* pFilter, WORD pixels);

	protected :				
		WORD			m_Width;		// 가로 pixel수
		WORD			m_Height;		// 세로 pixel수		
		WORD**			m_Pixels;		// pixels
		bool			m_bInit;		// data가 있는가?

		static WORD		s_Colorkey;

		// Blt Value
		static BYTE		s_Value1;
		static BYTE		s_Value2;
		static WORD		s_wValue1;
};

typedef CTypePack<CShadowSprite>			CShadowSpritePack;

#endif


