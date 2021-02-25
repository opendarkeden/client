//----------------------------------------------------------------------
// CSprite.h
//----------------------------------------------------------------------

#ifndef	__CSPRITE_H__
#define	__CSPRITE_H__

#include <Windows.h>
#include "CSpriteSurface.h"
#include "CSpriteDef.h"
class CFilter;
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
// 내부적으로 0번 압축을 사용한다.
//
// = 한 줄의 정보
//
// (반복회수) (투명수, 색깔수, 색깔들...) (투명수, 색깔수, 색깔들...) ..
//
//----------------------------------------------------------------------

class CSprite
{
	public :
		CSprite();
		~CSprite();

		bool		IsNotInit() const	{ return !m_bInit; }
		bool		IsInit() const		{ return m_bInit; }
		bool		IsLoading() const	{ return m_bLoading; }

		void		operator = (const CSprite& Sprite);

		//---------------------------------------------------------
		// s_Pixels의 memory를 Init/Release한다.		
		//---------------------------------------------------------
		//static void	InitBuffer(WORD width, WORD height);
		//static void ReleaseBuffer();

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
		virtual bool		SaveToFile(class ofstream& file) = 0;
		virtual bool		LoadFromFile(class ifstream& file) = 0;		
		//virtual bool		LoadFromFileToBuffer(ifstream& file) = 0;
		//void		LoadFromBuffer();
			
		DWORD		GetFileSize();
		
		//---------------------------------------------------------
		// CDirectDrawSurface의 영역을 읽어서 m_Pixels에 저장한다.
		//---------------------------------------------------------
		static void	GetTightColorRect(WORD *pSource, WORD pitch, WORD width, WORD height, WORD colorkey, RECT& rect);
		void		SetPixel(WORD* pSource, WORD pitch, WORD width, WORD height);
		void		SetPixelNoColorkey(WORD* pSource, WORD pitch, WORD width, WORD height);

		//---------------------------------------------------------
		// get functions		
		//---------------------------------------------------------
		WORD		GetWidth()		const			{ return m_Width; }
		WORD		GetHeight()		const			{ return m_Height; }
		WORD		GetPixel(int x, int y) const;
		WORD*		GetPixelLine(WORD y)	const	{ return m_Pixels[y]; }	

		//---------------------------------------------------------
		// Sprite안에서 (x,y)좌표는 색깔이 있는가?
		//---------------------------------------------------------
		bool		IsColorPixel(short x, short y);		
	
		//---------------------------------------------------------
		//
		// Blt
		//
		//---------------------------------------------------------

		//---------------------------------------------------------
		// 정상적인 Blt
		//---------------------------------------------------------
		void		Blt(WORD *pDest, WORD pitch);		
		void		BltClip(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// 좌우 바뀜
		//---------------------------------------------------------
	

		//---------------------------------------------------------
		// 반투명
		//---------------------------------------------------------
		void		BltHalf(WORD *pDest, WORD pitch);		
		void		BltHalfClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltHalfClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltHalfClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltHalfClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Alpha Blending
		//---------------------------------------------------------
		void		BltAlpha(WORD *pDest, WORD pitch, BYTE alpha);		
		void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);

