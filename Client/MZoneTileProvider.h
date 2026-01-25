#ifndef __MZONE_TILEPROVIDER_H__
#define __MZONE_TILEPROVIDER_H__

#include "MZone.h"
#include "TileRenderer.h"

/**
 * MZoneTileProvider - Adapter to make MZone work with ITileDataProvider interface
 *
 * This allows MZone to be used with TileRenderer without modifying MZone class.
 * It wraps MZone and provides the ITileDataProvider interface.
 */
class MZoneTileProvider : public ITileDataProvider
{
public:
	MZoneTileProvider();
	~MZoneTileProvider();

	// Set the MZone to wrap
	void SetZone(MZone* pZone) { m_pZone = pZone; }

	// ITileDataProvider interface implementation
	virtual int GetSpriteID(int sectorX, int sectorY) override
	{
		if (m_pZone == nullptr)
			return -1;  // SPRITEID_NULL

		if (!IsInBounds(sectorX, sectorY))
			return -1;  // Out of bounds

		const MSector& sector = m_pZone->GetSector(sectorX, sectorY);
		TYPE_SPRITEID spriteID = sector.GetSpriteID();

		// spriteID=0 or SPRITEID_NULL means no tile
		if (spriteID == 0 || spriteID == SPRITEID_NULL)
			return -1;

		return spriteID;
	}

	virtual int GetWidth() const override
	{
		if (m_pZone == nullptr)
			return 0;
		return m_pZone->GetWidth();
	}

	virtual int GetHeight() const override
	{
		if (m_pZone == nullptr)
			return 0;
		return m_pZone->GetHeight();
	}

private:
	bool IsInBounds(int x, int y) const
	{
		if (m_pZone == nullptr)
			return false;
		return (x >= 0 && y >= 0 && x < m_pZone->GetWidth() && y < m_pZone->GetHeight());
	}

private:
	MZone* m_pZone;
};

#endif // __MZONE_TILEPROVIDER_H__
