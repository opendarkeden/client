	//----------------------------------------------------------------------
// MSlayerGear.h
//----------------------------------------------------------------------
// Slayer의 Gear에 대한 class
//----------------------------------------------------------------------
/*
	//---------------------------------
	// 초기화 한다.
	//---------------------------------
	g_SlayerGear.Init();

	MItem* pNewItem;

	//--------------------------------------------------------
	// 테스트용 Item 생성
	//--------------------------------------------------------
	pNewItem = MItem::NewItem( ITEM_CLASS_BLADE );
	pNewItem->SetID( 3 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );


	pNewItem = MItem::NewItem( ITEM_CLASS_SWORD );
	pNewItem->SetID( 1 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	pNewItem = MItem::NewItem( ITEM_CLASS_BELT );
	pNewItem->SetID( 2 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	pNewItem = MItem::NewItem( ITEM_CLASS_RING );
	pNewItem->SetID( 4 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	pNewItem = MItem::NewItem( ITEM_CLASS_SHIELD );
	pNewItem->SetID( 5 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	pNewItem = MItem::NewItem( ITEM_CLASS_HELM );
	pNewItem->SetID( 6 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	pNewItem = MItem::NewItem( ITEM_CLASS_RING );
	pNewItem->SetID( 7 );
	pNewItem->SetItemType( 0 );
	g_SlayerGear.AddItem( pNewItem );

	//--------------------------------------------------------
	// Remove test
	//--------------------------------------------------------
  	g_SlayerGear.RemoveItem( MSlayerGear::GEAR_SLAYER_LEFTHAND );


	//--------------------------------------------------------
	// 출력방법1
	//--------------------------------------------------------
	// Item을 한번씩 출력
	//--------------------------------------------------------
	g_SlayerGear.SetBegin();
	while (g_SlayerGear.IsNotEnd())
	{
		const MItem* pItem = g_SlayerGear.Get();

		DEBUG_ADD_FORMAT("[ID=%d] name=%s, slot=%d", 
			pItem->GetID(),
			pItem->GetName(),
			pItem->GetItemSlot()
			);

		g_SlayerGear.Next();
	}

*/
//----------------------------------------------------------------------


#ifndef	__MSLAYERGEAR_H__
#define	__MSLAYERGEAR_H__

#include "MPlayerGear.h"
class MMagazine;


class MSlayerGear : public MPlayerGear	{
	public :
		//----------------------------------------------------
		// QuickItem Slot의 Slot ID
		//----------------------------------------------------
		// (!) ID를 바꾸면 s_GearSlotCheckTable의 순서도 바꿔줘야 한다.
		//----------------------------------------------------
		enum GEAR_SLAYER 
		{
			GEAR_SLAYER_HELM = 0,			// 모자
			GEAR_SLAYER_NECKLACE,			// 목걸이
			GEAR_SLAYER_COAT,				// 상의
			GEAR_SLAYER_LEFTHAND,			// 왼손
			GEAR_SLAYER_RIGHTHAND,			// 오른손
			GEAR_SLAYER_GLOVE,				// 장갑
			GEAR_SLAYER_BELT,				// 혁대
			GEAR_SLAYER_TROUSER,			// 하의			
			GEAR_SLAYER_BRACELET1,			// 팔찌1
			GEAR_SLAYER_BRACELET2,			// 팔찌2
			GEAR_SLAYER_RING1,				// 반지1
			GEAR_SLAYER_RING2,				// 반지2
			GEAR_SLAYER_RING3,				// 반지3
			GEAR_SLAYER_RING4,				// 반지4
			GEAR_SLAYER_SHOES,				// 신발
		
			GEAR_SLAYER_ZAP1,				// 잽1 - 링에 덛붙이는 아이템
			GEAR_SLAYER_ZAP2,				// 잽2
			GEAR_SLAYER_ZAP3,				// 잽3
			GEAR_SLAYER_ZAP4,				// 잽4

			GEAR_SLAYER_PDA,			// 승직용 허리장식
	        GEAR_SLAYER_SHOULDER,			// 승직용 장갑?

			GEAR_SLAYER_BLOOD_BIBLE1,				// 블러드 바이블 슬롯 1
			GEAR_SLAYER_BLOOD_BIBLE2,				// 블러드 바이블 슬롯 2
			GEAR_SLAYER_BLOOD_BIBLE3,				// 블러드 바이블 슬롯 3
			GEAR_SLAYER_BLOOD_BIBLE4,				// 블러드 바이블 슬롯 4
			GEAR_SLAYER_BLOOD_BIBLE5,				// 블러드 바이블 슬롯 5
			GEAR_SLAYER_BLOOD_BIBLE6,				// 블러드 바이블 슬롯 6

			MAX_GEAR_SLAYER
		};

