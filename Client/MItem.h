//----------------------------------------------------------------------
// MItem.h
//----------------------------------------------------------------------
// 아이템..
//----------------------------------------------------------------------
/*

 [ class hierarchy ]


  ITEM	- BELT
			- MOTORCYCLE
			- CORPSE

		- PILE_ITEM
			- POTION			
			- MAGAZINE

		- GEAR_ITEM
			- RING
			- BRACELET
			- NECKLACE
			- SHOES
			- SWORD
			- BLADE
			- SHIELD
			- CROSS
			- GLOVE
			- HELM
			- GUN_SG
			- GUN_SMG
			- GUN_AR
			- GUN_TR

		- WATER
		- HOLYWATER
		- ETC					
		- MOTORCYCLE(-I)
		- KEY(+V)
		- BELT	
		- BOMB_MATERIAL
		- LEARNINGITEM
		- MONEY(+V)
		- BOMB
		- MINE

		- VAMPIRE_ITEM(+V)
			- VAMPIRE_GEAR_ITEM
				- VAMPIRE_EARRING
				- VAMPIRE_RING
				- VAMPIRE_BRACELET
				- VAMPIRE_NECKLACE
				- VAMPIRE_COAT
				- VAMPIRE_SHOES

*/

#ifndef	__MITEM_H__
#define	__MITEM_H__

#include <string>
#include "MObject.h"
#include "MITemTable.h"
#include "MGridItemManager.h"
#include "MSlotItemManager.h"
#include "ItemClassDef.h"
//#include "SkillDef.h"
//#include "AddonDef.h"
#include "CAnimationFrame.h"
#include "RaceType.h"
class MCreature;


#define	MAX_DROP_COUNT					6


class MItem : public MObject, public CAnimationFrame {
	public :
		//----------------------------------------------------------------------
		// Item이 장착되는 위치
		//----------------------------------------------------------------------
		enum ITEM_GEAR_SLOT
		{	
			SLOT_HELM = 0,			// 모자
			SLOT_NECKLACE,			// 목걸이
			SLOT_COAT,				// 상의
			SLOT_LEFTHAND,			// 왼손
			SLOT_RIGHTHAND,			// 오른손
			SLOT_TWOHAND,			// 양손
			SLOT_BELT,				// 혁대
			SLOT_BRACELET,			// 팔찌
			SLOT_TROUSER,			// 하의
			SLOT_RING,				// 반지
			SLOT_SHOES,				// 신발
			SLOT_GLOVE,				// 장갑
			SLOT_PDA,				// PDA
			SLOT_SHOULDER,			// 어깨

			// vampire 
			SLOT_VAMPIRE_RING,
			SLOT_VAMPIRE_BRACELET,
			SLOT_VAMPIRE_NECKLACE,
			SLOT_VAMPIRE_COAT,
			SLOT_VAMPIRE_EARRING,
			SLOT_VAMPIRE_LEFTHAND,
			SLOT_VAMPIRE_RIGHTHAND,
			SLOT_VAMPIRE_TWOHAND,			// 양손
			SLOT_VAMPIRE_AMULET,
			SLOT_VAMPIRE_DERMIS,			// 문신
			SLOT_VAMPIRE_PERSONA,			// 가면

			// ousters
			SLOT_OUSTERS_CIRCLET,			// 서클릿
			SLOT_OUSTERS_COAT,				// 옷
			SLOT_OUSTERS_LEFTHAND,			// 왼손
			SLOT_OUSTERS_RIGHTHAND,			// 오른손
			SLOT_OUSTERS_TWOHAND,			// 양손
			SLOT_OUSTERS_BOOTS,				// 신발
			SLOT_OUSTERS_ARMSBAND,			// 암스밴드
			SLOT_OUSTERS_RING,				// 링
			SLOT_OUSTERS_PENDENT,			// 목걸이
			SLOT_OUSTERS_STONE,			// 정령석
			SLOT_OUSTERS_FASCIA,			// 허리장식
			SLOT_OUSTERS_MITTEN,			// 장갑


			// 공통
			SLOT_BLOOD_BIBLE,			 // 블러드 바이블

			MAX_SLOT,				// 최대값

			SLOT_NULL				// 없는 경우
		};


	public :
		MItem();
		virtual ~MItem();

		//-------------------------------------------------------
		// Item의 class
		//-------------------------------------------------------
		virtual ITEM_CLASS	GetItemClass() const				{ return ITEM_CLASS_NULL; }

		//-------------------------------------------------------
		// ItemType
		//-------------------------------------------------------
		virtual void		SetItemType(TYPE_ITEMTYPE type)		{ m_ItemType = type; }
		TYPE_ITEMTYPE		GetItemType() const					{ return m_ItemType; }

		//-------------------------------------------------------
		// ItemStyle
		//-------------------------------------------------------
		int					GetItemStyle() const				{ return (*g_pItemTable)[GetItemClass()][m_ItemType].ItemStyle; }
		int					IsNormalItem() const				{ return ( (*g_pItemTable)[GetItemClass()][m_ItemType].ItemStyle == 0 ); }
		int					IsUniqueItem() const				{ return ( (*g_pItemTable)[GetItemClass()][m_ItemType].ItemStyle == 1 ); }
		int					IsQuestItem() const;
		bool				IsSpecialColorItem() const;

		int					GetSpecialColorItemColorset();
		static int			GetSpecialColorItemColorset(unsigned short srcColor);
		static int			GetUniqueItemColorset();
		static int			GetQuestItemColorset();
		int					GetRareItemColorset();

