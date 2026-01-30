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
// Print Functions
//-----------------------------------------------------------------------------
// Implemented in RenderingFunctions.cpp for SDL builds.

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
