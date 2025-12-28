//--------------------------------------------------------------------------------
// 
// Filename    : RequestServerPacketFactoryManager.cpp 
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "Assert.h"
#include "RequestServerPacketFactoryManager.h"
#include "StringStream.h"

#include "Cpackets/CGAttack.h"
#include "Cpackets/CGAddGearToMouse.h"
#include "Cpackets/CGAddInventoryToMouse.h"
#include "Cpackets/CGAddMouseToGear.h"
#include "Cpackets/CGAddMouseToInventory.h"
#include "Cpackets/CGAddMouseToQuickSlot.h"
#include "Cpackets/CGAddMouseToZone.h"
#include "Cpackets/CGAddQuickSlotToMouse.h"
#include "Cpackets/CGAddZoneToMouse.h"
#include "Cpackets/CGAddZoneToInventory.h"
#include "Cpackets/CGBloodDrain.h"
#include "Cpackets/CGUnburrow.h"
#include "Cpackets/CGUntransform.h"
#include "Cpackets/CGCastingSkill.h"
#include "Cpackets/CGConnect.h"
#include "Cpackets/CGDissectionCorpse.h"
#include "Cpackets/CGDropMoney.h"
#include "Cpackets/CGGetOffMotorCycle.h"
#include "Cpackets/CGGlobalChat.h"
#include "Cpackets/CGLearnSkill.h"
#include "Cpackets/CGLogout.h"
#include "Cpackets/CGMakeItem.h"
#include "Cpackets/CGMove.h"
#include "Cpackets/CGNPCAskAnswer.h"
#include "Cpackets/CGNPCTalk.h"
#include "Cpackets/CGPickupMoney.h"
#include "Cpackets/CGReady.h"
#include "Cpackets/CGReloadFromInventory.h"
#include "Cpackets/CGReloadFromQuickSlot.h"
#include "Cpackets/CGRideMotorCycle.h"
#include "Cpackets/CGSay.h"
#include "Cpackets/CGShopRequestBuy.h"
#include "Cpackets/CGShopRequestList.h"
#include "Cpackets/CGShopRequestSell.h"
#include "Cpackets/CGSkillToInventory.h"
#include "Cpackets/CGThrowBomb.h"
#include "Gpackets/GCThrowBombOK1.h"
#include "Gpackets/GCThrowBombOK2.h"
#include "Gpackets/GCThrowBombOK3.h"
#include "Cpackets/CGThrowItem.h"
#include "Cpackets/CGUseBonusPoint.h"
#include "Cpackets/CGUsePotionFromInventory.h"
#include "Cpackets/CGUsePotionFromQuickSlot.h"
#include "Cpackets/CGRequestRepair.h"
#include "Cpackets/CGVisible.h"
#include "Cpackets/CGVerifyTime.h"
#include "Cpackets/CGRequestNewbieItem.h"

#include "Cpackets/CLCreatePC.h"
#include "Cpackets/CLDeletePC.h"
#include "Cpackets/CLGetPCList.h"
#include "Cpackets/CLLogin.h"
#include "Cpackets/CLLogout.h"
#include "Cpackets/CLQueryPlayerID.h"
#include "Cpackets/CLRegisterPlayer.h"
#include "Cpackets/CLSelectPC.h"
#include "Cpackets/CLVersionCheck.h"
#include "Cpackets/CLGetServerList.h"
#include "Cpackets/CLChangeServer.h"
#include "Cpackets/CLReconnectLogin.h"

