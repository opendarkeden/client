//----------------------------------------------------------------------
// MOustersGear.h
//----------------------------------------------------------------------
// Ousters의 Gear에 대한 class
//----------------------------------------------------------------------

#ifndef	__MOUSTERSGEAR_H__
#define	__MOUSTERSGEAR_H__

#include "MPlayerGear.h"

class MOustersGear : public MPlayerGear	{
	public :
		//----------------------------------------------------
		// QuickItem Slot의 Slot ID
		//----------------------------------------------------
		// (!) ID를 바꾸면 s_GearSlotCheckTable의 순서도 바꿔줘야 한다.
		//----------------------------------------------------
		enum GEAR_OUSTERS
		{
			GEAR_OUSTERS_CIRCLET,			// 서클릿
			GEAR_OUSTERS_COAT,				// 옷
			GEAR_OUSTERS_LEFTHAND,			// 왼손
			GEAR_OUSTERS_RIGHTHAND,			// 오른손
			GEAR_OUSTERS_BOOTS,				// 신발
			GEAR_OUSTERS_ARMSBAND1,			// 암스밴드1
			GEAR_OUSTERS_ARMSBAND2,			// 암스밴드2
			GEAR_OUSTERS_RING1,				// 링1
			GEAR_OUSTERS_RING2,				// 링2
			GEAR_OUSTERS_PENDENT1,			// 목걸이1
			GEAR_OUSTERS_PENDENT2,			// 목걸이2
			GEAR_OUSTERS_PENDENT3,			// 목걸이3
			GEAR_OUSTERS_STONE1,			// 정령석1
			GEAR_OUSTERS_STONE2,			// 정령석2
			GEAR_OUSTERS_STONE3,			// 정령석3
			GEAR_OUSTERS_STONE4,			// 정령석4
			
			GEAR_OUSTERS_ZAP1,				// 잽1 - 정령석에 덛붙이는 아이템
			GEAR_OUSTERS_ZAP2,				// 잽2
			GEAR_OUSTERS_ZAP3,				// 잽3
			GEAR_OUSTERS_ZAP4,				// 잽4
			//by csm 2004 12.31 add item slot
			GEAR_OUSTERS_FASCIA,			// 승직용 허리장식
	        GEAR_OUSTERS_MITTEN,			// 승직용 장갑?

			GEAR_OUSTERS_BLOOD_BIBLE1,				// 블러드 바이블 슬롯 1 - 클라이언트 전용
			GEAR_OUSTERS_BLOOD_BIBLE2,				// 블러드 바이블 슬롯 2
			GEAR_OUSTERS_BLOOD_BIBLE3,				// 블러드 바이블 슬롯 3
			GEAR_OUSTERS_BLOOD_BIBLE4,				// 블러드 바이블 슬롯 4
			GEAR_OUSTERS_BLOOD_BIBLE5,				// 블러드 바이블 슬롯 5
			GEAR_OUSTERS_BLOOD_BIBLE6,				// 블러드 바이블 슬롯 6
			MAX_GEAR_OUSTERS
		};	

		//----------------------------------------------------
		// 특정한 부위에 장착할 수 있는 Item인지 확인
		//----------------------------------------------------
		inline static bool		IsGearSlotCirclet(MItem* pItem);
		inline static bool		IsGearSlotCoat(MItem* pItem);
		inline static bool		IsGearSlotLeftHand(MItem* pItem);
		inline static bool		IsGearSlotRightHand(MItem* pItem);
		inline static bool		IsGearSlotBoots(MItem* pItem);
		inline static bool		IsGearSlotArmsBand(MItem* pItem);
		inline static bool		IsGearSlotRing(MItem* pItem);
		inline static bool		IsGearSlotPendent(MItem* pItem);
		inline static bool		IsGearSlotStone(MItem* pItem);
		inline static bool		IsGearSlotCoreZap(MItem* pItem);
		inline static bool		IsGearSlotBloodBible(MItem* pItem);
		inline static bool		IsGearSlotFascia(MItem* pItem);
		inline static bool		IsGearSlotMitten(MItem* pItem);


	public :
		MOustersGear();
		~MOustersGear();

		//------------------------------------------------
		// 깨끗하게~ 초기화 시켜 준다.
		//------------------------------------------------
		void			Init();

		//------------------------------------------------
		// AddItem : slot(n)에 pItem을 추가한다.
		//------------------------------------------------
		bool			AddItem(MItem* pItem);
		bool			AddItem(MItem* pItem, GEAR_OUSTERS n);

		//------------------------------------------------
		// GetItem : slot(n)의 item을 읽어온다.
		//------------------------------------------------
		MItem*			GetItem(GEAR_OUSTERS n) const { return MSlotItemManager::GetItem( (BYTE)n ); }
		
		//------------------------------------------------
		// RemoveItem : slot(n)의 item을 제거한다.
		//------------------------------------------------
		MItem*			RemoveItem(GEAR_OUSTERS n);

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
		static FUNCTION_GEARSLOT_CHECK		s_GearSlotCheckTable[MAX_GEAR_OUSTERS];

		BYTE		m_Gilles_CoreZap;

		BYTE		m_bBloodBibleOpenCount;

};


extern MOustersGear	*	g_pOustersGear;

#endif