		//-------------------------------------------------------
		// ItemOption
		//-------------------------------------------------------
		int									GetItemDefaultOptionCount() const	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].DefaultOptionList.size(); }
		bool								IsEmptyItemDefaultOption() const	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].DefaultOptionList.empty(); }
		const std::list<TYPE_ITEM_OPTION>&	GetItemDefaultOptionList() const	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].DefaultOptionList; }		
		void								SetItemOptionList(const std::list<TYPE_ITEM_OPTION> &option_list)	{ m_ItemOptionList = option_list; }
		const std::list<TYPE_ITEM_OPTION>&	GetItemOptionList() const					{ return m_ItemOptionList; }		
		int									GetItemOptionListCount() const				{ return m_ItemOptionList.size(); }
		bool								IsEmptyItemOptionList() const				{ return m_ItemOptionList.empty(); }
		void								RemoveItemOption(TYPE_ITEM_OPTION option);
		void								AddItemOption(TYPE_ITEM_OPTION option);
		void								ChangeItemOption(TYPE_ITEM_OPTION ori_option, TYPE_ITEM_OPTION new_option);
		void								ClearItemOption()	{ m_ItemOptionList.clear(); }

		//-------------------------------------------------------
		// Slayer / Vampire / Ousters ?
		//-------------------------------------------------------
		int					IsSlayerItem() const			{ return ((*g_pItemTable)[GetItemClass()][m_ItemType].Race & FLAG_RACE_SLAYER); }
		int					IsVampireItem() const			{ return ((*g_pItemTable)[GetItemClass()][m_ItemType].Race & FLAG_RACE_VAMPIRE); }
		int					IsOustersItem() const			{ return ((*g_pItemTable)[GetItemClass()][m_ItemType].Race & FLAG_RACE_OUSTERS); }

		//-------------------------------------------------------
		// 기본 Item Color
		//-------------------------------------------------------
		void				SetItemColorSet(WORD cs)		{ m_ItemColorSet = cs; }		
		WORD				GetItemColorSet() const			{ return m_ItemColorSet; }		

		//-------------------------------------------------------
		// 속성
		//-------------------------------------------------------
		// Container로 사용할 수 있는 Item인가?
		virtual bool		IsContainerItem() const		{ return false; }		

		// 같은 Item끼리 쌓이는가?
		virtual bool		IsPileItem() const				{ return false; }

		// 쌓이는거랑은 다르다. 개수만 바뀐다.
		virtual bool		IsChargeItem() const			{ return false; }

		// 장착할 수 있는가?
		virtual bool		IsGearItem() const			{ return false; }

		// 착용해서 그림이 바뀌는 Item인가?
		virtual bool		IsAddonItem() const			{ return false; }

		// 기본 공격 무기인가?
		virtual bool		IsBasicWeapon() const		{ return false; }

		// 총인가?
		virtual bool		IsGunItem() const			{ return false; }

		// Inventory에 들어갈 수 있는 Item인가?
		virtual bool		IsInventoryItem() const		{ return true; }
		
		// QuickItem에 들어가는가?
		// QuickItem은 기본적으로 사용가능한것들만 넣기 때문에 QuickItem이면 무조건 사용 가능하다.
		virtual bool		IsQuickItem() const			{ return false; }

		// 사용 가능한 아이템인가?
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		virtual void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0)	{}
	#else
		virtual void		UseInventory()				{}
	#endif
		virtual void		UseQuickItem()				{}
		virtual void		UseGear()					{}


		// 다른 Item에 추가될 수 있는가?
		virtual bool		IsInsertToItem(const MItem* pItem) const;

		// 장착할 수 있는 곳은?
		virtual ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_NULL; }

		// 특정 부위에 장착할 수 있는가?
		virtual bool		IsGearSlotHelm()	const			{ return false; }	// 모자
		virtual bool		IsGearSlotNecklace() const		{ return false; }	// 목걸이
		virtual bool		IsGearSlotCoat() const			{ return false; }	// 상의
		virtual bool		IsGearSlotLeftHand() const		{ return false; }	// 왼손
		virtual bool		IsGearSlotRightHand() const		{ return false; }	// 오른손
		virtual bool		IsGearSlotTwoHand() const		{ return false; }	// 양손
		virtual bool		IsGearSlotBelt() const			{ return false; }	// 혁대
		virtual bool		IsGearSlotBracelet() const		{ return false; }	// 팔찌
		virtual bool		IsGearSlotTrouser() const		{ return false; }	// 하의
		virtual bool		IsGearSlotRing() const			{ return false; }	// 반지
		virtual bool		IsGearSlotShoes() const			{ return false; }	// 신발		
		virtual bool		IsGearSlotGlove() const			{ return false; }	// 장갑		
		virtual bool		IsGearSlotCoreZap() const		{ return false; }	// 코어잽	
		virtual bool		IsGearSlotBloodBible() const		{ return false; }	// 블러드 바이블
		virtual bool		IsGearSlotPDA() const			{ return false; }	// PDA 슬롯
		virtual bool		IsGearSlotShoulder() const		{ return false; }	// 어깨 슬롯

		// vampire
		virtual bool		IsGearSlotVampireEarRing() const	{ return false; }
		virtual bool		IsGearSlotVampireRing() const	{ return false; }
		virtual bool		IsGearSlotVampireBracelet() const	{ return false; }
		virtual bool		IsGearSlotVampireNecklace() const	{ return false; }
		virtual bool		IsGearSlotVampireCoat() const	{ return false; }
		virtual bool		IsGearSlotVampireLeftHand() const	{ return false; }
		virtual bool		IsGearSlotVampireRightHand() const	{ return false; }
		virtual bool		IsGearSlotVampireTwoHand() const	{ return false; }	// 양손
		virtual bool		IsGearSlotVampireAmulet() const	{ return false; }
		virtual bool		IsGearSlotVampireCoreZap() const		{ return false; }	// 코어잽	
		virtual bool		IsGearSlotVampireBloodBible() const		{ return false; }	// 블러드 바이블
		virtual bool		IsGearSlotVampireDermis() const		{ return false; }	// 문신
		virtual bool		IsGearSlotVampirePersona() const		{ return false; }	// 페르소나

		// ousters
		virtual bool		IsGearSlotOustersCirclet() const		{ return false; }
		virtual bool		IsGearSlotOustersCoat() const			{ return false; }
		virtual bool		IsGearSlotOustersLeftHand() const		{ return false; }
		virtual bool		IsGearSlotOustersRightHand() const		{ return false; }
		virtual bool		IsGearSlotOustersTwoHand() const		{ return false; }	// 양손
		virtual bool		IsGearSlotOustersBoots() const			{ return false; }
		virtual bool		IsGearSlotOustersArmsBand() const		{ return false; }
		virtual bool		IsGearSlotOustersRing() const			{ return false; }
		virtual bool		IsGearSlotOustersPendent() const		{ return false; }
		virtual bool		IsGearSlotOustersStone() const			{ return false; }
		virtual bool		IsGearSlotOustersCoreZap() const		{ return false; }	// 코어잽	
		virtual bool		IsGearSlotOustersBloodBible() const		{ return false; }	// 블러드 바이블
		virtual bool		IsGearSlotOustersFascia() const		{ return false; }	// 허리장식
		virtual bool		IsGearSlotOustersMitten() const		{ return false; }	// 장갑

		// 2005, 1, 3, sobeit add start
		virtual bool		IsDurationAlwaysOkay() const		{ return false; }	// 내구 체크 안하는 아이템
		// 2005, 1, 3, sobeit add end
		
		//-------------------------------------------------------
		// 아이템 이름 : 한글 / 영어?
		//-------------------------------------------------------
		static bool			IsUseKorean()	{ return s_bUseKorean; }
		static bool			IsUseEnglish()	{ return !s_bUseKorean; }
		static void			UseKorean()		{ s_bUseKorean = true; }
		static void			UseEnglish()	{ s_bUseKorean = false; }

		//---------------------------------------------------
		// class마다 고정된 정보
		//---------------------------------------------------
		const char*				GetName();
		const char*				GetEName() const;
		const char*				GetDescription() const;
		TYPE_ITEM_WEIGHT		GetWeight() const;
		TYPE_ITEM_PRICE			GetPrice() const;
		BYTE					GetGridWidth() const;
		BYTE					GetGridHeight() const;

		// FrameID
		TYPE_FRAMEID			GetTileFrameID() const;			// Tile에서의 FrameID
		TYPE_FRAMEID			GetInventoryFrameID() const;	// Inventory에서의 Frame ID
		TYPE_FRAMEID			GetGearFrameID() const;			// Gear에서의 Frame ID
		TYPE_FRAMEID			GetAddonMaleFrameID() const;
		TYPE_FRAMEID			GetAddonFemaleFrameID() const;
		TYPE_FRAMEID			GetDropFrameID() const;			// 바닥에 떨어지는 Item

		// Sound ID
		TYPE_SOUNDID			GetUseSoundID() const;			// Item 사용 SoundID			
		TYPE_SOUNDID			GetTileSoundID() const;			// Item 줍기 SoundID
		TYPE_SOUNDID			GetInventorySoundID() const;	// Inventory에서의 Sound
		TYPE_SOUNDID			GetGearSoundID() const;			// Gear에서의 Sound	

		// addon slot
		virtual ADDON			GetAddonSlot() const	{ return ADDON_NULL; }

		//---------------------------------------------------
		// 성별
		//---------------------------------------------------
		bool					IsGenderForMale() const;
		bool					IsGenderForFemale() const;
		bool					IsGenderForAll() const;

		//---------------------------------------------------
		// 필요능력
		//---------------------------------------------------
		BYTE					GetRequireSTR()	const;
		BYTE					GetRequireDEX()	const;
		BYTE					GetRequireINT()	const;
		BYTE					GetRequireLevel() const;		
		WORD					GetRequireSUM() const;		

		//---------------------------------------------------
		// 장착했을 때 바뀌는.. BasicActionInfo
		//---------------------------------------------------
		TYPE_ACTIONINFO			GetUseActionInfo() const;

		//---------------------------------------------------
		// Option에 따른 정보
		//---------------------------------------------------
		BYTE				GetItemOptionPart(int OptionNum = 0) const;
		const char*			GetItemOptionName(int OptionNum = 0) const;
		const char*			GetItemOptionEName(int OptionNum = 0) const;
		BYTE				GetItemOptionPlusPoint(int OptionNum = 0) const;
