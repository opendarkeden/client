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
// 클라이언트가 게임 서버로부터 GCUpdateInfo 패킷을 받게 되면,
// 패킷 안의 데이터들을 클라이언트에 저장한 후, 데이터 로딩이
// 끝이 나면 게임 서버로 CGReady 패킷을 보내면 된다.
//----------------------------------------------------------------------
void GCUpdateInfoHandler::execute ( GCUpdateInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	#ifndef __DEBUG_OUTPUT__ 
		DEBUG_ADD("Execute UpdateInfo");
	#endif

	// EventManager의 Zone종속 이벤트를 삭제한다
	g_pEventManager->RemoveAllEventByType(EVENTTYPE_ZONE);

	//-----------------------------------------------------------
	// 다른 부분에서 mode를 참조하는 경우가 있어서
	// mode를 설정해준다.
	//-----------------------------------------------------------
	CLIENT_MODE	previousMode = g_Mode;
	g_Mode = MODE_WAIT_UPDATEINFO;

	//UI_SaveUserOption();

	//-----------------------------------------------------------
	// 현재 server의 상태 표시
	//-----------------------------------------------------------
	if (g_pServerInformation!=NULL)
	{
		g_pServerInformation->SetServerGroupStatus( pPacket->getServerStat() );
	}

	//-----------------------------------------------------------
	// AddonSPK를 분할 로딩하는데..
	// 여기서는 loading 안된 부분을 다 loading해야 한다. 2001.8.20
	//-----------------------------------------------------------
	LoadingAddonSPK( true );

//	g_pUserOption->LoadFromFile( FILE_INFO_USEROPTION );
//	UI_AffectUserOption();
	

	int i;

	//-----------------------------------------------------------
	// Sound를 출력하지 않는다.
	//-----------------------------------------------------------
	g_DXSound.SetMute();

	//-----------------------------------------------------------
	// updateInfo를 처리하기 시작할때의 시간을 저장한다.
	//-----------------------------------------------------------
	DWORD	startTime	= timeGetTime();
	
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	//--------------------------------------------------
	// 데이터 로딩을 시작한다.
	//--------------------------------------------------
	pClientPlayer->setPlayerStatus( CPS_WAITING_FOR_LOADING );

	
	//--------------------------------------------------
	//
	//			 Player 정보 설정
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
		// interface 변경에 관하여..
		//--------------------------------------------------
		/*
		if (g_pPlayer->IsVampire())
		{
			wasSlayerInterface = FALSE;
		}
		*/
		// 죽었을때, dialog 다 닫기
		if (g_pPlayer->IsDead())
		{
			//UI_CloseAllDialog();
		}
		

		//--------------------------------------------------
		// 이전에 사용중이던 기술 유지..
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
	//		복장 초기화
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
	//	Skill Info Table 초기화
	//
	//--------------------------------------------------
	DEBUG_ADD( "SkillInfoTable Init" );
	g_pSkillInfoTable->Init();

	//--------------------------------------------------
	//
	// Skill Tree 초기화
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
	//			 player의 정보를 저장한다.
	//
	//--------------------------------------------------		
	switch (pPacket->getPCInfo()->getPCType())
	{
		//--------------------------------------------------
		// Slayer인 경우
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
		// Vampire인 경우
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
		// Ousters인 경우
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
		else // 닉네임 인덱스가 있을 때
		{
			szNickName = (*g_pNickNameStringTable)[TempNick->getNicknameIndex()].GetString();
		}

		g_pPlayer->SetNickName(TempNick->getNicknameType(), (char*)szNickName);
//		g_char_slot_ingame.m_NickNameType = TempNick->getNicknameType();
//		g_char_slot_ingame.m_NickName = szNickName;
	}
	// 2004, 6, 15 sobeit add end - nick name

	// 2004, 9, 15, sobeit add start - non pk 관련
	g_pUserInformation->IsNonPK = (pPacket->isNonPK())?true:false;
	// 2004, 9, 15, sobeit add end - non pk 관련
	
	// 2004, 10, 18, sobeit add start - 유니온 길드 관련
	g_pUserInformation->dwUnionID = pPacket->getGuildUnionID();
	g_pUserInformation->bUnionGrade = pPacket->getGuildUnionUserType();
	g_pPlayer->SetUnionGuildID(g_pUserInformation->dwUnionID);
	// 2004, 10, 18, sobeit add end
	
	// 2004, 12, 2, sobeit add start 파워짱 포인트 
	g_char_slot_ingame.m_Powerjjang_Point		= pPacket->GetPowerjjang_Point();
	// 2004, 12, 2, sobeit add end

	//--------------------------------------------------
	// 이전에 지정해뒀던 actionInfo로 설정
	//--------------------------------------------------
	g_pPlayer->SetSpecialActionInfo( spActionInfo );

	//--------------------------------------------------
	// 공격 모드 설정
	//--------------------------------------------------
	g_pPlayer->SetAttackModeNormal();



	//-----------------------------------------------------------
	// 보관함 지운다.
	//-----------------------------------------------------------
	if (g_pStorage!=NULL)
	{
		delete g_pStorage;
		g_pStorage = NULL;		
	}

	//-----------------------------------------------------------
	// 정리 정리~~
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
	// QuickSlot을 없앤다. 필요한가? - -;
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
	// 초기화 되지 않았으면 초기화한다.
	//------------------------------
	if (!g_pTopView->IsInit())
	{
		g_pTopView->Init();			
	}

	UI_DrawProgress( 15 );
	
	//--------------------------------------------------
	//
	// Zone의 환경 정보를 설정한다.
	//
	//--------------------------------------------------
	DEBUG_ADD("[UpdateInfo] Set Weather");
	
	//--------------------------------------------------
	// Zone이동할때 다른 날씨로 바뀌는 경우라면..
	// 날씨를 완전히 제거한다.
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

	// 화면의 어둡기
	g_pTopView->SetDarkBits( darkLevel );		
	
	// Player시야 빛의 크기
	g_pPlayer->SetTimeLightSight( lightSight );

	
	//--------------------------------------------------
	//
	//		Game 시간
	//
	//--------------------------------------------------
	DEBUG_ADD( "Set Game Time" );
	
	GameTime gt = pPacket->getGameTime();
	
	// 기준 시간 설정
	g_pGameTime->SetStartTime(g_CurrentTime, 
							gt.getYear(),
							gt.getMonth(),
							gt.getDay(),
							gt.getHour(),
							gt.getMinute(),
							gt.getSecond()
							);
  
	// 시간 비율
	g_pGameTime->SetTimeRatio( 24 );


	//--------------------------------------------------
	//
	//	 Gear의 정보를 저장한다.
	//
	//--------------------------------------------------
	// 2004, 10 ,21, sobeit Modify start - 블러드바이블 장착아이템 관련
	SetGearInfo( pPacket->getGearInfo(), pPacket->getBloodBibleSignInfo() );
	// 2004, 10 ,21, sobeit Modify end - 블러드바이블 장착아이템 관련
	//--------------------------------------------------
	// 상태값 계산..
	//--------------------------------------------------
	g_pPlayer->CalculateStatus();


	//---------------------------------------------------------------
	// 임시 Belt
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
	// 임시로 칼 넣기
	static MBlade	blade;	blade.SetItemType( 0 ); blade.SetItemOption( 0 );
	//static MSword	sword;	sword.SetItemType( 0 );	sword.SetItemOption( 0 );
	g_pPlayer->SetAddonItem( &blade);
	*/

	/*
	// 임시로 총 넣기
	static MGunSMG	gun;	gun.SetItemType( 0 ); gun.SetItemOption( 0 );
	MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

	// 의미 없음 - -;
	pMagazine->SetID( 0 );

	// 이거는 총에 맞춰서 해줘야된다.
	for (int j=0; j<g_ItemTable[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
	{
		pMagazine->SetItemType(	j );

		if (pMagazine->IsInsertToItem( (MItem*)&gun ))
		{
			break;
		}
	}

	// 의미 없음
	pMagazine->SetItemOption( 0 );

	// 탄창 개수
	pMagazine->SetNumber( 1000 );

	//------------------------------------
	// 탄창 설정
	//------------------------------------
	gun.SetMagazine( pMagazine );

	g_pPlayer->SetAddonItem( &gun );
	*/


	//--------------------------------------------------
	//
	//			Inventory 초기화
	//
	//--------------------------------------------------
	SetInventoryInfo( pPacket->getInventoryInfo() );


	//--------------------------------------------------
	//
	// 기타 Item 정보 == Mouse에 붙어 있는 item
	//
	//--------------------------------------------------
	SetExtraInfo( pPacket->getExtraInfo() );


	//--------------------------------------------------
	//
	// 오토바이
	//
	//--------------------------------------------------
	if (pPacket->hasMotorcycle())
	{
		//--------------------------------------------
		// 오토바이 생성
		//--------------------------------------------
		MItem*	pMotor = new MMotorcycle;	
		pMotor->SetID( 0 );
		pMotor->SetItemType( 0 ); 
//		pMotor->SetItemOption( 0 );
						
		//--------------------------------------------
		// 오토바이 안의 아이템들..
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
					// slot 정보를 읽어서 pMotor에 추가하면 된다.
					// Motor는 MGridItemManager이다.

					delete pMotorSlotInfo;
				}
			}
		}

		//--------------------------------------------
		// 오토바이에 타게 한다.
		//--------------------------------------------
		g_pPlayer->SetAddonItem( pMotor );	
		
		// -_-;;		
		// 나중에는 player가 오토바이를 갖게 해야한다. -_-;;
		// 지우면 아이템 정보고 뭐고 다 사라짐 T_T;
		delete pMotor;	
	}

	UI_DrawProgress(17);

	//--------------------------------------------------
	// 현재 사용 가능한 skill들을 다시 체크한다.
	//--------------------------------------------------
	g_pSkillAvailable->SetAvailableSkills();

	//--------------------------------------------------
	//
	// 필요한 캐릭터 Sprite Load
	//
	//--------------------------------------------------
	// [ TEST CODE ]
	// 캐릭터 몸 로드
