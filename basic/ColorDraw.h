/*-----------------------------------------------------------------------------

	ColorDraw.h

	Color manipulation utilities extracted from CDirectDraw.
	These are pure mathematical operations for RGB 5:6:5 format,
	independent of any graphics API.

	2025.02.04 - Extracted to eliminate SpriteLib->DXLib dependency

-----------------------------------------------------------------------------*/

#ifndef __COLORDRAW_H__
#define __COLORDRAW_H__

#include "Platform.h"

class ColorDraw
{
public:
	//----------------------------------------------------------------------
	// Color component extraction (RGB 5:6:5 format)
	// R: bits 11-15, G: bits 5-10, B: bits 0-4
	//----------------------------------------------------------------------
	static inline BYTE	Red(const WORD& c)
	{
		return (c >> 11) & 0x1F;
	}

	static inline BYTE	Green(const WORD& c)
	{
		return (c >> 5) & 0x3F;
	}

	static inline BYTE	Blue(const WORD& c)
	{
		return c & 0x1F;
	}

	//----------------------------------------------------------------------
	// Pixel format detection
	//----------------------------------------------------------------------
	static inline bool	Is565()
	{
		return true;  // SDL2 backend always uses RGB 5:6:5
	}

	//----------------------------------------------------------------------
	// Color creation
	//----------------------------------------------------------------------
	static inline WORD	Color(BYTE r, BYTE g, BYTE b)
	{
		// For 5:6:5 format: R at bits 11-15, G at bits 5-10, B at bits 0-4
		return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
	}

	//----------------------------------------------------------------------
	// Color conversion (5:5:5 to 5:6:5 and vice versa)
	//----------------------------------------------------------------------
	static inline WORD	Convert555to565(WORD pixel)
	{
		return ((pixel & 0x7FE0) << 1) | (pixel & 0x001F);
	}

	static inline WORD	Convert565to555(WORD pixel)
	{
		return (pixel & 0xFFE0) >> 1;
	}

	//----------------------------------------------------------------------
	// Static shift values for color recombination
	// These match RGB 5:6:5 format: R@11, G@5, B@0
	//----------------------------------------------------------------------
	static BYTE	s_bSHIFT_R;	// = 11 for RGB 5:6:5
	static BYTE	s_bSHIFT_G;	// = 5 for RGB 5:6:5
	static BYTE	s_bSHIFT_B;	// = 0 for RGB 5:6:5
	static BYTE	s_bSHIFT_A;	// = 0 (unused)

	//----------------------------------------------------------------------
	// Static shift values for 4:4:4:4 ARGB format
	//----------------------------------------------------------------------
	static BYTE	s_bSHIFT4_R;	// = 12 for ARGB 4:4:4:4
	static BYTE	s_bSHIFT4_G;	// = 8 for ARGB 4:4:4:4
	static BYTE	s_bSHIFT4_B;	// = 4 for ARGB 4:4:4:4
	static BYTE	s_bSHIFT4_A;	// = 0 for ARGB 4:4:4:4

	//----------------------------------------------------------------------
	// Static mask arrays (for compatibility with original code)
	//----------------------------------------------------------------------
	static WORD	s_wMASK_SHIFT[5];
	static DWORD	s_dwMASK_SHIFT[5];
	static uint64_t	s_qwMASK_SHIFT[5];

	static WORD	s_wMASK_RGB[6];
	static DWORD	s_dwMASK_RGB[6];
	static uint64_t	s_qwMASK_RGB[6];

	static uint64_t	s_qwMASK_ALPHA0;
	static uint64_t	s_qwMASK_ALPHA1;
	static DWORD	s_dwMASK_ALPHA0;
	static DWORD	s_dwMASK_ALPHA1;
	static WORD	s_wMASK_ALPHA0;
	static WORD	s_wMASK_ALPHA1;

	static DWORD	s_dwMASK_SHIFT_COUNT[5];
	static DWORD	s_dwMASK_RGB_COUNT[6];

	//----------------------------------------------------------------------
	// Standard color constants
	//----------------------------------------------------------------------
	static WORD	RED;
	static WORD	GREEN;
	static WORD	BLUE;
	static WORD	WHITE;

	static bool	s_bUseIMEHandle;

	//----------------------------------------------------------------------
	// Initialization
	//----------------------------------------------------------------------
	static void InitMask(bool b565);
};

#endif // __COLORDRAW_H__
