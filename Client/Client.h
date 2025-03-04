//-----------------------------------------------------------------------------
// Client.h
//-----------------------------------------------------------------------------

#ifndef	__CLIENT_H__
#define	__CLIENT_H__

#pragma warning(disable:4786)
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <Windows.h>
#include "DebugInfo.h"

#include "DX3D.h"

#include "ClientDef.h"

//#include "MInput.h"
//#include "MMusic.h"
#include "CAVI.h"
//#include "MySound.h"

//#include "CInputManager.h"

#include "DXLib.h"
#include "SP.h"
#include "FR.h"
#include "VolumeLib.h"
#include "WinLib.h"


#include "MZone.h"
#include "MTopView.h"
#include "MPlayer.h"

#include "CSoundPartManager.h"

#include "MScreenEffectManager.h"

#include "MWorkThread.h"
#include "MLoadingSPKWorkNode.h"
#include "MItem.h"

class CMessageArray;
class Player;


//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
#ifdef __YHDK2__
#define PROGRAM_NAME                "YHDK2"
#define PROGRAM_TITLE               "YHDK2"
#else
#define PROGRAM_NAME                "TTDK2"
#define PROGRAM_TITLE               "TTDK2"
#endif

#define	PROGRAM_FILENAME			"fengshen.exe"
#define	UPDATER_FILENAME			"Updater.exe"
#define	UPDATER_NEW_FILENAME		"Updater2.exe"

#define	MAX_NPC						100
#define	MAX_IMAGEOBJECT				0
#define	MAX_ANIMATIONOBJECT			0
#define	MAX_INTERACTIONOBJECT		0
#define	MAX_ITEM					0


// DebugMessage
#define	MAX_DEBUGMESSAGE	25
#define	MAX_SYSTEMMESSAGE	5
#define MAX_PLAYERMESSAGE	5
#define	MAX_GAMEMESSAGE		5
#define	MAX_HELPMESSAGE		5


//------------------------------------------------------------
// Update delay 
//------------------------------------------------------------
// 20 --> 50 Frame
// 33 --> 30 Frame
// 50 --> 20 Frame
// 62 --> 16 Frame
// 66 --> 15 Frame
// 80 --> 12 Frame
// 100 --> 10 Frame
// 111 --> 9 Frame
#define	LIMIT_FPS_FOR_CURSOR	24		// 부드러운(?) 커서 출력을 위한 최소FPS
#define	DELAY_UPDATE_LOGIN		20
#define	DELAY_UPDATE_WAITING	50
#define	DELAY_UPDATE_GAME		62//62  렝岺속醵홍애긴좆

#define SAFE_DELETE(x)		{ if(x!=NULL) delete x; x=NULL; }
#define SAFE_DELETE_ARRAY(x){ if(x!=NULL) delete []x; x=NULL;}


const LONG SOUND_MIN	= DSBVOLUME_MIN / 4;
const LONG SOUND_RANGE	= DSBVOLUME_MAX - SOUND_MIN;
const LONG SOUND_DEGREE = SOUND_RANGE / 15;

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
extern HWND				g_hWnd;
extern HINSTANCE		g_hInstance;

// mouse cursor
extern int					g_x;
extern int					g_y;

// mp3
//CMySound*					g_pMP3 = NULL;

extern IVolume				*g_pMasterVolume; 
extern IVolume				*g_pWaveVolume; 
extern DWORD				g_MasterVolumeOriginal;
extern DWORD				g_WaveVolumeOriginal;

// Avi
extern CAVI					*g_pAvi;

// Sound PartManager
extern CSoundPartManager*	g_pSoundManager;

// Cursor 관리
extern CStorageSurface*		g_pCursorSurface;

extern POINT				g_SelectSector;
extern BOOL					g_bLButtonDown;
extern BOOL					g_bRButtonDown;
extern BOOL					g_bCButtonDown;
extern BOOL					g_bUIInput;

extern int					g_UpdateDelay;

// Zone
extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZoneLargeLoadImage;
extern bool					g_bZoneSmallLoadImage;
extern MZone*				g_pZoneLarge;
extern MZone*				g_pZoneSmall;
extern bool					g_bZonePlayerInLarge;
extern DWORD				g_ZoneRandomSoundTime;

// EffectManager
extern MScreenEffectManager*	g_pInventoryEffectManager;

// DirectDraw
extern CSpriteSurface*		g_pBack;
extern CSpriteSurface*		g_pLast;
extern int					g_ScreenShotNumber;

// Execute Program --> bActiveApp
// minimize | anotherWnd click--> !ActiveGame
extern BOOL					g_bActiveApp; // Is application active?
extern BOOL					g_bActiveGame; // Is Game Active?
extern BOOL					g_bNeedUpdate; // update해야되나?


// FPS
extern DWORD				g_CurrentTime;
extern DWORD				g_CurrentFrame;

extern int					g_FrameCount;
extern int					g_StartFrameCount;
extern DWORD				g_StartTime;
extern DWORD				g_EndTime;
extern int					g_FrameRate;

