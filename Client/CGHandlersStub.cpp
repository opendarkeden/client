/*-----------------------------------------------------------------------------

	CGHandlersStub.cpp

	Stub implementations for CG (Client to Game) packet handlers.
	These are needed because the actual handler implementations are server-side.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"

// Include packet headers to get handler class declarations
#include "Packet/Cpackets/CGBuyStoreItem.h"
#include "Packet/Cpackets/CGConnectSetKey.h"
#include "Packet/Cpackets/CGDisplayItem.h"
#include "Packet/Cpackets/CGDonationMoney.h"
#include "Packet/Cpackets/CGLotterySelect.h"
#include "Packet/Cpackets/CGMixItem.h"
#include "Packet/Cpackets/CGRequestStoreInfo.h"
#include "Packet/Cpackets/CGSelectQuest.h"
#include "Packet/Cpackets/CGSelectRegenZone.h"
#include "Packet/Cpackets/CGStoreClose.h"
#include "Packet/Cpackets/CGStoreOpen.h"
#include "Packet/Cpackets/CGStoreSign.h"
#include "Packet/Cpackets/CGUndisplayItem.h"
#include "Packet/Cpackets/CGWithdrawPet.h"

// Stub implementations for store-related handlers
void CGBuyStoreItemHandler::execute(CGBuyStoreItem* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGConnectSetKeyHandler::execute(CGConnectSetKey* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGDisplayItemHandler::execute(CGDisplayItem* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGDonationMoneyHandler::execute(CGDonationMoney* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGLotterySelectHandler::execute(CGLotterySelect* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGMixItemHandler::execute(CGMixItem* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGRequestStoreInfoHandler::execute(CGRequestStoreInfo* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGSelectQuestHandler::execute(CGSelectQuest* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGSelectRegenZoneHandler::execute(CGSelectRegenZone* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGStoreCloseHandler::execute(CGStoreClose* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGStoreOpenHandler::execute(CGStoreOpen* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGStoreSignHandler::execute(CGStoreSign* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGUndisplayItemHandler::execute(CGUndisplayItem* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
void CGWithdrawPetHandler::execute(CGWithdrawPet* pPacket, Player* pPlayer) throw(ProtocolException, Error) {}
