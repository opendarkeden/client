/*-----------------------------------------------------------------------------
	sprite_backend_example.cpp

	Complete example of using SpriteLib SDL2 backend
	Demonstrates sprite creation, loading, and rendering

	This example shows how to:
	1. Initialize SDL2 and SpriteLib backend
	2. Create sprite surfaces
	3. Load and create sprites
	4. Render sprites to screen
	5. Handle basic game loop

-----------------------------------------------------------------------------*/

#include "SpriteLibBackend.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Screen dimensions */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* Sprite dimensions */
#define SPRITE_WIDTH 64
#define SPRITE_HEIGHT 64

/* Frame rate */
#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

/*-----------------------------------------------------------------------------
 * Example 1: Create a simple colored sprite
 *----------------------------------------------------------------------------*/
static spritectl_sprite_t create_color_sprite(int width, int height, WORD r, WORD g, WORD b)
{
	/* Allocate pixel buffer */
	size_t pixel_count = width * height;
	size_t data_size = pixel_count * sizeof(WORD);
	WORD* pixels = (WORD*)malloc(data_size);

	if (!pixels) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Fill with solid color (RGB565 format) */
	WORD color = ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
	for (size_t i = 0; i < pixel_count; i++) {
		pixels[i] = color;
	}

	/* Create backend sprite */
	spritectl_sprite_t sprite = spritectl_create_sprite(
		width, height, SPRITECTL_FORMAT_RGB565,
		pixels, data_size
	);

	free(pixels);
	return sprite;
}

/*-----------------------------------------------------------------------------
 * Example 2: Create a gradient sprite
 *----------------------------------------------------------------------------*/
static spritectl_sprite_t create_gradient_sprite(int width, int height)
{
	size_t pixel_count = width * height;
	size_t data_size = pixel_count * sizeof(WORD);
	WORD* pixels = (WORD*)malloc(data_size);

	if (!pixels) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Create gradient pattern */
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			WORD r = (x * 31) / width;   /* Red varies horizontally */
			WORD g = (y * 63) / height;  /* Green varies vertically */
			WORD b = 15;                  /* Blue constant */

			pixels[y * width + x] = (r << 11) | (g << 5) | b;
		}
	}

	spritectl_sprite_t sprite = spritectl_create_sprite(
		width, height, SPRITECTL_FORMAT_RGB565,
		pixels, data_size
	);

	free(pixels);
	return sprite;
}

/*-----------------------------------------------------------------------------
 * Example 3: Create a checkerboard pattern sprite
 *----------------------------------------------------------------------------*/
static spritectl_sprite_t create_checkerboard_sprite(int width, int height, int tile_size)
{
	size_t pixel_count = width * height;
	size_t data_size = pixel_count * sizeof(WORD);
	WORD* pixels = (WORD*)malloc(data_size);

	if (!pixels) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Create checkerboard pattern */
	WORD color1 = ((31 << 11) | (63 << 5) | 31);  /* White */
	WORD color2 = 0;                             /* Black */

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int tile_x = x / tile_size;
			int tile_y = y / tile_size;
			WORD color = ((tile_x + tile_y) % 2) ? color1 : color2;
			pixels[y * width + x] = color;
		}
	}

	spritectl_sprite_t sprite = spritectl_create_sprite(
		width, height, SPRITECTL_FORMAT_RGB565,
		pixels, data_size
	);

	free(pixels);
	return sprite;
}