//		BYTE				GetItemOptionPlusRequireAbility() const;
		DWORD				GetItemOptionPriceMultiplier() const;
		WORD				GetItemOptionColorSet(int OptionNum = 0);
		int					GetItemOptionRequireSTR()	const;
		int					GetItemOptionRequireDEX()	const;
		int					GetItemOptionRequireINT()	const;
		int					GetItemOptionRequireLevel() const;		
		int					GetItemOptionRequireSUM() const;

		//---------------------------------------------------
		// 세부 정보
		//---------------------------------------------------
		// Inventory에서의 좌표
		void					SetGridXY(BYTE x, BYTE y)	{ m_GridX=x; m_GridY=y; }
		BYTE					GetGridX() const			{ return m_GridX; }
		BYTE					GetGridY() const			{ return m_GridY; }		

		// 어떤(?) Slot에서의 좌표 (m_GridX를 이용한다. 관계없다. - -;;)
		void					SetItemSlot(BYTE n)		{ m_GridX = n; }
		BYTE					GetItemSlot() const		{ return m_GridX; }
		
		// 내구성(현재)
		TYPE_ITEM_DURATION		GetCurrentDurability() const				{ return m_CurrentDurability; }
		virtual void			SetCurrentDurability(TYPE_ITEM_DURATION d)	{ m_CurrentDurability = d; }

		// 개수( 돈, 쌓인 개수)
		virtual TYPE_ITEM_NUMBER	GetNumber() const					{ return m_Number; }
		virtual void				SetNumber(TYPE_ITEM_NUMBER n)		{ m_Number = n; }

		// 최대 개수
		virtual TYPE_ITEM_NUMBER	GetMaxNumber() const				{ return 1; }

		
		//---------------------------------------------------
		// Value들..
		//---------------------------------------------------		
		virtual int		GetMaxDurability() const	{ return -1; }	// 내구성
		virtual int		GetCarryingCapacity() const	{ return -1; }	// 운반 가능 무게
		virtual int		GetProtectionValue() const	{ return -1; }	// 방어력
		virtual int		GetDefenseValue() const		{ return -1; }	// 방어력
		virtual int		GetPocketNumber() const		{ return -1; }	// 포켓수
		virtual int		GetMinDamage() const		{ return -1; }	// 최소 공격력
		virtual int		GetMaxDamage() const		{ return -1; }	// 최대 공격력
		virtual int		GetHealPoint() const		{ return -1; }	// HP수치
		virtual int		GetManaPoint() const		{ return -1; }	// MP수치
		virtual ITEM_CLASS	GetGunClass() const		{ return ITEM_CLASS_NULL; }	// 총의 종류
		virtual int		GetMagazineSize() const		{ return -1; }	// 탄창 수
		virtual int		GetReach() const			{ return  1; }	// 사정거리
		virtual int		GetToHit() const			{ return -1; }	// ToHit
		virtual int		GetCriticalHit() const		{ return -1; }	// 크리티컬힛 확률
		virtual int		GetLucky() const			{ return -9999; }	// 럭키!

		//---------------------------------------------------
		// Item떨어뜨리기..
		//---------------------------------------------------
		void		SetDropping();
		BOOL		IsDropping() const		{ return m_bDropping; }
		int			GetDropHeight() const	{ return s_DropHeight[m_DropCount]; }
		void		NextDropFrame();		

		//---------------------------------------------------
		// Identified
		//---------------------------------------------------
		BOOL		IsIdentified() const	{ return m_bIdentified; }
		void		SetIdentified()			{ m_bIdentified = TRUE; }
		void		UnSetIdentified()		{ m_bIdentified = FALSE; }

		//-----------------------------------------------
		// AffectStatus - 수치 적용되는가?
		//-----------------------------------------------
		bool	IsAffectStatus() const		{ return m_bAffectStatus; }
		void	SetAffectStatus() 			{ m_bAffectStatus = true; }
		void	UnSetAffectStatus()			{ m_bAffectStatus = false; }

		//---------------------------------------------------
		// Trade - 교환할려고 선택한 Item인가?
		//---------------------------------------------------
		BOOL		IsTrade() const			{ return m_bTrade; }
		void		SetTrade()				{ m_bTrade = TRUE; }
		void		UnSetTrade()			{ m_bTrade = FALSE; }

		//---------------------------------------------------
		// Silver
		//---------------------------------------------------
		int			GetSilverMax() const	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].SilverMax; }	// Silver coating최대값
		int			GetSilver() const		{ return m_Silver; }
		virtual void	SetSilver(int s)		{ m_Silver = s; }

		//---------------------------------------------------
		// Speed
		//---------------------------------------------------
		int			GetOriginalSpeed() const;
		int			GetSpeed() const;
		void		SetSpeed(WORD s)		{ m_Speed = s; }

		//---------------------------------------------------
		// Grade
		//---------------------------------------------------
		int		GetGrade() const		{ return m_Grade; }
		virtual void	SetGrade(int s)		{ m_Grade = s; }

		//---------------------------------------------------
		// EnchantLevel
		//---------------------------------------------------
		WORD		GetEnchantLevel() const		{ return m_EnchantLevel; }
		virtual void	SetEnchantLevel(WORD s)		{ m_EnchantLevel = s; }

		//-------------------------------------------------------
		// 이름 
		//-------------------------------------------------------
		void		SetName(const char* pName);

		//-------------------------------------------------------
		// Quest
		//-------------------------------------------------------
		void		SetQuestFlag(bool isQuest = true) { m_Quest = isQuest; }

		void					SetPersnalPrice(TYPE_ITEM_PRICE price) { m_persnal_price = price;}
		TYPE_ITEM_PRICE			GetPersnalPrice() { return m_persnal_price; }

		void			SetPersnal(bool  persnal) { m_persnal = persnal;}
		bool		GetPersnal() { return m_persnal; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		//void	SaveToFile(ofstream& file);
		//void	LoadFromFile(ifstream& file);

	public :		
		// NewItem()에 대한 functions pointer
		typedef MItem* (*FUNCTION_NEWITEMCLASS)();

		//-------------------------------------------------------
		// 원하는 class의 Item의 객체를 생성한다.
		//-------------------------------------------------------
		static MItem*		NewItem(ITEM_CLASS itemClass);

		// 각 class별로 NewItem()에 대한 function pointer
		static FUNCTION_NEWITEMCLASS		s_NewItemClassTable[MAX_ITEM_CLASS];

	
	//-------------------------------------------------------
	//
	//			하나의 Item instance에 대한 정보
	//
	//-------------------------------------------------------
	protected :				
		// Item의 종류에 대한 ID
		TYPE_ITEMTYPE			m_ItemType;	

		// Item의 option
		std::list<TYPE_ITEM_OPTION>		m_ItemOptionList;

		// 기본 Color
		WORD					m_ItemColorSet;
		
		// Inventory에서의 좌표
		BYTE					m_GridX;
		BYTE					m_GridY;

		// 세부 정보
		TYPE_ITEM_DURATION		m_CurrentDurability;		

		// 개수 (돈, 쌓인 개수)
		TYPE_ITEM_NUMBER		m_Number;

		//---------------------------------------------------
		// drop
		//---------------------------------------------------
		BOOL					m_bDropping;					// 떨어지고 있는 중
		int						m_DropCount;					// 현재 count
		static int				s_DropHeight[MAX_DROP_COUNT];	// Drop 높이

		//---------------------------------------------------
		// identified
		//---------------------------------------------------
		BOOL					m_bIdentified;

		//---------------------------------------------------
		// item입은 상태가 모두 제대로 적용됐는가?
		//---------------------------------------------------
		bool					m_bAffectStatus;	

		//---------------------------------------------------
		// 교환할 item인가?
		//---------------------------------------------------
		BOOL					m_bTrade;

		//---------------------------------------------------
		// Silver
		//---------------------------------------------------
		int						m_Silver;	
		
		//---------------------------------------------------
		// Speed
		//---------------------------------------------------
		int						m_Speed;

		//---------------------------------------------------
		// Grade
		//---------------------------------------------------
		int						m_Grade;		
		
		//---------------------------------------------------
		// Enchant Level
		//---------------------------------------------------
		int						m_EnchantLevel;		
		
		//---------------------------------------------------
		// item 이름
		//---------------------------------------------------
		char*					m_pName;

		// Item이름에 한글을 사용하는가?
		static bool				s_bUseKorean;
		
		//---------------------------------------------------
		// 아이템이 퀘스트 아이템인가? (Time Manager에 등록되지 않았을경우..
		//---------------------------------------------------
		bool					m_Quest;
		bool					m_persnal;
		//by thai00
		TYPE_ITEM_PRICE			m_persnal_price;

};

//----------------------------------------------------------------------
//
//          Gear에 사용하는 Item인 경우.. 착용할 수 있다.
//
//----------------------------------------------------------------------
class MGearItem : public MItem {
	public :
		MGearItem() {}
		virtual ~MGearItem() {}

	public :
		bool			IsGearItem() const			{ return true; }

		// 값
		virtual int	GetMaxDurability() const;//	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value1; }	// 내구성
		virtual int	GetProtectionValue() const	{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value2; }	// 방어력
		virtual int	GetDefenseValue() const		{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value6; }	// def
};

//----------------------------------------------------------------------
//
//               Pile(쌓이는) Item인 경우..
//
//----------------------------------------------------------------------
class MPileItem : public MItem {
	public :
		MPileItem() { m_Number = 1; }
		virtual ~MPileItem() {}

		TYPE_ITEM_NUMBER	GetNumber() const					{ return m_Number; }
		void				SetNumber(TYPE_ITEM_NUMBER n)		{ m_Number = n; }

		bool			IsPileItem() const			{ return true; }		
};

//-------------------------------------------------------
// MUsePotionItem - 포션 아이템은 기본적으로 같은 루틴이라 검사 조건을 제외하고는 상속받아서 으쌰으쌰
//-------------------------------------------------------
class MUsePotionItem : public MPileItem
{
public:
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		virtual void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		virtual void	UseInventory();
	#endif
	
};

//----------------------------------------------------------------------
// MArmorItem
//----------------------------------------------------------------------
class MArmorItem : public MGearItem
{
	public :
		MArmorItem() {}
		~MArmorItem() {}

		int		GetProtectionValue() const;
		int		GetDefenseValue() const;
};
//----------------------------------------------------------------------
// MArmorItem2
//----------------------------------------------------------------------
class MArmorItem2 : public MGearItem
{
	public :
		MArmorItem2() {}
		~MArmorItem2() {}

		int		GetProtectionValue() const;
		int		GetDefenseValue() const;
		int		GetMaxDurability() const;
};
//----------------------------------------------------------------------
// Accessory Item
//----------------------------------------------------------------------
class MAccessoryItem : public MGearItem
{
	public :
		MAccessoryItem() {}
		~MAccessoryItem() {}

		int		GetLucky() const;
};
//----------------------------------------------------------------------
// BELT
//----------------------------------------------------------------------
class MBelt : public MArmorItem2, public MSlotItemManager {
	public :
		MBelt() {}
		~MBelt() {}

