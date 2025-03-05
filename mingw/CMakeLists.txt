cmake_minimum_required(VERSION 3.16)

project(Client LANGUAGES CXX)

add_executable(darkeden
	       APICheck.cpp                         GAME1024.cpp                                MEffectSpriteTypeTable.cpp           MNPC.cpp                             MStopZoneEmptyCrossEffectGenerator.cpp         Profiler.cpp
	       AppendPatchInfo.cpp                  GCNotifyWin.cpp                             MEffectStatusTable.cpp               MNPCScriptTable.cpp                  MStopZoneEmptyHorizontalEffectGenerator.cpp    RankBonusTable.cpp
	       BloodBibleSignInfo.cpp               GCNotifyWinHandler.cpp                      MEffectTarget.cpp                    MNPCTable.cpp                        MStopZoneEmptyRectEffectGenerator.cpp          RequestFileManager.cpp
	       CAvi.cpp                             GameInit.cpp                                MEventManager.cpp                    MObject.cpp                          MStopZoneEmptyVerticalWallEffectGenerator.cpp  RequestFunction.cpp
	       CDirectDrawSurface.cpp               GameInitInfo.cpp                            MFakeCreature.cpp                    MObjectSelector.cpp                  MStopZoneMultipleEffectGenerator.cpp           RequestUserManager.cpp
	       CDirectInput.cpp                     GameMain.cpp                                MFallingEffectGenerator.cpp          MOustersGear.cpp                     MStopZoneRandomEffectGenerator.cpp             SectorSoundInfo.cpp
	       CDirectMusic.cpp                     GameUI.cpp                                  MFirePiecingEffectGenerator.cpp      MParabolaEffect.cpp                  MStopZoneRectEffectGenerator.cpp               ServerInfoFileParser.cpp
	       CDirectSetupGetVersion.cpp           GetWinVer.cpp                               MFollowPathEffectGenerator.cpp       MParty.cpp                           MStopZoneRhombusEffectGenerator.cpp            ServerInformation.cpp
	       CDirectSound.cpp                     GuildInfo.cpp                               MFunctionManager.cpp                 MPlayer.cpp                          MStopZoneSelectableEffectGenerator.cpp         ShowTimeChecker.cpp
	       CDirectSoundStream.cpp               GuildMemberInfo.cpp                         MGameStringTable.cpp                 MPlayerGear.cpp                      MStopZoneWallEffectGenerator.cpp               ShrineInfoManager.cpp
	       CGameUpdate.cpp                      LeakMemoryDumper.cpp                        MGameTime.cpp                        MPlayingWithFireEffectGenerator.cpp  MStopZoneXEffectGenerator.cpp                  SizeOfObjects.cpp
	       CMP3.cpp                             MActionInfoTable.cpp                        MGridItemManager.cpp                 MPortal.cpp                          MStorage.cpp                                   SkillDef.cpp
	       MActionResult.cpp                           MGuidanceEffect.cpp                  MPriceManager.cpp                    MString.cpp                                    SoundNode.cpp
	       CMessageArray.cpp                    MAnimationObject.cpp                        MGuildInfoMapper.cpp                 MQuestInfo.cpp                       MStringArray.cpp                               StdAfx.cpp
	       CNormalSpriteTexturePartManager.cpp  MAroundZoneEffectGenerator.cpp              MGuildMarkManager.cpp                MQuickSlot.cpp                       MStringList.cpp                                StoreInfo.cpp
	       COpeningUpdate.cpp                   MAttachCreatureEffectGenerator.cpp          MHelicopterManager.cpp               MRequestMode.cpp                     MStringMap.cpp                                 SystemAvailabilities.cpp
	       CShadowPartManager.cpp               MAttachCreatureEffectGroundGenerator.cpp    MHelpDisplayer.cpp                   MRippleZoneEffectGenerator.cpp       MTestDef.cpp                                   TalkBox.cpp
	       CSoundPartManager.cpp                MAttachCreatureOrbitEffectGenerator.cpp     MHelpManager.cpp                     MRippleZonePixelEffectGenerator.cpp  MTopView.cpp                                   TempInformation.cpp
	       CSpritePal.cpp                       MAttachEffect.cpp                           MHelpMessageManager.cpp              MRippleZoneWideEffectGenerator.cpp   MTopViewDraw.cpp                               UIDialog.cpp
	       CSpriteTexturePartManager.cpp        MAttachOrbitEffect.cpp                      MHelpStringTable.cpp                 MRisingEffectGenerator.cpp           MTopviewExceptionProc.cpp                      UIMessageManager.cpp
	       CSystemInfo.cpp                      MAttachZoneEffectGenerator.cpp              MHomingEffect.cpp                    MScreenEffect.cpp                    MTradeManager.cpp                              UserInformation.cpp
	       CTexturePartManager.cpp              MAttachZoneSelectableEffectGenerator.cpp    MImageObject.cpp                     MScreenEffectManager.cpp             MVampireGear.cpp                               UserOption.cpp
	       CToken.cpp                           MAttackCreatureEffectGenerator.cpp          MInteractionObject.cpp               MSector.cpp                          MWeather.cpp                                   UtilityFunction.cpp
	       CUsingColorSetTable.cpp              MAttackCreatureHomingEffectGenerator.cpp    MInteractionObjectTable.cpp          MShadowAnimationObject.cpp           MWorkNode.cpp                                  VolmeLib_PCH.cpp
	       CWaitPacketUpdate.cpp                MAttackCreatureParabolaEffectGenerator.cpp  MInternetConnection.cpp              MShadowObject.cpp                    MWorkThread.cpp                                VolumeOutMaster.cpp
	       CWaitUIUpdate.cpp                    MAttackZoneBombEffectGenerator.cpp          MInventory.cpp                       MShop.cpp                            MZLib_PCH.cpp                                  VolumeOutWave.cpp
	       Client.cpp                           MAttackZoneEffectGenerator.cpp              MItem.cpp                            MShopShelf.cpp                       MZone.cpp                                      WavePackFileManager.cpp
	       ClientConfig.cpp                     MAttackZoneParabolaEffectGenerator.cpp      MItemManager.cpp                     MShopTemplate.cpp                    MZoneSound.cpp                                 WhisperManager.cpp
	       ClientFunction.cpp                   MAttackZoneRectEffectGenerator.cpp          MItemOptionTable.cpp                 MSkillInfoTable.cpp                  MZoneSoundManager.cpp                          ZoneFileHeader.cpp
	       Client_PCH.cpp                       MBloodyWallEffectGenerator.cpp              MItemTable.cpp                       MSkillManager.cpp                    MZoneTable.cpp                                 header.cpp
	       CrashReport.cpp                      MBloodyWaveEffectGenerator.cpp              MItemTable_bak-2007-5-7.cpp          MSkipEffect.cpp                      MathTable.cpp                                  heckSystem.cpp
	       DEUtil.cpp                           MChaseEffect.cpp                            MJusticeAttackManager.cpp            MSkipEffectGenerator.cpp             MemoryPool.cpp                                 huffman.cpp
	       DebugInfo.cpp                        MChatManager.cpp                            MLevelNameTable.cpp                  MSlayerGear.cpp                      MitemTableInit.cpp                             md5.cpp
	       DebugKit.cpp                         MCompareManager.cpp                         MLinearEffect.cpp                    MSlotItemManager.cpp                 MitemTableinit2.cpp                            mp3.cpp
	       DirectXLib_PCH.cpp                   MCrashReportManager.cpp                     MLoadingSPKWorkNode.cpp              MSortedItemManager.cpp               ModifyStatusManager.cpp                        nProtect.cpp
	       DrawCreatureDivineGuidance.cpp       MCreature.cpp                               MMeteorDropEffectGenerator.cpp       MSoundTable.cpp                      MonsterNameTable.cpp                           reader.cpp
	       DrawCreatureEffect.cpp               MCreatureSpriteTable.cpp                    MMoneyManager.cpp                    MSpreadOutEffectGenerator.cpp        Netmarble.cpp                                  soundbuf.cpp
	       DrawCreatureShadow.cpp               MCreatureTable.cpp                          MMonsterKillQuestInfo.cpp            MStatus.cpp                          NicknameInfo.cpp                               subdecoder.cpp
	       EXECryptor.cpp                       MCreatureWear.cpp                           MMovingEffect.cpp                    MStatusManager.cpp                   PCConfigTable.cpp                              synfilt.cpp
	       ExpInfo.cpp                          MEffect.cpp                                 MMultipleFallingEffectGenerator.cpp  MStopInventoryEffectGenerator.cpp    PackFileManager.cpp
	       ExperienceTable.cpp                  MEffectGeneratorTable.cpp                   MMusic.cpp                           MStopZoneCrossEffectGenerator.cpp    PacketFunction.cpp
	       FameInfo.cpp                         MEffectManager.cpp                          MMusicTable.cpp                      MStopZoneEffectGenerator.cpp         ProfileManager.cpp)

add_subdirectory(FrameLib)
add_subdirectory(SpriteLib)
add_subdirectory(DXLib)
add_subdirectory(WinLib)

target_include_directories(darkeden PUBLIC Packet/Types FrameLib)

