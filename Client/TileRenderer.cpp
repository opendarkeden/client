#include "Client_PCH.h"
#include "TileRenderer.h"

//----------------------------------------------------------------------
// TileRenderer Implementation
//----------------------------------------------------------------------

// Debug counter to limit debug output
static int g_DebugDrawCount = 0;
static const int DEBUG_DRAW_LIMIT = 10;

TileRenderer::TileRenderer()
	: m_surface(nullptr)
	, m_spritePack(nullptr)
	, m_tileX(48)   // Default TILE_X
	, m_tileY(24)   // Default TILE_Y
	, m_nullTileSpriteID(0)  // Default SPRITEID_NULL
{
}

TileRenderer::~TileRenderer()
{
	Release();
}

bool TileRenderer::Init(CSpriteSurface* surface, CSpritePack* spritePack)
{
	if (surface == nullptr || spritePack == nullptr)
	{
		return false;
	}

	m_surface = surface;
	m_spritePack = spritePack;

	return true;
}

void TileRenderer::Release()
{
	m_surface = nullptr;
	m_spritePack = nullptr;
}

bool TileRenderer::DrawTile(int spriteID, int screenX, int screenY, float scale)
{
	POINT point = { screenX, screenY };
	return DrawTile(spriteID, &point, scale);
}

bool TileRenderer::DrawTile(int spriteID, POINT* pPoint, float scale)
{
	if (!IsInit())
	{
		return false;
	}

	if (pPoint == nullptr)
	{
		return false;
	}

	return DrawTileInternal(spriteID, pPoint, scale);
}

bool TileRenderer::DrawTileInternal(int spriteID, POINT* pPoint, float scale)
{
	// Handle null tiles
	if (spriteID == m_nullTileSpriteID || spriteID == -1)
	{
		// Skip null tiles
		return true;
	}

	// Validate sprite ID
	if (!IsValidSpriteID(spriteID))
	{
		// Invalid sprite ID - skip rendering
		return false;
	}

	// Get sprite from pack using subscript operator
	CSprite& sprite = (*m_spritePack)[spriteID];

	// Check if sprite is initialized
	if (!sprite.IsInit())
	{
		// Sprite not initialized - skip rendering
		return false;
	}

	// Draw the sprite
	// Note: We don't scale the sprite itself here.
	// The caller is responsible for positioning sprites at correct coordinates.
	// For zoom in map_viewer, coordinate spacing is adjusted, not sprite size.
	m_surface->BltSprite(pPoint, &sprite);

	return true;
}

bool TileRenderer::IsValidSpriteID(int spriteID) const
{
	if (m_spritePack == nullptr)
	{
		return false;
	}

	// Check if spriteID is within valid range
	// Sprite packs are 0-indexed, so valid range is [0, GetSize()-1]
	int maxSize = m_spritePack->GetSize();

	return (spriteID >= 0 && spriteID < maxSize);
}

void TileRenderer::DrawTiles(ITileDataProvider* provider,
							 int viewportX, int viewportY,
							 int viewportWidth, int viewportHeight,
							 int surfaceX, int surfaceY)
{
	if (!IsInit() || provider == nullptr)
	{
		return;
	}

	// Clamp viewport to provider bounds
	int mapWidth = provider->GetWidth();
	int mapHeight = provider->GetHeight();

	if (viewportX < 0) viewportX = 0;
	if (viewportY < 0) viewportY = 0;
	if (viewportX + viewportWidth > mapWidth) viewportWidth = mapWidth - viewportX;
	if (viewportY + viewportHeight > mapHeight) viewportHeight = mapHeight - viewportY;

	// Lock surface for rendering
	if (!m_surface->Lock())
	{
		return;
	}

	// Render each tile
	POINT tilePoint;
	tilePoint.y = surfaceY;

	for (int y = viewportY; y < viewportY + viewportHeight; y++)
	{
		tilePoint.x = surfaceX;

		for (int x = viewportX; x < viewportX + viewportWidth; x++)
		{
			// Get sprite ID for this sector
			int spriteID = provider->GetSpriteID(x, y);

			// Draw the tile
			DrawTileInternal(spriteID, &tilePoint);

			// Move to next tile position
			tilePoint.x += m_tileX;
		}

		// Move to next row
		tilePoint.y += m_tileY;
	}

	// Unlock surface
	m_surface->Unlock();
}

void TileRenderer::DrawTilesNoLock(ITileDataProvider* provider,
								   int viewportX, int viewportY,
								   int viewportWidth, int viewportHeight,
								   int surfaceX, int surfaceY)
{
	if (!IsInit() || provider == nullptr)
	{
		return;
	}

	// Clamp viewport to provider bounds
	int mapWidth = provider->GetWidth();
	int mapHeight = provider->GetHeight();

	if (viewportX < 0) viewportX = 0;
	if (viewportY < 0) viewportY = 0;
	if (viewportX + viewportWidth > mapWidth) viewportWidth = mapWidth - viewportX;
	if (viewportY + viewportHeight > mapHeight) viewportHeight = mapHeight - viewportY;

	// NOTE: Caller must have already called Lock() on the surface
	// This is for use in DrawTileSurface where Lock is already called

	// Render each tile
	POINT tilePoint;
	tilePoint.y = surfaceY;

	for (int y = viewportY; y < viewportY + viewportHeight; y++)
	{
		tilePoint.x = surfaceX;

		for (int x = viewportX; x < viewportX + viewportWidth; x++)
		{
			// Get sprite ID for this sector
			int spriteID = provider->GetSpriteID(x, y);

			// Draw the tile
			DrawTileInternal(spriteID, &tilePoint);

			// Move to next tile position
			tilePoint.x += m_tileX;
		}

		// Move to next row
		tilePoint.y += m_tileY;
	}

	// NOTE: Caller must call Unlock() after this method returns
}

void TileRenderer::ClearSurface(WORD color)
{
	if (!IsInit())
	{
		return;
	}

	m_surface->FillSurface(color);
}

void TileRenderer::SetTileDimensions(int tileX, int tileY)
{
	m_tileX = tileX;
	m_tileY = tileY;
}

void TileRenderer::SetNullTileSpriteID(int spriteID)
{
	m_nullTileSpriteID = spriteID;
}
