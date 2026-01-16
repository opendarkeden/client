/*-----------------------------------------------------------------------------

	CDirectDraw_StaticMembers.cpp

	This file contains only the static member definitions for CDirectDraw class.

-----------------------------------------------------------------------------*/

#include "../../basic/Typedef.h"

// Forward declaration
class CDirectDraw {
public:
	static BYTE s_bSHIFT_R;
	static BYTE s_bSHIFT_G;
	static BYTE s_bSHIFT_B;
	static BYTE s_bSHIFT4_R;
	static BYTE s_bSHIFT4_G;
	static BYTE s_bSHIFT4_B;
};

// Static member definitions
BYTE CDirectDraw::s_bSHIFT_R = 11;
BYTE CDirectDraw::s_bSHIFT_G = 5;
BYTE CDirectDraw::s_bSHIFT_B = 0;
BYTE CDirectDraw::s_bSHIFT4_R = 12;
BYTE CDirectDraw::s_bSHIFT4_G = 8;
BYTE CDirectDraw::s_bSHIFT4_B = 4;
