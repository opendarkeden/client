//----------------------------------------------------------------------
//
// Filename    : GCUpdateInfoHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "MTopView.h"
#include "GCUpdateInfo.h"
#include "ClientDef.h"
#include "AddonDef.h"
#include "MGameTime.h"
#include "MQuickSlot.h"
#include "MEffectStatusDef.h"
#include "MSkillManager.h"
#include "MMoneyManager.h"
#include "CDirectSound.h"
#include "COrderedList.h"
#include "MZoneTable.h"
#include "UIDialog.h"
#include "TalkBox.h"
#include "MWeather.h"
#include "MStorage.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "ServerInformation.h"
#include "UserInformation.h"
#include "MEventManager.h"
#include "UIMessageManager.h"
#include "ServerInfo.h"
#include "ClientPlayer.h"
#include "UIFunction.h"
#include "MGuildInfoMapper.h"
#include "CSystemInfo.h"

#ifdef __GAME_CLIENT__

	#include "ClientPlayer.h"
	#include "PCSlayerInfo2.h"
	#include "PCVampireInfo2.h"
	#include "PCOustersInfo2.h"
	#include "Cpackets/CGReady.h"
	#include "VS_UI_GameCommon2.h"
	#include "MGameStringTable.h"
#endif



extern DWORD		g_CurrentTime;
extern DWORD		g_CurrentFrame;
extern MGameTime*	g_pGameTime;
extern CMessageArray*			g_pNoticeMessage;

extern int			g_nZoneLarge;
extern MZone*		g_pZoneLarge;				
extern bool			g_bZonePlayerInLarge;

extern bool			LoadingAddonSPK( bool bLoadingAll );
extern bool				g_bHALAvailable;
extern DWORD			g_dwVideoMemory;
extern BOOL g_bEnable3DHAL;
extern WORD				g_ZoneCreatureColorSet;
extern WORD				g_MyBatColorSet;


DWORD	g_PreviousCreatureType = -1;
int		g_MorphCreatureType = 0;

