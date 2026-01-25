/**
 * @file tile_renderer_test.cpp
 * @brief Tile Renderer Test - 最小化 Tile 渲染测试程序
 *
 * 功能：
 * - 使用 TileRenderer 类渲染 tiles
 * - Hardcoded tile 数据（不依赖 map 文件）
 * - 验证 TileRenderer 可以独立工作
 * - 支持相机移动（WASD/方向键）
 */

#include "Client/TileRenderer.h"
#include "Client/SpriteLib/CSpriteSurface.h"
#include "Client/SpriteLib/CSpritePack.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <sys/stat.h>

//----------------------------------------------------------------------
// MockTileDataProvider - 提供 hardcoded tile 数据
//----------------------------------------------------------------------
class MockTileDataProvider : public ITileDataProvider
{
public:
	MockTileDataProvider(int width, int height)
		: m_width(width)
		, m_height(height)
		, m_data(nullptr)
		, m_spriteIDOffset(0)
		, m_spriteIDStep(1)
		, m_spriteIDRange(100)
	{
		// Allocate tile data
		m_data = new int[width * height];

		// Generate initial pattern
		GeneratePattern();
	}

	void SetSpriteIDRange(int offset, int step, int range)
	{
		m_spriteIDOffset = offset;
		m_spriteIDStep = step;
		m_spriteIDRange = range;
		GeneratePattern();
	}

	void GeneratePattern()
	{
		// Initialize with a checkerboard pattern of sprite IDs
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				int index = y * m_width + x;

				// Use configurable sprite ID range and step
				int patternIndex = (x + y * 16) % m_spriteIDRange;
				int spriteID = m_spriteIDOffset + (patternIndex * m_spriteIDStep);

				m_data[index] = spriteID;
			}
		}

		std::cout << "Generated pattern: offset=" << m_spriteIDOffset
				  << ", step=" << m_spriteIDStep
				  << ", range=" << m_spriteIDRange << std::endl;
	}

	~MockTileDataProvider()
	{
		delete[] m_data;
	}

	// ITileDataProvider interface
	virtual int GetSpriteID(int sectorX, int sectorY) override
	{
		// Clamp to valid range
		if (sectorX < 0 || sectorX >= m_width ||
			sectorY < 0 || sectorY >= m_height)
		{
			return -1; // SPRITEID_NULL
		}

		int index = sectorY * m_width + sectorX;
		return m_data[index];
	}

	virtual int GetWidth() const override
	{
		return m_width;
	}

	virtual int GetHeight() const override
	{
		return m_height;
	}

private:
	int m_width;
	int m_height;
	int* m_data;
	int m_spriteIDOffset;
	int m_spriteIDStep;
	int m_spriteIDRange;
};

//----------------------------------------------------------------------
// TileRendererTest - Main test application
//----------------------------------------------------------------------
class TileRendererTest
{
public:
	TileRendererTest()
		: m_window(nullptr)
		, m_renderer(nullptr)
		, m_screenSurface(nullptr)
		, m_spritePack(nullptr)
		, m_tileRenderer(nullptr)
		, m_tileDataProvider(nullptr)
		, m_cameraX(0)
		, m_cameraY(0)
		, m_running(false)
		, m_tileCountX(20)
		, m_tileCountY(15)
	{
	}

	~TileRendererTest()
	{
		Cleanup();
	}

