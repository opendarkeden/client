/*-----------------------------------------------------------------------------

	SDLMain.cpp

	SDL2-based main entry point for Dark Eden client on macOS/Linux.
	Replaces Windows WinMain with cross-platform SDL implementation.

	This file handles:
	- SDL initialization
	- Window creation
	- Event loop
	- Game loop integration
	- Clean shutdown

	2025.01.16

-----------------------------------------------------------------------------*/

#ifndef PLATFORM_WINDOWS

#include "Client_PCH.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <string.h>
#include "../VS_UI/src/hangul/Ci.h"
#include "UserInformation.h"
#include "SpriteLib/SpriteLibBackend.h"
#include "CGameUpdate.h"
#include "CSpriteSurface.h"
#include "ClientDef.h"  // For CLIENT_MODE enum definitions
#include "../VS_UI/src/header/VS_UI.h"  // For C_VS_UI class
#include "../basic/Platform.h"  // For basic types
#include "MTopView.h"  // For PrintInfo, etc.

/* Forward declarations for game functions (defined in GameInit.cpp, ClientFunction.cpp, etc.) */
extern bool InitGame();
extern void ReleaseAllObjects();

/* Global game update object */
extern CGameUpdate* g_pCGameUpdate;

/* Global back buffer surface */
extern CSpriteSurface* g_pBack;

/* Language detection and character input initialization */
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

//-----------------------------------------------------------------------------
// Cross-platform SetMode implementation (simplified)
//-----------------------------------------------------------------------------
// NOTE: SetMode() is implemented in GameMain.cpp - this stub commented out to avoid duplicate symbol
// g_Mode is defined in MissingGlobals.cpp
extern enum CLIENT_MODE g_Mode;
extern C_VS_UI gC_vs_ui;

/*
// SetMode() implementation moved to GameMain.cpp
void SetMode(enum CLIENT_MODE mode)
{
	printf("DEBUG: SetMode called with mode=%d\n", mode);

	g_Mode = mode;

	// Handle MODE_MAINMENU specifically
	if (g_Mode == MODE_MAINMENU)
	{
		printf("DEBUG: Processing MODE_MAINMENU\n");

		// Initialize sound (if needed)
		extern void InitSound();
		InitSound();

		// Start the title UI
		gC_vs_ui.StartTitle();

		printf("DEBUG: StartTitle() completed\n");
	}
}
*/

//-----------------------------------------------------------------------------
// Stub implementations for functions from GameMain.cpp that are not available
// on non-Windows platforms. These are needed to resolve linker errors.
//-----------------------------------------------------------------------------

// Stub for ExecuteActionInfoFromMainNode
void ExecuteActionInfoFromMainNode(
	unsigned short, unsigned short, unsigned short, int, int, unsigned int,
	unsigned short, unsigned short, int, unsigned long,
	struct MActionResult*, bool, int, int)
{
	// Stub - do nothing on SDL platform
}

// NOTE: ReleaseUselessCreatureSPKExcept() is implemented in GameMain.cpp
// Stub removed to avoid duplicate symbol
/*
void ReleaseUselessCreatureSPKExcept(class COrderedList<int> const&)
{
	// Stub - do nothing on SDL platform
}
*/

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

/* Game mode flags from command line */
enum GameMode {
	MODE_WINDOW = 1,
	MODE_FULLSCREEN = 2,
	MODE_WINDOW_1024 = 3,
	MODE_FULLSCREEN_1024 = 4
};

/* Global flags */
bool g_bRunning = true;
// g_bActiveApp is defined in Client.cpp (VS_UI library)
extern BOOL g_bActiveApp;

/*---------------------------------------------------------------------------
 *
 * Parse command line and extract game mode
 *
 *---------------------------------------------------------------------------*/
static GameMode ParseCommandLine(const char* cmdLine) {
	if (cmdLine == NULL || strlen(cmdLine) == 0) {
		return MODE_WINDOW;  // Default to window mode
	}

	char lastChar = cmdLine[strlen(cmdLine) - 1];
	switch (lastChar) {
		case '1': return MODE_WINDOW;
		case '2': return MODE_FULLSCREEN;
		case '3': return MODE_WINDOW_1024;
		case '4': return MODE_FULLSCREEN_1024;
		default:  return MODE_WINDOW;
	}
}

