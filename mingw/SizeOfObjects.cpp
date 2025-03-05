#ifdef __DEBUG_OUTPUT__
#include <io.h>
#include <process.h>
#include <stdio.h>
#include "Client.h"
#include "UIMessageManager.h"
#include "UIFunction.h"
#include "VS_UI.h" 
#include "VS_UI_Mouse_pointer.h"
#include "TalkBox.h"
#include "MShopShelf.h"
#include "MPriceManager.h"
#include "MMoneyManager.h"
#include "UIDialog.h"
#include "SkillDef.h"
#include "ClientFunction.h"
#include "MGameStringTable.h"
#include "MItemOptionTable.h"
#include "CToken.h"
#include "UserOption.h"
#include "MWeather.h"
#include "MStatusManager.h"
#include "MChatManager.h"
#include "PacketDef.h"
#include "UtilityFunction.h"
#include "ClientConfig.h"
#include "MMusic.h"
#include "MMusicTable.h"
#include "SoundDef.h"
#include "ServerInformation.h"
#include "MParty.h"
#include "MItemFinder.h"
#include "ServerInfo.h"
#include "MZoneTable.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "UserInformation.h"
#include "TempInformation.h"
#include "ProfileManager.h"
#include "CGameUpdate.h"
#include "MTestDef.h"

#include "Packet\CPackets\CGRequestNewbieItem.h"
#include "Packet\CPackets\CGSilverCoating.h"
#include "Packet\CPackets\CLChangeServer.h"
#include "Packet\CPackets\CLGetServerList.h"
#include "Packet\CPackets\CGSelectPortal.h"
#include "Packet\CPackets\CGSelectWayPoint.h"
#include "Packet\CPackets\CGPartyInvite.h"
#include "Packet\CPackets\CGPartyLeave.h"
#include "packet\Cpackets\CGResurrect.h"
#include "packet\Cpackets\CLSelectWorld.h"
#include "packet\Cpackets\CLSelectServer.h"
#include "packet\Cpackets\CLQueryCharacterName.h"
#include "packet\CPackets\CGRelicToObject.h"
#include "packet\CPackets\CGSelectGuild.h"
#include "packet\CPackets\CGSelectGuildMember.h"
#include "packet\CPackets\CGRequestGuildMemberList.h"
#include "packet\CPackets\CGTryJoinGuild.h"
#include "packet\CPackets\CGJoinGuild.h"
#include "packet\CPackets\CGRegistGuild.h"
#include "packet\CPackets\CGModifyGuildMember.h"
#include "packet\CPackets\CGAddItemToItem.h"
#include "packet\CPackets\CGModifyGuildIntro.h"
#include "packet\CPackets\CGModifyGuildMemberIntro.h"
#include "packet\CPackets\CGSkillToNamed.H"
#include "packet\CPackets\CGSelectRankBonus.H"
#include "packet\CPackets\CGUseMessageItemFromInventory.h"
#include "packet\CPackets\CGWithdrawTax.H"
#include "packet\CPackets\CGTypeStringList.h"
#include "packet\CPackets\CGUseItemFromGear.h"
#include "packet\CPackets\CGLotterySelect.h"
#include "packet\CPackets\CGTakeOutGood.h"
#include "packet/CPackets/CGMixItem.h"					// -_- 비줠 어시스트에서 이렇게 하면 나온다고하길래
#include "packet/CPackets/CGDownSkill.h"
#include "packet/GPackets/GCMiniGameScores.h"
#include "packet/CPackets/CGSubmitScore.h"
#include "packet/CPackets/CGAddItemToCodeSheet.h"
#include "packet/CPackets/CGFailQuest.h"

#include "RequestFunction.h"
#include "RequestServerPlayerManager.h"
#include "RequestUserManager.h"
#include "ClientCommunicationManager.h"
#include "WhisperManager.h"
#include "packet\Rpackets\RCSay.h"
#include "packet\Cpackets\CGGuildChat.h"
#include "CMP3.h"
#include "RankBonusTable.h"
#include "Profiler.h"

