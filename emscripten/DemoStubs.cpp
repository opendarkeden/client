/*----------------------------------------------------------------------------

	DemoStubs.cpp

	Stub implementations for game functions that MZone depends on.
	These are minimal implementations to satisfy the linker for the demo.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"
#include "MTopView.h"
#include "MCreature.h"
#include "MParty.h"
#include "MEffect.h"
#include "Packet/Properties.h"
#include "MZoneTable.h"
#include "MChaseEffect.h"
#include "MFakeCreature.h"
#include "MShadowObject.h"
#include "MZoneSoundManager.h"
#include "MItem.h"
#include "MPlayer.h"
#include "MEffectGeneratorTable.h"
#include "MTypeDef.h"

//-----------------------------------------------------------------------------
// Network stubs (demo is single-player)
//-----------------------------------------------------------------------------
void SendStatusInfoToParty() {
	// No-op for demo
}

void SendPositionInfoToParty() {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Action stubs
//-----------------------------------------------------------------------------
class MActionResult;  // Forward declaration

bool ExecuteActionInfoFromMainNode(
	WORD actionID,
	WORD actionSeq,
	WORD flags,
	int flagCount,
	int damage,
	UINT damagerRace,
	WORD itemID,
	WORD objectID,
	int option,
	UINT option2,
	MActionResult* result,
	bool bUpdateSchoolLevel,
	int X,
	int Y)
{
	// No-op for demo
	return false;
}

//-----------------------------------------------------------------------------
// UI stubs (demo has no UI)
//-----------------------------------------------------------------------------
bool UI_IsRunningSelectWayPoint() {
	return false;
}

void UI_RunSelectWayPoint() {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Sound stubs (demo has no sound)
//-----------------------------------------------------------------------------
void PlaySound(WORD soundID, bool repeat, int x, int y) {
	// No-op for demo
}

void PlaySound(WORD soundID) {
	// No-op for demo
}

void StopSound(WORD soundID) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Creature stubs
//-----------------------------------------------------------------------------
BYTE MCreature::GetActionCountMax() const {
	return 1;  // Default action count
}

//-----------------------------------------------------------------------------
// MTopView stubs
//-----------------------------------------------------------------------------
int MTopView::GetRandomMonsterTypeInZone() const {
	return 0;  // Default monster type
}

int MTopView::GetMaxEffectFrame(BLT_TYPE bltType, TYPE_FRAMEID frameID) const {
	return 1;  // Default max frame
}

int MTopView::GetEffectSpriteType(BLT_TYPE bltType, TYPE_FRAMEID frameID) const {
	return 0;  // Default effect type
}

//-----------------------------------------------------------------------------
// Effect stubs
//-----------------------------------------------------------------------------
// Note: MEffect::IsDelayFrame is defined in MEffect.cpp
// Note: MEffect::IsWaitFrame is defined in MEffect.cpp

//-----------------------------------------------------------------------------
// Party stubs (demo has no multiplayer)
//-----------------------------------------------------------------------------
PARTY_INFO* MParty::GetMemberInfo(const char* name) const {
	return nullptr;  // No party members in demo
}

bool MParty::HasMember(const char* name) const {
	return false;  // No party in demo
}

//-----------------------------------------------------------------------------
// Additional MCreature stubs
//-----------------------------------------------------------------------------
void MCreature::SetHalluCreature(TYPE_CREATURETYPE type) {
	// No-op for demo
}

void MCreature::SetFlyingCreature() {
	// No-op for demo
}

void MCreature::SetGroundCreature() {
	// No-op for demo
}

void MCreature::ClearChatString() {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Properties stub
//-----------------------------------------------------------------------------
std::string Properties::getProperty(const char* key) const throw(NoSuchElementException) {
	return "";  // Empty property for demo
}

//-----------------------------------------------------------------------------
// Zone info stubs
//-----------------------------------------------------------------------------
TYPE_SOUNDID ZONETABLE_INFO::GetRandomSoundID() const {
	return 0;  // No sound
}

void SECTORSOUND_INFO::SaveToFile(std::ofstream& file) const {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// CZoneTable implementation (uses actual class from MZoneTable.h)
//-----------------------------------------------------------------------------
ZONETABLE_INFO* CZoneTable::Get(TYPE_ZONEID zoneID) {
	static ZONETABLE_INFO instance;
	return &instance;
}

//-----------------------------------------------------------------------------
// Effect stubs (additional)
//-----------------------------------------------------------------------------
// Note: MChaseEffect has complex dependencies - commented out for now
// MChaseEffect::MChaseEffect(BYTE type) {
// 	// No-op for demo
// }

//-----------------------------------------------------------------------------
// Fake creature stub
//-----------------------------------------------------------------------------
bool MFakeCreature::IsFakeEnd() {
	return true;  // Always ended for demo
}

//-----------------------------------------------------------------------------
// Shadow object stub
//-----------------------------------------------------------------------------
MShadowObject::MShadowObject() {
	// Default constructor
}

//-----------------------------------------------------------------------------
// Guidance effect stubs (MLinearEffect and MMovingEffect now in actual .cpp files)
//-----------------------------------------------------------------------------
MGuidanceEffect::MGuidanceEffect(BYTE type) : MLinearEffect(type) {
	// Constructor - initialize base class
}

MGuidanceEffect::~MGuidanceEffect() {
	// Destructor
}

void MGuidanceEffect::SetTraceCreatureID(TYPE_OBJECTID creatureID) {
	// No-op for demo
}

bool MGuidanceEffect::TraceCreature() {
	return false;  // Not tracing
}

// Provide Update implementation for vtable
bool MGuidanceEffect::Update() {
	return false;  // Finished immediately
}

//-----------------------------------------------------------------------------
// Alpha sprite palette stub
//-----------------------------------------------------------------------------
void CAlphaSpritePal::Blt(WORD* pDest, WORD pitch, MPalette& palette) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Time checker stub
//-----------------------------------------------------------------------------
void ShowTimeChecker::SaveToFile(std::ofstream& file) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Zone sound node stubs
//-----------------------------------------------------------------------------
ZONESOUND_NODE::ZONESOUND_NODE(TYPE_SOUNDID soundID) {
	// Constructor
}

ZONESOUND_NODE::~ZONESOUND_NODE() {
	// Destructor
}

void ZONESOUND_NODE::Play(int x, int y, bool loop) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// Zone table info stub
//-----------------------------------------------------------------------------
ZONETABLE_INFO::ZONETABLE_INFO() {
	// Default constructor
}

//-----------------------------------------------------------------------------
// ShowTimeChecker stubs
//-----------------------------------------------------------------------------
ShowTimeChecker::ShowTimeChecker() {
	// Default constructor
}

ShowTimeChecker::~ShowTimeChecker() {
	// Destructor
}

void ShowTimeChecker::LoadFromFile(std::ifstream& file) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// MGridItemManager stub
//-----------------------------------------------------------------------------
MGridItemManager::~MGridItemManager() {
	// Destructor
}

//-----------------------------------------------------------------------------
// SECTORSOUND_INFO stub
//-----------------------------------------------------------------------------
void SECTORSOUND_INFO::LoadFromFile(std::ifstream& file) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// MEffectGeneratorTable stub
//-----------------------------------------------------------------------------
void MEffectGeneratorTable::GenerateNext(MEffect* pEffect) {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// MItem stubs
//-----------------------------------------------------------------------------
void MItem::SetDropping() {
	// No-op for demo
}

void MItem::NextDropFrame() {
	// No-op for demo
}

MItem* MItem::NewItem(ITEM_CLASS itemClass) {
	return nullptr;  // No items for demo
}

//-----------------------------------------------------------------------------
// MPlayer stubs
//-----------------------------------------------------------------------------
void MPlayer::ClearItemCheckBuffer() {
	// No-op for demo
}

//-----------------------------------------------------------------------------
// MTopView stubs
//-----------------------------------------------------------------------------
BYTE MTopView::GetDirectionToPosition(int fromX, int fromY, int toX, int toY) {
	return 0;  // Default direction
}

int MTopView::GetEffectLight(BLT_TYPE bltType, TYPE_FRAMEID frameID, int dir, int frame) const {
	return 0;  // No effect light
}
