//----------------------------------------------------------------------
// CDirectDrawSurface.h
//----------------------------------------------------------------------
//
//
// 5:6:5 와  5:5:5의 차이는 
// Bit Mask에 의해 처리되는 부분에서 발생하는데
// CDirectDraw class의 InitMask()에서 Video Card에 맞는
// 적절한 Mask를 생성하므로 그냥~ CDirectDraw의 Mask를 사용하면 된다.
//
//
//
//----------------------------------------------------------------------
/*

  < DDSurface가 생성되는 MEMORY >

	- DDSCAPS_SYSTEMMEMORY는 Lock을 사용할 때 빠르다.
	- DDSCAPS_VIDEOMEMORY는 Blt를 사용할 때 빠르다.
	  단, VIDEOMEMORY가 부족하면 Surface를 생성하지 않으므로 error!
	- 지정을 안 해 주면 VIDEOMEMORY가 사용가능하면 사용하고 
   	  아니면 SYSTEMMEMORY를 사용한다.
	
  < Alpha Blending >
    - 사용하려면 DDSCAPS_SYSTEMMEMORY를 사용하는게 좋다.
	- Surface --> Surface로의 Alpha Blending보다
	  Memory(SpriteFile) --> Surface로의 Alpha Blending이 빠르다.

  < Sprite File >
    - 0번 압축 된 것과 안 된 것 두가지의 경우가 있다.
	- 0번 압축 된 것은 투명색이 있는 모든 경우에 빠르다.
	  단, MMX instruction으로 투명색을 제거할 경우가 더 빠르겠지...

  < 사용시 주의 사항 >
    - Clipping이 되는 함수와 안 되는 함수가 있는데
	  적절한 경우에 맞춰서 사용해야할 것이다. 속도 문제가 있으니까!


*/
//----------------------------------------------------------------------

#ifndef	__CSPRITESURFACE_H__
#define	__CSPRITESURFACE_H__


#include "CDirectDrawSurface.h"
#include "CSpritePal.h"
#include "CAlphaSpritePal.h"
#include "MPalettePack.h"

class CSprite;
class CAlphaSprite;
class CIndexSprite;
class CShadowSprite;
class CSpriteOutlineManager;

typedef void (*FUNCTION_MEMCPYEFFECT)(WORD*, WORD*, WORD);
typedef void (*FUNCTION_MEMCPYPALEFFECT)(WORD*, BYTE*, WORD, MPalette &);

class CSpriteSurface : public CDirectDrawSurface {
	public :
		CSpriteSurface();		
		~CSpriteSurface();

		//------------------------------------------------------------
		// BltFast
		//------------------------------------------------------------
		void	BltHalf(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect);
		void	BltDarkness(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, BYTE DarkBits);
		void	BltBrightness(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, BYTE BrightBits);
		void	BltDarknessFilter(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, WORD TransColor=0);

		//------------------------------------------------------------
		// Drawing 함수
		//------------------------------------------------------------
		void	DrawRect(RECT* rect, WORD color);
		void	HLine(int x, int y, int length, WORD color);		// 가로
		void	VLine(int x, int y, int length, WORD color);		// 세로
		void	Line(int x1, int y1, int x2, int y2, WORD color);	// 직선
		
	
		//------------------------------------------------------------
		//
		// Lock & Draw Functions
		//
		//------------------------------------------------------------

		//------------------------------------------------------------
		// 어둡게 하기
		//------------------------------------------------------------
		void	ChangeBrightnessBit(RECT*  pRect, BYTE DarkBits);
		