#include "DebugInfo.h"
#include "MEventManager.h"
#include "MOustersGear.h"
#include "SoundSetting.h"
#include "WavePackFileManager.h"
#include "ShowTimeChecker.h"
#include "RequestFileManager.h"
#include "pcconfigtable.h"
#include "MZoneSoundManager.h"
#include "MZoneInfo.h"
#include "MTimeItemManager.h"
#include "MStopZoneXEffectGenerator.h"
#include "MStopZoneWallEffectGenerator.h"
#include "MStopZoneSelectableEffectGenerator.h"
#include "MStopZoneRhombusEffectGenerator.h"
#include "MStopZoneRectEffectGenerator.h"
#include "MStopZoneRandomEffectGenerator.h"
#include "MStopZoneRandomGenerator.h"
#include "MStopZoneEffectGenerator.h"
#include "MStopZoneEmptyCrossEffectGenerator.h"
#include "MStopZoneEmptyRectEffectGenerator.h"
#include "MWarManager.H"
#include "MStopZoneEmptyVerticalEffectGenerator.h"
#include "MStopZoneEmptyHorizontalWallEffectGenerator.h"
#include "MStopZoneCrossEffectGenerator.h"
#include "MStopInventoryEffectGenerator.h"
#include "MSpreadOutEffectGenerator.h"
#include "MSortedItemManager.h"
#include "MShopTemplateTable.h"
#include "MShadowAnimationObject.h"
#include "MShadowObject.h"
#include "MScreenEffect.h"
#include "MRisingEffectGenerator.h"
#include "MRippleZoneEffectGenerator.h"
#include "MRippleZonePixelEffectGenerator.h"
#include "MRippleZoneWideEffectGenerator.h"
#include "MPortal.h"
#include "MParabolaEffect.h"
#include "MonsterNameTable.h"
#include "ModifyStatusManager.h"
#include "MObjectSelector.h"
#include "MNPCTable.h"
#include "MNPCScriptTable.h"
#include "NPCInfo.h"
#include "MNPC.h"
#include "MMultipleFallingEffectGenerator.h"
#include "MQuestInfo.h"
#include "MMeteorDropEffectGenerator.h"
#include "MJusticeAttackManager.h"
#include "MInternetConnection.h"
#include "MMonsterKillQuestInfo.h"
#include "MInteractionObjectTable.h"
#include "MInput.h"
#include "MHomingEffect.h"
#include "MHelpStringTable.h"
#include "MHelpDisplayer.h"
#include "MGuildMarkManager.h"
#include "GuildInfo.h"
#include "MGuildInfoMapper.h"
#include "MGuidanceEffect.h"
#include "MGameTime.h"
#include "MFollowPathEffectGenerator.h"
#include "MFallingEffectGenerator.h"
#include "MFakeCreature.h"
#include "MActionInfoTable.h"
#include "MInteractionObject.h"
#include "MGuildWarManager.H"
#include "MBloodyWaveEffectGenerator.h"
#include "MBloodyWallEffectGenerator.h"
#include "MAttachCreatureEffectGenerator.h"
#include "MAttachZoneAroundEffectGenerator.h"
#include "MEffectGeneratorTable.h"
#include "MCompareManager.h"
#include "MChaseEffect.h"
#include "MBloodyBreakerEffectGenerator.h"
#include "MAttackZoneRectEffectGenerator.h"
#include "MAttackZoneBombEffectGenerator.h"
#include "MAttackZoneEffectGenerator.h"
#include "MAttackZoneParabolaEffectGenerator.h"
#include "MAttackZoneRectEffectGenerator.h"
#include "MAttackCreatureEffectGenerator.h"
#include "MAttackCreatureHomingEffectGenerator.h"
#include "MAttackZoneBombEffectGenerator.h"
#include "MAttachOrbitEffect.h"
#include "MAttachCreatureOrbitEffectGenerator.h"
#include "MAttachCreatureEffectGroundGenerator.h"
#include "MAroundZoneEffectGenerator.h"
#include "GuildMemberInfo.h"
#include "Gpackets/GCNotifyWin.h"
#include "FameInfo.h"
#include "ExperienceTable.h"
#include "CWaitUIUpdate.h"
#include "CWaitPacketUpdate.h"
#include "AppendPatchInfo.h"
#include "COpeningUpdate.h"
#include "CJpeg.h"
#include "COrderedList.h"
#include "MAttackCreatureParabolaEffectGenerator.h"
#include "MAttachZoneSelectableEffectGenerator.h"
#include "MAttachZoneEffectGenerator.h"