	public :
		//------------------------------------------------
		//
		//					MGearItem
		//
		//------------------------------------------------
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BELT; }

		//------------------------------------------------
		// ItemType설정
		//------------------------------------------------
		void				SetItemType(TYPE_ITEMTYPE type);

		ITEM_GEAR_SLOT	GetGearSlot() const		{ return SLOT_BELT; }
		bool			IsGearSlotBelt() const		{ return true; }

		bool			IsContainerItem() const		{ return true; }

		//------------------------------------------------
		//
		//				SlotItemManager
		//
		//------------------------------------------------
		// AddItem : slot(n)에 pItem을 추가한다.
		//------------------------------------------------
		bool			AddItem(MItem* pItem);
		bool			AddItem(MItem* pItem, BYTE n);
		
		//------------------------------------------------
		// ReplaceItem : pItem을 추가하고 딴게 있다면 Item교환
		//------------------------------------------------
		bool			ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);	

		//------------------------------------------------
		// Can ReplaceItem : n slot에 pItem을 추가가능한가? 
		// pOldItem에는 이전에 있던 것..
		//------------------------------------------------
		bool			CanReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);

		//------------------------------------------------
		// Find Slot To Add Item
		//------------------------------------------------
		bool			FindSlotToAddItem(MItem* pItem, int &slot) const;

		// 값
		virtual int	GetPocketNumber() const		{ return (*g_pItemTable)[ITEM_CLASS_BELT][m_ItemType].Value3; }	// 포켓수		

		static MItem*	NewItem()				{ return new MBelt; }
};

//----------------------------------------------------------------------
// CORPSE
//----------------------------------------------------------------------
class MCorpse : public MItem, public MGridItemManager  {
	public :
		MCorpse();
		~MCorpse();

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_CORPSE; }

		bool			IsInventoryItem() const		{ return false; }

		bool			IsContainerItem() const		{ return true; }

		static MItem*	NewItem()				{ return new MCorpse; }

		// 캐릭터 관련
		//----------------------------------------------------------
		// 탄창
		//----------------------------------------------------------
		MCreature*		GetCreature() const			{ return m_pCreature; }	
		MCreature*		SetCreature(MCreature* pCreature)	{ MCreature* pTemp=m_pCreature; m_pCreature=pCreature; return pTemp; }

	public :
		// 시체의 원래 캐릭터
		MCreature*		m_pCreature;
};

//----------------------------------------------------------------------
// POTION
//----------------------------------------------------------------------
class MPotion : public MUsePotionItem {
	public :
		MPotion() {}
		~MPotion() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_POTION; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// quick item
		bool		IsQuickItem() const			{ return true; }

		// heal 되는 수치
		int		GetHealPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_POTION][m_ItemType].Value1; }	// HP수치
		
		// mana 되는 수치
		int		GetManaPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_POTION][m_ItemType].Value2; }	// MP수치

		// use item
		void	UseQuickItem();
		
		static MItem*	NewItem()	{ return new MPotion; }
};

//----------------------------------------------------------------------
// WATER
//----------------------------------------------------------------------
class MWater : public MPileItem {
	public :
		MWater() {}
		~MWater() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_WATER; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// 사용 가능한 아이템이다.
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MWater; }
};

//----------------------------------------------------------------------
// HOLYWATER
//----------------------------------------------------------------------
class MHolyWater : public MPileItem {
	public :
		MHolyWater() {}
		~MHolyWater() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_HOLYWATER; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// damage
		virtual int	GetMinDamage() const	{ return (*g_pItemTable)[ITEM_CLASS_HOLYWATER][m_ItemType].Value1; }	// 최소 공격력
		virtual int	GetMaxDamage() const	{ return (*g_pItemTable)[ITEM_CLASS_HOLYWATER][m_ItemType].Value2; }	// 최대 공격력	

		static MItem*	NewItem()	{ return new MHolyWater; }
};

//----------------------------------------------------------------------
// MAGAZINE
//----------------------------------------------------------------------
class MMagazine : public MPileItem {
	public :
		MMagazine() {}
		~MMagazine() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_MAGAZINE; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// quick item
		bool		IsQuickItem() const			{ return true; }

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		void		UseQuickItem();

		// 다른 Item에 추가될 수 있는가?
		bool		IsInsertToItem(const MItem* pItem) const;
		
		// 탄창 수
		ITEM_CLASS	GetGunClass() const		{ return (ITEM_CLASS)(*g_pItemTable)[ITEM_CLASS_MAGAZINE][m_ItemType].Value1; }	// 총의 종류
		virtual int	GetMagazineSize() const	{ return (*g_pItemTable)[ITEM_CLASS_MAGAZINE][m_ItemType].Value2; }	// 탄창수

		static MItem*	NewItem()	{ return new MMagazine; }
};

//----------------------------------------------------------------------
// Ring
//----------------------------------------------------------------------
class MRing : public MAccessoryItem {
	public :
		MRing() {}
		~MRing() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_RING; }

		ITEM_GEAR_SLOT	GetGearSlot() const		{ return SLOT_RING; }
		bool		IsGearSlotRing() const		{ return true; }

		static MItem*	NewItem()	{ return new MRing; }
};

//----------------------------------------------------------------------
// Bracelet
//----------------------------------------------------------------------
class MBracelet : public MAccessoryItem {
	public :
		MBracelet() {}
		~MBracelet() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BRACELET; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_BRACELET; }
		bool		IsGearSlotBracelet() const			{ return true; }

		static MItem*	NewItem()	{ return new MBracelet; }
};
					

//----------------------------------------------------------------------
// NECKLACE
//----------------------------------------------------------------------
class MNecklace : public MAccessoryItem {
	public :
		MNecklace() {}
		~MNecklace() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_NECKLACE; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_NECKLACE; }
		bool		IsGearSlotNecklace() const			{ return true; }

		static MItem*	NewItem()	{ return new MNecklace; }
};

//----------------------------------------------------------------------
// COAT
//----------------------------------------------------------------------
class MCoat : public MArmorItem {
	public :
		MCoat() {}
		~MCoat() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_COAT; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_COAT; }
		bool		IsGearSlotCoat() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_COAT; }

		static MItem*	NewItem()	{ return new MCoat; }
};

//----------------------------------------------------------------------
// Trouser
//----------------------------------------------------------------------
class MTrouser : public MArmorItem {
	public :
		MTrouser() {}
		~MTrouser() {}

	public :
		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_TROUSER; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_TROUSER; }
		bool		IsGearSlotTrouser() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_TROUSER; }

		static MItem*	NewItem()	{ return new MTrouser; }
};
	
//----------------------------------------------------------------------
// SHOES
//----------------------------------------------------------------------
class MShoes : public MArmorItem2 {
	public :
		MShoes() {}
		~MShoes() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SHOES; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_SHOES; }
		bool		IsGearSlotShoes() const			{ return true; }

		static MItem*	NewItem()	{ return new MShoes; }
};

//----------------------------------------------------------------------
// BasicWeapon
//----------------------------------------------------------------------
class MWeaponItem : public MGearItem
{
	public :
		MWeaponItem() {}
		~MWeaponItem() {}

		bool		IsBasicWeapon() const		{ return true; }
		
		int			GetMinDamage() const;//		{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value3; }	// 최소 공격력
		int			GetMaxDamage() const;//		{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value4; }	// 최대 공격력		
		int			GetToHit() const;//			{ return (*g_pItemTable)[GetItemClass()][m_ItemType].ToHit; }		// 최대 공격력		
		int			GetCriticalHit() const;//		{ return (*g_pItemTable)[GetItemClass()][m_ItemType].CriticalHit; }

};

//----------------------------------------------------------------------
// SWORD
//----------------------------------------------------------------------
class MSword : public MWeaponItem {
	public :
		MSword() {}
		~MSword() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SWORD; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_RIGHTHAND; }
		bool		IsGearSlotRightHand() const		{ return true; }

		bool		IsGearSlotSword() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }

		static MItem*	NewItem()	{ return new MSword; }
};

//----------------------------------------------------------------------
// BLADE
//----------------------------------------------------------------------
class MBlade : public MWeaponItem {
	public :
		MBlade() {}
		~MBlade() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BLADE; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		bool		IsGearSlotBlade() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }

		static MItem*	NewItem()	{ return new MBlade; }
};

//----------------------------------------------------------------------
// Shield
//----------------------------------------------------------------------
class MShield : public MArmorItem2 {
	public :
		MShield() {}
		~MShield() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SHIELD; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_LEFTHAND; }
		bool		IsGearSlotLeftHand() const		{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_LEFTHAND; }
		
		static MItem*	NewItem()	{ return new MShield; }
};

//----------------------------------------------------------------------
// 
//----CROSS------------------------------------------------------------------
class MCross : public MWeaponItem {
	public :
		MCross() {}
		~MCross() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_CROSS; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		bool		IsGearSlotCross() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }

		// 값
		virtual int	GetManaPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_CROSS][m_ItemType].Value5; }	// MP수치
		
		static MItem*	NewItem()	{ return new MCross; }
};

//----------------------------------------------------------------------
// Mace
//----------------------------------------------------------------------
class MMace : public MWeaponItem {
	public :
		MMace() {}
		~MMace() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_MACE; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		bool		IsGearSlotMace() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }

		// 값		
		virtual int	GetManaPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_MACE][m_ItemType].Value5; }	// MP수치
		
		static MItem*	NewItem()	{ return new MMace; }
};