/*-----------------------------------------------------------------------------
 * Main example: Sprite rendering game loop
 *----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

	spritectl_surface_t backend_surface = SPRITECTL_INVALID_SURFACE;
	spritectl_sprite_t sprites[4] = {SPRITECTL_INVALID_SPRITE};
	int num_sprites = 0;

	bool running = true;
	SDL_Event event;
	Uint32 frame_count = 0;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;

	printf("==========================================\n");
	printf("SpriteLib Backend Usage Example\n");
	printf("==========================================\n\n");

	/* Initialize SDL2 */
	printf("1. Initializing SDL2...\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	/* Create window */
	window = SDL_CreateWindow(
		"SpriteLib Backend Example",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		printf("ERROR: Failed to create window\n");
		goto cleanup;
	}

	/* Create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("ERROR: Failed to create renderer\n");
		goto cleanup;
	}

	/* Create backend surface */
	printf("2. Creating backend surface (%dx%d)...\n", SCREEN_WIDTH, SCREEN_HEIGHT);
	backend_surface = spritectl_create_surface(SCREEN_WIDTH, SCREEN_HEIGHT, SPRITECTL_FORMAT_RGB565);
	if (!backend_surface) {
		printf("ERROR: Failed to create backend surface\n");
		goto cleanup;
	}
	printf("   ✓ Backend surface created\n");

	/* Create example sprites */
	printf("3. Creating example sprites...\n");

	/* Sprite 1: Red solid */
	printf("   Creating red sprite...\n");
	sprites[0] = create_color_sprite(SPRITE_WIDTH, SPRITE_HEIGHT, 31, 0, 0);
	if (sprites[0]) {
		num_sprites++;
		printf("   ✓ Red sprite created\n");
	}

	/* Sprite 2: Green solid */
	printf("   Creating green sprite...\n");
	sprites[1] = create_color_sprite(SPRITE_WIDTH, SPRITE_HEIGHT, 0, 63, 0);
	if (sprites[1]) {
		num_sprites++;
		printf("   ✓ Green sprite created\n");
	}

	/* Sprite 3: Blue solid */
	printf("   Creating blue sprite...\n");
	sprites[2] = create_color_sprite(SPRITE_WIDTH, SPRITE_HEIGHT, 0, 0, 31);
	if (sprites[2]) {
		num_sprites++;
		printf("   ✓ Blue sprite created\n");
	}

	/* Sprite 4: Gradient */
	printf("   Creating gradient sprite...\n");
	sprites[3] = create_gradient_sprite(128, 128);
	if (sprites[3]) {
		num_sprites++;
		printf("   ✓ Gradient sprite created (128x128)\n");
	}

	printf("\n4. Starting game loop...\n");
	printf("   Press ESC or close window to exit\n\n");

	/* Create display texture */
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB565,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	/* Game loop */
	while (running) {
		/* Handle events */
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		/* Animate sprite positions */
		int x1 = (int)((frame_count * 2) % (SCREEN_WIDTH + 100)) - 50;
		int y1 = 100;

		int x2 = SCREEN_WIDTH - ((int)((frame_count * 3) % (SCREEN_WIDTH + 100)) - 50);
		int y2 = 200;

		int x3 = SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2 + (int)(sin(frame_count * 0.05) * 200);
		int y3 = 300;

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
		SDL_RenderClear(renderer);

		/* Get surface pixels */
		if (texture) {
			void* pixels;
			int pitch;
			SDL_LockTexture(texture, NULL, &pixels, &pitch);

			/* Clear to dark blue */
			memset(pixels, 0, SCREEN_HEIGHT * pitch);

			/* Render sprites using backend */
			if (sprites[0]) {
				spritectl_blt_sprite(backend_surface, x1, y1, sprites[0], 0, 255);
			}
			if (sprites[1]) {
				spritectl_blt_sprite(backend_surface, x2, y2, sprites[1], 0, 255);
			}
			if (sprites[2]) {
				spritectl_blt_sprite(backend_surface, x3, y3, sprites[2], 0, 255);
			}
			if (sprites[3]) {
				/* Center gradient sprite */
				spritectl_blt_sprite(
					backend_surface,
					SCREEN_WIDTH / 2 - 64,
					SCREEN_HEIGHT / 2 - 64,
					sprites[3], 0, 255
				);
			}

			SDL_UnlockTexture(texture);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
		}

		SDL_RenderPresent(renderer);

		/* Frame rate control */
		SDL_Delay(FRAME_DELAY);
		frame_count++;
	}

	printf("\n5. Cleanup...\n");

cleanup:
	/* Destroy sprites */
	for (int i = 0; i < 4; i++) {
		if (sprites[i] != SPRITECTL_INVALID_SPRITE) {
			spritectl_destroy_sprite(sprites[i]);
		}
	}

	/* Destroy surface */
	if (backend_surface != SPRITECTL_INVALID_SURFACE) {
		spritectl_destroy_surface(backend_surface);
	}

	/* Cleanup SDL2 */
	if (texture) SDL_DestroyTexture(texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();

	printf("   ✓ All resources cleaned up\n");
	printf("\n==========================================\n");
	printf("Example completed successfully!\n");
	printf("==========================================\n\n");

	printf("Summary:\n");
	printf("  - Rendered %u frames\n", frame_count);
	printf("  - Created %d sprites\n", num_sprites);
	printf("  - Used SpriteLib backend for all operations\n");
	printf("\nKey concepts demonstrated:\n");
	printf("  1. Backend surface creation\n");
	printf("  2. Sprite creation from pixel data\n");
	printf("  3. Sprite blitting with spritectl_blt_sprite\n");
	printf("  4. RGB565 pixel format handling\n");
	printf("  5. Game loop with SDL2\n");

	return 0;
}
