/*-----------------------------------------------------------------------------

	CDirectDraw.h

	SDL2 Implementation (Cross-platform)
	Windows DirectDraw implementation removed - using SDL2 on all platforms
	NOTE: This class is deprecated. Use SDL2 surfaces and textures directly instead.

	1999.12.28. sigi, KJTINC
	2025: Refactored for SDL2 cross-platform support

-----------------------------------------------------------------------------*/

#ifndef	__CSDLGRAPHICS_H__
#define	__CSDLGRAPHICS_H__

/* Cross-platform includes */
#include "../basic/Platform.h"
#include <cstring>
#include <cstdint>

/* Windows type compatibility - define for all platforms */
#ifndef LPVOID_DEFINED
#define LPVOID_DEFINED
typedef void* LPVOID;
#endif

/* DirectDraw type stubs - kept for compatibility with old code */
typedef struct IDirectDraw* LPDIRECTDRAW7;
typedef struct IDirectDrawSurface* LPDIRECTDRAWSURFACE7;
typedef struct IDirectDrawGammaControl* LPDIRECTDRAWGAMMACONTROL;

/* DDPIXELFORMAT structure stub */
#ifndef TAGPIXELFORMAT_DEFINED
#define TAGPIXELFORMAT_DEFINED
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
#define DDPIXELFORMAT_DEFINED
#endif

/* DDSCAPS2 structure stub */
typedef struct {
	DWORD dwCaps;
	DWORD dwCaps2;
	DWORD dwCaps3;
	DWORD dwCaps4;
	DWORD dwVolumeDepth;
} DDSCAPS2;

/* DDSURFACEDESC2 structure stub */
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

/* DDGAMMARAMP structure stub */
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

/* QWORD type */
typedef uint64_t QWORD;

/* HWND type stub */
typedef void* HWND;

/* GDI types */
#ifndef HBITMAP_DEFINED
#define HBITMAP_DEFINED
typedef void* HBITMAP;
#endif

/* Memory utilities */
#ifndef ZeroMemory
#define ZeroMemory(p, s) memset((p), 0, (s))
#endif

/* DirectDraw constants */
#define DDSD_CAPS 0x00000001
#define DDSD_HEIGHT 0x00000002
#define DDSD_WIDTH 0x00000004
#define DDSD_PITCH 0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_LPSURFACE 0x00000800

/*-----------------------------------------------------------------------------
  Class CDirectDraw - SDL2 Cross-platform Stub
  NOTE: Most methods are stubs. Use SDL2 functions directly instead.
-----------------------------------------------------------------------------*/

class CSDLGraphics
{
public:
	// Constructor/Destructor (not implemented - class is deprecated)
	CSDLGraphics();
	~CSDLGraphics();

	enum SCREENMODE { FULLSCREEN, WINDOWMODE };

	// Color conversion utilities (still functional)
	static inline WORD	Color(const BYTE& r, const BYTE& g, const BYTE& b)
	{
		// For 5:6:5 format: R at bits 11-15, G at bits 5-10, B at bits 0-4
		return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
	}

	static inline BYTE	Red(const WORD& c)
	{
		// Extract R component (bits 11-15)
		return (c >> 11) & 0x1F;
	}

	static inline BYTE	Green(const WORD& c)
	{
		// Extract G component (bits 5-10)
		return (c >> 5) & 0x3F;
	}

	static inline BYTE	Blue(const WORD& c)
	{
		// Extract B component (bits 0-4)
		return c & 0x1F;
	}

	// Color conversion (5:5:5 to 5:6:5 and vice versa)
	static inline WORD	Convert555to565(WORD pixel)
	{
		return ((pixel & 0x7FE0) << 1) | (pixel & 0x001F);
	}

	static inline WORD	Convert565to555(WORD pixel)
	{
		return (((pixel & 0xFFE0) >> 1) & 0x7FE0) | (pixel & 0x001F);
	}