//----------------------------------------------------------------------
// GLOVE
//----------------------------------------------------------------------
class MGlove : public MArmorItem2 {
	public :
		MGlove() {}
		~MGlove() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_GLOVE; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_GLOVE; }
		bool		IsGearSlotGlove() const			{ return true; }

		static MItem*	NewItem()	{ return new MGlove; }
};

//----------------------------------------------------------------------
// HELM
//----------------------------------------------------------------------
class MHelm : public MArmorItem2 {
	public :
		MHelm() {}
		~MHelm() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_HELM; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_HELM; }
		bool		IsGearSlotHelm() const			{ return true; }

		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_HELM; }

		static MItem*	NewItem()	{ return new MHelm; }
};

//----------------------------------------------------------------------
//
// Gun Item
//
//----------------------------------------------------------------------
class MGunItem : public MWeaponItem {
	public :
		MGunItem() { m_pMagazine=NULL; }
		~MGunItem() { if (m_pMagazine!=NULL) delete m_pMagazine; }

	public :
		bool			IsGunItem() const				{ return true; }	// 총인가?

		bool			IsAddonItem() const				{ return true; }
		ADDON			GetAddonSlot() const			{ return ADDON_RIGHTHAND; }

		// 값		
		virtual int	GetReach() const			{ return (*g_pItemTable)[GetItemClass()][m_ItemType].Value5; }	// MP수치
		virtual int	GetMagazineSize() const		{ return (m_pMagazine==NULL)?0:m_pMagazine->GetNumber(); }	// 탄창 수
		
		//----------------------------------------------------------
		// 탄창
		//----------------------------------------------------------
		MMagazine*		GetMagazine() const					{ return m_pMagazine; }
		MMagazine*		RemoveMagazine()					{ MMagazine* pTemp=m_pMagazine; m_pMagazine=NULL; return pTemp; }
		MMagazine*		SetMagazine(MMagazine* pMagazine)	{ MMagazine* pTemp=m_pMagazine; m_pMagazine=pMagazine; return pTemp; }
		void			SetMagazineNULL()					{ m_pMagazine = NULL; }

	protected :
		MMagazine*		m_pMagazine;
};

//----------------------------------------------------------------------
// SG
//----------------------------------------------------------------------
class MGunSG : public MGunItem {
	public :
		MGunSG() {}
		~MGunSG() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SG; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		static MItem*	NewItem()	{ return new MGunSG; }
};

//----------------------------------------------------------------------
// SMG
//----------------------------------------------------------------------
class MGunSMG : public MGunItem {
	public :
		MGunSMG() {}
		~MGunSMG() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SMG; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		static MItem*	NewItem()	{ return new MGunSMG; }
};

//----------------------------------------------------------------------
// AR
//----------------------------------------------------------------------
class MGunAR : public MGunItem {
	public :
		MGunAR() {}
		~MGunAR() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_AR; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		static MItem*	NewItem()	{ return new MGunAR; }
};
		
//----------------------------------------------------------------------
// TR
//----------------------------------------------------------------------
class MGunTR : public MGunItem {
	public :
		MGunTR() {}
		~MGunTR() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SR; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_TWOHAND; }
		bool		IsGearSlotTwoHand() const			{ return true; }

		static MItem*	NewItem()	{ return new MGunTR; }
};			
				
//----------------------------------------------------------------------
// ETC
//----------------------------------------------------------------------
class MItemETC : public MPileItem {
	public :
		MItemETC() {}
		~MItemETC() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_ETC; }

		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MItemETC; }
};

//----------------------------------------------------------------------
// MOTORCYCLE
//----------------------------------------------------------------------
class MMotorcycle : public MItem, public MGridItemManager  {
	public :
		MMotorcycle() {}
		~MMotorcycle() {}

	public :

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_MOTORCYCLE; }

		bool			IsInventoryItem() const		{ return false; }

		bool			IsContainerItem() const		{ return true; }

		bool			IsAddonItem() const				{ return true; }
		ADDON			GetAddonSlot() const			{ return ADDON_MOTOR; }
		// 값
		virtual int	GetMaxDurability() const;//	{ return (*g_pItemTable)[ITEM_CLASS_MOTORCYCLE][m_ItemType].Value1; }	// 내구성
		virtual int	GetCarryingCapacity() const	{ return (*g_pItemTable)[ITEM_CLASS_MOTORCYCLE][m_ItemType].Value2; }	// 운반가능무게
		
		static MItem*	NewItem()	{ return new MMotorcycle; }
};

//----------------------------------------------------------------------
// KEY
//----------------------------------------------------------------------
class MKey : public MItem {
	public :
		MKey() {}
		~MKey() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_KEY; }

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		void		UseQuickItem();
		bool		IsQuickItem() const { return true; }

		static MItem*	NewItem()	{ return new MKey; }
};

//----------------------------------------------------------------------
// Relic
//----------------------------------------------------------------------
class MRelic : public MItem {
	public :
		MRelic() {}
		~MRelic() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_RELIC; }

		static MItem*	NewItem()	{ return new MRelic; }
};

//----------------------------------------------------------------------
// BOMB_MATERIAL
//----------------------------------------------------------------------
class MBombMaterial : public MPileItem {
	public :
		MBombMaterial() {}
		~MBombMaterial() {}

	public :
		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BOMB_MATERIAL; }

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MBombMaterial; }
};

//----------------------------------------------------------------------
// LEARNINGITEM
//----------------------------------------------------------------------
class MLearningItem : public MItem {
	public :
		MLearningItem() {}
		~MLearningItem() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_LEARNINGITEM; }

		static MItem*	NewItem()	{ return new MLearningItem; }
};

//----------------------------------------------------------------------
// MONEY
//----------------------------------------------------------------------
class MMoney : public MPileItem {
	public :
		MMoney() { m_Number = 1; }
		~MMoney() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_MONEY; }
		
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		static MItem*	NewItem()	{ return new MMoney; }

		//yckou
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

};

//----------------------------------------------------------------------
// BOMB
//----------------------------------------------------------------------
class MBomb : public MPileItem {
	public :
		MBomb() {}
		~MBomb() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BOMB; }

		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// 값
		int		GetMinDamage() const		{ return (*g_pItemTable)[ITEM_CLASS_BOMB][m_ItemType].Value1; }	// 최소 공격력
		int		GetMaxDamage() const		{ return (*g_pItemTable)[ITEM_CLASS_BOMB][m_ItemType].Value2; }	// 최대 공격력

		static MItem*	NewItem()	{ return new MBomb; }
};

//----------------------------------------------------------------------
// MINE
//----------------------------------------------------------------------
class MMine : public MPileItem {
	public :
		MMine() { m_bInstalled = false; }
		~MMine() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_MINE; }

		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// 값
		int		GetMinDamage() const		{ return (*g_pItemTable)[ITEM_CLASS_MINE][m_ItemType].Value1; }	// 최소 공격력
		int		GetMaxDamage() const		{ return (*g_pItemTable)[ITEM_CLASS_MINE][m_ItemType].Value2; }	// 최대 공격력

		// 설치되었나?
		bool	IsInstalled() const			{ return m_bInstalled; }
		void	SetInstalled() 				{ m_bInstalled = true; }
		void	UnSetInstalled() 			{ m_bInstalled = false; }

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()			{ return new MMine; }

	protected :
		bool				m_bInstalled;
};

//----------------------------------------------------------------------
// Skull
//-----------------------------------------------------------------------
/*
class MSkull : public MItem {
	public :
		MSkull() {}
		~MSkull() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SKULL; }
		
		bool		IsVampireItem() const		{ return true; }

		static MItem*	NewItem()	{ return new MSkull; }
};
*/

//----------------------------------------------------------------------
// VAMPIRE_RING
//----------------------------------------------------------------------
class MVampireRing : public MAccessoryItem {
	public :
		MVampireRing() {}
		~MVampireRing() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_RING; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_RING; }
		bool		IsGearSlotVampireRing() const			{ return true; }

		static MItem*	NewItem()	{ return new MVampireRing; }
};

//----------------------------------------------------------------------
// VAMPIRE_EARRING
//----------------------------------------------------------------------
class MVampireEarRing : public MAccessoryItem {
	public :
		MVampireEarRing() {}
		~MVampireEarRing() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_EARRING; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_EARRING; }
		bool		IsGearSlotVampireEarRing() const			{ return true; }

		static MItem*	NewItem()	{ return new MVampireEarRing; }
};

//----------------------------------------------------------------------
// VAMPIRE_BRACELET
//----------------------------------------------------------------------
class MVampireBracelet : public MAccessoryItem {
	public :
		MVampireBracelet() {}
		~MVampireBracelet() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_BRACELET; }

		ITEM_GEAR_SLOT	GetGearSlot() const					{ return SLOT_VAMPIRE_BRACELET; }
		bool		IsGearSlotVampireBracelet() const			{ return true; }

		static MItem*	NewItem()	{ return new MVampireBracelet; }
};

