//-----------------------------------------------------------------------------
// CIndexSprite.h
//-----------------------------------------------------------------------------
// width * height만큼의 pixel을 저장해둔다.
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
// - 내부적으로 투명색 압축을 사용한다.
// - IndexSet의 Gradation색깔별로 압축(?)한다. (효율은 높지 않을 듯 하다)
//
//
// [ 한 줄의 정보 ]
//
// (투명Pair반복수) 
//    ( (투명색수, IndexPair반복수, 
//                 (index색수, indexSet번호, gradation번호), 
//                 (index색수, indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//        고정된색깔수, 고정된색깔들)
//       (투명색수, IndexPair반복수, 
//                 (index색수, indexSet번호, gradation번호), 
//                 (index색수, indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//        고정된색깔수, 고정된색깔들)
//    )
//
//
// 투명색과 Gradation Index값과 색깔값을 같이 저장한다.
//-----------------------------------------------------------------------------
//
// SetColorSet555나 SetColorSet565를 꼭!! 외부에서 실행시켜줘야 한다.
//
//-----------------------------------------------------------------------------

#ifndef	__CINDEXSPRITE_H__
#define	__CINDEXSPRITE_H__

#include <Windows.h>
class ofstream;
class ifstream;

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
// Maximum value설정
#define	MAX_COLORSET_SEED			33	// 기본 색상의 개수
#define	MAX_COLORSET_SEED_MODIFY	15	// 기본 색상마다의 변형 색깔 수
#define	MAX_COLORSET				495 // MAX_COLORSET_SEED * MAX_COLORSET_SEED_MODIFY
#define	MAX_COLORGRADATION			30
#define	MAX_COLOR_TO_GRADATION		93	// R+G+B
#define	MAX_COLORGRADATION_HALF		15	// == MAX_COLORSET_SEED_MODIFY
#define	MAX_DARKBIT					5

// 한 Sprite에서 사용하는 최대의 ColorSet수
#define	MAX_COLORSET_USE			256

// index값 설정
#define	INDEX_NULL					0xFF
#define	INDEX_SELECT				0xFE
#define	INDEX_TRANS					0xFD
#define	INDEXSET_NULL				0xFF

class CIndexSprite {
	public :
		CIndexSprite();
		~CIndexSprite();
		
		bool		IsNotInit() const	{ return !m_bInit; }
		bool		IsInit() const		{ return m_bInit; }

		void		operator = (const CIndexSprite& Sprite);

		//---------------------------------------------------------
		// Static ColorSet Table을 초기화 해준다.
		//---------------------------------------------------------
		static	BOOL	SaveIndexTableToFile(class ofstream& file);
		static	BOOL	LoadIndexTableFromFile(class ifstream& file);
		static	void	SetColorSet();
		static	void	SetUsingColorSet(int set1, int set2);
		static	void	SetUsingColorSetOnly(BYTE setNumber, int colorSet);
		static	int		GetUsingColorSet(BYTE n)	{ return s_IndexValue[n]; }		
		static	void	GetIndexColor(WORD* pColor, int step, int r0, int g0, int b0, int r1, int g1, int b1);
		


		//---------------------------------------------------------
		// CDirectDrawSurface의 영역을 읽어서 m_Pixels에 저장한다.
		//---------------------------------------------------------
		void		SetPixel(WORD* pSource, WORD sourcePitch, 
							 WORD* pIndex1, WORD indexPitch1,	// 첫번째 index
							 WORD* pIndex2, WORD indexPitch2,	// 두번째 index
							 WORD width, WORD height);

		//---------------------------------------------------------
		// color에 대응하는 index색번호를 찾는다.
		//---------------------------------------------------------
		static	BYTE	GetIndexColor(WORD color);
		static	BYTE	GetColorToGradation(BYTE color);

		//---------------------------------------------------------
		// Generate Index
		//---------------------------------------------------------
		void			GenerateFromIndex(WORD** ppColor, WORD width, WORD height, WORD** ppIndex);
		void			GetIndexInfo(WORD**& ppIndex);

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

		//---------------------------------------------------------
		// get functions		
		//---------------------------------------------------------
		WORD		GetWidth()		const			{ return m_Width; }
		WORD		GetHeight()		const			{ return m_Height; }
		WORD		GetPixel(int x, int y) const;
		WORD*		GetPixelLine(WORD y)	const	{ return m_Pixels[y]; }
		