		//------------------------------------------------------------
		// 단색 Alpha blending (화면의 pRect를 color와 alpha로..)
		// alpha2가 0이면 color로.. 31이면.. 원래 surface
		//------------------------------------------------------------
		void	BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2);

		//virtual void	BltHalf(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, DWORD ColorKey=0);
		//virtual void	BltDarkness(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, BYTE bits, WORD ColorKey=0);		

		//------------------------------------------------------------
		// AlphaDepth는 1~32이다.
		//------------------------------------------------------------		
		//virtual void	BltTransAlpha(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, int alphaDepth, DWORD ColorKey=0);

		//------------------------------------------------------------
		//
		// CSprite 관련 함수
		//
		//------------------------------------------------------------
		// Sprite
		void	BltSprite(POINT* pPoint, CSprite* pSprite);		
		void	BltSpriteNoClip(POINT* pPoint, CSprite* pSprite);
		void	BltSpriteHalf(POINT* pPoint, CSprite* pSprite);
		void	BltSpriteAlpha(POINT* pPoint, CSprite* pSprite, BYTE alphaDepth);
		void	BltSpriteColor(POINT* pPoint, CSprite* pSprite, BYTE rgb);
		void	BltSpriteDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits);		
		void	BltSpriteColorSet(POINT* pPoint, CSprite* pSprite, WORD colorSet);
		void	BltSpriteEffect(POINT* pPoint, CSprite* pSprite);
		void	BltSpriteScale(POINT* pPoint, CSprite* pSprite, BYTE scale=1);
		void	BltSpriteAlpha4444SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha, BYTE shift);
		void	BltSpriteAlpha4444NotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha);
		void	BltSprite1555SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE shift);
		void	BltSprite1555NotTrans(POINT* pPoint, CSprite* pSprite);

		void	BltSpritePalEffect(POINT* pPoint, CSpritePal* pSprite, MPalette &pal);
		void	BltSpritePal1555SmallNotTrans(POINT* pPoint, CSpritePal* pSprite, BYTE shift, MPalette &pal);
		void	BltSpritePal1555NotTrans(POINT* pPoint, CSpritePal* pSprite, MPalette &pal);

		// Filter
		void	BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite);
		void	BltSpriteAlphaFilterDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits);
		void	BltSpriteDarkerFilter(POINT* pPoint, CSprite* pSprite);
		//void	BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite, CFilter* pFilter);

		// AlphaSprite
		void	BltAlphaSprite(POINT* pPoint, CAlphaSprite* pSprite);
		void	BltAlphaSpriteAlpha(POINT* pPoint, CAlphaSprite* pSprite, BYTE alpha);
		void	BltAlphaSprite4444(POINT* pPoint, CAlphaSprite* pSprite);
		void	BltAlphaSprite4444NotTrans(POINT* pPoint, CAlphaSprite* pSprite);
		void	BltAlphaSprite4444SmallNotTrans(POINT* pPoint, CAlphaSprite* pSprite, BYTE shift);

		void	BltAlphaSpritePal(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal);
		void	BltAlphaSpritePalAlpha(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE alpha, MPalette &pal);
		void	BltAlphaSpritePal4444(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal);
		void	BltAlphaSpritePal4444NotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal);
		void	BltAlphaSpritePal4444SmallNotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE shift, MPalette &pal);

		// IndexSprite
		void	BltIndexSprite(POINT* pPoint, CIndexSprite* pSprite);
		void	BltIndexSpriteDarkness(POINT* pPoint, CIndexSprite* pSprite, BYTE DarkBits);
		void	BltIndexSpriteAlpha(POINT* pPoint, CIndexSprite* pSprite, BYTE alpha);
		void	BltIndexSpriteColor(POINT* pPoint, CIndexSprite* pSprite, BYTE rgb);
		void	BltIndexSpriteColorSet(POINT* pPoint, CIndexSprite* pSprite, WORD colorSet);
		void	BltIndexSpriteEffect(POINT* pPoint, CIndexSprite* pSprite);
		void	BltIndexSpriteBrightness(POINT *pPoint, CIndexSprite* pSprite, BYTE BrightBits);

		// Sprite Outline
		void	BltSpriteOutline(CSpriteOutlineManager *pSOM, WORD color);
		void	BltSpriteOutlineOnly(CSpriteOutlineManager* pSOM, WORD color);
		void	BltSpriteOutlineDarkness(CSpriteOutlineManager* pSOM, WORD color, BYTE DarkBits);

		// ShadowSprite
		void	BltShadowSprite(POINT* pPoint, CShadowSprite* pSprite);
		void	BltShadowSpriteSmall(POINT* pPoint, CShadowSprite* pSprite, BYTE shift);
		void	BltShadowSpriteDarkness(POINT* pPoint, CShadowSprite* pSprite, BYTE DarkBits);
		void	BltShadowSprite4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel);
		void	BltShadowSpriteSmall4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel, BYTE shift);

	// class 내부에서 쓰는 함수
	protected :	
		bool	ClippingRectToPoint(RECT*& pRect, POINT*& pPoint);

	public :
		static void		InitEffectTable();
		static void		memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyColor(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyScale(WORD* pDest, WORD destPitch, WORD* pSource, WORD pixels);
		static void		memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyBrightness(WORD* pDest, WORD* pSource, WORD pixels);		
		static WORD		memcpyAlpha1Pixel(WORD pDest, WORD pSource)
		{
			int sr,sg,sb, dr,dg,db;

			// 한점 찍기
			sr = CDirectDraw::Red(pSource);
			sg = CDirectDraw::Green(pSource);
			sb = CDirectDraw::Blue(pSource);

			dr = CDirectDraw::Red(pDest);
			dg = CDirectDraw::Green(pDest);
			db = CDirectDraw::Blue(pDest);		
			
			return ((s_Value1 * (sb - db) >> 5) + db |
						((s_Value1 * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
						((s_Value1 * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
		}

		// memcpy..()에서 이용하는 값들
		static int		s_Value1;	// DarkBits, Alpha
		static int		s_Value2;	// 32-Alpha
		static int		s_Value3;

	public :
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
			EFFECT_SCREEN_ALPHA,
			MAX_EFFECT
		};

	public :
		// 특수효과 출력용
		static void		SetEffect(enum FUNCTION_EFFECT func)	{ s_pMemcpyEffectFunction = s_pMemcpyEffectFunctionTable[func]; }
		static void		SetPalEffect(enum FUNCTION_EFFECT func)	{ s_pMemcpyPalEffectFunction = s_pMemcpyPalEffectFunctionTable[func]; }
		static void		memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels)
		{
			(*s_pMemcpyEffectFunction)(pDest, pSource, pixels);
		}
		
		static void		memcpyPalEffect(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
		{
			(*s_pMemcpyPalEffectFunction)(pDest, pSource, pixels, pal);
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
		static void		memcpyEffectScreenAlpha(WORD* pDest, WORD* pSource, WORD pixels);

		// memcpyEffect
		static void		memcpyPalEffectDarker(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectGrayScale(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectLighten(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectDarken(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectColorDodge(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectScreen(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectDodgeBurn(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectDifferent(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectGradation(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);		
		static void		memcpyPalEffectSimpleOutline(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);	
		static void		memcpyPalEffectWipeOut(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectNet(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectGrayScaleVarious(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		static void		memcpyPalEffectScreenAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
		
		

		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunction;
		static FUNCTION_MEMCPYPALEFFECT		s_pMemcpyPalEffectFunction;
		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunctionTable[MAX_EFFECT];
		static FUNCTION_MEMCPYPALEFFECT		s_pMemcpyPalEffectFunctionTable[MAX_EFFECT];
		
		static WORD		s_EffectScreenTableR[32][32];
		static WORD		s_EffectScreenTableG[32][32];
		static WORD		s_EffectScreenTableB[32][32];
};

#endif

