/*----------------------------------------------------------------------------

	DemoGlobals.cpp

	Minimal global variable definitions for the web demo.
	Extracted from Client.cpp and GlobalVariables.cpp.
	Only includes globals needed for map rendering.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"
#include "DXLib/CDirectDraw.h"

//-----------------------------------------------------------------------------
// ColorDraw namespace - provides compatibility with old code
// These symbols are accessed as ColorDraw::s_bSHIFT_B, etc.
//-----------------------------------------------------------------------------
namespace ColorDraw {
	// Define symbols that old code expects
	// These mirror the CSDLGraphics static members
	BYTE s_bSHIFT_B = 11;
	BYTE s_bSHIFT_G = 5;
	BYTE s_bSHIFT_R = 0;
	BYTE s_bSHIFT4_B = 4;
	BYTE s_bSHIFT4_G = 8;
	BYTE s_bSHIFT4_R = 12;

	// Array symbols
	WORD s_wMASK_SHIFT[5] = {0, 0, 0, 0, 0};
	DWORD s_dwMASK_SHIFT[5] = {0, 0, 0, 0, 0};
	uint64_t s_qwMASK_SHIFT[5] = {0, 0, 0, 0, 0};
	WORD s_wMASK_RGB[6] = {0, 0, 0, 0, 0, 0};
}

//-----------------------------------------------------------------------------
// Sector dimensions (from Client.cpp)
//-----------------------------------------------------------------------------
extern int g_SECTOR_WIDTH;
extern int g_SECTOR_HEIGHT;
extern int g_SECTOR_WIDTH_HALF;
extern int g_SECTOR_HEIGHT_HALF;
extern int g_SECTOR_SKIP_PLAYER_LEFT;
extern int g_SECTOR_SKIP_PLAYER_UP;

int g_SECTOR_WIDTH           = 16;
int g_SECTOR_HEIGHT          = 25;
int g_SECTOR_WIDTH_HALF      = 8;
int g_SECTOR_HEIGHT_HALF     = 12;
int g_SECTOR_SKIP_PLAYER_LEFT = 0;
int g_SECTOR_SKIP_PLAYER_UP   = 0;

//-----------------------------------------------------------------------------
// Zone state (from GlobalVariables.cpp/MissingGlobals.cpp)
//-----------------------------------------------------------------------------
extern int g_nZoneLarge;
extern int g_nZoneSmall;
extern bool g_bZoneLargeLoadImage;
extern bool g_bZoneSmallLoadImage;
extern bool g_bZonePlayerInLarge;

int g_nZoneLarge = 0;
int g_nZoneSmall = 0;
bool g_bZoneLargeLoadImage = false;
bool g_bZoneSmallLoadImage = false;
bool g_bZonePlayerInLarge = false;
unsigned long g_ZoneRandomSoundTime = 0;

//-----------------------------------------------------------------------------
// Global pointers needed by MZone
//-----------------------------------------------------------------------------
class MTopView;
class CUserOption;
class CZoneTable;
class Player;
class MParty;

MTopView* g_pTopView = nullptr;
CUserOption* g_pUserOption = nullptr;
CZoneTable* g_pZoneTable = nullptr;
Player* g_pPlayer = nullptr;
MParty* g_pParty = nullptr;

//-----------------------------------------------------------------------------
// Effect and file tables
//-----------------------------------------------------------------------------
class EffectStatusTable;
class CFileDef;

EffectStatusTable* g_pEffectStatusTable = nullptr;
CFileDef* g_pFileDef = nullptr;

//-----------------------------------------------------------------------------
// Time-related globals (from Client.h)
//-----------------------------------------------------------------------------
extern DWORD g_CurrentTime;
extern DWORD g_CurrentFrame;

DWORD g_CurrentTime = 0;
DWORD g_CurrentFrame = 0;

//-----------------------------------------------------------------------------
// Logging function (stub for demo)
//-----------------------------------------------------------------------------
extern "C" void log_write(const char* format, ...) {
	// No-op for demo - could implement to console if needed
}

//-----------------------------------------------------------------------------
// Creature and Effect tables (from GlobalVariables.cpp)
//-----------------------------------------------------------------------------
class EffectSpriteTypeTable;
class CreatureTable;

EffectSpriteTypeTable* g_pEffectSpriteTypeTable = nullptr;
CreatureTable* g_pCreatureTable = nullptr;
class EffectGeneratorTable;
EffectGeneratorTable* g_pEffectGeneratorTable = nullptr;

//-----------------------------------------------------------------------------
// Demo toggles
//-----------------------------------------------------------------------------
// Toggle loading ImageObjects from map data in the web demo.
// When false, ImageObjects will be loaded (with bounds checks to avoid WASM traps).
bool g_demoSkipImageObjects = false;