#include "Gpackets/GCAddBat.h"
#include "Gpackets/GCAddBurrowingCreature.h"
#include "Gpackets/GCAddEffect.h"
#include "Gpackets/GCAddEffectToTile.h"
#include "Gpackets/GCAddGearToInventory.h"
#include "Gpackets/GCAddGearToZone.h"
#include "Gpackets/GCAddInstalledMineToZone.h"
#include "Gpackets/GCAddMonster.h"
#include "Gpackets/GCAddMonsterCorpse.h"
#include "Gpackets/GCAddMonsterFromBurrowing.h"
#include "Gpackets/GCAddMonsterFromTransformation.h"
#include "Gpackets/GCAddNewItemToZone.h"
#include "Gpackets/GCAddNPC.h"
#include "Gpackets/GCAddSlayer.h"
#include "Gpackets/GCAddSlayerCorpse.h"
#include "Gpackets/GCAddVampire.h"
#include "Gpackets/GCAddVampireCorpse.h"
#include "Gpackets/GCAddVampireFromBurrowing.h"
#include "Gpackets/GCAddVampireFromTransformation.h"
#include "Gpackets/GCAddWolf.h"
#include "Gpackets/GCAttackArmsOK1.h"
#include "Gpackets/GCAttackArmsOK2.h"
#include "Gpackets/GCAttackArmsOK3.h"
#include "Gpackets/GCAttackArmsOK4.h"
#include "Gpackets/GCAttackArmsOK5.h"
#include "Gpackets/GCAttackMeleeOK1.h"
#include "Gpackets/GCAttackMeleeOK2.h"
#include "Gpackets/GCAttackMeleeOK3.h"
#include "Gpackets/GCAttack.h"
#include "Gpackets/GCBloodDrainOK1.h"
#include "Gpackets/GCBloodDrainOK2.h"
#include "Gpackets/GCBloodDrainOK3.h"
#include "Gpackets/GCCannotAdd.h"
#include "Gpackets/GCCannotUse.h"
#include "Gpackets/GCCastingSkill.h"
#include "Gpackets/GCCreateItem.h"
#include "Gpackets/GCCreatureDied.h"
#include "Gpackets/GCChangeShape.h"
#include "Gpackets/GCChangeDarkLight.h"
#include "Gpackets/GCChangeWeather.h"
#include "Gpackets/GCCrossCounterOK1.h"
#include "Gpackets/GCCrossCounterOK2.h"
#include "Gpackets/GCCrossCounterOK3.h"
#include "Gpackets/GCDeleteInventoryItem.h"
#include "Gpackets/GCDeleteObject.h"
#include "Gpackets/GCRemoveCorpseHead.h"
#include "Gpackets/GCDeleteandPickUpOK.h"
#include "Gpackets/GCDeleteEffectFromTile.h"
#include "Gpackets/GCDisconnect.h"
#include "Gpackets/GCDropItemToZone.h"
#include "Gpackets/GCFastMove.h"
#include "Gpackets/GCGetDamage.h"
#include "Gpackets/GCGetOffMotorCycle.h"
#include "Gpackets/GCGetOffMotorCycleFailed.h"
#include "Gpackets/GCGetOffMotorCycleOK.h"
#include "Gpackets/GCGlobalChat.h"

#include "Gpackets/GCHPRecoveryStartToSelf.h"
#include "Gpackets/GCHPRecoveryStartToOthers.h"
#include "Gpackets/GCHPRecoveryEndToSelf.h"
#include "Gpackets/GCHPRecoveryEndToOthers.h"

#include "Gpackets/GCKnockBack.h"
#include "Gpackets/GCKnocksTargetBackOK1.h"
#include "Gpackets/GCKnocksTargetBackOK2.h"
#include "Gpackets/GCKnocksTargetBackOK4.h"
#include "Gpackets/GCKnocksTargetBackOK5.h"
#include "Gpackets/GCLearnSkillFailed.h"
#include "Gpackets/GCLearnSkillOK.h"
#include "Gpackets/GCLearnSkillReady.h"
#include "Gpackets/GCLightning.h"
#include "Gpackets/GCMakeItemOK.h"
#include "Gpackets/GCMakeItemFail.h"
#include "Gpackets/GCMineExplosionOK1.h"
#include "Gpackets/GCMineExplosionOK2.h"
#include "Gpackets/GCModifyInformation.h"
#include "Gpackets/GCMorph1.h"
#include "Gpackets/GCMorphSlayer2.h"
#include "Gpackets/GCMorphVampire2.h"
#include "Gpackets/GCMove.h"
#include "Gpackets/GCMoveOK.h"
#include "Gpackets/GCMoveError.h"

#include "Gpackets/GCMPRecoveryStart.h"
#include "Gpackets/GCMPRecoveryEnd.h"

#include "Gpackets/GCNPCAsk.h"
#include "Gpackets/GCNPCSay.h"
#include "Gpackets/GCRealWearingInfo.h"
#include "Gpackets/GCReconnect.h"
#include "Gpackets/GCReconnectLogin.h"
#include "Gpackets/GCRemoveEffect.h"
#include "Gpackets/GCRemoveFromGear.h"
#include "Gpackets/GCRideMotorCycle.h"
#include "Gpackets/GCRideMotorCycleFailed.h"
#include "Gpackets/GCRideMotorCycleOK.h"
#include "Gpackets/GCReloadOK.h"
#include "Gpackets/GCSay.h"
#include "Gpackets/GCSearchMotorcycleOK.h"
#include "Gpackets/GCSearchMotorcycleFail.h"
#include "Gpackets/GCSetPosition.h"
// Shop Interface
#include "Gpackets/GCShopBuyOK.h"
#include "Gpackets/GCShopBuyFail.h"
#include "Gpackets/GCShopList.h"
#include "Gpackets/GCShopListMysterious.h"
#include "Gpackets/GCShopSold.h"
#include "Gpackets/GCShopVersion.h"
#include "Gpackets/GCShopBought.h"
#include "Gpackets/GCShopSellOK.h"
#include "Gpackets/GCShopSellFail.h"
#include "Gpackets/GCShopMarketCondition.h"

