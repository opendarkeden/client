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

/* Forward declarations for game functions (defined in GameInit.cpp, ClientFunction.cpp, etc.) */
extern bool InitGame();
extern void RunGame();
extern void ReleaseAllObjects();

/* Game mode flags from command line */
enum GameMode {
	MODE_WINDOW = 1,
	MODE_FULLSCREEN = 2,
	MODE_WINDOW_1024 = 3,
	MODE_FULLSCREEN_1024 = 4
};

/* Global flags */
bool g_bRunning = true;
bool g_bActiveApp = true;

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

	/* Initialize game */
	printf("Initializing game...\n");
	if (!InitGame()) {
		fprintf(stderr, "ERROR: Game initialization failed\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return 1;
	}

	printf("Game initialization complete.\n");
	printf("\n");
	printf("Starting game loop...\n");
	printf("Press Ctrl+C or close window to exit.\n");
	printf("\n");

	/* Main game loop */
	SDL_Event event;
	Uint32 frameCount = 0;
	Uint32 lastFrameTime = SDL_GetTicks();

	g_bRunning = true;

	while (g_bRunning) {
		/* Calculate delta time */
		Uint32 currentFrameTime = SDL_GetTicks();
		Uint32 deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

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
			/* TODO: Integrate with existing game loop */
			/* RunGame(); */

			/* Clear screen */
			SDL_RenderClear(renderer);

			/* TODO: Render game scene */
			/* This will call into MTopView and other rendering code */

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

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	printf("Game exited cleanly.\n");
	printf("Total frames rendered: %u\n", frameCount);

	return 0;
}

#endif /* PLATFORM_WINDOWS */
