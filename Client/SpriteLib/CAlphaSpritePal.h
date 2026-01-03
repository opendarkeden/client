#ifndef __CALPHASPRITEPAL_H__
#define __CALPHASPRITEPAL_H__

#include <windows.h>
#include "CTypePack.h"
#include "CSpritePalBase.h"

class CAlphaSpritePal : public CSpritePalBase
{
public:
	void SetPixel(BYTE *pSource, WORD pitch, BYTE *pSourceAlpha, WORD alphaPitch, WORD width, WORD height);
	
	//---------------------------------------------------------
	// Blt functions
	//---------------------------------------------------------
	void		Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal);

	//---------------------------------------------------------
	// 정상적인 Blt
	//---------------------------------------------------------
	void		Blt(WORD *pDest, WORD pitch, MPalette &pal);				
	void		BltClip(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	
	//---------------------------------------------------------
	// Blt 4444  for Texture
	//---------------------------------------------------------
	void		Blt4444(WORD *pDest, WORD pitch, MPalette &pal);				
	void		Blt4444ClipLeft(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444ClipRight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444ClipWidth(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	
	//---------------------------------------------------------
	// Blt 4444 NotTrans for Texture (투명부분도 검게 칠한다)
	//---------------------------------------------------------
	void		Blt4444NotTrans(WORD *pDest, WORD pitch, MPalette &pal);				
	void		Blt4444NotTransClipLeft(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444NotTransClipRight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444NotTransClipWidth(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	
	//---------------------------------------------------------
	// shift만큼 shift해서 크기를 작게 출력시킨다.
	//---------------------------------------------------------
	void		Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift, MPalette &pal);
	
	//---------------------------------------------------------
	// Alpha Blending
	//---------------------------------------------------------
	void		BltAlpha(WORD *pDest, WORD pitch, BYTE alpha, MPalette &pal);
	void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal);
	void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal);
	void		BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal);
	void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal);
	
	//---------------------------------------------------------
	// Utility Functions
	//---------------------------------------------------------
	void		memcpyAlpha4444(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	void		memcpyAlpha4444Small(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	void		memcpyAlphaValue(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	void		memcpyAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	
	bool		IsColorPixel(short x, short y );
	WORD		GetPixel( short x, short y , MPalette &pal);
	
protected:
	// Blt Value
	static int		s_Value1;
	static int		s_Value2;
};

typedef CTypePack<CAlphaSpritePal>	CAlphaSpritePalPack;

#endif