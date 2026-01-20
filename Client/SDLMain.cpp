/*-----------------------------------------------------------------------------

	SDLMain.cpp

	SDL2-based main entry point for Dark Eden client on macOS/Linux.
	Based on WinMain from Client.cpp (lines 3020-4500).

	This file handles:
	- SDL initialization
	- Window creation
	- Event loop
	- Game loop integration
	- Clean shutdown

	2025.01.18 - Rewritten based on Client.cpp WinMain

-----------------------------------------------------------------------------*/

#ifndef PLATFORM_WINDOWS

#include "Client_PCH.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdio.h>

// Client.h - must be included to access g_pUpdate declaration
#include "Client.h"

// VS_UI headers
#include "../VS_UI/src/hangul/Ci.h"
#include "../VS_UI/src/header/VS_UI.h"

// Game headers
#include "UserInformation.h"
#include "SpriteLib/SpriteLibBackend.h"
#include "CGameUpdate.h"
#include "CSpriteSurface.h"
#include "ClientDef.h"
#include "MTopView.h"
#include "MPlayer.h"
#include "../basic/Platform.h"
#include "Packet/Exception.h"  // For NoSuchElementException, Throwable

// Language detection
enum DARKEDEN_LANGUAGE
{
	DARKEDEN_KOREAN = 0,
	DARKEDEN_CHINESE,
	DARKEDEN_JAPANESE,
	DARKEDEN_ENGLISH,
	DARKEDEN_TAIWAN,
	DARKEDEN_LANGUAGE_MAX
};
extern DARKEDEN_LANGUAGE CheckDarkEdenLanguage();

// Client mode and UI
extern enum CLIENT_MODE g_Mode;
extern C_VS_UI gC_vs_ui;

//-----------------------------------------------------------------------------
// SDL-specific globals
//-----------------------------------------------------------------------------
SDL_Window* g_pSDLWindow = NULL;
SDL_Renderer* g_pSDLRenderer = NULL;
bool g_bRunning = true;

//-----------------------------------------------------------------------------
// Stub implementations for functions not available on non-Windows platforms
//-----------------------------------------------------------------------------

// Stub for ExecuteActionInfoFromMainNode
void ExecuteActionInfoFromMainNode(
	unsigned short, unsigned short, unsigned short, int, int, unsigned int,
	unsigned short, unsigned short, int, unsigned long,
	struct MActionResult*, bool, int, int)
{
	// Stub - do nothing on SDL platform
}

// Stub for g_Print
void g_Print(int x, int y, const char* str, struct PrintInfo* pInfo)
{
	(void)x; (void)y;
	if (str && str[0]) {
		printf("g_Print: %s\n", str);
	}
}

// Stub for FillRect
void FillRect(S_SURFACEINFO* pInfo, struct Rect* pRect, int color)
{
	(void)pInfo; (void)pRect; (void)color;
	// Stub - do nothing on SDL platform
}

// Stub for rectangle
void rectangle(S_SURFACEINFO* pInfo, struct Rect* pRect, int color)
{
	(void)pInfo; (void)pRect; (void)color;
	// Stub - do nothing on SDL platform
}

/*-----------------------------------------------------------------------------
 *
 * SDL version of InitApp - Creates window and initializes game
 *
 *-----------------------------------------------------------------------------*/
static BOOL InitApp(int nCmdShow)
{
	int cx = 800, cy = 600;
	Uint32 flags = SDL_WINDOW_SHOWN;

	// Determine window size and flags based on command line
	extern bool g_bFullScreen;
	extern bool g_MyFull;

	if (g_bFullScreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
		flags |= SDL_WINDOW_BORDERLESS;

		// Get screen dimensions
		SDL_DisplayMode dm;
		if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
			cx = dm.w;
			cy = dm.h;
		}
	} else {
		flags |= SDL_WINDOW_RESIZABLE;

		// Check for 1024x768 mode
		extern RECT g_GameRect;
		if (g_MyFull) {
			cx = 1024;
			cy = 768;
		} else {
			cx = g_GameRect.right;
			cy = g_GameRect.bottom;
		}
	}

	// Create SDL window
	g_pSDLWindow = SDL_CreateWindow(
		"Dark Eden",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		cx, cy,
		flags
	);

	if (!g_pSDLWindow) {
		fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
		return FALSE;
	}

	// Create SDL renderer
	g_pSDLRenderer = SDL_CreateRenderer(
		g_pSDLWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!g_pSDLRenderer) {
		fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
		return FALSE;
	}

	// Hide cursor
	SDL_ShowCursor(0);

	// Initialize SpriteLib SDL backend
	if (spritectl_init() != 0) {
		fprintf(stderr, "Failed to initialize SpriteLib backend\n");
		return FALSE;
	}

	// Initialize game
	if (!InitGame()) {
		fprintf(stderr, "Failed to initialize game\n");
		return FALSE;
	}

	return TRUE;
}

