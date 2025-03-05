//----------------------------------------------------------------------
// MPlayerGear.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MPlayerGear.h"
#include "MItem.h"
#include "MHelpManager.h"
#include "debuginfo.h"

#ifdef __GAME_CLIENT__
	#include "ClientConfig.h"
#endif

//----------------------------------------------------------------------
// 
// constructor / destructor
//
//----------------------------------------------------------------------
MPlayerGear::MPlayerGear()
{
	m_nBrokenItem = 0;
	m_pItemStatus = NULL;
}

MPlayerGear::~MPlayerGear()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init ItemStatus ( n개 )
//----------------------------------------------------------------------
void			
MPlayerGear::Init(BYTE size)
{
	MSlotItemManager::Init( size );

	//--------------------------------------------------------
	// Item 상태를 모두 OK로 설정한다.
	//--------------------------------------------------------
	if (m_pItemStatus!=NULL)
	{
		delete [] m_pItemStatus;
	}

	m_pItemStatus = new ITEM_STATUS [size];

	for (int i=0; i<size; i++)
	{
		m_pItemStatus[i] = ITEM_STATUS_OK;
	}

	m_nBrokenItem = 0;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void			
MPlayerGear::Release()
{
	MSlotItemManager::Release();

	if (m_pItemStatus!=NULL)
	{
		delete [] m_pItemStatus;
		m_pItemStatus = NULL;
	}
}

//----------------------------------------------------------------------
// Add Item
//----------------------------------------------------------------------
bool			
MPlayerGear::AddItem(MItem* pItem, BYTE n)
{
	bool bOK = MSlotItemManager::AddItem(pItem, n);

	if (bOK)
	{
		CheckItemStatus( pItem, n );
	}

	return bOK;
}


//----------------------------------------------------------------------
// Get Item
//----------------------------------------------------------------------
// get get get~
//----------------------------------------------------------------------
MItem*
MPlayerGear::GetItem(BYTE n) const
{
	return MSlotItemManager::GetItem( n );	// 음냐 하하..
}

//----------------------------------------------------------------------
// Remove Item ( slot )
//----------------------------------------------------------------------
MItem*			
MPlayerGear::RemoveItem(BYTE n)
{
	MItem* pItem = MSlotItemManager::RemoveItem( n );

	if (pItem!=NULL)
	{
		// 제거한다는 의미이다.
		SetItemStatusOK( n );
	}

	return pItem;
}

//----------------------------------------------------------------------
// Remove Item ( id )
//----------------------------------------------------------------------
MItem*			
MPlayerGear::RemoveItem(TYPE_OBJECTID id)
{
	MItem* pItem = MSlotItemManager::RemoveItem( id );

	if (pItem!=NULL)
	{
		// 제거한다는 의미이다.
		SetItemStatusOK( pItem->GetItemSlot() );
	}

	return pItem;
}

//----------------------------------------------------------------------
// Replace Item
//----------------------------------------------------------------------
bool			
MPlayerGear::ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem)
{
	bool bAdded = MSlotItemManager::ReplaceItem( pItem, n, pOldItem );

	if (bAdded)
	{
		if (pOldItem!=NULL)
		{
			SetItemStatusOK( pOldItem->GetItemSlot() );		// 일단 OK상태로..
		}
		
		// Replace는 문제가 좀 많아서 상속받은데서 처리해야 한다.
		//CheckItemStatus( pItem, n );	// 새로운 item에 대한 체크				
	}

	return bAdded;
}

//----------------------------------------------------------------------
// Set ItemStatus OK ( slot )
//----------------------------------------------------------------------
// slot번째는 좋은 상태의 아이템이다..라고 한다. - -;
// Remove된 경우... 그냥 OK라고 하기 위해서다.
//----------------------------------------------------------------------
void			
MPlayerGear::SetItemStatusOK(int slot)
{
	//----------------------------------------------------------
	// 부서졌던 item인 경우...
	//----------------------------------------------------------
	if (m_pItemStatus[slot]!=ITEM_STATUS_OK)
	{
		m_pItemStatus[slot] = ITEM_STATUS_OK;

		if (--m_nBrokenItem < 0)
		{
			m_nBrokenItem = 0;
		}
	}
}

