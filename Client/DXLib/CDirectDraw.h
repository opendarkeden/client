/*-----------------------------------------------------------------------------

	CDirectDraw.h

	DirectX class library component.
	`16bit version.

	1999.12.28. sigi, KJTINC

-----------------------------------------------------------------------------*/

#ifndef	__CDIRECTDRAW_H__
#define	__CDIRECTDRAW_H__

/* Platform-specific includes */
#ifdef PLATFORM_WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif

	#include <DDraw.h>
	typedef	unsigned __int64		QWORD;
#else
	/* Non-Windows platform: Use stub types */
	#include "../basic/Platform.h"
	#include <cstring>

	/* Basic type definitions if not already defined */
	#ifndef LONG_DEFINED
	#define LONG_DEFINED
	typedef long LONG;
	#endif

	#ifndef LPVOID_DEFINED
	#define LPVOID_DEFINED
	typedef void* LPVOID;
	#endif

	/* Forward declarations for DirectDraw types */
	typedef struct IDirectDraw* LPDIRECTDRAW7;
	typedef struct IDirectDrawSurface* LPDIRECTDRAWSURFACE7;
	typedef struct IDirectDrawGammaControl* LPDIRECTDRAWGAMMACONTROL;

	/* DDPIXELFORMAT structure - must be defined before DDSURFACEDESC2 */
	/* Only define if not already defined by CDirect3D.h */
#ifndef __CDIRECT3D_H__
	typedef struct tagPIXELFORMAT {
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwFourCC;
		DWORD dwRGBBitCount;
		DWORD dwRBitMask;
		DWORD dwGBitMask;
		DWORD dwBBitMask;
		DWORD dwRGBAlphaBitMask;
	} DDPIXELFORMAT, *LPDDPIXELFORMAT;
#endif // __CDIRECT3D_H__

	/* DDSCAPS2 structure */
	typedef struct {
		DWORD dwCaps;
		DWORD dwCaps2;
		DWORD dwCaps3;
		DWORD dwCaps4;
		DWORD dwVolumeDepth;
	} DDSCAPS2;

	typedef struct {
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwHeight;
		DWORD dwWidth;
		LPVOID lpSurface;
		DDPIXELFORMAT ddpfPixelFormat;
		DDSCAPS2 ddsCaps;
		LONG lPitch;
	} DDSURFACEDESC2;

	typedef struct {
		WORD red[256];
		WORD green[256];
		WORD blue[256];
	} DDGAMMARAMP;

	/* Common Windows types - define if not already defined */
	#ifndef RECT_DEFINED
	#define RECT_DEFINED
	typedef struct tagRECT {
		LONG left;
		LONG top;
		LONG right;
		LONG bottom;
	} RECT, *PRECT, *LPRECT;
	#endif

	#ifndef POINT_DEFINED
	#define POINT_DEFINED
	typedef struct tagPOINT {
		LONG x;
		LONG y;
	} POINT, *PPOINT, *LPPOINT;
	#endif

	#ifndef COLORREF_DEFINED
	#define COLORREF_DEFINED
	typedef DWORD COLORREF;
	#endif

	#ifndef SIZE_DEFINED
	#define SIZE_DEFINED
	typedef struct tagSIZE {
		LONG cx;
		LONG cy;
	} SIZE, *PSIZE, *LPSIZE;
	#endif

	/* Use uint64_t for QWORD on non-Windows */
	#include <cstdint>
	typedef uint64_t QWORD;

	/* DirectDraw constants */
	#define DDSD_CAPS 0x00000001
	#define DDSD_HEIGHT 0x00000002
	#define DDSD_WIDTH 0x00000004
	#define DDSD_PITCH 0x00000008
	#define DDSD_PIXELFORMAT 0x00001000
	#define DDSD_LPSURFACE 0x00000800

	/* GDI types */
	#ifndef HBITMAP_DEFINED
	#define HBITMAP_DEFINED
	typedef void* HBITMAP;
	#endif

	/* Memory utilities */
	#ifndef ZeroMemory
	#define ZeroMemory(p, s) memset((p), 0, (s))
	#endif

	#define DDSCL_FULLSCREEN 0
	#define DDSCL_EXCLUSIVE 0
#endif


/*-----------------------------------------------------------------------------
  Class DirectDraw
  `CDirectDrawSurface���� ��ӹ޴´�. ���� member���� static���� �Ͽ��� �Ѵ�.
-----------------------------------------------------------------------------*/
class CDirectDraw
{	
protected:
	static LPDIRECTDRAW7					m_pDD;			// DirectDraw object
	static LPDIRECTDRAWSURFACE7				m_pDDSPrimary;	// DirectDraw primary surface
	static LPDIRECTDRAWSURFACE7				m_pDDSBack;		// DirectDraw back surface
	static DDSURFACEDESC2					m_ddsd;		// DirectDraw Surface Description

	static LPDIRECTDRAWGAMMACONTROL			m_pDDGammaControl;	// gamma control

	static bool								m_bFullscreen;	// Is FullScreen Mode?

	static HWND								m_hWnd;

	// Surface�� ũ��
	static WORD								m_ScreenWidth;
	static WORD								m_ScreenHeight;

	static bool								m_b565;

	// 3D ���?
	static bool								m_b3D;

	static bool								m_bMMX;
	static bool								m_bGammaControl;
	static DDGAMMARAMP						m_DDGammaRamp;
	static WORD								m_GammaStep;
	static WORD								m_AddGammaStep[3];

