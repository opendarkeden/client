//----------------------------------------------------------------------
// MGridItemManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MItem.h"
#include "MItemFinder.h"
#include "MGridItemManager.h"

//#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MGridItemManager::MGridItemManager()
{
	m_Width = 0;
	m_Height = 0;
	m_ItemGrid = NULL;
}

MGridItemManager::~MGridItemManager()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void		
MGridItemManager::Init(BYTE width, BYTE height)
{
	Release();

	if (width!=0 && height!=0)
	{
		m_Width = width;
		m_Height = height;

		// MItem* m_ItemGrid[m_Height][m_Width] 의 메모리 잡기
		m_ItemGrid = new MItem** [m_Height];

		for (int i=0; i<m_Height; i++)
		{
			m_ItemGrid[i] = new MItem* [m_Width];

			// NULL로 초기화
			for (int j=0; j<m_Width; j++)
			{
				m_ItemGrid[i][j] = NULL;
			}
		}
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void		
MGridItemManager::Release()
{
	MItemManager::Release();


	if (m_ItemGrid!=NULL)
	{
		for (int i=0; i<m_Height; i++)
		{
			delete [] m_ItemGrid[i];
		}			

		delete [] m_ItemGrid;
		
		m_ItemGrid = NULL;

		m_Width = 0;
		m_Height = 0;
	}
}

//----------------------------------------------------------------------
// Find 
//----------------------------------------------------------------------
// pItem을 넣을 수 있는 적절한 grid위치를 얻는다.
// First-Fit를 적용하면 된다.
//----------------------------------------------------------------------
bool			
MGridItemManager::GetFitPosition(MItem* pItem, POINT& point)
{
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();
	
	int x, y;
	int i, j;
	
	//---------------------------------------------------------
	// grid의 모든(x,y)에 pItem을 추가할 수 있는지 검사해본다.
	//---------------------------------------------------------
	int yLimit = m_Height - height;
	int xLimit = m_Width - width;

	int yPlusHeight, xPlusWidth;

	for (x=0; x<=xLimit; x++)
	{
		xPlusWidth = x+width;
		
		for (y=0; y<=yLimit; y++)
		{			
			yPlusHeight = y+height;

			//---------------------------------------------------------
			// (x,y)에 넣을 수 있는지 체크..
			//---------------------------------------------------------
			bool bPlace = true;

			for (i=y; bPlace && i<yPlusHeight; i++)
			{
				for (j=x; bPlace && j<xPlusWidth; j++)
				{
					//---------------------------------------------------------
					// 이미 다른 Item이 있는 grid가 하나라도 있다면 추가할 수 없다.
					//---------------------------------------------------------
					if (m_ItemGrid[i][j]!=NULL)
					{
						bPlace = false;

						// 다음에 체크할 것...
						//x = j + m_ItemGrid[i][j]->GetGridWidth() - 1;
						//y = i + m_ItemGrid[i][j]->GetGridHeight() - 1;

						break;
					}
				}
			}
			
			//---------------------------------------------------------
			// (x,y)에 넣을 수 있는 경우
			//---------------------------------------------------------
			if (bPlace)
			{
				point.x = x;
				point.y = y;
				
				return true;
			}
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Can Replace Item
//----------------------------------------------------------------------
// 실제로는 Add or Replace를 의미한다.
// (x,y)에 pItem을 넣을 수 있거나
// (x,y)에 놓았을 때, 다른 것 하나와 교체가 될 경우는 true이다.
// pOldItem에는 교체될 수 있는 경우에 원래 있던 item이 들어간다.(체크용)
//----------------------------------------------------------------------
bool			
MGridItemManager::CanReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem)
{
	pOldItem = NULL;

	//---------------------------------------------------------
	// ItemGrid 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x>=m_Width || y>=m_Height)
	{
		return false;
	}

	// (x,y)부터 pItem의 영역 안에 있는 Item들을 체크해본다.
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	int i,j;
	int yPlusHeight = y+height;
	int xPlusWidth = x+width;

	//---------------------------------------------------------
	// Grid의 범위를 벗어나는 경우
	//---------------------------------------------------------
	if (xPlusWidth > m_Width || yPlusHeight > m_Height)
	{
		return false;
	}

	//---------------------------------------------------------
	// pItem이 위치할 곳에 Item이 하나만 있거나
	// 없어야 한다.
	//---------------------------------------------------------
	MItem* pCheckItem = NULL;
	MItem* pCurrentCheckItem;

	for (i=y; i<yPlusHeight; i++)
	{
		for (j=x; j<xPlusWidth; j++)
		{
			//---------------------------------------------------------
			// 이미 다른 Item이 있는 경우.
			//---------------------------------------------------------
			pCurrentCheckItem = m_ItemGrid[i][j];
			if (pCurrentCheckItem!=NULL)
			{
				if (pCheckItem==NULL)
				{
					//---------------------------------------------------------
					// 하나의 Item은 기억해둔다.
					//---------------------------------------------------------
					pCheckItem = pCurrentCheckItem;				
				}
				else
				{
					//---------------------------------------------------------
					// 여러개의 Item이 존재하는 경우
					//---------------------------------------------------------
					if (pCheckItem->GetID() != pCurrentCheckItem->GetID())
					{					
						return false;
					}
				}

				
			}
		}
	}

	// 있든지 말든지.. 그냥 대입하면 된다.
	pOldItem = pCheckItem;

	return true;
}

//----------------------------------------------------------------------
// Add
//----------------------------------------------------------------------
// pItem을 적절한 grid위치에 추가한다.
// First-Fit를 적용하면 된다.
//----------------------------------------------------------------------
bool			
MGridItemManager::AddItem(MItem* pItem)
{
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	
	int x, y;
	int i, j;
	
	//---------------------------------------------------------
	// grid의 모든(x,y)에 pItem을 추가할 수 있는지 검사해본다.
	//---------------------------------------------------------
	int yLimit = m_Height - height;
	int xLimit = m_Width - width;

	int yPlusHeight, xPlusWidth;

	for (x=0; x<=xLimit; x++)
	{
		xPlusWidth = x+width;
		
		for (y=0; y<=yLimit; y++)	
		{			
			yPlusHeight = y+height;

			//---------------------------------------------------------
			// (x,y)에 넣을 수 있는지 체크..
			//---------------------------------------------------------
			bool bPlace = true;

			for (i=y; bPlace && i<yPlusHeight; i++)
			{
				for (j=x; bPlace && j<xPlusWidth; j++)
				{
					//---------------------------------------------------------
					// 이미 다른 Item이 있는 grid가 하나라도 있다면 추가할 수 없다.
					//---------------------------------------------------------
					if (m_ItemGrid[i][j]!=NULL)
					{
						bPlace = false;

						// 다음에 체크할 것...
						y = i + m_ItemGrid[i][j]->GetGridHeight() - 1;
						
						break;
					}
				}
			}
			
			//---------------------------------------------------------
			// (x,y)에 넣을 수 있는 경우
			//---------------------------------------------------------
			if (bPlace)
			{
				//---------------------------------------------------------
				// 내부의 map에 추가시킨다.
				// 제대로 추가되면. grid에도 표시를 한다.
				//---------------------------------------------------------
				if (MItemManager::AddItem( pItem ))
				{
					//---------------------------------------------------------
					// item의 grid좌표를 설정
					//---------------------------------------------------------
					pItem->SetGridXY( x, y );

					//---------------------------------------------------------
					// Grid에 표시
					//---------------------------------------------------------
					for (i=y; i<yPlusHeight; i++)
					{
						for (j=x; j<xPlusWidth; j++)
						{
							m_ItemGrid[i][j] = pItem;
						}
					}

					return true;
				}
			}
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Add
//----------------------------------------------------------------------
// pItem을 (x,y)에 추가한다.
//----------------------------------------------------------------------
bool			
MGridItemManager::AddItem(MItem* pItem, BYTE x, BYTE y)
{
	//---------------------------------------------------------
	// ItemGrid 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x>=m_Width || y>=m_Height)
	{
		return false;
	}

	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	
	int i,j;
	int yPlusHeight = y+height;
	int xPlusWidth = x+width;

	//---------------------------------------------------------
	// Grid의 범위를 벗어나는 경우
	//---------------------------------------------------------
	if (xPlusWidth > m_Width || yPlusHeight > m_Height)
	{
		return false;
	}

	//---------------------------------------------------------
	// pItem이 위치할 곳이 모두 비어있어야 한다.
	//---------------------------------------------------------
	for (i=y; i<yPlusHeight; i++)
	{
		for (j=x; j<xPlusWidth; j++)
		{
			//---------------------------------------------------------
			// 이미 다른 Item이 있는 grid가 하나라도 있다면 추가할 수 없다.
			//---------------------------------------------------------
			if (m_ItemGrid[i][j]!=NULL)
				return false;
		}
	}	

	//---------------------------------------------------------
	// 내부의 map에 추가시킨다.
	// 제대로 추가되면. grid에도 표시를 한다.
	//---------------------------------------------------------
	if (MItemManager::AddItem( pItem ))
	{
		//---------------------------------------------------------
		// item의 grid좌표를 설정
		//---------------------------------------------------------
		pItem->SetGridXY( x, y );

		//---------------------------------------------------------
		// Grid에 표시
		//---------------------------------------------------------
		for (i=y; i<yPlusHeight; i++)
		{
			for (j=x; j<xPlusWidth; j++)
			{
				m_ItemGrid[i][j] = pItem;
			}
		}

		return true;
	}

	return false;	
}

//----------------------------------------------------------------------
// Get
//----------------------------------------------------------------------
// grid(x,y)의 item을 읽어온다.
//----------------------------------------------------------------------
MItem*	
MGridItemManager::GetItem(BYTE x, BYTE y) const
{
	//---------------------------------------------------------
	// ItemGrid 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x>=m_Width || y>=m_Height)
	{
		return NULL;
	}

	return m_ItemGrid[y][x];
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// grid(x,y)의 item을 제거한다.
//----------------------------------------------------------------------
MItem*			
MGridItemManager::RemoveItem(BYTE x, BYTE y)
{
	//---------------------------------------------------------
	// ItemGrid 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x>=m_Width || y>=m_Height)
	{
		return NULL;
	}

	MItem* pItem = m_ItemGrid[y][x];

	//---------------------------------------------------------
	// (x,y)에 item이 없는 경우..
	//---------------------------------------------------------
	if (pItem == NULL)
	{
		return NULL;
	}

	//---------------------------------------------------------	
	// ItemManager의 map에서 제거한다.
	//---------------------------------------------------------	
	pItem = MItemManager::RemoveItem( pItem->GetID() );

	// 없는 경우..
	if (pItem == NULL)
	{
		return NULL;
	}

	// grid의 pItem과 map의 pItem이 같은지는
	// 체크 안해도 되겠지.. - -;

	//---------------------------------------------------------	
	// item이 존재하는 각 grid의 정보를 없애줘야 한다.
	//---------------------------------------------------------
	// (x,y)를 grid 첫위치로..
	x = pItem->GetGridX();
	y = pItem->GetGridY();
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	int i,j;
	int yPlusHeight = y + height;
	int xPlusWidth = x + width;

	// 각 grid를 NULL로 만든다.
	for (i=y; i<yPlusHeight; i++)
	{
		for (j=x; j<xPlusWidth; j++)
		{
			m_ItemGrid[i][j] = NULL;				
		}
	}	

	//---------------------------------------------------------
	// 좌표를 지워준다.
	//---------------------------------------------------------
	// 별로 의미 없는 코드 같은데.. - -;; 그냥..
	//---------------------------------------------------------
	//pItem->SetGridXY(0, 0);

	return pItem;
}

MItem *MGridItemManager::GetItem(TYPE_OBJECTID id) const
{
	return MItemManager::GetItem(id);
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// grid(x,y)의 item을 제거한다.
//----------------------------------------------------------------------
MItem*			
MGridItemManager::RemoveItem(TYPE_OBJECTID id)
{
	MItem* pItem = MItemManager::RemoveItem( id );

	//---------------------------------------------------------
	// 그런 id를 가진 item이 없는 경우
	//---------------------------------------------------------
	if (pItem==NULL)
	{
		return NULL;
	}

	//---------------------------------------------------------	
	// item이 존재하는 각 grid의 정보를 없애줘야 한다.
	//---------------------------------------------------------
	int x	= pItem->GetGridX();
	int y	= pItem->GetGridY();
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	int i,j;
	int yPlusHeight = y + height;
	int xPlusWidth = x + width;

	// 각 grid를 NULL로 만든다.
	for (i=y; i<yPlusHeight; i++)
	{
		for (j=x; j<xPlusWidth; j++)
		{
			m_ItemGrid[i][j] = NULL;				
		}
	}	

	//---------------------------------------------------------
	// 좌표를 지워준다.
	//---------------------------------------------------------
	// 별로 의미 없는 코드 같은데.. - -;; 그냥..
	//---------------------------------------------------------
	//pItem->SetGridXY(0, 0);

	return pItem;
}

//----------------------------------------------------------------------
// Replace Item
//----------------------------------------------------------------------
// (x,y)위치부터 pItem의 크기 안에 있는 Item과 replace
// pOldItem에 이전에 있던것이 들어간다.
//----------------------------------------------------------------------
bool			
MGridItemManager::ReplaceItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem)
{
	pOldItem = NULL;

	//---------------------------------------------------------
	// ItemGrid 경계를 넘어가는 경우..
	//---------------------------------------------------------
	if (x>=m_Width || y>=m_Height)
	{
		return false;
	}	

	// (x,y)부터 pItem의 영역 안에 있는 Item들을 체크해본다.
	int width = pItem->GetGridWidth();
	int height = pItem->GetGridHeight();

	int i,j;
	int yPlusHeight = y+height;
	int xPlusWidth = x+width;

	//---------------------------------------------------------
	// Grid의 범위를 벗어나는 경우
	//---------------------------------------------------------
	if (xPlusWidth > m_Width || yPlusHeight > m_Height)
	{
		return false;
	}

	//---------------------------------------------------------
	// pItem이 위치할 곳에 Item이 하나만 있거나
	// 없어야 한다.
	//---------------------------------------------------------
	MItem* pCheckItem = NULL;
	MItem* pCurrentCheckItem;

	for (i=y; i<yPlusHeight; i++)
	{
		for (j=x; j<xPlusWidth; j++)
		{
			//---------------------------------------------------------
			// 이미 다른 Item이 있는 경우.
			//---------------------------------------------------------
			pCurrentCheckItem = m_ItemGrid[i][j];
			if (pCurrentCheckItem!=NULL)
			{
				if (pCheckItem==NULL)
				{
					//---------------------------------------------------------
					// 하나의 Item은 기억해둔다.
					//---------------------------------------------------------
					pCheckItem = pCurrentCheckItem;				
				}
				else
				{
					//---------------------------------------------------------
					// 여러개의 Item이 존재하는 경우
					//---------------------------------------------------------
					if (pCheckItem->GetID() != pCurrentCheckItem->GetID())
					{					
						return false;
					}
				}

				
			}
		}
	}

	//---------------------------------------------------------
	// pItem을 추가할려는 자리에 다른 Item이 하나 있는 경우
	//---------------------------------------------------------
	// 그 Item을 제거한다.
	//---------------------------------------------------------
	if (pCheckItem!=NULL)
	{
		//---------------------------------------------------------	
		// 다른 Item을 제거한다.
		//---------------------------------------------------------	
		// ItemManager의 map에서 제거한다.
		//---------------------------------------------------------	
		if (MItemManager::RemoveItem( pCheckItem->GetID() ) == NULL)
		{
			return false;
		}

		// grid의 pItem과 map의 pItem이 같은지는
		// 체크 안해도 되겠지.. - -;

		//---------------------------------------------------------	
		// item이 존재하는 각 grid의 정보를 없애줘야 한다.
		//---------------------------------------------------------
		// (x,y)를 grid 첫위치로..
		int ox = pCheckItem->GetGridX();
		int oy = pCheckItem->GetGridY();
		int owidth = pCheckItem->GetGridWidth();
		int oheight = pCheckItem->GetGridHeight();

		int oyPlusHeight = oy + oheight;
		int oxPlusWidth = ox + owidth;

		// 각 grid를 NULL로 만든다.
		for (i=oy; i<oyPlusHeight; i++)
		{
			for (j=ox; j<oxPlusWidth; j++)
			{
				m_ItemGrid[i][j] = NULL;				
			}
		}	

		// 별로 의미 없는 코드 
		//pCheckItem->SetGridXY(0, 0);

		// 이전에 있던 Item을 넘겨준다.
		pOldItem = pCheckItem;
	}

	//---------------------------------------------------------
	// pItem을 (x,y)에 추가한다.
	//---------------------------------------------------------
	// 내부의 map에 추가시킨다.
	// 제대로 추가되면. grid에도 표시를 한다.
	//---------------------------------------------------------
	if (MItemManager::AddItem( pItem ))
	{
		//---------------------------------------------------------
		// item의 grid좌표를 설정
		//---------------------------------------------------------
		pItem->SetGridXY( x, y );

		//---------------------------------------------------------
		// Grid에 표시
		//---------------------------------------------------------
		for (i=y; i<yPlusHeight; i++)
		{
			for (j=x; j<xPlusWidth; j++)
			{
				m_ItemGrid[i][j] = pItem;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------
// Find Item Grid Order
//----------------------------------------------------------------------
// 좌측에서 오른쪽으로.. 
// 위에서 아래로 검색..
//----------------------------------------------------------------------
MItem*			
MGridItemManager::FindItemGridOrder( MItemFinder& itemFinder ) const
{
	int x, y;
	int i, j;	

	//---------------------------------------------------------
	// grid 체크용
	//---------------------------------------------------------	
	bool** bCheck = new bool* [m_Height];
	for (i=0; i<m_Height; i++)
	{
		bCheck[i] = new bool [m_Width];

		memset( bCheck[i], 0, m_Width );

		//for (int j=0; j<m_Width; j++)
		//{
		//	bCheck[i][j] = false;
		//}
	}
	
	//---------------------------------------------------------
	// grid의 순서대로 item을 검색해본다.
	//---------------------------------------------------------
	for (y=0; y<m_Height; y++)
	{
		for (x=0; x<m_Width; x++)	
		{		
			// 이미 체크 했다면.. 체크하지 않는다.
			if (bCheck[y][x])
			{
				continue;
			}

			MItem* pItem = m_ItemGrid[y][x];
			
			if (pItem!=NULL)
			{
				if (itemFinder( pItem ))
				{
					return pItem;
				}

				// 아이템의 grid만큼을 체크한다.
				int maxY = y + pItem->GetGridHeight();
				int maxX = x + pItem->GetGridWidth();

				// 이미 item이 들어간 좌표이므로 grid를 벗어나지 않는다고 가정한다.
				// 한칸짜리는 체크 안해도 되는데.. 그냥..
				for (i=y; i<maxY; i++)
				{
					for (j=x; j<maxX; j++)
					{
						bCheck[i][j] = true;
					}
				}
			}
		}
	}

	//---------------------------------------------------------
	// 체크용 지우기
	//---------------------------------------------------------
	if (bCheck!=NULL)
	{
		for (int i=0; i<m_Height; i++)
		{
			delete [] bCheck[i];
		}			

		delete [] bCheck;		
	}

	return false;
}

