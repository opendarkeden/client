//----------------------------------------------------------------------
// MSortedItemManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MSortedItemManager.h"
#include "MItem.h"

#define	TWO_BY_TWO_PACKING_SIZE		12

//----------------------------------------------------------------------
//
// constructor / destructor
// 
//----------------------------------------------------------------------
MSortedItemManager::MSortedItemManager()
{
	m_NumTwoByTwo = 0;
	m_NumTwoByTwoTemp = 0;
}

MSortedItemManager::~MSortedItemManager()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
// 모두 delete하고 map을 지운다.
//----------------------------------------------------------------------
void		
MSortedItemManager::Release()
{
	ITEM_MAP::iterator iItem = begin();

	while (iItem != end())
	{
		MItem* pItem = iItem->second;

		if (pItem!=NULL)
		{
			delete pItem;
		}

		iItem++;
	}

	clear();
}

//----------------------------------------------------------------------
// Set Two By Two Number
//----------------------------------------------------------------------
void		
MSortedItemManager::SetTwoByTwoNumber(int n)
{ 
	m_NumTwoByTwo = n; 
	m_NumTwoByTwoTemp = (n/3) * 3;
}

//----------------------------------------------------------------------
// Add Item
//----------------------------------------------------------------------
bool		
MSortedItemManager::AddItem(MItem* pItem)
{
	ULONGLONG key = GetKey( pItem );

	ITEM_MAP::const_iterator iItem = find( key );	// 의미 있을까 - -;

	// 없으면 추가한다.
	if (iItem == end())
	{
		insert( ITEM_MAP::value_type( key, pItem ) );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Get Key
//----------------------------------------------------------------------
//
// key는 8 byte이고 상위byte부터..
//
// 4 Byte : gridWidth*gridHeight
// 4 Byte : ItemObjectID
//
// 로 표현된다.
//----------------------------------------------------------------------
// m_NumTwoByTwoTemp에는 우선 순위가 높아질 
//							2x2아이템의 개수가 설정되어야 한다.
//----------------------------------------------------------------------
ULONGLONG		
MSortedItemManager::GetKey(MItem* pItem)
{
	TYPE_OBJECTID itemObjectID= pItem->GetID();

	// item의 grid 크기
	int gridWidth		= pItem->GetGridWidth();
	int gridHeight		= pItem->GetGridHeight();
	int gridSize		= gridWidth*gridHeight;
	
	if (gridSize==4)	//gridWidth==2 && gridHeight==2)
	{
		if (m_NumTwoByTwoTemp > 0)
		{
			m_NumTwoByTwoTemp --;

			gridSize = TWO_BY_TWO_PACKING_SIZE;	// 2x2 3개가 모이면 12가 된다.
		}
	}

	// 큰것부터 sort하기 위해서...
	gridSize = 0xFF - gridSize;

	// item의 grid에서 좌표
	//int gridX			= pItem->GetGridX();
	//int gridY			= pItem->GetGridY();

	ULONGLONG key = gridSize;	//(gridSize << 16) | (gridX << 8) | gridY;

	key = (key << 32) | itemObjectID;

	return key;
}

//----------------------------------------------------------------------
// Merge
//----------------------------------------------------------------------
// 다른 map을 추가한다.
//----------------------------------------------------------------------
/*
void		
MSortedItemManager::Merge(const MSortedItemManager& sim)
{
	//----------------------------------------------------------------
	//
	//		2x2 size 아이템의 개수 판단
	//
	//----------------------------------------------------------------
	// 세개씩 묶여지지 않고 남아있는 2x2 size의 아이템 개수
	//----------------------------------------------------------------
	int numOldReminder = m_NumTwoByTwo%3;		// 기존에 있던거 중에서.. 
	
	int numNewReminder = sim.m_NumTwoByTwo%3;	// 새로 추가되는 것 중에서..

	//----------------------------------------------------------------
	// 두 map을 합쳤을때.. 3개 이상이 나온다면.. 	
	//----------------------------------------------------------------
	if (numOldReminder+numNewReminder >= 3)
	{		
		// 기존에 있던 map에서 남은걸 모두 묶어야 한다.
		ChangeTwoByTwoPriority( numOldReminder );
	}

	//----------------------------------------------------------------
	// 새거 추가할때.. 2x2아이템의 개수 설정..
	//----------------------------------------------------------------
	m_NumTwoByTwoTemp = (sim.m_NumTwoByTwo/3)*3 + (3-numOldReminder);

	//----------------------------------------------------------------
	// this에 sim의 아이템들을 추가시킨다.
	//----------------------------------------------------------------
	ITEM_MAP::const_iterator iItem = sim.begin();

	while (iItem != sim.end())
	{
		ULONGLONG	key		= iItem->first;
		MItem*		pItem	= iItem->second;

		//AddItem( pItem ); 

		// 속도를 위해선.. 이거를 부르는게 좋다 - -;
		insert( ITEM_MAP::value_type( key, pItem ) );					

		iItem++;
	}

	//----------------------------------------------------------------
	// 개수 다시 설정
	//----------------------------------------------------------------
	m_NumTwoByTwo = m_NumTwoByTwo + sim.m_NumTwoByTwo;
}
*/

//----------------------------------------------------------------------
// ChangeTwoByTwoPriority ( 개수 )
//----------------------------------------------------------------------
// TwoByTwo 아이템들을 찾아서 Priority를 증가시킨다. - -;
//----------------------------------------------------------------------
/*
void
MSortedItemManager::ChangeTwoByTwoPriority(int n)
{	
	//-------------------------------------------------
	// 음냐..
	//-------------------------------------------------
	if (n <= 0)
	{
		return;
	}

	//-------------------------------------------------
	// default 2x2 아이템 size
	//-------------------------------------------------
	const ULONGLONG TwoByTwoGridSize = 0xFF - 2*2;
	const ULONGLONG TwoByTwoPackingGridSize = 0xFF - TWO_BY_TWO_PACKING_SIZE;
	
	ITEM_MAP::iterator iItem = begin();
	ITEM_MAP::iterator iItemTemp;

	while (n>0 && iItem != end())
	{
		ULONGLONG	key		= iItem->first;
		MItem*		pItem	= iItem->second;

		//-------------------------------------------------
		// 2x2 이면...
		//-------------------------------------------------
		if ((key >> 32) == TwoByTwoGridSize)
		{
			iItemTemp = iItem;

			iItem++;

			//-------------------------------------------------
			// 제거 했다가
			//-------------------------------------------------
			erase( iItemTemp );

			key = (TwoByTwoPackingGridSize << 32) | (key & 0xFFFFFFFF);

			//-------------------------------------------------
			// 다시 추가
			//-------------------------------------------------
			// 추가될때는.. 우선 순위가 높으므로
			// 현재 iterating에서는 문제가 없다..고 본다 - -;
			insert( ITEM_MAP::value_type(key, pItem) );			

			//-------------------------------------------------
			// 개수 감소
			//-------------------------------------------------
			n--;
		}
		else
		{
			iItem++;
		}
	}
}
*/