//----------------------------------------------------------------------
// VAMPIRE_NECKLACE
//----------------------------------------------------------------------
class MVampireNecklace : public MAccessoryItem {
	public :
		MVampireNecklace() {}
		~MVampireNecklace() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_NECKLACE; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_NECKLACE; }
		bool		IsGearSlotVampireNecklace() const	{ return true; }

		static MItem*	NewItem()	{ return new MVampireNecklace; }
};

//----------------------------------------------------------------------
// VAMPIRE_COAT
//----------------------------------------------------------------------
class MVampireCoat : public MArmorItem {
	public :
		MVampireCoat() {}
		~MVampireCoat() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_COAT; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_COAT; }
		bool		IsGearSlotVampireCoat() const	{ return true; }
		
		bool		IsAddonItem() const				{ return true; }
		ADDON		GetAddonSlot() const			{ return ADDON_COAT; }
		

		static MItem*	NewItem()	{ return new MVampireCoat; }
};

//----------------------------------------------------------------------
// MSkull
//----------------------------------------------------------------------
class MSkull : public MPileItem {
	public :
		MSkull() {}
		~MSkull() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SKULL; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MSkull; }
};

//----------------------------------------------------------------------
// Serum - 혈청
//----------------------------------------------------------------------
class MSerum : public MUsePotionItem {
	public :
		MSerum() {}
		~MSerum() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SERUM; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;
		
		// heal 되는 수치
		int		GetHealPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_SERUM][m_ItemType].Value1; }	// HP수치	

		static MItem*	NewItem()	{ return new MSerum; }
};

//----------------------------------------------------------------------
// ETC
//----------------------------------------------------------------------
class MVampireETC : public MPileItem {
	public :
		MVampireETC() {}
		~MVampireETC() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_ETC; }

		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MVampireETC; }
};


//----------------------------------------------------------------------
// Slayer Portal
//----------------------------------------------------------------------
class MSlayerPortalItem : public MItem {
	public :
		MSlayerPortalItem() { m_Number = 0; }
		virtual ~MSlayerPortalItem() {}

		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SLAYER_PORTAL_ITEM; }

		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		TYPE_ITEM_NUMBER	GetNumber() const					{ return m_Number; }
		void				SetNumber(TYPE_ITEM_NUMBER n)		{ m_Number = n; }

		bool				IsChargeItem() const				{ return true; }

		void				SetEnchantLevel(WORD s);

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MSlayerPortalItem; }
};

//----------------------------------------------------------------------
// Vampire Portal
//----------------------------------------------------------------------
class MVampirePortalItem : public MItem {
	public :
		MVampirePortalItem();
		virtual ~MVampirePortalItem() {}

		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_PORTAL_ITEM; }
		
		// 쌓이는 최대 개수
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		TYPE_ITEM_NUMBER	GetNumber() const					{ return m_Number; }
		void				SetNumber(TYPE_ITEM_NUMBER n)		{ m_Number = n; }

		bool				IsChargeItem() const				{ return true; }

		// 목표 zone에 대한 정보
		bool				IsMarked() const;
		void				SetZone(int zoneID, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		int					GetZoneID() const	{ return m_ZoneID; }
		TYPE_SECTORPOSITION	GetZoneX() const	{ return m_ZoneX; }
		TYPE_SECTORPOSITION	GetZoneY() const	{ return m_ZoneY; }

		// zone정보 설정한다. - -;;
		void				SetSilver(int s);
		void				SetCurrentDurability(TYPE_ITEM_DURATION d);
		void				SetEnchantLevel(WORD s);

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

		static MItem*	NewItem()	{ return new MVampirePortalItem; }

	protected :
		int						m_ZoneID;
		TYPE_SECTORPOSITION		m_ZoneX;
		TYPE_SECTORPOSITION		m_ZoneY;
};

//----------------------------------------------------------------------
// Event GiftBox
//----------------------------------------------------------------------
class MEventGiftBoxItem : public MItem {
	public :
		MEventGiftBoxItem()	{}
		virtual ~MEventGiftBoxItem() {}

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_EVENT_GIFT_BOX; }

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		
		static MItem*	NewItem()	{ return new MEventGiftBoxItem; }
};

//----------------------------------------------------------------------
// Event Star
//----------------------------------------------------------------------
class MEventStarItem : public MPileItem {
	public :
		MEventStarItem()	{}
		virtual ~MEventStarItem() {}

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_EVENT_STAR; }

		TYPE_ITEM_NUMBER	GetMaxNumber() const;

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		
		static MItem*	NewItem()	{ return new MEventStarItem; }
};

//----------------------------------------------------------------------
// Vampire Weapon
//----------------------------------------------------------------------
class MVampireWeapon : public MWeaponItem {
	public :
		MVampireWeapon() {}
		~MVampireWeapon() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_WEAPON; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_VAMPIRE_TWOHAND; }
		bool		IsGearSlotTwoHand() const		{ return true; }

		bool		IsGearSlotVampireWeapon() const			{ return true; }

		static MItem*	NewItem()	{ return new MVampireWeapon; }
};

//----------------------------------------------------------------------
// VAMPIRE_Amulet
//----------------------------------------------------------------------
class MVampireAmulet : public MAccessoryItem {
	public :
		MVampireAmulet() {}
		~MVampireAmulet() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_AMULET; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_AMULET; }
		bool		IsGearSlotVampireAmulet() const			{ return true; }
		bool		IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
		static MItem*	NewItem()	{ return new MVampireAmulet; }
};

//----------------------------------------------------------------------
// QuestItem
//----------------------------------------------------------------------
class MQuestItem : public MItem {
	public :
		MQuestItem()	{}
		virtual ~MQuestItem() {}

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_QUEST_ITEM; }

		static MItem*	NewItem()	{ return new MQuestItem; }
};

//----------------------------------------------------------------------
// Event Tree
//----------------------------------------------------------------------
class MEventTreeItem : public MItem {
	public :
		MEventTreeItem()	{}
		virtual ~MEventTreeItem() {}

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_EVENT_TREE; }

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		
		static MItem*	NewItem()	{ return new MEventTreeItem; }
};

//----------------------------------------------------------------------
// Event ETC
//----------------------------------------------------------------------
class MEventEtcItem : public MPileItem {
	public :
		MEventEtcItem()	{}
		virtual ~MEventEtcItem() {}

		ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_EVENT_ETC; }
		
		TYPE_ITEM_NUMBER	GetMaxNumber() const;

		// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
		void		UseQuickItem();
		bool		IsQuickItem() const { return (GetItemType()>=14&& GetItemType()<=17)?true:false; }

		static MItem*	NewItem()	{ return new MEventEtcItem; }
};

//----------------------------------------------------------------------
// BloodBible
//----------------------------------------------------------------------
class MBloodBible : public MItem {
	public :
		MBloodBible() {}
		~MBloodBible() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_BLOOD_BIBLE; }

		static MItem*	NewItem()	{ return new MBloodBible; }
};


//----------------------------------------------------------------------
// CastleSymbol
//----------------------------------------------------------------------
class MCastleSymbol : public MItem {
	public :
		MCastleSymbol() {}
		~MCastleSymbol() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_CASTLE_SYMBOL; }

		static MItem*	NewItem()	{ return new MCastleSymbol; }
};

//----------------------------------------------------------------------
// CoupleRing
//----------------------------------------------------------------------
class MCoupleRing : public MRing {
	public :
		MCoupleRing() {}
		~MCoupleRing() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_COUPLE_RING; }

		// use item
		void	UseGear();
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템

		static MItem*	NewItem()	{ return new MCoupleRing; }
};

//----------------------------------------------------------------------
// VampireCoupleRing
//----------------------------------------------------------------------
class MVampireCoupleRing : public MVampireRing {
	public :
		MVampireCoupleRing() {}
		~MVampireCoupleRing() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_VAMPIRE_COUPLE_RING; }
		
		// use item
		void	UseGear();
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
		static MItem*	NewItem()	{ return new MVampireCoupleRing; }
};

class MEventItem : public MItem {
public :
	MEventItem() {}
	~MEventItem() {}

	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_EVENT_ITEM; }

	static MItem* NewItem()		{ return new MEventItem; }

};

class MDyePotionItem : public MItem {
public :
	MDyePotionItem() {}
	~MDyePotionItem() {}

	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_DYE_POTION; }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
//	void		UseQuickItem();
//	bool		IsQuickItem() const { return true; }


	static MItem* NewItem()	{ return new MDyePotionItem; }
};

class MResurrectItem : public MPileItem {
public :
	MResurrectItem() {}
	~MResurrectItem() {}

	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_RESURRECT_ITEM; }
	TYPE_ITEM_NUMBER	GetMaxNumber() const;

	static MItem* NewItem() { return new MResurrectItem; }
};

class MMixingItem : public MPileItem {
public :
	MMixingItem() {}
	~MMixingItem() {}
	
	ITEM_CLASS	GetItemClass() const { return ITEM_CLASS_MIXING_ITEM; }
	TYPE_ITEM_NUMBER	GetMaxNumber() const;

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

	static MItem* NewItem() { return new MMixingItem; }
};

