//----------------------------------------------------------------------
// PacketDef.h
//----------------------------------------------------------------------
// Client에서 사용하는 Packet에 대한 정의
//----------------------------------------------------------------------
#ifndef	__PACKETDEF_H__
#define	__PACKETDEF_H__

//-----------------------------------------------------------------------------
// Login
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CLVersionCheck.h"
#include "Packet\Cpackets\CLLogout.h"
#include "Packet\Cpackets\CLLogin.h"
#include "Packet\Cpackets\CLRegisterPlayer.h"
#include "Packet\Cpackets\CLGetPCList.h"
#include "Packet\Cpackets\CLCreatePC.h"
#include "Packet\Cpackets\CLDeletePC.h"
#include "Packet\Cpackets\CLSelectPC.h"
#include "Packet\Cpackets\CLQueryPlayerID.h"

//-----------------------------------------------------------------------------
// core
//-----------------------------------------------------------------------------
#include "Packet\PacketFactoryManager.h"
#include "Packet\PacketValidator.h"
#include "Packet\Properties.h"
#include "Packet\ClientPlayer.h"
#include "Packet\Socket.h"

//-----------------------------------------------------------------------------
// connect
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGConnect.h"
#include "Packet\Cpackets\CGReady.h"
#include "Packet\Cpackets\CGMove.h"
#include "Packet\Cpackets\CGLogout.h"

//-----------------------------------------------------------------------------
// Phone / Whisper
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGDialUp.h"
#include "Packet\Cpackets\CGPhoneDisconnect.h"
#include "Packet\Cpackets\CGPhoneSay.h"
#include "Packet\Cpackets\CGWhisper.h"

//-----------------------------------------------------------------------------
// 채팅
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGSay.h"
#include "Packet\Cpackets\CGGlobalChat.h"


//-----------------------------------------------------------------------------
// 기술 사용
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGAttack.h"
#include "Packet\Cpackets\CGCastingSkill.h"
#include "Packet\Cpackets\CGSkillToInventory.h"
#include "Packet\Cpackets\CGSkillToSelf.h"
#include "Packet\Cpackets\CGSkillToTile.h"
#include "Packet\Cpackets\CGSkillToObject.h"
#include "Packet\Cpackets\CGUnburrow.h"
#include "Packet\Cpackets\CGBloodDrain.h"
// 2001.4.23에 추가
#include "Packet\Cpackets\CGThrowBomb.h"
#include "Packet\Cpackets\CGUntransform.h"
#include "Packet\Cpackets\CGVisible.h"
#include "Packet\Cpackets\CGAbsorbSoul.h"

//-----------------------------------------------------------------------------
// skill 배움 / skill 단축키
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGLearnSkill.h"
#include "Packet\Cpackets\CGUseBonusPoint.h"		// for vampire lev up
#include "Packet\Cpackets\CGSetSlayerHotKey.h"
#include "Packet\Cpackets\CGSetVampireHotKey.h"


//-----------------------------------------------------------------------------
// Item 
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGAddGearToMouse.h"
#include "Packet\Cpackets\CGAddInventoryToMouse.h"
#include "Packet\Cpackets\CGAddMouseToGear.h"
#include "Packet\Cpackets\CGAddMouseToInventory.h"
#include "Packet\Cpackets\CGAddMouseToQuickSlot.h"
#include "Packet\Cpackets\CGAddMouseToZone.h"
#include "Packet\Cpackets\CGAddQuickSlotToMouse.h"
#include "Packet\Cpackets\CGAddZoneToInventory.h"
#include "Packet\Cpackets\CGAddZoneToMouse.h"
#include "Packet\Cpackets\CGPickupMoney.h"
#include "Packet\Cpackets\CGDropMoney.h"
#include "Packet\Cpackets\CGRequestRepair.h"
#include "Packet\Cpackets\CGMakeItem.h"

//-----------------------------------------------------------------------------
// 시체 click
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGDissectionCorpse.h"

// 탄창
#include "Packet\Cpackets\CGReloadFromInventory.h"
#include "Packet\Cpackets\CGReloadFromQuickSlot.h"

// 오토바이
#include "Packet\Cpackets\CGGetOffMotorCycle.h"
#include "Packet\Cpackets\CGRideMotorCycle.h"

// 던지기
#include "Packet\Cpackets\CGThrowItem.h"

// potion
#include "Packet\Cpackets\CGUsePotionFromInventory.h"
#include "Packet\Cpackets\CGUsePotionFromQuickSlot.h"

#include "Packet\Cpackets\CGUseItemFromInventory.h"

//-----------------------------------------------------------------------------
// NPC
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGNPCTalk.h"
#include "Packet\Cpackets\CGNPCAskAnswer.h"

//-----------------------------------------------------------------------------
// 상점
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGShopRequestList.h"
#include "Packet\Cpackets\CGShopRequestBuy.h"
#include "Packet\Cpackets\CGShopRequestSell.h"

//-----------------------------------------------------------------------------
// 보관
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGStashRequestBuy.h"
#include "Packet\Cpackets\CGStashList.h"
#include "Packet\Cpackets\CGMouseToStash.h"
#include "Packet\Cpackets\CGStashToMouse.h"
#include "Packet\Cpackets\CGStashDeposit.h"
#include "Packet\Cpackets\CGStashWithdraw.h"

//-----------------------------------------------------------------------------
// 교환
//-----------------------------------------------------------------------------
#include "Packet\Cpackets\CGTradePrepare.h"
#include "Packet\Cpackets\CGTradeAddItem.h"
#include "Packet\Cpackets\CGTradeRemoveItem.h"
#include "Packet\Cpackets\CGTradeMoney.h"
#include "Packet\Cpackets\CGTradeFinish.h"





#endif