//----------------------------------------------------------------------
// Check ItemStatus
//----------------------------------------------------------------------
// pItem의 상태를 slot에 설정한다.
//----------------------------------------------------------------------
void			
MPlayerGear::CheckItemStatus(const MItem* pItem, int slot)
{
	ITEM_STATUS oldStatus = m_pItemStatus[slot];

	TYPE_ITEM_DURATION	maxDur = pItem->GetMaxDurability();
	TYPE_ITEM_DURATION	curDur = pItem->GetCurrentDurability();

	// 내구성 상태.. % 
	TYPE_ITEM_DURATION	itemStatusPer = 0;

	if(maxDur <= 0 ||pItem->IsSpecialColorItem() || pItem->IsDurationAlwaysOkay())
		itemStatusPer = 100; 
	else
		itemStatusPer = curDur*100 / maxDur;


	//----------------------------------------------------------
	// 정상적인 상태		
	//----------------------------------------------------------
	if (itemStatusPer > 
#ifdef __GAME_CLIENT__
		g_pClientConfig->PERCENTAGE_ITEM_SOMEWHAT_BROKEN
#else
		25
#endif
		)
	{
		m_pItemStatus[slot] = ITEM_STATUS_OK;

		//----------------------------------------------------------
		// 부서졌다가 좋아진 상태이다.
		//----------------------------------------------------------
		if (oldStatus!=ITEM_STATUS_OK)
		{
			if (--m_nBrokenItem < 0)
			{
				m_nBrokenItem = 0;
			}
		}
	}
	//----------------------------------------------------------
	// 부서진 경우..
	//----------------------------------------------------------
	else
	{
		//----------------------------------------------------------
		// 거의 부서져가는 상태 --> 빨간색
		//----------------------------------------------------------
		if (itemStatusPer <= 
#ifdef __GAME_CLIENT__
			g_pClientConfig->PERCENTAGE_ITEM_ALMOST_BROKEN
#else
			10
#endif
			)
		{
			m_pItemStatus[slot] = ITEM_STATUS_ALMOST_BROKEN;
		}
		//----------------------------------------------------------
		// 약간? 부서진 상태			
		//----------------------------------------------------------
		else
		{
			m_pItemStatus[slot] = ITEM_STATUS_SOMEWHAT_BROKEN;
		}

		//----------------------------------------------------------
		// 좋아졌다가 부서졌으면..
		//----------------------------------------------------------
		if (oldStatus==ITEM_STATUS_OK)
		{
			m_nBrokenItem++;	// 한계를 넘을 수 있을까? - -;			
		}
	}		
}


//----------------------------------------------------------------------
// Modify Durability ( slot, 현재값 )
//----------------------------------------------------------------------
// 원래는 '변화값'이었는데.. '현재값'으로 설정한다.
//----------------------------------------------------------------------
bool			
MPlayerGear::ModifyDurability(BYTE n, int changeValue)
{
	//---------------------------------------------------------
	// ItemSlot 범위를 넘어가는 경우..
	//---------------------------------------------------------	
	if (n>=m_Size)
	{
		return false;
	}

	MItem* pItem = m_ItemSlot[n];

	if (pItem==NULL)
	{
		return false;
	}

	//int currentDurability	= pItem->GetCurrentDurability();
	int maxDurability		= pItem->GetMaxDurability();

	int modifyDurability = changeValue;//currentDurability + changeValue;

	//---------------------------------------------------------	
	// max를 넘어가는 경우
	//---------------------------------------------------------	
	if ( modifyDurability > maxDurability)
	{
		pItem->SetCurrentDurability( maxDurability );
	}
	//---------------------------------------------------------	
	// 0보다 적은 경우
	//---------------------------------------------------------	
	else if (modifyDurability < 0)
	{
		pItem->SetCurrentDurability( 0 );
	}
	//---------------------------------------------------------	
	// 정상적으로 바뀌는 경우
	//---------------------------------------------------------	
	else
	{
		pItem->SetCurrentDurability( modifyDurability );
	}

	//---------------------------------------------------------	
	// Item상태 체크
	//---------------------------------------------------------	
	ITEM_STATUS oldStatus = m_pItemStatus[n];
	
	CheckItemStatus( pItem, n );
#ifdef __GAME_CLIENT__
//#ifdef __USE_HELP_EVENT
//	__BEGIN_HELP_EVENT	
//		// OK였는데.. OK가 아니게 되는 경우
		if (oldStatus==ITEM_STATUS_OK && m_pItemStatus[n]!=ITEM_STATUS_OK)
		{
			// [도움말] 아이템이 부서져가는 경우
			ExecuteHelpEvent( HELP_EVENT_ITEM_REPAIR );
		}
//	__END_HELP_EVENT
//#endif
#endif	
	return true;
}

//----------------------------------------------------------------------
// Check ItemStatus All
//----------------------------------------------------------------------
// 모든 item의 item상태를 체크한다.
//----------------------------------------------------------------------
void			
MPlayerGear::CheckItemStatusAll()
{
	for (BYTE i=0; i<m_Size; i++)
	{
		m_pItemStatus[i] = ITEM_STATUS_OK;
	}

	m_nBrokenItem = 0;

	//---------------------------------------------------
	// 모두 체크
	//---------------------------------------------------
	for (i=0; i<m_Size; i++)
	{
		if (m_ItemSlot[i]!=NULL)
		{
			CheckItemStatus( m_ItemSlot[i], i );
		}
	}
}