		//----------------------------------------------------
		// 특정한 부위에 장착할 수 있는 Item인지 확인
		//----------------------------------------------------
		inline static bool		IsGearSlotHelm(MItem* pItem);
		inline static bool		IsGearSlotNecklace(MItem* pItem);
		inline static bool		IsGearSlotCoat(MItem* pItem);
		inline static bool		IsGearSlotLeftHand(MItem* pItem);
		inline static bool		IsGearSlotRightHand(MItem* pItem);
		inline static bool		IsGearSlotGlove(MItem* pItem);
		inline static bool		IsGearSlotBelt(MItem* pItem);
		inline static bool		IsGearSlotTrouser(MItem* pItem);
		inline static bool		IsGearSlotBracelet(MItem* pItem);
		inline static bool		IsGearSlotRing(MItem* pItem);
		inline static bool		IsGearSlotShoes(MItem* pItem);
		inline static bool		IsGearSlotCoreZap(MItem* pItem);
		inline static bool		IsGearSlotBloodBible(MItem* pItem);
		inline static bool		IsGearSlotPDA(MItem* pItem);
		inline static bool		IsGearSlotShoulder(MItem* pItem);
		


	public :
		MSlayerGear();
		~MSlayerGear();

		//------------------------------------------------
		// 깨끗하게~ 초기화 시켜 준다.
		//------------------------------------------------
		void			Init();

		//------------------------------------------------
		// AddItem : slot(n)에 pItem을 추가한다.
		//------------------------------------------------
		bool			AddItem(MItem* pItem);
		bool			AddItem(MItem* pItem, GEAR_SLAYER n);

		//------------------------------------------------
		// GetItem : slot(n)의 item을 읽어온다.
		//------------------------------------------------
		MItem*			GetItem(GEAR_SLAYER n) const { return MSlotItemManager::GetItem( (BYTE)n ); }
		
		//------------------------------------------------
		// RemoveItem : slot(n)의 item을 제거한다.
		//------------------------------------------------
		MItem*			RemoveItem(GEAR_SLAYER n);

		//------------------------------------------------
		// RemoveItem : id가 같은 item을 제거한다.
		//------------------------------------------------
		MItem*			RemoveItem(TYPE_OBJECTID id);

		//------------------------------------------------
		// ReplaceItem : pItem을 추가하고 딴게 있다면 Item교환
		//------------------------------------------------
		bool			ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);	

		//------------------------------------------------
		// Can ReplaceItem : (n) slot에 pItem을 추가하거나 
		//						원래 있던 Item과 교체가 가능한가?
		//------------------------------------------------
		bool			CanReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);

		//------------------------------------------------------		
		// Get FitSlot
		//------------------------------------------------------		
		bool			GetFitSlot(MItem* pItem, GEAR_SLAYER& n);	// pItem이 들어갈 수 있는 적절한 slot위치를 구한다.

		//------------------------------------------------------
		// 현재 사용가능한건가?
		//------------------------------------------------------
		void			CheckAffectStatus(MItem* pItem);	// 특정 아이템

		//------------------------------------------------
		// GetItem : 기어 창에서 pick 할때만 사용 slot(n)의 item을 읽어온다.
		//------------------------------------------------
		MItem*			GetGearItem_PickUp(int &n) const ;
		//------------------------------------------------
		// GetItem : 코어잽 아이템이 덛붙여져 있는지 검사
		//------------------------------------------------
		MItem*			GetGearCoreZapItem(int slot) const	;
		//------------------------------------------------
		// GetItem : 코어잽 아이템 밑의 아이템을 검사
		//------------------------------------------------
		MItem*			GetGearCoreZapedItem(int slot) const	;
		//------------------------------------------------
		// GetItem : 코어잽을 다 모았는지..
		//------------------------------------------------
		const bool			IsHasAllCoreZap(int CoreZapType) const	;

		//------------------------------------------------
		// bool : 블러드 기어창이 열렸는지.
		//------------------------------------------------
		void			SetBloodBibleOpenSlot(int Num) { m_bBloodBibleOpenCount = Num;	}
		BYTE			GetBloodBibleOpenSlot() { return m_bBloodBibleOpenCount;	}
		const bool		IsCloseBloodBibleSlot(int slot) const	;
	protected :
		//------------------------------------------------
		// IsGearSlot...()에 대한 function pointer
		//------------------------------------------------
		typedef bool (*FUNCTION_GEARSLOT_CHECK)(MItem*);

		//------------------------------------------------
		// 각 부위별로 IsGearSlot...()에 대한 function pointer
		//------------------------------------------------
		static FUNCTION_GEARSLOT_CHECK		s_GearSlotCheckTable[MAX_GEAR_SLAYER];

		BYTE		m_Gilles_CoreZap;
		BYTE		m_bBloodBibleOpenCount;
};


extern MSlayerGear*		g_pSlayerGear;

extern MMagazine*		g_pCurrentMagazine;

#endif

