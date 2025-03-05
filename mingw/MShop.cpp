//----------------------------------------------------------------------
// MShop.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MShop.h"
#include "MShopShelf.h"

#ifdef __GAME_CLIENT__
	#include "MPlayer.h"
#endif

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MShop::MShop()
{
	m_Size			= 0;
	m_CurrentShelf	= 0;
	m_pShelf		= NULL;

	m_ShopType		= SHOP_NORMAL;
}

MShop::~MShop()
{
	Release();
}

//----------------------------------------------------------------------
// Init ( n )
//----------------------------------------------------------------------
void				
MShop::Init(unsigned int n)
{
	Release();
	
	m_Size		= n;
	m_pShelf	= new MShopShelf* [m_Size];

	// NULL로 초기화 한다.
	for (int i=0; i<m_Size; i++)
	{
		m_pShelf[i] = NULL;
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void				
MShop::Release()
{
	//---------------------------------------------------
	// 지운다.
	//---------------------------------------------------
	if (m_pShelf!=NULL)
	{
		for (int i=0; i<m_Size; i++)
		{
			if (m_pShelf[i]!=NULL)
			{
				delete m_pShelf[i];
			}
		}

		delete [] m_pShelf;
	}

	m_pShelf		= NULL;
	m_Size			= 0;
	m_CurrentShelf	= 0;
}

//----------------------------------------------------------------------
// SetShelf( n,  pShelf )
//----------------------------------------------------------------------
bool				
MShop::SetShelf(unsigned int n, MShopShelf* pShelf)
{
	if (pShelf==NULL)
	{
		return false;
	}

	//--------------------------------------------------------
	// 기존에 다른 shelf가 이미 있는 경우... --> 지운다.
	//--------------------------------------------------------
	if (m_pShelf[n]!=NULL)
	{
		delete m_pShelf[n];
	}

	//--------------------------------------------------------
	// 새로운 pShelf를 대입
	//--------------------------------------------------------
	m_pShelf[n] = pShelf;

	return true;
}

//----------------------------------------------------------------------
// Get Shelf ( n )
//----------------------------------------------------------------------
MShopShelf*			
MShop::GetShelf(unsigned int n) const
{
	if (n>=m_Size)
	{
		return NULL;
	}

	return m_pShelf[n];
}

//----------------------------------------------------------------------
// Set Current( n )
//----------------------------------------------------------------------
// 현재 접근 중인 Shelf를 지정한다.
//----------------------------------------------------------------------
void				
MShop::SetCurrent(unsigned int n)
{
	if (n>=m_Size)
	{
		return;
	}

	//------------------------------------------------------
	// 사용 가능 여부 체크
	//------------------------------------------------------
	#ifdef __GAME_CLIENT__
		MShopShelf* pShopShelf = m_pShelf[n];

		if (pShopShelf!=NULL)
		{		
			for (int i=0; i<SHOP_SHELF_SLOT; i++)
			{
				MItem* pItem = pShopShelf->GetItem( i );

				if (pItem!=NULL)
				{	
					g_pPlayer->CheckAffectStatus( pItem );
				}
			}
		}
	#endif

	m_CurrentShelf = n;
}

