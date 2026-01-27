//----------------------------------------------------------------------
// CSpriteSurface_Effects.cpp
//
// Pure pixel manipulation effect functions extracted from CSpriteSurface.cpp
// These functions are backend-agnostic and work with both DirectX and SDL
//
// 2025.01.27 - Extracted for SDL backend support
//----------------------------------------------------------------------

#include "Client_PCH.h"
#include "CSpriteSurface.h"
#include "CIndexSprite.h"
#include "MPalette.h"
#include "../DXLib/CDirectDraw.h"

// Local macros for CDirectDraw color functions
#define Red(c)      CDirectDraw::Red(c)
#define Green(c)    CDirectDraw::Green(c)
#define Blue(c)     CDirectDraw::Blue(c)

// Local macros for CDirectDraw bit shift constants
#define s_bSHIFT_R  CDirectDraw::s_bSHIFT_R
#define s_bSHIFT_G  CDirectDraw::s_bSHIFT_G
#define s_bSHIFT_B  CDirectDraw::s_bSHIFT_B

//----------------------------------------------------------------------
// Effect Copy - Darker
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특정효과를 처리한다.
//----------------------------------------------------------------------
void
CSpriteSurface::memcpyPalEffectDarker(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darker : BG - SPR_Filter
	//------------------------------------------------------------
	int		darkR,
			darkG,
			darkB;
			
	while (i--)
	{
		darkR = Red(pal[*pSource]);
		darkG = Green(pal[*pSource]);
		darkB = Blue(pal[*pSource]);;

		*pDest = 
			(((Red(*pDest) * darkR) >> 5) << s_bSHIFT_R)
			| (((Green(*pDest) * darkG) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * darkB) >> 5);
			
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - GrayScale
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGrayScale(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	BYTE average;

	while (i--)
	{
		average = ( Red(pal[*pSource]) +
					Green(pal[*pSource]) +
					Blue(pal[*pSource]) ) / 3;

		*pDest = (average << s_bSHIFT_R) 
				| (average << s_bSHIFT_G)
				| average;			
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Lighten
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectLighten(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Lighten : max(BG, SPR)
	//------------------------------------------------------------
	while (i--)
	{
		*pDest = 
			(max( Red(*pDest), Red(pal[*pSource]) ) << s_bSHIFT_R)
			| (max( Green(*pDest), Green(pal[*pSource]) ) << s_bSHIFT_G)
			| (max( Blue(*pDest), Blue(pal[*pSource]) ));
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Darken
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDarken(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darken : min(BG, SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			(min( Red(*pDest), Red(pal[*pSource]) ) << s_bSHIFT_R)
			| (min( Green(*pDest), Green(pal[*pSource]) ) << s_bSHIFT_G)
			| (min( Blue(*pDest), Blue(pal[*pSource]) ));

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - ColorDodge
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectColorDodge(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;
	
	//------------------------------------------------------------	
	// Color Dodge : (BG*MAXDAC)/(MAXDAC-SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			((Red(*pDest) << 5) / (32-Red(pal[*pSource])) << s_bSHIFT_R)
			| ((Green(*pDest) << 5) / (32-Green(pal[*pSource])) << s_bSHIFT_G)
			| (Blue(*pDest) << 5) / (32-Blue(pal[*pSource]));
			

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectScreen(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Screen :  TempSum=(MAXDAC-max(BG,SPR))/MAXDAC*min(BG,SPR)
	//           Screen=max(BG,SPR)+TempSum
	//------------------------------------------------------------	
	WORD temp;
	int d, s;
	//int m; 
	while (i--)
	{
		// R
		d = Red(*pDest);
		s = Red(pal[*pSource]);	

		//m = max(d,s);
		//temp = (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_R;
		temp = s_EffectScreenTableR[d][s];

		// G
		d = Green(*pDest);
		s = Green(pal[*pSource]);

		//m = max(d,s);
		//temp |= (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_G;
		temp |= s_EffectScreenTableG[d][s];
		
		// B
		d = Blue(*pDest);
		s = Blue(pal[*pSource]);

		//m = max(d,s);
		//temp |= ((32 - m) * min(d,s) >> 5) + m;
		temp |= s_EffectScreenTableB[d][s];

		// 
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectScreenAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
}

//----------------------------------------------------------------------
// Effect Copy - DodgeBurn
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDodgeBurn(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// DodgeBurn = (BG*(MAXDAC-SPR))/MAXDAC
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 		
			 (( (Red(*pDest) * (32 - Red(pal[*pSource])) ) >> 5) << s_bSHIFT_R)
			| ((( Green(*pDest) * (32 - Green(pal[*pSource]))) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * (32 - Blue(pal[*pSource]))) >> 5);
			

		pDest++;
		pSource++;
	}

}

//----------------------------------------------------------------------
// Effect Copy - Different
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDifferent(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	int temp;
	WORD d, s;
	
	while (i--)
	{
		temp = 0;

		d = Red(*pDest);
		s = Red(pal[*pSource]);
		temp |= (max(d,s)-min(d,s)) << 11;

		d = Green(*pDest);
		s = Green(pal[*pSource]);
		temp |= (max(d,s)-min(d,s)) << 5;

		d = Blue(*pDest);
		s = Blue(pal[*pSource]);
		temp |= max(d,s)-min(d,s);
		
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Gradation
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//
// s_Value1�� ColorSet��ȣ�̴�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGradation(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	WORD s;
	BYTE r, g, b;
	BYTE gradation;

	while (i--)
	{
		s = pal[*pSource];

		r = s >> s_bSHIFT_R;		
		g = (s >> s_bSHIFT_G) & 0x1F;
		b = s & 0x1F;

		gradation = CIndexSprite::ColorToGradation[ r + g + b ];

		*pDest = CIndexSprite::ColorSet[s_Value1][gradation];

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect SimpleOutline
//----------------------------------------------------------------------
// �� �������� �ܰ��� ���(������ ������ �� �� ���� ���)
// pixels�� 1�̻��̶�� �����Ѵ�.
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectSimpleOutline(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	// ù ��
	*pDest = pal[*pSource];
	
	int pixels_1 = pixels-1;

	// �� �� - pixels�� 1�϶� ���ϴ°ź��� �׳� ��°� �� ������?
	*(pDest+pixels_1) = *(pSource+pixels_1);	
}

//----------------------------------------------------------------------
// Effect WipeOut
//----------------------------------------------------------------------
// s_Value1 : �� ����� ����? 
//				64 - ����
//				0 - �� ���
//
// �߽ɿ������� �������� ����������.
//
// ***************
// ******   ******
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectWipeOut(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	int skipPixels = (pixels * s_Value1) >> 6;	// / 64
	int drawPixels = (pixels - skipPixels)>>1;
	int drawPixels2 = pixels - drawPixels - skipPixels;
	
	// [1] drawPixels��ŭ ����ϰ�..
	// [2] skipPixels ��ŭ �ǳʶ��
	// [3] drawPixels2��ŭ ���
	
	//------------------------------------------------------------	
	// drawPixels��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels<<1));
	pDest += drawPixels;
	pSource += drawPixels;

	//------------------------------------------------------------	
	// skipPixels��ŭ �ǳʶ�
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels2<<1));
	//pDest += drawPixels2;
	//pSource += drawPixels2;
}

//----------------------------------------------------------------------
// Effect Net
//----------------------------------------------------------------------
// s_Value1 : �ǳʶ�� ��
//
// �� �� ���.. s_Value1�� �� ����ŭ �ǳʶ��.
//
// ***************
// * * * * * * * *	: s_Value1 = 1
// *  *  *  *  *    : s_Value1 = 2
// *   *   *   *    : s_Value1 = 3
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectNet(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	int skipPixels = 1 + s_Value1;
	
	//------------------------------------------------------------	
	// drawPixels��ŭ ���
	//------------------------------------------------------------	
	do {
		//memcpy(pDest, pSource, (drawPixels<<1));
		*pDest = pal[*pSource];

		pDest += skipPixels;
		pSource += skipPixels;

		i -= skipPixels;
	} while (i > 0);
}

//----------------------------------------------------------------------
// Effect Copy - GrayScaleVarious
//----------------------------------------------------------------------
// s_Value1 �� �󸶳� gray�Ǵ°�?(-_-;)�̴�..
// s_Value1���� 0~31.. 32�ΰ�?.. - -
// 0�̸� ���� gray
// 32�̸� ��������
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGrayScaleVarious(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//int grayValue = s_Value1;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	int average;
	int r, g, b;

	while (i--)
	{	
		r = Red(pal[*pSource]);
		g = Green(pal[*pSource]);
		b = Blue(pal[*pSource]);

		average = ( r + g + b ) / 3;		// 0 ~ 31

		/*
		if (average==0)
		{
			*pDest = 0;
		}
		else
		{
		*/
			r = average + ((r-average)*s_Value1 >> 5);
			g = average + ((g-average)*s_Value1 >> 5);
			b = average + ((b-average)*s_Value1 >> 5);
		
			*pDest = (r << s_bSHIFT_R) 
					| (g << s_bSHIFT_G)
					| b;			
		//}
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// memcpyPalEffect - wrapper function
//----------------------------------------------------------------------
void
CSpriteSurface::memcpyPalEffect(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	if (s_pMemcpyPalEffectFunction != NULL) {
		(*s_pMemcpyPalEffectFunction)(pDest, pSource, pixels, pal);
	} else {
		// Default: simple copy
		for (int i = 0; i < pixels; i++) {
			pDest[i] = pal[pSource[i]];
		}
	}
}