//----------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ���� �����κ��� GCUpdateInfo ��Ŷ�� �ް� �Ǹ�,
// ��Ŷ ���� �����͵��� Ŭ���̾�Ʈ�� ������ ��, ������ �ε���
// ���� ���� ���� ������ CGReady ��Ŷ�� ������ �ȴ�.
//----------------------------------------------------------------------
void GCUpdateInfoHandler::execute ( GCUpdateInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	#ifndef __DEBUG_OUTPUT__ 
		DEBUG_ADD("Execute UpdateInfo");
	#endif

	// EventManager�� Zone���� �̺�Ʈ�� �����Ѵ�
	g_pEventManager->RemoveAllEventByType(EVENTTYPE_ZONE);

	//-----------------------------------------------------------
	// �ٸ� �κп��� mode�� �����ϴ� ��찡 �־
	// mode�� �������ش�.
	//-----------------------------------------------------------
	CLIENT_MODE	previousMode = g_Mode;
	g_Mode = MODE_WAIT_UPDATEINFO;

	//UI_SaveUserOption();

	//-----------------------------------------------------------
	// ���� server�� ���� ǥ��
	//-----------------------------------------------------------
	if (g_pServerInformation!=NULL)
	{
		g_pServerInformation->SetServerGroupStatus( pPacket->getServerStat() );
	}

	//-----------------------------------------------------------
	// AddonSPK�� ���� �ε��ϴµ�..
	// ���⼭�� loading �ȵ� �κ��� �� loading�ؾ� �Ѵ�. 2001.8.20
	//-----------------------------------------------------------
	LoadingAddonSPK( true );

//	g_pUserOption->LoadFromFile( FILE_INFO_USEROPTION );
//	UI_AffectUserOption();
	

	int i;

	//-----------------------------------------------------------
	// Sound�� ������� �ʴ´�.
	//-----------------------------------------------------------
	g_DXSound.SetMute();

	//-----------------------------------------------------------
	// updateInfo�� ó���ϱ� �����Ҷ��� �ð��� �����Ѵ�.
	//-----------------------------------------------------------
	DWORD	startTime	= timeGetTime();
	
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	//--------------------------------------------------
	// ������ �ε��� �����Ѵ�.
	//--------------------------------------------------
	pClientPlayer->setPlayerStatus( CPS_WAITING_FOR_LOADING );

	
	//--------------------------------------------------
	//
	//			 Player ���� ����
	//
	//--------------------------------------------------
	Race wasInterfaceRace = UI_GetInterfaceRace();//TRUE;
	int spActionInfo = ACTIONINFO_NULL;
	g_PreviousCreatureType = -1;

	if (g_pPlayer!=NULL)
	{	
		if (g_MorphCreatureType!=0)
		{
			g_PreviousCreatureType = g_MorphCreatureType;
			g_MorphCreatureType = 0;
		}
		else
		{
			g_PreviousCreatureType = g_pPlayer->GetCreatureType();
		}

		//--------------------------------------------------
		// interface ���濡 ���Ͽ�..
		//--------------------------------------------------
		/*
		if (g_pPlayer->IsVampire())
		{
			wasSlayerInterface = FALSE;
		}
		*/
		// �׾�����, dialog �� �ݱ�
		if (g_pPlayer->IsDead())
		{
			//UI_CloseAllDialog();
		}
		

		//--------------------------------------------------
		// ������ ������̴� ��� ����..
		//--------------------------------------------------
		spActionInfo = g_pPlayer->GetSpecialActionInfo();
		if (g_pZone!=NULL)
		{
			g_pZone->RemoveCreature( g_pPlayer->GetID() );
		}
		delete g_pPlayer;
	}	
	g_pPlayer = new MPlayer;
	DEBUG_ADD_FORMAT("[g_pPlayer] %x", g_pPlayer);

	//--------------------------------------------------
	//
	//		���� �ʱ�ȭ
	//
	//--------------------------------------------------
	DEBUG_ADD( "Remove All Addon" );
	
	for (i=0; i<ADDON_MAX; i++)
	{
		g_pPlayer->RemoveAddon((enum ADDON)i);
	}			

	g_pPlayer->SetBasicActionInfo( SKILL_ATTACK_MELEE );
	g_pPlayer->SetBasicAttackDistance( 1 );
		


	//--------------------------------------------------
	//
	//	Skill Info Table �ʱ�ȭ
	//
	//--------------------------------------------------
	DEBUG_ADD( "SkillInfoTable Init" );
	g_pSkillInfoTable->Init();

	//--------------------------------------------------
	//
	// Skill Tree �ʱ�ȭ
	//
	//--------------------------------------------------
	DEBUG_ADD( "SkillManager Init" );
	g_pSkillManager->Init();	

	g_PreviousCreatureType = -1;
	g_MorphCreatureType = 0;

	EffectInfo *pEInfo = new EffectInfo;	
	*pEInfo = *pPacket->getEffectInfo();
	
	if( pEInfo != NULL )
	{
		for( int i=0;i<pEInfo->getListNum();i++)
		{
			EFFECTSTATUS	status = (EFFECTSTATUS) pEInfo->popFrontListElement();
			WORD			delay = pEInfo->popFrontListElement();
			
			if( status == EFFECTSTATUS_TRANSFORM_TO_BAT )
			{
				g_PreviousCreatureType = CREATURETYPE_BAT;
			} else
			if( status == EFFECTSTATUS_TRANSFORM_TO_WOLF )
			{
				g_PreviousCreatureType = CREATURETYPE_WOLF;
			} else
			if( status == EFFECTSTATUS_TRANSFORM_TO_WERWOLF )
			{
				g_PreviousCreatureType = CREATURETYPE_WER_WOLF;
			}
		}
		delete pEInfo;
	}

	g_pGuildInfoMapper->ClearGuildName();

	//--------------------------------------------------
	//
	//			 player�� ������ �����Ѵ�.
	//
	//--------------------------------------------------		
	switch (pPacket->getPCInfo()->getPCType())
	{
		//--------------------------------------------------
		// Slayer�� ���
		//--------------------------------------------------
		case PC_SLAYER :
		{
			SetPCSlayerInfo( (PCSlayerInfo2*)pPacket->getPCInfo() );

			if (wasInterfaceRace != RACE_SLAYER)
			{
				UI_ChangeInterfaceRace(RACE_SLAYER);
			}
		}
		break;

		//--------------------------------------------------
		// Vampire�� ���
		//--------------------------------------------------
		case PC_VAMPIRE :
		{
			SetPCVampireInfo( (PCVampireInfo2*)pPacket->getPCInfo() );

			if (wasInterfaceRace != RACE_VAMPIRE)
			{
				UI_ChangeInterfaceRace(RACE_VAMPIRE);
			}
		}
		break;

		//--------------------------------------------------
		// Ousters�� ���
		//--------------------------------------------------
		case PC_OUSTERS :
		{
			SetPCOustersInfo( (PCOustersInfo2*)pPacket->getPCInfo() );
			
			if (wasInterfaceRace != RACE_OUSTERS)
			{
				UI_ChangeInterfaceRace(RACE_OUSTERS);
			}
		}
		break;
	}	
	// 2004, 6, 4 sobeit add start - sms charge
	g_char_slot_ingame.m_SMS_Charge		= pPacket->GetSMSCharge();
	// 2004, 6, 4 sobeit add end - sms charge
	
	// 2004, 6, 15 sobeit add end - nick name
	NicknameInfo* TempNick	= pPacket->getNicknameInfo();
	const char* szNickName;
	if(TempNick != NULL)
	{
		if(TempNick->getNicknameType() == NicknameInfo::NICK_NONE)
		{
			szNickName = "";
		}
		else 
		if(TempNick->getNicknameType() == NicknameInfo::NICK_CUSTOM_FORCED ||
			TempNick->getNicknameType() == NicknameInfo::NICK_CUSTOM)
		{
			szNickName = (TempNick->getNickname()).c_str();
		}
		else // �г��� �ε����� ���� ��
		{
			szNickName = (*g_pNickNameStringTable)[TempNick->getNicknameIndex()].GetString();
		}

		g_pPlayer->SetNickName(TempNick->getNicknameType(), (char*)szNickName);
//		g_char_slot_ingame.m_NickNameType = TempNick->getNicknameType();
//		g_char_slot_ingame.m_NickName = szNickName;
	}
	// 2004, 6, 15 sobeit add end - nick name

	// 2004, 9, 15, sobeit add start - non pk ����
	g_pUserInformation->IsNonPK = (pPacket->isNonPK())?true:false;
	// 2004, 9, 15, sobeit add end - non pk ����
	
	// 2004, 10, 18, sobeit add start - ���Ͽ� ��� ����
	g_pUserInformation->dwUnionID = pPacket->getGuildUnionID();
	g_pUserInformation->bUnionGrade = pPacket->getGuildUnionUserType();
	g_pPlayer->SetUnionGuildID(g_pUserInformation->dwUnionID);
	// 2004, 10, 18, sobeit add end
	
	// 2004, 12, 2, sobeit add start �Ŀ�¯ ����Ʈ 
	g_char_slot_ingame.m_Powerjjang_Point		= pPacket->GetPowerjjang_Point();
	// 2004, 12, 2, sobeit add end

	//--------------------------------------------------
	// ������ �����ص״� actionInfo�� ����
	//--------------------------------------------------
	g_pPlayer->SetSpecialActionInfo( spActionInfo );

	//--------------------------------------------------
	// ���� ��� ����
	//--------------------------------------------------
	g_pPlayer->SetAttackModeNormal();



	//-----------------------------------------------------------
	// ������ �����.
	//-----------------------------------------------------------
	if (g_pStorage!=NULL)
	{
		delete g_pStorage;
		g_pStorage = NULL;		
	}

	//-----------------------------------------------------------
	// ���� ����~~
	//-----------------------------------------------------------
	UI_UnlockGear();
	UI_UnlockItem();
	UI_UnlockItemTrade();
	UI_CloseShop();
	UI_CloseBookcase();
	UI_CloseExchange();
	UI_CloseExchangeAsk();
	UI_CloseExchangeCancel();
	UI_CloseRepair();
	UI_CloseSell();
	UI_CloseStorage();
	UI_ClosePartyCancel();
	UI_ClosePartyAsk();
	UI_ClosePartyRequest();
	UI_ClosePartyCancel();
	g_pUIMessageManager->Execute(UI_CLOSE_TRACE_WINDOW, 0, 0, NULL);

	if (g_pUIDialog!=NULL)
	{
		g_pUIDialog->ClosePCTalkDlg();
		g_pUIDialog->CloseMessageDlg();
//		g_pUIDialog->CloseHelpDlg();
	}

	if (g_pPCTalkBox!=NULL)
	{
		g_pPCTalkBox->Release();
	}
		

	//--------------------------------------------------
	// QuickSlot�� ���ش�. �ʿ��Ѱ�? - -;
	//--------------------------------------------------	
	g_pQuickSlot = NULL;
	g_pArmsBand1 = NULL;
	g_pArmsBand2 = NULL;

				
	UI_StartProgress( pPacket->getZoneID() );

	//UI_SetCurrentServerName(g_pServerInformation->GetServerGroupName(), 
	//						g_pServerInformation->GetServerName(), 
	//						g_pServerInformation->GetServerStatus());

	UI_DrawProgress(0);

	DEBUG_ADD("Init g_pTopView");
	
	//------------------------------
	// �ʱ�ȭ ���� �ʾ����� �ʱ�ȭ�Ѵ�.
	//------------------------------
	if (!g_pTopView->IsInit())
	{
		g_pTopView->Init();			
	}

	UI_DrawProgress( 15 );
	
	//--------------------------------------------------
	//
	// Zone�� ȯ�� ������ �����Ѵ�.
	//
	//--------------------------------------------------
	DEBUG_ADD("[UpdateInfo] Set Weather");
	
	//--------------------------------------------------
	// Zone�̵��Ҷ� �ٸ� ������ �ٲ�� �����..
	// ������ ������ �����Ѵ�.
	//--------------------------------------------------
	if (g_pWeather->GetWeatherType()==MWeather::WEATHER_SNOW
		&& pPacket->getWeather()!=WEATHER_SNOWY
		
		|| g_pWeather->GetWeatherType()==MWeather::WEATHER_RAIN
		&& pPacket->getWeather()!=WEATHER_RAINY	)
	{
		g_pWeather->Release();
	}
	
	SetWeather(pPacket->getWeather(), pPacket->getWeatherLevel());

	DEBUG_ADD_FORMAT( "[UpdateInfo] Set Dark/Light : %d / %d", pPacket->getDarkLevel(), pPacket->getLightLevel());
	
	int darkLevel = pPacket->getDarkLevel();
	int lightSight = pPacket->getLightLevel();

	if (g_pUserInformation->IsMaster)
	{
		darkLevel = 0;
		lightSight = 13;
	}

	// ȭ���� ��ӱ�
	g_pTopView->SetDarkBits( darkLevel );		
	
	// Player�þ� ���� ũ��
	g_pPlayer->SetTimeLightSight( lightSight );

	
	//--------------------------------------------------
	//
	//		Game �ð�
	//
	//--------------------------------------------------
	DEBUG_ADD( "Set Game Time" );
	
	GameTime gt = pPacket->getGameTime();
	
	// ���� �ð� ����
	g_pGameTime->SetStartTime(g_CurrentTime, 
							gt.getYear(),
							gt.getMonth(),
							gt.getDay(),
							gt.getHour(),
							gt.getMinute(),
							gt.getSecond()
							);
  
	// �ð� ����
	g_pGameTime->SetTimeRatio( 24 );


	//--------------------------------------------------
	//
	//	 Gear�� ������ �����Ѵ�.
	//
	//--------------------------------------------------
	// 2004, 10 ,21, sobeit Modify start - ������̺� ���������� ����
	SetGearInfo( pPacket->getGearInfo(), pPacket->getBloodBibleSignInfo() );
	// 2004, 10 ,21, sobeit Modify end - ������̺� ���������� ����
	//--------------------------------------------------
	// ���°� ���..
	//--------------------------------------------------
	g_pPlayer->CalculateStatus();


	//---------------------------------------------------------------
	// �ӽ� Belt
	//---------------------------------------------------------------
	/*
	MItem* pItem = MItem::NewItem( ITEM_CLASS_BELT );
			
	// object ID
	pItem->SetID( 9999 );

	// type / option
	pItem->SetItemType(	1 );
	pItem->SetItemOption( 0 );

	g_SlayerGear.AddItem( pItem, (MSlayerGear::GEAR_SLAYER)MSlayerGear::GEAR_SLAYER_BELT );			
	*/
	/*
	// �ӽ÷� Į �ֱ�
	static MBlade	blade;	blade.SetItemType( 0 ); blade.SetItemOption( 0 );
	//static MSword	sword;	sword.SetItemType( 0 );	sword.SetItemOption( 0 );
	g_pPlayer->SetAddonItem( &blade);
	*/

	/*
	// �ӽ÷� �� �ֱ�
	static MGunSMG	gun;	gun.SetItemType( 0 ); gun.SetItemOption( 0 );
	MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

	// �ǹ� ���� - -;
	pMagazine->SetID( 0 );

	// �̰Ŵ� �ѿ� ���缭 ����ߵȴ�.
	for (int j=0; j<g_ItemTable[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
	{
		pMagazine->SetItemType(	j );

		if (pMagazine->IsInsertToItem( (MItem*)&gun ))
		{
			break;
		}
	}

	// �ǹ� ����
	pMagazine->SetItemOption( 0 );

	// źâ ����
	pMagazine->SetNumber( 1000 );

	//------------------------------------
	// źâ ����
	//------------------------------------
	gun.SetMagazine( pMagazine );

	g_pPlayer->SetAddonItem( &gun );
	*/


	//--------------------------------------------------
	//
	//			Inventory �ʱ�ȭ
	//
	//--------------------------------------------------
	SetInventoryInfo( pPacket->getInventoryInfo() );


	//--------------------------------------------------
	//
	// ��Ÿ Item ���� == Mouse�� �پ� �ִ� item
	//
	//--------------------------------------------------
	SetExtraInfo( pPacket->getExtraInfo() );


	//--------------------------------------------------
	//
	// �������
	//
	//--------------------------------------------------
	if (pPacket->hasMotorcycle())
	{
		//--------------------------------------------
		// ������� ����
		//--------------------------------------------
		MItem*	pMotor = new MMotorcycle;	
		pMotor->SetID( 0 );
		pMotor->SetItemType( 0 ); 
//		pMotor->SetItemOption( 0 );
						
		//--------------------------------------------
		// ������� ���� �����۵�..
		//--------------------------------------------
		RideMotorcycleInfo* pMotorInfo = pPacket->getRideMotorcycleInfo();

		if (pMotorInfo!=NULL)
		{
			pMotor->SetID( pMotorInfo->getObjectID() );
			pMotor->SetItemType( pMotorInfo->getItemType() );
			pMotor->SetItemOptionList( pMotorInfo->getOptionType() );

			int num = pMotorInfo->getListNum();

			for (i=0; i<num; i++)
			{
				RideMotorcycleSlotInfo* pMotorSlotInfo = pMotorInfo->popFrontListElement();

				if (pMotorSlotInfo!=NULL)
				{
					// slot ������ �о pMotor�� �߰��ϸ� �ȴ�.
					// Motor�� MGridItemManager�̴�.

					delete pMotorSlotInfo;
				}
			}
		}

		//--------------------------------------------
		// ������̿� Ÿ�� �Ѵ�.
		//--------------------------------------------
		g_pPlayer->SetAddonItem( pMotor );	
		
		// -_-;;		
		// ���߿��� player�� ������̸� ���� �ؾ��Ѵ�. -_-;;
		// ����� ������ ������ ���� �� ����� T_T;
		delete pMotor;	
	}

	UI_DrawProgress(17);

	//--------------------------------------------------
	// ���� ��� ������ skill���� �ٽ� üũ�Ѵ�.
	//--------------------------------------------------
	g_pSkillAvailable->SetAvailableSkills();

	//--------------------------------------------------
	//
	// �ʿ��� ĳ���� Sprite Load
	//
	//--------------------------------------------------
	// [ TEST CODE ]
	// ĳ���� �� �ε�
//	DEBUG_ADD( "Load Creature Sprites" );
	
	//----------------------------------------
	// Player ĳ���� �׸� Load
	//----------------------------------------
	// Slayer����	- �׳�.. - -; �ǹ̰� ������..
//	LoadCreature( 0 );
//	UI_DrawProgress(25);
//
//	// Slayer����
//	LoadCreature( 1 );
//	UI_DrawProgress(35);
//
//	// Vampire����
//	LoadCreature( 2 );
//	UI_DrawProgress(38);
//
//	// Vampire����
//	LoadCreature( 3 );
//	UI_DrawProgress(39);
//
//	// ���� : spriteType�̴�.
//	LoadCreature( 58 );
//	// ���� : spriteType�̴�.
//	LoadCreature( 59 );
//	UI_DrawProgress(40);


	int numNPC = pPacket->getNPCCount();
	int numMonster = pPacket->getMonsterCount();
	
	int total = numNPC+numMonster;

	if (total==0) total = 1;

	int progress = 40;
	int progressInc = 30 / total;

	COrderedList<int>	listUse;

	//--------------------------------------------------
	// NPC Load
	//--------------------------------------------------
	for (i=0; i<numNPC; i++)
	{
		int spriteType = pPacket->getNPCType( i );

		// 2001.8.20 �ּ�ó��
		//LoadCreature( spriteType );

		// �� zone���� ����ϴ� creature�� SpriteType
		listUse.Add( spriteType );

		progress += progressInc;
		UI_DrawProgress(progress);		
	}

	//--------------------------------------------------
	// Monster Load
	//--------------------------------------------------
	for (i=0; i<numMonster; i++)
	{	
		int spriteType = pPacket->getMonsterType( i );

		// 2001.8.20 �ּ�ó��
		if(g_pUserOption->IsPreLoadMonster)
			LoadCreature( spriteType );

		// �� zone���� ����ϴ� creature�� SpriteType
		listUse.Add( spriteType );

		progress += progressInc;
		UI_DrawProgress(progress);		
	}

	//--------------------------------------------------
	// �� Zone���� ������� �ʴ� creature���� �����Ѵ�.
	//	
	// ��, LargeZone���� LargeZone���� �̵��ϴ� ��쿡��!
	//--------------------------------------------------
	if (g_bZonePlayerInLarge && g_nZoneLarge!=ZONEID_NULL && g_pZoneLarge!=NULL)
	{
		ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( pPacket->getZoneID() );

		if (pZoneInfo->Property & FLAG_ZONESIZE_SMALL)
		{
			g_pTopView->AddMonsterSpriteTypes( listUse );
		}
		//--------------------------------------------------
		// Large Zone���� �̵��ϴ� ���
		//--------------------------------------------------	
		else
		{
			ReleaseUselessCreatureSPKExcept( listUse ); 
		}
	}	
	else
	{
		g_pTopView->AddMonsterSpriteTypes( listUse );
	}
	//*/

	if (progress<70)
	{
		UI_DrawProgress( 70 );
	}

	//--------------------------------------------------
	// Zone Loading...			
	//--------------------------------------------------
	// ���� ���̾�����.. Zone �̵�.
	DEBUG_ADD( "Load Zone" );
	
	//--------------------------------------------------
	// �ӽ� Zone��ǥ
	//--------------------------------------------------
	g_pPlayer->SetX( pPacket->getZoneX() );
	g_pPlayer->SetY( pPacket->getZoneY() );

	if (previousMode == MODE_GAME)
	{
		MoveZone( pPacket->getZoneID() );
	}
	// �ƴϸ�.. �׳� Zone Loading
	else
	{
		LoadZone( pPacket->getZoneID() );
	}
	
	// Packet Encrypt
	g_pSocket->setEncryptCode();

	//-----------------------------------------------------------
	//
	// Zone������ NPC ��ǥ ����
	//
	//-----------------------------------------------------------
	while (1)
	{
		NPCInfo* pInfo = pPacket->popNPCInfo();

		if (pInfo==NULL)
		{
			break;
		}

		const char* pName = pInfo->getName().c_str();
		int npcID = pInfo->getNPCID();
		int x = pInfo->getX();
		int y = pInfo->getY();

		if(g_pZone)
		{
			int zoneID	= g_pZone->GetID();
			if(zoneID< 1500 || zoneID >1506)
				pName = (*g_pCreatureTable)[npcID].Name;
		}else
			pName = (*g_pCreatureTable)[npcID].Name;

		UI_SetNPCInfo( pName, npcID, x, y );

		delete pInfo;		
	}

	//--------------------------------------------------
	//
	//	���� Player�� �ɷ��ִ� Effect Status�� �����Ѵ�.
	//
	//--------------------------------------------------
	// [!] Zone Loading ���Ŀ� �ؾ��Ѵ�.
	//--------------------------------------------------
	
	//--------------------------------------------------
	// ���� �ð��� Frame ����� �ٽ� ���ش�.
	//--------------------------------------------------
	g_CurrentTime = timeGetTime();	// �ʿ��ұ�? -_-;
	
	// updateInfo�� ó���Ѵٰ� �ҿ�� �ð�
	// ...�� frame���� ȯ���� ��
	int delayedFrame = ConvertMillisecondToFrame( g_CurrentTime - startTime );		

	// ���� frame ����
	g_CurrentFrame += delayedFrame;
	g_MyBatColorSet = 0xFFFF;

	switch (pPacket->getPCInfo()->getPCType())
	{
		//--------------------------------------------------
		// Vampire�� ���
		//--------------------------------------------------
		case PC_VAMPIRE :
		{
			PCVampireInfo2 *pInfo = dynamic_cast< PCVampireInfo2* >( pPacket->getPCInfo() );
			if( pInfo->getBatColor() != 0 )
				g_MyBatColorSet = pInfo->getBatColor();

//			g_pPlayer->SetChangeColorSet( pInfo->getBatColor() );
		}
		break;
	}



	SetEffectInfo( g_pPlayer, pPacket->getEffectInfo(), delayedFrame );

	
	UI_DrawProgress(100);
	UI_DrawProgress(100);

	//--------------------------------------------------
	//
	// Skill Tree
	//
	//--------------------------------------------------
	/*
	g_SkillManager.Init( MAX_SKILLDOMAIN );

	//---------------------------------------------------------------------
	// �⺻ ����κ��� skill tree�� �ʱ�ȭ�Ѵ�.
	//---------------------------------------------------------------------
	g_SkillManager[SKILLDOMAIN_BLADE].SetRootSkill( SKILL_SINGLE_BLOW );
	g_SkillManager[SKILLDOMAIN_SWORD].SetRootSkill( SKILL_DOUBLE_IMPACT );
	g_SkillManager[SKILLDOMAIN_GUN].SetRootSkill( SKILL_SNIPPING );
	g_SkillManager[SKILLDOMAIN_GUN].SetRootSkill( SKILL_SHARP_SHOOTING );
	g_SkillManager[SKILLDOMAIN_ENCHANT].SetRootSkill( MAGIC_CREATE_HOLY_WATER );
	g_SkillManager[SKILLDOMAIN_HEAL].SetRootSkill( MAGIC_CURE_POISON );
	g_SkillManager[SKILLDOMAIN_VAMPIRE].SetRootSkill( MAGIC_HIDE );	
	*/

	/*
	g_SkillAvailable.AddSkill( SKILL_DOUBLE_IMPACT );
	g_SkillAvailable.AddSkill( SKILL_TRIPLE_SLASHER );
	g_SkillAvailable.DisableSkill( SKILL_TRIPLE_SLASHER );
	g_SkillAvailable.AddSkill( SKILL_SCREW_SLASHER );
	
	g_SkillAvailable.AddSkill( SKILL_SINGLE_BLOW );
	g_SkillAvailable.DisableSkill( SKILL_SINGLE_BLOW );
	
	g_SkillAvailable.AddSkill( SKILL_ARMOR_CRASH );

	g_SkillAvailable.AddSkill( SKILL_SNIPPING );
	g_SkillAvailable.AddSkill( SKILL_KNOCKS_TARGET_BACK );

	g_SkillAvailable.AddSkill( MAGIC_LIGHT );
	g_SkillAvailable.AddSkill( MAGIC_DETECT_HIDDEN );
	g_SkillAvailable.DisableSkill( MAGIC_DETECT_HIDDEN );
	
	g_SkillAvailable.AddSkill( MAGIC_CURE_POISON );
	g_SkillAvailable.AddSkill( MAGIC_PROTECTION_FROM_CURSE );
	
	g_SkillAvailable.AddSkill( MAGIC_CURE_POISON );
	g_SkillAvailable.DisableSkill( MAGIC_CURE_POISON );
	g_SkillAvailable.AddSkill( MAGIC_PROTECTION_FROM_POISON );
	g_SkillAvailable.AddSkill( MAGIC_CURE_PARALYSIS );
	*/

	UI_EndProgress();	


	//-----------------------------------------------------------
	// Sound�� ����� �� �ְ� �Ѵ�.
	//-----------------------------------------------------------
	g_DXSound.UnSetMute();

	// blood drainüũ���� �� �� ��� ��.��
	UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	
	//UI_AffectUserOption();

	//--------------------------------------------------
	// message �ʱ�ȭ
	//--------------------------------------------------
	g_pSystemMessage->Clear();
	g_pGameMessage->Clear();
	g_pNoticeMessage->Clear();

	//--------------------------------------------------
	// player�� ��ġ�� ��ٸ���.
	//--------------------------------------------------
	SetMode( MODE_WAIT_SETPOSITION );	


//	FILE *fp=fopen("InventoryLog.txt","rt");
//	if( fp != NULL )
//	{
//		char szBuffer[128] = "InventoryLogStart012301230123012301230123012301230123InventoryLogEnd";		                                       
//		fseek(fp,0,SEEK_END);
//		int filesize = ftell(fp);
//
//		if( filesize < strlen(szBuffer) )
//		{
//			char filebuffer[128];
//			sprintf(filebuffer,"InventoryLog:%d", filesize);
//			
//			SendBugReport( filebuffer );
//		}
//		fclose(fp);
//	}

/*
	FILE *fp=fopen("fs.his","rt");
	if( fp == NULL )
	{
		fp = fopen("fs.his","wt");
		fprintf(fp,"!@#");
		CSystemInfo SystemInfo;
		SendBugReport("SystemInfo : %d %d CPU : %dMhz",g_bEnable3DHAL, g_dwVideoMemory,SystemInfo.GetCpuClock());
	}
	fclose(fp);
*/
//	ExecuteHelpEvent( HELP_EVENT_INTERFACE );

#endif

	__END_CATCH
}