//----------------------------------------------------------------------
// Ousters ArmsBand
//----------------------------------------------------------------------
class MOustersArmsBand : public MArmorItem2, public MSlotItemManager {
public :
	MOustersArmsBand() {}
	~MOustersArmsBand() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_ARMSBAND; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_OUSTERS_ARMSBAND; }
	
	bool		IsGearSlotOustersArmsBand() const			{ return true; }
	bool		IsContainerItem() const { return true;}

	void		SetItemType(TYPE_ITEMTYPE type);	
	
	//------------------------------------------------
	//
	//				SlotItemManager
	//
	//------------------------------------------------
	// AddItem : slot(n)에 pItem을 추가한다.
	//------------------------------------------------
	bool			AddItem(MItem* pItem);
	bool			AddItem(MItem* pItem, BYTE n);
	
	//------------------------------------------------
	// ReplaceItem : pItem을 추가하고 딴게 있다면 Item교환
	//------------------------------------------------
	bool			ReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);		
	//------------------------------------------------
	// Can ReplaceItem : n slot에 pItem을 추가가능한가? 
	// pOldItem에는 이전에 있던 것..
	//------------------------------------------------
	bool			CanReplaceItem(MItem* pItem, BYTE n, MItem*& pOldItem);
	
	//------------------------------------------------
	// Find Slot To Add Item
	//------------------------------------------------
	bool			FindSlotToAddItem(MItem* pItem, int &slot) const;	
	// 값
	virtual int	GetPocketNumber() const		{ return (*g_pItemTable)[ITEM_CLASS_OUSTERS_ARMSBAND][m_ItemType].Value3; }	// 포켓수
	
	static MItem*	NewItem()	{ return new MOustersArmsBand; }
};

//----------------------------------------------------------------------
// Ousters Boots
//----------------------------------------------------------------------
class MOustersBoots : public MArmorItem {
public :
	MOustersBoots() {}
	~MOustersBoots() {}
	
public :
	ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_OUSTERS_BOOTS; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_BOOTS; }
	bool		IsGearSlotOustersBoots() const			{ return true; }
	
	bool		IsAddonItem() const				{ return true; }
	ADDON		GetAddonSlot() const			{ return ADDON_TROUSER; }
	
	static MItem*	NewItem()	{ return new MOustersBoots; }
};

//----------------------------------------------------------------------
// Ousters Chakram
//----------------------------------------------------------------------
class MOustersChakram : public MWeaponItem {
public :
	MOustersChakram() {}
	~MOustersChakram() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_CHAKRAM; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_OUSTERS_TWOHAND; }
	bool		IsGearSlotTwoHand() const		{ return true; }
	
	bool		IsGearSlotOustersWeapon() const			{ return true; }
	
	bool		IsAddonItem() const				{ return true; }
	ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }
	
	static MItem*	NewItem()	{ return new MOustersChakram; }
};

//----------------------------------------------------------------------
// Ousters Circlet
//----------------------------------------------------------------------
class MOustersCirclet : public MArmorItem2 {
public :
	MOustersCirclet() {}
	~MOustersCirclet() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_CIRCLET; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_CIRCLET; }
	bool		IsGearSlotOustersCirclet() const			{ return true; }
	
	static MItem*	NewItem()	{ return new MOustersCirclet; }
};

//----------------------------------------------------------------------
// Ousters Coat
//----------------------------------------------------------------------
class MOustersCoat : public MArmorItem {
public :
	MOustersCoat() {}
	~MOustersCoat() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_COAT; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_COAT; }
	bool		IsGearSlotOustersCoat() const	{ return true; }
	
	bool		IsAddonItem() const				{ return true; }
	ADDON		GetAddonSlot() const			{ return ADDON_COAT; }
	
	
	static MItem*	NewItem()	{ return new MOustersCoat; }
};

//----------------------------------------------------------------------
// Ousters Pendent
//----------------------------------------------------------------------
class MOustersPendent : public MAccessoryItem {
public :
	MOustersPendent() {}
	~MOustersPendent() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_PENDENT; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_PENDENT; }
	bool		IsGearSlotOustersPendent() const			{ return true; }
	
	static MItem*	NewItem()	{ return new MOustersPendent; }
};

//----------------------------------------------------------------------
// Ousters Ring
//----------------------------------------------------------------------
class MOustersRing : public MAccessoryItem {
public :
	MOustersRing() {}
	~MOustersRing() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_RING; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_RING; }
	bool		IsGearSlotOustersRing() const			{ return true; }
	
	static MItem*	NewItem()	{ return new MOustersRing; }
};

//----------------------------------------------------------------------
// Ousters Stone
//----------------------------------------------------------------------
class MOustersStone : public MAccessoryItem {
public :
	MOustersStone() {}
	~MOustersStone() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_STONE; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_STONE; }
	bool		IsGearSlotOustersStone() const			{ return true; }
	
	static MItem*	NewItem()	{ return new MOustersStone; }
};

//----------------------------------------------------------------------
// Ousters Wristlet
//----------------------------------------------------------------------
class MOustersWristlet : public MWeaponItem {
public :
	MOustersWristlet() {}
	~MOustersWristlet() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_WRISTLET; }
	
	ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_OUSTERS_TWOHAND; }
	bool		IsGearSlotTwoHand() const		{ return true; }
	
	bool		IsGearSlotOustersWristlet() const			{ return true; }
	
	bool		IsAddonItem() const				{ return true; }
	ADDON		GetAddonSlot() const			{ return ADDON_RIGHTHAND; }
	
	static MItem*	NewItem()	{ return new MOustersWristlet; }
};

//----------------------------------------------------------------------
// Ousters Larva
//----------------------------------------------------------------------
class MOustersLarva : public MPileItem {
public :
	MOustersLarva() {}
	~MOustersLarva() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_LARVA; }
	TYPE_ITEM_NUMBER	GetMaxNumber() const;	
	static MItem*	NewItem()	{ return new MOustersLarva; }
};

//----------------------------------------------------------------------
// Ousters Pupa
//----------------------------------------------------------------------
class MOustersPupa : public MUsePotionItem {
public :
	MOustersPupa() {}
	~MOustersPupa() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_PUPA; }
	TYPE_ITEM_NUMBER	GetMaxNumber() const;
	
	// heal 되는 수치
	int		GetHealPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_PUPA][m_ItemType].Value1; }	// HP수치	
	// mana 되는 수치
	int		GetManaPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_PUPA][m_ItemType].Value2; }	// MP수치

	bool	IsQuickItem() const			{ return true; }

	// use item
	void	UseQuickItem();
		
	
	static MItem*	NewItem()	{ return new MOustersPupa; }
};

//----------------------------------------------------------------------
// Ousters ComposMei
//----------------------------------------------------------------------
class MOustersComposMei : public MUsePotionItem {
public :
	MOustersComposMei() {}
	~MOustersComposMei() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_COMPOS_MEI; }
	TYPE_ITEM_NUMBER	GetMaxNumber() const;

	// heal 되는 수치
	int		GetHealPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_COMPOS_MEI][m_ItemType].Value1; }	// HP수치
	
	// mana 되는 수치
	int		GetManaPoint() const		{ return (*g_pItemTable)[ITEM_CLASS_COMPOS_MEI][m_ItemType].Value2; }	// MP수치

	bool	IsQuickItem() const			{ return true; }

	// use item
	void	UseQuickItem();
	
	
	static MItem*	NewItem()	{ return new MOustersComposMei; }
};

//----------------------------------------------------------------------
// Ousters SummonGem
//----------------------------------------------------------------------
class MOustersSummonGem : public MItem {
public :
	MOustersSummonGem() {}
	~MOustersSummonGem() {}
	
public :
	ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_OUSTERS_SUMMON_ITEM; }
	
	static MItem*	NewItem()	{ return new MOustersSummonGem; }

	// 쌓이는 최대 개수
	TYPE_ITEM_NUMBER	GetMaxNumber() const;
	
	TYPE_ITEM_NUMBER	GetNumber() const					{ return m_Number; }
	void				SetNumber(TYPE_ITEM_NUMBER n)		{ m_Number = n; }
	
	void				SetEnchantLevel(WORD s);

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	bool				IsChargeItem() const				{ return true; }
};

class MEffectItem : public MPileItem {
public :	
	MEffectItem() {}
	~MEffectItem() {}

public :
	ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_EFFECT_ITEM; }

	static MItem* NewItem()		{ return new MEffectItem; }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

	TYPE_ITEM_NUMBER			GetMaxNumber() const;
};

class MCodeSheetItem : public MItem {
public :
	MCodeSheetItem() {}
	~MCodeSheetItem() {}

	ITEM_CLASS	GetItemClass() const			{ return ITEM_CLASS_CODE_SHEET; }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif

	static MItem* NewItem()		{ return new MCodeSheetItem; }
};

class MMoonCardItem : public MPileItem {
public : 
	MMoonCardItem() { }
	~MMoonCardItem() { }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_MOON_CARD; }	
	static MItem*			NewItem()					{ return new MMoonCardItem; }	
	TYPE_ITEM_NUMBER		GetMaxNumber() const;
};