#include "Gpackets/GCSkillFailed1.h"
#include "Gpackets/GCSkillFailed2.h"
#include "Gpackets/GCSkillToInventoryOK1.h"
#include "Gpackets/GCSkillToInventoryOK2.h"
#include "Gpackets/GCSkillToObjectOK1.h"
#include "Gpackets/GCSkillToObjectOK2.h"
#include "Gpackets/GCSkillToObjectOK3.h"
#include "Gpackets/GCSkillToObjectOK4.h"
#include "Gpackets/GCSkillToObjectOK5.h"
#include "Gpackets/GCSkillToObjectOK6.h"
#include "Gpackets/GCSkillToSelfOK1.h"
#include "Gpackets/GCSkillToSelfOK2.h"
#include "Gpackets/GCSkillToSelfOK3.h"
#include "Gpackets/GCSkillToTileOK1.h"
#include "Gpackets/GCSkillToTileOK2.h"
#include "Gpackets/GCSkillToTileOK3.h"
#include "Gpackets/GCSkillToTileOK4.h"
#include "Gpackets/GCSkillToTileOK5.h"
#include "Gpackets/GCSkillToTileOK6.h"
#include "Gpackets/GCSystemMessage.h"
#include "Gpackets/GCUnburrowOK.h"
#include "Gpackets/GCUnburrowFail.h"
#include "Gpackets/GCUntransformOK.h"
#include "Gpackets/GCUntransformFail.h"
#include "Gpackets/GCTeachSkillInfo.h"
#include "Gpackets/GCUpdateInfo.h"
#include "Gpackets/GCUseBonusPointFail.h"
#include "Gpackets/GCUseBonusPointOK.h"
#include "Gpackets/GCUseOK.h"
#include "Gpackets/GCTakeOff.h"
#include "Gpackets/GCThrowItemOK1.h"
#include "Gpackets/GCThrowItemOK2.h"
#include "Gpackets/GCThrowItemOK3.h"
#include "Gpackets/GCVisibleOK.h"
#include "Gpackets/GCVisibleFail.h"

#ifndef __GAME_CLIENT__
	#include "Gpackets/GLIncomingConnection.h"
	#include "Gpackets/GLIncomingConnectionError.h"
	#include "Gpackets/GLIncomingConnectionOK.h"
#endif

#include "Lpackets/LCCreatePCError.h"
#include "Lpackets/LCCreatePCOK.h"
#include "Lpackets/LCDeletePCError.h"
#include "Lpackets/LCDeletePCOK.h"
#include "Lpackets/LCLoginError.h"
#include "Lpackets/LCLoginOK.h"
#include "Lpackets/LCPCList.h"
#include "Lpackets/LCQueryResultPlayerID.h"
#include "Lpackets/LCReconnect.h"
#include "Lpackets/LCRegisterPlayerError.h"
#include "Lpackets/LCRegisterPlayerOK.h"
#include "Lpackets/LCSelectPCError.h"

#ifndef __GAME_CLIENT__
	#include "Lpackets/LGIncomingConnection.h"
	#include "Lpackets/LGIncomingConnectionOK.h"
	#include "Lpackets/LGIncomingConnectionError.h"
#endif

#include "Lpackets/LCVersionCheckOK.h"
#include "Lpackets/LCVersionCheckError.h"
#include "Lpackets/LCServerList.h"

// added by elcastle 2000-11-29
//#include "Cpackets/CGDialUp.h"
//#include "Cpackets/CGPhoneDisconnect.h"
//#include "Cpackets/CGPhoneSay.h"
#include "Cpackets/CGWhisper.h"

//#include "Gpackets/GCPhoneConnected.h"
//#include "Gpackets/GCRing.h"
//#include "Gpackets/GCPhoneDisconnected.h"
//#include "Gpackets/GCPhoneConnectionFailed.h"
//#include "Gpackets/GCPhoneSay.h"
#include "Gpackets/GCWhisper.h"
#include "Gpackets/GCWhisperFailed.h"

// added by elca 2000-11-29
#include "Gpackets/GCSkillInfo.h"

// added by elca 2000-12-09
#include "Gpackets/GCStatusCurrentHP.h"

// added by elca 2000-12-09
#include "Cpackets/CGSetSlayerHotKey.h"
#include "Cpackets/CGSetVampireHotKey.h"


// added by elca 2001-06-26
#include "Cpackets/CGSelectPortal.h"

