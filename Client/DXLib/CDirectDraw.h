/*-----------------------------------------------------------------------------

	CDirectDraw.h

	DirectX class library component.
	`16bit version.

	1999.12.28. sigi, KJTINC

-----------------------------------------------------------------------------*/

#ifndef	__CDIRECTDRAW_H__
#define	__CDIRECTDRAW_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <DDraw.h>


typedef	unsigned __int64		QWORD;


/*-----------------------------------------------------------------------------
  Class DirectDraw
  `CDirectDrawSurface에서 상속받는다. 따라서 member들을 static으로 하여야 한다.
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

	// Surface의 크기
	static WORD								m_ScreenWidth;
	static WORD								m_ScreenHeight;

	static bool								m_b565;

	// 3D 사용?
	static bool								m_b3D;

	static bool								m_bMMX;
	static bool								m_bGammaControl;
	static DDGAMMARAMP						m_DDGammaRamp;
	static WORD								m_GammaStep;
	static WORD								m_AddGammaStep[3];

	// 창모드에 필요한 것들
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
	// Window Message 처리
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
	static inline WORD	Convert555to565(WORD pixel)		// 5:5:5를 5:6:5로 바꿀 때
	{
		return ((pixel & 0x7FE0)<<1)	// RRRRRGGG GG000000
				| (pixel & 0x001F);		// 00000000 000BBBBB										
	}

	//     RRRRRGGG GGGBBBBB
	// --> 0RRRRRGG GGGBBBBB 
	static inline WORD Convert565to555(WORD pixel)		// 5:6:5를 5:5:5로 바꿀 때
	{
		return (((pixel & 0xFFE0) >> 1)		// 0RRRRRGG GGGG0000
				& 0x7FE0)					// 0RRRRRGG GGG00000
				| (pixel & 0x001F);			// 00000000 000BBBBB
	}

	//-----------------------------------
	// (R,G,B) <--> RGB
	//-----------------------------------
	// 5:5:5이면 (32,32,32)
	// 5:6:5이면 (32,64,32)
	// `단순히 합치기만 한다.
	static inline WORD	Color(const BYTE& r, const BYTE& g, const BYTE& b)
	{			
		return (r << s_bSHIFT_R) | (g << s_bSHIFT_G) | b;
	}

	// Red값
	static inline BYTE	Red(const WORD& c)
	{			
		return (c >> s_bSHIFT_R);
	}

	// Green값
	static inline BYTE	Green(const WORD& c)
	{
		//return ((c >> s_bSHIFT_G) & s_bMASK_G);	

		BYTE g;
		g = c >> s_bSHIFT_G;
		g &= 0x1F;
		return g;			
	}

	// Blue값
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
	
	// 기본 색상
	static WORD		RED;
	static WORD		GREEN;
	static WORD		BLUE;
	static WORD		WHITE;

	static bool		s_bUseIMEHandle;
};


#endif