void SizeOfObjects()
{
	FILE *fp = fopen("SizeOfObjects.txt","wt");
	fprintf(fp,"FILEINFO_ZONE_HEADER : %d\n",sizeof(FILEINFO_ZONE_HEADER) );
	fprintf(fp,"WhisperManager : %d\n",sizeof(WhisperManager) );
	fprintf(fp,"WhisperInfo : %d\n",sizeof(WhisperInfo) );
	fprintf(fp,"WHISPER_MESSAGE : %d\n",sizeof(WHISPER_MESSAGE) );
	fprintf(fp,"WavePackFileManager : %d\n",sizeof(WavePackFileManager) );
	fprintf(fp,"WavePackFileInfo : %d\n",sizeof(WavePackFileInfo) );
	fprintf(fp,"UserOption : %d\n",sizeof(UserOption) );
	fprintf(fp,"UserInformation : %d\n",sizeof(UserInformation) );
	fprintf(fp,"WAR_INFO : %d\n",sizeof(WAR_INFO) );
	fprintf(fp,"UIMessageManager : %d\n",sizeof(UIMessageManager) );
	fprintf(fp,"UIDialog : %d\n",sizeof(UIDialog) );
	fprintf(fp,"TempInformation : %d\n",sizeof(TempInformation) );
	fprintf(fp,"PCTalkBox : %d\n",sizeof(PCTalkBox) );
	fprintf(fp,"SOUND_NODE : %d\n",sizeof(SOUND_NODE) );
	fprintf(fp,"ShowTimeChecker : %d\n",sizeof(ShowTimeChecker) );
	fprintf(fp,"ServerInformation : %d\n",sizeof(ServerInformation) );
	fprintf(fp,"ServerGroup : %d\n",sizeof(ServerGroup) );
	fprintf(fp,"SERVER_INFO : %d\n",sizeof(SERVER_INFO) );
	fprintf(fp,"SECTORSOUND_INFO : %d\n",sizeof(SECTORSOUND_INFO) );
	fprintf(fp,"RequestUserManager : %d\n",sizeof(RequestUserManager) );
	fprintf(fp,"RequestUserInfo : %d\n",sizeof(RequestUserInfo) );
	fprintf(fp,"RequestFileManager : %d\n",sizeof(RequestFileManager) );
	fprintf(fp,"RequestSendInfo : %d\n",sizeof(RequestSendInfo) );
	fprintf(fp,"RequestReceiveInfo : %d\n",sizeof(RequestReceiveInfo) );
	fprintf(fp,"ReceiveFileInfo : %d\n",sizeof(ReceiveFileInfo) );
	fprintf(fp,"SendFileInfo : %d\n",sizeof(SendFileInfo) );
	fprintf(fp,"RankBonusTable : %d\n",sizeof(RankBonusTable) );
	fprintf(fp,"RankBonusInfo : %d\n",sizeof(RankBonusInfo) );
	fprintf(fp,"ProfilerInfo : %d\n",sizeof(ProfilerInfo) );
	fprintf(fp,"ProfileManager : %d\n",sizeof(ProfileManager) );
	fprintf(fp,"WorldPlayerConfigTable : %d\n",sizeof(WorldPlayerConfigTable) );
	fprintf(fp,"PlayerConfigTable : %d\n",sizeof(PlayerConfigTable) );
	fprintf(fp,"PlayerConfig : %d\n",sizeof(PlayerConfig) );
//	fprintf(fp,"CharacterConfig : %d\n",sizeof(CharacterConfig) );
//	fprintf(fp,"PackFileManager : %d\n",sizeof(PackFileManager) );
	fprintf(fp,"PackFileInfo : %d\n",sizeof(PackFileInfo) );
//	fprintf(fp,"CNPGameLib : %d\n",sizeof(CNPGameLib) );
	fprintf(fp,"CZoneTable : %d\n",sizeof(CZoneTable) );
	fprintf(fp,"ZONETABLE_INFO : %d\n",sizeof(ZONETABLE_INFO) );
	fprintf(fp,"MZoneSoundManager : %d\n",sizeof(MZoneSoundManager) );
	fprintf(fp,"MZoneSoundTable : %d\n",sizeof(MZoneSoundTable) );
	fprintf(fp,"ZONESOUND_NODE : %d\n",sizeof(ZONESOUND_NODE) );
	fprintf(fp,"ZONESOUND_INFO : %d\n",sizeof(ZONESOUND_INFO) );
	fprintf(fp,"MZoneInfo : %d\n",sizeof(MZoneInfo) );
	fprintf(fp,"MZone : %d\n",sizeof(MZone) );
	fprintf(fp,"UI_PORTAL_FLAG : %d\n",sizeof(UI_PORTAL_FLAG) );
	fprintf(fp,"MWorkThread : %d\n",sizeof(MWorkThread) );
	fprintf(fp,"MWorkNode : %d\n",sizeof(MWorkNode) );
	fprintf(fp,"MWeather : %d\n",sizeof(MWeather) );
	fprintf(fp,"MAP_EFFECT : %d\n",sizeof(MAP_EFFECT) );
	fprintf(fp,"MWarManager : %d\n",sizeof(MWarManager) );
	fprintf(fp,"MTradeManager : %d\n",sizeof(MTradeManager) );
	fprintf(fp,"MTopView : %d\n",sizeof(MTopView) );
	fprintf(fp,"TextComparison : %d\n",sizeof(TextComparison) );
	fprintf(fp,"MTimeItemManager : %d\n",sizeof(MTimeItemManager) );
//	fprintf(fp,"UDPTest : %d\n",sizeof(UDPTest) );
	fprintf(fp,"MStringMap : %d\n",sizeof(MStringMap) );
	fprintf(fp,"MStringPointerCompare : %d\n",sizeof(MStringPointerCompare) );
	fprintf(fp,"MStringList : %d\n",sizeof(MStringList) );
	fprintf(fp,"MStringArray : %d\n",sizeof(MStringArray) );
	fprintf(fp,"MString : %d\n",sizeof(MString) );
	fprintf(fp,"MStorage : %d\n",sizeof(MStorage) );
	fprintf(fp,"MStopZoneXEffectGenerator : %d\n",sizeof(MStopZoneXEffectGenerator) );
	fprintf(fp,"MStopZoneWallEffectGenerator : %d\n",sizeof(MStopZoneWallEffectGenerator) );
	fprintf(fp,"MStopZoneSelectableEffectGenerator : %d\n",sizeof(MStopZoneSelectableEffectGenerator) );
	fprintf(fp,"MStopZoneRhombusEffectGenerator : %d\n",sizeof(MStopZoneRhombusEffectGenerator) );
	fprintf(fp,"MStopZoneRectEffectGenerator : %d\n",sizeof(MStopZoneRectEffectGenerator) );
	fprintf(fp,"MStopZoneRandomEffectGenerator : %d\n",sizeof(MStopZoneRandomEffectGenerator) );
	fprintf(fp,"MStopZoneEmptyVerticalWallEffectGenerator : %d\n",sizeof(MStopZoneEmptyVerticalWallEffectGenerator) );
	fprintf(fp,"MStopZoneEmptyRectEffectGenerator : %d\n",sizeof(MStopZoneEmptyRectEffectGenerator) );
	fprintf(fp,"MStopZoneEmptyHorizontalWallEffectGenerator : %d\n",sizeof(MStopZoneEmptyHorizontalWallEffectGenerator) );
	fprintf(fp,"MStopZoneEmptyCrossEffectGenerator : %d\n",sizeof(MStopZoneEmptyCrossEffectGenerator) );
	fprintf(fp,"MStopZoneEffectGenerator : %d\n",sizeof(MStopZoneEffectGenerator) );
	fprintf(fp,"MStopZoneCrossEffectGenerator : %d\n",sizeof(MStopZoneCrossEffectGenerator) );
	fprintf(fp,"MStopInventoryEffectGenerator : %d\n",sizeof(MStopInventoryEffectGenerator) );
//	fprintf(fp,"__EX : %d\n",sizeof(__EX) );
	fprintf(fp,"MStatus : %d\n",sizeof(MStatus) );
	fprintf(fp,"MSpreadOutEffectGenerator : %d\n",sizeof(MSpreadOutEffectGenerator) );
//	fprintf(fp,"SOUNDTABLE_INFO : %d\n",sizeof(SOUNDTABLE_INFO) );
	fprintf(fp,"MSortedItemManager : %d\n",sizeof(MSortedItemManager) );
	fprintf(fp,"MSlotItemManager : %d\n",sizeof(MSlotItemManager) );
	fprintf(fp,"MSkillManager : %d\n",sizeof(MSkillManager) );
	fprintf(fp,"MSkillInfoTable : %d\n",sizeof(MSkillInfoTable) );
	fprintf(fp,"MSkillDomain : %d\n",sizeof(MSkillDomain) );
	fprintf(fp,"MSkillSet : %d\n",sizeof(MSkillSet) );
	fprintf(fp,"SKILLID_NODE : %d\n",sizeof(SKILLID_NODE) );
	fprintf(fp,"SKILLINFO_NODE : %d\n",sizeof(SKILLINFO_NODE) );
	fprintf(fp,"MShopTemplateTable : %d\n",sizeof(MShopTemplateTable) );
	fprintf(fp,"MShopTemplate : %d\n",sizeof(MShopTemplate) );
	fprintf(fp,"MShopUnknownShelf : %d\n",sizeof(MShopUnknownShelf) );
	fprintf(fp,"MShopSpecialShelf : %d\n",sizeof(MShopSpecialShelf) );
	fprintf(fp,"MShopFixedShelf : %d\n",sizeof(MShopFixedShelf) );
	fprintf(fp,"MShopShelf : %d\n",sizeof(MShopShelf) );
	fprintf(fp,"MShop : %d\n",sizeof(MShop) );
	fprintf(fp,"MShadowObject : %d\n",sizeof(MShadowObject) );
	fprintf(fp,"MShadowAnimationObject : %d\n",sizeof(MShadowAnimationObject) );
	fprintf(fp,"MSectorInfo : %d\n",sizeof(MSectorInfo) );
	fprintf(fp,"MSector : %d\n",sizeof(MSector) );
	fprintf(fp,"PORTAL_INFO : %d\n",sizeof(PORTAL_INFO) );
	fprintf(fp,"MScreenEffectManager : %d\n",sizeof(MScreenEffectManager) );
	fprintf(fp,"MScreenEffect : %d\n",sizeof(MScreenEffect) );
	fprintf(fp,"MRisingEffectGenerator : %d\n",sizeof(MRisingEffectGenerator) );
	fprintf(fp,"MRippleZoneWideEffectGenerator : %d\n",sizeof(MRippleZoneWideEffectGenerator) );
	fprintf(fp,"MRippleZonePixelEffectGenerator : %d\n",sizeof(MRippleZonePixelEffectGenerator) );
	fprintf(fp,"MRippleZoneEffectGenerator : %d\n",sizeof(MRippleZoneEffectGenerator) );
	fprintf(fp,"MRequestMode : %d\n",sizeof(MRequestMode) );
	fprintf(fp,"MPriceManager : %d\n",sizeof(MPriceManager) );
	fprintf(fp,"STAR_ITEM_PRICE : %d\n",sizeof(STAR_ITEM_PRICE) );
	fprintf(fp,"MPortal : %d\n",sizeof(MPortal) );
	fprintf(fp,"P_RECT : %d\n",sizeof(P_RECT) );
	fprintf(fp,"B_RECT : %d\n",sizeof(B_RECT) );
	fprintf(fp,"MPlayerGear : %d\n",sizeof(MPlayerGear) );
	fprintf(fp,"MPlayer : %d\n",sizeof(MPlayer) );
	fprintf(fp,"Comparison : %d\n",sizeof(Comparison) );
	fprintf(fp,"DNode : %d\n",sizeof(DNode) );
	fprintf(fp,"MParty : %d\n",sizeof(MParty) );
	fprintf(fp,"PARTY_INFO : %d\n",sizeof(PARTY_INFO) );
	fprintf(fp,"MParabolaEffect : %d\n",sizeof(MParabolaEffect) );
	fprintf(fp,"MonsterNameTable : %d\n",sizeof(MonsterNameTable) );
	fprintf(fp,"ModifyStatusManager : %d\n",sizeof(ModifyStatusManager) );
	fprintf(fp,"MODIFY_VALUE : %d\n",sizeof(MODIFY_VALUE) );
	fprintf(fp,"MObjectSelector : %d\n",sizeof(MObjectSelector) );
	fprintf(fp,"MObject : %d\n",sizeof(MObject) );
//	fprintf(fp,"hierachy : %d\n",sizeof(hierachy) );
	fprintf(fp,"MServerNPCTable : %d\n",sizeof(MServerNPCTable) );
	fprintf(fp,"SERVERNPC_INFO : %d\n",sizeof(SERVERNPC_INFO) );
	fprintf(fp,"MNPCTable : %d\n",sizeof(MNPCTable) );
	fprintf(fp,"NPC_INFO : %d\n",sizeof(NPC_INFO) );
	fprintf(fp,"MNPCScriptTable : %d\n",sizeof(MNPCScriptTable) );
	fprintf(fp,"NPC_SCRIPT : %d\n",sizeof(NPC_SCRIPT) );
	fprintf(fp,"MNPC : %d\n",sizeof(MNPC) );
	fprintf(fp,"MUSICTABLE_INFO : %d\n",sizeof(MUSICTABLE_INFO) );
	fprintf(fp,"MMusic : %d\n",sizeof(MMusic) );
	fprintf(fp,"MMultipleFallingEffectGenerator : %d\n",sizeof(MMultipleFallingEffectGenerator) );
	fprintf(fp,"MMovingEffect : %d\n",sizeof(MMovingEffect) );
	fprintf(fp,"MQuestInfoManager : %d\n",sizeof(MQuestInfoManager) );
	fprintf(fp,"QUEST_INFO : %d\n",sizeof(QUEST_INFO) );
	fprintf(fp,"MMoneyManager : %d\n",sizeof(MMoneyManager) );
	fprintf(fp,"MMeteorDropEffectGenerator : %d\n",sizeof(MMeteorDropEffectGenerator) );
	fprintf(fp,"MLoadingSPKWorkNode4 : %d\n",sizeof(MLoadingSPKWorkNode4) );
	fprintf(fp,"MLoadingSPKWorkNode3 : %d\n",sizeof(MLoadingSPKWorkNode3) );
	fprintf(fp,"MLoadingSPKWorkNode2 : %d\n",sizeof(MLoadingSPKWorkNode2) );
	fprintf(fp,"MLoadingSPKWorkNode1 : %d\n",sizeof(MLoadingSPKWorkNode1) );
	fprintf(fp,"MLoadingSPKWorkNode : %d\n",sizeof(MLoadingSPKWorkNode) );
	fprintf(fp,"MLinearEffect : %d\n",sizeof(MLinearEffect) );
	fprintf(fp,"MJusticeAttackManager : %d\n",sizeof(MJusticeAttackManager) );
	fprintf(fp,"ITEMCLASS_TABLE : %d\n",sizeof(ITEMCLASS_TABLE) );
	fprintf(fp,"ITEMTYPE_TABLE : %d\n",sizeof(ITEMTYPE_TABLE) );
	fprintf(fp,"ITEMTABLE_INFO : %d\n",sizeof(ITEMTABLE_INFO) );
	fprintf(fp,"ITEMOPTION_TABLE : %d\n",sizeof(ITEMOPTION_TABLE) );
	fprintf(fp,"ITEMOPTION_INFO : %d\n",sizeof(ITEMOPTION_INFO) );
	fprintf(fp,"MItemManager : %d\n",sizeof(MItemManager) );
	fprintf(fp,"MOustersSummonGemItemFinder : %d\n",sizeof(MOustersSummonGemItemFinder) );
	fprintf(fp,"MItemClassRangeTypeFinder : %d\n",sizeof(MItemClassRangeTypeFinder) );
	fprintf(fp,"MItemClassTypeNumberFinder : %d\n",sizeof(MItemClassTypeNumberFinder) );
	fprintf(fp,"MSlayerPortalItemFinder : %d\n",sizeof(MSlayerPortalItemFinder) );
	fprintf(fp,"MVampirePortalItemFinder : %d\n",sizeof(MVampirePortalItemFinder) );
	fprintf(fp,"MItemClassTypeFinder : %d\n",sizeof(MItemClassTypeFinder) );
	fprintf(fp,"MItemClassFinder : %d\n",sizeof(MItemClassFinder) );
	fprintf(fp,"MItemFinder : %d\n",sizeof(MItemFinder) );
	fprintf(fp,"MCodeSheetItem : %d\n",sizeof(MCodeSheetItem) );
	fprintf(fp,"MEffectItem : %d\n",sizeof(MEffectItem) );
	fprintf(fp,"MOustersSummonGem : %d\n",sizeof(MOustersSummonGem) );
	fprintf(fp,"MOustersComposMei : %d\n",sizeof(MOustersComposMei) );
	fprintf(fp,"MOustersPupa : %d\n",sizeof(MOustersPupa) );
	fprintf(fp,"MOustersLarva : %d\n",sizeof(MOustersLarva) );
	fprintf(fp,"MOustersWristlet : %d\n",sizeof(MOustersWristlet) );
	fprintf(fp,"MOustersStone : %d\n",sizeof(MOustersStone) );
	fprintf(fp,"MOustersRing : %d\n",sizeof(MOustersRing) );
	fprintf(fp,"MOustersPendent : %d\n",sizeof(MOustersPendent) );
	fprintf(fp,"MOustersCoat : %d\n",sizeof(MOustersCoat) );
	fprintf(fp,"MOustersCirclet : %d\n",sizeof(MOustersCirclet) );
	fprintf(fp,"MOustersChakram : %d\n",sizeof(MOustersChakram) );
	fprintf(fp,"MOustersBoots : %d\n",sizeof(MOustersBoots) );
	fprintf(fp,"MOustersArmsBand : %d\n",sizeof(MOustersArmsBand) );
	fprintf(fp,"MMixingItem : %d\n",sizeof(MMixingItem) );
	fprintf(fp,"MResurrectItem : %d\n",sizeof(MResurrectItem) );
	fprintf(fp,"MDyePotionItem : %d\n",sizeof(MDyePotionItem) );
	fprintf(fp,"MEventItem : %d\n",sizeof(MEventItem) );
	fprintf(fp,"MVampireCoupleRing : %d\n",sizeof(MVampireCoupleRing) );
	fprintf(fp,"MCoupleRing : %d\n",sizeof(MCoupleRing) );
	fprintf(fp,"MCastleSymbol : %d\n",sizeof(MCastleSymbol) );
	fprintf(fp,"MBloodBible : %d\n",sizeof(MBloodBible) );
	fprintf(fp,"MEventEtcItem : %d\n",sizeof(MEventEtcItem) );
	fprintf(fp,"MEventTreeItem : %d\n",sizeof(MEventTreeItem) );
	fprintf(fp,"MQuestItem : %d\n",sizeof(MQuestItem) );
	fprintf(fp,"MVampireAmulet : %d\n",sizeof(MVampireAmulet) );
	fprintf(fp,"MEventStarItem : %d\n",sizeof(MEventStarItem) );
	fprintf(fp,"MEventGiftBoxItem : %d\n",sizeof(MEventGiftBoxItem) );
	fprintf(fp,"MVampirePortalItem : %d\n",sizeof(MVampirePortalItem) );
	fprintf(fp,"MSlayerPortalItem : %d\n",sizeof(MSlayerPortalItem) );
	fprintf(fp,"MVampireETC : %d\n",sizeof(MVampireETC) );
	fprintf(fp,"MSerum : %d\n",sizeof(MSerum) );
	fprintf(fp,"MVampireCoat : %d\n",sizeof(MVampireCoat) );
	fprintf(fp,"MVampireNecklace : %d\n",sizeof(MVampireNecklace) );
	fprintf(fp,"MVampireBracelet : %d\n",sizeof(MVampireBracelet) );
	fprintf(fp,"MVampireEarRing : %d\n",sizeof(MVampireEarRing) );
	fprintf(fp,"MVampireRing : %d\n",sizeof(MVampireRing) );
	fprintf(fp,"MSkull : %d\n",sizeof(MSkull) );
	fprintf(fp,"MMine : %d\n",sizeof(MMine) );
	fprintf(fp,"MMoney : %d\n",sizeof(MMoney) );
	fprintf(fp,"MLearningItem : %d\n",sizeof(MLearningItem) );
	fprintf(fp,"MBombMaterial : %d\n",sizeof(MBombMaterial) );
	fprintf(fp,"MRelic : %d\n",sizeof(MRelic) );
	fprintf(fp,"MKey : %d\n",sizeof(MKey) );
	fprintf(fp,"MMotorcycle : %d\n",sizeof(MMotorcycle) );
	fprintf(fp,"MItemETC : %d\n",sizeof(MItemETC) );
	fprintf(fp,"MGunTR : %d\n",sizeof(MGunTR) );
	fprintf(fp,"MGunAR : %d\n",sizeof(MGunAR) );
	fprintf(fp,"MGunSMG : %d\n",sizeof(MGunSMG) );
	fprintf(fp,"MGunSG : %d\n",sizeof(MGunSG) );
	fprintf(fp,"MGunItem : %d\n",sizeof(MGunItem) );
	fprintf(fp,"MHelm : %d\n",sizeof(MHelm) );
	fprintf(fp,"MGlove : %d\n",sizeof(MGlove) );
	fprintf(fp,"MMace : %d\n",sizeof(MMace) );
	fprintf(fp,"MCross : %d\n",sizeof(MCross) );
	fprintf(fp,"MShield : %d\n",sizeof(MShield) );
	fprintf(fp,"MBlade : %d\n",sizeof(MBlade) );
	fprintf(fp,"MSword : %d\n",sizeof(MSword) );
	fprintf(fp,"MWeaponItem : %d\n",sizeof(MWeaponItem) );
	fprintf(fp,"MShoes : %d\n",sizeof(MShoes) );
	fprintf(fp,"MTrouser : %d\n",sizeof(MTrouser) );
	fprintf(fp,"MCoat : %d\n",sizeof(MCoat) );
	fprintf(fp,"MNecklace : %d\n",sizeof(MNecklace) );
	fprintf(fp,"MBracelet : %d\n",sizeof(MBracelet) );
	fprintf(fp,"MRing : %d\n",sizeof(MRing) );
	fprintf(fp,"MMagazine : %d\n",sizeof(MMagazine) );
	fprintf(fp,"MHolyWater : %d\n",sizeof(MHolyWater) );
	fprintf(fp,"MWater : %d\n",sizeof(MWater) );
	fprintf(fp,"MPotion : %d\n",sizeof(MPotion) );
	fprintf(fp,"MCorpse : %d\n",sizeof(MCorpse) );
	fprintf(fp,"MBelt : %d\n",sizeof(MBelt) );
	fprintf(fp,"MPileItem : %d\n",sizeof(MPileItem) );
	fprintf(fp,"MGearItem : %d\n",sizeof(MGearItem) );
	fprintf(fp,"MItem : %d\n",sizeof(MItem) );
	fprintf(fp,"MInventory : %d\n",sizeof(MInventory) );
	fprintf(fp,"MInternetFile : %d\n",sizeof(MInternetFile) );
	fprintf(fp,"MInternetConnection : %d\n",sizeof(MInternetConnection) );
	fprintf(fp,"INTERACTIONOBJECTTABLE_INFO : %d\n",sizeof(INTERACTIONOBJECTTABLE_INFO) );
	fprintf(fp,"MInteractionObject : %d\n",sizeof(MInteractionObject) );
	fprintf(fp,"MInput : %d\n",sizeof(MInput) );
	fprintf(fp,"MImageObject : %d\n",sizeof(MImageObject) );
	fprintf(fp,"MHomingEffect : %d\n",sizeof(MHomingEffect) );
	fprintf(fp,"MHelpStringTable : %d\n",sizeof(MHelpStringTable) );
	fprintf(fp,"MHelpManager : %d\n",sizeof(MHelpManager) );
	fprintf(fp,"MOutputHelpNode : %d\n",sizeof(MOutputHelpNode) );
	fprintf(fp,"MCompareHelpNode : %d\n",sizeof(MCompareHelpNode) );
	fprintf(fp,"MHelpNode : %d\n",sizeof(MHelpNode) );
	fprintf(fp,"MHelpDisplayer : %d\n",sizeof(MHelpDisplayer) );
	fprintf(fp,"MHelicopterManager : %d\n",sizeof(MHelicopterManager) );
	fprintf(fp,"MHelicopter : %d\n",sizeof(MHelicopter) );
	fprintf(fp,"MGuildWarManager : %d\n",sizeof(MGuildWarManager) );
//	fprintf(fp,"GUILD_SPRITES : %d\n",sizeof(GUILD_SPRITES) );
	fprintf(fp,"MGuildMarkManager : %d\n",sizeof(MGuildMarkManager) );
	fprintf(fp,"MGuildInfoMapper : %d\n",sizeof(MGuildInfoMapper) );
	fprintf(fp,"GUILD_INFO : %d\n",sizeof(GUILD_INFO) );
	fprintf(fp,"MGuidanceEffect : %d\n",sizeof(MGuidanceEffect) );
	fprintf(fp,"MGridItemManager : %d\n",sizeof(MGridItemManager) );
	fprintf(fp,"MGameTime : %d\n",sizeof(MGameTime) );
	fprintf(fp,"MFunctionManager : %d\n",sizeof(MFunctionManager) );
	fprintf(fp,"MFollowPathEffectGenerator : %d\n",sizeof(MFollowPathEffectGenerator) );
	fprintf(fp,"MFallingEffectGenerator : %d\n",sizeof(MFallingEffectGenerator) );
	fprintf(fp,"MFakeCreature : %d\n",sizeof(MFakeCreature) );
	fprintf(fp,"MEventManager : %d\n",sizeof(MEventManager) );
	fprintf(fp,"MEvent : %d\n",sizeof(MEvent) );
	fprintf(fp,"MPortalEffectTarget : %d\n",sizeof(MPortalEffectTarget) );
	fprintf(fp,"MEffectTarget : %d\n",sizeof(MEffectTarget) );
	fprintf(fp,"EFFECTSTATUS_NODE : %d\n",sizeof(EFFECTSTATUS_NODE) );
//	fprintf(fp,"MActionEffectSpriteTypeTable : %d\n",sizeof(MActionEffectSpriteTypeTable) );
//	fprintf(fp,"ACTION_FRAMEID_ARRAY : %d\n",sizeof(ACTION_FRAMEID_ARRAY) );
//	fprintf(fp,"FRAME_TYPE : %d\n",sizeof(FRAME_TYPE) );
//	fprintf(fp,"EFFECTSPRITETYPETABLE_INFO : %d\n",sizeof(EFFECTSPRITETYPETABLE_INFO) );
	fprintf(fp,"MEffectManager : %d\n",sizeof(MEffectManager) );
	fprintf(fp,"MEffectGeneratorTable : %d\n",sizeof(MEffectGeneratorTable) );
	fprintf(fp,"MEffectGenerator : %d\n",sizeof(MEffectGenerator) );
	fprintf(fp,"EFFECTGENERATOR_INFO : %d\n",sizeof(EFFECTGENERATOR_INFO) );
	fprintf(fp,"MSelectableEffect : %d\n",sizeof(MSelectableEffect) );
	fprintf(fp,"MEffect : %d\n",sizeof(MEffect) );
//	fprintf(fp,"hierarchy : %d\n",sizeof(hierarchy) );
//	fprintf(fp,"ADDON_INFO : %d\n",sizeof(ADDON_INFO) );
	fprintf(fp,"MCreatureWear : %d\n",sizeof(MCreatureWear) );
	fprintf(fp,"CreatureSpriteTypeMapper : %d\n",sizeof(CreatureSpriteTypeMapper) );
	fprintf(fp,"CREATURETABLE_INFO : %d\n",sizeof(CREATURETABLE_INFO) );
	fprintf(fp,"ITEM_WEARINFO : %d\n",sizeof(ITEM_WEARINFO) );
	fprintf(fp,"CREATURESPRITETABLE_INFO : %d\n",sizeof(CREATURESPRITETABLE_INFO) );
//	fprintf(fp,"HPModify : %d\n",sizeof(HPModify) );
//	fprintf(fp,"MoveNode : %d\n",sizeof(MoveNode) );
	fprintf(fp,"MCreature : %d\n",sizeof(MCreature) );
	fprintf(fp,"MCompareManager : %d\n",sizeof(MCompareManager) );
	fprintf(fp,"MChatManager : %d\n",sizeof(MChatManager) );
	fprintf(fp,"MChaseEffect : %d\n",sizeof(MChaseEffect) );
	fprintf(fp,"MBloodyWaveEffectGenerator : %d\n",sizeof(MBloodyWaveEffectGenerator) );
	fprintf(fp,"MBloodyWallEffectGenerator : %d\n",sizeof(MBloodyWallEffectGenerator) );
	fprintf(fp,"MBloodyBreakerEffectGenerator : %d\n",sizeof(MBloodyBreakerEffectGenerator) );
	fprintf(fp,"MAttackZoneRectEffectGenerator : %d\n",sizeof(MAttackZoneRectEffectGenerator) );
	fprintf(fp,"MAttackZoneParabolaEffectGenerator : %d\n",sizeof(MAttackZoneParabolaEffectGenerator) );
	fprintf(fp,"MAttackZoneEffectGenerator : %d\n",sizeof(MAttackZoneEffectGenerator) );
	fprintf(fp,"MAttackZoneBombEffectGenerator : %d\n",sizeof(MAttackZoneBombEffectGenerator) );
	fprintf(fp,"MAttackCreatureParabolaEffectGenerator : %d\n",sizeof(MAttackCreatureParabolaEffectGenerator) );
	fprintf(fp,"MAttackCreatureHomingEffectGenerator : %d\n",sizeof(MAttackCreatureHomingEffectGenerator) );
	fprintf(fp,"MAttackCreatureEffectGenerator : %d\n",sizeof(MAttackCreatureEffectGenerator) );
	fprintf(fp,"MAttachZoneSelectableEffectGenerator : %d\n",sizeof(MAttachZoneSelectableEffectGenerator) );
	fprintf(fp,"MAttachZoneEffectGenerator : %d\n",sizeof(MAttachZoneEffectGenerator) );
	fprintf(fp,"MAttachZoneAroundEffectGenerator : %d\n",sizeof(MAttachZoneAroundEffectGenerator) );
	fprintf(fp,"MAttachOrbitEffect : %d\n",sizeof(MAttachOrbitEffect) );
	fprintf(fp,"MAttachEffect : %d\n",sizeof(MAttachEffect) );
	fprintf(fp,"MAttachCreatureOrbitEffectGenerator : %d\n",sizeof(MAttachCreatureOrbitEffectGenerator) );
	fprintf(fp,"MAttachCreatureEffectGroundGenerator : %d\n",sizeof(MAttachCreatureEffectGroundGenerator) );
	fprintf(fp,"MAttachCreatureEffectGenerator : %d\n",sizeof(MAttachCreatureEffectGenerator) );
	fprintf(fp,"MAroundZoneEffectGenerator : %d\n",sizeof(MAroundZoneEffectGenerator) );
	fprintf(fp,"MAnimationObject : %d\n",sizeof(MAnimationObject) );
	fprintf(fp,"MActionResultNodeEvent : %d\n",sizeof(MActionResultNodeEvent) );
	fprintf(fp,"MActionResultNodeFakeDie : %d\n",sizeof(MActionResultNodeFakeDie) );
	fprintf(fp,"MActionResultNodeChangeItemGenderInInventory : %d\n",sizeof(MActionResultNodeChangeItemGenderInInventory) );
	fprintf(fp,"MActionResultNodeChangeItemOptionInInventory : %d\n",sizeof(MActionResultNodeChangeItemOptionInInventory) );
	fprintf(fp,"MActionResultNodeRemoveItemInInventory : %d\n",sizeof(MActionResultNodeRemoveItemInInventory) );
	fprintf(fp,"MActionResultNodeCreatureTurning : %d\n",sizeof(MActionResultNodeCreatureTurning) );
	fprintf(fp,"MActionResultNodeOpenCasket : %d\n",sizeof(MActionResultNodeOpenCasket) );
	fprintf(fp,"MActionResultNodeSummonCasket : %d\n",sizeof(MActionResultNodeSummonCasket) );
	fprintf(fp,"MActionResultNodeChangeCreatureType : %d\n",sizeof(MActionResultNodeChangeCreatureType) );
	fprintf(fp,"MActionResultNodeCreatureSnipping : %d\n",sizeof(MActionResultNodeCreatureSnipping) );
	fprintf(fp,"MActionResultNodeAddEffectStatus : %d\n",sizeof(MActionResultNodeAddEffectStatus) );
	fprintf(fp,"MActionResultNodeCreatureVisible : %d\n",sizeof(MActionResultNodeCreatureVisible) );
	fprintf(fp,"MActionResultNodeCreatureInvisible : %d\n",sizeof(MActionResultNodeCreatureInvisible) );
	fprintf(fp,"MActionResultNodeCreatureBurrow : %d\n",sizeof(MActionResultNodeCreatureBurrow) );
	fprintf(fp,"MActionResultNodeCreatureDie : %d\n",sizeof(MActionResultNodeCreatureDie) );
	fprintf(fp,"MActionResultNodeChangeStatus : %d\n",sizeof(MActionResultNodeChangeStatus) );
	fprintf(fp,"MActionResultNodeChangePosition : %d\n",sizeof(MActionResultNodeChangePosition) );
	fprintf(fp,"MActionResultNodeActionInfo : %d\n",sizeof(MActionResultNodeActionInfo) );
	fprintf(fp,"MActionResultNode : %d\n",sizeof(MActionResultNode) );
	fprintf(fp,"MActionInfoTable : %d\n",sizeof(MActionInfoTable) );
	fprintf(fp,"MActionInfo : %d\n",sizeof(MActionInfo) );
	fprintf(fp,"ACTION_INFO_NODE : %d\n",sizeof(ACTION_INFO_NODE) );
	fprintf(fp,"GuildMemberInfo : %d\n",sizeof(GuildMemberInfo) );
	fprintf(fp,"GuildInfo : %d\n",sizeof(GuildInfo) );
	fprintf(fp,"GCNotifyWinHandler : %d\n",sizeof(GCNotifyWinHandler) );
	fprintf(fp,"GCNotifyWinFactory : %d\n",sizeof(GCNotifyWinFactory) );
	fprintf(fp,"GCNotifyWin : %d\n",sizeof(GCNotifyWin) );
	fprintf(fp,"FameInfoTable : %d\n",sizeof(FameInfoTable) );
	fprintf(fp,"FameInfo : %d\n",sizeof(FameInfo) );
	fprintf(fp,"ExpInfo : %d\n",sizeof(ExpInfo) );
	fprintf(fp,"ExperienceTable : %d\n",sizeof(ExperienceTable) );
	fprintf(fp,"DRAWTEXT_NODE : %d\n",sizeof(DRAWTEXT_NODE) );
	fprintf(fp,"DRAWITEMNAME_NODE : %d\n",sizeof(DRAWITEMNAME_NODE) );
	fprintf(fp,"CWaitUIUpdate : %d\n",sizeof(CWaitUIUpdate) );
	fprintf(fp,"CWaitPacketUpdate : %d\n",sizeof(CWaitPacketUpdate) );
//	fprintf(fp,"USINGCOLORSET_NODE : %d\n",sizeof(USINGCOLORSET_NODE) );
//	fprintf(fp,"CTypeTable : %d\n",sizeof(CTypeTable) );
//	fprintf(fp,"CTypeMap2 : %d\n",sizeof(CTypeMap2) );
//	fprintf(fp,"CTypeMap : %d\n",sizeof(CTypeMap) );
	fprintf(fp,"CToken : %d\n",sizeof(CToken) );
	fprintf(fp,"CTexturePartManager : %d\n",sizeof(CTexturePartManager) );
	fprintf(fp,"CNormalSpriteTexturePartManager : %d\n",sizeof(CNormalSpriteTexturePartManager) );
	fprintf(fp,"CSpriteTexturePartManager : %d\n",sizeof(CSpriteTexturePartManager) );
	fprintf(fp,"CSpritePalPack : %d\n",sizeof(CSpritePalPack) );
	fprintf(fp,"CSpritePal : %d\n",sizeof(CSpritePal) );
	fprintf(fp,"MPalettePack : %d\n",sizeof(MPalettePack) );
	fprintf(fp,"MPalette565 : %d\n",sizeof(MPalette565) );
	fprintf(fp,"ofstream : %d\n",sizeof(ofstream) );
	fprintf(fp,"ifstream : %d\n",sizeof(ifstream) );
	fprintf(fp,"MPalette555 : %d\n",sizeof(MPalette555) );
	fprintf(fp,"MPalette : %d\n",sizeof(MPalette) );
	fprintf(fp,"CSoundPartManager : %d\n",sizeof(CSoundPartManager) );
	fprintf(fp,"CShadowPartManager : %d\n",sizeof(CShadowPartManager) );
//	fprintf(fp,"CPositionList : %d\n",sizeof(CPositionList) );
//	fprintf(fp,"POSITION_NODE : %d\n",sizeof(POSITION_NODE) );
//	fprintf(fp,"CPartManager : %d\n",sizeof(CPartManager) );
//	fprintf(fp,"IndexType, : %d\n",sizeof(IndexType,) );
//	fprintf(fp,"COrderedList : %d\n",sizeof(COrderedList) );
//	fprintf(fp,"COpeningUpdate : %d\n",sizeof(COpeningUpdate) );
	fprintf(fp,"COGGSTREAM : %d\n",sizeof(COGGSTREAM) );
	fprintf(fp,"tag_position_set : %d\n",sizeof(tag_position_set) );
	fprintf(fp,"CDirectSoundBuffer : %d\n",sizeof(CDirectSoundBuffer) );
	fprintf(fp,"tag_wave_ima_adpcm_header : %d\n",sizeof(tag_wave_ima_adpcm_header) );
	fprintf(fp,"tag_wave_pcm_header : %d\n",sizeof(tag_wave_pcm_header) );
	fprintf(fp,"tag_wav_fourcc : %d\n",sizeof(tag_wav_fourcc) );
	fprintf(fp,"tag_sound_handle : %d\n",sizeof(tag_sound_handle) );
	fprintf(fp,"CMP3 : %d\n",sizeof(CMP3) );
	fprintf(fp,"CMessageArray : %d\n",sizeof(CMessageArray) );
	fprintf(fp,"ClientConfig : %d\n",sizeof(ClientConfig) );
	fprintf(fp,"CJpeg : %d\n",sizeof(CJpeg) );
	fprintf(fp,"CGameUpdate : %d\n",sizeof(CGameUpdate) );
//	fprintf(fp,"CDataTable : %d\n",sizeof(CDataTable) );
	fprintf(fp,"CAVI : %d\n",sizeof(CAVI) );
	fprintf(fp,"AppendPatchTable : %d\n",sizeof(AppendPatchTable) );
	fprintf(fp,"AppendPatch : %d\n",sizeof(AppendPatch) );
	fprintf(fp,"APPEND_PATCH_NODE : %d\n",sizeof(APPEND_PATCH_NODE) );
	fprintf(fp,"MTopView : %d\n",sizeof(MTopView) );
	fclose(fp);
}
#endif