//---------------------------------------------------------------------------
// GameInitInfo.cpp
//---------------------------------------------------------------------------
// Information Table에 대한 초기화를 한다.
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// IMPORTANT: Include Platform.h BEFORE PCH to get correct CRITICAL_SECTION size
// This prevents incomplete type issues with ProfileManager
#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif

#include "Client_PCH.h"
#ifdef PLATFORM_WINDOWS
#include <MMSystem.h>
#endif
//#include "MFileDef.h"
#include "Properties.h"
#include "Client.h"
//#include "AddonDef.h"
#include "GameObject.h"
#include "UIDialog.h"
#include "MChatManager.h"
#include "MHelpManager.h"
#include "MHelpStringTable.h"
#include "MCompareManager.h"
#include "MHelpDisplayer.h"
#include "MLevelNameTable.h"
#include "MonsterNameTable.h"
#include "ExperienceTable.h"
#include "MNPCScriptTable.h"
#include "ModifyStatusManager.h"
#include "AcceleratorDef.h"
#include "KeyAccelerator.h"
#include "AcceleratorManager.h"
#include "ProfileManager.h"
#include "MGuildMarkManager.h"
#include "MGuildInfoMapper.h"
#include "MEventManager.h"
#include "MJusticeAttackManager.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "RankBonusTable.h"
#include "MWarManager.h"
#include "FameInfo.h"	
#include "MTimeItemManager.h"
#include "MMonsterKillQuestInfo.h"
#include "ShrineInfoManager.h"

#include "md5.h" //add by viva

#ifdef OUTPUT_DEBUG
	//#define	SAVE_TO_FILE_CONFIG
	//#define SAVE_TO_FILE_INFO
#endif

//#define	new		DEBUG_NEW


//-----------------------------------------------------------------------------
// Init Config
//-----------------------------------------------------------------------------
BOOL
InitClientConfig()
{
	// Debug Message
	DEBUG_ADD("[ InitGame ]  ClientConfig");
	
	//---------------------------------------------------------------------
	// 메모리 잡기
	//---------------------------------------------------------------------
	if (g_pClientConfig==NULL)
	{
		g_pClientConfig = new ClientConfig;
	}

	//---------------------------------------------------------------------
	//
	//			Client Config 설정
	//
	//---------------------------------------------------------------------
	///*
	//---------------------------------------------------------------------
	// Load
	//---------------------------------------------------------------------
	if (!g_pClientConfig->LoadFromFile(g_pFileDef->getProperty("FILE_INFO_CLIENTCONFIG").c_str()))
	{
		DEBUG_ADD_FORMAT("[Error] File Open Error : [%s]", g_pFileDef->getProperty("FILE_INFO_CLIENTCONFIG").c_str());
		//InitFail("[Error] File Open Error : [%s]", FILE_INFO_CLIENTCONFIG);
		
		//return FALSE;
	}

	// 다시 한 번 저장해둔다.
	g_pClientConfig->SaveToFile(g_pFileDef->getProperty("FILE_INFO_CLIENTCONFIG").c_str());

	return TRUE;
}

