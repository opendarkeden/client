//----------------------------------------------------------------------
// MShop.h
//----------------------------------------------------------------------
// Shop은 Shelf가 모여서 이루어진다.
//
// --> Shelf의 array
//
// SetShelf는 외부에서 new한 shelf를 추가하면 되고..
// delete는 MShop 내부에서 하게 된다.
//
// current를 설정해서 현재에 사용중인 shelf를 설정할 수 있다.
//----------------------------------------------------------------------
/*

	MShop*		pShop;
	MShopShelf*	pShelf;

	//----------------------------------------------------
	// 3개의 shelf를 가진 상점
	//----------------------------------------------------
	pShop = new MShop;
	pShop->Init( 3 );		

	//----------------------------------------------------
	// [0] Fixed Shelf
	//----------------------------------------------------
	pShelf = new MShopFixedShelf;
	.....	// pShelf에 대한 초기화..
	if (!pShop->SetShelf(0, pShelf))
	{
		// 추가가 안 된 경우는 외부에서 delete해줘야 한다.
		// shelf 번호가 상점 전체 개수보다 큰 경우에 추가 실패당..
		delete pShelf;
	}
  
	//----------------------------------------------------
	// [1] Special Shelf
	//----------------------------------------------------
	pShelf = new MShopSpecialShelf;
	.....
	pShop->SetShelf(0, pShelf);

	//----------------------------------------------------
	// [2] Unknown Shelf
	//----------------------------------------------------
	pShelf = new MShopUnknownShelf;
	.....
	pShop->SetShelf(0, pShelf);

	
	//----------------------------------------------------
	// 접근
	//----------------------------------------------------
	for (int i=0; i<pShop->GetSize(); i++)
	{
		// [i]번째 shelf를 읽어온다.
		MShopShelf* pShelf = pShop->GetShelf(i);		

		pShelf->GetName(); // 각 shelf의 이름
	}
	
	//----------------------------------------------------
	// 현재 shelf 지정하기
	//----------------------------------------------------
	pShop->SetCurrent( 1 );		// special shelf에 접근한다.
	MShopShelf* pShelf = pShop->GetCurrentShelf();		// 현재 사용중인 shelf읽어오기

	....
*/
//----------------------------------------------------------------------

#ifndef __MSHOP_H__
#define	__MSHOP_H__

class MShopShelf;

class MShop {
	public :
		enum SHOP_TYPE
		{
			SHOP_NORMAL,
			SHOP_EVENT_STAR
		};

	public :
		MShop();
		~MShop();

		//-------------------------------------------------------
		// Init / Release
		//-------------------------------------------------------
		void				Init(unsigned int shopType);
		void				Release();
	
		//-------------------------------------------------------		
		// Shop Type
		//-------------------------------------------------------		
		SHOP_TYPE			GetShopType() const				{ return m_ShopType; }
		void				SetShopType(SHOP_TYPE type)		{ m_ShopType = type; }

		//-------------------------------------------------------		
		// Size
		//-------------------------------------------------------		
		unsigned int		GetSize() const		{ return m_Size; }

		//-------------------------------------------------------
		// Shelf
		//-------------------------------------------------------		
		bool				SetShelf(unsigned int shopType, MShopShelf* pShelf);
		MShopShelf*			GetShelf(unsigned int shopType) const;

		//-------------------------------------------------------		
		// 현재 접근 중인 shelf
		//-------------------------------------------------------		
		void				SetCurrent(unsigned int shopType);
		unsigned int		GetCurrent() const			{ return m_CurrentShelf; }
		MShopShelf*			GetCurrentShelf() const		{ return m_pShelf[m_CurrentShelf]; }


	protected :
		unsigned int		m_Size;			// 전체 shelf 개수
		MShopShelf**		m_pShelf;

		// 현재 사용중인 shelf
		unsigned int		m_CurrentShelf;

		// 상점의 종류
		SHOP_TYPE			m_ShopType;
};

#endif


