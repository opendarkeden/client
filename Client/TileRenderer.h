#ifndef __TILE_RENDERER_H__
#define __TILE_RENDERER_H__

// Platform-specific type definitions (includes POINT, RECT, WORD, etc.)
#include "Client_PCH.h"

// SpriteLib includes - needed for full type definitions
#include "SpriteLib/CSpriteSurface.h"
#include "SpriteLib/CSpritePack.h"

// Forward declarations
class CSprite;

//----------------------------------------------------------------------
// ITileDataProvider - Abstract interface for providing tile data
//----------------------------------------------------------------------
// This allows the TileRenderer to work with different data sources
// without depending on MZone, MPlayer, or other game classes
//----------------------------------------------------------------------
class ITileDataProvider
{
public:
	virtual ~ITileDataProvider() {}

	// Get the sprite ID for a tile at the given sector coordinates
	// Returns SPRITEID_NULL (-1) if no tile sprite exists
	virtual int GetSpriteID(int sectorX, int sectorY) = 0;

	// Get the width of the tile map (in sectors)
	virtual int GetWidth() const = 0;

	// Get the height of the tile map (in sectors)
	virtual int GetHeight() const = 0;
};

//----------------------------------------------------------------------
// TileRenderer - Minimal tile rendering component
//----------------------------------------------------------------------
// Extracted from MTopView::DrawTileSurface() to reduce dependencies
// Only depends on CSpriteSurface and CSpritePack
//----------------------------------------------------------------------
class TileRenderer
{
public:
	TileRenderer();
	~TileRenderer();

	//------------------------------------------------------------
	// Initialization
	//------------------------------------------------------------
	// Initialize the renderer with a target surface and sprite pack
	// Returns true on success, false on failure
	bool Init(CSpriteSurface* surface, CSpritePack* spritePack);

	// Release resources
	void Release();

	//------------------------------------------------------------
	// Rendering
	//------------------------------------------------------------

	// Draw a single tile at the specified screen position
	// spriteID: The ID of the sprite to draw (or SPRITEID_NULL for default)
	// screenX, screenY: Screen coordinates where to draw the tile
	// scale: Optional scale factor (1.0 = normal size, 2.0 = 2x, etc.)
	// Returns true on success, false on failure
	bool DrawTile(int spriteID, int screenX, int screenY, float scale = 1.0f);

	// Draw a single tile using POINT for position
	// scale: Optional scale factor (1.0 = normal size, 2.0 = 2x, etc.)
	bool DrawTile(int spriteID, POINT* pPoint, float scale = 1.0f);

	// Draw all visible tiles within the given viewport
	// provider: Data source for tile information
	// viewportX, viewportY: Top-left corner of viewport in sector coordinates
	// viewportWidth, viewportHeight: Size of viewport in sectors
	// surfaceX, surfaceY: Position on target surface to start drawing
	// NOTE: This method calls Lock/Unlock internally
	void DrawTiles(ITileDataProvider* provider,
				   int viewportX, int viewportY,
				   int viewportWidth, int viewportHeight,
				   int surfaceX = 0, int surfaceY = 0);

	// Draw tiles without Lock/Unlock (for use when surface is already locked)
	// Use this in DrawTileSurface where Lock() is already called
	void DrawTilesNoLock(ITileDataProvider* provider,
						 int viewportX, int viewportY,
						 int viewportWidth, int viewportHeight,
						 int surfaceX = 0, int surfaceY = 0);

	// Clear the rendering surface with the specified color
	void ClearSurface(WORD color = 0);

	//------------------------------------------------------------
	// Configuration
	//------------------------------------------------------------

	// Set tile dimensions (default: TILE_X=48, TILE_Y=24)
	void SetTileDimensions(int tileX, int tileY);

	// Set the sprite ID to use for null tiles (default: SPRITEID_NULL)
	void SetNullTileSpriteID(int spriteID);

	// Get tile dimensions
	int GetTileX() const { return m_tileX; }
	int GetTileY() const { return m_tileY; }

	//------------------------------------------------------------
	// State
	//------------------------------------------------------------

	// Check if renderer is initialized
	bool IsInit() const { return m_surface != nullptr && m_spritePack != nullptr; }

	// Get the target surface
	CSpriteSurface* GetSurface() const { return m_surface; }

	// Get the sprite pack
	CSpritePack* GetSpritePack() const { return m_spritePack; }

private:
	// Internal helper to draw a tile at a point
	// scale: Optional scale factor (1.0 = normal size)
	bool DrawTileInternal(int spriteID, POINT* pPoint, float scale = 1.0f);

	// Internal helper to validate sprite ID
	bool IsValidSpriteID(int spriteID) const;

private:
	// Rendering target
	CSpriteSurface* m_surface;

	// Sprite data source
	CSpritePack* m_spritePack;

	// Tile dimensions (pixels)
	int m_tileX;  // Default: 48
	int m_tileY;  // Default: 24

	// Default sprite for null tiles
	int m_nullTileSpriteID;  // Default: SPRITEID_NULL (usually 0)
};

#endif // __TILE_RENDERER_H__
