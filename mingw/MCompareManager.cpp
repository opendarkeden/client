//-----------------------------------------------------------------------------
// MCompareManager.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include "MCompareManager.h"
#include "MPlayer.h"
#include "MQuickSlot.h"
#include "MSlayerGear.h"
#include "MInventory.h"
#include "MHelpManager.h"
#include "UIFunction.h"
#include "DebugInfo.h"

//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
MCompareManager*		g_pCompareManager = NULL;

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
MCompareManager::MCompareManager()
{
	for (int i=0; i<MAX_HELP_COMPARE; i++)
	{
		m_CompareFunction[i] = NULL;
	}

	Init();
}

MCompareManager::~MCompareManager()
{
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
MCompareManager::Init()
{	
	m_CompareFunction[HC_RANDOM]						= Execute_HC_RANDOM;

	m_CompareFunction[HC_PLAYER_IN_SHOP]				= Execute_HC_PLAYER_IN_SHOP;
	m_CompareFunction[HC_PLAYER_IN_REPAIR]				= Execute_HC_PLAYER_IN_REPAIR;
	m_CompareFunction[HC_PLAYER_IN_STORAGE]				= Execute_HC_PLAYER_IN_STORAGE;
	m_CompareFunction[HC_PLAYER_IN_TRADE]				= Execute_HC_PLAYER_IN_TRADE;
	m_CompareFunction[HC_PLAYER_IN_NORMAL_GAME_MODE]	= Execute_HC_PLAYER_IN_NORMAL_GAME_MODE;
	m_CompareFunction[HC_PLAYER_IS_SLAYER]				= Execute_HC_PLAYER_IS_SLAYER;
	m_CompareFunction[HC_PLAYER_IS_VAMPIRE]				= Execute_HC_PLAYER_IS_VAMPIRE;

	m_CompareFunction[HC_NO_SKILL_SELECTED]				= Execute_HC_NO_SKILL_SELECTED;
	m_CompareFunction[HC_NO_SKILL_FUNCTION_KEY_DEFINED] = Execute_HC_NO_SKILL_FUNCTION_KEY_DEFINED;

	m_CompareFunction[HC_HAS_HEAL_POTION_IN_BELT]		= Execute_HC_HAS_HEAL_POTION_IN_BELT;
	m_CompareFunction[HC_HAS_HEAL_POTION_IN_INVENTORY]	= Execute_HC_HAS_HEAL_POTION_IN_INVENTORY;
	m_CompareFunction[HC_HAS_MANA_POTION_IN_BELT]		= Execute_HC_HAS_MANA_POTION_IN_BELT;
	m_CompareFunction[HC_HAS_MANA_POTION_IN_INVENTORY]	= Execute_HC_HAS_MANA_POTION_IN_INVENTORY;
	m_CompareFunction[HC_RECEIVE_FIRST_BONUS_POINT]		= Execute_HC_RECEIVE_FIRST_BONUS_POINT;
	m_CompareFunction[HC_DIDNOT_WHISPER_YET]			= Execute_HC_DIDNOT_WHISPER_YET;
	m_CompareFunction[HC_ITEM_PICKED_UP]				= Execute_HC_ITEM_PICKED_UP;
	m_CompareFunction[HC_USED_ALT_FOR_ITEM]				= Execute_HC_USED_ALT_FOR_ITEM;
	m_CompareFunction[HC_USED_FUNCTION_KEY_FOR_QUICKITEM] = Execute_HC_USED_FUNCTION_KEY_FOR_QUICKITEM;
	m_CompareFunction[HC_EMPTY_BULLET]					= Execute_HC_EMPTY_BULLET;
}

//-----------------------------------------------------------------------------
// HC_RANDOM
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_RANDOM()
{
	return rand()%2;
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IN_SHOP
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IN_SHOP()
{	
	return UI_IsRunningShop();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IN_REPAIR
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IN_REPAIR()
{	
	return UI_IsRunningRepair();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IN_STORAGE
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IN_STORAGE()
{	
	return UI_IsRunningStorage();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IN_TRADE
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IN_TRADE()
{	
	return UI_IsRunningExchange();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IN_NORMAL_GAME_MODE
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IN_NORMAL_GAME_MODE()
{	
	// -_-; 별 의미 없나.. 으흠..
	return g_pPlayer!=NULL && g_pPlayer->IsWaitVerifyNULL() && g_pPlayer->IsItemCheckBufferNULL();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IS_SLAYER
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IS_SLAYER()
{	
	return g_pPlayer!=NULL && g_pPlayer->IsSlayer();
}

//-----------------------------------------------------------------------------
// HC_PLAYER_IS_VAMPIRE
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_PLAYER_IS_VAMPIRE()
{	
	return g_pPlayer!=NULL && g_pPlayer->IsVampire();
}

//-----------------------------------------------------------------------------
// HC_NO_SKILL_SELECTED
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_NO_SKILL_SELECTED()
{	
	return g_pPlayer!=NULL && g_pPlayer->GetSpecialActionInfo()==ACTIONINFO_NULL;
}

//-----------------------------------------------------------------------------
// HC_NO_SKILL_FUNCTION_KEY_DEFINED
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_NO_SKILL_FUNCTION_KEY_DEFINED()
{	
	BOOL bNotDefined = TRUE;
	int maxHotKey = (g_pPlayer->IsSlayer()? 4 : 8);//C_VS_UI_SKILL_BASE::HOTKEY_MAX;

	for (int i=0; i < maxHotKey; i++)
	{
		int id = UI_GetHotKey( i );

		if (id < maxHotKey)	//!= NOT_SELECTED)
		{
			bNotDefined = FALSE;
			break;
		}
	}

	return bNotDefined;	
}

//-----------------------------------------------------------------------------
// HC_HAS_HEAL_POTION_IN_BELT
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_HAS_HEAL_POTION_IN_BELT()
{	
	if (g_pQuickSlot==NULL)
	{
		return FALSE;
	}

	g_pQuickSlot->SetBegin();

	while (g_pQuickSlot->IsNotEnd())
	{
		MItem* pItem = g_pQuickSlot->Get();

		// Heal포션인 경우..
		if (pItem!=NULL 
			&& pItem->GetItemClass()==ITEM_CLASS_POTION
			&& pItem->GetItemType() < 5)
		{
			return TRUE;
		}

		g_pQuickSlot->Next();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// HC_HAS_HEAL_POTION_IN_INVENTORY
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_HAS_HEAL_POTION_IN_INVENTORY()
{	
	if (g_pInventory==NULL)
	{
		return FALSE;
	}

	g_pInventory->SetBegin();

	while (g_pInventory->IsNotEnd())
	{
		MItem* pItem = g_pInventory->Get();

		// Heal포션인 경우..
		if (pItem!=NULL 
			&& pItem->GetItemClass()==ITEM_CLASS_POTION
			&& pItem->GetItemType() < 5)
		{
			return TRUE;
		}

		g_pInventory->Next();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// HC_HAS_MANA_POTION_IN_BELT
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_HAS_MANA_POTION_IN_BELT()
{	
	if (g_pQuickSlot==NULL)
	{
		return FALSE;
	}

	g_pQuickSlot->SetBegin();

	while (g_pQuickSlot->IsNotEnd())
	{
		MItem* pItem = g_pQuickSlot->Get();

		// Mana포션인 경우..
		if (pItem!=NULL 
			&& pItem->GetItemClass()==ITEM_CLASS_POTION
			&& pItem->GetItemType() >= 5)
		{
			return TRUE;
		}

		g_pQuickSlot->Next();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// HC_HAS_MANA_POTION_IN_INVENTORY
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_HAS_MANA_POTION_IN_INVENTORY()
{	
	if (g_pInventory==NULL)
	{
		return FALSE;
	}

	g_pInventory->SetBegin();

	while (g_pInventory->IsNotEnd())
	{
		MItem* pItem = g_pInventory->Get();

		// Heal포션인 경우..
		if (pItem!=NULL 
			&& pItem->GetItemClass()==ITEM_CLASS_POTION
			&& pItem->GetItemType() >= 5)
		{
			return TRUE;
		}

		g_pInventory->Next();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// HC_RECEIVE_FIRST_BONUS_POINT
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_RECEIVE_FIRST_BONUS_POINT()
{	
	// 2 Level이고 Bonus Point가 있으면 처음 받은거다.
	return g_pPlayer!=NULL 
			&& g_pPlayer->IsVampire()
			&& g_pPlayer->GetLEVEL()==2
			&& g_pPlayer->GetBonusPoint()!=0;
}

//-----------------------------------------------------------------------------
// HC_DIDNOT_WHISPER_YET
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_DIDNOT_WHISPER_YET()
{	
	// 귓속말 해본적이 있다면..
	return false;
	//return !g_pHelpManager->IsEventOccured( HE_CHAT_WHISPER );
}

//-----------------------------------------------------------------------------
// HC_ITEM_PICKED_UP
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_ITEM_PICKED_UP()
{		
	// item주운적이 있다면..
	return false;
	//return g_pHelpManager->IsEventOccured( HE_ITEM_PICKUP );
}

//-----------------------------------------------------------------------------
// HC_USED_ALT_FOR_ITEM
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_USED_ALT_FOR_ITEM()
{	
	// alt 누른적이 있다면..
	return false;
//	return g_pHelpManager->IsEventOccured( HE_PRESSED_ALT );
}

//-----------------------------------------------------------------------------
// HC_USED_FUNCTION_KEY_FOR_QUICKITEM
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_USED_FUNCTION_KEY_FOR_QUICKITEM()
{	
	// 단축키로 quickitem쓴적이 잇다면..
	return false;
	//return g_pHelpManager->IsEventOccured( HE_PRESSED_FUNCTION_KEY_FOR_QUICKITEM );
}

//-----------------------------------------------------------------------------
// HC_EMPTY_BULLET
//-----------------------------------------------------------------------------
BOOL	
MCompareManager::Execute_HC_EMPTY_BULLET()
{	
	return g_pCurrentMagazine==NULL || g_pCurrentMagazine->GetNumber()==0;
}

//-----------------------------------------------------------------------------
// Compare
//-----------------------------------------------------------------------------
// message와 연결되어 있는 적절한 처리 함수를 호출한다.
//-----------------------------------------------------------------------------
BOOL
MCompareManager::Compare(HELP_COMPARE message)
{
	if (m_CompareFunction[message]==NULL)
	{
		DEBUG_ADD_FORMAT("[Error] NO CompareFunction. msg=%d", (int)message);
	}
	else
	{
		return (this->*m_CompareFunction[message])();
	}

	return FALSE;
}

