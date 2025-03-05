//----------------------------------------------------------------------
// MVampireGear.h
//----------------------------------------------------------------------
// Vampire의 Gear에 대한 class
//----------------------------------------------------------------------

#ifndef	__MVAMPIREGEAR_H__
#define	__MVAMPIREGEAR_H__

#include "MPlayerGear.h"

class MVampireGear : public MPlayerGear	{
	public :
		//----------------------------------------------------
		// QuickItem Slot의 Slot ID
		//----------------------------------------------------
		// (!) ID를 바꾸면 s_GearSlotCheckTable의 순서도 바꿔줘야 한다.
		//----------------------------------------------------
		enum GEAR_VAMPIRE 
		{
			GEAR_VAMPIRE_NECKLACE,			// 목걸이
			GEAR_VAMPIRE_COAT,				// 상의
			GEAR_VAMPIRE_BRACELET1,			// 팔찌1
			GEAR_VAMPIRE_BRACELET2,			// 팔찌2
			GEAR_VAMPIRE_RING1,				// 반지1
			GEAR_VAMPIRE_RING2,				// 반지2
			GEAR_VAMPIRE_RING3,				// 반지3
			GEAR_VAMPIRE_RING4,				// 반지4
			GEAR_VAMPIRE_EARRING1,			// 귀걸이1
			GEAR_VAMPIRE_EARRING2,			// 귀걸이2
			GEAR_VAMPIRE_LEFTHAND,			// 왼손
			GEAR_VAMPIRE_RIGHTHAND,			// 오른손
			GEAR_VAMPIRE_AMULET1,			// 아뮬렛1
			GEAR_VAMPIRE_AMULET2,			// 아뮬렛2
			GEAR_VAMPIRE_AMULET3,			// 아뮬렛3
			GEAR_VAMPIRE_AMULET4,			// 아뮬렛4
			
			GEAR_VAMPIRE_ZAP1,				// 잽1 - 반지에 덛붙이는 아이템
			GEAR_VAMPIRE_ZAP2,				// 잽2
			GEAR_VAMPIRE_ZAP3,				// 잽3
			GEAR_VAMPIRE_ZAP4,				// 잽4

			GEAR_VAMPIRE_PERSONA,		// 승직용 페르소나
			GEAR_VAMPIRE_DERMIS,		// 승직용 문신?
			
 			
			GEAR_VAMPIRE_BLOOD_BIBLE1,				// 블러드 바이블 슬롯 1
			GEAR_VAMPIRE_BLOOD_BIBLE2,				// 블러드 바이블 슬롯 2
			GEAR_VAMPIRE_BLOOD_BIBLE3,				// 블러드 바이블 슬롯 3
			GEAR_VAMPIRE_BLOOD_BIBLE4,				// 블러드 바이블 슬롯 4
			GEAR_VAMPIRE_BLOOD_BIBLE5,				// 블러드 바이블 슬롯 5
			GEAR_VAMPIRE_BLOOD_BIBLE6,				// 블러드 바이블 슬롯 6
			
			MAX_GEAR_VAMPIRE
		};

		//----------------------------------------------------
		// 특정한 부위에 장착할 수 있는 Item인지 확인
		//----------------------------------------------------
		inline static bool		IsGearSlotNecklace(MItem* pItem);
		inline static bool		IsGearSlotCoat(MItem* pItem);
		inline static bool		IsGearSlotBracelet(MItem* pItem);
		inline static bool		IsGearSlotRing(MItem* pItem);
		inline static bool		IsGearSlotEarRing(MItem* pItem);
		inline static bool		IsGearSlotLeftHand(MItem* pItem);
		inline static bool		IsGearSlotRightHand(MItem* pItem);
		inline static bool		IsGearSlotAmulet(MItem* pItem);
		inline static bool		IsGearSlotCoreZap(MItem* pItem);
		inline static bool		IsGearSlotBloodBible(MItem* pItem);
		inline static bool		IsGearSlotDermis(MItem* pItem);
		inline static bool		IsGearSlotPersona(MItem* pItem);
		


	public :
		MVampireGear();
		~MVampireGear();

		//------------------------------------------------
		// 깨끗하게~ 초기화 시켜 준다.
		//------------------------------------------------
		void			Init();

		//------------------------------------------------
		// AddItem : slot(n)에 pItem을 추가한다.
		//------------------------------------------------
		bool			AddItem(MItem* pItem);
		bool			AddItem(MItem* pItem, GEAR_VAMPIRE n);

		//------------------------------------------------
		// GetItem : slot(n)의 item을 읽어온다.
		//------------------------------------------------
		MItem*			GetItem(GEAR_VAMPIRE n ) const	{ return MSlotItemManager::GetItem( (BYTE)n ); }
		
		//------------------------------------------------
		// RemoveItem : slot(n)의 item을 제거한다.
		//------------------------------------------------
		MItem*			RemoveItem(GEAR_VAMPIRE n);

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
		bool			CanReplaceItem(MItem* pItem, GEAR_VAMPIRE n, MItem*& pOldItem);

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
		static FUNCTION_GEARSLOT_CHECK		s_GearSlotCheckTable[MAX_GEAR_VAMPIRE];

		BYTE			m_Gilles_CoreZap;
		BYTE		m_bBloodBibleOpenCount;
};


extern MVampireGear	*	g_pVampireGear;

#endif