		//---------------------------------------------------------
		// RGB값 바뀜
		//---------------------------------------------------------
		void		BltColor(WORD *pDest, WORD pitch, BYTE rgb);		
		void		BltColorClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);

		//---------------------------------------------------------
		// Scale바꾸기
		//---------------------------------------------------------
		void		BltScale(WORD *pDest, WORD pitch, BYTE scale);		
		void		BltScaleClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale);
		void		BltScaleClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale);
		void		BltScaleClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale);
		void		BltScaleClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale);


		//---------------------------------------------------------
		// 어둡게 하기
		//---------------------------------------------------------
		void		BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		void		BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);


		//---------------------------------------------------------
		// Effect
		//---------------------------------------------------------
		void		BltEffect(WORD *pDest, WORD pitch);				
		void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// 특정ColorSet으로 찍기
		//---------------------------------------------------------
		void		BltColorSet(WORD *pDest, WORD pitch, WORD colorSet);		
		void		BltColorSetClipLeft(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipRight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipWidth(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);


		//---------------------------------------------------------
		// Filter를 사용해서 어둡게 하기
		//---------------------------------------------------------
		//void		BltDarknessFilter(WORD *pDest, WORD pitch, BYTE DarkBits, char cx, char cy, CSprite* pFilter);		
		//void		BltDarknessFilterClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits, char cx, char cy, CSprite* pFilter);
		//void		BltDarknessFilterClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits, char cx, char cy, CSprite* pFilter);
		//void		BltDarknessFilterClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits, char cx, char cy, CSprite* pFilter);
		//void		BltDarknessFilterClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits, char cx, char cy, CSprite* pFilter);

		//---------------------------------------------------------		
		//
		// Filter 관련 함수
		//
		//---------------------------------------------------------		
		bool		IsIntersectFilter();
		static void	SetFilter(CFilter* pFilter)
		{
			s_pFilter	= pFilter;
		}

		static void	SetFilter(short x, short y, CFilter* pFilter)
		{
			s_X			= x;
			s_Y			= y;
			s_pFilter	= pFilter;
		}

		static CFilter* GetFilter()
		{
			return s_pFilter;
		}

		//---------------------------------------------------------
		// AlphaChannel Filter를 이용한 출력
		//---------------------------------------------------------		
		void		BltAlphaFilter(WORD *pDest, WORD pitch);
		void		BltAlphaFilterClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltAlphaFilterClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltAlphaFilterClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltAlphaFilterClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// AlphaChannel Filter를 이용한 출력
		//---------------------------------------------------------		
		void		BltAlphaFilterDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);
		void		BltAlphaFilterDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltAlphaFilterDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltAlphaFilterDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltAlphaFilterDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// DarkerChannel Filter를 이용한 출력
		//---------------------------------------------------------		
		void		BltDarkerFilter(WORD *pDest, WORD pitch);
		void		BltDarkerFilterClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltDarkerFilterClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltDarkerFilterClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltDarkerFilterClipHeight(WORD *pDest, WORD pitch, RECT* pRect);
	
		//---------------------------------------------------------		
		// Alpha 4444 Small Not Trans
		//---------------------------------------------------------		
		void		BltAlpha4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE alpha, BYTE shift);
		void		BltAlpha4444NotTrans(WORD *pDest, WORD pitch, BYTE alpha);

		//---------------------------------------------------------		
		// 1555 
		//---------------------------------------------------------		
		void		Blt1555SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift);
		void		Blt1555NotTrans(WORD *pDest, WORD pitch);

	

		//---------------------------------------------------------
		// Utility Functions
		//---------------------------------------------------------
		//void		memcpyDarknessFilter(WORD* pDest, WORD* pSource, WORD* pFilter, WORD pixels);

		static void		memcpyAlphaFilter(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels);
		static void		memcpyAlphaFilterDarkness(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels);
		static void		memcpyDarkerFilter(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels);
		
		static void		memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpy1555(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpy1555Small(WORD* pDest, WORD* pSource, WORD pixels);




	protected :				
		WORD			m_Width;		// 가로 pixel수
		WORD			m_Height;		// 세로 pixel수		
		WORD**			m_Pixels;		// pixels
		bool			m_bInit;		// data가 있는가?
		bool			m_bLoading;		// Loading중인가?

	public :
		static WORD		s_Colorkey;

		// Blt Value
		static DWORD	s_Value1;
		static DWORD	s_Value2;
		static DWORD	s_Value3;
	
		// Filter 관련
		static short	s_X;
		static short	s_Y;
		static CFilter*	s_pFilter;
};


#endif