class MSweeperItem : public MItem {
public :
	MSweeperItem() { }
	~MSweeperItem() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_SWEEPER; }
	static MItem*			NewItem()					{ return new MSweeperItem; }
};

class MPetItem : public MItem {
private:
	DWORD	m_UpdateTime;
	DWORD	m_PetExpRemain;
	//2004, 5, 11 sobeit add start
	DWORD	m_PetKeepedDay;//보관날짜
	//2004, 5, 11 sobeit add end
	WORD	m_PetFoodType;
	bool	m_bCanGamble;
	bool	m_bCutHead;
	bool	m_bCanAttack;
public :
	MPetItem();
	~MPetItem() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_PET_ITEM; }

	// pet info
	void					SetPetExpRemain(DWORD exp)			{ m_PetExpRemain = exp; }
	const DWORD				GetPetExpRemain() const				{ return m_PetExpRemain; }
	void					SetPetFoodType(WORD type)		{ m_PetFoodType = type; }
	const DWORD				GetPetFoodType() const			{ return m_PetFoodType; }
	void					SetPetGamble(bool bGamble)		{ m_bCanGamble = bGamble; }
	const bool				IsCanGamble() const				{ return m_bCanGamble; }
	void					SetPetCutHead(bool bCutHead)	{ m_bCutHead = bCutHead; }
	const bool				IsCanCutHead() const			{ return m_bCutHead; }
	void					SetPetAttack(bool bAttack)	{ m_bCanAttack = bAttack; }
	const bool				IsCanAttack() const			{ return m_bCanAttack; }
	const DWORD				GetUpdateTime() const			{ return m_UpdateTime; }
	void					SetUpdateTime(DWORD updateTime)	{ m_UpdateTime = updateTime; }
	void					SetCurrentDurability(TYPE_ITEM_DURATION d)	{ MItem::SetCurrentDurability(d); SetUpdateTime(timeGetTime()); }

	//2004, 5, 11 sobeit add start
	void					SetPetKeepedDay(DWORD day)			{ m_PetKeepedDay = day; }
	const DWORD				GetPetKeepedDay() const				{ return m_PetKeepedDay; }
	//2004, 5, 11 sobeit add end
	std::string				GetPetName();
	std::string				GetPetOptionName();
	std::string				GetPetOptionEName();
	
	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	static MItem*			NewItem()					{ return new MPetItem; }
};

class MPetFood : public MPileItem {
public :
	MPetFood() { }
	~MPetFood() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_PET_FOOD; }

	TYPE_ITEM_NUMBER		GetMaxNumber() const;
	
	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	static MItem*			NewItem()					{ return new MPetFood; }
};

class MPetEnchantItem : public MPileItem {
public :
	MPetEnchantItem() { }
	~MPetEnchantItem() { }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	TYPE_ITEM_NUMBER		GetMaxNumber() const;

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_PET_ENCHANT_ITEM; }

	static MItem*			NewItem()					{ return new MPetEnchantItem; }
};

class MLuckyBag : public MPileItem {
public :
	MLuckyBag() { }
	~MLuckyBag() { }

	TYPE_ITEM_NUMBER		GetMaxNumber() const;

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_LUCKY_BAG; }

	static MItem*			NewItem()					{ return new MLuckyBag; }
};

class MSms_item : public MItem {
public :
	MSms_item() { }
	~MSms_item() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_SMS_ITEM; }


	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	static MItem*			NewItem()					{ return new MSms_item; }
};


class MCoreZap : public MAccessoryItem {
public :
	MCoreZap() { }
	~MCoreZap() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_CORE_ZAP; }

	
	static MItem*			NewItem()					{ return new MCoreZap; }
	bool	IsGearSlotCoreZap() const					{ return true; }
	bool	IsGearSlotVampireCoreZap() const			{ return true; }
	bool	IsGearSlotOustersCoreZap() const			{ return true; }
	bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

class MGQuestItem : public MItem {
public :
	MGQuestItem() { }
	~MGQuestItem() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_GQUEST_ITEM; }
	static MItem*			NewItem()					{ return new MGQuestItem; }

};


class MTrapItem : public MItem {
public :
	MTrapItem() { }
	~MTrapItem() { }

	// use item
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);
	#else
		void		UseInventory();
	#endif
	
	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_TRAP_ITEM; }
	static MItem*			NewItem()					{ return new MTrapItem; }

};
 

class MBloodBibleSign : public MGearItem {
public :
	MBloodBibleSign() { }
	~MBloodBibleSign() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_BLOOD_BIBLE_SIGN; }
	
	static MItem*			NewItem()					{ return new MBloodBibleSign; }

	ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_BLOOD_BIBLE; }

	bool	IsGearSlotBloodBible() const				{ return true; }
	bool	IsGearSlotVampireBloodBible() const			{ return true; }
	bool	IsGearSlotOustersBloodBible() const			{ return true; }

	bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

class MWarItem : public MItem {
public :
	MWarItem() { }
	~MWarItem() { }

	ITEM_CLASS				GetItemClass() const		{ return ITEM_CLASS_WAR_ITEM; }
	static MItem*			NewItem()					{ return new MWarItem; }

};

// By csm 2004.12.27 전직 아이템 추가 

class MCarryingReceiver : public MAccessoryItem {
	public :
		MCarryingReceiver() {}
		~MCarryingReceiver() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_CARRYING_RECEIVER; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_PDA; }
//
		static MItem*	NewItem()	{ return new MCarryingReceiver; }

		bool		IsGearSlotPDA() const			{ return true; }	// PDA 슬롯
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

class MShoulderArmor : public MAccessoryItem {
	public :
		MShoulderArmor() {}
		~MShoulderArmor() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SHOULDER_ARMOR; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_SHOULDER; }

		static MItem*	NewItem()	{ return new MShoulderArmor; }

		bool		IsGearSlotShoulder() const		{ return true; }	// 어깨 슬롯
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
		bool		IsAddonItem() const				{ return true; }
};
class MDermis : public MAccessoryItem {
	public :
		MDermis() {}
		~MDermis() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_DERMIS; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_DERMIS; }
		//bool		IsGearSlotVampireRing() const			{ return true; }

		static MItem*	NewItem()	{ return new MDermis; }
		bool		IsGearSlotVampireDermis() const		{ return true; }	// 문신
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

class MPersona : public MAccessoryItem {
	public :
		MPersona() {}
		~MPersona() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_PERSONA; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_VAMPIRE_PERSONA; }
//		bool		IsGearSlotVampireRing() const			{ return true; }

		static MItem*	NewItem()	{ return new MPersona; }
		bool		IsGearSlotVampirePersona() const		{ return true; }	// 페르소나
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};


class MFascia : public MAccessoryItem {
	public :
		MFascia() {}
		~MFascia() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_FASCIA; }

		ITEM_GEAR_SLOT	GetGearSlot() const				{ return SLOT_OUSTERS_FASCIA; }
//		bool		IsGearSlotVampireRing() const			{ return true; }

		static MItem*	NewItem()	{ return new MFascia; }
		bool		IsGearSlotOustersFascia() const		{ return true; }	// 허리장식
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

class MMitten : public MArmorItem {
	public :
		MMitten() {}
		~MMitten() {}

	public :
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_MITTEN; }

		ITEM_GEAR_SLOT	GetGearSlot() const			{ return SLOT_OUSTERS_MITTEN; }
//		bool		IsGearSlotCoat() const			{ return true; }

//		bool		IsAddonItem() const				{ return true; }
//		ADDON		GetAddonSlot() const			{ return ADDON_COAT; }

		static MItem*	NewItem()	{ return new MMitten; }
		bool		IsGearSlotOustersMitten() const		{ return true; }	// 장갑
		bool	IsDurationAlwaysOkay() const		{ return true; }	// 내구 체크 안하는 아이템
};

#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관

class MSubInventory : public MItem , public MGridItemManager {
	public :
		MSubInventory() {}
		~MSubInventory() {}

	public :
		
		ITEM_CLASS	GetItemClass() const		{ return ITEM_CLASS_SUB_INVENTORY; }

		void		UseInventory(TYPE_OBJECTID SubInventoryItemID = 0);

		static MItem*	NewItem()	{ return new MSubInventory; }

		void			SetItemType(TYPE_ITEMTYPE type);

		bool			IsContainerItem() const		{ return true; }

		//------------------------------------------------
		//
		//				SlotItemManager
		//
		//------------------------------------------------
		// AddItem : slot(n)에 pItem을 추가한다.
		//------------------------------------------------
		bool			AddItem(MItem* pItem);
		bool			AddItem(MItem* pItem, BYTE X, BYTE Y);
		
		//------------------------------------------------
		// ReplaceItem : pItem을 추가하고 딴게 있다면 Item교환
		//------------------------------------------------
		bool			ReplaceItem(MItem* pItem, BYTE X, BYTE Y, MItem*& pOldItem);	

		//------------------------------------------------
		// Can ReplaceItem : n slot에 pItem을 추가가능한가? 
		// pOldItem에는 이전에 있던 것..
		//------------------------------------------------
		bool			CanReplaceItem(MItem* pItem,BYTE X, BYTE Y, MItem*& pOldItem);
};
#endif
#endif