// 2001-01-08 김성민
#include "Cpackets/CGMouseToStash.h"
#include "Cpackets/CGStashToMouse.h"
#include "Cpackets/CGStashList.h"
#include "Gpackets/GCStashList.h"
#include "Cpackets/CGStashDeposit.h"
#include "Cpackets/CGStashWithdraw.h"
#include "Cpackets/CGStashRequestBuy.h"
#include "Gpackets/GCStashSell.h"

#include "Cpackets/CGTradePrepare.h"
#include "Cpackets/CGTradeAddItem.h"
#include "Cpackets/CGTradeRemoveItem.h"
#include "Cpackets/CGTradeMoney.h"
#include "Cpackets/CGTradeFinish.h"
#include "Gpackets/GCTradePrepare.h"
#include "Gpackets/GCTradeAddItem.h"
#include "Gpackets/GCTradeRemoveItem.h"
#include "Gpackets/GCTradeMoney.h"
#include "Gpackets/GCTradeFinish.h"
#include "Gpackets/GCTradeError.h"
#include "Gpackets/GCTradeVerify.h"

#include "Cpackets/CGSkillToObject.h"
#include "Cpackets/CGSkillToSelf.h"
#include "Cpackets/CGSkillToTile.h"

#include "Cpackets/CGSilverCoating.h"
#include "Gpackets/GCNPCResponse.h"

#include "Gpackets/GCNPCSayDynamic.h"
#include "Gpackets/GCNPCAskDynamic.h"

#include "Cpackets/CGUseItemFromInventory.h"
#include "Cpackets/CGSelectWayPoint.h"
#include "Gpackets/GCAddHelicopter.h"
#include "Gpackets/GCAddVampirePortal.h"
#include "Gpackets/GCEnterVampirePortal.h"

#include "Cpackets/CGSelectTileEffect.h"

#include "Cpackets/CGPartyInvite.h"
#include "Cpackets/CGPartyLeave.h"

#include "Gpackets/GCPartyInvite.h"
#include "Gpackets/GCPartyLeave.h"
#include "Gpackets/GCPartyJoined.h"
#include "Gpackets/GCPartyError.h"

#include "Cpackets/CGResurrect.h"

//////////////////////////////////////////////////////////////////////
//
// constructor
//
//////////////////////////////////////////////////////////////////////
RequestServerPacketFactoryManager::RequestServerPacketFactoryManager () 
	throw ()
