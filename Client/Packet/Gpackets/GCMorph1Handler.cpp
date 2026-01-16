//--------------------------------------------------------------------------------
//
// Filename    : GCMorph1Handler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCMorph1.h"
#include "ClientDef.h"
#include "CDirectSound.h"
#include "MQuickSlot.h"
#include "SkillDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCMorph1Handler::execute ( GCMorph1 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	if (g_pZone==NULL
		|| g_pPlayer==NULL)
	{
		DEBUG_ADD("[Error] g_pZone or g_pPlayer NULL");

		return;
	}

	//-----------------------------------------------------------
	// Sound를 출력하지 않는다.
	//-----------------------------------------------------------
	g_DXSound.SetMute();

	//-----------------------------------------------------------
	// ID가 다르면.. ID를 바꿔야 한다.
	//-----------------------------------------------------------
	int objectID;
	
	switch (pPacket->getPCInfo2()->getPCType())
	{
		//--------------------------------------------------
		// Slayer인 경우
		//--------------------------------------------------
		case PC_SLAYER :		
			objectID = ((PCSlayerInfo2*)pPacket->getPCInfo2())->getObjectID();
		break;

		//--------------------------------------------------
		// Vampire인 경우
		//--------------------------------------------------
		case PC_VAMPIRE :
			objectID = ((PCVampireInfo2*)pPacket->getPCInfo2())->getObjectID();			
		break;

		default :
			DEBUG_ADD("[Error] wrong PC Type");
		return;
	}

	if (g_pPlayer->GetID()!=objectID)
	{
		// Zone에 들어있는거도 바꾼다.
		g_pZone->RemoveCreature( g_pPlayer->GetID() );

		// 새로운 ID
		g_pPlayer->SetID( objectID );

		g_pZone->SetPlayer();
		g_pPlayer->SetZone(g_pZone);
	}

	//--------------------------------------------------
	//
	//			 Player 변신
	//
	//--------------------------------------------------
	if (g_pPlayer->IsSlayer())
	{
		g_pPlayer->ChangeToVampire();

		// 임시로
		g_pPlayer->SetGuildNumber( 2 );
	}
	else
	{
		g_pPlayer->ChangeToSlayer();

		// 임시로
		g_pPlayer->SetGuildNumber( -1 );
	}


	//--------------------------------------------------
	// 특수 기술 설정
	//--------------------------------------------------
	//g_pPlayer->SetSpecialActionInfo( MAGIC_LIGHT );

	//--------------------------------------------------
	// 공격 모드 설정
	//--------------------------------------------------
//	g_pPlayer->SetAttackModeNormal();


	//--------------------------------------------------
	// QuickSlot을 없앤다. 필요한가? - -;
	//--------------------------------------------------	
	g_pQuickSlot = NULL;
	g_pArmsBand1 = NULL;
	g_pArmsBand2 = NULL;

			
	//--------------------------------------------------
	//		복장 초기화
	//--------------------------------------------------
	// 필요 없을 듯..
	//--------------------------------------------------
	DEBUG_ADD( "Remove All Addon" );
	
	//for (int i=0; i<ADDON_MAX; i++)
	//{
	//	g_pPlayer->RemoveAddon((enum ADDON)i);
	//}			

	g_pPlayer->SetBasicActionInfo( SKILL_ATTACK_MELEE );
	g_pPlayer->SetBasicAttackDistance( 1 );
		

	//--------------------------------------------------
	//
	//			 player의 정보를 설정한다.
	//
	//--------------------------------------------------		
	switch (pPacket->getPCInfo2()->getPCType())
	{
		//--------------------------------------------------
		// Slayer인 경우
		//--------------------------------------------------
		case PC_SLAYER :
		{
			SetPCSlayerInfo( (PCSlayerInfo2*)pPacket->getPCInfo2() );
		}
		break;

		//--------------------------------------------------
		// Vampire인 경우
		//--------------------------------------------------
		case PC_VAMPIRE :
		{
			SetPCVampireInfo( (PCVampireInfo2*)pPacket->getPCInfo2() );
		}
		break;

		//--------------------------------------------------
		// Ousters인 경우
		//--------------------------------------------------
		case PC_OUSTERS :
		{
			SetPCOustersInfo( (PCOustersInfo2*)pPacket->getPCInfo2() );
		}
		break;
	}

	//--------------------------------------------------
	//	 Gear의 정보를 저장한다.
	//--------------------------------------------------
	SetGearInfo( pPacket->getGearInfo() );

	// 계산을 다시해준다.
	g_pPlayer->CalculateStatus();

	//--------------------------------------------------
	//	Inventory 초기화
	//--------------------------------------------------
	SetInventoryInfo( pPacket->getInventoryInfo() );

	//--------------------------------------------------
	// 기타 Item 정보 == Mouse에 붙어 있는 item
	//--------------------------------------------------
	SetExtraInfo( pPacket->getExtraInfo() );
	

	//--------------------------------------------------
	// Skill 
	//--------------------------------------------------


	//-----------------------------------------------------------
	// Sound를 출력할 수 있게 한다.
	//-----------------------------------------------------------
	g_DXSound.UnSetMute();


#endif

	__END_CATCH
}
