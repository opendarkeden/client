#include <fstream>
#ifndef __CSPRITE_PAL__
#define __CSPRITE_PAL__

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
#else
	#include "../basic/Platform.h"
#endif

#include "CTypePack.h"
#include "CSpritePalBase.h"

class CSpritePal : public CSpritePalBase
{
public:
	void SetPixel(BYTE *pSource, WORD pitch, WORD width, WORD height);

	//---------------------------------------------------------
	// Blt functions
	//---------------------------------------------------------
	void Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal);
	
	//---------------------------------------------------------
	// Effect
	//---------------------------------------------------------
	void		BltEffect(WORD *pDest, WORD pitch, MPalette &pal);				
	void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltEffectClipWidth(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal);
	
	//---------------------------------------------------------		
	// 1555 
	//---------------------------------------------------------		
	void		Blt1555SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift, MPalette &pal);
	void		Blt1555NotTrans(WORD *pDest, WORD pitch, MPalette &pal);

	//---------------------------------------------------------
	// Utility Functions
	//---------------------------------------------------------
	static void		memcpy1555(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	static void		memcpy1555Small(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal);
	
	bool	IsColorPixel( short x, short y );
	WORD	GetPixel( short x, short y, MPalette &pal );
	
protected:
	// Blt Value
	static DWORD	s_Value1;
	static DWORD	s_Value2;
	static DWORD	s_Value3;

};

typedef CTypePack<CSpritePal>			CSpritePalPack;


#endif