	// Bitmask methods
	static int		Get_Count_Rbit();
	static int		Get_Count_Gbit();
	static int		Get_Count_Bbit();
	static DWORD		Get_R_Bitmask();
	static DWORD		Get_G_Bitmask();
	static DWORD		Get_B_Bitmask();
	static DWORD		Get_BPP();

	// Property methods (stub implementations)
	static inline bool		IsFullscreen()			{ return true; }
	static inline WORD		GetScreenWidth()		{ return 800; }
	static inline WORD		GetScreenHeight()		{ return 600; }
	static inline bool		IsMMX()	 				{ return false; }
	static inline bool		IsSupportGammaControl()	{ return false; }
	static inline bool		Is565()	 				{ return true; }
	static inline HWND		GetHwnd()				{ return nullptr; }
	static inline LPDIRECTDRAW7 GetDD()			{ return nullptr; }

	// Display methods (stub implementations - use SDL2 instead)
	static inline void		Flip() { }
	static inline void		FlipToGDISurface() { }
	static inline void		OnMove() { }
	static inline bool		RestoreAllSurfaces() { return true; }
	static inline void		ReleaseSurface() { }
	static inline void		ReleaseAll() { }
	static inline void		SetGammaRamp(WORD step = (WORD)-1) { }
	static inline void		RestoreGammaRamp() { }
	static inline void		SetAddGammaRamp(WORD rStep = 0, WORD gStep = 0, WORD bStep = 0) { }
	static inline void		SetDisplayMode(WORD width, WORD height, WORD bpp, DWORD flags1, DWORD flags2) { }
	static inline void		RestoreDisplayMode() { }

	// InitMask - implemented in .cpp to initialize static arrays
	static void		InitMask(bool b565);

private:
	// DirectDraw objects (stub pointers)
	static LPDIRECTDRAW7					m_pDD;
	static LPDIRECTDRAWSURFACE7				m_pDDSPrimary;
	static LPDIRECTDRAWSURFACE7				m_pDDSBack;
	static LPDIRECTDRAWGAMMACONTROL			m_pDDGammaControl;

	// Screen properties
	static HWND								m_hWnd;
	static bool								m_bFullscreen;
	static WORD								m_ScreenWidth;
	static WORD								m_ScreenHeight;
	static bool								m_b565;
	static bool								m_b3D;
	static bool								m_bMMX;
	static bool								m_bGammaControl;
	static DDGAMMARAMP						m_DDGammaRamp;
	static WORD								m_GammaStep;
	static WORD								m_AddGammaStep[3];

	// Window rectangles
	static RECT								m_rcWindow;
	static RECT								m_rcViewport;
	static RECT								m_rcScreen;

public:
	// Static member variables (kept for compatibility)
	static WORD		s_wMASK_SHIFT[5];
	static DWORD	s_dwMASK_SHIFT[5];
	static QWORD	s_qwMASK_SHIFT[5];

	static WORD		s_wMASK_RGB[6];
	static DWORD	s_dwMASK_RGB[6];
	static QWORD	s_qwMASK_RGB[6];

	static QWORD	s_qwMASK_ALPHA0;
	static QWORD	s_qwMASK_ALPHA1;
	static DWORD	s_dwMASK_ALPHA0;
	static DWORD	s_dwMASK_ALPHA1;
	static WORD		s_wMASK_ALPHA0;
	static WORD		s_wMASK_ALPHA1;

	static BYTE		s_bSHIFT_R;
	static BYTE		s_bSHIFT_G;
	static BYTE		s_bSHIFT_B;
	static BYTE		s_bSHIFT_A;  // Added for compatibility

	static BYTE		s_bSHIFT4_R;
	static BYTE		s_bSHIFT4_G;
	static BYTE		s_bSHIFT4_B;

	static DWORD	s_dwMASK_SHIFT_COUNT[5];  // Added for compatibility
	static DWORD	s_dwMASK_RGB_COUNT[6];   // Added for compatibility

	static WORD		RED;
	static WORD		GREEN;
	static WORD		BLUE;
	static WORD		WHITE;

	static bool		s_bUseIMEHandle;
};

#endif
