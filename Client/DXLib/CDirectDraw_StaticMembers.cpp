/*-----------------------------------------------------------------------------

	CSDLGraphics_StaticMembers.cpp

	This file contains only the static member definitions for CSDLGraphics class.

-----------------------------------------------------------------------------*/

#include "../../basic/Typedef.h"
#include "CDirectDraw.h"

// Static member definitions
BYTE CSDLGraphics::s_bSHIFT_R = 11;
BYTE CSDLGraphics::s_bSHIFT_G = 5;
BYTE CSDLGraphics::s_bSHIFT_B = 0;
BYTE CSDLGraphics::s_bSHIFT_A = 0;
BYTE CSDLGraphics::s_bSHIFT4_R = 12;
BYTE CSDLGraphics::s_bSHIFT4_G = 8;
BYTE CSDLGraphics::s_bSHIFT4_B = 4;

// Additional static members
WORD CSDLGraphics::s_wMASK_SHIFT[5] = {0, 0, 0, 0, 0};
DWORD CSDLGraphics::s_dwMASK_SHIFT[5] = {0, 0, 0, 0, 0};
uint64_t CSDLGraphics::s_qwMASK_SHIFT[5] = {0, 0, 0, 0, 0};

WORD CSDLGraphics::s_wMASK_RGB[6] = {0, 0, 0, 0, 0, 0};
DWORD CSDLGraphics::s_dwMASK_RGB[6] = {0, 0, 0, 0, 0, 0};
uint64_t CSDLGraphics::s_qwMASK_RGB[6] = {0, 0, 0, 0, 0, 0};

uint64_t CSDLGraphics::s_qwMASK_ALPHA0 = 0;
uint64_t CSDLGraphics::s_qwMASK_ALPHA1 = 0;
DWORD CSDLGraphics::s_dwMASK_ALPHA0 = 0;
DWORD CSDLGraphics::s_dwMASK_ALPHA1 = 0;
WORD CSDLGraphics::s_wMASK_ALPHA0 = 0;
WORD CSDLGraphics::s_wMASK_ALPHA1 = 0;

DWORD CSDLGraphics::s_dwMASK_SHIFT_COUNT[5] = {0, 0, 0, 0, 0};
DWORD CSDLGraphics::s_dwMASK_RGB_COUNT[6] = {0, 0, 0, 0, 0, 0};

WORD CSDLGraphics::RED = 0xF800;
WORD CSDLGraphics::GREEN = 0x07E0;
WORD CSDLGraphics::BLUE = 0x001F;
WORD CSDLGraphics::WHITE = 0xFFFF;

bool CSDLGraphics::s_bUseIMEHandle = false;
