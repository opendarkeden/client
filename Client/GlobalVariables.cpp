/*----------------------------------------------------------------------------

	GlobalVariables.cpp

	Global variable definitions for Dark Eden client.
	Extracted from Client.cpp for cross-platform SDL builds.

	2025.01.16

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"

// Forward declarations - include headers for type definitions
#include "ClientDef.h"
#include "Packet/RequestClientPlayerManager.h"
#include "Packet/RequestServerPlayerManager.h"
#include "Packet/PacketFactoryManager.h"
#include "Packet/PacketValidator.h"
#include "PacketFunction.h"
#include "Packet/Properties.h"
#include "ServerInfo.h"
#include "ProfileManager.h"
#include "WavePackFileManager.h"
#include "CSoundPartManager.h"
#include "MZone.h"
#include "CMessageArray.h"
#include "IVolume.h"
#include "MScreenEffectManager.h"
#include "SpriteLib/CSpriteSurface.h"
#include "SpriteLib/CStorageSurface.h"
#include "CDirectSoundStream.h"
#include "CMP3.h"
#include "CAvi.h"
#include "ClientCommunicationManager.h"
#include "MWorkThread.h"
#include "../VS_UI/src/widget/u_window.h"  // For WindowManager
#include "../VS_UI/src/hangul/Ci.h"
#include "../basic/timer2.h"

// Only include what's needed
#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include <sys/time.h>
#endif

//-----------------------------------------------------------------------------
// Platform-specific globals
//-----------------------------------------------------------------------------

#ifdef PLATFORM_WINDOWS
	HWND				g_hWnd = NULL;
	HINSTANCE			g_hInstance = NULL;
#else
	void*				g_hWnd = NULL;
	void*				g_hInstance = NULL;
#endif

int					g_x = 400;
int					g_y = 300;

// Windows time function stub for macOS
#ifdef PLATFORM_MACOS
DWORD timeGetTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (DWORD)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
#endif

//-----------------------------------------------------------------------------
// Message Arrays
//-----------------------------------------------------------------------------
CMessageArray*		g_pSystemMessage = NULL;
CMessageArray*		g_pPlayerMessage = NULL;
CMessageArray*		g_pGameMessage = NULL;
CMessageArray*		g_pHelpMessage = NULL;
CMessageArray*		g_pNoticeMessage = NULL;

#ifdef	OUTPUT_DEBUG
CMessageArray*		g_pDebugMessage = NULL;
#endif

//-----------------------------------------------------------------------------
// Network & Socket
//-----------------------------------------------------------------------------
ClientPlayer*						g_pSocket = NULL;
RequestClientPlayerManager*		g_pRequestClientPlayerManager = NULL;
RequestServerPlayerManager*		g_pRequestServerPlayerManager = NULL;

//-----------------------------------------------------------------------------
// Sound
//-----------------------------------------------------------------------------
IVolume*				g_pMasterVolume = NULL;
IVolume*				g_pWaveVolume = NULL;
CSoundPartManager*		g_pSoundManager = NULL;
WavePackFileManager*	g_pWavePackFileManager = NULL;
DWORD					g_MasterVolumeOriginal = 100;
DWORD					g_WaveVolumeOriginal = 100;

//-----------------------------------------------------------------------------
// Zones
//-----------------------------------------------------------------------------
int					g_nZoneLarge = 0;
int					g_nZoneSmall = 0;
bool				g_bZoneLargeLoadImage = false;
bool				g_bZoneSmallLoadImage = false;
MZone*				g_pZoneLarge = NULL;
MZone*				g_pZoneSmall = NULL;
bool				g_bZonePlayerInLarge = false;
DWORD				g_ZoneRandomSoundTime = 0;

//-----------------------------------------------------------------------------
// Managers
//-----------------------------------------------------------------------------
ProfileManager*			g_pProfileManager = NULL;
MScreenEffectManager*	g_pInventoryEffectManager = NULL;

//-----------------------------------------------------------------------------
// Graphics
//-----------------------------------------------------------------------------
CSpriteSurface*		g_pBack = NULL;
CSpriteSurface*		g_pLast = NULL;
CStorageSurface*	g_pCursorSurface = NULL;
int					g_ScreenShotNumber = 0;

//-----------------------------------------------------------------------------
// Input State
//-----------------------------------------------------------------------------
POINT				g_SelectSector = { 0, 0 };
BOOL				g_bLButtonDown = FALSE;
BOOL				g_bRButtonDown = FALSE;
BOOL				g_bCButtonDown = FALSE;
BOOL				g_bUIInput = FALSE;

//-----------------------------------------------------------------------------
// Game State
//-----------------------------------------------------------------------------
BOOL				g_bActiveApp = TRUE;
BOOL				g_bActiveGame = FALSE;
BOOL				g_bNeedUpdate = FALSE;
int					g_UpdateDelay = 0;

//-----------------------------------------------------------------------------
// FPS
//-----------------------------------------------------------------------------
DWORD				g_CurrentTime = 0;
DWORD				g_CurrentFrame = 0;
int					g_FrameCount = 0;
int					g_StartFrameCount = 0;
DWORD				g_StartTime = 0;
DWORD				g_EndTime = 0;
int					g_FrameRate = 0;

//-----------------------------------------------------------------------------
// Rendering flags
//-----------------------------------------------------------------------------
bool				g_bPutMessage = true;
bool				g_bNewDraw = false;
bool				g_bSmoothCursor = false;
bool				g_bNetStatusGood = true;

//-----------------------------------------------------------------------------
// Other globals
//-----------------------------------------------------------------------------
CDirectSoundStream*	g_pDXSoundStream = NULL;
MWorkThread*		g_pLoadingThread = NULL;
bool				g_bFullScreen = false;
int					g_MaxNPC = 100;
bool				g_bHAL = false;
bool				g_bMusicSW = true;

//-----------------------------------------------------------------------------
// Additional missing globals (from basic/)
//-----------------------------------------------------------------------------
bool gbl_info_show = true;
bool gbl_show_item = true;

//-----------------------------------------------------------------------------
// Packet system globals
//-----------------------------------------------------------------------------
PacketFactoryManager*	g_pPacketFactoryManager = NULL;
PacketValidator*			g_pPacketValidator = NULL;
MItem*					g_pPacketItemOustersCoat[OUSTERS_COAT_MAX] = { NULL };
WindowManager*			gpC_window_manager = NULL;

//-----------------------------------------------------------------------------
// Configuration & Files
//-----------------------------------------------------------------------------
Properties*				g_pConfigKorean = NULL;
Properties*				g_pFileDef = NULL;

//-----------------------------------------------------------------------------
// MP3 Player
//-----------------------------------------------------------------------------
CMP3*					g_pMP3 = NULL;

//-----------------------------------------------------------------------------
// Video & Music
//-----------------------------------------------------------------------------
CAVI*					g_pAvi = NULL;
bool					g_DXSound = false;
bool					g_Music = true;

//-----------------------------------------------------------------------------
// Game Mode
//-----------------------------------------------------------------------------
enum CLIENT_MODE			g_Mode = MODE_OPENING;
enum CLIENT_MODE			g_ModeNext = MODE_OPENING;

//-----------------------------------------------------------------------------
// Zone Settings
//-----------------------------------------------------------------------------
int						g_ZoneCreatureColorSet = 0;
WORD					g_MyBatColorSet = 0xFFFF;
bool					g_bHolyLand = false;
bool					g_bZoneSafe = false;

//-----------------------------------------------------------------------------
// Watch Mode
//-----------------------------------------------------------------------------
bool					g_bWatchMode = false;

//-----------------------------------------------------------------------------
// Mouse Position (for VS_UI)
//-----------------------------------------------------------------------------
int						g_mouse_x = 0;
int						g_mouse_y = 0;

//-----------------------------------------------------------------------------
// Creature Morph Type
//-----------------------------------------------------------------------------
int						g_MorphCreatureType = 0;

//-----------------------------------------------------------------------------
// Sound Settings
//-----------------------------------------------------------------------------
int						g_SoundPerSecond = 10;

//-----------------------------------------------------------------------------
// Client Communication Manager
//-----------------------------------------------------------------------------
class ClientCommunicationManager*	g_pClientCommunicationManager = NULL;

//-----------------------------------------------------------------------------
// VS_UI & Basic library globals
//-----------------------------------------------------------------------------
CI*				gC_ci = NULL;
C_TIMER2		gC_timer2;