//-----------------------------------------------------------------------------
// Init Config
//-----------------------------------------------------------------------------
BOOL
InitUserOption()
{
	// Debug Message
	DEBUG_ADD("[ InitGame ]  UserOption");
	
	//---------------------------------------------------------------------
	// 메모리 잡기
	//---------------------------------------------------------------------
	if (g_pUserOption==NULL)
	{
		g_pUserOption = new UserOption;
	}

	//---------------------------------------------------------------------
	//
	//			User Option 설정
	//
	//---------------------------------------------------------------------
	///*
	//*/
	//---------------------------------------------------------------------
	// Load
	//---------------------------------------------------------------------
	if (!g_pUserOption->LoadFromFile(g_pFileDef->getProperty("FILE_INFO_USEROPTION").c_str()))
	{
		DEBUG_ADD_FORMAT("[Error] File Open Error : [%s]", g_pFileDef->getProperty("FILE_INFO_USEROPTION").c_str());
		//InitFail("[Error] File Open Error : [%s]", FILE_INFO_USEROPTION);
		
		//return FALSE;
	}
	
	g_pUserOption->persnalShopupdatetime = GetPersnalShopUpdateTime();
	g_pUserOption->PlayWaveMusic = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Init Infomation
//-----------------------------------------------------------------------------
BOOL		
InitInfomation()
{
	// Debug Message
	DEBUG_ADD("[ InitGame ]  Information");
	
	//---------------------------------------------------------------------
	// 메모리 잡기
	//---------------------------------------------------------------------
	SAFE_DELETE( g_pRegenTowerInfoManager );
	SAFE_DELETE( g_pQuestInfoManager );
	SAFE_DELETE( g_pTimeItemManager  );
	SAFE_DELETE( g_pFameInfoTable  );
	SAFE_DELETE( g_pWarManager  );
	SAFE_DELETE( g_pKeyAccelerator );
	SAFE_DELETE( g_pAcceleratorManager );
	SAFE_DELETE( g_pModifyStatusManager );
	SAFE_DELETE( g_pEffectGeneratorTable );
	SAFE_DELETE( g_pUIDialog );
	SAFE_DELETE( g_pProfileManager );
	SAFE_DELETE( g_pGuildMarkManager );
	SAFE_DELETE( g_pGuildInfoMapper );
	SAFE_DELETE( g_pEventManager );
	SAFE_DELETE( g_pEffectSpriteTypeTable );
	SAFE_DELETE( g_pActionEffectSpriteTypeTable );
	SAFE_DELETE( g_pCreatureTable );
	SAFE_DELETE( g_pCreatureSpriteTypeMapper );
	SAFE_DELETE( g_pItemTable );
	SAFE_DELETE( g_pCreatureSpriteTable );
//	SAFE_DELETE( g_pCreatureActionSpriteTable );
//	SAFE_DELETE( g_pAddonSpriteTable );
	SAFE_DELETE( g_pEffectStatusTable );
	SAFE_DELETE( g_pInteractionObjectTable );
	SAFE_DELETE( g_pItemOptionTable );
	SAFE_DELETE( g_pNPCTable );
	SAFE_DELETE( g_pJusticeAttackManager );
	SAFE_DELETE( g_pNPCScriptTable );
	SAFE_DELETE( g_pShopTemplateTable );
	SAFE_DELETE( g_pSkillInfoTable );
	SAFE_DELETE( g_pRankBonusTable );
	SAFE_DELETE( g_pExperienceTable );
	SAFE_DELETE( g_pSoundTable );
	SAFE_DELETE( g_pMusicTable );
	SAFE_DELETE( g_pZoneTable );
	SAFE_DELETE( g_pSoundManager );
	SAFE_DELETE( g_pUsingColorSetTable );
	SAFE_DELETE( g_pActionInfoTable );
	SAFE_DELETE( g_pChatManager );
	SAFE_DELETE( g_pHelpManager );
	SAFE_DELETE( g_pHelpStringTable );
	SAFE_DELETE( g_pCompareManager );
	SAFE_DELETE( g_pHelpDisplayer );
	SAFE_DELETE( g_pLevelNameTable );
	SAFE_DELETE( g_pMonsterNameTable );

	if (g_pKeyAccelerator==NULL)
	{
		g_pKeyAccelerator = new KeyAccelerator;
		g_pKeyAccelerator->Init( MAX_ACCELERATOR );
		SetDefaultAccelerator();		// 일단은..
		//g_pKeyAccelerator->LoadFromFile( ) 이 나을 듯..
	}

	if (g_pWarManager == NULL)
	{
		g_pWarManager = new MWarManager;
	}
	
	if( g_pTimeItemManager == NULL)
	{
		g_pTimeItemManager = new MTimeItemManager;
	}

	if (g_pAcceleratorManager==NULL)
	{
		g_pAcceleratorManager = new AcceleratorManager;		
	}
	
	if (g_pModifyStatusManager==NULL)
	{
		g_pModifyStatusManager = new ModifyStatusManager;
	}

	if (g_pEffectGeneratorTable==NULL)
	{
		g_pEffectGeneratorTable = new MEffectGeneratorTable;
	}

	if (g_pUIDialog==NULL)
	{
		g_pUIDialog = new UIDialog;
	}

	if (g_pProfileManager==NULL)
	{
		g_pProfileManager = new ProfileManager;
	}

	if (g_pGuildInfoMapper==NULL)
	{
		g_pGuildInfoMapper = new MGuildInfoMapper;

		// 이 화일은 없을 수도 있다.
		std::ifstream guildInfoFile(g_pFileDef->getProperty("FILE_INFO_GUILD_INFO_MAPPER").c_str(), std::ios::binary);

		if (guildInfoFile.is_open())
		{
			g_pGuildInfoMapper->LoadFromFile(guildInfoFile);
			guildInfoFile.close();
		}		
	}

	if (g_pGuildMarkManager==NULL)
	{
		g_pGuildMarkManager = new MGuildMarkManager(g_pFileDef->getProperty("FILE_SPRITE_GUILD_MARK").c_str(), g_pFileDef->getProperty("FILE_INFO_GUILD_INFO_MAPPER").c_str(), g_pFileDef->getProperty("FILE_SPRITE_GRADE_MARK_SLAYER").c_str(), g_pFileDef->getProperty("FILE_SPRITE_GRADE_MARK_VAMPIRE").c_str(), g_pFileDef->getProperty("FILE_SPRITE_GRADE_MARK_OUSTERS").c_str(), g_pFileDef->getProperty("FILE_SPRITE_LEVEL_MARK_PET").c_str());
	}

	if (g_pEventManager==NULL)
	{
		g_pEventManager = new MEventManager;
	}

	if (g_pEffectSpriteTypeTable==NULL)
	{
		g_pEffectSpriteTypeTable = new EFFECTSPRITETYPE_TABLE;
#ifdef __SANITIZE_ADDRESS__
		// Update shadow copies for corruption detection
		extern EFFECTSPRITETYPE_TABLE* g_pEffectSpriteTypeTable_shadow;
		extern EFFECTSPRITETYPE_TABLE::TYPE* g_pEffectSpriteTypeTable_m_pTypeInfo_shadow;
		g_pEffectSpriteTypeTable_shadow = g_pEffectSpriteTypeTable;
		g_pEffectSpriteTypeTable_m_pTypeInfo_shadow = g_pEffectSpriteTypeTable->GetInternalPointer();
#endif
	}

	if (g_pActionEffectSpriteTypeTable==NULL)
	{
		g_pActionEffectSpriteTypeTable = new MActionEffectSpriteTypeTable;
	}

	if (g_pCreatureTable==NULL)
	{
		g_pCreatureTable = new CREATURE_TABLE;
	}

	if (g_pCreatureSpriteTypeMapper==NULL)
	{
		g_pCreatureSpriteTypeMapper = new CreatureSpriteTypeMapper;
	}

	if (g_pItemTable==NULL)
	{
		g_pItemTable = new ITEMCLASS_TABLE;
	}

	if (g_pEffectSpriteTypeTable==NULL)
	{
		g_pEffectSpriteTypeTable = new EFFECTSPRITETYPE_TABLE;
#ifdef __SANITIZE_ADDRESS__
		// Update shadow copies for corruption detection
		extern EFFECTSPRITETYPE_TABLE* g_pEffectSpriteTypeTable_shadow;
		extern EFFECTSPRITETYPE_TABLE::TYPE* g_pEffectSpriteTypeTable_m_pTypeInfo_shadow;
		g_pEffectSpriteTypeTable_shadow = g_pEffectSpriteTypeTable;
		g_pEffectSpriteTypeTable_m_pTypeInfo_shadow = g_pEffectSpriteTypeTable->GetInternalPointer();
#endif
	}

	if (g_pCreatureSpriteTable==NULL)
	{
		g_pCreatureSpriteTable = new CREATURESPRITE_TABLE;
	}

//	if (g_pCreatureActionSpriteTable==NULL)
//	{
//		g_pCreatureActionSpriteTable = new CREATURESPRITE_TABLE;
//	}
//
//	if (g_pAddonSpriteTable==NULL)
//	{
//		g_pAddonSpriteTable = new CREATURESPRITE_TABLE;
//	}

	if (g_pEffectStatusTable==NULL)
	{
		g_pEffectStatusTable = new EFFECTSTATUS_TABLE;
	}

	if (g_pInteractionObjectTable==NULL)
	{
		g_pInteractionObjectTable = new INTERACTIONOBJECT_TABLE;
	}

	if (g_pItemOptionTable==NULL)
	{
		g_pItemOptionTable = new ITEMOPTION_TABLE;
	}

	if (g_pNPCTable==NULL)
	{
		g_pNPCTable = new MNPCTable;
	}

	if (g_pJusticeAttackManager==NULL)
	{
		g_pJusticeAttackManager = new MJusticeAttackManager;
	}

	if (g_pNPCScriptTable==NULL)
	{
		g_pNPCScriptTable = new MNPCScriptTable;
	}

	if (g_pShopTemplateTable==NULL)
	{
		g_pShopTemplateTable = new MShopTemplateTable;
	}

	if (g_pSkillInfoTable==NULL)
	{
		g_pSkillInfoTable = new MSkillInfoTable;
	}

	if (g_pRankBonusTable==NULL)
	{
		g_pRankBonusTable = new RankBonusTable;
	}

	if (g_pExperienceTable==NULL)
	{
		g_pExperienceTable = new ExperienceTable;
	}

	if (g_pSoundTable==NULL)
	{
		g_pSoundTable = new SOUND_TABLE;
	}

	if (g_pMusicTable==NULL)
	{
		g_pMusicTable = new MUSIC_TABLE;
	}

	if (g_pZoneTable==NULL)
	{
		g_pZoneTable = new CZoneTable;
	}

	if (g_pSoundManager==NULL)
	{
		g_pSoundManager = new CSoundPartManager;
	}

	if (g_pUsingColorSetTable==NULL)
	{
		g_pUsingColorSetTable = new USINGCOLORSET_TABLE;
	}

	if (g_pActionInfoTable==NULL)
	{
		g_pActionInfoTable = new MActionInfoTable;
	}

	if (g_pChatManager==NULL)
	{
		g_pChatManager = new MChatManager;
	}
	
	if (g_pHelpManager==NULL)
	{
		g_pHelpManager = new MHelpManager;
	}

	if (g_pHelpStringTable==NULL)
	{
		g_pHelpStringTable= new MHelpStringTable;
	}

	if (g_pCompareManager==NULL)
	{
		g_pCompareManager= new MCompareManager;
	}

	if (g_pHelpDisplayer==NULL)
	{
		g_pHelpDisplayer = new MHelpDisplayer;
	}

	if (g_pLevelNameTable==NULL)
	{
		g_pLevelNameTable = new MLevelNameTable;
	}

	if (g_pMonsterNameTable==NULL)
	{
		g_pMonsterNameTable = new MonsterNameTable;
	}

	if (g_pFameInfoTable == NULL)
	{
		g_pFameInfoTable = new FameInfoTable;
	}

	if (g_pQuestInfoManager == NULL)
	{
		g_pQuestInfoManager = new MQuestInfoManager;
	}

	if( g_pRegenTowerInfoManager == NULL )
	{
		g_pRegenTowerInfoManager = new RegenTowerInfoManager();
		if( g_pRegenTowerInfoManager->LoadRegenTowerInfo() == false )
		{
			MessageBox(NULL,"Cannot Open RTI File","Error",MB_OK);
			return FALSE;			
		}
	}


	//int i;
	//---------------------------------------------------------------------
	//
	//			Global 정보 설정
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - PacketItemTable");

	InitPacketItemTable();

	//---------------------------------------------------------------------
	//
	//          class에 있는 정보 설정..
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - InitMoveTable");
	
	MCreature::InitMoveTable();

	DEBUG_ADD("[ InitGame ]  Information - UIDialog-Init");
	
	g_pUIDialog->Init();

	//---------------------------------------------------------------------
	//
	//      EffectGenerator 생성
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - EffectGenerator");
	
	(*g_pEffectGeneratorTable).Init();

	//---------------------------------------------------------------------
	//
	//		Chat Manager Loading
	//
	//---------------------------------------------------------------------
	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - ChatManager");
	std::ifstream chatInfoFile2;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_CHAT").c_str(), chatInfoFile2))
		return FALSE;
	chatInfoFile2.close();		// file체크용. - -;

	g_pChatManager->LoadFromFile(g_pFileDef->getProperty("FILE_INFO_CHAT").c_str());
	
	//---------------------------------------------------------------------
	//
	//		Help Manager , Help String Table
	//
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	// Help Manager
	//---------------------------------------------------------------------
//	DEBUG_ADD("[ InitGame ]  Information - HelpManager");
//	std::ifstream helpManagerFile;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
//	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SMART_HELP").c_str(), helpManagerFile))
//		return FALSE;
//	g_pHelpManager->LoadFromFile( helpManagerFile );
//	helpManagerFile.close();

	//---------------------------------------------------------------------
	// Help String Table
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - HelpString");
	std::ifstream helpStringTableFile;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SMART_HELP_STRING").c_str(), helpStringTableFile))
		return FALSE;
	g_pHelpStringTable->LoadFromFile( helpStringTableFile );
	helpStringTableFile.close();
	
	
	//---------------------------------------------------------------------
	//
	//						LevelName Table
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - LevelName");

	std::ifstream levelNameTableFile;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_LEVEL_NAME").c_str(), levelNameTableFile))
		return FALSE;
	g_pLevelNameTable->LoadFromFile( levelNameTableFile );
	levelNameTableFile.close();

	//---------------------------------------------------------------------
	//
	//						MonsterName Table
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - MonsterName");

	std::ifstream MonsterNameTableFile;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_MONSTER_NAME").c_str(), MonsterNameTableFile))
		return FALSE;
	g_pMonsterNameTable->LoadFromFile( MonsterNameTableFile );
	MonsterNameTableFile.close();
	

	//---------------------------------------------------------------------
	//
	//          UsingColor 정보 생성
	//
	//---------------------------------------------------------------------
	/*
	#ifdef SAVE_TO_FILE_INFO
		(*g_pUsingColorSetTable).Init( MAX_USINGCOLORSET );

		(*g_pUsingColorSetTable)[USINGCOLORSET_NONE].Disable(0, MAX_USINGCOLORSET);
		(*g_pUsingColorSetTable)[USINGCOLORSET_ALL].Enable(0, MAX_USINGCOLORSET);
		
		for (int i=0; i<MAX_COLORSET; i++)
		{
			(*g_pUsingColorSetTable)[USINGCOLORSET_SKIN].Enable(i*MAX_COLORSET_SEED_MODIFY);
			(*g_pUsingColorSetTable)[USINGCOLORSET_SKIN].Enable(i*MAX_COLORSET_SEED_MODIFY + 1);
			(*g_pUsingColorSetTable)[USINGCOLORSET_SKIN].Enable(i*MAX_COLORSET_SEED_MODIFY + 2);
		}

		for (int i=0; i<MAX_COLORSET; i++)
		{
			(*g_pUsingColorSetTable)[USINGCOLORSET_ITEM].Enable(i*MAX_COLORSET_SEED_MODIFY);
			(*g_pUsingColorSetTable)[USINGCOLORSET_ITEM].Enable(i*MAX_COLORSET_SEED_MODIFY + 7);
			(*g_pUsingColorSetTable)[USINGCOLORSET_ITEM].Enable((i+1)*MAX_COLORSET_SEED_MODIFY - 1);
		}

		//------------------------------------------------
		// Save
		//------------------------------------------------
		std::ofstream usingColorSetTable(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
		(*g_pUsingColorSetTable).SaveToFile(usingColorSetTable);
		usingColorSetTable.close();
	#endif
	*/

	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - UsingColorSet");

	std::ifstream usingColorSetTable2;//(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_USINGCOLORSET").c_str(), usingColorSetTable2))
		return FALSE;
	(*g_pUsingColorSetTable).LoadFromFile(usingColorSetTable2);
	usingColorSetTable2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - SoundTable");

	std::ifstream soundTable2;//(g_pFileDef->getProperty("FILE_INFO_SOUND").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SOUND").c_str(), soundTable2))
		return FALSE;
	(*g_pSoundTable).LoadFromFile(soundTable2);
	soundTable2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - MusicTable");

	std::ifstream musicTable2;//(g_pFileDef->getProperty("FILE_INFO_MUSIC").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_MUSIC").c_str(), musicTable2))
		return FALSE;
	(*g_pMusicTable).LoadFromFile(musicTable2);
	musicTable2.close();

	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - ZoneTable");

	std::ifstream zoneTable2;//(g_pFileDef->getProperty("FILE_INFO_ZONE").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ZONE").c_str(), zoneTable2))
		return FALSE;
	(*g_pZoneTable).LoadFromFile(zoneTable2);
	zoneTable2.close();

	
	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD_FORMAT("[ InitGame ]  Information - ItemTable(%s)", g_pFileDef->getProperty("FILE_INFO_ITEM").c_str());	
	std::ifstream itemTable2;//(g_pFileDef->getProperty("FILE_INFO_ITEM").c_str(), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ITEM").c_str(), itemTable2))
		return FALSE;
	(*g_pItemTable).LoadFromFile(itemTable2);
	itemTable2.close();

	DEBUG_ADD("[ InitGame ]  Information - ItemOptionTable");

	std::ifstream itemOptionTable2(g_pFileDef->getProperty("FILE_INFO_ITEMOPTION").c_str(), ios::binary);
	(*g_pItemOptionTable).LoadFromFile( itemOptionTable2 );
	itemOptionTable2.close();

	// 0을 추가한다.
	/*
	ITEMOPTION_TABLE ItemOptionTable;
	ItemOptionTable.Init( (*g_pItemOptionTable).GetSize()+1 );

	ItemOptionTable[0].Part = 0;
	ItemOptionTable[0].Name = "NONE";
	ItemOptionTable[0].PlusPoint = 0;
	ItemOptionTable[0].PlusRequireAbility = 0;
	ItemOptionTable[0].Worth = 0;
	ItemOptionTable[0].ColorSet = 0;

	for (int o=0; o<(*g_pItemOptionTable).GetSize(); o++)
	{
		ItemOptionTable[o+1] = (*g_pItemOptionTable)[o];
	}

	std::ofstream itemOptionTable(g_pFileDef->getProperty("FILE_INFO_ITEMOPTION").c_str(), ios::binary);
	ItemOptionTable.SaveToFile( itemOptionTable );
	itemOptionTable.close();
	*/

	//---------------------------------------------------------------------
	//
	//    InteractionObjectTable에 대한 정보 설정
	//
	//---------------------------------------------------------------------
	/*
#ifdef SAVE_TO_FILE_INFO
	(*g_pInteractionObjectTable).Init( 2 );

	(*g_pInteractionObjectTable)[0].Type	= MInteractionObject::TYPE_DOOR;
	(*g_pInteractionObjectTable)[0].FrameID = 0;
	(*g_pInteractionObjectTable)[0].SoundID = 10;

	(*g_pInteractionObjectTable)[1].Type	= MInteractionObject::TYPE_TRAP;
	(*g_pInteractionObjectTable)[1].FrameID = 1;
	(*g_pInteractionObjectTable)[1].SoundID = 11;
	
	//------------------------------------------------
	// Save
	//------------------------------------------------
	std::ofstream interactionObjectTable(g_pFileDef->getProperty("FILE_INFO_INTERACTIONOBJECT"), ios::binary);
	(*g_pInteractionObjectTable).SaveToFile(interactionObjectTable);
	interactionObjectTable.close();
#endif
	*/

	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - InteractionObjectTable");

	std::ifstream interactionObjectTable2;//(g_pFileDef->getProperty("FILE_INFO_INTERACTIONOBJECT"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_INTERACTIONOBJECT").c_str(), interactionObjectTable2))
		return FALSE;
	(*g_pInteractionObjectTable).LoadFromFile(interactionObjectTable2);
	interactionObjectTable2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	std::ifstream creatureSpriteTable2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_CREATURESPRITE").c_str(), creatureSpriteTable2))
		return FALSE;
	g_pCreatureSpriteTable->LoadFromFile(creatureSpriteTable2);
	creatureSpriteTable2.close();

	//------------------------------------------------
	// Addon SpriteTable
	//------------------------------------------------
//	std::ifstream addonSpriteTable2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
//	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ADDONSPRITE").c_str(), addonSpriteTable2))
//		return FALSE;
//	g_pAddonSpriteTable->LoadFromFile(addonSpriteTable2);
//	addonSpriteTable2.close();

	//------------------------------------------------
	// CreatureAction SpriteTable
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - CreatureActionSpriteTable");

//	std::ifstream creatureActionSpriteTable2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
//	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_CREATUREACTIONSPRITE"), creatureActionSpriteTable2))
//		return FALSE;
//	g_pCreatureActionSpriteTable->LoadFromFile(creatureActionSpriteTable2);
//	creatureActionSpriteTable2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - CreatureTable");

	std::ifstream creatureTable2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_CREATURE").c_str(), creatureTable2))
		return FALSE;
	(*g_pCreatureTable).LoadFromFile(creatureTable2);
	creatureTable2.close();


	//---------------------------------------------------------------------
	//
	//		CreatureSpriteTypeMapper
	//
	//---------------------------------------------------------------------	
	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - CreatureSpriteTypeMapper");

	std::ifstream CreatureSpriteTypeMapper2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_CREATURESPRITE_MAPPER").c_str(), CreatureSpriteTypeMapper2))
		return FALSE;
	g_pCreatureSpriteTypeMapper->LoadFromFile(CreatureSpriteTypeMapper2);
	CreatureSpriteTypeMapper2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - ShopTemplate");

	std::ifstream ShopTemplateTable2;//(g_pFileDef->getProperty("FILE_INFO_SHOPTEMPLATE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SHOPTEMPLATE").c_str(), ShopTemplateTable2))
		return FALSE;
	(*g_pShopTemplateTable).LoadFromFile(ShopTemplateTable2);
	ShopTemplateTable2.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - NPCTable");

	std::ifstream NPCTable2;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_NPC").c_str(), NPCTable2))
		return FALSE;
	(*g_pNPCTable).LoadFromFile(NPCTable2);
	NPCTable2.close();

	//---------------------------------------------------------------------
	//
	//    NPC Script에 대한 정보 설정
	//
	//---------------------------------------------------------------------	

	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - NPCScript");

	std::ifstream NPCScriptTable;//(g_pFileDef->getProperty("FILE_INFO_CREATURE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_NPC_SCRIPT_INFO").c_str(), NPCScriptTable))
		return FALSE;
	g_pNPCScriptTable->LoadFromFile(NPCScriptTable);
	NPCScriptTable.close();


	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - EffectSpriteTypeTable");

	std::ifstream effectSpriteTypeTable2;//(g_pFileDef->getProperty("FILE_INFO_EFFECTSPRITETYPE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_EFFECTSPRITETYPE").c_str(), effectSpriteTypeTable2))
		return FALSE;
	(*g_pEffectSpriteTypeTable).LoadFromFile(effectSpriteTypeTable2);
	effectSpriteTypeTable2.close();

#ifdef __SANITIZE_ADDRESS__
	// CRITICAL FIX: Update shadow copies after LoadFromFile reallocates m_pTypeInfo
	// LoadFromFile calls Init() which reallocates the internal array
	// Without updating shadows, corruption detection gives false positives
	extern EFFECTSPRITETYPE_TABLE* g_pEffectSpriteTypeTable_shadow;
	extern EFFECTSPRITETYPE_TABLE::TYPE* g_pEffectSpriteTypeTable_m_pTypeInfo_shadow;
	g_pEffectSpriteTypeTable_shadow = g_pEffectSpriteTypeTable;
	g_pEffectSpriteTypeTable_m_pTypeInfo_shadow = g_pEffectSpriteTypeTable->GetInternalPointer();
#endif

	//------------------------------------------------
	// Load
	//------------------------------------------------
	std::ifstream actionEffectSpriteTypeTable2;//(g_pFileDef->getProperty("FILE_INFO_EFFECTSPRITETYPE"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ACTIONEFFECTSPRITETYPE").c_str(), actionEffectSpriteTypeTable2))
		return FALSE;
	g_pActionEffectSpriteTypeTable->LoadFromFile(actionEffectSpriteTypeTable2);
	actionEffectSpriteTypeTable2.close();



	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - EffectStatusTable");

	std::ifstream EffectStatusTable2;//(g_pFileDef->getProperty("FILE_INFO_EffectStatus"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_EFFECTSTATUS").c_str(), EffectStatusTable2))
		return FALSE;
	(*g_pEffectStatusTable).LoadFromFile(EffectStatusTable2);
	EffectStatusTable2.close();



	//---------------------------------------------------------------------
	//
	//    SkillInfoTable에 대한 정보 설정
	//
	//---------------------------------------------------------------------
	//------------------------------------------------
	// Load
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - SkillInfoTable");

	std::ifstream skillTable2;//(g_pFileDef->getProperty("FILE_INFO_skill"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SKILL").c_str(), skillTable2))
		return FALSE;
	g_pSkillInfoTable->LoadFromFile(skillTable2);
	skillTable2.close();

	//------------------------------------------------
	// Server 정보를 loading한다.
	//------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - ServerSkillInfo");

	std::ifstream serverSkillInfoFile;//(g_pFileDef->getProperty("FILE_INFO_skill"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SKILL_INFO").c_str(), serverSkillInfoFile))
		return FALSE;

	g_pSkillInfoTable->LoadFromFileServerSkillInfo( serverSkillInfoFile );
	serverSkillInfoFile.close();

	//---------------------------------------------------------------------
	//
	//    RankBonusTable에 대한 정보 설정
	//
	//---------------------------------------------------------------------
	DEBUG_ADD("[ InitGame ]  Information - rankBonusInfoFile");

	std::ifstream rankBonusInfoFile;//(g_pFileDef->getProperty("FILE_INFO_skill"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_RANK_BONUS").c_str(), rankBonusInfoFile))
		return FALSE;

	g_pRankBonusTable->LoadFromFile( rankBonusInfoFile );
	rankBonusInfoFile.close();

	std::ifstream fameInfoFile;

	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_FAME_LIMIT").c_str(), fameInfoFile))
		return FALSE;

	g_pFameInfoTable->LoadFromFile( fameInfoFile );
	fameInfoFile.close();


	//---------------------------------------------------------------------
	//
	//    ExperienceTable에 대한 정보 설정
	//
	//---------------------------------------------------------------------
	//------------------------------------------------
	// Load
	//------------------------------------------------
	std::ifstream strExpFile;
	std::ifstream dexExpFile;
	std::ifstream intExpFile;
	std::ifstream vampireExpFile;
	std::ifstream oustersExpFile;
	std::ifstream slayerRankExpFile;
	std::ifstream vampireRankExpFile;
	std::ifstream oustersRankExpFile;
	std::ifstream advancementExp;
	std::ifstream petExpFile;

	//------------------------------------------------
	// STR
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_STR_EXP").c_str(), strExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - STR");
	g_pExperienceTable->LoadFromFileSTR(strExpFile);

	//------------------------------------------------
	// DEX
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_DEX_EXP").c_str(), dexExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - DEX");
	g_pExperienceTable->LoadFromFileDEX(dexExpFile);

	//------------------------------------------------
	// INT
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_INT_EXP").c_str(), intExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - INT");
	g_pExperienceTable->LoadFromFileINT(intExpFile);

	//------------------------------------------------
	// Vampire
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_VAMPIRE_EXP").c_str(), vampireExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - Vampire");
	g_pExperienceTable->LoadFromFileVampire(vampireExpFile);
	
	//------------------------------------------------
	// Ousters
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_OUSTERS_EXP").c_str(), oustersExpFile))
		return FALSE;
	
	DEBUG_ADD("[ InitGame ]  Information - Ousters");
	g_pExperienceTable->LoadFromFileOusters(oustersExpFile);
	
	//------------------------------------------------
	// Slayer Rank
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_SLAYER_RANK_EXP").c_str(), slayerRankExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - Slayer Rank");
	g_pExperienceTable->LoadFromFileSlayerRank(slayerRankExpFile);
	
	//------------------------------------------------
	// Vampire Rank
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_VAMPIRE_RANK_EXP").c_str(), vampireRankExpFile))
		return FALSE;

	DEBUG_ADD("[ InitGame ]  Information - Vampire Rank");
	g_pExperienceTable->LoadFromFileVampireRank(vampireRankExpFile);
	
	//------------------------------------------------
	// Ousters Rank
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_OUSTERS_RANK_EXP").c_str(), oustersRankExpFile))
		return FALSE;
	
	DEBUG_ADD("[ InitGame ]  Information - Ousters Rank");
	g_pExperienceTable->LoadFromFileOustersRank(oustersRankExpFile);

	//------------------------------------------------
	// PetExp
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_PET_EXP_INFO").c_str(), petExpFile))
		return FALSE;
	
	DEBUG_ADD("[ InitGame ]  Information - Ousters Rank");
	g_pExperienceTable->LoadFromFilePetExp(petExpFile);




	//------------------------------------------------
	// Advancement
	//------------------------------------------------
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_ADVANCE_EXP_INFO").c_str(), advancementExp))
		return FALSE;
	
	DEBUG_ADD("[ InitGame ]  Information - Ousters Rank");
	g_pExperienceTable->LoadFromFileAdvanceMent(advancementExp);
	

	petExpFile.close();
	strExpFile.close();
	dexExpFile.close();
	intExpFile.close();
	vampireExpFile.close();
	oustersExpFile.close();
	slayerRankExpFile.close();
	vampireRankExpFile.close();
	oustersRankExpFile.close();
	advancementExp.close();


	//-----------------------------------
	// Load From File
	//-----------------------------------
	DEBUG_ADD("[ InitGame ]  Information - ActionInfoTable");

	std::ifstream actionInfoFile2;//(g_pFileDef->getProperty("FILE_INFO_ACTION"), ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_ACTION").c_str(), actionInfoFile2))
		return FALSE;
	g_pActionInfoTable->LoadFromFile(actionInfoFile2);
	actionInfoFile2.close();

	DEBUG_ADD("[ InitGame ]  Information - MonsterKillQuest");
	std::ifstream MonsterKillQuestInfoFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_MONSTER_KILL_QUEST").c_str(), MonsterKillQuestInfoFile))
		return FALSE;

	DWORD QuestVer = 0;
	MonsterKillQuestInfoFile.read((char*)&QuestVer, sizeof(DWORD) );
	g_pQuestInfoManager->LoadFromFile( MonsterKillQuestInfoFile );
	MonsterKillQuestInfoFile.close();
	DEBUG_ADD("[ InitGame ]  Information - All Clear");

	return TRUE;
}