/*---------------------------------------------------------------------------
 *
 * Main entry point for SDL2 builds
 *
 *---------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	GameMode mode = MODE_WINDOW;
	const char* cmdLine = "0000000001";  // Default: window mode

	/* Parse command line */
	if (argc > 1 && argv[1] != NULL) {
		cmdLine = argv[1];
		mode = ParseCommandLine(cmdLine);
	}

	printf("========================================\n");
	printf("Dark Eden Client (SDL2 Backend)\n");
	printf("========================================\n");
	printf("Command line: %s\n", cmdLine);
	printf("Mode: %d\n", mode);
	printf("\n");

	/* Initialize SDL2 */
	printf("Initializing SDL2...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
		fprintf(stderr, "ERROR: SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	/* Set up clean shutdown via atexit */
	atexit(SDL_Quit);

	/* Initialize SpriteLib SDL backend */
	if (spritectl_init() != 0) {
		fprintf(stderr, "ERROR: SpriteLib backend initialization failed\n");
		return 1;
	}
	atexit(spritectl_shutdown);

	/* Create window and renderer based on mode */
	int windowWidth = 800;
	int windowHeight = 600;
	Uint32 windowFlags = SDL_WINDOW_SHOWN;

	const char* windowTitle = "Dark Eden";

	switch (mode) {
		case MODE_FULLSCREEN:
		case MODE_FULLSCREEN_1024:
			windowFlags |= SDL_WINDOW_FULLSCREEN;
			break;
		default:
			windowFlags |= SDL_WINDOW_RESIZABLE;
			break;
	}

	if (mode == MODE_WINDOW_1024 || mode == MODE_FULLSCREEN_1024) {
		windowWidth = 1024;
		windowHeight = 768;
	}

	printf("Creating window: %dx%d\n", windowWidth, windowHeight);

	SDL_Window* window = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		windowFlags
	);

	if (!window) {
		fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
		return 1;
	}

	/* Set game as active (will get focus when window is shown) */
	g_bActiveApp = TRUE;

	/* Create renderer */
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!renderer) {
		fprintf(stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}

	printf("SDL initialization complete.\n");
	printf("\n");

	//-----------------------------------------------------------------
	// Initialize language and character input (IM E)
	// This MUST be done before InitGame() - same order as WinMain
	//-----------------------------------------------------------------
	printf("Detecting language and initializing IME...\n");
	DARKEDEN_LANGUAGE Language = CheckDarkEdenLanguage();

	// Override to Chinese for macOS/Linux builds with Chinese resources
	// (Windows detection might not work properly on these platforms)
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

#ifndef _DEBUG
	// Crash report initialization (Windows only, skip on non-Windows platforms)
	// if (gC_ci->IsKorean() == true)
	//     InitCrashReport();
#endif

	/* Initialize game */
	printf("Initializing game...\n");
	if (!InitGame()) {
		fprintf(stderr, "ERROR: Game initialization failed\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return 1;
	}

	//-----------------------------------------------------------------
	// Set user language based on detected language
	// This MUST be done after InitGame() - matches WinMain order
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

	printf("Game initialization complete.\n");
	fflush(stdout);
	printf("\n");
	printf("Starting game loop...\n");
	fflush(stdout);
	printf("Press Ctrl+C or close window to exit.\n");
	fflush(stdout);
	printf("\n");
	fflush(stdout);

	/* Main game loop */
	SDL_Event event;
	Uint32 frameCount = 0;
	Uint32 lastFrameTime = SDL_GetTicks();

	g_bRunning = true;

	/* Debug: Print initial g_Mode value */
	extern enum CLIENT_MODE g_Mode;
	printf("DEBUG: Initial g_Mode = %d (MAINMENU=%d, GAME=%d, NULL=%d)\n",
	       g_Mode, MODE_MAINMENU, MODE_GAME, MODE_NULL);

	while (g_bRunning) {
		/* Calculate delta time */
		Uint32 currentFrameTime = SDL_GetTicks();
		Uint32 deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		/* Debug: Print g_Mode every 60 frames (approx 1 second) */
		static Uint32 debugFrameCount = 0;
		if ((++debugFrameCount % 60) == 0) {
			printf("DEBUG: Frame %u, g_Mode = %d\n", debugFrameCount, g_Mode);
		}

		/* Process SDL events */
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					g_bRunning = false;
					break;

				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
						g_bActiveApp = false;
					} else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
						g_bActiveApp = true;
					}
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						g_bRunning = false;
					}
					break;

				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					/* TODO: Forward mouse events to game input system */
					break;
			}
		}

		/* Only update game if active */
		if (g_bActiveApp) {
			/* Call game update/render function */
			if (g_pCGameUpdate != NULL) {
				g_pCGameUpdate->Update();
			}

			/* Present back buffer to screen */
			if (g_pBack != NULL) {
				spritectl_surface_t backend_surface = g_pBack->GetBackendSurface();
				if (backend_surface != SPRITECTL_INVALID_SURFACE) {
					spritectl_present_surface(backend_surface, renderer);
				}
			}

			/* Present rendered frame */
			SDL_RenderPresent(renderer);

			frameCount++;
		} else {
			/* Game inactive - sleep to reduce CPU usage */
			SDL_Delay(10);
		}

		/* Cap at ~60 FPS */
		Uint32 frameTime = SDL_GetTicks() - currentFrameTime;
		if (frameTime < 16) {
			SDL_Delay(16 - frameTime);
		}
	}

	/* Cleanup */
	printf("\n");
	printf("Shutting down...\n");

	ReleaseAllObjects();

	// Clean up character input object (must be done after ReleaseAllObjects)
	// This matches the cleanup order in WinMain
	delete gC_ci;
	gC_ci = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	printf("Game exited cleanly.\n");
	printf("Total frames rendered: %u\n", frameCount);

	return 0;
}

#endif /* PLATFORM_WINDOWS */
