/*----------------------------------------------------------------------------

	MissingGlobals.cpp

	Global variable definitions that were in GlobalVariables.cpp
	but are not in Client.cpp (which is compiled into VS_UI library).

	This file contains ONLY symbols that are NOT already defined in Client.cpp.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"

// Forward declarations for types
#include "ClientDef.h"

// Forward declare types that may not exist on client
class RequestClientPlayerManager;
class RequestServerPlayerManager;

#include "Packet/PacketFactoryManager.h"
#include "Packet/PacketValidator.h"
#include "PacketFunction.h"
#include "Packet/Properties.h"
#include "ProfileManager.h"
#include "WavePackFileManager.h"
#include "CSoundPartManager.h"
#include "MZone.h"
#include "CMessageArray.h"
// #include "IVolume.h"  // VolumeLib not available in SDL build
#include "MScreenEffectManager.h"
#include "SpriteLib/CSpriteSurface.h"
#include "SpriteLib/CStorageSurface.h"
#include "CDirectSoundStream.h"
#include "CMP3.h"
#include "CAvi.h"
#include "ClientCommunicationManager.h"
#include "MWorkThread.h"
#include "../VS_UI/src/widget/u_window.h"
#include "../VS_UI/src/hangul/Ci.h"
#include "../basic/timer2.h"

#include "SkillDef.h"  // For OUSTERS_COAT_MAX

//-----------------------------------------------------------------------------
// Message Arrays (not in Client.cpp)
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
// Network & Socket (not in Client.cpp)
//-----------------------------------------------------------------------------
ClientPlayer*						g_pSocket = NULL;

//-----------------------------------------------------------------------------
// Sound (not in Client.cpp)
//-----------------------------------------------------------------------------
CSoundPartManager*		g_pSoundManager = NULL;
WavePackFileManager*	g_pWavePackFileManager = NULL;

//-----------------------------------------------------------------------------
// Zones (not in Client.cpp)
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
// Graphics (not in Client.cpp)
//-----------------------------------------------------------------------------
CSpriteSurface*		g_pBack = NULL;
CSpriteSurface*		g_pLast = NULL;
CStorageSurface*	g_pCursorSurface = NULL;
int					g_ScreenShotNumber = 0;

//-----------------------------------------------------------------------------
// Input State (not in Client.cpp)
//-----------------------------------------------------------------------------
POINT				g_SelectSector = { 0, 0 };
BOOL				g_bLButtonDown = FALSE;
BOOL				g_bRButtonDown = FALSE;
BOOL				g_bCButtonDown = FALSE;
BOOL				g_bUIInput = FALSE;

//-----------------------------------------------------------------------------
// Game State (not in Client.cpp)
//-----------------------------------------------------------------------------
int					g_UpdateDelay = 0;

//-----------------------------------------------------------------------------
// Managers (not in Client.cpp)
//-----------------------------------------------------------------------------
MScreenEffectManager*	g_pInventoryEffectManager = NULL;

//-----------------------------------------------------------------------------
// Rendering flags (not in Client.cpp)
//-----------------------------------------------------------------------------
bool				g_bPutMessage = true;
bool				g_bNewDraw = false;
bool				g_bSmoothCursor = false;
bool				g_bNetStatusGood = true;

//-----------------------------------------------------------------------------
// Other globals (not in Client.cpp)
//-----------------------------------------------------------------------------
CDirectSoundStream*	g_pSDLStream = NULL;
MWorkThread*		g_pLoadingThread = NULL;
bool				g_bFullScreen = false;
int					g_MaxNPC = 100;
bool				g_bHAL = false;
bool				g_bMusicSW = true;

//-----------------------------------------------------------------------------
// Additional missing globals (not in Client.cpp)
//-----------------------------------------------------------------------------
bool gbl_info_show = true;
bool gbl_show_item = true;

//-----------------------------------------------------------------------------
// Packet system globals (not in Client.cpp)
//-----------------------------------------------------------------------------
// g_pPacketItemOustersCoat is defined in PacketFunction.cpp

//-----------------------------------------------------------------------------
// MP3 Player (not in Client.cpp)
//-----------------------------------------------------------------------------
CMP3*					g_pMP3 = NULL;

//-----------------------------------------------------------------------------
// Video & Music (not in Client.cpp)
//-----------------------------------------------------------------------------
CAVI*					g_pAvi = NULL;

//-----------------------------------------------------------------------------
// Game Mode (not in Client.cpp)
//-----------------------------------------------------------------------------
enum CLIENT_MODE			g_Mode = MODE_OPENING;
enum CLIENT_MODE			g_ModeNext = MODE_OPENING;

//-----------------------------------------------------------------------------
// Zone Settings (not in Client.cpp)
//-----------------------------------------------------------------------------
int						g_ZoneCreatureColorSet = 0;
WORD					g_MyBatColorSet = 0xFFFF;
bool					g_bHolyLand = false;
bool					g_bZoneSafe = false;

//-----------------------------------------------------------------------------
// Watch Mode (not in Client.cpp)
//-----------------------------------------------------------------------------
bool					g_bWatchMode = false;

//-----------------------------------------------------------------------------
// Mouse Position (not in Client.cpp)
//-----------------------------------------------------------------------------
int						g_mouse_x = 0;
int						g_mouse_y = 0;

//-----------------------------------------------------------------------------
// Sound Settings (not in Client.cpp)
//-----------------------------------------------------------------------------
int						g_SoundPerSecond = 10;