: m_Factories(NULL) , m_Size(Packet::PACKET_MAX)
{
	__BEGIN_TRY

	Assert(m_Size > 0);
	
	// 패킷팩토리배열을 생성한다.
	m_Factories = new PacketFactory*[ m_Size ];
	
	// 팩토리에 대한 포인터들을 NULL 로 초기화한다.
	for (int i = 0 ; i < m_Size ; i ++) 
		m_Factories[i] = NULL;
			
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
//
// destructor
//
//////////////////////////////////////////////////////////////////////
RequestServerPacketFactoryManager::~RequestServerPacketFactoryManager () 
	throw ()
{
	__BEGIN_TRY
		
	Assert(m_Factories != NULL);

	// 각각의 패킷팩토리들을 삭제한다.
	for (int i = 0 ; i < m_Size ; i ++) 
	{
#ifdef __GAME_CLIENT__
		if (m_Factories[i] != NULL)
		{
			delete m_Factories[i];
			m_Factories[i] = NULL;
		}
#else
		SAFE_DELETE(m_Factories[i]);
#endif
	}
	
	// 패킷팩토리배열을 삭제한다.
#ifdef __GAME_CLIENT__
	if (m_Factories != NULL)
	{
		delete [] m_Factories;
		m_Factories = NULL;
	}
#else
	SAFE_DELETE_ARRAY(m_Factories);
#endif
			
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// 정의된 모든 패킷팩토리들을 여기에 추가한다.
//
//////////////////////////////////////////////////////////////////////
void RequestServerPacketFactoryManager::init ()
	 throw (Error)
{
	__BEGIN_TRY
		
#ifndef __GAME_CLIENT__
	addFactory(new CGAttackFactory());
	addFactory(new CGAddGearToMouseFactory());
	addFactory(new CGAddInventoryToMouseFactory());
	addFactory(new CGAddMouseToGearFactory());
	addFactory(new CGAddMouseToInventoryFactory());
	addFactory(new CGAddMouseToQuickSlotFactory());
	addFactory(new CGAddMouseToZoneFactory());
	addFactory(new CGAddQuickSlotToMouseFactory());
	addFactory(new CGAddZoneToInventoryFactory());
	addFactory(new CGAddZoneToMouseFactory());
	addFactory(new CGBloodDrainFactory());
	addFactory(new CGCastingSkillFactory());
	addFactory(new CGConnectFactory());
	addFactory(new CGDissectionCorpseFactory());
	addFactory(new CGDropMoneyFactory());
	addFactory(new CGGetOffMotorCycleFactory());
	addFactory(new CGGlobalChatFactory());
	addFactory(new CGLearnSkillFactory());
	addFactory(new CGLogoutFactory());
	addFactory(new CGMakeItemFactory());
	addFactory(new CGMoveFactory());
	addFactory(new CGNPCAskAnswerFactory());
	addFactory(new CGNPCTalkFactory());
	addFactory(new CGPickupMoneyFactory());
	addFactory(new CGReadyFactory());
	addFactory(new CGReloadFromInventoryFactory());
	addFactory(new CGReloadFromQuickSlotFactory());
	addFactory(new CGRideMotorCycleFactory());
	addFactory(new CGSayFactory());
	addFactory(new CGSetSlayerHotKeyFactory());
	addFactory(new CGSetVampireHotKeyFactory());
	addFactory(new CGSelectPortalFactory());
	addFactory(new CGShopRequestBuyFactory());
	addFactory(new CGShopRequestListFactory());
	addFactory(new CGShopRequestSellFactory());
	addFactory(new CGSkillToInventoryFactory());
	addFactory(new CGThrowBombFactory());
	addFactory(new GCThrowBombOK1Factory());
	addFactory(new GCThrowBombOK2Factory());
	addFactory(new GCThrowBombOK3Factory());
	addFactory(new CGThrowItemFactory());
	addFactory(new CGUnburrowFactory());
	addFactory(new CGUntransformFactory());
	addFactory(new CGUseBonusPointFactory());
	addFactory(new CGUsePotionFromInventoryFactory());
	addFactory(new CGUsePotionFromQuickSlotFactory());
	addFactory(new CGRequestRepairFactory());
	addFactory(new CGVisibleFactory());
	addFactory(new CGVerifyTimeFactory());

	addFactory(new CLCreatePCFactory());
	addFactory(new CLDeletePCFactory());
	addFactory(new CLGetPCListFactory());
	addFactory(new CLLoginFactory());
	addFactory(new CLLogoutFactory());
	addFactory(new CLQueryPlayerIDFactory());
	addFactory(new CLRegisterPlayerFactory);
	addFactory(new CLSelectPCFactory());
	addFactory(new CLVersionCheckFactory());
	addFactory(new CLGetServerListFactory());
	addFactory(new CLChangeServerFactory());
	addFactory(new CLReconnectLoginFactory());

	// added by elcastle 2000-11-29
	/*
	addFactory(new CGDialUpFactory());
	addFactory(new CGPhoneDisconnectFactory());
	addFactory(new CGPhoneSayFactory());
	*/
	addFactory(new CGWhisperFactory());

	addFactory(new CGMouseToStashFactory());
	addFactory(new CGStashToMouseFactory());
	addFactory(new CGStashListFactory());
	addFactory(new CGStashDepositFactory());
	addFactory(new CGStashWithdrawFactory());

	addFactory(new CGStashRequestBuyFactory());

	addFactory(new CGTradePrepareFactory());
	addFactory(new CGTradeAddItemFactory());
	addFactory(new CGTradeRemoveItemFactory());
	addFactory(new CGTradeMoneyFactory());
	addFactory(new CGTradeFinishFactory());

	addFactory(new CGSkillToObjectFactory());
	addFactory(new CGSkillToSelfFactory());
	addFactory(new CGSkillToTileFactory());

	addFactory(new CGSilverCoatingFactory());
	addFactory(new CGRequestNewbieItemFactory());
	addFactory(new CGUseItemFromInventoryFactory());
	addFactory(new CGSelectWayPointFactory());
	addFactory(new CGSelectTileEffectFactory());

	addFactory(new CGPartyInviteFactory());
	addFactory(new CGPartyLeaveFactory());

	addFactory(new CGResurrectFactory());

#endif
	
	addFactory(new GCAddBatFactory());
	addFactory(new GCAddBurrowingCreatureFactory());
	addFactory(new GCAddEffectFactory());
	addFactory(new GCAddEffectToTileFactory());
	addFactory(new GCAddGearToInventoryFactory());
	addFactory(new GCAddGearToZoneFactory());
	addFactory(new GCAddInstalledMineToZoneFactory());
	addFactory(new GCAddMonsterFactory());
	addFactory(new GCAddMonsterCorpseFactory());
	addFactory(new GCAddMonsterFromBurrowingFactory());
	addFactory(new GCAddMonsterFromTransformationFactory());
	addFactory(new GCAddNewItemToZoneFactory());
	addFactory(new GCAddNPCFactory());
	addFactory(new GCAddSlayerFactory());
	addFactory(new GCAddSlayerCorpseFactory());
	addFactory(new GCAddVampireFactory());
	addFactory(new GCAddVampireCorpseFactory());
	addFactory(new GCAddVampireFromBurrowingFactory());
	addFactory(new GCAddVampireFromTransformationFactory());
	addFactory(new GCAddWolfFactory());
    addFactory(new GCAttackFactory());
    addFactory(new GCAttackArmsOK1Factory());
    addFactory(new GCAttackArmsOK2Factory());
    addFactory(new GCAttackArmsOK3Factory());
    addFactory(new GCAttackArmsOK4Factory());
    addFactory(new GCAttackArmsOK5Factory());
    addFactory(new GCAttackMeleeOK1Factory());
    addFactory(new GCAttackMeleeOK2Factory());
    addFactory(new GCAttackMeleeOK3Factory());
	addFactory(new GCBloodDrainOK1Factory());
	addFactory(new GCBloodDrainOK2Factory());
	addFactory(new GCBloodDrainOK3Factory());
    addFactory(new GCCastingSkillFactory());
    addFactory(new GCCannotAddFactory());
    addFactory(new GCCannotUseFactory());
    addFactory(new GCChangeShapeFactory());
    addFactory(new GCChangeDarkLightFactory());
    addFactory(new GCChangeWeatherFactory());
    addFactory(new GCCreateItemFactory());
    addFactory(new GCCreatureDiedFactory());
    addFactory(new GCCrossCounterOK1Factory());
    addFactory(new GCCrossCounterOK2Factory());
    addFactory(new GCCrossCounterOK3Factory());
	addFactory(new GCDeleteandPickUpOKFactory());
	addFactory(new GCDeleteInventoryItemFactory());
	addFactory(new GCDeleteObjectFactory());
	addFactory(new GCRemoveCorpseHeadFactory());
	addFactory(new GCDeleteEffectFromTileFactory());
	addFactory(new GCDisconnectFactory());
	addFactory(new GCDropItemToZoneFactory());
	addFactory(new GCFastMoveFactory());
	addFactory(new GCGetDamageFactory());
	addFactory(new GCGetOffMotorCycleFactory());
	addFactory(new GCGetOffMotorCycleOKFactory());
	addFactory(new GCGetOffMotorCycleFailedFactory());
	addFactory(new GCGlobalChatFactory());
	addFactory(new GCHPRecoveryStartToSelfFactory());
	addFactory(new GCHPRecoveryStartToOthersFactory());
	addFactory(new GCHPRecoveryEndToSelfFactory());
	addFactory(new GCHPRecoveryEndToOthersFactory());
 	addFactory(new GCLightningFactory());
	addFactory(new GCLearnSkillFailedFactory());
	addFactory(new GCLearnSkillOKFactory());
	addFactory(new GCLearnSkillReadyFactory());
	addFactory(new GCKnockBackFactory());
	addFactory(new GCKnocksTargetBackOK1Factory());
	addFactory(new GCKnocksTargetBackOK2Factory());
	addFactory(new GCKnocksTargetBackOK4Factory());
	addFactory(new GCKnocksTargetBackOK5Factory());
	addFactory(new GCMakeItemOKFactory());
	addFactory(new GCMakeItemFailFactory());
	addFactory(new GCMineExplosionOK1Factory());
	addFactory(new GCMineExplosionOK2Factory());
	addFactory(new GCModifyInformationFactory());
	addFactory(new GCMorph1Factory());
	addFactory(new GCMorphSlayer2Factory());
	addFactory(new GCMorphVampire2Factory());
	addFactory(new GCMoveFactory());
	addFactory(new GCMoveErrorFactory());
	addFactory(new GCMoveOKFactory());
	addFactory(new GCMPRecoveryStartFactory());
	addFactory(new GCMPRecoveryEndFactory());
	addFactory(new GCNPCAskFactory());
	addFactory(new GCNPCSayFactory());
	addFactory(new GCRealWearingInfoFactory());
	addFactory(new GCReconnectFactory());
	addFactory(new GCReconnectLoginFactory());
	addFactory(new GCRemoveEffectFactory());
	addFactory(new GCRemoveFromGearFactory());
	addFactory(new GCRideMotorCycleFactory());
	addFactory(new GCRideMotorCycleOKFactory());
	addFactory(new GCRideMotorCycleFailedFactory());
	addFactory(new GCReloadOKFactory());
	addFactory(new GCUnburrowOKFactory());
	addFactory(new GCUnburrowFailFactory());
	addFactory(new GCUntransformOKFactory());
	addFactory(new GCUntransformFailFactory());
	addFactory(new GCUseBonusPointFailFactory());
	addFactory(new GCUseBonusPointOKFactory());
	addFactory(new GCSayFactory());
	addFactory(new GCSearchMotorcycleOKFactory());
	addFactory(new GCSearchMotorcycleFailFactory());
	addFactory(new GCSetPositionFactory());
	// shop interface
	addFactory(new GCShopVersionFactory());
	addFactory(new GCShopListFactory());
	addFactory(new GCShopListMysteriousFactory());
	addFactory(new GCShopBuyOKFactory());
	addFactory(new GCShopBuyFailFactory());
	addFactory(new GCShopSoldFactory());
	addFactory(new GCShopSellFailFactory());
	addFactory(new GCShopSellOKFactory());
	addFactory(new GCShopBoughtFactory());
	addFactory(new GCShopMarketConditionFactory());

	addFactory(new GCSkillFailed1Factory());
	addFactory(new GCSkillFailed2Factory());
	addFactory(new GCSkillToInventoryOK1Factory());
	addFactory(new GCSkillToInventoryOK2Factory());
	addFactory(new GCSkillToObjectOK1Factory());
	addFactory(new GCSkillToObjectOK2Factory());
	addFactory(new GCSkillToObjectOK3Factory());
	addFactory(new GCSkillToObjectOK4Factory());
	addFactory(new GCSkillToObjectOK5Factory());
	addFactory(new GCSkillToObjectOK6Factory());
	addFactory(new GCSkillToSelfOK1Factory());
	addFactory(new GCSkillToSelfOK2Factory());
	addFactory(new GCSkillToSelfOK3Factory());
	addFactory(new GCSkillToTileOK1Factory());
	addFactory(new GCSkillToTileOK2Factory());
	addFactory(new GCSkillToTileOK3Factory());
	addFactory(new GCSkillToTileOK4Factory());
	addFactory(new GCSkillToTileOK5Factory());
	addFactory(new GCSkillToTileOK6Factory());
	addFactory(new GCSystemMessageFactory());
	addFactory(new GCTakeOffFactory());
	addFactory(new GCTeachSkillInfoFactory());
	addFactory(new GCThrowItemOK1Factory());
	addFactory(new GCThrowItemOK2Factory());
	addFactory(new GCThrowItemOK3Factory());
	addFactory(new GCUpdateInfoFactory());
	addFactory(new GCUseOKFactory());

	// Add by elcastle 2000-11-29
//	addFactory(new GCPhoneConnectedFactory());
//	addFactory(new GCRingFactory());
//	addFactory(new GCPhoneDisconnectedFactory());
//	addFactory(new GCPhoneConnectionFailedFactory());
//	addFactory(new GCPhoneSayFactory());
	addFactory(new GCWhisperFactory());
	addFactory(new GCWhisperFailedFactory());

	// Add by elcastle 2000-12-2
	addFactory(new GCSkillInfoFactory());

	// Add by elcastle 2000-12-9
	addFactory(new GCStatusCurrentHPFactory());

    addFactory(new GCStashListFactory());
    addFactory(new GCStashSellFactory());

	addFactory(new GCTradePrepareFactory());
	addFactory(new GCTradeAddItemFactory());
	addFactory(new GCTradeRemoveItemFactory());
	addFactory(new GCTradeMoneyFactory());
	addFactory(new GCTradeFinishFactory());
	addFactory(new GCTradeErrorFactory());
	addFactory(new GCTradeVerifyFactory());
	addFactory(new GCVisibleOKFactory());
	addFactory(new GCVisibleFailFactory());

	addFactory(new GCNPCResponseFactory());

	addFactory(new GCNPCSayDynamicFactory());
	addFactory(new GCNPCAskDynamicFactory());

	addFactory(new GCAddHelicopterFactory());
	addFactory(new GCAddVampirePortalFactory());
	addFactory(new GCEnterVampirePortalFactory());

	addFactory(new GCPartyInviteFactory());
	addFactory(new GCPartyLeaveFactory());
	addFactory(new GCPartyJoinedFactory());
	addFactory(new GCPartyErrorFactory());

	#ifndef __GAME_CLIENT__
		addFactory(new GLIncomingConnectionFactory());
		addFactory(new GLIncomingConnectionErrorFactory());
		addFactory(new GLIncomingConnectionOKFactory());
	#endif

	addFactory(new LCCreatePCErrorFactory());
	addFactory(new LCCreatePCOKFactory());
	addFactory(new LCDeletePCErrorFactory());
	addFactory(new LCDeletePCOKFactory());
	addFactory(new LCLoginErrorFactory());
	addFactory(new LCLoginOKFactory());
	addFactory(new LCPCListFactory());
	addFactory(new LCQueryResultPlayerIDFactory());
	addFactory(new LCReconnectFactory());
	addFactory(new LCRegisterPlayerErrorFactory());
	addFactory(new LCRegisterPlayerOKFactory());
	addFactory(new LCSelectPCErrorFactory());
	addFactory(new LCVersionCheckOKFactory());
	addFactory(new LCVersionCheckErrorFactory());
	addFactory(new LCServerListFactory());

	#ifndef __GAME_CLIENT__
		addFactory(new LGIncomingConnectionFactory());
		addFactory(new LGIncomingConnectionOKFactory());
		addFactory(new LGIncomingConnectionErrorFactory());
	#endif


#if __OUTPUT_INIT__
	cout << toString() << endl;
#endif
	
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// 팩토리 객체를 특정 인덱스에 추가한다.
//
//////////////////////////////////////////////////////////////////////
void RequestServerPacketFactoryManager::addFactory (PacketFactory * pFactory) 
	 throw (Error)
{
	__BEGIN_TRY
		
	if (m_Factories[ pFactory->getPacketID() ] != NULL) {
		StringStream msg;
#ifdef __GAME_CLIENT__
	#ifdef __DEBUG_OUTPUT__
		msg << "duplicate packet factories, " << pFactory->getPacketName() ;
	#else	
		msg << "duplicate packet factories ";
	#endif
#else
		msg << "duplicate packet factories, " << pFactory->getPacketName() ;
#endif
		throw Error(msg.toString());
	}
	
	// 패킷팩토리를 등록한다.
	m_Factories[ pFactory->getPacketID() ] = pFactory;
			
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
//
// 패킷아이디로 패킷객체를 생성한다.
//
//////////////////////////////////////////////////////////////////////
Packet * RequestServerPacketFactoryManager::createPacket (PacketID_t packetID) 
	   throw (InvalidProtocolException , Error)
{
	__BEGIN_TRY

	// 패킷 아이디가 범위를 넘어섬으로 인해서 Seg.Fault 가 발생하지 않도록.
	// 이런 사용자는 당장 짤라야 한다.
	if (packetID >= m_Size || m_Factories[packetID] == NULL) {
		StringStream msg;
		msg << "packet factory [" << packetID << "] not exist.";
		throw InvalidProtocolException(msg.toString());
	}

	return m_Factories[packetID]->createPacket();
			
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// 패킷아이디로 특정 패킷의 최대 크기를 리턴한다.
//
//////////////////////////////////////////////////////////////////////
PacketSize_t RequestServerPacketFactoryManager::getPacketMaxSize (PacketID_t packetID) 
	   throw (InvalidProtocolException , Error)
{
	__BEGIN_TRY

	// 패킷 아이디가 범위를 넘어섬으로 인해서 Seg.Fault 가 발생하지 않도록.
	// 이런 사용자는 당장 짤라야 한다.
	if (packetID >= m_Size || m_Factories[packetID] == NULL) {
		StringStream msg;
		msg << "invalid packet id(" << (int)packetID << ")";
		throw InvalidProtocolException(msg.toString());
	}

	return m_Factories[packetID]->getPacketMaxSize();
			
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// 패킷아이디로 특정 패킷의 이름을 리턴한다.
//
//////////////////////////////////////////////////////////////////////
#if !defined(__GAME_CLIENT__) || defined(__GAME_CLIENT__) && defined(__DEBUG_OUTPUT__)
std::string RequestServerPacketFactoryManager::getPacketName (PacketID_t packetID) 
	   throw (InvalidProtocolException , Error)
{
	__BEGIN_TRY

	// 패킷 아이디가 범위를 넘어섬으로 인해서 Seg.Fault 가 발생하지 않도록.
	// 이런 사용자는 당장 짤라야 한다.
	if (packetID >= m_Size || m_Factories[packetID] == NULL) {
		StringStream msg;
		msg << "invalid packet id(" << (int)packetID << ")";
		throw InvalidProtocolException(msg.toString());
	}

	return m_Factories[packetID]->getPacketName();
			
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get debug string
//////////////////////////////////////////////////////////////////////
std::string RequestServerPacketFactoryManager::toString () const
	throw ()
{
	__BEGIN_TRY

	StringStream msg;

	msg << "RequestServerPacketFactoryManager(\n";

	for (int i = 0 ; i < m_Size ; i ++)
		msg << "PacketFactories[" << i << "] == " << (m_Factories[i] == NULL ? "NULL" : m_Factories[i]->getPacketName()) << "\n";

	msg << ")";

	return msg.toString();

	__END_CATCH
}
#endif

//////////////////////////////////////////////////
// global variable declaration
//////////////////////////////////////////////////

RequestServerPacketFactoryManager * g_pRequestServerPacketFactoryManager = NULL;