/*-----------------------------------------------------------------------------
 *
 * Cleanup SDL resources
 *
 *-----------------------------------------------------------------------------*/
static void CleanupSDL()
{
	if (g_pSDLRenderer) {
		SDL_DestroyRenderer(g_pSDLRenderer);
		g_pSDLRenderer = NULL;
	}
	if (g_pSDLWindow) {
		SDL_DestroyWindow(g_pSDLWindow);
		g_pSDLWindow = NULL;
	}
	spritectl_shutdown();
}

/*-----------------------------------------------------------------------------
 *
 * Main entry point for SDL2 builds
 *
 *-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	// Immediate debug output (flushed)
	fprintf(stderr, "DEBUG: Dark Eden SDL2 starting...\n");
	fflush(stderr);

	// Parse command line
	const char* cmdLine = "0000000001";  // Default: window mode
	if (argc > 1 && argv[1] != NULL) {
		cmdLine = argv[1];
	}

	fprintf(stderr, "Command line: %s\n", cmdLine);
	fflush(stderr);

	printf("========================================\n");
	printf("Dark Eden Client (SDL2 Backend)\n");
	printf("Based on Client.cpp WinMain\n");
	printf("========================================\n");
	printf("Command line: %s\n", cmdLine);
	printf("\n");
	fflush(stdout);

	// Global variables from WinMain
	extern bool g_MyFull;
	extern bool g_bFullScreen;
	extern RECT g_GameRect;
	extern int g_x, g_y;
	extern int g_SECTOR_WIDTH, g_SECTOR_HEIGHT;
	extern int g_SECTOR_WIDTH_HALF, g_SECTOR_HEIGHT_HALF;
	extern int g_SECTOR_SKIP_PLAYER_LEFT, g_SECTOR_SKIP_PLAYER_UP;
	extern int g_TILESURFACE_SECTOR_WIDTH, g_TILESURFACE_SECTOR_HEIGHT;
	extern int g_TILESURFACE_SECTOR_OUTLINE_RIGHT, g_TILESURFACE_SECTOR_OUTLINE_DOWN;
	extern int g_TILESURFACE_WIDTH, g_TILESURFACE_HEIGHT;
	extern int g_TILESURFACE_OUTLINE_RIGHT, g_TILESURFACE_OUTLINE_DOWN;
	extern int g_TILE_X_HALF, g_TILE_Y_HALF;
	extern DWORD g_dwVideoMemory;
	extern MTopView* g_pTopView;

	bool cmpFullScreen = false;

	// Parse command line (from WinMain lines 3047-3115)
	size_t cmdLen = strlen(cmdLine);
	if (cmdLen > 0) {
		char lastChar = cmdLine[cmdLen - 1];

		if (lastChar == '1') {
			g_MyFull = false;
			cmpFullScreen = false;
		}
		else if (lastChar == '2') {
			g_MyFull = false;
			cmpFullScreen = true;
		}
		else if (lastChar == '3') {
			g_MyFull = true;
			cmpFullScreen = false;
			g_GameRect.left = 1023;
			g_GameRect.top = 767;
			g_GameRect.right = 1024;
			g_GameRect.bottom = 768;
			g_x = 512;
			g_y = 384;
			g_SECTOR_WIDTH = 21;
			g_SECTOR_HEIGHT = 32;
			g_SECTOR_WIDTH_HALF = 11;
			g_SECTOR_HEIGHT_HALF = 17;
			g_SECTOR_SKIP_PLAYER_LEFT = -10;
			g_SECTOR_SKIP_PLAYER_UP = -16;
			g_TILESURFACE_SECTOR_WIDTH = 27;
			g_TILESURFACE_SECTOR_HEIGHT = 37;
			g_TILESURFACE_SECTOR_OUTLINE_RIGHT = 24;
			g_TILESURFACE_SECTOR_OUTLINE_DOWN = 34;
			g_TILESURFACE_WIDTH = 1296;
			g_TILESURFACE_HEIGHT = 888;
			g_TILESURFACE_OUTLINE_RIGHT = 1152;
			g_TILESURFACE_OUTLINE_DOWN = 816;
			g_TILE_X_HALF = 24;
			g_TILE_Y_HALF = 12;
		}
		else if (lastChar == '4') {
			g_MyFull = true;
			cmpFullScreen = true;
			g_GameRect.left = 1023;
			g_GameRect.top = 767;
			g_GameRect.right = 1024;
			g_GameRect.bottom = 768;
			g_x = 512;
			g_y = 384;
			g_SECTOR_WIDTH = 21;
			g_SECTOR_HEIGHT = 32;
			g_SECTOR_WIDTH_HALF = 11;
			g_SECTOR_HEIGHT_HALF = 17;
			g_SECTOR_SKIP_PLAYER_LEFT = -10;
			g_SECTOR_SKIP_PLAYER_UP = -16;
			g_TILESURFACE_SECTOR_WIDTH = 27;
			g_TILESURFACE_SECTOR_HEIGHT = 37;
			g_TILESURFACE_SECTOR_OUTLINE_RIGHT = 24;
			g_TILESURFACE_SECTOR_OUTLINE_DOWN = 34;
			g_TILESURFACE_WIDTH = 1296;
			g_TILESURFACE_HEIGHT = 888;
			g_TILESURFACE_OUTLINE_RIGHT = 1152;
			g_TILESURFACE_OUTLINE_DOWN = 816;
			g_TILE_X_HALF = 24;
			g_TILE_Y_HALF = 12;
		}
	}

	// Set full screen flag
	g_bFullScreen = cmpFullScreen;

	//-----------------------------------------------------------------
	// Get current directory (from WinMain lines 3201-3213)
	//-----------------------------------------------------------------
	char g_CWD[_MAX_PATH];

	// On macOS/Linux, use getcwd() to get current directory
	// Note: When running from DarkEden directory, this should already be correct
	if (getcwd(g_CWD, _MAX_PATH) != NULL) {
		printf("Working directory: %s\n", g_CWD);
	} else {
		// Fallback to "." if getcwd fails
		g_CWD[0] = '.';
		g_CWD[1] = '\0';
		printf("Working directory: . (getcwd failed)\n");
	}

	//-----------------------------------------------------------------
	// Initialize SDL2
	//-----------------------------------------------------------------
	printf("Initializing SDL2...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
		fprintf(stderr, "ERROR: SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	// Initialize SDL_ttf for font rendering
	printf("Initializing SDL_ttf...\n");
	if (TTF_Init() < 0) {
		fprintf(stderr, "ERROR: TTF_Init failed: %s\n", TTF_GetError());
		return 1;
	}
	atexit(TTF_Quit);

	//-----------------------------------------------------------------
	// Initialize language and character input (IME)
	// This MUST be done before InitGame() - same order as WinMain
	//-----------------------------------------------------------------
	printf("Detecting language and initializing IME...\n");
	DARKEDEN_LANGUAGE Language = CheckDarkEdenLanguage();

	// Override to Chinese for macOS/Linux builds with Chinese resources
	Language = DARKEDEN_CHINESE;

	switch (Language)
	{
	case DARKEDEN_CHINESE:
		gC_ci = new CI_CHINESE;
		printf("Language: Chinese (Simplified)\n");
		break;

	case DARKEDEN_KOREAN:
		gC_ci = new CI_KOREAN;
		printf("Language: Korean\n");
		break;

	default:
		// Default to Chinese for non-Windows platforms
		gC_ci = new CI_CHINESE;
		printf("Language: Chinese (Simplified - default)\n");
		break;
	}

	//-----------------------------------------------------------------
	// Initialize App (create window, init game)
	//-----------------------------------------------------------------
	printf("Initializing application...\n");
	int nCmdShow = 1;  // Default: show window

	// Frame rate tracking variables (declared outside InitApp block for cleanup access)
	const int g_FrameGood = 15;  // Minimum acceptable FPS (from Client.cpp line 154)
	DWORD g_FrameCount = 0;  // Local frame counter

	try {
		if (InitApp(nCmdShow))
		{
			// Set default video memory for non-Windows platforms
			g_dwVideoMemory = 256 * 1024 * 1024;  // 256 MB default

		//-----------------------------------------------------------------
		// Set user language based on detected language
		//-----------------------------------------------------------------
		extern UserInformation* g_pUserInformation;
		if (g_pUserInformation != NULL)
		{
			if (gC_ci->IsKorean())
			{
				g_pUserInformation->SetKorean();
				printf("User language set to: Korean\n");
			}
			else if (gC_ci->IsChinese())
			{
				g_pUserInformation->SetChinese();
				printf("User language set to: Chinese\n");
			}
			else if (gC_ci->IsJapanese())
			{
				g_pUserInformation->SetJapanese();
				printf("User language set to: Japanese\n");
			}
		}

		//-----------------------------------------------------------------
		// Initialize TopView
		//-----------------------------------------------------------------
		printf("Initializing TopView...\n");
		if (!g_pTopView->IsInit())
		{
			printf("TopView not initialized, calling Init()...\n");
			try {
				g_pTopView->Init();
				printf("TopView Init() completed\n");
			} catch (NoSuchElementException& e) {
				fprintf(stderr, "ERROR: NoSuchElementException in TopView::Init(): %s\n", e.toString().c_str());
				// Continue anyway - some resources might be missing
			} catch (Throwable& t) {
				fprintf(stderr, "ERROR: Throwable in TopView::Init(): %s\n", t.toString().c_str());
				// Continue anyway
			}
		}
		printf("TopView initialization complete\n");

		g_bActiveApp = TRUE;

		// Ensure window has focus for mouse input
		SDL_RaiseWindow(g_pSDLWindow);

		printf("\n");
		printf("Starting game loop...\n");
		printf("Press Ctrl+C or close window to exit.\n");
		printf("\n");
		fflush(stdout);
		fprintf(stderr, "DEBUG: Entering game loop, g_bActiveApp = %d\n", g_bActiveApp);
		fflush(stderr);

		//-----------------------------------------------------------------
		// Main game loop (from WinMain lines 4244-4341)
		//-----------------------------------------------------------------
		// Note: All SDL events are now handled by dxlib_input_update() in DXLibBackendSDL.cpp
		// This prevents event queue conflicts and ensures mouse events are properly processed
		static int loopCount = 0;

		// Frame rate tracking variables
		g_StartTime = SDL_GetTicks();
		g_StartFrameCount = 0;
		g_FrameCount = 0;

		while (g_bRunning)
		{
			// Debug: Print loop status periodically
//			if (++loopCount % 60 == 1) {
//				fprintf(stderr, "DEBUG: Loop iteration %d, g_bActiveApp = %d, g_bRunning = %d\n",
//						loopCount, g_bActiveApp, g_bRunning);
//				fflush(stderr);
//			}

			// Game update when active
			if (g_bActiveApp)
			{
				g_CurrentTime = SDL_GetTicks();  // Replaces timeGetTime()

				// Game update (from WinMain lines 4275-4290)
				if (g_pUpdate != NULL)
				{
					g_pUpdate->Update();
				}
				else
				{
					static int nullCount = 0;
					if (++nullCount % 60 == 1) {
						printf("WARNING: g_pUpdate is NULL! Count = %d\n", nullCount);
					}
				}

				// Frame rate tracking (from WinMain lines 4320-4333)
				DWORD timeGap = g_CurrentTime - g_StartTime;

				if (timeGap > 1000)
				{
					g_FrameRate = (g_FrameCount - g_StartFrameCount) * 1000 / timeGap;
					g_bGoodFPS = (g_FrameRate >= g_FrameGood);

					g_StartTime = g_CurrentTime;
					g_StartFrameCount = g_FrameCount;
				}

				// Present back buffer to screen
				// CRITICAL: First copy g_pLast to g_pBack (UI renders to g_pLast)
				if (g_pLast != NULL && g_pBack != NULL) {
					POINT origin = {0, 0};
					g_pBack->Blt(&origin, g_pLast, NULL);  // Copy entire g_pLast to g_pBack
				}

				if (g_pBack != NULL) {
					spritectl_surface_t backend_surface = g_pBack->GetBackendSurface();
					if (backend_surface != SPRITECTL_INVALID_SURFACE) {
						spritectl_present_surface(backend_surface, g_pSDLRenderer);
					}
				}

				SDL_RenderPresent(g_pSDLRenderer);  // Replaces CDirectDraw::Flip()
				g_FrameCount++;
			}
			else
			{
				// Game inactive - sleep to reduce CPU usage
				SDL_Delay(10);  // Replaces WaitMessage()
			}
		}
	}
	else
	{
		fprintf(stderr, "ERROR: InitApp failed\n");
	}
	}
	catch (FileNotExistException& e) {
		fprintf(stderr, "ERROR: FileNotExistException: %s\n", e.toString().c_str());
		return 1;
	}
	catch (Throwable& t) {
		fprintf(stderr, "ERROR: Throwable: %s\n", t.toString().c_str());
		return 1;
	}
	catch (std::exception& e) {
		fprintf(stderr, "ERROR: std::exception: %s\n", e.what());
		return 1;
	}
	catch (...) {
		fprintf(stderr, "ERROR: Unknown exception\n");
		return 1;
	}

	//-----------------------------------------------------------------
	// Cleanup (from WinMain lines 4434-4446)
	//-----------------------------------------------------------------
	printf("\n");
	printf("Shutting down...\n");

	ReleaseAllObjects();

	// Clean up character input object (must be done after ReleaseAllObjects)
	delete gC_ci;
	gC_ci = NULL;

	// Clean up SDL resources
	CleanupSDL();

	printf("Game exited cleanly.\n");
	printf("Total frames rendered: %u\n", g_FrameCount);

	return 0;
}

#endif /* PLATFORM_WINDOWS */