	bool Initialize(const char* spkFile, int windowWidth, int windowHeight)
	{
		// Initialize SpriteLib backend FIRST
		if (spritectl_init() != 0)
		{
			std::cerr << "Failed to initialize SpriteLib backend" << std::endl;
			return false;
		}

		// Create window
		m_window = SDL_CreateWindow(
			"Tile Renderer Test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_SHOWN
		);

		if (!m_window)
		{
			std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
			return false;
		}

		// Create renderer
		m_renderer = SDL_CreateRenderer(m_window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_renderer)
		{
			std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
			return false;
		}

		// Create screen surface
		m_screenSurface = new CSpriteSurface();
		if (!m_screenSurface->Init(windowWidth, windowHeight))
		{
			std::cerr << "Failed to initialize screen surface" << std::endl;
			return false;
		}

		// Load sprite pack
		m_spritePack = new CSpritePack();
		if (!m_spritePack->LoadFromFile(spkFile))
		{
			std::cerr << "Failed to load sprite pack: " << spkFile << std::endl;
			std::cerr << "Note: This test requires a tile sprite pack (.spk file)" << std::endl;
			return false;
		}

		std::cout << "Loaded " << m_spritePack->GetSize() << " sprites from pack" << std::endl;

		// Debug: Show first few sprites info
		std::cout << "First 5 sprites status:" << std::endl;
		for (int i = 0; i < 5 && i < m_spritePack->GetSize(); i++)
		{
			CSprite& sprite = (*m_spritePack)[i];
			std::cout << "  Sprite " << i << ": "
					  << (sprite.IsInit() ? "Initialized" : "NOT initialized")
					  << ", Width: " << sprite.GetWidth()
					  << ", Height: " << sprite.GetHeight()
					  << std::endl;
		}

		// Validate sprite pack has enough sprites
		if (m_spritePack->GetSize() < 10)
		{
			std::cerr << "Warning: Sprite pack has less than 10 sprites" << std::endl;
			std::cerr << "Pattern uses sprite IDs 0-9, adjusting to available range" << std::endl;
		}

		// Create tile renderer
		m_tileRenderer = new TileRenderer();
		if (!m_tileRenderer->Init(m_screenSurface, m_spritePack))
		{
			std::cerr << "Failed to initialize TileRenderer" << std::endl;
			return false;
		}

		// Create mock tile data provider (20x15 tiles)
		m_tileDataProvider = new MockTileDataProvider(50, 50);

		// Calculate how many tiles fit on screen
		int tileX = m_tileRenderer->GetTileX();
		int tileY = m_tileRenderer->GetTileY();
		m_tileCountX = (windowWidth / tileX) + 2;
		m_tileCountY = (windowHeight / tileY) + 2;

		std::cout << "Tile dimensions: " << tileX << "x" << tileY << std::endl;
		std::cout << "Tiles on screen: " << m_tileCountX << "x" << m_tileCountY << std::endl;

		m_running = true;
		return true;
	}

	void Cleanup()
	{
		delete m_tileDataProvider;
		delete m_tileRenderer;
		delete m_spritePack;
		delete m_screenSurface;

		if (m_renderer)
		{
			SDL_DestroyRenderer(m_renderer);
		}
		if (m_window)
		{
			SDL_DestroyWindow(m_window);
		}

		spritectl_shutdown();
	}

	void Run()
	{
		SDL_Event event;

		while (m_running)
		{
			// Process events
			while (SDL_PollEvent(&event))
			{
				HandleEvent(event);
			}

			// Render
			Render();

			// Present
			SDL_RenderPresent(m_renderer);

			// Cap FPS
			SDL_Delay(16); // ~60 FPS
		}
	}

private:
	void HandleEvent(const SDL_Event& event)
	{
		if (event.type == SDL_QUIT)
		{
			m_running = false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_running = false;
				break;

			case SDLK_UP:
			case SDLK_w:
				m_cameraY -= 1;
				break;

			case SDLK_DOWN:
			case SDLK_s:
				m_cameraY += 1;
				break;

			case SDLK_LEFT:
			case SDLK_a:
				m_cameraX -= 1;
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				m_cameraX += 1;
				break;

			// Switch sprite ID modes
			case SDLK_1:
				// Mode 1: First 100 tiles (0-99)
				m_tileDataProvider->SetSpriteIDRange(0, 1, 100);
				std::cout << "Mode 1: First 100 tiles" << std::endl;
				break;

			case SDLK_2:
				// Mode 2: Tiles 100-199
				m_tileDataProvider->SetSpriteIDRange(100, 1, 100);
				std::cout << "Mode 2: Tiles 100-199" << std::endl;
				break;

			case SDLK_3:
				// Mode 3: Every 100th tile (0, 100, 200, ...)
				m_tileDataProvider->SetSpriteIDRange(0, 100, 100);
				std::cout << "Mode 3: Every 100th tile" << std::endl;
				break;

			case SDLK_4:
				// Mode 4: Every 1000th tile (0, 1000, 2000, ...)
				m_tileDataProvider->SetSpriteIDRange(0, 1000, 100);
				std::cout << "Mode 4: Every 1000th tile" << std::endl;
				break;

			case SDLK_5:
				// Mode 5: Random range starting from 10000
				m_tileDataProvider->SetSpriteIDRange(10000, 1, 100);
				std::cout << "Mode 5: Tiles 10000-10099" << std::endl;
				break;

			default:
				break;
			}

			// Clamp camera position
			if (m_cameraX < 0) m_cameraX = 0;
			if (m_cameraY < 0) m_cameraY = 0;
			if (m_cameraX > m_tileDataProvider->GetWidth() - m_tileCountX)
				m_cameraX = m_tileDataProvider->GetWidth() - m_tileCountX;
			if (m_cameraY > m_tileDataProvider->GetHeight() - m_tileCountY)
				m_cameraY = m_tileDataProvider->GetHeight() - m_tileCountY;
		}
	}

	void Render()
	{
		// Clear screen surface
		m_tileRenderer->ClearSurface(0);

		// Render visible tiles
		m_tileRenderer->DrawTiles(
			m_tileDataProvider,
			m_cameraX,
			m_cameraY,
			m_tileCountX,
			m_tileCountY,
			0, 0  // Render to surface origin
		);

		// Present surface to screen
		spritectl_surface_t backendSurface = m_screenSurface->GetBackendSurface();
		spritectl_present_surface(backendSurface, m_renderer);
	}

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	CSpriteSurface* m_screenSurface;
	CSpritePack* m_spritePack;
	TileRenderer* m_tileRenderer;
	MockTileDataProvider* m_tileDataProvider;

	int m_cameraX;
	int m_cameraY;
	int m_tileCountX;
	int m_tileCountY;
	bool m_running;
};

