//-----------------------------------------------------------------------------
// ClientFunction.h
//-----------------------------------------------------------------------------

#ifndef __CLIENTFUNCTION_H__
#define __CLIENTFUNCTION_H__

#include <Windows.h>
#include "MTypeDef.h"

extern const char*	GetWhisperID();

extern void			PlaySound(TYPE_SOUNDID soundID);

extern void			DrawInventoryEffect();

extern void			AddNewInventoryEffect(TYPE_OBJECTID id, TYPE_ACTIONINFO ai, DWORD delayFrame, DWORD value = 0);

extern void			DrawTitleEffect();

extern void			DrawBloodBibleEffect_InGear(int X, int Y);

//---------------------------------------------------------------------------
// Draw AlphaBox
//---------------------------------------------------------------------------
// (r,g,b)는 각각 0~31, alpha도 아마 0~31 -_-;
//---------------------------------------------------------------------------
extern void		DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha);

extern BOOL		IsPlayerInSafePosition();

extern bool		IsPlayerInSafeZone();

//class CSpriteSurface;
//extern void		InitMinimap(CSpriteSurface *minimap_surface);
//extern void		DrawMinimapAlpha(int x, int y, CSpriteSurface *minimap_surface);

#endif
