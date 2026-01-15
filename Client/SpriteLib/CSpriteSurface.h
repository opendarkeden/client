//----------------------------------------------------------------------
// CDirectDrawSurface.h
//----------------------------------------------------------------------
//
//
// 5:6:5 ��  5:5:5�� ���̴� 
// Bit Mask�� ���� ó���Ǵ� �κп��� �߻��ϴµ�
// CDirectDraw class�� InitMask()���� Video Card�� �´�
// ������ Mask�� �����ϹǷ� �׳�~ CDirectDraw�� Mask�� ����ϸ� �ȴ�.
//
//
//
//----------------------------------------------------------------------
/*

  < DDSurface�� �����Ǵ� MEMORY >

	- DDSCAPS_SYSTEMMEMORY�� Lock�� ����� �� ������.
	- DDSCAPS_VIDEOMEMORY�� Blt�� ����� �� ������.
	  ��, VIDEOMEMORY�� �����ϸ� Surface�� �������� �����Ƿ� error!
	- ������ �� �� �ָ� VIDEOMEMORY�� ��밡���ϸ� ����ϰ� 
   	  �ƴϸ� SYSTEMMEMORY�� ����Ѵ�.
	
  < Alpha Blending >
    - ����Ϸ��� DDSCAPS_SYSTEMMEMORY�� ����ϴ°� ����.
	- Surface --> Surface���� Alpha Blending����
	  Memory(SpriteFile) --> Surface���� Alpha Blending�� ������.

  < Sprite File >
    - 0�� ���� �� �Ͱ� �� �� �� �ΰ����� ��찡 �ִ�.
	- 0�� ���� �� ���� �������� �ִ� ��� ��쿡 ������.
	  ��, MMX instruction���� �������� ������ ��찡 �� ��������...

  < ���� ���� ���� >
    - Clipping�� �Ǵ� �Լ��� �� �Ǵ� �Լ��� �ִµ�
	  ������ ��쿡 ���缭 ����ؾ��� ���̴�. �ӵ� ������ �����ϱ�!


*/
//----------------------------------------------------------------------

#ifndef	__CSPRITESURFACE_H__
#define	__CSPRITESURFACE_H__

/* Backend selection */
#ifdef SPRITELIB_BACKEND_SDL
	#include "SpriteLibBackend.h"
	/* SDL backend: Independent class, not inheriting from CDirectDrawSurface */
	#define SPRITESURFACE_STANDALONE
	#include "../../basic/2d.h"  // For S_SURFACEINFO
#endif

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

#ifdef SPRITESURFACE_STANDALONE

/* ============================================================================
 * SDL Backend: Standalone CSpriteSurface (not inheriting from CDirectDrawSurface)
 * ============================================================================ */

class CSpriteSurface {
	public :
		CSpriteSurface();
		~CSpriteSurface();

		//------------------------------------------------------------
		// Surface Initialization
		//------------------------------------------------------------
		bool Init(int width, int height);
		bool InitFromFile(const char* filename);
		void Release();

		//------------------------------------------------------------
		// Drawing Functions
		//------------------------------------------------------------
		void	DrawRect(RECT* rect, WORD color);
		void	HLine(int x, int y, int length, WORD color);
		void	VLine(int x, int y, int length, WORD color);
		void	Line(int x1, int y1, int x2, int y2, WORD color);
		void	FillRect(RECT* rect, WORD color);

		//------------------------------------------------------------
		// BltFast methods (stubs for compatibility)
		// Note: In SDL backend, use CSpriteSurface* instead of CDirectDrawSurface*
		//------------------------------------------------------------
		void	BltHalf(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect);
		void	BltNoColorkey(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect);
		void	BltDarkness(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, BYTE DarkBits);
		void	BltBrightness(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, BYTE BrightBits);
		void	BltDarknessFilter(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, WORD TransColor=0);

		// Change brightness
		void	ChangeBrightnessBit(RECT* pRect, BYTE DarkBits);
		void	BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2);

		//------------------------------------------------------------
		// Sprite Blitting Methods
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
		void	BltIndexSpriteBrightness(POINT* pPoint, CIndexSprite* pSprite, BYTE BrightBits);

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

		//------------------------------------------------------------
		// Static Effect Functions (stubs for compatibility)
		//------------------------------------------------------------
		static void		InitEffectTable();
		static void		memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyColor(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyScale(WORD* pDest, WORD destPitch, WORD* pSource, WORD pixels);
		static void		memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels);
		static void		memcpyBrightness(WORD* pDest, WORD* pSource, WORD pixels);

		static WORD		memcpyAlpha1Pixel(WORD pDest, WORD pSource) { return 0; }

		static int		s_Value1;
		static int		s_Value2;
		static int		s_Value3;

	public:
		enum FUNCTION_EFFECT
		{
			EFFECT_DARKER = 0,
			EFFECT_GRAY_SCALE,
			EFFECT_GRAY_SCALE_VARIOUS,
			EFFECT_SCREEN,
			MAX_EFFECT
		};

		static void		SetEffect(enum FUNCTION_EFFECT func) {}
		static void		memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels) {}
		static void		memcpyPalEffect(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal) {}

		static void		memcpyEffectDarker(WORD* pDest, WORD* pSource, WORD pixels) {}
		static void		memcpyPalEffectDarker(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal) {}

		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunction;
		static FUNCTION_MEMCPYPALEFFECT		s_pMemcpyPalEffectFunction;
		static FUNCTION_MEMCPYEFFECT		s_pMemcpyEffectFunctionTable[MAX_EFFECT];
		static FUNCTION_MEMCPYPALEFFECT		s_pMemcpyPalEffectFunctionTable[MAX_EFFECT];

		static WORD		s_EffectScreenTableR[32][32];
		static WORD		s_EffectScreenTableG[32][32];
		static WORD		s_EffectScreenTableB[32][32];

		//------------------------------------------------------------
		// DirectX Compatibility Methods (Stubs for SDL backend)
		//------------------------------------------------------------
		bool				Restore();
		void*				Lock(RECT* rect = NULL, DWORD* pitch = NULL);
		void				Unlock();
		int					GetSurfacePitch() const;
		void*				GetSurfacePointer();
		int					GetWidth() const;
		int					GetHeight() const;