//----------------------------------------------------------------------
// Helper function to check if file exists
//----------------------------------------------------------------------
bool FileExists(const char* path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

//----------------------------------------------------------------------
// Main
//----------------------------------------------------------------------
int main(int argc, char* argv[])
{
	std::cout << "=== Tile Renderer Test ===" << std::endl;
	std::cout << "Testing TileRenderer class with hardcoded tile data" << std::endl;
	std::cout << std::endl;

	// Check command line arguments
	const char* spkFile = nullptr;

	if (argc > 1)
	{
		spkFile = argv[1];
	}
	else
	{
		// Try to find a tile sprite pack in common locations
		const char* commonPaths[] = {
			"../../DarkEden/Data/Image/Tile.spk",
			"../../../DarkEden/Data/Image/Tile.spk",
			"DarkEden/Data/Image/Tile.spk",
			"Data/Image/Tile.spk",
			"Tile.spk",
			nullptr
		};

		for (int i = 0; commonPaths[i] != nullptr; i++)
		{
			if (FileExists(commonPaths[i]))
			{
				spkFile = commonPaths[i];
				std::cout << "Auto-detected sprite pack: " << spkFile << std::endl;
				break;
			}
		}
	}

	if (!spkFile)
	{
		std::cerr << "Usage: " << argv[0] << " <tile_spk_file>" << std::endl;
		std::cerr << std::endl;
		std::cerr << "Please provide a tile sprite pack (.spk) file." << std::endl;
		std::cerr << "Common locations:" << std::endl;
		std::cerr << "  - DarkEden/Data/Image/Tile.spk" << std::endl;
		std::cerr << "  - ../../DarkEden/Data/Image/Tile.spk" << std::endl;
		return 1;
	}

	// Create and run test
	TileRendererTest test;

	if (!test.Initialize(spkFile, 800, 600))
	{
		std::cerr << "Failed to initialize test" << std::endl;
		return 1;
	}

	std::cout << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "  WASD / Arrow Keys - Move camera" << std::endl;
	std::cout << "  1 - First 100 tiles (0-99)" << std::endl;
	std::cout << "  2 - Tiles 100-199" << std::endl;
	std::cout << "  3 - Every 100th tile" << std::endl;
	std::cout << "  4 - Every 1000th tile" << std::endl;
	std::cout << "  5 - Tiles 10000-10099" << std::endl;
	std::cout << "  ESC - Exit" << std::endl;
	std::cout << std::endl;

	test.Run();

	return 0;
}
