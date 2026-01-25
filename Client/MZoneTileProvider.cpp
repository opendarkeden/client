#include "MZoneTileProvider.h"

MZoneTileProvider::MZoneTileProvider()
	: m_pZone(nullptr)
{
}

MZoneTileProvider::~MZoneTileProvider()
{
	// Note: We don't own the MZone, so don't delete it
}