		//---------------------------------------------------------
		// (x,y)는 색깔이 있는 부분인가?
		//---------------------------------------------------------
		bool		IsColorPixel(short x, short y);

		//---------------------------------------------------------
		// Index Blt
		//---------------------------------------------------------
		void		Blt(WORD *pDest, WORD pitch);				
		void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// 어둡게 하기
		//---------------------------------------------------------
		void		BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		void		BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// 밝게 하기
		//---------------------------------------------------------
		void		BltBrightness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		void		BltBrightnessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltBrightnessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltBrightnessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		void		BltBrightnessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// Alpha Blending
		//---------------------------------------------------------
		void		BltAlpha(WORD *pDest, WORD pitch, BYTE alpha);		
		void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);

		//---------------------------------------------------------
		// 특정색깔로 찍기
		//---------------------------------------------------------
		void		BltColor(WORD *pDest, WORD pitch, BYTE rgb);		

		
		void		BltColorClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		void		BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);

		//---------------------------------------------------------
		// 특정ColorSet으로 찍기
		//---------------------------------------------------------
		void		BltColorSet(WORD *pDest, WORD pitch, WORD colorSet);		
		void		BltColorSetClipLeft(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipRight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipWidth(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);
		void		BltColorSetClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet);

		//---------------------------------------------------------
		// Effect
		//---------------------------------------------------------
		void		BltEffect(WORD *pDest, WORD pitch);				
		void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect);


		//---------------------------------------------------------
		// Blt Wave - Test - -;;
		//---------------------------------------------------------
		//void		BltWave(WORD *pDest, WORD pitch);
		
	public :
		//--------------------------------------------------------
		// ColorSet Table
		//--------------------------------------------------------
		// 25 Set ,  13 Gradation
		static WORD		ColorSet[MAX_COLORSET][MAX_COLORGRADATION];			// 실제 색
		static WORD		GradationValue[MAX_COLORGRADATION];					// 각 gradation의 값 - IndexEditor에서 계산을 위한 값
		static WORD		ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];	// 어둡게 했을 때의 색깔 값
		static BYTE		ColorToGradation[MAX_COLOR_TO_GRADATION];			// R+G+B의 값으로 Gradation값을 얻어낸다.

	protected :
		WORD			m_Width;		// 가로 pixel수
		WORD			m_Height;		// 세로 pixel수		
		WORD**			m_Pixels;		// pixels
		bool			m_bInit;		// data가 있는가?

		static WORD		s_Colorkey;
	
		// Blt Value (parameter 대용)
		static int		s_IndexValue[MAX_COLORSET_USE];

	
	public :		
		typedef void (*FUNCTION_MEMCPYEFFECT)(WORD*, WORD*, WORD);

		// memcpyEffect의 종류
		enum FUNCTION_EFFECT
		{
			EFFECT_DARKER = 0,
			EFFECT_GRAY_SCALE,
			EFFECT_LIGHTEN,
			EFFECT_DARKEN,
			EFFECT_COLOR_DODGE,
			EFFECT_SCREEN,
			EFFECT_DODGE_BURN,
			EFFECT_DIFFERENT,
			EFFECT_GRADATION,
			EFFECT_SIMPLE_OUTLINE,
			EFFECT_WIPE_OUT,
			EFFECT_NET,
			EFFECT_GRAY_SCALE_VARIOUS,
			MAX_EFFECT
		};

		static void		SetEffect(enum FUNCTION_EFFECT func)	{ s_pMemcpyEffectFunction = s_pMemcpyEffectFunctionTable[func]; }
		static void		memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels)
		{
			(*s_pMemcpyEffectFunction)(pDest, pSource, pixels);
		}

		// memcpyEffect
		static void		memcpyEffectDarker(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectGrayScale(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectLighten(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectDarken(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectColorDodge(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectScreen(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectDodgeBurn(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectDifferent(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectGradation(WORD* pDest, WORD* pSource, WORD pixels);		
		static void		memcpyEffectSimpleOutline(WORD* pDest, WORD* pSource, WORD pixels);	
		static void		memcpyEffectWipeOut(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectNet(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyEffectGrayScaleVarious(WORD* pDest, WORD* pSource, WORD pixels);


	protected :
		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunction;
		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunctionTable[MAX_EFFECT];
	
};

#endif


