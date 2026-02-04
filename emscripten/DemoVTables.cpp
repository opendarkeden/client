/*----------------------------------------------------------------------------

	DemoVTables.cpp

	Minimal vtable definitions for classes needed by the demo.
	These are required for dynamic_cast and typeinfo to work.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"
#include "MItem.h"
#include "MItemManager.h"

// Provide destructor implementations to force vtable emission
// These are the "key functions" that must be defined for typeinfo to work
MItem::~MItem() {}
MCorpse::~MCorpse() {}
MItemManager::~MItemManager() {}

// Provide implementation for first virtual function in MItemManager
// This is required for vtable emission
void MItemManager::CheckAffectStatus(MItem* pItem) {
	// No-op for demo
}

// Provide implementations for pure virtual functions in MItem
// These are required for vtable emission
TYPE_FRAMEID MItem::GetDropFrameID() const {
	return 0;  // Default frame
}

TYPE_SOUNDID MItem::GetTileSoundID() const {
	return 0;  // No sound
}

bool MItem::IsInsertToItem(const MItem* pItem) const {
	return false;  // Not insertable by default
}