//	DEBUG_ADD( "Load Creature Sprites" );
	
	//----------------------------------------
	// Player 캐릭터 그림 Load
	//----------------------------------------
	// Slayer남자	- 그냥.. - -; 의미가 있을까..
//	LoadCreature( 0 );
//	UI_DrawProgress(25);
//
//	// Slayer여자
//	LoadCreature( 1 );
//	UI_DrawProgress(35);
//
//	// Vampire남자
//	LoadCreature( 2 );
//	UI_DrawProgress(38);
//
//	// Vampire여자
//	LoadCreature( 3 );
//	UI_DrawProgress(39);
//
//	// 늑대 : spriteType이다.
//	LoadCreature( 58 );
//	// 박쥐 : spriteType이다.
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

		// 2001.8.20 주석처리
		//LoadCreature( spriteType );

		// 이 zone에서 사용하는 creature의 SpriteType
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

		// 2001.8.20 주석처리
		if(g_pUserOption->IsPreLoadMonster)
			LoadCreature( spriteType );

		// 이 zone에서 사용하는 creature의 SpriteType
		listUse.Add( spriteType );

		progress += progressInc;
		UI_DrawProgress(progress);		
	}

	//--------------------------------------------------
	// 이 Zone에서 사용하지 않는 creature들은 제거한다.
	//	
	// 단, LargeZone에서 LargeZone으로 이동하는 경우에만!
	//--------------------------------------------------
	if (g_bZonePlayerInLarge && g_nZoneLarge!=ZONEID_NULL && g_pZoneLarge!=NULL)
	{
		ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( pPacket->getZoneID() );

		if (pZoneInfo->Property & FLAG_ZONESIZE_SMALL)
		{
			g_pTopView->AddMonsterSpriteTypes( listUse );
		}
		//--------------------------------------------------
		// Large Zone으로 이동하는 경우
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
	// 게임 중이었으면.. Zone 이동.
	DEBUG_ADD( "Load Zone" );
	
	//--------------------------------------------------
	// 임시 Zone좌표
	//--------------------------------------------------
	g_pPlayer->SetX( pPacket->getZoneX() );
	g_pPlayer->SetY( pPacket->getZoneY() );

	if (previousMode == MODE_GAME)
	{
		MoveZone( pPacket->getZoneID() );
	}
	// 아니면.. 그냥 Zone Loading
	else
	{
		LoadZone( pPacket->getZoneID() );
	}
	
	// Packet Encrypt
	g_pSocket->setEncryptCode();

	//-----------------------------------------------------------
	//
	// Zone에서의 NPC 좌표 설정
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
	//	현재 Player에 걸려있는 Effect Status를 설정한다.
	//
	//--------------------------------------------------
	// [!] Zone Loading 이후에 해야한다.
	//--------------------------------------------------
	
	//--------------------------------------------------
	// 현재 시간과 Frame 계산을 다시 해준다.
	//--------------------------------------------------
	g_CurrentTime = timeGetTime();	// 필요할까? -_-;
	
	// updateInfo를 처리한다고 소요된 시간
	// ...을 frame으로 환산한 값
	int delayedFrame = ConvertMillisecondToFrame( g_CurrentTime - startTime );		

	// 게임 frame 증가
	g_CurrentFrame += delayedFrame;
	g_MyBatColorSet = 0xFFFF;

	switch (pPacket->getPCInfo()->getPCType())
	{
		//--------------------------------------------------
		// Vampire인 경우
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
	// 기본 기술로부터 skill tree를 초기화한다.
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
	// Sound를 출력할 수 있게 한다.
	//-----------------------------------------------------------
	g_DXSound.UnSetMute();

	// blood drain체크때메 함 더 출력 ㅡ.ㅡ
	UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	
	//UI_AffectUserOption();

	//--------------------------------------------------
	// message 초기화
	//--------------------------------------------------
	g_pSystemMessage->Clear();
	g_pGameMessage->Clear();
	g_pNoticeMessage->Clear();

	//--------------------------------------------------
	// player의 위치를 기다린다.
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
