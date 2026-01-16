//////////////////////////////////////////////////////////////////////////////
// Filename    : CGRangerSayHandler.cpp
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGRangerSay.h"

#ifdef __GAME_SERVER__
//	#include "GamePlayer.h"
//	#include "Creature.h"
//	#include "PCFinder.h"
//	#include "LogNameManager.h"
//
//	#include "Gpackets/GCWhisper.h"
//	#include "Gpackets/GCWhisperFailed.h"
//	#include "Gpackets/GGServerChat.h"
//
//	#include "DB.h"
//
//	#include "GameServerInfoManager.h"
//	#include "LoginServerManager.h"
//	#include "Properties.h"

#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
void CGRangerSayHandler::execute (CGRangerSay* pPacket , Player* pPlayer)
	 throw (ProtocolException , Error)
{
}
#endif