	// â��忡 �ʿ��� �͵�
	static RECT								m_rcWindow;		// Saves the window size & pos.
	static RECT								m_rcViewport;	// Pos. & size to blt from
	static RECT								m_rcScreen;		// Screen pos. for blt

	static bool		Check565();
	static bool		CheckGammaControl();
	static bool		CheckMMX();	

public:
	CDirectDraw();
	~CDirectDraw();

	enum SCREENMODE { FULLSCREEN, WINDOWMODE };

	static bool		Init(HWND hWnd, WORD wWidth, WORD wHeight, SCREENMODE mode, bool b3D=false, bool bUsseIMEWindow = false);
	static bool		InitFullscreen(WORD wWidth, WORD wHeight);
	static bool		InitWindowMode(WORD wWidth, WORD wHeight);
	
	static void		InitMask(bool b565);

	static void		ReleaseSurface();
	static void		ReleaseAll();

	static bool		RestoreAllSurfaces();

	static void		Flip();
	static void		FlipToGDISurface();

	static void		SetGammaRamp(WORD step = -1);
	static void		RestoreGammaRamp();
	static void		SetAddGammaRamp(WORD rStep = 0, WORD gStep = 0, WORD bStep = 0);

	static void		DirectDrawFailed(const char * str);

	//------------------------------------
	// Get
	//------------------------------------
	static HWND		GetHwnd()			{ return m_hWnd; }
	static const LPDIRECTDRAW7 GetDD()	{ return m_pDD; }

	//------------------------------------
	// Window Message ó��
	//------------------------------------
	static void		OnMove();

	//------------------------------------
	// Get Functions
	//------------------------------------
	static inline bool		IsFullscreen()			{ return m_bFullscreen; }
	static inline WORD		GetScreenWidth()		{ return m_ScreenWidth; }
	static inline WORD		GetScreenHeight()		{ return m_ScreenHeight; }
	static inline bool		IsMMX()	 				{ return m_bMMX; }
	static inline bool		IsSupportGammaControl()	{ return m_bGammaControl; }
	static inline bool		Is565()	 				{ return m_b565; }

public:
	static int		Get_Count_Rbit();
	static int		Get_Count_Gbit();
	static int		Get_Count_Bbit();
	static DWORD		Get_R_Bitmask();
	static DWORD		Get_G_Bitmask();
	static DWORD		Get_B_Bitmask();
	static DWORD		Get_BPP();

	//     0RRRRRGG GGGBBBBB 
	// --> RRRRRGGG GG0BBBBB
	static inline WORD	Convert555to565(WORD pixel)		// 5:5:5�� 5:6:5�� �ٲ� ��
	{
		return ((pixel & 0x7FE0)<<1)	// RRRRRGGG GG000000
				| (pixel & 0x001F);		// 00000000 000BBBBB										
	}

	//     RRRRRGGG GGGBBBBB
	// --> 0RRRRRGG GGGBBBBB 
	static inline WORD Convert565to555(WORD pixel)		// 5:6:5�� 5:5:5�� �ٲ� ��
	{
		return (((pixel & 0xFFE0) >> 1)		// 0RRRRRGG GGGG0000
				& 0x7FE0)					// 0RRRRRGG GGG00000
				| (pixel & 0x001F);			// 00000000 000BBBBB
	}

	//-----------------------------------
	// (R,G,B) <--> RGB
	//-----------------------------------
	// 5:5:5�̸� (32,32,32)
	// 5:6:5�̸� (32,64,32)
	// `�ܼ��� ��ġ�⸸ �Ѵ�.
	static inline WORD	Color(const BYTE& r, const BYTE& g, const BYTE& b)
	{			
		return (r << s_bSHIFT_R) | (g << s_bSHIFT_G) | b;
	}

	// Red��
	static inline BYTE	Red(const WORD& c)
	{			
		return (c >> s_bSHIFT_R);
	}

	// Green��
	static inline BYTE	Green(const WORD& c)
	{
		//return ((c >> s_bSHIFT_G) & s_bMASK_G);	

		BYTE g;
		g = c >> s_bSHIFT_G;
		g &= 0x1F;
		return g;			
	}

	// Blue��
	static inline BYTE	Blue(const WORD& c)
	{
		return (c & 0x1F);
	}

public :
	// Shift Mask
	static WORD		s_wMASK_SHIFT[5];
	static DWORD	s_dwMASK_SHIFT[5];
	static QWORD	s_qwMASK_SHIFT[5];

	// Rgb Mask - R,G,B,RG,RB,GB
	static WORD		s_wMASK_RGB[6];
	static DWORD	s_dwMASK_RGB[6];
	static QWORD	s_qwMASK_RGB[6];

	// Alpha Mask
	static QWORD	s_qwMASK_ALPHA0;
	static QWORD	s_qwMASK_ALPHA1;
	static DWORD	s_dwMASK_ALPHA0;
	static DWORD	s_dwMASK_ALPHA1;
	static WORD		s_wMASK_ALPHA0;
	static WORD		s_wMASK_ALPHA1;

	// 1 Byte ShiftCount & Mask
	static BYTE		s_bSHIFT_R; 
	static BYTE		s_bSHIFT_G;
	static BYTE		s_bSHIFT_B;
	
	// 1 Byte ShiftCount & Mask for 4444
	static BYTE		s_bSHIFT4_R; 	
	static BYTE		s_bSHIFT4_G;	
	static BYTE		s_bSHIFT4_B;
	
	// �⺻ ����
	static WORD		RED;
	static WORD		GREEN;
	static WORD		BLUE;
	static WORD		WHITE;

	static bool		s_bUseIMEHandle;
};


#endif