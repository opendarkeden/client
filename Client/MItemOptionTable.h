//----------------------------------------------------------------------
// MItemOptionTable.h
//----------------------------------------------------------------------
// Item의 Option의 종류에 따른 정보를 저장하는 class
//----------------------------------------------------------------------

#ifndef	__MITEMOPTIONTABLE_H__
#define	__MITEMOPTIONTABLE_H__

#include "CTypeTable.h"
#include "MTypeDef.h"
#include "MString.h"

//#define	MAX_PARTNAME_LENGTH		48
#define UNIQUE_ITEM_COLOR		0xffff
#define QUEST_ITEM_COLOR		0xfffe

//----------------------------------------------------------------------
//
// 한 종류의 Item에 대한 정보
//
//----------------------------------------------------------------------
class ITEMOPTION_INFO {
	public :
	public :
		MString					EName;					// ItemOption 영어 이름		
		MString					Name;					// ItemOption 이름
		int						Part;					// ItemOption Part				
		int						PlusPoint;				// 증가치
		int						PriceMultiplier;		// 가치	(%로 되어 있으므로, 100으로 나누어야 함)	
		//int						PlusRequireAbility;		// 필요 능력 증가치		

		// 필요 능력치
		int						RequireSTR;
		int						RequireDEX;
		int						RequireINT;
		int						RequireSUM;		// 능력치 총합
		int						RequireLevel;
		
		int						ColorSet;				// 색 번호
		int						UpgradeOptionType;		// 업그래이드 되면 변-_-신하는 타입 0이면 변-_-신 안함
		int						PreviousOptionType;		// 업그래이드 실패해서 변-_-신하는 타입 0이면 변-_-신 안함

	public :
		ITEMOPTION_INFO();
		~ITEMOPTION_INFO();

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(std::ofstream& file);		
		void			LoadFromFile(ifstream& file);		
};


//--------------------------------------------------------------------------
// 하나의 class에 대한 type들의 정보
//--------------------------------------------------------------------------
class ITEMOPTION_TABLE:public CTypeTable<ITEMOPTION_INFO>
{
public:
	enum ITEMOPTION_PART
	{
		PART_STR = 0,      // increase STR
		PART_DEX,          // increase DEX 
		PART_INT,          // increase INT 
		PART_HP,           // increase HP 
		PART_MP,           // increase MP 
		PART_HP_STEAL,     // steal HP 
		PART_MP_STEAL,     // steal MP 
		PART_HP_REGEN,     // regenerate hp over time
		PART_MP_REGEN,     // regenerate mp over time
		PART_TOHIT,        // increase tohit
		PART_DEFENSE,      // increase defense
		PART_DAMAGE,       // increase damage 
		PART_PROTECTION,   // increase protection
		PART_DURABILITY,   // increase item durability
		PART_POISON,       // increase poison resistence
		PART_ACID,         // increase acid resistence
		PART_CURSE,        // increase curse resistence
		PART_BLOOD,        // increase blood resistence
		PART_VISION,       // increase vision range
		PART_ATTACK_SPEED, // increase attack speed
		PART_CRITICAL_HIT,
			
		// 2002.10.21일 추가
		PART_LUCK,         // increase looting item type
		PART_ALL_RES,      // increase all registance
		PART_ALL_ATTR,     // increase all attributes(str, dex, int)

		// 2002.12.03일 추가
		PART_STR_TO_DEX,   // STR to DEX
		PART_STR_TO_INT,   // STR to INT
		PART_DEX_TO_STR,   // DEX to STR
		PART_DEX_TO_INT,   // DEX to INT
		PART_INT_TO_STR,   // INT to STR
		PART_INT_TO_DEX,   // INT to DEX

		// 2003.2.12
		PART_CONSUME_MP,       // decrease consume mana
		PART_TRANS,        // translate race language
		PART_MAGIC_DAMAGE,  // increase magic damage
		PART_PHYSIC_DAMAGE, // increase physical damage
		PART_GAMBLE_PRICE, // decrease gamble price
		PART_POTION_PRICE, // decrease gamble price
 
		OPTION_MAGIC_PRO,	// 마법 방어력
		OPTION_PHYSIC_PRO,	// 물리 방어력

		MAX_PART
	};

	// option part의 name string
	MString ITEMOPTION_PARTNAME[MAX_PART];
	MString ITEMOPTION_PARTENAME[MAX_PART];
		

	void	LoadFromFile(ifstream& file);
};

extern	ITEMOPTION_TABLE*		g_pItemOptionTable;


#endif
