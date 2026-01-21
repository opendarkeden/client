//-----------------------------------------------------------------------------
// Globals.cpp
//-----------------------------------------------------------------------------
// Global variables and utility functions needed by the game
// These are extracted from GamePacketFunctions.cpp to avoid symbol duplication
//-----------------------------------------------------------------------------

#include "Client_PCH.h"
#include "VS_UI/src/hangul/Fl2.h"  // For PrintInfo definition

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Wave pack file manager
class CWavePackFileManager;
CWavePackFileManager* g_pWavePackFileManager = NULL;

// Info show globals
extern "C" {
	bool gbl_info_show = true;
	bool gbl_show_item = true;
}

//-----------------------------------------------------------------------------
// Print Functions (Windows GDI-based, stubbed for macOS)
//-----------------------------------------------------------------------------

// g_Print - Defined in PacketFunction.cpp
// g_PrintColorStr - Print colored string (Windows GDI)
int g_PrintColorStr(int x, int y, const char* str, PrintInfo& info, DWORD color)
{
	(void)x; (void)y; (void)str; (void)info; (void)color;
	// Stub implementation - Windows GDI not available on macOS
	return 0;
}

// g_PrintColorStrOut - Print colored string with outline
int g_PrintColorStrOut(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
	// Stub implementation - Windows GDI not available on macOS
	return 0;
}

// g_PrintColorStrShadow - Print colored string with shadow
int g_PrintColorStrShadow(int x, int y, const char* str, PrintInfo& info, DWORD color1, DWORD color2)
{
	(void)x; (void)y; (void)str; (void)info; (void)color1; (void)color2;
	// Stub implementation - Windows GDI not available on macOS
	return 0;
}

//-----------------------------------------------------------------------------
// Platform-Specific Functions (Windows-specific)
//-----------------------------------------------------------------------------

// CheckMacScreenMode - Check Mac screen mode (Windows-specific stub)
void CheckMacScreenMode()
{
	// Stub implementation - Windows-specific function
}

// GetNMClipData - Get Netmarble clipboard data (Windows-specific)
bool GetNMClipData(char* pBuffer, unsigned int bufferSize, const char* pURL, bool bUseHTML)
{
	(void)pBuffer; (void)bufferSize; (void)pURL; (void)bUseHTML;
	// Stub implementation - Windows-specific function
	return false;
}

// SendUserIDToGameMonA - Send user ID to GameMon anti-cheat (Windows-specific)
void SendUserIDToGameMonA(const char* pUserID)
{
	(void)pUserID;
	// Stub implementation - Windows GameMon anti-cheat not available on macOS
}
