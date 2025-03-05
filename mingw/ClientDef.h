//-----------------------------------------------------------------------------
// ClientDef.h
//-----------------------------------------------------------------------------
// Client에 관련된 header file   - -;
//-----------------------------------------------------------------------------


#ifndef	__CLIENTDEF_H__
#define	__CLIENTDEF_H__

#pragma warning(disable:4786)

//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

class CSpriteSurface;
class MTopView;
class MItem;
//#include "MMusic.h"
#include "MPlayer.h"
//#include "Packet\Player.h"
class ClientPlayer;
#include "MZone.h"
#include "CMessageArray.h"
//#include "UIFunction.h"
#include "PacketFunction.h"
#include "MHelpManager.h"
#include "DebugInfo.h"
#include "COrderedList.h"

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
extern HWND							g_hWnd;

// DirectDraw
extern CSpriteSurface*			g_pBack;
extern CSpriteSurface*			g_pLast;

// Chat string
#define	MAX_RECEIVE	20
#ifdef	OUTPUT_DEBUG
	extern CMessageArray*			g_pDebugMessage;
#endif

extern CMessageArray*				g_pSystemMessage;

extern CMessageArray*				g_pPlayerMessage;  //鯤소斤口잚

extern CMessageArray*				g_pGameMessage;

extern CMessageArray*				g_pHelpMessage;

//-----------------------------------------------------------------------------
// client mode
//-----------------------------------------------------------------------------
enum	CLIENT_MODE
{
	MODE_NULL = 0,
	MODE_OPENING,
	MODE_MAINMENU,
	MODE_CHANGE_OPTION,	// 3d가속 <--> 2d
	MODE_NEWUSER,
	MODE_CREATECHARACTER,
	MODE_WAIT_CREATEPCOK,
	MODE_WAIT_DELETEPCOK,
	MODE_SELECTCHARACTER,
	MODE_WAIT_LOGINOK,
	MODE_WAIT_WORLD_LIST,
	MODE_WAIT_SELECT_WORLD,
	MODE_WAIT_SERVER_LIST,
	MODE_WAIT_SELECT_SERVER,
	MODE_LOGIN_WRONG,
	MODE_WAIT_PCLIST,
	MODE_WAIT_SELECTPC,
	MODE_WAIT_RECONNECT,
	MODE_WAIT_UPDATEINFO,
	MODE_WAIT_SETPOSITION,		
	MODE_WAIT_REGISTERPLAYEROK,
	MODE_WAIT_RECONNECT_LOGIN,	// logout하고 자동 재접속
	MODE_GAME,
	MODE_QUIT
};	

//-----------------------------------------------------------------------------
// update type
//-----------------------------------------------------------------------------
enum	CLIENT_UPDATE
{
	UPDATE_OPENING,
	UPDATE_WAITUI,
	UPDATE_WAITPACKET,
	UPDATE_GAME
};


//-----------------------------------------------------------------------------
// 현재의 Client Mode에 대한 설정..
//-----------------------------------------------------------------------------
extern enum CLIENT_MODE		g_Mode;
extern enum CLIENT_MODE		g_ModeNext;


//-----------------------------------------------------------------------------
//
//					Global Function Prototypes
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// init / mode
//-----------------------------------------------------------------------------
extern BOOL		InitPlayer(int x, int y, int dir);
extern void		SetMode(enum CLIENT_MODE mode);

//-----------------------------------------------------------------------------
// sound
//-----------------------------------------------------------------------------
extern void		PlaySound(TYPE_SOUNDID soundID, bool repeat, int x, int y);
extern void		PlaySound(TYPE_SOUNDID soundID);
extern void		StopSound(TYPE_SOUNDID soundID);

//-----------------------------------------------------------------------------
// weather
//-----------------------------------------------------------------------------
extern void		SetWeather(int weather, int level);
extern void		SetLightning(DWORD delay);

//-----------------------------------------------------------------------------
// load, zone
//-----------------------------------------------------------------------------
extern BOOL		LoadZone(int n);
extern BOOL		LoadZoneInfo(int n);
extern void		MoveZone(int n);
extern BOOL		LoadCreature(int spriteType);
extern BOOL		LoadCreatureType(int creatureType);
extern void		ReleaseUselessCreatureSPKExcept(const COrderedList<int>& listUse);

//-----------------------------------------------------------------------------
// Convert 
//-----------------------------------------------------------------------------
extern DWORD	ConvertDurationToFrame(int duration);
extern DWORD	ConvertMillisecondToFrame(DWORD ms);
extern DWORD	ConvertDurationToMillisecond(int duration);
extern POINT	ConvertPositionMapToScreen(int sX, int sY);


#endif