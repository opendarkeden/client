//----------------------------------------------------------------------
// CAlphaSprite.h
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITE_H__
#define	__CALPHASPRITE_H__

#include <Windows.h>

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
// (반복회수) (투명수, 색깔수, 색깔들(alpha,색깔,alpha,색깔...)) (투명수, 색깔수, 색깔들(alpha,색깔,alpha,색깔...)...) ..
//
//
// Alpha값과 색깔값을 같이 저장한다.
//----------------------------------------------------------------------

class CAlphaSprite
{
	public :
		CAlphaSprite();
		~CAlphaSprite();

		bool		IsNotInit() const	{ return !m_bInit; }
		bool		IsInit() const		{ return m_bInit; }

		void		operator = (const CAlphaSprite& Sprite);

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
		virtual bool		SaveToFile(ofstream& file) = 0;
		virtual bool		LoadFromFile(ifstream& file) = 0;		
		//virtual bool		LoadFromFileToBuffer(ifstream& file) = 0;
		//void		LoadFromBuffer();
			
		//---------------------------------------------------------
		// CDirectDrawSurface의 영역을 읽어서 m_Pixels에 저장한다.
		//---------------------------------------------------------
		void		SetPixel(WORD* pSource, WORD sourcePitch, 
							 WORD* pFilter, WORD filterPitch, 
							 WORD width, WORD height);

		// (x,y)는 sprite의 색깔 부분인가?
		bool		IsColorPixel(short x, short y);

		//---------------------------------------------------------
		// get functions		
		//---------------------------------------------------------
		WORD		GetWidth()		const			{ return m_Width; }
		WORD		GetHeight()		const			{ return m_Height; }
		WORD		GetPixel(int x, int y, int bColor=1) const;
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
		void		BltClip(WORD* pDest, WORD pitch, RECT* pRect);
		void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Blt 4444  for Texture
		//---------------------------------------------------------
		void		Blt4444(WORD *pDest, WORD pitch);				
		void		Blt4444ClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Blt 4444 NotTrans for Texture (투명부분도 검게 칠한다)
		//---------------------------------------------------------
		void		Blt4444NotTrans(WORD *pDest, WORD pitch);				
		void		Blt4444NotTransClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// shift만큼 shift해서 크기를 작게 출력시킨다.
		//---------------------------------------------------------
		void		Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift);

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
		void		BltAlpha(WORD *pDest, WORD pitch, BYTE alpha);
		void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);

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
		//void		BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		//void		BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		//void		BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		//void		BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// Effect
		//---------------------------------------------------------
		//void		BltEffect(WORD *pDest, WORD pitch);				
		//void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// AlphaChannel Filter를 이용한 출력
		//---------------------------------------------------------
		//void		BltAlphaFilter(WORD *pDest, WORD pitch, CFilter* pFilter);		

		//---------------------------------------------------------
		// Utility Functions
		//---------------------------------------------------------
		//void			memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlphaValue(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyColor(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels);

		//void			memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels);
		//void		memcpyDarknessFilter(WORD* pDest, WORD* pSource, WORD* pFilter, WORD pixels);


		//---------------------------------------------------------
		// debug function
		//---------------------------------------------------------
		//static void		OutputLog()
		//{
		//	ofstream file("log.txt",ios::app);
		//	file << "New     = " << s_New << endl;
		//	file << "Delete  = " << s_Delete << endl;
		//	file << "------------------------" << endl;
		//}


	protected :				
		WORD			m_Width;		// 가로 pixel수
		WORD			m_Height;		// 세로 pixel수		
		WORD**			m_Pixels;		// pixels
		bool			m_bInit;		// data가 있는가?

		static WORD		s_Colorkey;

		// Blt Value
		static int		s_Value1;
		static int		s_Value2;

};


#endif