#ifdef SPRITELIB_BACKEND_SDL
		// Lock/Unlock methods for SDL backend compatibility (different signatures from Windows)
		bool				LockSDL();  // Renamed to avoid conflict with Lock()
		void				UnlockSDL();
		bool				IsLock();
		void				GetSurfaceInfo(S_SURFACEINFO* info);
		// GetDDSD compatibility wrapper - returns pointer to internal surface info
		S_SURFACEINFO*		GetDDSD();
#endif
		bool				InitTextureSurface(int width, int height, void* pixels = NULL, void* pixelFormat = NULL);

		// Off-screen surface initialization (compatibility with CDirectDrawSurface)
		bool				InitOffsurface(int width, int height);
		void				SetTransparency(int value);
		int					GetTransparency() const;

		// GDI text rendering (stub for SDL backend - text rendering not implemented)
		void				GDI_Text(int x, int y, const char* text, DWORD color);

		// FillSurface (compatibility with CDirectDrawSurface)
		void				FillSurface(WORD color);

		// Clipping methods (compatibility with CDirectDrawSurface)
		void				SetClip(RECT* rect);
		void				SetClipNULL();

		// Blt method (compatibility with CDirectDrawSurface)
		void				Blt(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect);

	protected:
		bool	ClippingRectToPoint(RECT*& pRect, POINT*& pPoint);

#ifdef SPRITELIB_BACKEND_SDL
		/* Backend surface handle */
		spritectl_surface_t m_backend_surface;
		int m_width;
		int m_height;
		int m_transparency;  // Transparency value for compatibility
#endif
};

#else

/* ============================================================================
 * Windows Backend: Original implementation inheriting from CDirectDrawSurface
 * ============================================================================ */

#include "CDirectDrawSurface.h"

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
		// Drawing �Լ�
		//------------------------------------------------------------
		void	DrawRect(RECT* rect, WORD color);
		void	HLine(int x, int y, int length, WORD color);		// ����
		void	VLine(int x, int y, int length, WORD color);		// ����
		void	Line(int x1, int y1, int x2, int y2, WORD color);	// ����
		
	
		//------------------------------------------------------------
		//
		// Lock & Draw Functions
		//
		//------------------------------------------------------------

		//------------------------------------------------------------
		// ��Ӱ� �ϱ�
		//------------------------------------------------------------
		void	ChangeBrightnessBit(RECT*  pRect, BYTE DarkBits);
		
		//------------------------------------------------------------
		// �ܻ� Alpha blending (ȭ���� pRect�� color�� alpha��..)
		// alpha2�� 0�̸� color��.. 31�̸�.. ���� surface
		//------------------------------------------------------------
		void	BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2);

		//virtual void	BltHalf(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, DWORD ColorKey=0);
		//virtual void	BltDarkness(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, BYTE bits, WORD ColorKey=0);		

		//------------------------------------------------------------
		// AlphaDepth�� 1~32�̴�.
		//------------------------------------------------------------		
		//virtual void	BltTransAlpha(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect, int alphaDepth, DWORD ColorKey=0);

		//------------------------------------------------------------
		//
		// CSprite ���� �Լ�
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

	// class ���ο��� ���� �Լ�
	protected :
		bool	ClippingRectToPoint(RECT*& pRect, POINT*& pPoint);

#ifdef SPRITELIB_BACKEND_SDL
		/* Backend surface handle for SDL2 implementation */
		spritectl_surface_t m_backend_surface;
#endif

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

			// ���� ���
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

		// memcpy..()���� �̿��ϴ� ����
		static int		s_Value1;	// DarkBits, Alpha
		static int		s_Value2;	// 32-Alpha
		static int		s_Value3;

	public :
		// memcpyEffect�� ����
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
		// Ư��ȿ�� ��¿�
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

#endif /* SPRITESURFACE_STANDALONE */

#endif /* __CSPRITESURFACE_H__ */