// Chat string
#ifdef	OUTPUT_DEBUG
	extern CMessageArray*		g_pDebugMessage;
#endif

extern bool					g_bPutMessage;		// 화면에 debug메세지를 출력할까?
	
extern bool					g_bNewDraw;			// 화면을 다시 그렸는가? (커서 제외)
extern bool					g_bSmoothCursor;	// 부드러운(?) cursor를 출력할 수 있나?
extern bool					g_bNetStatusGood;	// 네트웍 상태가 좋은가?

extern CMessageArray*		g_pSystemMessage;

extern CMessageArray*		g_pPlayerMessage;  //鯤소斤口잚

extern CMessageArray*		g_pGameMessage;
extern CMessageArray*		g_pHelpMessage;
extern CMessageArray*		g_pNoticeMessage;

extern CDirectSoundStream*	g_pDXSoundStream;

// Thread
extern MWorkThread*			g_pLoadingThread;
//extern HANDLE				g_hFileThread;
//extern HANDLE				g_hFileEvent;

//enum THREADJOB
//{			
//	THREADJOB_NONE,
//	THREADJOB_LOAD_IMAGEOBJECT_LARGEZONE,
//	THREADJOB_LOAD_IMAGEOBJECT_SMALLZONE,
//	THREADJOB_LOAD_CREATURE	
//};
	

// Server IP
//extern char				g_ServerIP[80];
extern bool				g_bFullScreen;
extern int				g_MaxNPC;
extern bool				g_bHAL;
extern bool				g_bMusicSW;

extern CWinUpdate*				g_pUpdate;



//-----------------------------------------------------------------------------
// Global Function Prototypes
//-----------------------------------------------------------------------------

HRESULT		InitFail(LPCTSTR szError,...);
//bool		CheckDXVersion();
void		ReleaseAllObjects();
void		ReleaseSocket();
void		ReleaseGameUpdate();
void		ReleaseGameObject();
void		UnInitSound();
void		StopLoadingThread();

BOOL		InitGame();

// ALT + TAB 처리
void		CheckActivate(BOOL bActiveGame);

// Init DX..
BOOL		InitInput();
BOOL		InitSound();
BOOL		InitMusic();
BOOL		InitDraw();

// Init Other
BOOL		InitClientConfig();
BOOL		InitUserOption();
BOOL		InitVolume();
BOOL		InitSurface();
BOOL		InitTimer();
BOOL		InitThread();
BOOL		InitInfomation();
BOOL		InitGameObject();
BOOL		InitGameUpdate();
BOOL		InitPlayer(int x, int y, int dir);
BOOL		InitSocket();

// Utility Func
BOOL		LoadZone(int n);
BOOL		LoadZoneInfo(int n);
void		MoveZone(int n);
BOOL		LoadCreature(int spriteType);
BOOL		LoadCreatureType(int creatureType);
void		PlaySound(TYPE_SOUNDID soundID, bool repeat, int x, int y);
void		PlaySound(TYPE_SOUNDID soundID);
void		StopSound(TYPE_SOUNDID soundID);
void		PlayMusicCurrentZone();

void		SetMode(enum CLIENT_MODE mode);
bool		UpdateSocketInput();
bool		UpdateSocketOutput();
void		UpdateDisconnected();

void		UpdateInput();
void		UpdateMouse();

void		SelectLastSelectedCharacter();
void		SaveLastSelectedCharacter( int slot );

bool		GetMakeItemFitPosition(MItem* pItem, ITEM_CLASS itemClass, int itemType, POINT& fitPoint);

// thread
//LONG		FileThreadProc(LPVOID lpParameter);
//void		StopFileThread();

// Etc
void		MakeScreenShot();

// UI
void		UI_ResultReceiver(DWORD message, int left, int right, void *void_ptr);

// 날씨
void		SetWeather(int weather, int level);
void		SetLightning(DWORD delay);

// file열기
bool		FileOpenBinary(const char* filename, class ifstream& file);

// 2004, 3, 29 sobeit add start
void		Add_GDR_Ghost(int ZoneID);
// 2004, 3, 29 sobeit add end

// by csm 2004/12/14
int			GetPersnalShopUpdateTime();
// Pre-make table of advancement class Creature's frame. by sonee 2004.12.30
extern BYTE g_AdvanceVampireActionMaxCount[ ACTION_ADVANCEMENT_MAX - ACTION_ADVANCEMENT_STOP ];
extern BYTE g_AdvanceSlayerActionMaxCount[ ACTION_ADVANCEMENT_SLAYER_MAX - ACTION_ADVANCEMENT_SLAYER_STOP_SWORD ];
extern BYTE g_AdvanceOustersActionMaxCount[ ACTION_ADVANCEMENT_OUSTERS_MAX - ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP ];

void		PrecalculateAdvancementClassCreatureFrames();
//Add by sonic 2006.7.29
BOOL		GetSystem();
#endif
