   //----------------------------------------------------------------------
// MItemTable.cpp
//----------------------------------------------------------------------
// Item冻绢瘤绰吧 窍绊 酵栏搁..
// 阿 info俊促啊 DropFrameID( ...)甫 力措肺 汲沥窍搁 等促.
// 拱沸 .. MTopView俊绰 ItemDropFPK客 ItemDropFPK啊 力措肺 等霸 乐绢具 摆瘤..
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MItem.h"
#include "MItemTable.h"
#include "AddonDef.h"
#include "SkillDef.h"
#include "SoundDef.h"
#include "MGameStringTable.h"

#ifdef __GAME_CLIENT__
	#include "DebugInfo.h"
#endif

//#if !defined(__GAME_CLIENT__) || defined(OUTPUT_DEBUG) || defined(_DEBUG)
#if defined(__INIT_INFO__) || !defined(__GAME_CLIENT__)
	#define __INIT_ITEM__
#endif

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
ITEMCLASS_TABLE	*	g_pItemTable = NULL;
COLORREF g_ELEMENTAL_COLOR[5] = { RGB(255, 100, 100), RGB(100, 100, 255), RGB(255, 180, 100), RGB(100, 100, 255), RGB(192, 192, 255) };
int g_ELEMENTAL_STRING_ID[5] = { UI_STRING_MESSAGE_ELEMENTAL_FIRE, UI_STRING_MESSAGE_ELEMENTAL_WATER, UI_STRING_MESSAGE_ELEMENTAL_EARTH, UI_STRING_MESSAGE_ELEMENTAL_WIND, UI_STRING_MESSAGE_ELEMENTAL_SUM};

//----------------------------------------------------------------------
//
//				ITEMTABLE_INFO
//
//----------------------------------------------------------------------
ITEMTABLE_INFO::ITEMTABLE_INFO()
{
	// Frame ID
	TileFrameID			= FRAMEID_NULL;		// Tile俊辑狼 FrameID
	InventoryFrameID	= FRAMEID_NULL;		// Inventory俊辑狼 Frame ID
	GearFrameID			= FRAMEID_NULL;		// Gear俊辑狼 Frame ID
	AddonMaleFrameID	= FRAMEID_NULL;		// 厘馒沁阑 锭狼 悼累 FrameID - 巢磊
	AddonFemaleFrameID	= FRAMEID_NULL;		// 厘馒沁阑 锭狼 悼累 FrameID - 咯磊

	// Sound ID
	UseSoundID			= SOUNDID_NULL;		// Item 荤侩 SoundID			
	TileSoundID			= SOUNDID_NULL;		// Item 凛扁 SoundID
	InventorySoundID	= SOUNDID_NULL;		// Inventory俊辑狼 Sound
	GearSoundID			= SOUNDID_NULL;		// Gear俊辑狼 Sound

	bMaleOnly			= false;
	bFemaleOnly			= false;
	
	// inventory俊辑狼 Grid农扁
	GridWidth			= 1;
	GridHeight			= 1;

	// item 磊眉俊 措茄 绊沥等 沥焊
	Weight				= 0;				// 公霸	

	// 蔼甸.. --> Protection, 傍拜仿, 荤沥芭府
	Value1				= 0;
	Value2				= 0;
	Value3				= 0;
	Value4				= 0;
	Value5				= 0;
	Value6				= 0;
	Value7				= 0;

	// 鞘夸瓷仿
	RequireSTR			= 0;
	RequireDEX			= 0;
	RequireINT			= 0;
	RequireSUM			= 0;
	RequireAdvancementLevel = 0;
	RequireLevel		= 0;

	// 扁夯 傍拜 ActionInfo
	UseActionInfo		= ACTIONINFO_NULL;

	// silver coating
	SilverMax			= 0;

	ToHit				= 0;

	MaxNumber			= 1;

	CriticalHit			= 0;

	ItemStyle			= 0;

	ElementalType		= ELEMENTAL_TYPE_ANY;
	Elemental			= 0;

	DescriptionFrameID = 0;
}

ITEMTABLE_INFO::~ITEMTABLE_INFO()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set SoundID
//----------------------------------------------------------------------
void	
ITEMTABLE_INFO::SetSoundID(TYPE_SOUNDID tile, TYPE_SOUNDID inventory, TYPE_SOUNDID gear, TYPE_SOUNDID use)
{
	TileSoundID = tile;
	InventorySoundID = inventory;
	GearSoundID = gear;
	UseSoundID = use;	
}

//----------------------------------------------------------------------
// Set FrameID
//----------------------------------------------------------------------
void	
ITEMTABLE_INFO::SetFrameID(TYPE_FRAMEID tile, TYPE_FRAMEID inventory, TYPE_FRAMEID gear)
{
	TileFrameID = tile;
	InventoryFrameID = inventory;
	GearFrameID = gear;	
}

//----------------------------------------------------------------------
// Set DropFrameID
//----------------------------------------------------------------------
void	
ITEMTABLE_INFO::SetDropFrameID(TYPE_FRAMEID drop)
{
	DropFrameID	= drop;
}

//----------------------------------------------------------------------
// Set Addon FrameID
//----------------------------------------------------------------------
void
ITEMTABLE_INFO::SetAddonFrameID(TYPE_FRAMEID male, TYPE_FRAMEID female)
{
	AddonMaleFrameID = male;
	AddonFemaleFrameID = female;
}

//----------------------------------------------------------------------
// Set Grid
//----------------------------------------------------------------------
void	
ITEMTABLE_INFO::SetGrid(BYTE width, BYTE height)
{
	GridWidth = width;
	GridHeight = height;
}

//----------------------------------------------------------------------
// Set Values
//----------------------------------------------------------------------
void	
ITEMTABLE_INFO::SetValue(int v1, int v2, int v3, int v4, int v5, int v6, int v7)
{
	Value1 = v1;
	Value2 = v2;
	Value3 = v3;
	Value4 = v4;
	Value5 = v5;
	Value6 = v6;
	Value7 = v7;
}

//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
void			
ITEMTABLE_INFO::SaveToFile(class ofstream& file)
{
	// 捞抚 历厘
	EName.SaveToFile( file );
	HName.SaveToFile( file );
	Description.SaveToFile( file );

	// Frame ID
	file.write((const char*)&TileFrameID, SIZE_FRAMEID);
	file.write((const char*)&InventoryFrameID, SIZE_FRAMEID);
	file.write((const char*)&GearFrameID, SIZE_FRAMEID);
	file.write((const char*)&DropFrameID, SIZE_FRAMEID);
	file.write((const char*)&AddonMaleFrameID, SIZE_FRAMEID);
	file.write((const char*)&AddonFemaleFrameID, SIZE_FRAMEID);

	
	// Sound ID
	file.write((const char*)&UseSoundID, SIZE_SOUNDID);
	file.write((const char*)&TileSoundID, SIZE_SOUNDID);
	file.write((const char*)&InventorySoundID, SIZE_SOUNDID);
	file.write((const char*)&GearSoundID, SIZE_SOUNDID);

	// inventory俊辑狼 Grid农扁
	file.write((const char*)&GridWidth, 1);
	file.write((const char*)&GridHeight, 1);

	// 啊拜
	file.write((const char*)&Price, SIZE_ITEM_PRICE);

	// 公霸
	file.write((const char*)&Weight, SIZE_ITEM_WEIGHT);

	// 蔼甸
	file.write((const char*)&Value1, 4);
	file.write((const char*)&Value2, 4);
	file.write((const char*)&Value3, 4);
	file.write((const char*)&Value4, 4);
	file.write((const char*)&Value5, 4);
	file.write((const char*)&Value6, 4);
	file.write((const char*)&Value7, 4);
	
	// 鞘夸瓷仿
	file.write((const char*)&RequireSTR, 1);
	file.write((const char*)&RequireDEX, 1);
	file.write((const char*)&RequireINT, 1);		
	file.write((const char*)&RequireSUM, 2);
	file.write((const char*)&RequireLevel, 1);
	file.write((const char*)&RequireAdvancementLevel, 1);
	file.write((const char*)&bMaleOnly, 1);
	file.write((const char*)&bFemaleOnly, 1);

	// UseActionInfo
	file.write((const char*)&UseActionInfo, 4);
	

	file.write((const char*)&SilverMax, 4);

	file.write((const char*)&ToHit, 4);

	file.write((const char*)&MaxNumber, SIZE_ITEM_NUMBER);

	file.write((const char*)&CriticalHit, 4);	

	BYTE DefaultOptionListSize = DefaultOptionList.size();
	file.write((const char*)&DefaultOptionListSize, 1);
	std::list<TYPE_ITEM_OPTION>::iterator itr = DefaultOptionList.begin();

	while(itr != DefaultOptionList.end())
	{
		TYPE_ITEM_OPTION Option = *itr;
		file.write((const char*)&Option, sizeof(TYPE_ITEM_OPTION));

		itr++;
	}

	file.write((const char*)&ItemStyle, 4);

	file.write((const char*)&ElementalType, 4);
	file.write((const char*)&Elemental, 2);

	file.write((const char*)&Race, 1);

	// 2005, 1, 14, sobeit add start - ItemDescription.spk 俊辑 静绰 frameID
	file.write((const char*)&DescriptionFrameID, SIZE_FRAMEID);
	// 2005, 1, 14, sobeit add end
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
ITEMTABLE_INFO::LoadFromFile(class ifstream& file)
{
	EName.LoadFromFile( file );
	HName.LoadFromFile( file );
	Description.LoadFromFile( file );

	// Frame ID
	file.read((char*)&TileFrameID, SIZE_FRAMEID);
	file.read((char*)&InventoryFrameID, SIZE_FRAMEID);
	file.read((char*)&GearFrameID, SIZE_FRAMEID);
	file.read((char*)&DropFrameID, SIZE_FRAMEID);
	file.read((char*)&AddonMaleFrameID, SIZE_FRAMEID);
	file.read((char*)&AddonFemaleFrameID, SIZE_FRAMEID);

	// Sound ID
	file.read((char*)&UseSoundID, SIZE_SOUNDID);
	file.read((char*)&TileSoundID, SIZE_SOUNDID);
	file.read((char*)&InventorySoundID, SIZE_SOUNDID);
	file.read((char*)&GearSoundID, SIZE_SOUNDID);	

	// grid 农扁
	file.read((char*)&GridWidth, 1);	
	file.read((char*)&GridHeight, 1);	

	// 啊拜
	file.read((char*)&Price, SIZE_ITEM_PRICE);

	// 公霸
	file.read((char*)&Weight, SIZE_ITEM_WEIGHT);	
	
	// 蔼甸 
	file.read((char*)&Value1, 4);	
	file.read((char*)&Value2, 4);
	file.read((char*)&Value3, 4);
	file.read((char*)&Value4, 4);
	file.read((char*)&Value5, 4);
	file.read((char*)&Value6, 4);
	file.read((char*)&Value7, 4);

	// 鞘夸瓷仿
	file.read((char*)&RequireSTR, 1);
	file.read((char*)&RequireDEX, 1);
	file.read((char*)&RequireINT, 1);		
	file.read((char*)&RequireSUM, 2);
	file.read((char*)&RequireLevel, 1);
	file.read((char*)&RequireAdvancementLevel, 1);
	file.read((char*)&bMaleOnly, 1);
	file.read((char*)&bFemaleOnly, 1);
	
	// UseActionInfo
	file.read((char*)&UseActionInfo, 4);

	file.read((char*)&SilverMax, 4);

	file.read((char*)&ToHit, 4);

	file.read((char*)&MaxNumber, SIZE_ITEM_NUMBER);

	file.read((char*)&CriticalHit, 4);

	BYTE DefaultOptionListSize = 0;
	file.read((char*)&DefaultOptionListSize, 1);
	
	for(int i = 0; i < DefaultOptionListSize; i++)
	{
		TYPE_ITEM_OPTION TempOptionType;
		file.read((char*)&TempOptionType, sizeof(TYPE_ITEM_OPTION));
		DefaultOptionList.push_back(TempOptionType);
	}

	file.read((char*)&ItemStyle, 4);

	file.read((char*)&ElementalType, 4);
	file.read((char*)&Elemental, 2);

	file.read((char*)&Race, 1);

	// 2005, 1, 14, sobeit add start - ItemDescription.spk 俊辑 静绰 frameID
	file.read((char*)&DescriptionFrameID, SIZE_FRAMEID);
	// 2005, 1, 14, sobeit add end
}

//----------------------------------------------------------------------
//
//							ITEMTYPE_TABLE
//
//----------------------------------------------------------------------
void
ITEMTYPE_TABLE::LoadFromFile(class ifstream& file)
{
	CTypeTable<ITEMTABLE_INFO>::LoadFromFile(file);

	
	m_AveragePrice = 0;

	int count = 0;
	for (int i=0; i<m_Size; i++)
	{
		if(m_pTypeInfo[i].DefaultOptionList.empty())
		{
			m_AveragePrice += m_pTypeInfo[i].Price;
			count ++;
		}
	}
	if(count)
	{
		m_AveragePrice /= count;
		m_AveragePrice /= 1000;
		m_AveragePrice *= 100;
	}
}
//----------------------------------------------------------------------
//
//							ITEMCLASS_TABLE
//
//----------------------------------------------------------------------
ITEMCLASS_TABLE::ITEMCLASS_TABLE()
{

#ifdef __INIT_ITEM__
	// class 俺荐 汲沥
	Init( MAX_ITEM_CLASS );
	
	
	// (tile, inventory, gear, actionFrame)	

	//---------------------------------------------------------------------
	// ITEM_CLASS_MOTORCYCLE
	//---------------------------------------------------------------------
	// 郴备己(1), 款馆啊瓷公霸(2)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_MOTORCYCLE, 8 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].HName = "胶妮乔攫";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].EName = "Scorpion";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].Description = "";//坷弗率 滚瓢阑 努腐窍搁 坷配官捞甫 阂矾 棵 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetFrameID( 119, 115, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].SetValue(300, 80);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][0].Price = 24000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].HName = "龋农酒捞";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].EName = "Hawkeye";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetFrameID( 119, 115, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][1].Price = 40000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].HName = "乔葱胶";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].EName = "Phoenix";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetFrameID( 119, 115, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][2].Price = 60000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].HName = "姐歹滚靛";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].EName = "Thunder Bird";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetFrameID( 119, 115, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][3].Price = 80000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].HName = "浚俩 喇";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].EName = "Angel Wing";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetFrameID( 119, 115, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][4].Price = 120000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].HName = "风厚酒都";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].EName = "Rubiate";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetFrameID( 119, 115, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetAddonFrameID( ADDONID_MOTORCYCLE_MALE, ADDONID_MOTORCYCLE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][5].Price = 120000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].HName = "B2-阁胶磐";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].EName = "B2-Monster";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetFrameID( 968, 1002, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetAddonFrameID( AC_BIKE_1, AC_BIKE_1 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][6].Price = 120000;

	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].HName = "D-配付龋农";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].EName = "D-Tomahawk";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetFrameID( 969, 1003, 0 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetDropFrameID( FRAMEID_NULL );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetAddonFrameID( AC_BIKE_2, AC_BIKE_2 );
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].SetValue(400, 100);
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MOTORCYCLE][7].Price = 120000;

// d- 配付龋农鳖瘤 969, 1003
	//---------------------------------------------------------------------
	// ITEM_CLASS_POTION
	//---------------------------------------------------------------------
	// 雀汗荐摹(1)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_POTION, 18);

	m_pTypeInfo[ITEM_CLASS_POTION][0].HName = "固聪 鳃傅 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][0].EName = "Light Healing Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][0].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_HPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][0].SetFrameID( 0, 139, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][0].SetDropFrameID( 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][0].SetValue(40);
	m_pTypeInfo[ITEM_CLASS_POTION][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][0].Price	= 50;

	m_pTypeInfo[ITEM_CLASS_POTION][1].HName = "扼捞飘 鳃傅 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][1].EName = "Light Healing Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][1].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_HPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][1].SetFrameID( 0, 139, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][1].SetDropFrameID( 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][1].SetValue(40);
	m_pTypeInfo[ITEM_CLASS_POTION][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][1].Price	= 50;

	m_pTypeInfo[ITEM_CLASS_POTION][2].HName = "孽磐 鳃傅 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][2].EName = "Quarter Healing Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][2].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_HPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][2].SetFrameID( 1, 138, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][2].SetDropFrameID( 1 );
	m_pTypeInfo[ITEM_CLASS_POTION][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][2].SetValue(170);
	m_pTypeInfo[ITEM_CLASS_POTION][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][2].Price	= 125;

	m_pTypeInfo[ITEM_CLASS_POTION][3].HName = "窍橇 鳃傅 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][3].EName = "Half Healing Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][3].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_HPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][3].SetFrameID( 1, 138, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][3].SetDropFrameID( 1 );
	m_pTypeInfo[ITEM_CLASS_POTION][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][3].SetValue(170);
	m_pTypeInfo[ITEM_CLASS_POTION][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][3].Price	= 300;

	m_pTypeInfo[ITEM_CLASS_POTION][4].HName = "钱 鳃傅 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][4].EName = "Full Healing Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][4].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_HPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][4].SetFrameID( 2, 137, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][4].SetDropFrameID( 2 );
	m_pTypeInfo[ITEM_CLASS_POTION][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][4].SetValue(400);
	m_pTypeInfo[ITEM_CLASS_POTION][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][4].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][5].HName = "固聪 付唱 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][5].EName = "Light Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][5].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_MPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][5].SetFrameID( 3, 142, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][5].SetDropFrameID( 3 );
	m_pTypeInfo[ITEM_CLASS_POTION][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][5].SetValue(-1, 50);
	m_pTypeInfo[ITEM_CLASS_POTION][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][5].Price	= 50;

	m_pTypeInfo[ITEM_CLASS_POTION][6].HName = "扼捞飘 付唱 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][6].EName = "Light Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][6].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_MPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][6].SetFrameID( 3, 142, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][6].SetDropFrameID( 3 );
	m_pTypeInfo[ITEM_CLASS_POTION][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][6].SetValue(-1, 50);
	m_pTypeInfo[ITEM_CLASS_POTION][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][6].Price	= 50;

	m_pTypeInfo[ITEM_CLASS_POTION][7].HName = "孽磐 付唱 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][7].EName = "Half Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][7].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_MPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][7].SetFrameID( 4, 141, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][7].SetDropFrameID( 4 );
	m_pTypeInfo[ITEM_CLASS_POTION][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][7].SetValue(-1, 160);
	m_pTypeInfo[ITEM_CLASS_POTION][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][7].Price	= 125;

	m_pTypeInfo[ITEM_CLASS_POTION][8].HName = "窍橇 付唱 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][8].EName = "Half Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][8].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_MPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][8].SetFrameID( 4, 141, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][8].SetDropFrameID( 4 );
	m_pTypeInfo[ITEM_CLASS_POTION][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][8].SetValue(-1, 160);
	m_pTypeInfo[ITEM_CLASS_POTION][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][8].Price	= 300;

	m_pTypeInfo[ITEM_CLASS_POTION][9].HName = "钱 付唱 器记";
	m_pTypeInfo[ITEM_CLASS_POTION][9].EName = "Full Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][9].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_USE_MPOTION );
	m_pTypeInfo[ITEM_CLASS_POTION][9].SetFrameID( 5, 140, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][9].SetDropFrameID( 5 );
	m_pTypeInfo[ITEM_CLASS_POTION][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][9].SetValue(-1, 420);
	m_pTypeInfo[ITEM_CLASS_POTION][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][9].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][10].HName = "颇鄂 荤帕";
	m_pTypeInfo[ITEM_CLASS_POTION][10].EName = "Full Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][10].Description = "冈栏搁 塞捞 稼酒夸";
	m_pTypeInfo[ITEM_CLASS_POTION][10].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][10].SetFrameID( 243, 247, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][10].SetDropFrameID( 243 );
	m_pTypeInfo[ITEM_CLASS_POTION][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][10].SetValue(500, 500);
	m_pTypeInfo[ITEM_CLASS_POTION][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][10].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][11].HName = "闰祸 侗惫";
	m_pTypeInfo[ITEM_CLASS_POTION][11].EName = "Full Mana Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][11].Description = "冈栏搁 塞捞 稼酒夸";
	m_pTypeInfo[ITEM_CLASS_POTION][11].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][11].SetFrameID( 408, 422, 0 );//( 377, 391, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][11].SetDropFrameID( 408 );//( 377 );
	m_pTypeInfo[ITEM_CLASS_POTION][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][11].SetValue(500, 500);
	m_pTypeInfo[ITEM_CLASS_POTION][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][11].Price	= 750;

//	m_pTypeInfo[ITEM_CLASS_POTION][12].HName = "鳃傅 焊撇"
//	m_pTypeInfo[ITEM_CLASS_POTION][12].EName = "Healing Bottle";
//	m_pTypeInfo[ITEM_CLASS_POTION][12].Description = "";
//	m_pTypeInfo[ITEM_CLASS_POTION][12].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
//	m_pTypeInfo[ITEM_CLASS_POTION][12].SetFrameID( 421, 436, 0 );//( 377, 391, 0 );	
//	m_pTypeInfo[ITEM_CLASS_POTION][12].SetDropFrameID( 421 );//( 377 );
//	m_pTypeInfo[ITEM_CLASS_POTION][12].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_POTION][12].SetValue(500, 500);
//	m_pTypeInfo[ITEM_CLASS_POTION][12].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_POTION][12].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][12].HName = "窍橇 鳃傅 焊撇";
	m_pTypeInfo[ITEM_CLASS_POTION][12].EName = "Half Healing Bottle";
	m_pTypeInfo[ITEM_CLASS_POTION][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][12].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][12].SetFrameID( 422, 437, 0 );//( 377, 391, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][12].SetDropFrameID( 422 );//( 377 );
	m_pTypeInfo[ITEM_CLASS_POTION][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][12].SetValue(500, 500);
	m_pTypeInfo[ITEM_CLASS_POTION][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][12].Price	= 750;

//	m_pTypeInfo[ITEM_CLASS_POTION][14].HName = "付唱 焊撇";
//	m_pTypeInfo[ITEM_CLASS_POTION][14].EName = "Mana Bottle";
//	m_pTypeInfo[ITEM_CLASS_POTION][14].Description = "";
//	m_pTypeInfo[ITEM_CLASS_POTION][14].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
//	m_pTypeInfo[ITEM_CLASS_POTION][14].SetFrameID( 423, 438, 0 );//( 377, 391, 0 );	
//	m_pTypeInfo[ITEM_CLASS_POTION][14].SetDropFrameID( 423 );//( 377 );
//	m_pTypeInfo[ITEM_CLASS_POTION][14].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_POTION][14].SetValue(500, 500);
//	m_pTypeInfo[ITEM_CLASS_POTION][14].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_POTION][14].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][13].HName = "窍橇 付唱 焊撇";
	m_pTypeInfo[ITEM_CLASS_POTION][13].EName = "Half Mana Bottle";
	m_pTypeInfo[ITEM_CLASS_POTION][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][13].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][13].SetFrameID( 424, 439, 0 );//( 377, 391, 0 );	
	m_pTypeInfo[ITEM_CLASS_POTION][13].SetDropFrameID( 424 );//( 377 );
	m_pTypeInfo[ITEM_CLASS_POTION][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][13].SetValue(500, 500);
	m_pTypeInfo[ITEM_CLASS_POTION][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][13].Price	= 750;

	m_pTypeInfo[ITEM_CLASS_POTION][14].HName = "胶隔 圈府器记";
	m_pTypeInfo[ITEM_CLASS_POTION][14].EName = "Small Holy Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][14].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][14].SetFrameID( 764,778, 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][14].SetDropFrameID( 764 );
	m_pTypeInfo[ITEM_CLASS_POTION][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][14].SetValue(500,500 );
	m_pTypeInfo[ITEM_CLASS_POTION][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][14].Price= 0;

	m_pTypeInfo[ITEM_CLASS_POTION][15].HName = "固点 圈府器记";
	m_pTypeInfo[ITEM_CLASS_POTION][15].EName = "Medium Holy Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][15].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][15].SetFrameID( 757,771, 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][15].SetDropFrameID( 757 );
	m_pTypeInfo[ITEM_CLASS_POTION][15].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][15].SetValue(500,500 );
	m_pTypeInfo[ITEM_CLASS_POTION][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][15].Price= 0;

	m_pTypeInfo[ITEM_CLASS_POTION][16].HName = "扼瘤 圈府器记";
	m_pTypeInfo[ITEM_CLASS_POTION][16].EName = "Large Holy Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][16].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][16].SetFrameID( 755,769, 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][16].SetDropFrameID( 755 );
	m_pTypeInfo[ITEM_CLASS_POTION][16].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][16].SetValue(500,500 );
	m_pTypeInfo[ITEM_CLASS_POTION][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][16].Price= 0;

	m_pTypeInfo[ITEM_CLASS_POTION][17].HName = "钱 圈府器记";
	m_pTypeInfo[ITEM_CLASS_POTION][17].EName = "Full Holy Potion";
	m_pTypeInfo[ITEM_CLASS_POTION][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_POTION][17].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_POTION][17].SetFrameID( 753,767, 0 );
	m_pTypeInfo[ITEM_CLASS_POTION][17].SetDropFrameID( 753 );
	m_pTypeInfo[ITEM_CLASS_POTION][17].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_POTION][17].SetValue(500,500 );
	m_pTypeInfo[ITEM_CLASS_POTION][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_POTION][17].Price= 0;

	//---------------------------------------------------------------------
	// ITEM_CLASS_WATER
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_WATER, 7 );

	m_pTypeInfo[ITEM_CLASS_WATER][0].HName = "窍橇 况磐 官撇";
	m_pTypeInfo[ITEM_CLASS_WATER][0].EName = "Half Water Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][0].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][0].SetFrameID( 6, 50, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][0].SetDropFrameID( 6 );
	m_pTypeInfo[ITEM_CLASS_WATER][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][0].Price	= 5;

	m_pTypeInfo[ITEM_CLASS_WATER][1].HName = "钱 况磐 官撇";
	m_pTypeInfo[ITEM_CLASS_WATER][1].EName = "Full Water Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][1].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][1].SetFrameID( 7, 52, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][1].SetDropFrameID( 7 );
	m_pTypeInfo[ITEM_CLASS_WATER][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][1].Price	= 15;

	m_pTypeInfo[ITEM_CLASS_WATER][2].HName = "扼瘤 况磐 官撇";
	m_pTypeInfo[ITEM_CLASS_WATER][2].EName = "Large Water Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][2].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][2].SetFrameID( 8, 51, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][2].SetDropFrameID( 8 );
	m_pTypeInfo[ITEM_CLASS_WATER][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][2].Price	= 40;

	// 器记捍狼 value2 俊绰 potion item class 狼 itemtype 捞 甸绢埃促.
	m_pTypeInfo[ITEM_CLASS_WATER][3].HName = "胶隔 器记捍";
	m_pTypeInfo[ITEM_CLASS_WATER][3].EName = "Small Potion Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][3].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][3].SetFrameID( 765, 779, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][3].SetDropFrameID( 765 );
	m_pTypeInfo[ITEM_CLASS_WATER][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][3].Value3 = 14;
	m_pTypeInfo[ITEM_CLASS_WATER][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][3].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_WATER][4].HName = "固点 器记捍";
	m_pTypeInfo[ITEM_CLASS_WATER][4].EName = "Medium Potion Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][4].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][4].SetFrameID( 758, 772, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][4].SetDropFrameID( 758 );
	m_pTypeInfo[ITEM_CLASS_WATER][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][4].Value3 = 15;
	m_pTypeInfo[ITEM_CLASS_WATER][4].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_WATER][5].HName = "扼瘤 器记捍";
	m_pTypeInfo[ITEM_CLASS_WATER][5].EName = "Large Potion Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][5].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][5].SetFrameID( 756, 770, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][5].SetDropFrameID( 756 );
	m_pTypeInfo[ITEM_CLASS_WATER][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][5].Value3 = 16;
	m_pTypeInfo[ITEM_CLASS_WATER][5].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_WATER][6].HName = "钱 器记捍";
	m_pTypeInfo[ITEM_CLASS_WATER][6].EName = "Full Potion Bottle";
	m_pTypeInfo[ITEM_CLASS_WATER][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_WATER][6].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_WATER][6].SetFrameID( 754, 768, 0 );	
	m_pTypeInfo[ITEM_CLASS_WATER][6].SetDropFrameID( 754 );
	m_pTypeInfo[ITEM_CLASS_WATER][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WATER][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WATER][6].Value3 = 17;
	m_pTypeInfo[ITEM_CLASS_WATER][6].Price	= 40;

	//---------------------------------------------------------------------
	// ITEM_CLASS_HOLYWATER
	//---------------------------------------------------------------------
	// MinDam(1)~MaxDam(2)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_HOLYWATER, 3 );
	
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].HName = "孽磐 圈府 况磐";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].EName = "Quarter Holy Water";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].SetFrameID( 9, 101, 0 );	
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].SetDropFrameID( 9 );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].SetValue(10, 20);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].HName = "窍橇 圈府 况磐";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].EName = "Half Holy Water";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].SetFrameID( 10, 100, 0 );	
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].SetDropFrameID( 10 );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].SetValue(15, 30);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].HName = "钱 圈府 况磐";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].EName = "Full Holy Water";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].SetSoundID( SOUND_ITEM_MOVE_BOTTLE, SOUND_ITEM_MOVE_BOTTLE, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].SetFrameID( 11, 99, 0 );	
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].SetDropFrameID( 11 );
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].SetValue(20, 40);
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HOLYWATER][2].Price	= 0;

	//---------------------------------------------------------------------
	// ITEM_CLASS_MAGAZINE
	//---------------------------------------------------------------------
	// 醚Class(1), Max藕芒荐(2)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_MAGAZINE, 24 );

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].HName = "SG Shell-8";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].EName = "SGS-1";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].SetFrameID( 17, 126, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].SetDropFrameID( 17 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].SetValue(ITEM_CLASS_SG, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][0].Price	= 10;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].HName = "SG Shell-16";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].EName = "ARM-15";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].SetFrameID( 254, 267, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].SetDropFrameID( 254 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].SetValue(ITEM_CLASS_SG, 15);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][1].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].HName = "AR Magazine-20";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].EName = "SMGM-60";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].SetFrameID( 14, 127, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].SetDropFrameID( 14 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].SetValue(ITEM_CLASS_AR, 60);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][2].Price	= 70;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].HName = "AR Magazine-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].EName = "SMGM-30";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].SetFrameID( 14, 127, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].SetDropFrameID( 14 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].SetValue(ITEM_CLASS_AR, 30);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][3].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].HName = "SMG Magazine-60";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].EName = "TRM-5";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].SetFrameID( 16, 130, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].SetDropFrameID( 16 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].SetValue(ITEM_CLASS_SMG, 5);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][4].Price	= 30;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].HName = "SMG Magazine-120";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].EName = "TRM-8";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].SetFrameID( 15, 131, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].SetDropFrameID( 15 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].SetValue(ITEM_CLASS_SMG, 8);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][5].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].HName = "SR Magazine-10";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].EName = "SGS-6";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].SetFrameID( 13, 129, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].SetDropFrameID( 13 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].SetValue(ITEM_CLASS_SR, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][6].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].HName = "SR Magazine-20";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].EName = "SGS-6";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].SetFrameID( 12, 128, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].SetDropFrameID( 12 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].SetValue(ITEM_CLASS_SR, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][7].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].HName = "Silver SG Shell-8";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].EName = "SGS-1";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].SetFrameID( 17, 126, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].SetDropFrameID( 17 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].SetValue(ITEM_CLASS_SG, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][8].Price	= 10;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].HName = "Silver SG Shell-16";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].EName = "ARM-15";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].SetFrameID( 254, 267, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].SetDropFrameID( 254 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].SetValue(ITEM_CLASS_SG, 15);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][9].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].HName = "Silver AR Magazine-20";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].EName = "SMGM-60";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].SetFrameID( 14, 127, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].SetDropFrameID( 14 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].SetValue(ITEM_CLASS_AR, 60);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][10].Price	= 70;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].HName = "Silver AR Magazine-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].EName = "SMGM-30";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].SetFrameID( 14, 127, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].SetDropFrameID( 14 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].SetValue(ITEM_CLASS_AR, 30);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][11].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].HName = "Silver SMG Magazine-60";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].EName = "TRM-5";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].SetFrameID( 16, 130, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].SetDropFrameID( 16 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].SetValue(ITEM_CLASS_SMG, 5);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][12].Price	= 30;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].HName = "Silver SMG Magazine-120";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].EName = "TRM-8";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].SetFrameID( 15, 131, 0 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].SetDropFrameID( 15 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].SetValue(ITEM_CLASS_SMG, 8);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][13].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].HName = "Silver SR Magazine-10";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].EName = "SGS-6";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].SetFrameID( 13, 129, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].SetDropFrameID( 13 );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].SetValue(ITEM_CLASS_SR, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][14].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].HName = "Silver SR Magazine-20";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].EName = "SGS-6";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].SetFrameID( 12, 128, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].SetDropFrameID( 12 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].SetValue(ITEM_CLASS_SR, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][15].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].HName = "SG 僵-32";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].EName = "SG Shell-32";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].SetFrameID( 759, 773, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].SetDropFrameID( 759 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].SetValue(ITEM_CLASS_SG, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][16].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].HName = "AR 藕芒-80";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].EName = "AR Magazine-80";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].SetFrameID( 752, 766, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].SetDropFrameID( 752 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].SetValue(ITEM_CLASS_AR, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][17].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].HName = "SMG 藕芒-160";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].EName = "SMG Magazine-160";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].SetFrameID( 766, 780, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].SetDropFrameID( 766 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].SetValue(ITEM_CLASS_SMG, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][18].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].HName = "SR 藕芒-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].EName = "SR Magazine-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].SetFrameID( 767, 781, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].SetDropFrameID( 767 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].SetValue(ITEM_CLASS_SR, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][19].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].HName = "篮力 SG 僵-32";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].EName = "Silver SG Shell-32";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].SetFrameID( 761, 775, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].SetDropFrameID( 761 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].SetValue(ITEM_CLASS_SG, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][20].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].HName = "篮力 AR 藕芒-80";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].EName = "Silver AR Magazine-80";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].SetFrameID( 760, 774, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].SetDropFrameID( 760 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].SetValue(ITEM_CLASS_AR, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][21].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].HName = "篮力 SMG 藕芒-160";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].EName = "Silver SMG Magazine-160";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].SetFrameID( 762, 776, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].SetDropFrameID( 762 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].SetValue(ITEM_CLASS_SMG, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][22].Price	= 40;

	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].HName = "篮力 SR 藕芒-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].EName = "Silver SR Magazine-40";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].SetSoundID( SOUND_ITEM_MOVE_MAGAZINE, SOUND_ITEM_MOVE_MAGAZINE, SOUNDID_NULL, SOUND_ITEM_USE_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].SetFrameID( 763, 777, 0 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].SetDropFrameID( 763 );	
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].SetValue(ITEM_CLASS_SR, 1, SKILL_VIVID_MAGAZINE );
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MAGAZINE][23].Price	= 40;

	//---------------------------------------------------------------------
	// ITEM_CLASS_BOMB_MATERIAL
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_BOMB_MATERIAL, 10 );

	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].HName = "胶敲赴磐 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].EName = "Splinter Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].SetFrameID( 245, 249, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].SetDropFrameID( 245 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][0].Price	= 100;

	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].HName = "俊捞辑 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].EName = "Acer Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].SetFrameID( 246, 251, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].SetDropFrameID( 246 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][1].Price	= 100;

	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].HName = "阂胶 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].EName = "Bulls Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].SetFrameID( 247, 253, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].SetDropFrameID( 247 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][2].Price	= 100;


	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].HName = "胶畔 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].EName = "Stun Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].SetFrameID( 252, 263, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].SetDropFrameID( 252 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][3].Price	= 100;


	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].HName = "农肺胶焊快 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].EName = "Crossbow Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].SetFrameID( 248, 255, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].SetDropFrameID( 248 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][4].Price	= 100;


	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].HName = "巨努懦矾 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].EName = "AnkleKiller Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].SetFrameID( 270, 283, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].SetDropFrameID( 270 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][5].Price	= 100;

	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].HName = "汽令 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].EName = "Pomz Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].SetFrameID( 249, 257, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].SetDropFrameID( 249 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][6].Price	= 100;


	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].HName = "AP-C1 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].EName = "AP-C1 Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].SetFrameID( 253, 266, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].SetDropFrameID( 253 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][7].Price	= 100;


	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].HName = "促捞酒阁靛 归";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].EName = "DiamondBack Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].SetFrameID( 251, 261, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].SetDropFrameID( 251 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][8].Price	= 100;

	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].HName = "Swift-EX 犁丰";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].EName = "Swift-EX Material";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].SetFrameID( 250, 259, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].SetDropFrameID( 250 );
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB_MATERIAL][9].Price	= 100;

	//---------------------------------------------------------------------
	// ITEM_CLASS_ETC
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_ETC, 2 );

	m_pTypeInfo[ITEM_CLASS_ETC][0].HName = "傍备 冠胶";
	m_pTypeInfo[ITEM_CLASS_ETC][0].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_ETC][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_ETC][0].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_ETC][0].SetFrameID( 19, 25, 0 );	
	m_pTypeInfo[ITEM_CLASS_ETC][0].SetDropFrameID( 19 );
	m_pTypeInfo[ITEM_CLASS_ETC][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_ETC][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_ETC][0].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_ETC][1].HName = "1雀侩 锅开扁";
	m_pTypeInfo[ITEM_CLASS_ETC][1].EName = "Slayer Translator";
	m_pTypeInfo[ITEM_CLASS_ETC][1].Description = "10盒埃 促弗 辆练狼 措拳甫 葛滴 舅酒 甸阑 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_ETC][1].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_ETC][1].SetFrameID( 579, 593, 0 );
	m_pTypeInfo[ITEM_CLASS_ETC][1].SetDropFrameID( 579 );
	m_pTypeInfo[ITEM_CLASS_ETC][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_ETC][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_ETC][1].Price = 1;

	//---------------------------------------------------------------------
	// ITEM_CLASS_KEY
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_KEY, 14 );

	m_pTypeInfo[ITEM_CLASS_KEY][0].HName = "辨飘 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][0].EName = "Gilt Key";
	m_pTypeInfo[ITEM_CLASS_KEY][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][0].SetSoundID( SOUND_ITEM_MOVE_KEY, SOUND_ITEM_MOVE_KEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][0].SetFrameID( 23, 112, 0 );
	m_pTypeInfo[ITEM_CLASS_KEY][0].SetDropFrameID( 23 );	
	m_pTypeInfo[ITEM_CLASS_KEY][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_KEY][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_KEY][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_KEY][1].HName = "榜靛 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][1].EName = "Gold Key";
	m_pTypeInfo[ITEM_CLASS_KEY][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][1].SetSoundID( SOUND_ITEM_MOVE_KEY, SOUND_ITEM_MOVE_KEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][1].SetFrameID( 21, 113, 0 );	
	m_pTypeInfo[ITEM_CLASS_KEY][1].SetDropFrameID( 21 );
	m_pTypeInfo[ITEM_CLASS_KEY][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_KEY][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_KEY][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_KEY][2].HName = "付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][2].EName = "Margnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][2].SetSoundID( SOUND_ITEM_MOVE_KEY, SOUND_ITEM_MOVE_KEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][2].SetFrameID( 22, 115, 0 );	
	m_pTypeInfo[ITEM_CLASS_KEY][2].SetDropFrameID( 22 );
	m_pTypeInfo[ITEM_CLASS_KEY][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_KEY][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_KEY][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_KEY][3].HName = "胶其既 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][3].EName = "Special Key";
	m_pTypeInfo[ITEM_CLASS_KEY][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][3].SetSoundID( SOUND_ITEM_MOVE_KEY, SOUND_ITEM_MOVE_KEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][3].SetFrameID( 20, 114, 0 );	
	m_pTypeInfo[ITEM_CLASS_KEY][3].SetDropFrameID( 20 );
	m_pTypeInfo[ITEM_CLASS_KEY][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_KEY][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_KEY][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_KEY][4].HName = "檬废祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][4].EName = "Green Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][4].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][4].SetFrameID( 709,723,0);
	m_pTypeInfo[ITEM_CLASS_KEY][4].SetDropFrameID( 709 );

	m_pTypeInfo[ITEM_CLASS_KEY][5].HName = "颇鄂祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][5].EName = "Blue Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][5].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][5].SetFrameID( 712,726,0);
	m_pTypeInfo[ITEM_CLASS_KEY][5].SetDropFrameID( 712 );

	m_pTypeInfo[ITEM_CLASS_KEY][6].HName = "弧埃祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][6].EName = "Red Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][6].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][6].SetFrameID( 713,727,0);
	m_pTypeInfo[ITEM_CLASS_KEY][6].SetDropFrameID( 713 );

	m_pTypeInfo[ITEM_CLASS_KEY][7].HName = "畴鄂祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][7].EName = "Yellow Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][7].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][7].SetFrameID( 714,728,0);
	m_pTypeInfo[ITEM_CLASS_KEY][7].SetDropFrameID( 714 );

	m_pTypeInfo[ITEM_CLASS_KEY][8].HName = "林炔祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][8].EName = "Orange Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][8].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][8].SetFrameID( 715,729,0);
	m_pTypeInfo[ITEM_CLASS_KEY][8].SetDropFrameID( 715 );

	m_pTypeInfo[ITEM_CLASS_KEY][9].HName = "窍疵祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][9].EName = "Skyblue Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][9].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][9].SetFrameID( 716,730,0);
	m_pTypeInfo[ITEM_CLASS_KEY][9].SetDropFrameID( 716 );

	m_pTypeInfo[ITEM_CLASS_KEY][10].HName = "焊扼祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][10].EName = "Violet Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][10].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][10].SetFrameID( 717,731,0);
	m_pTypeInfo[ITEM_CLASS_KEY][10].SetDropFrameID( 717 );

	m_pTypeInfo[ITEM_CLASS_KEY][11].HName = "八篮祸 付弊齿 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][11].EName = "Black Magnet Key";
	m_pTypeInfo[ITEM_CLASS_KEY][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][11].SetSoundID( SOUND_ITEM_MOVE_KEY,SOUND_ITEM_MOVE_KEY,SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][11].SetFrameID( 718,732,0);
	m_pTypeInfo[ITEM_CLASS_KEY][11].SetDropFrameID( 718 );



	m_pTypeInfo[ITEM_CLASS_KEY][12].HName = "B2-阁胶磐 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][12].EName = "B2-Monster Key";
	m_pTypeInfo[ITEM_CLASS_KEY][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][12].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][12].SetFrameID( 966, 1002, 0 );
	m_pTypeInfo[ITEM_CLASS_KEY][12].SetDropFrameID( FRAMEID_NULL );
	
	m_pTypeInfo[ITEM_CLASS_KEY][13].HName = "D-配付龋农 虐";
	m_pTypeInfo[ITEM_CLASS_KEY][13].EName = "D-Tomahawk Key";
	m_pTypeInfo[ITEM_CLASS_KEY][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_KEY][13].SetSoundID( SOUND_WORLD_BIKE_STOP, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_KEY][13].SetFrameID( 967, 1003, 0 );
	m_pTypeInfo[ITEM_CLASS_KEY][13].SetDropFrameID( FRAMEID_NULL );
	

	//---------------------------------------------------------------------
	// ITEM_CLASS_RING
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_RING, 14 );

	m_pTypeInfo[ITEM_CLASS_RING][0].HName = "宏沸令 傅";
	m_pTypeInfo[ITEM_CLASS_RING][0].EName = "Bronze Ring";
	m_pTypeInfo[ITEM_CLASS_RING][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][0].SetFrameID( 27, 53, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][0].SetDropFrameID( 27 );
	m_pTypeInfo[ITEM_CLASS_RING][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][0].SetValue(80, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][0].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][0].SetRequireAbility(0,0, 20);
	m_pTypeInfo[ITEM_CLASS_RING][0].Price = 500;

	m_pTypeInfo[ITEM_CLASS_RING][1].HName = "喉发 傅";
	m_pTypeInfo[ITEM_CLASS_RING][1].EName = "Black Ring";
	m_pTypeInfo[ITEM_CLASS_RING][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][1].SetFrameID( 25, 54, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][1].SetDropFrameID( 25 );
	m_pTypeInfo[ITEM_CLASS_RING][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][1].SetValue(100, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][1].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][1].SetRequireAbility(0,0, 40);
	m_pTypeInfo[ITEM_CLASS_RING][1].Price = 1000;

	m_pTypeInfo[ITEM_CLASS_RING][2].HName = "橇府胶飘 傅";
	m_pTypeInfo[ITEM_CLASS_RING][2].EName = "Priest Ring";
	m_pTypeInfo[ITEM_CLASS_RING][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][2].SetFrameID( 26, 55, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][2].SetDropFrameID( 26 );
	m_pTypeInfo[ITEM_CLASS_RING][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][2].SetValue(150, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][2].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][2].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_RING][2].Price = 1600;

	m_pTypeInfo[ITEM_CLASS_RING][3].HName = "榜靛 傅";
	m_pTypeInfo[ITEM_CLASS_RING][3].EName = "Gold Ring";
	m_pTypeInfo[ITEM_CLASS_RING][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][3].SetFrameID( 24, 56, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][3].SetDropFrameID( 24 );
	m_pTypeInfo[ITEM_CLASS_RING][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][3].SetValue(200, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][3].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][3].SetRequireAbility(0,0, 80);
	m_pTypeInfo[ITEM_CLASS_RING][3].Price = 6000;

	m_pTypeInfo[ITEM_CLASS_RING][4].HName = "坷矾胶 傅";
	m_pTypeInfo[ITEM_CLASS_RING][4].EName = "Aurous Ring";
	m_pTypeInfo[ITEM_CLASS_RING][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][4].SetFrameID( 28, 57, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][4].SetDropFrameID( 28 );
	m_pTypeInfo[ITEM_CLASS_RING][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][4].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][4].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][4].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][4].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][5].HName = "青款狼 馆瘤";
	m_pTypeInfo[ITEM_CLASS_RING][5].EName = "Lucky Ring";
	m_pTypeInfo[ITEM_CLASS_RING][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][5].SetFrameID( 153, 157, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][5].SetDropFrameID( 153 );
	m_pTypeInfo[ITEM_CLASS_RING][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][5].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][5].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][5].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][5].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][6].HName = "肺磊府坷";
	m_pTypeInfo[ITEM_CLASS_RING][6].EName = "Rosario";
	m_pTypeInfo[ITEM_CLASS_RING][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][6].SetFrameID( 152, 156, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][6].SetDropFrameID( 152 );
	m_pTypeInfo[ITEM_CLASS_RING][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][6].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][6].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][6].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][6].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][7].HName = "榜电 促捞酒阁靛 傅";
	m_pTypeInfo[ITEM_CLASS_RING][7].EName = "Golden Diamond Ring";
	m_pTypeInfo[ITEM_CLASS_RING][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][7].SetFrameID( 154, 158, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][7].SetDropFrameID( 154 );
	m_pTypeInfo[ITEM_CLASS_RING][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][7].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][7].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][7].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][7].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][8].HName = "敲贰萍逞 胶颇捞凡";
	m_pTypeInfo[ITEM_CLASS_RING][8].EName = "Platinum Spiral";
	m_pTypeInfo[ITEM_CLASS_RING][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][8].SetFrameID( 151, 155, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][8].SetDropFrameID( 151 );
	m_pTypeInfo[ITEM_CLASS_RING][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][8].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][8].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][8].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][8].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][9].HName = "巨农 傅";
	m_pTypeInfo[ITEM_CLASS_RING][9].EName = "Ankh Ring";
	m_pTypeInfo[ITEM_CLASS_RING][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][9].SetFrameID( 155, 159, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][9].SetDropFrameID( 155 );
	m_pTypeInfo[ITEM_CLASS_RING][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][9].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][9].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][9].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][9].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][10].HName = "坷其府坷 傅";
	m_pTypeInfo[ITEM_CLASS_RING][10].EName = "Operio Ring";
	m_pTypeInfo[ITEM_CLASS_RING][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][10].SetFrameID( 355, 369, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][10].SetDropFrameID( 355 );
	m_pTypeInfo[ITEM_CLASS_RING][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][10].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][10].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][10].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][10].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][11].HName = "捞胶评 傅";
	m_pTypeInfo[ITEM_CLASS_RING][11].EName = "Esteem Ring";
	m_pTypeInfo[ITEM_CLASS_RING][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][11].SetFrameID( 453, 467, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][11].SetDropFrameID( 453 );
	m_pTypeInfo[ITEM_CLASS_RING][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][11].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][11].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][11].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][11].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][12].HName = "纳杭饭鸥 傅";
	m_pTypeInfo[ITEM_CLASS_RING][12].EName = "Kabollefa Ring";
	m_pTypeInfo[ITEM_CLASS_RING][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][12].SetFrameID( 481, 495, 0 );	
	m_pTypeInfo[ITEM_CLASS_RING][12].SetDropFrameID( 481 );
	m_pTypeInfo[ITEM_CLASS_RING][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][12].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][12].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][12].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][12].Price = 15000;

	m_pTypeInfo[ITEM_CLASS_RING][13].HName = "杭扼矫坷 傅";
	m_pTypeInfo[ITEM_CLASS_RING][13].EName = "Volasio";
	m_pTypeInfo[ITEM_CLASS_RING][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_RING][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_RING][13].SetFrameID( 917 , 940 ,0 );	
	m_pTypeInfo[ITEM_CLASS_RING][13].SetDropFrameID( 917 );
	m_pTypeInfo[ITEM_CLASS_RING][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_RING][13].SetValue(300, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_RING][13].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_RING][12].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_RING][13].Price = 15000;
	//---------------------------------------------------------------------
	// ITEM_CLASS_BRACELET
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_BRACELET, 14 );

	m_pTypeInfo[ITEM_CLASS_BRACELET][0].HName = "皋呕 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].EName = "Metal Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetFrameID( 29, 132, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetDropFrameID( 29 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetValue(100, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][0].SetRequireAbility(0,0, 20);
	m_pTypeInfo[ITEM_CLASS_BRACELET][0].Price = 750;

	m_pTypeInfo[ITEM_CLASS_BRACELET][1].HName = "胶瓶 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].EName = "Steel Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetFrameID( 31, 133, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetDropFrameID( 31 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetValue(120, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][1].SetRequireAbility(0,0, 40);
	m_pTypeInfo[ITEM_CLASS_BRACELET][1].Price = 1200;

	m_pTypeInfo[ITEM_CLASS_BRACELET][2].HName = "傅 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].EName = "Ring Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetFrameID( 30, 134, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetDropFrameID( 30 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetValue(170, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][2].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_BRACELET][2].Price = 2000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][3].HName = "敲饭捞飘 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].EName = "Plate Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetFrameID( 32, 135, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetDropFrameID( 32 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetValue(250, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][3].SetRequireAbility(0,0, 80);
	m_pTypeInfo[ITEM_CLASS_BRACELET][3].Price = 8000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][4].HName = "坷矾胶 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetFrameID( 33, 136, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetDropFrameID( 33 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][4].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][4].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][5].HName = "肺凡 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetFrameID( 157, 161, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetDropFrameID( 157 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][5].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][5].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][6].HName = "块狼 迫骂";
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetFrameID( 156, 160, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetDropFrameID( 156 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][6].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][6].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][7].HName = "浇饭捞宏 桂臂";
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetFrameID( 160, 164, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetDropFrameID( 160 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][7].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][7].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][8].HName = "农肺胶 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetFrameID( 159, 163, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetDropFrameID( 159 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][8].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][8].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][9].HName = "圈府 胶乔府飘";
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].EName = "Aurous Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetFrameID( 158, 162, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetDropFrameID( 158 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][9].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][9].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][10].HName = "酒宁扼 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].EName = "Aquila Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetFrameID( 354, 368, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetDropFrameID( 354 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][10].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][10].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][11].HName = "贾府胶 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].EName = "Solis bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetFrameID( 452, 466, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetDropFrameID( 452 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][11].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][11].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_BRACELET][12].HName = "淬乔福 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].EName = "Dhampir Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetFrameID( 480, 494, 0 );	
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetDropFrameID( 480 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][12].Price = 20000;

		m_pTypeInfo[ITEM_CLASS_BRACELET][13].HName = "农风胶葱 宏饭捞浇";
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].EName = "Krsnik Bracelet";
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].SetFrameID( 932 , 955 ,0);	
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].SetDropFrameID( 932 );
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].SetValue(350, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_BRACELET][12].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_BRACELET][13].Price = 20000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_NECKLACE
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_NECKLACE, 14 );

	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].HName = "农肺胶 埔带飘";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].EName = "Cross Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetFrameID( 36, 45, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetDropFrameID( 36 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetValue(80, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][0].SetRequireAbility(0,0, 20);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][0].Price = 1000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].HName = "俊赣凡靛 埔带飘";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].EName = "Emerald Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetFrameID( 38, 46, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetDropFrameID( 38 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetValue(95, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][1].SetRequireAbility(0,0, 40);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][1].Price = 2000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].HName = "敲饭萍逞 埔带飘";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].EName = "Platinum Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetFrameID( 35, 47, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetDropFrameID( 35 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetValue(120, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][2].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][2].Price = 7800;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].HName = "坷矾胶 埔带飘";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].EName = "Aurous Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetFrameID( 37, 48, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetDropFrameID( 37 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetValue(150, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][3].SetRequireAbility(0,0, 80);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][3].Price = 20000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].HName = "农饭季飘 埔带飘";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetFrameID( 34, 49, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetDropFrameID( 34 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][4].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][4].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].HName = "喉风 季胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetFrameID( 161, 165, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetDropFrameID( 161 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][5].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][5].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].HName = "牢府 农肺胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetFrameID( 163, 167, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetDropFrameID( 163 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][6].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][6].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].HName = "何劝狼 喊";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetFrameID( 164, 168, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetDropFrameID( 164 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][7].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][7].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].HName = "喉矾靛 农肺胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetFrameID( 165, 169, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetDropFrameID( 165 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][8].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][8].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].HName = "角滚 辑努复";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].EName = "Crescent Pandent";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetFrameID( 162, 166, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetDropFrameID( 162 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][9].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][9].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].HName = "赣揪 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].EName = "Mercy Necklace";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetFrameID( 353, 367, 0 );	
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetDropFrameID( 353 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetValue(200, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].Weight = 1;
	//m_pTypeInfo[ITEM_CLASS_NECKLACE][10].SetRequireAbility(0,0, 90);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][10].Price = 50000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].HName = "飘府聪萍 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].EName = "trinity necklace";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].SetFrameID( 451, 465, 0 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].SetDropFrameID( 451 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_NECKLACE][11].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].HName = "葛扒 福 其捞";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].EName = "Morgan le Fay";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].SetFrameID( 482, 496, 0 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].SetDropFrameID( 482 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_NECKLACE][12].Price	= 150000;

		m_pTypeInfo[ITEM_CLASS_NECKLACE][13].HName = "扼付胶 匙努府胶";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].EName = "Lamassu Necklace";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].SetFrameID( 912 , 935 ,0 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].SetDropFrameID( 909 );
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_NECKLACE][13].Price	= 150000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_COAT
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	// 121 惑狼 巢备
	// 122 惑狼 咯备
	// 123 惑狼 巢脚
	// 124 惑狼 咯脚
	InitClass(ITEM_CLASS_COAT, 26 );

	m_pTypeInfo[ITEM_CLASS_COAT][0].HName = "巢磊 敲发 犁哦";
	m_pTypeInfo[ITEM_CLASS_COAT][0].EName = "Combat Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetFrameID( 121, 26, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetDropFrameID( 121 );
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetAddonFrameID( ADDONID_COAT1_MALE, ADDONID_COAT1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][0].SetValue(200, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][0].Price = 1500;

	m_pTypeInfo[ITEM_CLASS_COAT][1].HName = "咯磊 敲发 犁哦";
	m_pTypeInfo[ITEM_CLASS_COAT][1].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetFrameID( 122, 106, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetDropFrameID( 122 );
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetAddonFrameID( ADDONID_COAT1_MALE, ADDONID_COAT1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][1].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][1].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][2].HName = "巢磊 海胶飘";
	m_pTypeInfo[ITEM_CLASS_COAT][2].EName = "Combat Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetFrameID( 121, 26, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetDropFrameID( 121 );
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetAddonFrameID( ADDONID_COAT1_MALE, ADDONID_COAT1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][2].SetValue(200, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][2].Price = 1500;

	m_pTypeInfo[ITEM_CLASS_COAT][3].HName = "咯磊 海胶飘";
	m_pTypeInfo[ITEM_CLASS_COAT][3].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetFrameID( 122, 106, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetDropFrameID( 122 );
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetAddonFrameID( ADDONID_COAT1_MALE, ADDONID_COAT1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][3].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][3].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][4].HName = "巢磊 硅撇酱飘";
	m_pTypeInfo[ITEM_CLASS_COAT][4].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetFrameID( 123, 29, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetDropFrameID( 123 );
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetAddonFrameID( ADDONID_COAT2_MALE, ADDONID_COAT2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][4].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][4].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][5].HName = "咯磊 硅撇酱飘";
	m_pTypeInfo[ITEM_CLASS_COAT][5].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetFrameID( 124, 109, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetDropFrameID( 124 );
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetAddonFrameID( ADDONID_COAT2_MALE, ADDONID_COAT2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][5].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][5].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][6].HName = "巢磊 哪诡酱飘";
	m_pTypeInfo[ITEM_CLASS_COAT][6].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetFrameID( 123, 29, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetDropFrameID( 123 );
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetAddonFrameID( ADDONID_COAT2_MALE, ADDONID_COAT2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][6].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][6].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][7].HName = "咯磊 哪诡酱飘";
	m_pTypeInfo[ITEM_CLASS_COAT][7].EName = "Plate Mail";
	m_pTypeInfo[ITEM_CLASS_COAT][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetFrameID( 124, 109, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetDropFrameID( 124 );
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetAddonFrameID( ADDONID_COAT2_MALE, ADDONID_COAT2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][7].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][7].Price = 2500;
// 咯扁何磐 3窜渴
	
	m_pTypeInfo[ITEM_CLASS_COAT][8].HName = "硅撇 皋老";
	m_pTypeInfo[ITEM_CLASS_COAT][8].EName = "Battle Mail M";
	m_pTypeInfo[ITEM_CLASS_COAT][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetFrameID( 279, 293, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetDropFrameID( 279 );
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][8].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][8].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][9].HName = "硅撇 皋老";
	m_pTypeInfo[ITEM_CLASS_COAT][9].EName = "Battle Mail W";
	m_pTypeInfo[ITEM_CLASS_COAT][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetFrameID( 287, 301, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetDropFrameID( 287 );
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][9].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][9].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][10].HName = "哪诡 皋老";
	m_pTypeInfo[ITEM_CLASS_COAT][10].EName = "Combat Mail M";
	m_pTypeInfo[ITEM_CLASS_COAT][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetFrameID( 279, 293, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetDropFrameID( 279 );
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][10].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][10].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][11].HName = "哪诡 皋老";
	m_pTypeInfo[ITEM_CLASS_COAT][11].EName = "Combat Mail W";
	m_pTypeInfo[ITEM_CLASS_COAT][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetFrameID( 287, 301, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetDropFrameID( 287 );
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][11].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][11].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][12].HName = "哪诡 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][12].EName = "Combat Armor M";
	m_pTypeInfo[ITEM_CLASS_COAT][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetFrameID( 280, 294, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetDropFrameID( 280 );
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][12].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][12].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][13].HName = "哪诡 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][13].EName = "Combat Armor W";
	m_pTypeInfo[ITEM_CLASS_COAT][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetFrameID( 288, 302, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetDropFrameID( 288 );
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][13].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][13].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][14].HName = "况 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][14].EName = "War Armor M";
	m_pTypeInfo[ITEM_CLASS_COAT][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetFrameID( 280, 294, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetDropFrameID( 280 );
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][14].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][14].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][15].HName = "况 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][15].EName = "War Armor W";
	m_pTypeInfo[ITEM_CLASS_COAT][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetFrameID( 288, 302, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetDropFrameID( 288 );
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][15].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][15].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][16].HName = "靛贰帮 内 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][16].EName = "Dragon's cor Armor M";
	m_pTypeInfo[ITEM_CLASS_COAT][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetFrameID( 366, 380, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetDropFrameID( 366 );
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][16].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][16].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][17].HName = "靛贰帮 内 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][17].EName = "Dragon's cor Armor W";
	m_pTypeInfo[ITEM_CLASS_COAT][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetFrameID( 367, 381, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetDropFrameID( 367 );
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][17].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][17].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][18].HName = "墨扼父 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][18].EName = "Kahraman Armor M";
	m_pTypeInfo[ITEM_CLASS_COAT][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetFrameID( 442, 456, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetDropFrameID( 442 );
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][18].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][18].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][19].HName = "墨扼父 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][19].EName = "Kahraman Armor W";
	m_pTypeInfo[ITEM_CLASS_COAT][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetFrameID( 443, 457, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetDropFrameID( 443 );
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][19].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][19].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][20].HName = "孽矾矫绢 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][20].EName = "Cuirassir Armor M";
	m_pTypeInfo[ITEM_CLASS_COAT][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetFrameID( 463, 477, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetDropFrameID( 463 );
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][20].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][20].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][21].HName = "孽矾矫绢 酒赣";
	m_pTypeInfo[ITEM_CLASS_COAT][21].EName = "Cuirassir Armor W";
	m_pTypeInfo[ITEM_CLASS_COAT][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetFrameID( 465, 479, 0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetDropFrameID( 465 );
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][21].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][21].Price = 2500;


	m_pTypeInfo[ITEM_CLASS_COAT][22].HName = "配呈刚飘 酒赣-M";
	m_pTypeInfo[ITEM_CLASS_COAT][22].EName = "Tournament Armor-M";
	m_pTypeInfo[ITEM_CLASS_COAT][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetFrameID( 933 , 956 ,0);	
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetDropFrameID( 933 );
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][22].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][22].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][23].HName = "配呈刚飘 酒赣-W";
	m_pTypeInfo[ITEM_CLASS_COAT][23].EName = "Tournament Armor-W";
	m_pTypeInfo[ITEM_CLASS_COAT][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetFrameID( 934 , 957 ,0 );	
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetDropFrameID( 934 );
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][23].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][23].Price = 2500;


	m_pTypeInfo[ITEM_CLASS_COAT][24].HName = "皋浆胶挪";
	m_pTypeInfo[ITEM_CLASS_COAT][24].EName = "Mesh Skin M";
	m_pTypeInfo[ITEM_CLASS_COAT][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetFrameID( 956, 992, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetDropFrameID(956 );
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][24].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][24].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_COAT][25].HName = "皋浆胶挪";
	m_pTypeInfo[ITEM_CLASS_COAT][25].EName = "Mesh Skin W";
	m_pTypeInfo[ITEM_CLASS_COAT][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetFrameID( 958, 994, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetDropFrameID(958 );
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetAddonFrameID( ADDONID_COAT3_MALE, ADDONID_COAT3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_COAT][25].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COAT][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COAT][25].Price = 2500;

//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].HName = "赣浇 酱飘";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].EName = "Muscle Suit M";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][2].Price = 0;
//
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].HName = "赣浇 酱飘";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].EName = "Muscle Suit W";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_COAT][3].Price = 0;



	//---------------------------------------------------------------------
	// ITEM_CLASS_TROUSER
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	// 125 窍狼 巢备
	// 126 窍狼 咯备
	// 127 窍狼 巢脚
	// 128 窍狼 咯脚	
	InitClass(ITEM_CLASS_TROUSER, 26 );
	
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].HName = "巢磊 浇发";
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].EName = "Combat Pants";
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetFrameID( 125, 27, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetDropFrameID( 125 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetAddonFrameID( ADDONID_TROUSER1_MALE, ADDONID_TROUSER1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][0].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][1].HName = "咯磊 浇发";
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].EName = "Plate Trouser";
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetFrameID( 126, 107, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetDropFrameID( 126 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetAddonFrameID( ADDONID_TROUSER1_MALE, ADDONID_TROUSER1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][1].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][2].HName = "巢磊 飘扼快历";
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].EName = "Combat Pants";
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetFrameID( 125, 27, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetDropFrameID( 125 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetAddonFrameID( ADDONID_TROUSER1_MALE, ADDONID_TROUSER1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][2].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][3].HName = "咯磊 飘扼快历";
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].EName = "Plate Trouser";
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetFrameID( 126, 107, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetDropFrameID( 126 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetAddonFrameID( ADDONID_TROUSER1_MALE, ADDONID_TROUSER1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][3].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][4].HName = "巢磊 饭彪令";
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].EName = "Combat Pants";
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetFrameID( 127, 30, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetDropFrameID( 127 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetAddonFrameID( ADDONID_TROUSER2_MALE, ADDONID_TROUSER2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][4].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][5].HName = "咯磊 饭彪令";
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].EName = "Plate Trouser";
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetFrameID( 128, 110, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetDropFrameID( 128 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetAddonFrameID( ADDONID_TROUSER2_MALE, ADDONID_TROUSER2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][5].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][6].HName = "巢磊 硅撇饭彪令";
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].EName = "Combat Pants";
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetFrameID( 127, 30, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetDropFrameID( 127 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetAddonFrameID( ADDONID_TROUSER2_MALE, ADDONID_TROUSER2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][6].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][7].HName = "咯磊 硅撇饭彪令";
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].EName = "Plate Trouser";
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetFrameID( 128, 110, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetDropFrameID( 128 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetAddonFrameID( ADDONID_TROUSER2_MALE, ADDONID_TROUSER2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][7].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][8].HName = "硅撇 霸捞磐";
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].EName = "Battle Gaiters M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetFrameID( 283, 297, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetDropFrameID( 283 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][8].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][9].HName = "硅撇 霸捞磐";
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].EName = "Battle Gaiters W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetFrameID( 291, 305, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetDropFrameID( 291 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][9].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][10].HName = "哪诡 霸捞磐";
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].EName = "Combat Gaiters M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetFrameID( 283, 297, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetDropFrameID( 283 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][10].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][11].HName = "哪诡 霸捞磐";
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].EName = "Combat Gaiters W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetFrameID( 291, 305, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetDropFrameID( 291 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][11].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][12].HName = "哪诡 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].EName = "Combat Guards M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetFrameID( 284, 298, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetDropFrameID( 284 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][12].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][13].HName = "哪诡 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].EName = "Combat Guards W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetFrameID( 292, 306, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetDropFrameID( 292 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][13].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][14].HName = "况 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].EName = "War Guards M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetFrameID( 284, 298, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetDropFrameID( 284 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][14].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][15].HName = "况 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].EName = "War Guards W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetFrameID( 292, 306, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetDropFrameID( 292 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][15].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][16].HName = "靛贰帮 胶纳老 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].EName = "Dragon's Scale Guards M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetFrameID( 368, 382, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetDropFrameID( 368 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][16].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][17].HName = "靛贰帮 胶纳老 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].EName = "Dragon's Scale Guards W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetFrameID( 369, 383, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetDropFrameID( 369 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][17].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][18].HName = "墨扼父 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].EName = "Kahraman Guard M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetFrameID( 444, 458, 0 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetDropFrameID( 444 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].SetValue(180, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][18].Price = 1400;

	m_pTypeInfo[ITEM_CLASS_TROUSER][19].HName = "墨扼父 啊靛";
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].EName = "Kahraman Guard W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetFrameID( 445, 459, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetDropFrameID( 445 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][19].Price = 1700;

	m_pTypeInfo[ITEM_CLASS_TROUSER][20].HName = "孽矾矫绢 孽胶";
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].EName = "Cuirassir Cuisse M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetFrameID( 464, 478, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetDropFrameID( 464 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].SetValue(2000, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][20].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_TROUSER][21].HName = "孽矾矫绢 孽胶";
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].EName = "Cuirassir Cuisse W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetFrameID( 466, 480, 0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetDropFrameID( 466 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][21].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_TROUSER][22].HName = "配呈刚飘 孽胶-M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].EName = "Tournament Cuisse M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetFrameID( 935 , 958 ,0 );	
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetDropFrameID( 935 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].SetValue(2000, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][22].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_TROUSER][23].HName = "配呈刚飘 孽胶-W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].EName = "Tournament Cuisse W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetFrameID( 936 , 959 ,0);	
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetDropFrameID( 936 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][23].Price = 2500;
	
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].HName = "努傅 浇发";
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].EName = "Cling Slack M";
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetFrameID( 957, 993, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetDropFrameID(957 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][24].Price = 2500;

	m_pTypeInfo[ITEM_CLASS_TROUSER][25].HName = "努傅 浇发";
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].EName = "Cling Slack W";
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetFrameID( 959, 995, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetDropFrameID(959 );
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetAddonFrameID( ADDONID_TROUSER3_MALE, ADDONID_TROUSER3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TROUSER][25].Price = 2500;

//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].HName = "僵 啊靛";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].EName = "Shell Guard M";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][2].Price = 0;
//
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].HName = "僵 啊靛";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].EName = "Shell Guard W";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ADVANCED_TROUSER][3].Price = 0;


	//---------------------------------------------------------------------
	// ITEM_CLASS_SHOES
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SHOES, 11 );

	m_pTypeInfo[ITEM_CLASS_SHOES][0].HName = "扼捞飘 酱令";
	m_pTypeInfo[ITEM_CLASS_SHOES][0].EName = "Light Shoes";
	m_pTypeInfo[ITEM_CLASS_SHOES][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][0].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][0].SetFrameID( 42, 102, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][0].SetDropFrameID( 42 );
	m_pTypeInfo[ITEM_CLASS_SHOES][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][0].SetValue(120, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][0].Price = 700; 
	//m_pTypeInfo[ITEM_CLASS_SHOES][0].SetRequireAbility(20, 20, 0, ITEMTABLE_INFO::REQ_OR);
	

	m_pTypeInfo[ITEM_CLASS_SHOES][1].HName = "饭歹 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][1].EName = "Leather Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][1].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][1].SetFrameID( 40, 103, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][1].SetDropFrameID( 40 );
	m_pTypeInfo[ITEM_CLASS_SHOES][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][1].SetValue(150, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][1].Price	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][1].SetRequireAbility(40, 40, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][2].HName = "胶瓶 酱令";
	m_pTypeInfo[ITEM_CLASS_SHOES][2].EName = "Steel Shoes";
	m_pTypeInfo[ITEM_CLASS_SHOES][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][2].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][2].SetFrameID( 39, 104, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][2].SetDropFrameID( 39 );
	m_pTypeInfo[ITEM_CLASS_SHOES][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][2].SetValue(200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][2].Price	= 2000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][2].SetRequireAbility(60, 60, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][3].HName = "哪诡 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][3].EName = "Combat Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][3].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][3].SetFrameID( 41, 105, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][3].SetDropFrameID( 41 );
	m_pTypeInfo[ITEM_CLASS_SHOES][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][3].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][3].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][3].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][4].HName = "绢汲飘 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][4].EName = "Assault Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][4].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][4].SetFrameID( 261, 274, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][4].SetDropFrameID( 261 );
	m_pTypeInfo[ITEM_CLASS_SHOES][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][4].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][4].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][4].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][5].HName = "荤鄂 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][5].EName = "Saran Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][5].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][5].SetFrameID( 263, 276, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][5].SetDropFrameID( 263 );
	m_pTypeInfo[ITEM_CLASS_SHOES][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][5].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][5].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][5].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][6].HName = "况 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][6].EName = "War Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][6].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][6].SetFrameID( 262, 275, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][6].SetDropFrameID( 262 );
	m_pTypeInfo[ITEM_CLASS_SHOES][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][6].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][6].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][6].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][7].HName = "赣钮府 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][7].EName = "Mercury Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][7].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][7].SetFrameID( 371, 385, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][7].SetDropFrameID( 371 );
	m_pTypeInfo[ITEM_CLASS_SHOES][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][7].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][7].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][7].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][8].HName = "贾 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][8].EName = "B&R SEG-1 Sole Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][8].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][8].SetFrameID( 448, 462, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][8].SetDropFrameID( 448 );
	m_pTypeInfo[ITEM_CLASS_SHOES][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][8].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][8].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][8].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	m_pTypeInfo[ITEM_CLASS_SHOES][9].HName = "辑硅瓢 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][9].EName = "Sabbaton Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][9].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][9].SetFrameID( 475, 489, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHOES][9].SetDropFrameID( 475 );
	m_pTypeInfo[ITEM_CLASS_SHOES][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][9].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][9].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][9].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

		m_pTypeInfo[ITEM_CLASS_SHOES][10].HName = "混矾复 何明";
	m_pTypeInfo[ITEM_CLASS_SHOES][10].EName = "Solleret Boots";
	m_pTypeInfo[ITEM_CLASS_SHOES][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOES][10].SetSoundID( SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUND_ITEM_MOVE_SHOES, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHOES][10].SetFrameID( 919 , 942 , 0);	
	m_pTypeInfo[ITEM_CLASS_SHOES][10].SetDropFrameID( 919 );
	m_pTypeInfo[ITEM_CLASS_SHOES][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHOES][10].SetValue(500, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHOES][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOES][10].Price	= 10000;
	//m_pTypeInfo[ITEM_CLASS_SHOES][9].SetRequireAbility(80, 80, 0, ITEMTABLE_INFO::REQ_OR);

	//---------------------------------------------------------------------
	// ITEM_CLASS_SWORD
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SWORD, 15 );

	m_pTypeInfo[ITEM_CLASS_SWORD][0].HName = "扼捞飘 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][0].EName = "Light Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetFrameID( 44, 21, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetDropFrameID( 44 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetGrid(1, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SetValue(700, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][0].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_SWORD][0].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][0].SetRequireAbility(20);
	m_pTypeInfo[ITEM_CLASS_SWORD][0].UseActionInfo = SKILL_ATTACK_SWORD;
	
	m_pTypeInfo[ITEM_CLASS_SWORD][1].HName = "况 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][1].EName = "War Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetFrameID( 43, 20, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetDropFrameID( 43 );
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetGrid(1, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SetValue(1500, -1, 15, 20, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][1].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_SWORD][1].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][1].SetRequireAbility(30);
	m_pTypeInfo[ITEM_CLASS_SWORD][1].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][2].HName = "宏肺靛 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][2].EName = "New Gladius";
	m_pTypeInfo[ITEM_CLASS_SWORD][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetFrameID( 46, 22, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetDropFrameID( 46 );
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetGrid(1, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SetValue(3000, -1, 20, 25, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][2].Price	= 12000;
	m_pTypeInfo[ITEM_CLASS_SWORD][2].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][2].SetRequireAbility(40);
	m_pTypeInfo[ITEM_CLASS_SWORD][2].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][3].HName = "官胶鸥靛 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][3].EName = "Bastard Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetFrameID( 45, 23, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetDropFrameID( 45 );
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SetValue(5000, -1, 25, 30, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][3].Price	= 50000;
	m_pTypeInfo[ITEM_CLASS_SWORD][3].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][3].SetRequireAbility(50);
	m_pTypeInfo[ITEM_CLASS_SWORD][3].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][4].HName = "宏肺靛 饭捞乔绢";
	m_pTypeInfo[ITEM_CLASS_SWORD][4].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetFrameID( 183, 187, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetDropFrameID( 183 );
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][4].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][4].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][4].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][4].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][5].HName = "绊雕 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][5].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetFrameID( 184, 188, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetDropFrameID( 184 );
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][5].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][5].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][5].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][5].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][6].HName = "农风技捞歹 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][6].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetFrameID( 182, 186, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetDropFrameID( 182 );
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][6].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][6].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][6].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][6].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][7].HName = "弊饭捞飘 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][7].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetFrameID( 185, 189, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetDropFrameID( 185 );
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][7].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][7].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][7].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][7].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][8].HName = "努饭捞葛绢";
	m_pTypeInfo[ITEM_CLASS_SWORD][8].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetFrameID( 181, 185, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetDropFrameID( 181 );
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][8].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][8].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][8].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][8].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][9].HName = "啊叼胶 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][9].EName = "Goddess Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetFrameID( 47, 24, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetDropFrameID( 47 );
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][9].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][9].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][9].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][9].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][10].HName = "墨滴矫快胶 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][10].EName = "Caduceus Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetFrameID( 346, 360, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetDropFrameID( 346 );
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][10].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][10].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][10].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][10].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][11].HName = "官角府胶农 家靛";
	m_pTypeInfo[ITEM_CLASS_SWORD][11].EName = "Basilisk Sword";
	m_pTypeInfo[ITEM_CLASS_SWORD][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetFrameID( 434, 448, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetDropFrameID( 434 );
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][11].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][11].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][11].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][11].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][12].HName = "臂扼叼快胶";
	m_pTypeInfo[ITEM_CLASS_SWORD][12].EName = "Gladius";
	m_pTypeInfo[ITEM_CLASS_SWORD][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetFrameID( 468, 482, 0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetDropFrameID( 468 );
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][12].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][12].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][12].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][12].UseActionInfo = SKILL_ATTACK_SWORD;

	m_pTypeInfo[ITEM_CLASS_SWORD][13].HName = "明官捞茄歹";
	m_pTypeInfo[ITEM_CLASS_SWORD][13].EName = "Zweihander";
	m_pTypeInfo[ITEM_CLASS_SWORD][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetFrameID( 928 , 951 ,0 );	
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetDropFrameID( 928 );
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][13].Price	= 100000;
	m_pTypeInfo[ITEM_CLASS_SWORD][13].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_SWORD][12].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_SWORD][13].UseActionInfo = SKILL_ATTACK_SWORD;
	
	m_pTypeInfo[ITEM_CLASS_SWORD][14].HName = "聪甸 咖廉";
	m_pTypeInfo[ITEM_CLASS_SWORD][14].EName = "Niddle Edger";
	m_pTypeInfo[ITEM_CLASS_SWORD][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetFrameID( 946, 982, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetDropFrameID(946 );
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetAddonFrameID( ADDONID_SWORD_MALE, ADDONID_SWORD_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SWORD][14].SetValue(10000, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SWORD][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SWORD][14].Price = 0;
	m_pTypeInfo[ITEM_CLASS_SWORD][14].UseActionInfo = SKILL_ATTACK_SWORD;

//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].HName = "巨臂目磐";
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].EName = "Angle Cutter";
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_CELOX_CUTTER][1].Price = 0;



	//---------------------------------------------------------------------
	// ITEM_CLASS_BLADE
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_BLADE, 15 );

	m_pTypeInfo[ITEM_CLASS_BLADE][0].HName = "目撇贰胶";
	m_pTypeInfo[ITEM_CLASS_BLADE][0].EName = "Cutlass";
	m_pTypeInfo[ITEM_CLASS_BLADE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetFrameID( 49, 32, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetDropFrameID( 49 );
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][0].Price	= 1500;
	m_pTypeInfo[ITEM_CLASS_BLADE][0].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][0].SetRequireAbility(20);
	m_pTypeInfo[ITEM_CLASS_BLADE][0].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][1].HName = "氛 激寂";
	m_pTypeInfo[ITEM_CLASS_BLADE][1].EName = "Long Shamsher";
	m_pTypeInfo[ITEM_CLASS_BLADE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetFrameID( 50, 33, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetDropFrameID( 50 );
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SetValue(2000, -1, 15, 20, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][1].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_BLADE][1].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][1].SetRequireAbility(30);
	m_pTypeInfo[ITEM_CLASS_BLADE][1].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][2].HName = "妻门";
	m_pTypeInfo[ITEM_CLASS_BLADE][2].EName = "Severd Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetFrameID( 189, 193, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetDropFrameID( 189 );
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SetValue(3200, -1, 20, 25, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][2].Price	= 15000;
	m_pTypeInfo[ITEM_CLASS_BLADE][2].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][2].SetRequireAbility(40);
	m_pTypeInfo[ITEM_CLASS_BLADE][2].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][3].HName = "技滚靛 喉饭捞靛";
	m_pTypeInfo[ITEM_CLASS_BLADE][3].EName = "Moon Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetFrameID( 48, 34, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetDropFrameID( 48 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SetValue(4000, -1, 25, 30, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][3].Price	= 50000;
	m_pTypeInfo[ITEM_CLASS_BLADE][3].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][3].SetRequireAbility(50);
	m_pTypeInfo[ITEM_CLASS_BLADE][3].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][4].HName = "巩 喉饭捞靛";
	m_pTypeInfo[ITEM_CLASS_BLADE][4].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetFrameID( 51, 35, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetDropFrameID( 51 );
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][4].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][4].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][4].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][4].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][5].HName = "技捞滚";
	m_pTypeInfo[ITEM_CLASS_BLADE][5].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetFrameID( 186, 190, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetDropFrameID( 186 );
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][5].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][5].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][5].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][5].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][6].HName = "傅 喉饭捞靛";
	m_pTypeInfo[ITEM_CLASS_BLADE][6].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetFrameID( 52, 36, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetDropFrameID( 52 );
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][6].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][6].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][6].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][6].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][7].HName = "矫固磐";
	m_pTypeInfo[ITEM_CLASS_BLADE][7].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetFrameID( 187, 191, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetDropFrameID( 187 );
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][7].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][7].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][7].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][7].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][8].HName = "内其浆";
	m_pTypeInfo[ITEM_CLASS_BLADE][8].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetFrameID( 190, 194, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetDropFrameID( 190 );
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][8].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][8].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][8].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][8].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][9].HName = "墨鸥唱";
	m_pTypeInfo[ITEM_CLASS_BLADE][9].EName = "Ring Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetFrameID( 188, 192, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetDropFrameID( 188 );
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][9].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][9].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][9].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][9].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][10].HName = "夯 喉贰捞靛";
	m_pTypeInfo[ITEM_CLASS_BLADE][10].EName = "Bone Blade";
	m_pTypeInfo[ITEM_CLASS_BLADE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetFrameID( 347, 361, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetDropFrameID( 347 );
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][10].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][10].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][10].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][10].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][11].HName = "内扼";
	m_pTypeInfo[ITEM_CLASS_BLADE][11].EName = "Cora";
	m_pTypeInfo[ITEM_CLASS_BLADE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetFrameID( 435, 449, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetDropFrameID( 435 );
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][11].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][11].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][11].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][11].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][12].HName = "芬颇捞酒";
	m_pTypeInfo[ITEM_CLASS_BLADE][12].EName = "Rhomphaia";
	m_pTypeInfo[ITEM_CLASS_BLADE][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetFrameID( 467, 481, 0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetDropFrameID( 467 );
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][12].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][12].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][12].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][12].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_BLADE][13].HName = "窍福其";
	m_pTypeInfo[ITEM_CLASS_BLADE][13].EName = "Harpe";
	m_pTypeInfo[ITEM_CLASS_BLADE][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetFrameID( 938 , 961 ,0 );	
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetDropFrameID( 938 );
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][13].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][13].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][12].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][13].UseActionInfo = SKILL_ATTACK_BLADE;

	
	m_pTypeInfo[ITEM_CLASS_BLADE][14].HName = "掂坷墨器";
	m_pTypeInfo[ITEM_CLASS_BLADE][14].EName = "Duocarpo";
	m_pTypeInfo[ITEM_CLASS_BLADE][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetFrameID( 947, 983, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetDropFrameID(947 );
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetAddonFrameID( ADDONID_BLADE_MALE, ADDONID_BLADE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SetValue(9800, -1, 30, 35, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BLADE][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLADE][14].Price	= 110000;
	m_pTypeInfo[ITEM_CLASS_BLADE][14].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_BLADE][12].SetRequireAbility(60);
	m_pTypeInfo[ITEM_CLASS_BLADE][14].UseActionInfo = SKILL_ATTACK_BLADE;


//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].HName = "乔绢 宏傅绢";
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].EName = "Fear Bringer";
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_GRAVIS_CUTTER][1].Price = 0;
//
	//---------------------------------------------------------------------
	// ITEM_CLASS_SHIELD
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SHIELD, 14 );
	
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].HName = "扼款靛 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].EName = "Round Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetFrameID( 53, 58, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetDropFrameID( 53 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetAddonFrameID( ADDONID_SHIELD3_MALE, ADDONID_SHIELD3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetValue(500, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][0].Price	= 2000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][0].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_SHIELD][1].HName = "宏沸令 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].EName = "Bronze Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetFrameID( 55, 59, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetDropFrameID( 55 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetAddonFrameID( ADDONID_SHIELD3_MALE, ADDONID_SHIELD3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetValue(800, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][1].Price	= 3000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][1].SetRequireAbility(30);

	m_pTypeInfo[ITEM_CLASS_SHIELD][2].HName = "荣 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].EName = "Whirl Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetFrameID( 54, 60, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetDropFrameID( 54 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetAddonFrameID( ADDONID_SHIELD3_MALE, ADDONID_SHIELD3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetValue(1200, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][2].Price	= 7000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][2].SetRequireAbility(40);

	m_pTypeInfo[ITEM_CLASS_SHIELD][3].HName = "胶匙捞农 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].EName = "Snake Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetFrameID( 56, 61, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetDropFrameID( 56 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetAddonFrameID( ADDONID_SHIELD3_MALE, ADDONID_SHIELD3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetValue(3000, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][3].Price	= 20000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][3].SetRequireAbility(50);

	m_pTypeInfo[ITEM_CLASS_SHIELD][4].HName = "家靛 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].EName = "Sword Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetFrameID( 57, 62, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetDropFrameID( 57 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetAddonFrameID( ADDONID_SHIELD3_MALE, ADDONID_SHIELD3_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetValue(4000, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][4].Price	= 50000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][4].SetRequireAbility(60);

	m_pTypeInfo[ITEM_CLASS_SHIELD][5].HName = "靛贰帮 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].EName = "Dragon Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetFrameID( 58, 63, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetDropFrameID( 58 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][5].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][5].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][6].HName = "酒弊府颇 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].EName = "Shield of Agrippa";
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetFrameID( 255, 268, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetDropFrameID( 255 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][6].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][6].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][7].HName = "蜡聪能 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].EName = "Unicorn Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetFrameID( 256, 269, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetDropFrameID( 256 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][7].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][7].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][8].HName = "饭坷 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].EName = "Leo Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetFrameID( 257, 270, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetDropFrameID( 257 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][8].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][8].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][9].HName = "敲贰赣 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].EName = "Flamma Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetFrameID( 352, 366, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetDropFrameID( 352 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][9].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][9].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][10].HName = "坷喉氛 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].EName = "Oblong Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetFrameID( 450, 464, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetDropFrameID( 450 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][10].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][10].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][11].HName = "龋敲沸 蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].EName = "Hoplon Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetFrameID( 477, 491, 0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetDropFrameID( 477 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][11].Price	= 120000;
	//m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetRequireAbility(70);

	m_pTypeInfo[ITEM_CLASS_SHIELD][12].HName = "酒胶乔胶  蒋靛";
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].EName = "Aspis Shield";
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetSoundID( SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUND_ITEM_MOVE_SHIELD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetFrameID( 921 , 944 ,0 );	
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetDropFrameID( 915 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][12].Price	= 120000;

	m_pTypeInfo[ITEM_CLASS_SHIELD][13].HName = "胶欺 滚努矾";
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].EName = "Spur Buckler";
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetFrameID( 949, 985, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetDropFrameID(949 );
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetAddonFrameID( ADDONID_SHIELD2_MALE, ADDONID_SHIELD2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].SetValue(5000, 6, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHIELD][13].Price	= 120000;

//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].HName = "胶鸥快飘 滚努矾";
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].EName = "Stout Buckler";
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_BUCKLER][1].Price = 0;




	//m_pTypeInfo[ITEM_CLASS_SHIELD][11].SetRequireAbility(70);

	//---------------------------------------------------------------------
	// ITEM_CLASS_CROSS
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), MP刘啊(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_CROSS, 13);

	m_pTypeInfo[ITEM_CLASS_CROSS][0].HName = "角滚 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][0].EName = "Silver Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetFrameID( 146, 150, 0 );	
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetDropFrameID( 146 );
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SetValue(300, -1, 3, 6, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][0].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_CROSS][0].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][0].SetRequireAbility(0,0, 20);
	m_pTypeInfo[ITEM_CLASS_CROSS][0].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][1].HName = "扼凭 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][1].EName = "Latin Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetFrameID( 60, 95, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetDropFrameID( 60 );
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SetValue(500, -1, 6, 9, 10, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][1].Price	= 7000;
	m_pTypeInfo[ITEM_CLASS_CROSS][1].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][1].SetRequireAbility(0,0, 30);
	m_pTypeInfo[ITEM_CLASS_CROSS][1].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][2].HName = "菩记 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][2].EName = "Passion Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetFrameID( 61, 96, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetDropFrameID( 61 );
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SetValue(700, -1, 9, 12, 20, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][2].Price	= 20000;
	m_pTypeInfo[ITEM_CLASS_CROSS][2].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][2].SetRequireAbility(0,0, 40);
	m_pTypeInfo[ITEM_CLASS_CROSS][2].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][3].HName = "扁府矫窜 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][3].EName = "Girisidan Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetFrameID( 62, 97, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetDropFrameID( 62 );
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SetValue(1200, -1, 12, 15, 40, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][3].Price	= 50000;
	m_pTypeInfo[ITEM_CLASS_CROSS][3].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][3].SetRequireAbility(0,0, 50);
	m_pTypeInfo[ITEM_CLASS_CROSS][3].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][4].HName = "俊乔胶内妻 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][4].EName = "Episcopal Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetFrameID( 63, 98, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetDropFrameID( 63 );
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][4].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][4].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][4].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][4].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][5].HName = "漠官府酒 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][5].EName = "Calvaria Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetFrameID( 265, 278, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetDropFrameID( 265 );
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][5].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][5].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][5].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][5].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][6].HName = "堡硅狼 绞磊啊";
	m_pTypeInfo[ITEM_CLASS_CROSS][6].EName = "Gloria Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetFrameID( 266, 279, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetDropFrameID( 266 );
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][6].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][6].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][6].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][6].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][7].HName = "己赤 秋饭唱狼 绞磊啊";
	m_pTypeInfo[ITEM_CLASS_CROSS][7].EName = "St.Helena Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetFrameID( 264, 277, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetDropFrameID( 264 );
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][7].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][7].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][7].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][7].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][8].HName = "炼促聪救 绞磊啊";
	m_pTypeInfo[ITEM_CLASS_CROSS][8].EName = "Jordanian Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetFrameID( 349, 363, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetDropFrameID( 349 );
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][8].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][8].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][8].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][8].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][9].HName = "其捞妻 农肺胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][9].EName = "Papal Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetFrameID( 436, 450, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetDropFrameID( 436 );
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][9].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][9].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][9].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][9].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][10].HName = "肺坊狼 绞磊啊";
	m_pTypeInfo[ITEM_CLASS_CROSS][10].EName = "Lorraine's Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetFrameID( 474, 488, 0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetDropFrameID( 474 );
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][10].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][10].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][10].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_CROSS][11].HName = "墨鸥府唱狼 绞磊啊";
	m_pTypeInfo[ITEM_CLASS_CROSS][11].EName = "Catharina's Cross";
	m_pTypeInfo[ITEM_CLASS_CROSS][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetFrameID( 930 , 953 ,0 );
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetDropFrameID( 930 );
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][11].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][11].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][11].UseActionInfo = SKILL_ATTACK_BLADE;


	
	m_pTypeInfo[ITEM_CLASS_CROSS][12].HName = "副扼捞攫飘 农风矫胶";
	m_pTypeInfo[ITEM_CLASS_CROSS][12].EName = "Reliant Crucis";
	m_pTypeInfo[ITEM_CLASS_CROSS][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetFrameID( 951, 987, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetDropFrameID(951 );
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetAddonFrameID( ADDONID_CROSS_MALE, ADDONID_CROSS_FEMALE );
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_CROSS][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CROSS][12].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_CROSS][12].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_CROSS][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_CROSS][12].UseActionInfo = SKILL_ATTACK_BLADE;

//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].HName = "绢钮飘 农风矫胶";
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].EName = "Acute Crucis";
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_CRUCIS][1].Price = 0;


	//---------------------------------------------------------------------
	// ITEM_CLASS_GLOVE
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_GLOVE, 12);

	m_pTypeInfo[ITEM_CLASS_GLOVE][0].HName = "喉发 臂矾宏";
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].EName = "Black Glove";
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetFrameID( 64, 116, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetDropFrameID( 64 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetValue(200, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][0].Price	= 800;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][0].SetRequireAbility(20, 20);

	m_pTypeInfo[ITEM_CLASS_GLOVE][1].HName = "宏沸令 臂矾宏";
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].EName = "Bronze Glove";
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetFrameID( 65, 117, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetDropFrameID( 65 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetValue(250, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][1].Price	= 2000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][1].SetRequireAbility(30, 30);

	m_pTypeInfo[ITEM_CLASS_GLOVE][2].HName = "橇府胶飘 臂矾宏";
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].EName = "Priest Glove";
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetFrameID( 67, 118, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetDropFrameID( 67 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetValue(300, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][2].Price	= 5000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][2].SetRequireAbility(0, 0, 40);

	m_pTypeInfo[ITEM_CLASS_GLOVE][3].HName = "唱捞飘 臂矾宏";
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].EName = "Knight Glove";
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetFrameID( 66, 119, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetDropFrameID( 66 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetValue(400, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][3].Price	= 7000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][3].SetRequireAbility(40, 40);

	m_pTypeInfo[ITEM_CLASS_GLOVE][4].HName = "技胶磐胶";
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].EName = "Cestus";
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetFrameID( 68, 120, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetDropFrameID( 68 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][4].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][4].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][5].HName = "凭敲饭捞飘 扒撇房";
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].EName = "Tinplate Gauntlet";
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetFrameID( 259, 272, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetDropFrameID( 259 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][5].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][5].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][6].HName = "咀萍宏 扒撇房";
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].EName = "Active Gauntlet";
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetFrameID( 260, 273, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetDropFrameID( 260 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][6].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][6].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][7].HName = "配福狼 扒撇房";
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].EName = "Gauntlet of Thor";
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetFrameID( 258, 271, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetDropFrameID( 258 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][7].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][7].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][8].HName = "胶瓶 扒撇房";
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].EName = "Steel Gauntlet";
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetFrameID( 350, 364, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetDropFrameID( 350 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][8].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][8].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][9].HName = "R-俊呈力平 臂矾宏";
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].EName = "R-energetic Gloves";
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetFrameID( 447, 461, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetDropFrameID( 447 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][9].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][9].SetRequireAbility(50, 50);

	m_pTypeInfo[ITEM_CLASS_GLOVE][10].HName = "孽矾矫绢 扒撇房";
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].EName = "Cuirassir Gauntlet";
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetFrameID( 479, 493, 0 );	
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetDropFrameID( 479 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][10].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_GLOVE][10].SetRequireAbility(50, 50);
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].HName = "酒阁 扒撇复";
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].EName = "Amon Gauntlet";
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].SetFrameID( 920 , 943 ,0);	
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].SetDropFrameID( 920 );
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].SetValue(550, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GLOVE][11].Price	= 13000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_HELM
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_HELM, 14);

	m_pTypeInfo[ITEM_CLASS_HELM][0].HName = "饶靛";
	m_pTypeInfo[ITEM_CLASS_HELM][0].EName = "Hood";
	m_pTypeInfo[ITEM_CLASS_HELM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetFrameID( 69, 40, 0 );	
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetDropFrameID( 69 );
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetAddonFrameID( ADDONID_HELM1_MALE, ADDONID_HELM1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][0].SetValue(210, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][0].Price	= 500;
	//m_pTypeInfo[ITEM_CLASS_HELM][0].SetRequireAbility(20, 20);

	m_pTypeInfo[ITEM_CLASS_HELM][1].HName = "酒捞攫 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][1].EName = "Iron Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetFrameID( 70, 41, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetDropFrameID( 70 );
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetAddonFrameID( ADDONID_HELM1_MALE, ADDONID_HELM1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][1].SetValue(300, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][1].Price	= 1000;
	//m_pTypeInfo[ITEM_CLASS_HELM][1].SetRequireAbility(40, 40);

	m_pTypeInfo[ITEM_CLASS_HELM][2].HName = "橇扼饭靛 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][2].EName = "Frared Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetFrameID( 71, 42, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetDropFrameID( 71 );
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetAddonFrameID( ADDONID_HELM1_MALE, ADDONID_HELM1_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][2].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][2].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][2].SetRequireAbility(60, 60);

	m_pTypeInfo[ITEM_CLASS_HELM][3].HName = "俊烙 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][3].EName = "Aim Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetFrameID( 72, 43, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetDropFrameID( 72 );
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][3].SetValue(550, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][3].Price	= 7000;
	//m_pTypeInfo[ITEM_CLASS_HELM][3].SetRequireAbility(80, 80);

	m_pTypeInfo[ITEM_CLASS_HELM][4].HName = "哪诡 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][4].EName = "Combat Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetFrameID( 73, 44, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetDropFrameID( 73 );
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][4].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][4].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_HELM][4].SetRequireAbility(90, 90);

	m_pTypeInfo[ITEM_CLASS_HELM][5].HName = "庆靛扁绢";
	m_pTypeInfo[ITEM_CLASS_HELM][5].EName = "HeadGear";
	m_pTypeInfo[ITEM_CLASS_HELM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetFrameID( 299, 313, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetDropFrameID( 299 );
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][5].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][5].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_HELM][5].SetRequireAbility(90, 90);

	m_pTypeInfo[ITEM_CLASS_HELM][6].HName = "哪诡 庆靛扁绢";
	m_pTypeInfo[ITEM_CLASS_HELM][6].EName = "HeadGear";
	m_pTypeInfo[ITEM_CLASS_HELM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetFrameID( 299, 313, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetDropFrameID( 299 );
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][6].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][6].Price	= 13000;
	//m_pTypeInfo[ITEM_CLASS_HELM][6].SetRequireAbility(90, 90);

	m_pTypeInfo[ITEM_CLASS_HELM][7].HName = "牢橇扼饭靛 胶某醋 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][7].EName = "Infrared Scanning Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetFrameID( 300, 314, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetDropFrameID( 300 );
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][7].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][7].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][7].SetRequireAbility(60, 60);

	m_pTypeInfo[ITEM_CLASS_HELM][8].HName = "况 敲饭捞飘 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][8].EName = "War Plate Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetFrameID( 302, 316, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetDropFrameID( 302 );
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][8].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][8].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][8].SetRequireAbility(60, 60);

	m_pTypeInfo[ITEM_CLASS_HELM][9].HName = "掂农 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][9].EName = "Duke Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetFrameID( 365, 379, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetDropFrameID( 365 );
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][9].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][9].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][9].SetRequireAbility(60, 60);

	m_pTypeInfo[ITEM_CLASS_HELM][10].HName = "哪厚匙捞记 哪邯 秋抚";
	m_pTypeInfo[ITEM_CLASS_HELM][10].EName = "Combination Combat Helm";
	m_pTypeInfo[ITEM_CLASS_HELM][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetFrameID( 446, 460, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetDropFrameID( 446 );
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][10].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][10].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][10].SetRequireAbility(60, 60);

	m_pTypeInfo[ITEM_CLASS_HELM][11].HName = "胶曝扒 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][11].EName = "Spangen Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetFrameID( 476, 490, 0 );
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetDropFrameID( 476 );
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][11].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][11].Price	= 4000;
	//m_pTypeInfo[ITEM_CLASS_HELM][11].SetRequireAbility(60, 60);

	
	m_pTypeInfo[ITEM_CLASS_HELM][12].HName = "滚芭齿 秋镐";
	m_pTypeInfo[ITEM_CLASS_HELM][12].EName = "Burgonet Helmet";
	m_pTypeInfo[ITEM_CLASS_HELM][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetSoundID( SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUND_ITEM_MOVE_HELM, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetFrameID( 916 , 939 ,0 );
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetDropFrameID( 916 );
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][12].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][12].Price	= 4000;


	m_pTypeInfo[ITEM_CLASS_HELM][13].HName = "付款飘 绊臂";
	m_pTypeInfo[ITEM_CLASS_HELM][13].EName = "Mount Goggle";
	m_pTypeInfo[ITEM_CLASS_HELM][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetFrameID( 952, 988, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetDropFrameID(952 );
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetAddonFrameID( ADDONID_HELM2_MALE, ADDONID_HELM2_FEMALE );
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_HELM][13].SetValue(400, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_HELM][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_HELM][13].Price	= 4000;

//	m_pTypeInfo[ITEM_CLASS_HELMET][1].HName = "钢萍 胶墨快磐";
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].EName = "Multi Scouter";
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_HELMET][1].Price = 0;

	//---------------------------------------------------------------------
	// ITEM_CLASS_SG
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), 荤沥芭府(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SG, 14);
	
	m_pTypeInfo[ITEM_CLASS_SG][0].HName = "MT-10 酱泼 胶鸥";
	m_pTypeInfo[ITEM_CLASS_SG][0].EName = "SG-7";
	m_pTypeInfo[ITEM_CLASS_SG][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][0].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][0].SetFrameID( 76, 5, 0 );
	m_pTypeInfo[ITEM_CLASS_SG][0].SetDropFrameID( 76 );
	m_pTypeInfo[ITEM_CLASS_SG][0].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][0].SetValue(800, -1, 13, 18, 4, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][0].Price	= 1500;
	//m_pTypeInfo[ITEM_CLASS_SG][0].SetRequireAbility(0, 20);
	m_pTypeInfo[ITEM_CLASS_SG][0].UseActionInfo = SKILL_ATTACK_GUN_SG;	

	m_pTypeInfo[ITEM_CLASS_SG][1].HName = "MT-21 磊捞攫飘 胶鸥";
	m_pTypeInfo[ITEM_CLASS_SG][1].EName = "SG-AC21";
	m_pTypeInfo[ITEM_CLASS_SG][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][1].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][1].SetFrameID( 75, 6, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][1].SetDropFrameID( 75 );
	m_pTypeInfo[ITEM_CLASS_SG][1].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][1].SetValue(2000, -1, 18, 23, 4, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][1].Price	= 5000;
	//m_pTypeInfo[ITEM_CLASS_SG][1].SetRequireAbility(0, 40);
	m_pTypeInfo[ITEM_CLASS_SG][1].UseActionInfo = SKILL_ATTACK_GUN_SG;	

	m_pTypeInfo[ITEM_CLASS_SG][2].HName = "MTB-11 内镐 抛老";
	m_pTypeInfo[ITEM_CLASS_SG][2].EName = "SG-LE97";
	m_pTypeInfo[ITEM_CLASS_SG][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][2].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][2].SetFrameID( 74, 7, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][2].SetDropFrameID( 74 );
	m_pTypeInfo[ITEM_CLASS_SG][2].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][2].SetValue(3200, -1, 23, 27, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][2].Price	= 15000;
	//m_pTypeInfo[ITEM_CLASS_SG][2].SetRequireAbility(0, 60);
	m_pTypeInfo[ITEM_CLASS_SG][2].UseActionInfo = SKILL_ATTACK_GUN_SG;
	

	m_pTypeInfo[ITEM_CLASS_SG][3].HName = "AM-99 妇葛胶99";
	m_pTypeInfo[ITEM_CLASS_SG][3].EName = "SG-HA99";
	m_pTypeInfo[ITEM_CLASS_SG][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][3].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][3].SetFrameID( 78, 8, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][3].SetDropFrameID( 78 );
	m_pTypeInfo[ITEM_CLASS_SG][3].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][3].SetValue(5000, -1, 27, 32, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][3].Price	= 50000;
	//m_pTypeInfo[ITEM_CLASS_SG][3].SetRequireAbility(0, 70);
	m_pTypeInfo[ITEM_CLASS_SG][3].UseActionInfo = SKILL_ATTACK_GUN_SG;
	

	m_pTypeInfo[ITEM_CLASS_SG][4].HName = "TND-5 酒捞攫乔胶飘";
	m_pTypeInfo[ITEM_CLASS_SG][4].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][4].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][4].SetFrameID( 191, 195, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][4].SetDropFrameID( 191 );
	m_pTypeInfo[ITEM_CLASS_SG][4].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][4].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][4].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][4].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][4].UseActionInfo = SKILL_ATTACK_GUN_SG;
	

	m_pTypeInfo[ITEM_CLASS_SG][5].HName = "SS-10 畴农促款";
	m_pTypeInfo[ITEM_CLASS_SG][5].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][5].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][5].SetFrameID( 194, 198, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][5].SetDropFrameID( 194 );
	m_pTypeInfo[ITEM_CLASS_SG][5].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][5].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][5].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][5].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][5].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][6].HName = "RESS-1 单葛聪厩";
	m_pTypeInfo[ITEM_CLASS_SG][6].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][6].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][6].SetFrameID( 195, 199, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][6].SetDropFrameID( 195 );
	m_pTypeInfo[ITEM_CLASS_SG][6].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][6].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][6].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][6].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][6].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][7].HName = "某胶纳捞靛";
	m_pTypeInfo[ITEM_CLASS_SG][7].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][7].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][7].SetFrameID( 193, 197, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][7].SetDropFrameID( 193 );
	m_pTypeInfo[ITEM_CLASS_SG][7].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][7].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][7].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][7].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][7].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][8].HName = "RESS-4a 举府霸捞磐";
	m_pTypeInfo[ITEM_CLASS_SG][8].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][8].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][8].SetFrameID( 192, 196, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][8].SetDropFrameID( 192 );
	m_pTypeInfo[ITEM_CLASS_SG][8].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][8].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][8].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][8].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][8].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][9].HName = "S-200 胶盆";
	m_pTypeInfo[ITEM_CLASS_SG][9].EName = "SG-N2000";
	m_pTypeInfo[ITEM_CLASS_SG][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][9].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][9].SetFrameID( 77, 9, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][9].SetDropFrameID( 77 );
	m_pTypeInfo[ITEM_CLASS_SG][9].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][9].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][9].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][9].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][9].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][10].HName = "ST-2A 箕农幂欺";
	m_pTypeInfo[ITEM_CLASS_SG][10].EName = "ST-2A SharkChopper";
	m_pTypeInfo[ITEM_CLASS_SG][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][10].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][10].SetFrameID( 358, 372, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][10].SetDropFrameID( 358 );
	m_pTypeInfo[ITEM_CLASS_SG][10].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][10].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][10].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][10].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][10].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][11].HName = "Spas-12 窍捞官牢歹";
	m_pTypeInfo[ITEM_CLASS_SG][11].EName = "Spas-12 Highbinder";
	m_pTypeInfo[ITEM_CLASS_SG][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][11].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][11].SetFrameID( 438, 452, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][11].SetDropFrameID( 438 );
	m_pTypeInfo[ITEM_CLASS_SG][11].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][11].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][11].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][11].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][11].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][12].HName = "S-EV11";
	m_pTypeInfo[ITEM_CLASS_SG][12].EName = "Explosion";
	m_pTypeInfo[ITEM_CLASS_SG][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][12].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][12].SetFrameID( 470, 484, 0 );	
	m_pTypeInfo[ITEM_CLASS_SG][12].SetDropFrameID( 470 );
	m_pTypeInfo[ITEM_CLASS_SG][12].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][12].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][12].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][12].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][12].UseActionInfo = SKILL_ATTACK_GUN_SG;

	m_pTypeInfo[ITEM_CLASS_SG][13].HName = "PG-13 肺农";
	m_pTypeInfo[ITEM_CLASS_SG][13].EName = "PG-13 Roc";
	m_pTypeInfo[ITEM_CLASS_SG][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SG][13].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SG][13].SetFrameID( 905, 928 ,0 );	
	m_pTypeInfo[ITEM_CLASS_SG][13].SetDropFrameID( 905 );
	m_pTypeInfo[ITEM_CLASS_SG][13].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SG][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SG][13].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SG][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SG][13].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][12].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SG][13].UseActionInfo = SKILL_ATTACK_GUN_SG;

	
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].HName = "喉饭捞历 LRS";
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].EName = "Blaser LRS";
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_SINGLELOADER][1].Price = 0;



	//---------------------------------------------------------------------
	// ITEM_CLASS_SMG
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), 荤沥芭府(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SMG, 14);
	
	m_pTypeInfo[ITEM_CLASS_SMG][0].HName = "MD-4";
	m_pTypeInfo[ITEM_CLASS_SMG][0].EName = "SMG-5";
	m_pTypeInfo[ITEM_CLASS_SMG][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetFrameID( 79, 10, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetDropFrameID( 79 );
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][0].SetValue(900, -1, 11, 15, 4, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][0].Price	= 1500;
	//m_pTypeInfo[ITEM_CLASS_SMG][0].SetRequireAbility(0, 20);
	m_pTypeInfo[ITEM_CLASS_SMG][0].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	

	m_pTypeInfo[ITEM_CLASS_SMG][1].HName = "IS-100 瘤弊";
	m_pTypeInfo[ITEM_CLASS_SMG][1].EName = "SMG-BS45";
	m_pTypeInfo[ITEM_CLASS_SMG][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetFrameID( 81, 12, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetDropFrameID( 81 );
	//m_pTypeInfo[ITEM_CLASS_SMG][1].SetFrameID( 80, 11, 0 );
	//m_pTypeInfo[ITEM_CLASS_SMG][1].SetDropFrameID( 80 );
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][1].SetValue(2000, -1, 15, 19, 4, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][1].Price	= 6000;
	//m_pTypeInfo[ITEM_CLASS_SMG][1].SetRequireAbility(0, 40);
	m_pTypeInfo[ITEM_CLASS_SMG][1].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	

	m_pTypeInfo[ITEM_CLASS_SMG][2].HName = "M-INTER";
	m_pTypeInfo[ITEM_CLASS_SMG][2].EName = "SMG-HA85";
	m_pTypeInfo[ITEM_CLASS_SMG][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetFrameID( 200, 204, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetDropFrameID( 200 );
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][2].SetValue(3200, -1, 19, 22, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][2].Price	= 17000;
	//m_pTypeInfo[ITEM_CLASS_SMG][2].SetRequireAbility(0, 60);
	m_pTypeInfo[ITEM_CLASS_SMG][2].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	

	m_pTypeInfo[ITEM_CLASS_SMG][3].HName = "B-INTER";
	m_pTypeInfo[ITEM_CLASS_SMG][3].EName = "SMG-AM100";
	m_pTypeInfo[ITEM_CLASS_SMG][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetFrameID( 199, 203, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetDropFrameID( 199 );
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][3].SetValue(7000, -1, 22, 28, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][3].Price	= 55000;
	//m_pTypeInfo[ITEM_CLASS_SMG][3].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SMG][3].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	

	m_pTypeInfo[ITEM_CLASS_SMG][4].HName = "VK-45 靛贰帮敲扼捞";
	m_pTypeInfo[ITEM_CLASS_SMG][4].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetFrameID( 80, 14, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetDropFrameID( 80 );
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][4].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][4].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][4].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][4].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	
	m_pTypeInfo[ITEM_CLASS_SMG][5].HName = "MD-9 胶妮乔攫";
	m_pTypeInfo[ITEM_CLASS_SMG][5].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetFrameID( 197, 201, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetDropFrameID( 197 );
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][5].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][5].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][5].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][5].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	
	m_pTypeInfo[ITEM_CLASS_SMG][6].HName = "Smith 官捞矾胶";
	m_pTypeInfo[ITEM_CLASS_SMG][6].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetFrameID( 82, 11, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetDropFrameID( 82 );	
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][6].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][6].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][6].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][6].UseActionInfo = SKILL_ATTACK_GUN_SMG;
		
	m_pTypeInfo[ITEM_CLASS_SMG][7].HName = "P2K 捞瘤扼捞歹";
	m_pTypeInfo[ITEM_CLASS_SMG][7].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetFrameID( 83, 13, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetDropFrameID( 83 );		
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][7].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][7].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][7].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][7].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	
	m_pTypeInfo[ITEM_CLASS_SMG][8].HName = "MD-Z 力鸥";
	m_pTypeInfo[ITEM_CLASS_SMG][8].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetFrameID( 198, 202, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetDropFrameID( 198 );
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][8].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][8].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][8].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][8].UseActionInfo = SKILL_ATTACK_GUN_SMG;
		
	m_pTypeInfo[ITEM_CLASS_SMG][9].HName = "MD-000 饭厚酒藕";
	m_pTypeInfo[ITEM_CLASS_SMG][9].EName = "SMG-S2000";
	m_pTypeInfo[ITEM_CLASS_SMG][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetFrameID( 196, 200, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetDropFrameID( 196 );
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][9].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][9].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][9].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][9].UseActionInfo = SKILL_ATTACK_GUN_SMG;
	
	m_pTypeInfo[ITEM_CLASS_SMG][10].HName = "NS-32 匙捞厚";
	m_pTypeInfo[ITEM_CLASS_SMG][10].EName = "NS-32 Navy";
	m_pTypeInfo[ITEM_CLASS_SMG][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetFrameID( 356, 370, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetDropFrameID( 356 );
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][10].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][10].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][10].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][10].UseActionInfo = SKILL_ATTACK_GUN_SMG;

	m_pTypeInfo[ITEM_CLASS_SMG][11].HName = "OICW-敲傅芭";
	m_pTypeInfo[ITEM_CLASS_SMG][11].EName = "OICW-Flinger";
	m_pTypeInfo[ITEM_CLASS_SMG][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetFrameID( 439, 453, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetDropFrameID( 439 );
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][11].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][11].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][11].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][11].UseActionInfo = SKILL_ATTACK_GUN_SMG;

	m_pTypeInfo[ITEM_CLASS_SMG][12].HName = "KH-03A1";
	m_pTypeInfo[ITEM_CLASS_SMG][12].EName = "Mobius";
	m_pTypeInfo[ITEM_CLASS_SMG][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetFrameID( 472, 486, 0 );
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetDropFrameID( 472 );
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][12].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][12].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][12].UseActionInfo = SKILL_ATTACK_GUN_SMG;


	m_pTypeInfo[ITEM_CLASS_SMG][13].HName = "MG-90 角橇";
	m_pTypeInfo[ITEM_CLASS_SMG][13].EName = "MG-90 Shylph";
	m_pTypeInfo[ITEM_CLASS_SMG][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetFrameID(  904,927 ,0);
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetDropFrameID( 904 );
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SMG][13].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SMG][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMG][13].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SMG][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SMG][13].UseActionInfo = SKILL_ATTACK_GUN_SMG;




	//---------------------------------------------------------------------
	// ITEM_CLASS_AR
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), 荤沥芭府(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_AR, 15);

	m_pTypeInfo[ITEM_CLASS_AR][0].HName = "MK-74 厚寂胶";
	m_pTypeInfo[ITEM_CLASS_AR][0].EName = "AR-95";
	m_pTypeInfo[ITEM_CLASS_AR][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][0].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][0].SetFrameID( 84, 0, 0 );	
	m_pTypeInfo[ITEM_CLASS_AR][0].SetDropFrameID( 84 );
	m_pTypeInfo[ITEM_CLASS_AR][0].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][0].SetValue(850, -1, 11, 15, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][0].Weight = 1;	
	m_pTypeInfo[ITEM_CLASS_AR][0].Price	= 1700;
	//m_pTypeInfo[ITEM_CLASS_AR][0].SetRequireAbility(0, 20);
	m_pTypeInfo[ITEM_CLASS_AR][0].UseActionInfo = SKILL_ATTACK_GUN_AR;
	

	m_pTypeInfo[ITEM_CLASS_AR][1].HName = "P-1A 弃鄂";
	m_pTypeInfo[ITEM_CLASS_AR][1].EName = "AR-L591";
	m_pTypeInfo[ITEM_CLASS_AR][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][1].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][1].SetFrameID( 85, 1, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][1].SetDropFrameID( 85 );
	m_pTypeInfo[ITEM_CLASS_AR][1].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][1].SetValue(2150, -1, 15, 18, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][1].Price	= 6000;
	//m_pTypeInfo[ITEM_CLASS_AR][1].SetRequireAbility(0, 40);
	m_pTypeInfo[ITEM_CLASS_AR][1].UseActionInfo = SKILL_ATTACK_GUN_AR;
	

	m_pTypeInfo[ITEM_CLASS_AR][2].HName = "MK-101 备胶";
	m_pTypeInfo[ITEM_CLASS_AR][2].EName = "AR-LA";
	m_pTypeInfo[ITEM_CLASS_AR][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][2].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][2].SetFrameID( 86, 2, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][2].SetDropFrameID( 86 );
	m_pTypeInfo[ITEM_CLASS_AR][2].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][2].SetValue(3300, -1, 18, 22, 7, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][2].Price	= 20000;
	//m_pTypeInfo[ITEM_CLASS_AR][2].SetRequireAbility(0, 60);
	m_pTypeInfo[ITEM_CLASS_AR][2].UseActionInfo = SKILL_ATTACK_GUN_AR;	

	m_pTypeInfo[ITEM_CLASS_AR][3].HName = "MK-2 G2";
	m_pTypeInfo[ITEM_CLASS_AR][3].EName = "AR-C2000";
	m_pTypeInfo[ITEM_CLASS_AR][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][3].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][3].SetFrameID( 203, 207, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][3].SetDropFrameID( 203 );
	m_pTypeInfo[ITEM_CLASS_AR][3].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][3].SetValue(7500, -1, 22, 28, 7, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][3].Price	= 54000;
	//m_pTypeInfo[ITEM_CLASS_AR][3].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_AR][3].UseActionInfo = SKILL_ATTACK_GUN_AR;
	

	m_pTypeInfo[ITEM_CLASS_AR][4].HName = "MK-2000 G2000";
	m_pTypeInfo[ITEM_CLASS_AR][4].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][4].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][4].SetFrameID( 88, 3, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][4].SetDropFrameID( 88 );
	m_pTypeInfo[ITEM_CLASS_AR][4].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][4].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][4].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][4].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][4].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][5].HName = "P-61S 固匙福官";
	m_pTypeInfo[ITEM_CLASS_AR][5].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][5].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][5].SetFrameID( 87, 4, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][5].SetDropFrameID( 87 );
	m_pTypeInfo[ITEM_CLASS_AR][5].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][5].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][5].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][5].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][5].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][6].HName = "IS-200 腔府";
	m_pTypeInfo[ITEM_CLASS_AR][6].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][6].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][6].SetFrameID( 202, 206, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][6].SetDropFrameID( 202 );
	m_pTypeInfo[ITEM_CLASS_AR][6].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][6].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][6].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][6].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][6].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][7].HName = "MK-2002 况秦赣";
	m_pTypeInfo[ITEM_CLASS_AR][7].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][7].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][7].SetFrameID( 201, 205, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][7].SetDropFrameID( 201 );
	m_pTypeInfo[ITEM_CLASS_AR][7].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][7].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][7].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][7].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][7].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][8].HName = "P-38 家导";
	m_pTypeInfo[ITEM_CLASS_AR][8].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][8].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][8].SetFrameID( 205, 209, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][8].SetDropFrameID( 205 );
	m_pTypeInfo[ITEM_CLASS_AR][8].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][8].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][8].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][8].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][8].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][9].HName = "P-40 郡 某浇";
	m_pTypeInfo[ITEM_CLASS_AR][9].EName = "AR-NU01";
	m_pTypeInfo[ITEM_CLASS_AR][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][9].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][9].SetFrameID( 204, 208, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][9].SetDropFrameID( 204 );
	m_pTypeInfo[ITEM_CLASS_AR][9].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][9].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][9].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][9].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][9].UseActionInfo = SKILL_ATTACK_GUN_AR;
	
	m_pTypeInfo[ITEM_CLASS_AR][10].HName = "AR-颇捞绢胶盆";
	m_pTypeInfo[ITEM_CLASS_AR][10].EName = "AR-FireStorm";
	m_pTypeInfo[ITEM_CLASS_AR][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][10].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][10].SetFrameID( 357, 371, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][10].SetDropFrameID( 357 );
	m_pTypeInfo[ITEM_CLASS_AR][10].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][10].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][10].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][10].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][10].UseActionInfo = SKILL_ATTACK_GUN_AR;

	m_pTypeInfo[ITEM_CLASS_AR][11].HName = "AR-颇捞绢滚弊";
	m_pTypeInfo[ITEM_CLASS_AR][11].EName = "AR-FireBug";
	m_pTypeInfo[ITEM_CLASS_AR][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][11].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][11].SetFrameID( 440, 454, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][11].SetDropFrameID( 440 );
	m_pTypeInfo[ITEM_CLASS_AR][11].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][11].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][11].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][11].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][11].UseActionInfo = SKILL_ATTACK_GUN_AR;

	m_pTypeInfo[ITEM_CLASS_AR][12].HName = "EA-V1";
	m_pTypeInfo[ITEM_CLASS_AR][12].EName = "Angel Guard";
	m_pTypeInfo[ITEM_CLASS_AR][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][12].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][12].SetFrameID( 469, 483, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][12].SetDropFrameID( 469 );
	m_pTypeInfo[ITEM_CLASS_AR][12].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][12].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][12].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][12].UseActionInfo = SKILL_ATTACK_GUN_AR;

	m_pTypeInfo[ITEM_CLASS_AR][13].HName = "LM-700 墨扼";
	m_pTypeInfo[ITEM_CLASS_AR][13].EName = "LM-700 Kar";
	m_pTypeInfo[ITEM_CLASS_AR][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][13].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_AR][13].SetFrameID( 903, 926, 0 );
	m_pTypeInfo[ITEM_CLASS_AR][13].SetDropFrameID( 903 );
	m_pTypeInfo[ITEM_CLASS_AR][13].SetAddonFrameID( ADDONID_GUN_AR_MALE, ADDONID_GUN_AR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_AR][13].SetValue(10000, -1, 28, 33, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][13].Price	= 115000;
	//m_pTypeInfo[ITEM_CLASS_AR][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_AR][13].UseActionInfo = SKILL_ATTACK_GUN_AR;

	m_pTypeInfo[ITEM_CLASS_AR][14].HName = "官呕府柯";
	m_pTypeInfo[ITEM_CLASS_AR][14].EName = "Battalion";
	m_pTypeInfo[ITEM_CLASS_AR][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_AR][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_AR][14].SetFrameID( 953, 989, 0);
	m_pTypeInfo[ITEM_CLASS_AR][14].SetDropFrameID(953 );
	m_pTypeInfo[ITEM_CLASS_AR][14].SetAddonFrameID( ADDONID_GUN_SMG_MALE, ADDONID_GUN_SMG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_AR][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_AR][14].SetValue(9900, -1, 28, 35, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_AR][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_AR][14].Price = 0;
	m_pTypeInfo[ITEM_CLASS_AR][14].UseActionInfo = SKILL_ATTACK_GUN_SMG;

	//---------------------------------------------------------------------
	// ITEM_CLASS_SR
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), 荤沥芭府(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_SR, 15);

	m_pTypeInfo[ITEM_CLASS_SR][0].HName = "X-31M 剐扼畴";
	m_pTypeInfo[ITEM_CLASS_SR][0].EName = "TR-33";
	m_pTypeInfo[ITEM_CLASS_SR][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][0].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][0].SetFrameID( 89, 15, 0 );	
	m_pTypeInfo[ITEM_CLASS_SR][0].SetDropFrameID( 89 );
	m_pTypeInfo[ITEM_CLASS_SR][0].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][0].SetValue(900, -1, 10, 16, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][0].Price	= 1700;
	//m_pTypeInfo[ITEM_CLASS_SR][0].SetRequireAbility(0, 20);
	m_pTypeInfo[ITEM_CLASS_SR][0].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][1].HName = "X-55 V2";
	m_pTypeInfo[ITEM_CLASS_SR][1].EName = "TR-R59";
	m_pTypeInfo[ITEM_CLASS_SR][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][1].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][1].SetFrameID( 90, 16, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][1].SetDropFrameID( 90 );
	m_pTypeInfo[ITEM_CLASS_SR][1].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][1].SetValue(2300, -1, 16, 18, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][1].Price	= 6000;
	//m_pTypeInfo[ITEM_CLASS_SR][1].SetRequireAbility(0, 40);
	m_pTypeInfo[ITEM_CLASS_SR][1].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][2].HName = "其烽";
	m_pTypeInfo[ITEM_CLASS_SR][2].EName = "TR-L61";
	m_pTypeInfo[ITEM_CLASS_SR][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][2].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][2].SetFrameID( 92, 17, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][2].SetDropFrameID( 92 );
	m_pTypeInfo[ITEM_CLASS_SR][2].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][2].SetValue(4200, -1, 18, 23, 7, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][2].Price	= 20000;
	//m_pTypeInfo[ITEM_CLASS_SR][2].SetRequireAbility(0, 60);
	m_pTypeInfo[ITEM_CLASS_SR][2].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][3].HName = "X-45T 配付龋农";
	m_pTypeInfo[ITEM_CLASS_SR][3].EName = "TR-P99";
	m_pTypeInfo[ITEM_CLASS_SR][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][3].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][3].SetFrameID( 208, 212, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][3].SetDropFrameID( 208 );
	m_pTypeInfo[ITEM_CLASS_SR][3].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][3].SetValue(6000, -1, 23, 28, 7, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][3].Price	= 60000;
	//m_pTypeInfo[ITEM_CLASS_SR][3].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SR][3].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][4].HName = "IS-99 窍乔";
	m_pTypeInfo[ITEM_CLASS_SR][4].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][4].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][4].SetFrameID( 93, 18, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][4].SetDropFrameID( 93 );
	//m_pTypeInfo[ITEM_CLASS_SR][4].SetFrameID( 91, 19, 0 );
	//m_pTypeInfo[ITEM_CLASS_SR][4].SetDropFrameID( 91 );
	m_pTypeInfo[ITEM_CLASS_SR][4].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][4].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][4].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][4].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][4].UseActionInfo = SKILL_ATTACK_GUN_SR;
	
	m_pTypeInfo[ITEM_CLASS_SR][5].HName = "SR-1 喷胶配捞";
	m_pTypeInfo[ITEM_CLASS_SR][5].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][5].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][5].SetFrameID( 210, 214, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][5].SetDropFrameID( 210 );
	m_pTypeInfo[ITEM_CLASS_SR][5].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][5].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][5].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][5].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][5].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][6].HName = "GX90 圈飘";
	m_pTypeInfo[ITEM_CLASS_SR][6].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][6].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][6].SetFrameID( 209, 213, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][6].SetDropFrameID( 209 );
	m_pTypeInfo[ITEM_CLASS_SR][6].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][6].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][6].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][6].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][6].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][7].HName = "盆焊捞";
	m_pTypeInfo[ITEM_CLASS_SR][7].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][7].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][7].SetFrameID( 207, 211, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][7].SetDropFrameID( 207 );
	m_pTypeInfo[ITEM_CLASS_SR][7].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][7].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][7].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][7].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][7].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][8].HName = "窍具何荤";
	m_pTypeInfo[ITEM_CLASS_SR][8].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][8].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][8].SetFrameID( 91, 19, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][8].SetDropFrameID( 91 );
	m_pTypeInfo[ITEM_CLASS_SR][8].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][8].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][8].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][8].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][8].UseActionInfo = SKILL_ATTACK_GUN_SR;
	

	m_pTypeInfo[ITEM_CLASS_SR][9].HName = "IS-101 葛单扼配";
	m_pTypeInfo[ITEM_CLASS_SR][9].EName = "TR-TS900";
	m_pTypeInfo[ITEM_CLASS_SR][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][9].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][9].SetFrameID( 206, 210, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][9].SetDropFrameID( 206 );
	m_pTypeInfo[ITEM_CLASS_SR][9].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][9].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][9].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][9].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][9].UseActionInfo = SKILL_ATTACK_GUN_SR;
	
	m_pTypeInfo[ITEM_CLASS_SR][10].HName = "BFG-50";
	m_pTypeInfo[ITEM_CLASS_SR][10].EName = "BFG-50";
	m_pTypeInfo[ITEM_CLASS_SR][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][10].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][10].SetFrameID( 359, 373, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][10].SetDropFrameID( 359 );
	m_pTypeInfo[ITEM_CLASS_SR][10].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][10].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][10].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][10].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][10].UseActionInfo = SKILL_ATTACK_GUN_SR;
	
	m_pTypeInfo[ITEM_CLASS_SR][11].HName = "BFG-50";
	m_pTypeInfo[ITEM_CLASS_SR][11].EName = "BFG-50";
	m_pTypeInfo[ITEM_CLASS_SR][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][11].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][11].SetFrameID( 441, 455, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][11].SetDropFrameID( 441 );
	m_pTypeInfo[ITEM_CLASS_SR][11].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][11].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][11].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][11].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][11].UseActionInfo = SKILL_ATTACK_GUN_SR;

	m_pTypeInfo[ITEM_CLASS_SR][12].HName = "T-03";
	m_pTypeInfo[ITEM_CLASS_SR][12].EName = "Nightmare";
	m_pTypeInfo[ITEM_CLASS_SR][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][12].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][12].SetFrameID( 471, 485, 0 );
	m_pTypeInfo[ITEM_CLASS_SR][12].SetDropFrameID( 471 );
	m_pTypeInfo[ITEM_CLASS_SR][12].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][12].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][12].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][12].UseActionInfo = SKILL_ATTACK_GUN_SR;


		m_pTypeInfo[ITEM_CLASS_SR][13].HName = "v-03 纳葛胶";
	m_pTypeInfo[ITEM_CLASS_SR][13].EName = "V-03 Chmos";
	m_pTypeInfo[ITEM_CLASS_SR][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][13].SetSoundID( SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUND_ITEM_MOVE_GUN, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SR][13].SetFrameID( 906, 929 ,0 );
	m_pTypeInfo[ITEM_CLASS_SR][13].SetDropFrameID( 906 );
	m_pTypeInfo[ITEM_CLASS_SR][13].SetAddonFrameID( ADDONID_GUN_SR_MALE, ADDONID_GUN_SR_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][13].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][13].SetValue(10500, -1, 30, 35, 8, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][13].Price	= 140000;
	//m_pTypeInfo[ITEM_CLASS_SR][12].SetRequireAbility(0, 90);
	m_pTypeInfo[ITEM_CLASS_SR][13].UseActionInfo = SKILL_ATTACK_GUN_SR;


	m_pTypeInfo[ITEM_CLASS_SR][14].HName = "琶萍拿 胶墨快飘";
	m_pTypeInfo[ITEM_CLASS_SR][14].EName = "Tactical Scout";
	m_pTypeInfo[ITEM_CLASS_SR][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_SR][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_SR][14].SetFrameID( 955, 991, 0);
	m_pTypeInfo[ITEM_CLASS_SR][14].SetDropFrameID(955 );
	m_pTypeInfo[ITEM_CLASS_SR][14].SetAddonFrameID( ADDONID_GUN_SG_MALE, ADDONID_GUN_SG_FEMALE );
	m_pTypeInfo[ITEM_CLASS_SR][14].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_SR][14].SetValue(10000, -1, 32, 40, 6, -1, 0);
	m_pTypeInfo[ITEM_CLASS_SR][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SR][14].Price	= 110000;
	//m_pTypeInfo[ITEM_CLASS_SG][12].SetRequireAbility(0, 80);
	m_pTypeInfo[ITEM_CLASS_SR][14].UseActionInfo = SKILL_ATTACK_GUN_SG;
	


	//---------------------------------------------------------------------
	// ITEM_CLASS_BOMB
	//---------------------------------------------------------------------
	// MinDam(1)~MaxDam(2)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_BOMB, 5 );

	m_pTypeInfo[ITEM_CLASS_BOMB][0].HName = "胶敲赴磐";
	m_pTypeInfo[ITEM_CLASS_BOMB][0].EName = "Splinter";
	m_pTypeInfo[ITEM_CLASS_BOMB][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][0].SetFrameID( 245, 250, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][0].SetDropFrameID( 245 );
	m_pTypeInfo[ITEM_CLASS_BOMB][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BOMB][0].SetValue(5, 10);
	m_pTypeInfo[ITEM_CLASS_BOMB][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][0].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][0].UseActionInfo = BOMB_SPLINTER;

	m_pTypeInfo[ITEM_CLASS_BOMB][1].HName = "俊捞辑";
	m_pTypeInfo[ITEM_CLASS_BOMB][1].EName = "Acer";
	m_pTypeInfo[ITEM_CLASS_BOMB][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][1].SetFrameID( 246, 252, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][1].SetDropFrameID( 246 );
	m_pTypeInfo[ITEM_CLASS_BOMB][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BOMB][1].SetValue(7, 14);
	m_pTypeInfo[ITEM_CLASS_BOMB][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][1].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][1].UseActionInfo = BOMB_ACER;

	m_pTypeInfo[ITEM_CLASS_BOMB][2].HName = "阂胶";
	m_pTypeInfo[ITEM_CLASS_BOMB][2].EName = "Bulls";
	m_pTypeInfo[ITEM_CLASS_BOMB][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][2].SetFrameID( 247, 254, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][2].SetDropFrameID( 247 );
	m_pTypeInfo[ITEM_CLASS_BOMB][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BOMB][2].SetValue(12, 20);
	m_pTypeInfo[ITEM_CLASS_BOMB][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][2].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][2].UseActionInfo = BOMB_BULLS;

	m_pTypeInfo[ITEM_CLASS_BOMB][3].HName = "胶畔";
	m_pTypeInfo[ITEM_CLASS_BOMB][3].EName = "Stun";
	m_pTypeInfo[ITEM_CLASS_BOMB][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][3].SetFrameID( 252, 264, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][3].SetDropFrameID( 252 );
	m_pTypeInfo[ITEM_CLASS_BOMB][3].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB][3].SetValue(18, 23);
	m_pTypeInfo[ITEM_CLASS_BOMB][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][3].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][3].UseActionInfo = BOMB_STUN;

	m_pTypeInfo[ITEM_CLASS_BOMB][4].HName = "农肺胶焊快";
	m_pTypeInfo[ITEM_CLASS_BOMB][4].EName = "Crossbow";
	m_pTypeInfo[ITEM_CLASS_BOMB][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][4].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][4].SetFrameID( 248, 256, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][4].SetDropFrameID( 248 );
	m_pTypeInfo[ITEM_CLASS_BOMB][4].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB][4].SetValue(20, 30);
	m_pTypeInfo[ITEM_CLASS_BOMB][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][4].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][4].UseActionInfo = BOMB_CROSSBOW;

	/*
	m_pTypeInfo[ITEM_CLASS_BOMB][5].HName = "飘困胶磐";
	m_pTypeInfo[ITEM_CLASS_BOMB][5].EName = "Twister";
	m_pTypeInfo[ITEM_CLASS_BOMB][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_BOMB][5].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BOMB][5].SetFrameID( 99, 148, 0 );	
	m_pTypeInfo[ITEM_CLASS_BOMB][5].SetDropFrameID( 99 );
	m_pTypeInfo[ITEM_CLASS_BOMB][5].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_BOMB][5].SetValue(28, 35);
	m_pTypeInfo[ITEM_CLASS_BOMB][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BOMB][5].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_BOMB][5].UseActionInfo = BOMB_TWISTER;
	*/

	//---------------------------------------------------------------------
	// ITEM_CLASS_MINE
	//---------------------------------------------------------------------
	// MinDam(1)~MaxDam(2)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_MINE, 5 );
	
	m_pTypeInfo[ITEM_CLASS_MINE][0].HName = "巨努懦矾";
	m_pTypeInfo[ITEM_CLASS_MINE][0].EName = "AnkleKiller";
	m_pTypeInfo[ITEM_CLASS_MINE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_MINE][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MINE][0].SetFrameID( 270, 284, 0 );	
	m_pTypeInfo[ITEM_CLASS_MINE][0].SetDropFrameID( 270 );
	m_pTypeInfo[ITEM_CLASS_MINE][0].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_MINE][0].SetValue(12, 20);
	m_pTypeInfo[ITEM_CLASS_MINE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MINE][0].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_MINE][0].UseActionInfo = MINE_ANKLE_KILLER;

	m_pTypeInfo[ITEM_CLASS_MINE][1].HName = "汽令";
	m_pTypeInfo[ITEM_CLASS_MINE][1].EName = "Pomz";
	m_pTypeInfo[ITEM_CLASS_MINE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_MINE][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MINE][1].SetFrameID( 249, 258, 0 );	
	m_pTypeInfo[ITEM_CLASS_MINE][1].SetDropFrameID( 249 );
	m_pTypeInfo[ITEM_CLASS_MINE][1].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_MINE][1].SetValue(18, 25);
	m_pTypeInfo[ITEM_CLASS_MINE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MINE][1].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_MINE][1].UseActionInfo = MINE_POMZ;

	m_pTypeInfo[ITEM_CLASS_MINE][2].HName = "AP-C1";
	m_pTypeInfo[ITEM_CLASS_MINE][2].EName = "AP-C1";
	m_pTypeInfo[ITEM_CLASS_MINE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_MINE][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MINE][2].SetFrameID( 253, 265, 0 );	
	m_pTypeInfo[ITEM_CLASS_MINE][2].SetDropFrameID( 253 );
	m_pTypeInfo[ITEM_CLASS_MINE][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_MINE][2].SetValue(22, 27);
	m_pTypeInfo[ITEM_CLASS_MINE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MINE][2].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_MINE][2].UseActionInfo = MINE_AP_C1;

	m_pTypeInfo[ITEM_CLASS_MINE][3].HName = "促捞酒阁靛归";
	m_pTypeInfo[ITEM_CLASS_MINE][3].EName = "DiamondBack";
	m_pTypeInfo[ITEM_CLASS_MINE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_MINE][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MINE][3].SetFrameID( 251, 262, 0 );	
	m_pTypeInfo[ITEM_CLASS_MINE][3].SetDropFrameID( 251 );
	m_pTypeInfo[ITEM_CLASS_MINE][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_MINE][3].SetValue(25, 35);
	m_pTypeInfo[ITEM_CLASS_MINE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MINE][3].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_MINE][3].UseActionInfo = MINE_DIAMONDBACK;

	m_pTypeInfo[ITEM_CLASS_MINE][4].HName = "Swift-EX";
	m_pTypeInfo[ITEM_CLASS_MINE][4].EName = "Swift-EX";
	m_pTypeInfo[ITEM_CLASS_MINE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_MINE][4].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MINE][4].SetFrameID( 250, 260, 0 );	
	m_pTypeInfo[ITEM_CLASS_MINE][4].SetDropFrameID( 250 );
	m_pTypeInfo[ITEM_CLASS_MINE][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_MINE][4].SetValue(25, 35);
	m_pTypeInfo[ITEM_CLASS_MINE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MINE][4].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_MINE][4].UseActionInfo = MINE_SWIFT_EX;

	//---------------------------------------------------------------------
	// ITEM_CLASS_BELT
	//---------------------------------------------------------------------
	// 郴备己(?), Protection(2), 器南荐(3), Def(6)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_BELT, 8 );

	m_pTypeInfo[ITEM_CLASS_BELT][0].HName = "骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][0].EName = "Belt";
	m_pTypeInfo[ITEM_CLASS_BELT][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][0].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][0].SetFrameID( 104, 90, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][0].SetDropFrameID( 104 );
	m_pTypeInfo[ITEM_CLASS_BELT][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][0].SetValue(1000, 0, 2, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][0].Price	= 2000;

	m_pTypeInfo[ITEM_CLASS_BELT][1].HName = "骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][1].EName = "Belt(4)";
	m_pTypeInfo[ITEM_CLASS_BELT][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][1].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][1].SetFrameID( 105, 91, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][1].SetDropFrameID( 105 );
	m_pTypeInfo[ITEM_CLASS_BELT][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][1].SetValue(2000, 0, 4, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][1].Price	= 10000;

	m_pTypeInfo[ITEM_CLASS_BELT][2].HName = "骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][2].EName = "Belt(6)";
	m_pTypeInfo[ITEM_CLASS_BELT][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][2].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][2].SetFrameID( 106, 92, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][2].SetDropFrameID( 106 );
	m_pTypeInfo[ITEM_CLASS_BELT][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][2].SetValue(3000, 0, 6, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][2].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_BELT][3].HName = "骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][3].EName = "Belt(8)";
	m_pTypeInfo[ITEM_CLASS_BELT][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][3].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][3].SetFrameID( 107, 93, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][3].SetDropFrameID( 107 );
	m_pTypeInfo[ITEM_CLASS_BELT][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][3].SetValue(4000, 0, 8, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][3].Price	= 250000;

	m_pTypeInfo[ITEM_CLASS_BELT][4].HName = "V-骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][4].EName = "v-belt";
	m_pTypeInfo[ITEM_CLASS_BELT][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][4].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][4].SetFrameID( 351, 365, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][4].SetDropFrameID( 351 );
	m_pTypeInfo[ITEM_CLASS_BELT][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][4].SetValue(4000, 0, 8, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][4].Price	= 250000;

	m_pTypeInfo[ITEM_CLASS_BELT][5].HName = "况 骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][5].EName = "War Belt";
	m_pTypeInfo[ITEM_CLASS_BELT][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][5].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][5].SetFrameID( 449, 463, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][5].SetDropFrameID( 449 );
	m_pTypeInfo[ITEM_CLASS_BELT][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][5].SetValue(4000, 0, 8, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][5].Price	= 250000;

	m_pTypeInfo[ITEM_CLASS_BELT][6].HName = "捡歹 骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][6].EName = "Shoulder Belt";
	m_pTypeInfo[ITEM_CLASS_BELT][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][6].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][6].SetFrameID( 478, 492, 0 );	
	m_pTypeInfo[ITEM_CLASS_BELT][6].SetDropFrameID( 478 );
	m_pTypeInfo[ITEM_CLASS_BELT][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][6].SetValue(4000, 0, 8, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][6].Price	= 250000;

	m_pTypeInfo[ITEM_CLASS_BELT][7].HName = "傀胶畔  骇飘";
	m_pTypeInfo[ITEM_CLASS_BELT][7].EName = "Western Belt";
	m_pTypeInfo[ITEM_CLASS_BELT][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_BELT][7].SetSoundID( SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUND_ITEM_MOVE_BELT, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_BELT][7].SetFrameID( 926 , 949 ,0);	
	m_pTypeInfo[ITEM_CLASS_BELT][7].SetDropFrameID( 920 );
	m_pTypeInfo[ITEM_CLASS_BELT][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BELT][7].SetValue(4000, 0, 8, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_BELT][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BELT][7].Price	= 250000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_LEARNINGITEM
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_LEARNINGITEM, 9 );

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].HName = "胶农费";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].EName = "Scroll";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].SetFrameID( 108, 70, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].SetDropFrameID( 108 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].HName = "哪腔磐 叼胶南";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].EName = "Diskette";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].SetFrameID( 109, 65, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].SetDropFrameID( 109 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].HName = "笼 靛扼捞宏";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].EName = "Zip Drive";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].SetFrameID( 110, 66, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].SetDropFrameID( 110 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].HName = "CD";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].EName = "CD";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].SetFrameID( 111, 64, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].SetDropFrameID( 111 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].HName = "轨何 乔胶";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].EName = "Trite Bamboo";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].SetFrameID( 112, 68, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].SetDropFrameID( 112 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][4].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].HName = "坷府浚呕 合";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].EName = "Oriental Book";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].SetFrameID( 113, 67, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].SetDropFrameID( 113 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][5].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].HName = "官捞喉";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].EName = "Bible";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].SetFrameID( 114, 69, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].SetDropFrameID( 114 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][6].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].HName = "俊捞记飘 合";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].EName = "Ancient Book";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].SetFrameID( 115, 71, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].SetDropFrameID( 115 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][7].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].HName = "颇老";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].EName = "File";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_USE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].SetFrameID( 116, 72, 0 );	
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].SetDropFrameID( 116 );
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LEARNINGITEM][8].Price	= 0;



	//---------------------------------------------------------------------
	// ITEM_CLASS_MONEY
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_MONEY, 3 );

	m_pTypeInfo[ITEM_CLASS_MONEY][0].HName = "饭捞";
	m_pTypeInfo[ITEM_CLASS_MONEY][0].EName = "Lei";
	m_pTypeInfo[ITEM_CLASS_MONEY][0].Description = "浇饭捞绢狼 拳企";
	m_pTypeInfo[ITEM_CLASS_MONEY][0].SetSoundID( SOUND_ITEM_MOVE_MONEY, SOUND_ITEM_MOVE_MONEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MONEY][0].SetFrameID( 117, 37, 0 );	
	m_pTypeInfo[ITEM_CLASS_MONEY][0].SetDropFrameID( 117 );
	m_pTypeInfo[ITEM_CLASS_MONEY][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MONEY][0].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_MONEY][1].HName = "爸靛";
	m_pTypeInfo[ITEM_CLASS_MONEY][1].EName = "Geld";
	m_pTypeInfo[ITEM_CLASS_MONEY][1].Description = "轨颇捞绢狼 拳企";
	m_pTypeInfo[ITEM_CLASS_MONEY][1].SetSoundID( SOUND_ITEM_MOVE_MONEY, SOUND_ITEM_MOVE_MONEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MONEY][1].SetFrameID( 304, 318, 0 );	
	m_pTypeInfo[ITEM_CLASS_MONEY][1].SetDropFrameID( 304 );
	m_pTypeInfo[ITEM_CLASS_MONEY][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MONEY][1].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_MONEY][2].HName = "捣";
	m_pTypeInfo[ITEM_CLASS_MONEY][2].EName = "Money";
	m_pTypeInfo[ITEM_CLASS_MONEY][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_MONEY][2].SetSoundID( SOUND_ITEM_MOVE_MONEY, SOUND_ITEM_MOVE_MONEY, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MONEY][2].SetFrameID( 681, 695, 0 );	
	m_pTypeInfo[ITEM_CLASS_MONEY][2].SetDropFrameID( 681 );
	m_pTypeInfo[ITEM_CLASS_MONEY][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MONEY][2].Weight = 1;

//	m_pTypeInfo[ITEM_CLASS_MONEY][3].HName = "磊靛";
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].EName = "Zard";
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].Description = "";
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].SetSoundID( SOUND_ITEM_MOVE_MONEY, SOUND_ITEM_MOVE_MONEY, SOUNDID_NULL, SOUNDID_NULL );
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].SetFrameID( 118, 39, 0 );	
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].SetDropFrameID( 118 );
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_MONEY][3].Weight = 1;

	//---------------------------------------------------------------------
	// ITEM_CLASS_CORPSE
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_CORPSE, 1 );

	m_pTypeInfo[ITEM_CLASS_CORPSE][0].HName = "矫眉";
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].EName = "Corpse";
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].SetFrameID( 0, 500, 0 );	
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].SetDropFrameID( 0 );
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CORPSE][0].Weight = 1;


	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_RING
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_RING, 14);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].HName = "酒捞攫 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].EName = "Iron Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].SetFrameID( 134, 78, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].SetDropFrameID( 134 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].SetValue(250, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][0].Price	= 1000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].HName = "角滚 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].EName = "Silver Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].SetFrameID( 138, 79, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].SetDropFrameID( 138 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].SetValue(300, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][1].Price	= 2500;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].HName = "喇努 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].EName = "Winkle Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].SetFrameID( 137, 80, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].SetDropFrameID( 137 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].SetValue(410, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][2].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].HName = "辨飘 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].EName = "Gilt Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].SetFrameID( 136, 81, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].SetDropFrameID( 136 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].SetValue(550, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][3].Price	= 10000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].HName = "矩萍 姐 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].SetFrameID( 135, 82, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].SetDropFrameID( 135 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][4].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].HName = "喉风 矫掂急";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].SetFrameID( 166, 170, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].SetDropFrameID( 166 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][5].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].HName = "喉矾靛 辑目";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].SetFrameID( 167, 171, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].SetDropFrameID( 167 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][6].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].HName = "攫圈府 蒋靛";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].SetFrameID( 168, 172, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].SetDropFrameID( 168 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][7].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].HName = "傅 坷宏 胶拿令";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].SetFrameID( 170, 174, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].SetDropFrameID( 170 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][8].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].HName = "胶颇捞歹 酒捞令";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].EName = "Anti Sun Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].SetFrameID( 169, 173, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].SetDropFrameID( 169 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][9].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].HName = "葛呕 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].EName = "Mortal Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].SetFrameID( 362, 376, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].SetDropFrameID( 362 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][10].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].HName = "奇副 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].EName = "Fenrir Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].SetFrameID( 459, 473, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].SetDropFrameID( 459 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][11].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].HName = "酒胶橇 傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].EName = "Asp Ring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].SetFrameID( 486, 500, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].SetDropFrameID( 486 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][12].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].HName = "舅泛魂靛扼捞飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].EName = "Alexandrite";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].SetFrameID( 924 , 947 ,0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].SetDropFrameID( 924 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].SetValue(700, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_RING][13].Price	= 50000;
	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_BRACELET
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_BRACELET, 13);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].HName = "饭捞胶 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].EName = "Lace Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].SetFrameID( 141, 86, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].SetDropFrameID( 141 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].SetValue(300, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][0].Price	= 1000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].HName = "酒赣府倔 喉饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].EName = "Armorial Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].SetFrameID( 140, 85, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].SetDropFrameID( 140 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].SetValue(450, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][1].Price	= 2500;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].HName = "陵溃 喉饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].EName = "Jewel Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].SetFrameID( 142, 87, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].SetDropFrameID( 142 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].SetValue(600, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][2].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].HName = "付胶磐 喉饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].SetFrameID( 139, 88, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].SetDropFrameID( 139 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][3].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].HName = "葛芭唱捞飘 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].SetFrameID( 175, 179, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].SetDropFrameID( 175 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][4].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].HName = "喉风 葛畴酒捞";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].SetFrameID( 174, 178, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].SetDropFrameID( 174 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][5].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].HName = "喉风况磐";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].SetFrameID( 173, 177, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].SetDropFrameID( 173 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][6].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].HName = "官捞欺 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].SetFrameID( 171, 175, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].SetDropFrameID( 171 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][7].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].HName = "绢厚胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].EName = "Master Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].SetFrameID( 172, 176, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].SetDropFrameID( 172 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][8].Price	= 12000;
	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].HName = "绢其乔 宏饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].EName = "Apepi Braclet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].SetFrameID( 361, 375, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].SetDropFrameID( 361 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][9].Price	= 12000;
	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].HName = "挪 喉饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].EName = "Keen Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].SetFrameID( 458, 472, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].SetDropFrameID( 458 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][10].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].HName = "扼乔胶扼榴府 喉饭捞浇复";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].EName = "Lapis Lazuli Bracelet";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].SetFrameID( 487, 501, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].SetDropFrameID( 487 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][11].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].HName = "骇扼捣唱";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].EName = "Belladonna";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].SetSoundID( SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUND_ITEM_MOVE_BRACELET, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].SetFrameID( 915 , 938 ,0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].SetDropFrameID( 915 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].SetValue(800, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_BRACELET][12].Price	= 12000;
	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_NECKLACE
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_NECKLACE, 14);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].HName = "何皋尔 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].EName = "Boomerang Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].SetFrameID( 129, 73, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].SetDropFrameID( 129 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].SetValue(200, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][0].Price	= 2000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].HName = "胶拿 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].EName = "Skull Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].SetFrameID( 132, 74, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].SetDropFrameID( 132 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].SetValue(300, 2, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][1].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].HName = "目胶乔靛 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].EName = "Spid Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].SetFrameID( 133, 77, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].SetDropFrameID( 133 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].SetValue(450, 3, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][2].Price	= 12000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].HName = "连 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].EName = "Jet Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].SetFrameID( 130, 76, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].SetDropFrameID( 130 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].SetValue(700, 4, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][3].Price	= 50000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].HName = "酒捞倒 匙农府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].SetFrameID( 131, 75, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].SetDropFrameID( 131 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][4].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].HName = "矩萍 农肺胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].SetFrameID( 180, 181, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].SetDropFrameID( 180 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][5].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].HName = "单胶 胶鸥";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].SetFrameID( 179, 182, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].SetDropFrameID( 179 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][6].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].HName = "坷膏 酒鼓房";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].SetFrameID( 178, 183, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].SetDropFrameID( 178 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][7].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].HName = "喉发 巨农";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].SetFrameID( 176, 180, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].SetDropFrameID( 176 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][8].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].HName = "磷篮 磊狼 朝俺";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].EName = "Idol Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].SetFrameID( 177, 184, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].SetDropFrameID( 177 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][9].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].HName = "榜电喇";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].EName = "Golden Wings";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].SetFrameID( 360, 374, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].SetDropFrameID( 360 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][10].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].HName = "捞喉 酒捞";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].EName = "Evil Eye";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].SetFrameID( 457, 471, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].SetDropFrameID( 457 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][11].Price	= 150000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].HName = "颇橇聪福 匙努府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].EName = "Fegfnir Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].SetFrameID( 489, 503, 0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].SetDropFrameID( 489 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][12].Price	= 150000;

		m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].HName = "快磊飘  匙努府胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].EName = "Uzat Necklace";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].SetSoundID( SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUND_ITEM_MOVE_NECKLACE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].SetFrameID( 925 , 948 ,0 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].SetDropFrameID( 925 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_NECKLACE][13].Price	= 150000;

	
	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_COAT
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(2), Def(6)
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_COAT, 22);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].HName = "胶喇 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].EName = "Swing Coat";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetFrameID( 144, 83, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetDropFrameID( 144 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetAddonFrameID( 2, 3 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][0].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].HName = "官叼 酱飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].EName = "Body Suit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetFrameID( 145, 84, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetDropFrameID( 145 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetAddonFrameID( 2, 3 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][1].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].HName = "橇废 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].EName = "Frock Coat";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetFrameID( 144, 83, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetDropFrameID( 144 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetAddonFrameID( 2, 3 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][2].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].HName = "痢橇 酱飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].EName = "Jump Suit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetFrameID( 145, 84, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetDropFrameID( 145 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetAddonFrameID( 2, 3 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][3].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].HName = "浇府欺府 肺宏";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].EName = "Slipery Robe";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetFrameID( 462, 476, 0 );//( 402, 416, 0);//	//
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetDropFrameID( 462 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetAddonFrameID( 514, 515 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][4].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].HName = "敲冯 努废";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].EName = "Plume Cloak";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetFrameID( 403, 417, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetDropFrameID( 403 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetAddonFrameID( 514, 515 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][5].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].HName = "窍捞爹 肺宏";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].EName = "Hiding Robe";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetFrameID( 462, 476, 0 );//( 402, 416, 0);	//	//
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetDropFrameID( 462 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetAddonFrameID( 514, 515 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][6].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].HName = "饭厚抛捞飘叼 努废";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].EName = "Levitated Cloak";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetFrameID( 403, 417, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetDropFrameID( 403 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetAddonFrameID( 514, 515 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][7].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].HName = "救萍-姐 弃";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].EName = "Anti-Sun Pall";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetFrameID( 430, 444, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetDropFrameID( 430 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][8].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].HName = "救萍-姐 内橇";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].EName = "Anti-Sun Cope";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetFrameID( 431, 445, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetDropFrameID( 431 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][9].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].HName = "付胶磐 弃";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].EName = "Master Pall";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetFrameID( 430, 444, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetDropFrameID( 430 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][10].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].HName = "付胶磐 内橇";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].EName = "Master Cope";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetFrameID( 431, 445, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetDropFrameID( 431 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][11].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].HName = "目盖歹 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].EName = "Commander Coat M";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetFrameID( 363, 377, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetDropFrameID( 363 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][12].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].HName = "目盖歹 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].EName = "Commander Coat W";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetFrameID( 364, 378, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetDropFrameID( 364 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][13].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].HName = "喉矾靛 弃";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].EName = "Blood pall";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetFrameID( 454, 468, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetDropFrameID( 454 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][14].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].HName = "喉矾靛 倪";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].EName = "Blood cope";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetFrameID( 455, 469, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetDropFrameID( 455 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][15].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].HName = "肺府墨 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].EName = "Lorica Coat M";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetFrameID( 484, 498, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetDropFrameID( 484 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][16].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].HName = "肺府墨 内飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].EName = "Lorica Coat W";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetFrameID( 483, 497, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetDropFrameID( 483 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][17].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].HName = "酒涅瓢 内飘-M";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].EName = "Aqueton Coat M";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetFrameID( 922 , 945 ,0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetDropFrameID( 922 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][18].Weight = 1;


	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].HName = "酒涅瓢 内飘-W";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].EName = "Aqueton Coat W";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetSoundID( SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUND_ITEM_MOVE_CLOTH, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetFrameID( 923 , 946 ,0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetDropFrameID( 923 );	// 145绰 咯磊渴
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][19].Weight = 1;//	// 轨颇 内飘 2窜 烙矫阜扁侩

	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].HName = "叼颇捞攫胶 努废";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].EName = "Defiance cloak";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetFrameID( 944, 980, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetDropFrameID(944 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][20].Weight = 1;//	// 轨颇 内飘 2窜 烙矫阜扁侩

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].HName = "舅风绢 绢其凡";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].EName = "Allure apparel";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetFrameID( 945, 981, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetDropFrameID(945 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetAddonFrameID( 558, 559 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].SetValue(880, 5, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][21].Weight = 1;//	// 轨颇 内飘 2窜 烙矫阜扁侩

//	m_pTypeInfo[ITEM_CLASS_VESTON][2].HName = "捞葛饿 努废";
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].EName = "Immoral cloak";
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].Description = "";
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_VESTON][2].Price = 0;
//
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].HName = "橇扼快靛 绢其凡";
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].EName = "Proud apparel";
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].Description = "";
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_VESTON][3].Price = 0;



//	for(int i = 4; i < 14; i+=2)
//	{
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i].SetFrameID( 144, 83, 0 );	
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i].SetDropFrameID( 144 );	// 145绰 咯磊渴
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i].SetAddonFrameID( 2, 3 );
//
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i+1].SetFrameID( 145, 84, 0 );	
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i+1].SetDropFrameID( 145 );	// 145绰 咯磊渴
//		m_pTypeInfo[ITEM_CLASS_VAMPIRE_COAT][i+1].SetAddonFrameID( 2, 3 );
//	}

	//---------------------------------------------------------------------
	// ITEM_CLASS_SKULL
	//---------------------------------------------------------------------
	// 
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_SKULL, 73);

	m_pTypeInfo[ITEM_CLASS_SKULL][0].HName = "秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][0].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][0].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][0].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][0].SetDropFrameID( 120 );
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_SHOES][0].SetAddonFrameID( 0, 0 );
	m_pTypeInfo[ITEM_CLASS_SKULL][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][0].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][1].HName = "歹萍胶飘扼捞歹";
	m_pTypeInfo[ITEM_CLASS_SKULL][1].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][1].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][1].SetFrameID( 211, 215, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][1].SetDropFrameID( 211 );
	m_pTypeInfo[ITEM_CLASS_SKULL][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][1].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][2].HName = "喉矾靛况废";
	m_pTypeInfo[ITEM_CLASS_SKULL][2].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][2].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][2].SetFrameID( 212, 216, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][2].SetDropFrameID( 212 );
	m_pTypeInfo[ITEM_CLASS_SKULL][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][2].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][3].HName = "孤畔飘";
	m_pTypeInfo[ITEM_CLASS_SKULL][3].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][3].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][3].SetFrameID( 213, 217, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][3].SetDropFrameID( 213 );
	m_pTypeInfo[ITEM_CLASS_SKULL][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][3].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][4].HName = "磐醋家匡";
	m_pTypeInfo[ITEM_CLASS_SKULL][4].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][4].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][4].SetFrameID( 214, 218, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][4].SetDropFrameID( 214 );
	m_pTypeInfo[ITEM_CLASS_SKULL][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][4].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][5].HName = "磐醋单靛";
	m_pTypeInfo[ITEM_CLASS_SKULL][5].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][5].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][5].SetFrameID( 215, 219, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][5].SetDropFrameID( 215 );
	m_pTypeInfo[ITEM_CLASS_SKULL][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][5].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][6].HName = "虐靛";
	m_pTypeInfo[ITEM_CLASS_SKULL][6].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][6].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][6].SetFrameID( 216, 220, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][6].SetDropFrameID( 216 );
	m_pTypeInfo[ITEM_CLASS_SKULL][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][6].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][7].HName = "母凭";
	m_pTypeInfo[ITEM_CLASS_SKULL][7].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][7].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][7].SetFrameID( 217, 221, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][7].SetDropFrameID( 217 );
	m_pTypeInfo[ITEM_CLASS_SKULL][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][7].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][8].HName = "舅沫";
	m_pTypeInfo[ITEM_CLASS_SKULL][8].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][8].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][8].SetFrameID( 218, 222, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][8].SetDropFrameID( 218 );
	m_pTypeInfo[ITEM_CLASS_SKULL][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][8].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][9].HName = "贾廉";
	m_pTypeInfo[ITEM_CLASS_SKULL][9].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][9].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][9].SetFrameID( 219, 223, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][9].SetDropFrameID( 219 );
	m_pTypeInfo[ITEM_CLASS_SKULL][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][9].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][10].HName = "饭靛酒捞";
	m_pTypeInfo[ITEM_CLASS_SKULL][10].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][10].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][10].SetFrameID( 220, 224, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][10].SetDropFrameID( 220 );
	m_pTypeInfo[ITEM_CLASS_SKULL][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][10].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][11].HName = "单靛官叼";
	m_pTypeInfo[ITEM_CLASS_SKULL][11].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][11].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][11].SetFrameID( 221, 225, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][11].SetDropFrameID( 221 );
	m_pTypeInfo[ITEM_CLASS_SKULL][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][11].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][12].HName = "榜电胶拿";
	m_pTypeInfo[ITEM_CLASS_SKULL][12].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][12].SetSoundID( SOUND_EVENT_FANFARE, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][12].SetFrameID( 225, 229, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][12].SetDropFrameID( 225 );
	m_pTypeInfo[ITEM_CLASS_SKULL][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][12].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][13].HName = "磊荐沥胶拿";
	m_pTypeInfo[ITEM_CLASS_SKULL][13].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][13].SetSoundID( SOUND_EVENT_FANFARE, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][13].SetFrameID( 226, 230, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][13].SetDropFrameID( 226 );
	m_pTypeInfo[ITEM_CLASS_SKULL][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][13].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][14].HName = "喉发胶拿";
	m_pTypeInfo[ITEM_CLASS_SKULL][14].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][14].SetSoundID( SOUND_EVENT_FANFARE, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][14].SetFrameID( 227, 231, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][14].SetDropFrameID( 227 );
	m_pTypeInfo[ITEM_CLASS_SKULL][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][14].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][15].HName = "农府胶呕胶拿";
	m_pTypeInfo[ITEM_CLASS_SKULL][15].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][15].SetSoundID( SOUND_EVENT_FANFARE, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][15].SetFrameID( 228, 232, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][15].SetDropFrameID( 228 );
	m_pTypeInfo[ITEM_CLASS_SKULL][15].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][15].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][16].HName = "俊皋饿靛胶拿";
	m_pTypeInfo[ITEM_CLASS_SKULL][16].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][16].SetSoundID( SOUND_EVENT_FANFARE, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][16].SetFrameID( 229, 233, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][16].SetDropFrameID( 229 );
	m_pTypeInfo[ITEM_CLASS_SKULL][16].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][16].Weight = 1;

	// 馆胆扼捞历 赣府
	m_pTypeInfo[ITEM_CLASS_SKULL][17].HName = "馆胆扼捞历";
	m_pTypeInfo[ITEM_CLASS_SKULL][17].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][17].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][17].SetFrameID( 230, 234, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][17].SetDropFrameID( 230 );
	m_pTypeInfo[ITEM_CLASS_SKULL][17].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][17].Weight = 1;
	
	m_pTypeInfo[ITEM_CLASS_SKULL][18].HName = "农覆郊浇肺磐";
	m_pTypeInfo[ITEM_CLASS_SKULL][18].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][18].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][18].SetFrameID( 233, 237, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][18].SetDropFrameID( 233 );
	m_pTypeInfo[ITEM_CLASS_SKULL][18].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][18].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][19].HName = "酒捞攫萍静";
	m_pTypeInfo[ITEM_CLASS_SKULL][19].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][19].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][19].SetFrameID( 234, 238, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][19].SetDropFrameID( 234 );
	m_pTypeInfo[ITEM_CLASS_SKULL][19].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][19].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][20].HName = "葛单扼胶";
	m_pTypeInfo[ITEM_CLASS_SKULL][20].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][20].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][20].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][20].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][20].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][20].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][21].HName = "俊胶飘肺捞歹";
	m_pTypeInfo[ITEM_CLASS_SKULL][21].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][21].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][21].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][21].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][21].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][21].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][22].HName = "困档快令";
	m_pTypeInfo[ITEM_CLASS_SKULL][22].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][22].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][22].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][22].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][22].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][22].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][23].HName = "龋喉";
	m_pTypeInfo[ITEM_CLASS_SKULL][23].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][23].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][23].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][23].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][23].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][23].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][24].HName = "候曝";
	m_pTypeInfo[ITEM_CLASS_SKULL][24].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][24].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][24].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][24].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][24].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][24].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][25].HName = "榜饭赣";
	m_pTypeInfo[ITEM_CLASS_SKULL][25].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][25].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][25].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][25].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][25].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][25].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][26].HName = "溅档快喇";
	m_pTypeInfo[ITEM_CLASS_SKULL][26].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][26].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][26].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][26].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][26].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][26].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][26].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][27].HName = "府欺";
	m_pTypeInfo[ITEM_CLASS_SKULL][27].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][27].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][27].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][27].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][27].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][27].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][27].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][28].HName = "墨坷胶唱捞飘";
	m_pTypeInfo[ITEM_CLASS_SKULL][28].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][28].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][28].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][28].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][28].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][28].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][28].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][29].HName = "秋困磊靛";
	m_pTypeInfo[ITEM_CLASS_SKULL][29].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][29].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][29].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][29].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][29].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][29].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][29].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][30].HName = "促农胶农府赣";
	m_pTypeInfo[ITEM_CLASS_SKULL][30].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][30].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][30].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][30].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][30].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][30].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][30].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][31].HName = "墨坷胶啊叼攫";
	m_pTypeInfo[ITEM_CLASS_SKULL][31].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][31].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][31].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][31].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][31].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][31].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][31].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][32].HName = "秋啊叼攫";
	m_pTypeInfo[ITEM_CLASS_SKULL][32].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][32].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][32].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][32].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][32].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][32].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][32].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][33].HName = "肺靛促农聪胶";
	m_pTypeInfo[ITEM_CLASS_SKULL][33].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][33].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][33].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][33].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][33].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][33].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][33].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][34].HName = "促农啊叼攫";
	m_pTypeInfo[ITEM_CLASS_SKULL][34].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][34].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][34].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][34].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][34].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][34].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][34].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][35].HName = "肺靛墨坷胶";
	m_pTypeInfo[ITEM_CLASS_SKULL][35].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][35].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][35].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][35].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][35].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][35].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][35].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][36].HName = "墨坷胶弊府靛";
	m_pTypeInfo[ITEM_CLASS_SKULL][36].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][36].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][36].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][36].SetFrameID( 303, 317, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][36].SetDropFrameID( 303 );
	m_pTypeInfo[ITEM_CLASS_SKULL][36].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][36].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][37].HName = "秋巧靛";
	m_pTypeInfo[ITEM_CLASS_SKULL][37].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][37].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][37].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][37].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][37].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][37].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][37].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][38].HName = "darkhaze";
	m_pTypeInfo[ITEM_CLASS_SKULL][38].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][38].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][38].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][38].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][38].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][38].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][38].Weight = 1;

	// 俊俊.. 持阑锭 drop捞尔 tile捞尔 鉴辑 肋给持菌促.
	m_pTypeInfo[ITEM_CLASS_SKULL][39].HName = "Dun Wolfarch";
	m_pTypeInfo[ITEM_CLASS_SKULL][39].EName = "带 匡橇酒农";
	m_pTypeInfo[ITEM_CLASS_SKULL][39].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][39].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][39].SetFrameID( 385, 397, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][39].SetDropFrameID( 383 );
	m_pTypeInfo[ITEM_CLASS_SKULL][39].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][39].Weight = 1;

	// 俊俊.. 持阑锭 drop捞尔 tile捞尔 鉴辑 肋给持菌促.
	m_pTypeInfo[ITEM_CLASS_SKULL][40].HName = "肛 覆刚";
	m_pTypeInfo[ITEM_CLASS_SKULL][40].EName = "Mum Rimmon";
	m_pTypeInfo[ITEM_CLASS_SKULL][40].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][40].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][40].SetFrameID( 383, 399, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][40].SetDropFrameID( 385 );
	m_pTypeInfo[ITEM_CLASS_SKULL][40].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][40].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][41].HName = "箕刚 坷橇";
	m_pTypeInfo[ITEM_CLASS_SKULL][41].EName = "Shaman Oaf";
	m_pTypeInfo[ITEM_CLASS_SKULL][41].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][41].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][41].SetFrameID( 384, 398, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][41].SetDropFrameID( 384 );
	m_pTypeInfo[ITEM_CLASS_SKULL][41].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][41].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][42].HName = "促农海府飘";
	m_pTypeInfo[ITEM_CLASS_SKULL][42].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][42].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][42].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][42].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][42].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][42].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][42].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][43].HName = "府摹俩";
	m_pTypeInfo[ITEM_CLASS_SKULL][43].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][43].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][43].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][43].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][43].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][43].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][43].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][44].HName = "付款飘农贰弊";
	m_pTypeInfo[ITEM_CLASS_SKULL][44].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][44].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][44].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][44].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][44].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][44].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][44].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][45].HName = "磊捞攫飘坷胶";
	m_pTypeInfo[ITEM_CLASS_SKULL][45].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][45].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][45].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][45].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][45].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][45].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][45].Weight = 1;
	
	m_pTypeInfo[ITEM_CLASS_SKULL][46].HName = "吊啊绊老";
	m_pTypeInfo[ITEM_CLASS_SKULL][46].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][46].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][46].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][46].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][46].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][46].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][46].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][47].HName = "杭官皋滴荤";
	m_pTypeInfo[ITEM_CLASS_SKULL][47].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][47].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][47].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][47].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][47].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][47].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][47].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][48].HName = "局浆惯废";
	m_pTypeInfo[ITEM_CLASS_SKULL][48].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][48].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][48].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][48].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][48].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][48].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][48].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][49].HName = "促农匡橇 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][49].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][49].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][49].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][49].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][49].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][49].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][49].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][50].HName = "喉矾靛匡橇 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][50].EName = "Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][50].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][50].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][50].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][50].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][50].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][50].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][51].HName = "阂繁飘飘贰弊 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][51].EName = "Blunt Crag Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][51].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][51].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][51].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][51].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][51].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][51].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][52].HName = "孤畔飘咖廉 赣府";
	m_pTypeInfo[ITEM_CLASS_SKULL][52].EName = "Mutant Edger Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][52].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][52].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][52].SetFrameID( 213, 217, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][52].SetDropFrameID( 213 );
	m_pTypeInfo[ITEM_CLASS_SKULL][52].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][52].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][53].HName = "夯啊叼攫";
	m_pTypeInfo[ITEM_CLASS_SKULL][53].EName = "Bone Guardian Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][53].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][53].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][53].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][53].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][53].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][53].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][54].HName = "烦啊歹 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][54].EName = "Rum Guarder Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][54].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][54].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][54].SetFrameID( 120, 149, 0 );
	m_pTypeInfo[ITEM_CLASS_SKULL][54].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][54].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][54].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][55].HName = "酒捞矫 矾乔攫 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][55].EName = "Icy Ruffian Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][55].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][55].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][55].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][55].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][55].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][55].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][56].HName = "敲府芭 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][56].EName = "Flieger Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][56].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][56].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][56].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][56].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][56].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][56].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][57].HName = "扁橇饭捞磐 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][57].EName = "Gefreiter Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][57].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][57].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][57].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][57].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][57].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][57].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][58].HName = "飘扼浇扼 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][58].EName = "Trasla Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][58].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][58].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][58].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][58].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][58].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][58].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][59].HName = "仟胶墨 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][59].EName = "Pusca Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][59].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][59].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][59].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][59].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][59].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][59].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][60].HName = "漠靛坷皋匙胶农 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][60].EName = "Clad Omenesc Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][60].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][60].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][60].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][60].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][60].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][60].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][61].HName = "畴靛内乔扼 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][61].EName = "Nod Copila Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][61].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][61].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][61].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][61].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][61].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][61].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][62].HName = "饭捞历 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][62].EName = "Razor Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][62].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][62].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][62].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][62].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][62].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][62].Weight = 1;
	
	m_pTypeInfo[ITEM_CLASS_SKULL][63].HName = "蹲动捞 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][63].EName = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][63].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][63].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][63].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][63].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][63].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][63].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][64].HName = "磐弊泛 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][64].EName = "Tug Leg Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][64].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][64].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][64].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][64].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][64].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][64].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][65].HName = "磐弊饭芭 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][65].EName = "Tug Legger Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][65].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][65].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][65].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][65].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][65].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][65].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][66].HName = "肺捞墨促宏风 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][66].EName = "Roi Cadavru Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][66].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][66].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][66].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][66].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][66].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][66].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][67].HName = "矫酒皋胶 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][67].EName = "Siamese Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][67].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][67].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][67].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][67].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][67].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][67].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][68].HName = "奉啊抛胶鸥 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][68].EName = "Lunga Testa Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][68].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][68].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][68].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][68].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][68].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][68].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][69].HName = "坷海福酱明 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][69].EName = "Obersculze Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][69].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][69].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][69].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][69].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][69].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][69].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][70].HName = "胶磐福父 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][70].EName = "Stummann Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][70].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][70].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][70].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][70].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][70].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][70].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][71].HName = "窍快飘父 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][71].EName = "Hauptmann Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][71].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][71].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][71].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][71].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][71].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][71].Weight = 1;

	m_pTypeInfo[ITEM_CLASS_SKULL][72].HName = "坷海福胶飘 秦榜";
	m_pTypeInfo[ITEM_CLASS_SKULL][72].EName = "Oberst Skull";
	m_pTypeInfo[ITEM_CLASS_SKULL][72].Description = "";
	m_pTypeInfo[ITEM_CLASS_SKULL][72].SetSoundID( SOUND_ITEM_MOVE_SKULL, SOUND_ITEM_MOVE_SKULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SKULL][72].SetFrameID( 120, 149, 0 );	
	m_pTypeInfo[ITEM_CLASS_SKULL][72].SetDropFrameID( 120 );
	m_pTypeInfo[ITEM_CLASS_SKULL][72].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SKULL][72].Weight = 1;

	//---------------------------------------------------------------------
	// ITEM_CLASS_MACE
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), MP刘啊(5), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_MACE, 13);

	m_pTypeInfo[ITEM_CLASS_MACE][0].HName = "酒捞攫 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][0].EName = "Iron Mace";
	m_pTypeInfo[ITEM_CLASS_MACE][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetFrameID( 59, 94, 0 );	
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetDropFrameID( 59 );
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][0].SetValue(300, -1, 3, 6, 5, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][0].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_MACE][0].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][0].SetRequireAbility(0,0, 20);
	m_pTypeInfo[ITEM_CLASS_MACE][0].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][1].HName = "角滚 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][1].EName = "Latin MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetFrameID( 148, 152, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetDropFrameID( 148 );
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][1].SetValue(500, -1, 6, 9, 10, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][1].Price	= 7000;
	m_pTypeInfo[ITEM_CLASS_MACE][1].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][1].SetRequireAbility(0,0, 30);
	m_pTypeInfo[ITEM_CLASS_MACE][1].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][2].HName = "弊腐 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][2].EName = "Passion MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetFrameID( 149, 153, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetDropFrameID( 149 );
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][2].SetValue(700, -1, 9, 12, 20, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][2].Price	= 20000;
	m_pTypeInfo[ITEM_CLASS_MACE][2].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][2].SetRequireAbility(0,0, 40);
	m_pTypeInfo[ITEM_CLASS_MACE][2].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][3].HName = "酒农厚俭 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][3].EName = "Girisidan MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetFrameID( 150, 154, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetDropFrameID( 150 );
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][3].SetValue(1200, -1, 12, 15, 40, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][3].Price	= 50000;
	m_pTypeInfo[ITEM_CLASS_MACE][3].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][3].SetRequireAbility(0,0, 50);
	m_pTypeInfo[ITEM_CLASS_MACE][3].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][4].HName = "迄萍橇 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][4].EName = "Episcopal MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetFrameID( 147, 151, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetDropFrameID( 147 );
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][4].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][4].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][4].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][4].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][4].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][5].HName = "内弊荣 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][5].EName = "Cogwheel MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetFrameID( 268, 281, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetDropFrameID( 268 );
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][5].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][5].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][5].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][5].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][5].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][6].HName = "纳颇 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][6].EName = "Cephas MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetFrameID( 267, 280, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetDropFrameID( 267 );
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][6].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][6].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][6].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][6].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][6].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][7].HName = "漠腐胶 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][7].EName = "Calix MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetFrameID( 269, 282, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetDropFrameID( 269 );
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][7].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][7].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][7].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][7].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][7].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][8].HName = "坷胶橇饭捞 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][8].EName = "Osprey Mace";
	m_pTypeInfo[ITEM_CLASS_MACE][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetFrameID( 348, 362, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetDropFrameID( 269 );
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][8].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][8].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][8].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][8].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][8].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][9].HName = "乾绊 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][9].EName = "Pungo MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetFrameID( 437, 451, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetDropFrameID( 437 );
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][9].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][9].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][9].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][9].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][9].UseActionInfo = SKILL_ATTACK_BLADE;

	m_pTypeInfo[ITEM_CLASS_MACE][10].HName = "何扼客 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][10].EName = "Bulawa MACE";
	m_pTypeInfo[ITEM_CLASS_MACE][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetFrameID( 473, 487, 0 );
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetDropFrameID( 473 );
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][10].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][10].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][10].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][10].UseActionInfo = SKILL_ATTACK_BLADE;

		m_pTypeInfo[ITEM_CLASS_MACE][11].HName = "备福令 皋捞胶";
	m_pTypeInfo[ITEM_CLASS_MACE][11].EName = "Gruz Mace";
	m_pTypeInfo[ITEM_CLASS_MACE][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetSoundID( SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUND_ITEM_MOVE_SWORD, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetFrameID( 907 , 930 ,0 );
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetDropFrameID( 907 );
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][11].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][11].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][11].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][11].UseActionInfo = SKILL_ATTACK_BLADE;
	
	m_pTypeInfo[ITEM_CLASS_MACE][12].HName = "技老府攫胶 拒辑";
	m_pTypeInfo[ITEM_CLASS_MACE][12].EName = "Salience Asser";
	m_pTypeInfo[ITEM_CLASS_MACE][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetFrameID( 948, 984, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetDropFrameID(948 );
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetAddonFrameID( ADDONID_MACE_MALE, ADDONID_MACE_FEMALE );
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_MACE][12].SetValue(3000, -1, 15, 20, 60, -1, 0);
	m_pTypeInfo[ITEM_CLASS_MACE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MACE][12].Price	= 250000;
	m_pTypeInfo[ITEM_CLASS_MACE][12].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_MACE][10].SetRequireAbility(0,0, 60);
	m_pTypeInfo[ITEM_CLASS_MACE][12	].UseActionInfo = SKILL_ATTACK_BLADE;

//	m_pTypeInfo[ITEM_CLASS_ASSER][1].HName = "农疯 拒辑";
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].EName = "Crook Asser";
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ASSER][1].Price = 0;


	//---------------------------------------------------------------------
	// ITEM_CLASS_SERUM
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_SERUM, 7 );

	m_pTypeInfo[ITEM_CLASS_SERUM][0].HName = "趋没";
	m_pTypeInfo[ITEM_CLASS_SERUM][0].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][0].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_MOVE_POTION );
	m_pTypeInfo[ITEM_CLASS_SERUM][0].SetFrameID( 224, 228, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][0].SetDropFrameID( 224 );
	m_pTypeInfo[ITEM_CLASS_SERUM][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][0].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][1].HName = "趋没";
	m_pTypeInfo[ITEM_CLASS_SERUM][1].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][1].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_MOVE_POTION );
	m_pTypeInfo[ITEM_CLASS_SERUM][1].SetFrameID( 224, 228, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][1].SetDropFrameID( 224 );
	m_pTypeInfo[ITEM_CLASS_SERUM][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][1].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][2].HName = "趋没";
	m_pTypeInfo[ITEM_CLASS_SERUM][2].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][2].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_MOVE_POTION );
	m_pTypeInfo[ITEM_CLASS_SERUM][2].SetFrameID( 224, 228, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][2].SetDropFrameID( 224 );
	m_pTypeInfo[ITEM_CLASS_SERUM][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][2].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][3].HName = "趋没";
	m_pTypeInfo[ITEM_CLASS_SERUM][3].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][3].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_MOVE_POTION );
	m_pTypeInfo[ITEM_CLASS_SERUM][3].SetFrameID( 224, 228, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][3].SetDropFrameID( 224 );
	m_pTypeInfo[ITEM_CLASS_SERUM][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][3].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][4].HName = "弧碍 荤帕";
	m_pTypeInfo[ITEM_CLASS_SERUM][4].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][4].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX);
	m_pTypeInfo[ITEM_CLASS_SERUM][4].SetFrameID( 244, 248, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][4].SetDropFrameID( 244 );
	m_pTypeInfo[ITEM_CLASS_SERUM][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][4].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][5].HName = "畴鄂 侗惫";
	m_pTypeInfo[ITEM_CLASS_SERUM][5].EName = "Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][5].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX);
	m_pTypeInfo[ITEM_CLASS_SERUM][5].SetFrameID( 409, 423, 0 );//( 378, 392, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][5].SetDropFrameID( 409 );//( 378 );
	m_pTypeInfo[ITEM_CLASS_SERUM][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][5].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_SERUM][6].HName = "劝己拳 趋没";
	m_pTypeInfo[ITEM_CLASS_SERUM][6].EName = "Active Serum";
	m_pTypeInfo[ITEM_CLASS_SERUM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SERUM][6].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_ITEM_MOVE_POTION );
	m_pTypeInfo[ITEM_CLASS_SERUM][6].SetFrameID( 224, 228, 0 );//( 378, 392, 0 );	
	m_pTypeInfo[ITEM_CLASS_SERUM][6].SetDropFrameID( 224 );//( 378 );
	m_pTypeInfo[ITEM_CLASS_SERUM][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SERUM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SERUM][6].Price	= 5000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_ETC
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_ETC, 3 );

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].HName = "戳措惯砰";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].SetFrameID( 223, 227, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].SetDropFrameID( 223 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][0].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].HName = "冠零朝俺";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].SetFrameID( 222, 226, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].SetDropFrameID( 222 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][1].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].HName = "1雀侩 锅开扁";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].EName = "Vampire Translator";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].Description = "10盒埃 促弗 辆练狼 措拳甫 葛滴 舅酒 甸阑 荐 乐嚼聪促. ";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].SetFrameID( 578, 592, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].SetDropFrameID( 578 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_ETC][2].Price	= 5000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_SLAYER_PORTAL_ITEM
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_SLAYER_PORTAL_ITEM, 3 );

	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].HName = "公傈扁5";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_WORLD_WALKIETALKIE );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].SetFrameID( 232, 236, 0 );	
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].SetDropFrameID( 232 );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][0].MaxNumber = 5;

	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].HName = "公傈扁10";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_WORLD_WALKIETALKIE );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].SetFrameID( 232, 236, 0 );	
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].SetDropFrameID( 232 );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][1].MaxNumber = 10;

	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].HName = "公傈扁20";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_WORLD_WALKIETALKIE );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].SetFrameID( 232, 236, 0 );	
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].SetDropFrameID( 232 );
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_SLAYER_PORTAL_ITEM][2].MaxNumber = 20;

	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_PORTAL_ITEM
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_PORTAL_ITEM, 18 );

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][0].MaxNumber = 5;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][1].MaxNumber = 10;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][2].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][3].MaxNumber = 5;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][4].MaxNumber = 10;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][5].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][6].MaxNumber = 5;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][7].MaxNumber = 10;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][8].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][9].MaxNumber = 5;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][10].MaxNumber = 10;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][11].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][12].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][13].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][14].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][15].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][16].MaxNumber = 20;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].HName = "轨颇捞绢器呕";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].SetSoundID( SOUND_ITEM_MOVE_JEWEL, SOUND_ITEM_MOVE_JEWEL, SOUNDID_NULL, SOUND_ITEM_MOVE_JEWEL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].SetFrameID( 231, 235, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].SetDropFrameID( 231 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].Price	= 5000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_PORTAL_ITEM][17].MaxNumber = 20;



	//---------------------------------------------------------------------
	// ITEM_CLASS_EVENT_GIFT_BOX
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_EVENT_GIFT_BOX, 27 );

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].HName = "急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].EName = "Tool kit";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].SetFrameID( 235, 239, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].SetDropFrameID( 235 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][0].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].HName = "急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].EName = "gift taken";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].SetFrameID( 236, 240, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].SetDropFrameID( 236 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][1].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].HName = "弧碍祸 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].EName = "Red Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].SetFrameID( 833, 856, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].SetDropFrameID( 833 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][2].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].HName = "颇鄂祸 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].EName = "Blue Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].SetFrameID( 834, 857, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].SetDropFrameID( 834 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][3].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].HName = "檬废祸 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].EName = "Green Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].SetFrameID( 835, 858, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].SetDropFrameID( 835 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][4].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].HName = "畴鄂祸 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].EName = "Yellow Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].SetFrameID( 836, 859, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].SetDropFrameID( 836 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][5].Price	= 5000;

	for(int i = 6; i < 16; i++)
	{
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].HName = "八篮祸 急拱 惑磊";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].EName = "Black Gift Box";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Description = "";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetFrameID( 837, 860, 0 );	
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetDropFrameID( 837 );
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetGrid(2, 2);
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Weight = 1;
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Price	= 5000;
	}
	// 2004, 04, 28 sobeit add 付南 捞亥飘 急拱惑磊 start
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].HName = "弧碍祸 付南 捞亥飘 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].EName = "Red Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].SetFrameID( 833, 856, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].SetDropFrameID( 833 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][16].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].HName = "颇鄂祸 付南 捞亥飘 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].EName = "Blue Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].SetFrameID( 834, 857, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].SetDropFrameID( 834 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][17].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].HName = "畴鄂祸 付南 捞亥飘 急拱 惑磊";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].EName = "Yellow Gift Box";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].SetFrameID( 836, 859, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].SetDropFrameID( 836 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][18].Price	= 5000;

	for( i = 19; i < 22; i++)
	{
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].HName = "八篮祸 急拱 惑磊";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].EName = "Black Gift Box";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Description = "";
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetFrameID( 837, 860, 0 );	
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetDropFrameID( 837 );
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].SetGrid(2, 2);
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Weight = 1;
		m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][i].Price	= 5000;
	}
	// 2004, 04, 28 sobeit add 付南 捞亥飘 急拱惑磊 end

	// 2004, 6, 18 sobeit add start - naming pen (864, 887)
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].HName = "匙捞怪 奇";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].EName = "Naming Pen";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].Description = "葱匙烙阑 函版且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].SetFrameID( 865, 888, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].SetDropFrameID( 865 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][22].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].HName = "脐 匙捞怪 奇";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].EName = "Pet Naming Pen";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].Description = "脐狼 葱匙烙阑 函版且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].SetFrameID( 866, 889, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].SetDropFrameID( 866 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][23].Price	= 5000;
	// 2004, 6, 18 sobeit add end - naming pen

	// 2004, 6, 26 sobeit add start - 眠啊 匙捞怪 奇
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].HName = "竣胶飘扼 匙捞怪 奇";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].EName = "Extra Naming Pen";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].SetFrameID( 865, 888, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].SetDropFrameID( 865 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][24].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].HName = "府固萍靛 匙捞怪 奇";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].EName = "Limited Naming Pen";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].SetFrameID( 865, 888, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].SetDropFrameID( 865 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][25].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].HName = "绊沥巧";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].EName = "Push-Pin";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].SetFrameID( 865, 888, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].SetDropFrameID( 865 );
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_GIFT_BOX][26].Price	= 5000;
	// 2004, 6, 26 sobeit add end - 眠啊 匙捞怪 奇
	
	//---------------------------------------------------------------------
	// ITEM_CLASS_EVENT_STAR
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_EVENT_STAR, 23 );

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].HName = "捞亥飘喊 八沥";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].EName = "Black Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].SetFrameID( 237, 241, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].SetDropFrameID( 237 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][0].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].HName = "捞亥飘喊 弧碍";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].EName = "Red Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].SetFrameID( 238, 242, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].SetDropFrameID( 238 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][1].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].HName = "捞亥飘喊 颇尔";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].EName = "Blue Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].SetFrameID( 239, 243, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].SetDropFrameID( 239 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][2].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].HName = "捞亥飘喊 檬废";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].EName = "Green Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].SetFrameID( 240, 244, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].SetDropFrameID( 240 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][3].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].HName = "捞亥飘喊 窍疵";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].EName = "Cyan Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].SetFrameID( 241, 245, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].SetDropFrameID( 241 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][4].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].HName = "捞亥飘喊 闰祸";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].EName = "White Star";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].SetFrameID( 242, 246, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].SetDropFrameID( 242 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][5].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].HName = "绵备傍";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].EName = "Soccer Ball";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].Description = "老沥 肮荐甫 葛栏搁 绊鞭 酒捞袍 肚绰 捣栏肺 背券啊瓷";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].SetFrameID( 272, 286, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].SetDropFrameID( 272 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][6].Price	= 40000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].HName = "喉风 靛酚";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].EName = "Blue Drop";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].Description = "酒捞袍阑 Enchant且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].SetFrameID( 315, 329, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].SetDropFrameID( 315 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][7].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].HName = "弧埃 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].EName = "Red SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].Description = "可记捞 绝绰 酒捞袍阑 Str可记栏肺 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].SetFrameID( 404, 418, 0 ); //( 379, 393, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].SetDropFrameID( 404 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][8].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].HName = "踌祸 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].EName = "Green SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].Description = "可记捞 绝绰 酒捞袍阑 Dex可记栏肺 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].SetFrameID( 405, 419, 0 );//( 380, 394, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].SetDropFrameID( 405 );//( 380 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][9].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].HName = "颇鄂 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].EName = "Blue SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].Description = "可记捞 绝绰 酒捞袍阑 Int可记栏肺 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].SetFrameID( 406, 420, 0 );//( 381, 395, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].SetDropFrameID( 406 );//( 381 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][10].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].HName = "八篮 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].EName = "Black SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].Description = "可记捞 绝绰 酒捞袍阑 Damage可记栏肺 Enchant且 荐 乐嚼聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].SetFrameID( 407, 421, 0 );//( 382, 396, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].SetDropFrameID( 407 );//( 382 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][11].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].HName = "喉风 滚靛";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].EName = "Blue Bird";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].Description = "饭绢 酒捞袍阑 Enchant且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].SetFrameID( 519, 533, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].SetDropFrameID( 519 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][12].Price	= 2000000000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].HName = "没踌 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].EName = "Bluish Green Rice Cake Soup";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].Description = "可记捞 绝绰 酒捞袍阑 葛电瓷仿摹+1 可记栏肺 Enchant 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].SetFrameID( 591, 605, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].SetDropFrameID( 591 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][13].Price	= 2000000000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].HName = "睛祸 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].EName = "Mugwort Rice Cake Soup";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].Description = "可记捞 绝绰 酒捞袍阑 Lucky+1 可记栏肺 Enchant 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].SetFrameID( 590, 604, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].SetDropFrameID( 590 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][14].Price	= 2000000000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].HName = "喉风 靛酚 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].EName = "Blue Drop 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].Description = "老馆 喉风靛酚焊促 臭篮 犬伏肺 Enchant 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].SetFrameID ( 586, 600, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].SetDropFrameID( 586 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][15].Price = 2000000000;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].HName = "飘罚胶 酒捞袍 虐飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].EName = "Trans Item Kit";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].Description = "酒捞袍捞 啊瘤绊 乐绰 馒侩 力茄 己喊阑 傈券 矫难 凛聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].SetFrameID ( 593, 607, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].SetDropFrameID( 593 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][16].Price = 2000000000;


	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].HName = "捞亥飘 弧埃 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].EName = "Event Red SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].Description = "公可记 酒捞袍俊 str+1 可记阑 100% 牢忙飘钦聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].SetFrameID( 404, 418, 0 ); //( 379, 393, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].SetDropFrameID( 404 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][17].Price	= 1;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].HName = "捞亥飘 踌祸 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].EName = "Event Green SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].Description = "公可记 酒捞袍俊 dex+1 可记阑 100% 牢忙飘钦聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].SetFrameID( 405, 419, 0 );//( 380, 394, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].SetDropFrameID( 405 );//( 380 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][18].Price	= 1;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].HName = "捞亥飘 颇鄂 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].EName = "Event Blue SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].Description = "公可记 酒捞袍俊 int+1 可记阑 100% 牢忙飘钦聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].SetFrameID( 406, 420, 0 );//( 381, 395, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].SetDropFrameID( 406 );//( 381 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][19].Price	= 1;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].HName = "捞亥飘 八篮 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].EName = "Event Black SP";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].Description = "公可记 酒捞袍俊 dam+1 可记阑 100% 牢忙飘钦聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].SetFrameID( 407, 421, 0 );//( 382, 396, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].SetDropFrameID( 407 );//( 382 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][20].Price	= 1;

	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].HName = "捞亥飘 睛祸 侗惫";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].EName = "Event Mugwort Rice Cake Soup";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].Description = "公可记 酒捞袍俊 青款+1 可记阑 100% 牢忙飘钦聪促";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].SetFrameID( 590, 604, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].SetDropFrameID( 590 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][21].Price	= 1;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].HName = "炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].EName = "Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].Description = "酒捞袍狼 鞭荐甫 老沥 犬伏肺 臭咯凛聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].SetFrameID( 980,1014, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].SetDropFrameID( 980 );
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_STAR][22].Price	= 1;


	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_EARRING
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_EARRING, 14 );

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].HName = "宏沸令 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].EName = "Bronze Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].SetFrameID( 305, 319, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].SetDropFrameID( 305 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][0].Price	= 3000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].HName = "角滚 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].EName = "Silver Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].SetFrameID( 306, 320, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].SetDropFrameID( 306 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][1].Price	= 4000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].HName = "榜靛 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].EName = "Gold Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].SetFrameID( 307, 321, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].SetDropFrameID( 307 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][2].Price	= 6000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].HName = "单静 姐 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].EName = "Death Sun Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].SetFrameID( 308, 322, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].SetDropFrameID( 308 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][3].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].HName = "海明 辑努 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].EName = "Bats Circle Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].SetFrameID( 309, 323, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].SetDropFrameID( 309 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][4].Price	= 13000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].HName = "蒋靛坷宏 促农聪胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].EName = "Shield of Darkness";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].SetFrameID( 310, 324, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].SetDropFrameID( 310 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][5].Price	= 19000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].HName = "喉矾叼 乔胶飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].EName = "Bloody Feast";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].SetFrameID( 311, 325, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].SetDropFrameID( 311 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][6].Price	= 28000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].HName = "捞绢傅 坷宏 捞磐澄 扼捞橇";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].EName = "Earring of Eternal Life";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].SetFrameID( 312, 326, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].SetDropFrameID( 312 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][7].Price	= 42000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].HName = "歹 墓明 酒捞令";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].EName = "The Cats Eyes";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].SetFrameID( 313, 327, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].SetDropFrameID( 313 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][8].Price	= 63000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].HName = "胶拿 坷宏 叼胶飘钒记";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].EName = "Skull of Destruction";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].SetFrameID( 314, 328, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].SetDropFrameID( 314 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][9].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].HName = "俊勒 捞绢傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].EName = "Ate's Earring";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].SetFrameID( 370, 384, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].SetDropFrameID( 370 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][10].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].HName = "拳钦狼 辑距";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].EName = "The Oath of Harmony";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].SetFrameID( 460, 474, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].SetDropFrameID( 460 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][11].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].HName = "洒酒脚胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].EName = "Hyacinth";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].SetFrameID( 490, 504, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].SetDropFrameID( 490 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][12].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].HName = "墨弛府救";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].EName = "Carnelian";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].SetFrameID( 929 , 952 ,0);	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].SetDropFrameID( 929 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_EARRING][13].Price	= 94000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_RELIC
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_RELIC, 2 );

	m_pTypeInfo[ITEM_CLASS_RELIC][0].HName = "芬羔狼绕厘";
	m_pTypeInfo[ITEM_CLASS_RELIC][0].EName = "Rommels Tag";
	m_pTypeInfo[ITEM_CLASS_RELIC][0].Description = "浇饭捞绢狼 己拱";
	m_pTypeInfo[ITEM_CLASS_RELIC][0].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_RELIC][0].SetFrameID( 274, 288, 0 );	
	m_pTypeInfo[ITEM_CLASS_RELIC][0].SetDropFrameID( 274 );
	m_pTypeInfo[ITEM_CLASS_RELIC][0].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_RELIC][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_RELIC][0].Price	= 0;

//	m_pTypeInfo[ITEM_CLASS_RELIC][1].HName = "己狼";
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].EName = "The Holy Linen";
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].Description = "浇饭捞绢狼 己拱";
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetFrameID( 275, 289, 0 );	
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetDropFrameID( 275 );
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetGrid(2, 2);
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_RELIC][1].Price	= 0;
//
	m_pTypeInfo[ITEM_CLASS_RELIC][1].HName = "贸赤狼乔";
	m_pTypeInfo[ITEM_CLASS_RELIC][1].EName = "The Blood of Virgin";
	m_pTypeInfo[ITEM_CLASS_RELIC][1].Description = "轨颇捞绢狼 己拱";
	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetFrameID( 273, 287, 0 );	
	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetDropFrameID( 273 );
	m_pTypeInfo[ITEM_CLASS_RELIC][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_RELIC][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_RELIC][1].Price	= 0;

//	m_pTypeInfo[ITEM_CLASS_RELIC][3].HName = "开绞磊啊";
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].EName = "The Anti-Cross";
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].Description = "轨颇捞绢狼 己拱";
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].SetSoundID( SOUND_XMAS_STAR, SOUND_XMAS_STAR, SOUNDID_NULL, SOUND_XMAS_STAR );
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].SetFrameID( 276, 290, 0 );	
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].SetDropFrameID( 276 );
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].SetGrid(2, 3);
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_RELIC][3].Price	= 0;


	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_WEAPON
	//---------------------------------------------------------------------
	// 郴备己(1), Protection(?), MinDam(3)~MaxDam(4), Speed(7)
	//---------------------------------------------------------------------	
	InitClass(ITEM_CLASS_VAMPIRE_WEAPON, 20 );

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].HName = "呈努 颇飘";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].EName = "Knuckle part";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetFrameID( 316, 330, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetDropFrameID( 316 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][0].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].HName = "芥 弊乏";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].EName = "Thumb Grab";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetFrameID( 317, 331, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetDropFrameID( 317 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][1].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].HName = "器胶 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].EName = "Force Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetFrameID( 318, 333, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetDropFrameID( 318 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][2].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].HName = "力固澄傅 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].EName = "Geminal ring Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetFrameID( 319, 335, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetDropFrameID( 319 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][3].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].HName = "抗萍 农肺快";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].EName = "Yetis Craw";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetFrameID( 326, 340, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetDropFrameID( 326 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][4].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].HName = "南明 农肺快";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].EName = "Cats Craw";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetFrameID( 327, 341, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetDropFrameID( 327 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][5].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].HName = "乔胶飘 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].EName = "Fist Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetFrameID( 320, 336, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetDropFrameID( 320 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][6].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].HName = "俊捞橇 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].EName = "Ape Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetFrameID( 321, 334, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetDropFrameID( 321 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][7].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].HName = "荤宏 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].EName = "Sav Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetFrameID( 322, 339, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetDropFrameID( 322 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][8].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].HName = "瘤橇 勤令 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].EName = "Sifs Hand Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetFrameID( 323, 338, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetDropFrameID( 323 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][9].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].HName = "烽 促捞滚 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].EName = "Loon Diver Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetFrameID( 324, 337, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetDropFrameID( 324 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][10].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].HName = "付捞飘 呈努";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].EName = "Might Knuckle";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetFrameID( 325, 332, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetDropFrameID( 325 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][11].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].HName = "磊付促福";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].EName = "Zamadar";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetFrameID( 328, 342, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetDropFrameID( 328 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][12].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].HName = "墨鸥福";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].EName = "Katar";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetFrameID( 329, 343, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetDropFrameID( 329 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][13].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].HName = "酒皋皋飘 农肺快";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].EName = "Amemets Craw";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetFrameID( 330, 344, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetDropFrameID( 330 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][14].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].HName = "付遏胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].EName = "MARAX";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetFrameID( 332, 346, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetDropFrameID( 332 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][15].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].HName = "磊埃";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].EName = "Zagan";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetFrameID( 456, 470, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetDropFrameID( 456 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][16].SetRequireAbility(20);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].HName = "官弊唱快";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].EName = "Bagh Nakh";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetFrameID( 485, 499, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetDropFrameID( 485 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SilverMax	= 1000;


	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].HName = "沫磊福 农肺快";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].EName = "Khanjar Crow";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SetSoundID( SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUND_ITEM_MOVE_GLOVE, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SetFrameID( 931 , 954 ,0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SetDropFrameID( 931 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][18].SilverMax	= 1000;
	//m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][17].SetRequireAbility(20);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].HName = "俊矫令 匙老";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].EName = "Acies Nail";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SetFrameID( 943, 979, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SetDropFrameID(943 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SetValue(800, -1, 10, 15, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].Price	= 2000;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_WEAPON][19].SilverMax	= 1000;


	
	
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].HName = "夯 努府宏";
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].EName = "Bone Cleave";
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_CLAW][1].Price = 0;



	//---------------------------------------------------------------------
	// ITEM_CLASS_VAMPIRE_AMULET
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_VAMPIRE_AMULET, 14 );

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].HName = "扼靛";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].EName = "Rad";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].SetFrameID( 336, 350, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].SetDropFrameID( 336 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][0].Price	= 3000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].HName = "乃";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].EName = "Ken";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].SetFrameID( 337, 351, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].SetDropFrameID( 337 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][1].Price	= 4000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].HName = "力扼";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].EName = "Jera";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].SetFrameID( 338, 352, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].SetDropFrameID( 338 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][2].Price	= 6000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].HName = "俊坷福";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].EName = "Eolh";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].SetFrameID( 339, 353, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].SetDropFrameID( 339 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][3].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].HName = "父";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].EName = "Man";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].SetFrameID( 340, 354, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].SetDropFrameID( 340 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][4].Price	= 13000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].HName = "何父";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].EName = "Bu Man";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].SetFrameID( 341, 355, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].SetDropFrameID( 341 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][5].Price	= 19000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].HName = "雷弊";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].EName = "Ing";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].SetFrameID( 342, 356, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].SetDropFrameID( 342 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][6].Price	= 28000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].HName = "坷伎";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].EName = "Othel";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].SetFrameID( 343, 357, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].SetDropFrameID( 343 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][7].Price	= 42000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].HName = "坷崔";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].EName = "Odal";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].SetFrameID( 344, 358, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].SetDropFrameID( 344 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][8].Price	= 63000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].HName = "促俊弊";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].EName = "Daeg";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].SetFrameID( 345, 359, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].SetDropFrameID( 345 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][9].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].HName = "矫爸";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].EName = "Sigel";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].SetFrameID( 372, 386, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].SetDropFrameID( 372 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][10].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].HName = "捞胶";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].EName = "IS";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].SetFrameID( 461, 475, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].SetDropFrameID( 461 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][11].Price	= 94000;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].HName = "其坷";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].EName = "Feoh";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].SetFrameID( 488, 502, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].SetDropFrameID( 488 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][12].Price	= 94000;


	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].HName = "聪捞靛";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].EName = "Nied";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].SetFrameID( 910,	933, 0 );	
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].SetDropFrameID( 910 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_AMULET][13].Price	= 94000;

	//---------------------------------------------------------------------
	// ITEM_CLASS_QUEST_ITEM
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_QUEST_ITEM, 10 );

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].HName = "官配府 格吧捞";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].EName = "Bathory Necklace";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].SetFrameID( 374, 388, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].SetDropFrameID( 374 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][0].Price	= 3000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].HName = "官配府 奇带飘";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].EName = "Bathory Pendant";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].SetFrameID( 373, 387, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].SetDropFrameID( 373 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][1].Price	= 4000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].HName = "抛其令 格吧捞";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].EName = "Tepez Necklace";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].SetFrameID( 376, 390, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].SetDropFrameID( 376 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][2].Price	= 6000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].HName = "抛其令 奇带飘";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].EName = "Tepez Pendant";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].SetFrameID( 375, 389, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].SetDropFrameID( 375 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][3].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].HName = "联胶沛";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].EName = "Gemstone";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].SetFrameID( 751, 765, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].SetDropFrameID( 751 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][4].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].HName = "焊抚崔 墨靛";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].EName = "The Full Moon Card";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].SetFrameID( 771, 785, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].SetDropFrameID( 771 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][5].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].HName = "弊故崔 墨靛";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].EName = "The Old Moon Card";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].SetFrameID( 768, 782, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].SetDropFrameID( 768 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][6].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].HName = "弧埃祸 汗林赣聪";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].EName = "The Red Lucky Bag";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].SetFrameID( 829, 852, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].SetDropFrameID( 829 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][7].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].HName = "龙靛饭 厚陵";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].EName = "Gilles de Rais Beads";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].SetFrameID( 867, 890, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].SetDropFrameID( 867 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][8].Price	= 9000;

	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].HName = "龙靛饭 奇带飘";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].EName = "Gilles de Rais Pendant";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].SetFrameID( 868, 891, 0 );	
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].SetDropFrameID( 868 );
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_QUEST_ITEM][9].Price	= 9000;
	
	//---------------------------------------------------------------------
	// ITEM_CLASS_EVENT_TREE
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_EVENT_TREE, 42 );

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].HName = "炼阿1";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].SetFrameID( 390, 404, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].SetDropFrameID( 390 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].HName = "炼阿2";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].SetFrameID( 391, 405, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].SetDropFrameID( 391 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].HName = "炼阿3";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].SetFrameID( 392, 406, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].SetDropFrameID( 392 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].HName = "炼阿4";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].SetFrameID( 393, 407, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].SetDropFrameID( 393 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].HName = "炼阿5";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].SetFrameID( 394, 408, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].SetDropFrameID( 394 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][4].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].HName = "炼阿6";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].SetFrameID( 395, 409, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].SetDropFrameID( 395 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][5].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].HName = "炼阿7";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].SetFrameID( 396, 410, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].SetDropFrameID( 396 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][6].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].HName = "炼阿8";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].SetFrameID( 397, 411, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].SetDropFrameID( 397 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][7].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].HName = "炼阿9";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].SetFrameID( 398, 412, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].SetDropFrameID( 398 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][8].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].HName = "炼阿10";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].SetFrameID( 399, 413, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].SetDropFrameID( 399 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][9].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].HName = "炼阿11";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].SetFrameID( 400, 414, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].SetDropFrameID( 400 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][10].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].HName = "炼阿12";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].EName = "Part";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].Description = "12俺狼 欺榴 炼阿阑 葛滴 葛酒 焊酒夸~*";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].SetFrameID( 401, 415, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].SetDropFrameID( 401 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][11].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].HName = "飘府";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].EName = "Tree";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].SetFrameID( 389, 403, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].SetDropFrameID( 389 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][12].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].HName = "绊措 巩清 炼阿 1";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].EName = "Part Of Ancient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].SetFrameID( 492, 506, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].SetDropFrameID( 492 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][13].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].HName = "绊措 巩清 炼阿 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].SetFrameID( 493, 507, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].SetDropFrameID( 493 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][14].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].HName = "绊措 巩清 炼阿 3";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].SetFrameID( 494, 508, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].SetDropFrameID( 494 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][15].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].HName = "绊措 巩清 炼阿 4";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].SetFrameID( 495, 509, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].SetDropFrameID( 495 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][16].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].HName = "绊措 巩清 炼阿 5";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].SetFrameID( 496, 510, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].SetDropFrameID( 496 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][17].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].HName = "绊措 巩清 炼阿 6";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].SetFrameID( 497, 511, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].SetDropFrameID( 497 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][18].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].HName = "绊措 巩清 炼阿 7";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].SetFrameID( 498, 512, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].SetDropFrameID( 498 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][19].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].HName = "绊措 巩清 炼阿 8";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].SetFrameID( 499, 513, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].SetDropFrameID( 499 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][20].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].HName = "绊措 巩清 炼阿 9";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].SetFrameID( 500, 514, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].SetDropFrameID( 500 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][21].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].HName = "绊措 巩清 炼阿 10";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].SetFrameID( 501, 515, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].SetDropFrameID( 501 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][22].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].HName = "绊措 巩清 炼阿 11";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].SetFrameID( 502, 516, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].SetDropFrameID( 502 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][23].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].HName = "绊措 巩清 炼阿 12";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].EName = "Part Of Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].Description = "12俺狼 绊措 巩清 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].SetFrameID( 503, 517, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].SetDropFrameID( 503 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][24].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].HName = "绊措 巩清";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].EName = "Acient Document";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].Description = "NPC 俊霸 啊廉啊搁 涅胶飘甫 肯丰且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].SetFrameID( 491, 505, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].SetDropFrameID( 491 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][25].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].HName = "签富 1";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].EName = "Signpost 1";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].Description = "舅覆 臂阑 累己窍咯 烙狼狼 厘家俊 签富肺 技况 笛 荐 乐嚼聪促.(6矫埃)";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].SetFrameID( 592, 606, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].SetDropFrameID( 592 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][26].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].HName = "签富 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].EName = "Signpost 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].Description = "舅覆 臂阑 累己窍咯 烙狼狼 厘家俊 签富肺 技况 笛 荐 乐嚼聪促.(12矫埃)";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].SetFrameID( 592, 606, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].SetDropFrameID( 592 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][27].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].HName = "签富 3";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].EName = "Signpost 3";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].Description = "舅覆 臂阑 累己窍咯 烙狼狼 厘家俊 签富肺 技况 笛 荐 乐嚼聪促.(24矫埃)";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].SetFrameID( 592, 606, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].SetDropFrameID( 592 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][28].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].HName = "痢配牢屈狼 炼阿 1";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].SetFrameID( 728, 742, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].SetDropFrameID( 728 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][29].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].HName = "痢配牢屈狼 炼阿 2";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].SetFrameID( 729, 743, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].SetDropFrameID( 729 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][30].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].HName = "痢配牢屈狼 炼阿 3";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].SetFrameID( 730, 744, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].SetDropFrameID( 730 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][31].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].HName = "痢配牢屈狼 炼阿 4";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].SetFrameID( 731, 745, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].SetDropFrameID( 731 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][32].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].HName = "痢配牢屈狼 炼阿 5";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].SetFrameID( 732, 746, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].SetDropFrameID( 732 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][33].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].HName = "痢配牢屈狼 炼阿 6";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].SetFrameID( 733, 747, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].SetDropFrameID( 733 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][34].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].HName = "痢配牢屈狼 炼阿 7";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].SetFrameID( 734, 748, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].SetDropFrameID( 734 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][35].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].HName = "痢配牢屈狼 炼阿 8";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].SetFrameID( 735, 749, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].SetDropFrameID( 735 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][36].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].HName = "痢配牢屈狼 炼阿 9";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].SetFrameID( 736, 750, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].SetDropFrameID( 736 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][37].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].HName = "痢配牢屈狼 炼阿 10";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].SetFrameID( 737, 751, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].SetDropFrameID( 737 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][38].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].HName = "痢配牢屈狼 炼阿 11";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].SetFrameID( 738, 752, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].SetDropFrameID( 738 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][39].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].HName = "痢配牢屈狼 炼阿 12";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].EName = "Part Of Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].Description = "12俺狼 痢配牢屈狼 炼阿阑 葛酒具 钦聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].SetFrameID( 739, 753, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].SetDropFrameID( 739 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][40].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].HName = "痢配 牢屈";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].EName = "Clay Doll";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].Description = "NPC 俊霸 啊廉啊搁 涅胶飘甫 肯丰且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].SetFrameID( 727, 741, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].SetDropFrameID( 727 );
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_TREE][41].Price	= 0;


	//---------------------------------------------------------------------
	// ITEM_CLASS_EVENT_ETC
	//---------------------------------------------------------------------
	InitClass(ITEM_CLASS_EVENT_ETC, 18);

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].HName = "窍疵祸 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].EName = "FireCracker1";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].SetFrameID( 386, 400, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].SetDropFrameID( 386 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].HName = "踌祸 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].EName = "FireCracker2";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].SetFrameID( 387, 401, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].SetDropFrameID( 387 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].HName = "焊扼祸 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].EName = "FireCracker3";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].SetFrameID( 388, 402, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].SetDropFrameID( 388 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].HName = "靛贰帮 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].EName = "Dragon FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].SetFrameID( 515,529,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].SetDropFrameID( 515 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][3].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].HName = "林炔祸 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].EName = "Orange FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].SetFrameID( 516,530,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].SetDropFrameID( 516 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][4].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].HName = "窍疵祸 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].EName = "Skyblue Triple FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].SetFrameID( 571,585,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].SetDropFrameID( 571 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][5].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].HName = "踌祸 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].EName = "Green Triple FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].SetFrameID( 572,586,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].SetDropFrameID( 572 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][6].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].HName = "焊扼祸 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].EName = "Purple Triple FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].SetFrameID( 573,587,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].SetDropFrameID( 573 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][7].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].HName = "林炔祸 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].EName = "Orange Triple FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].SetFrameID( 517,531,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].SetDropFrameID( 517 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][8].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].HName = "窍疵祸 堡开 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].EName = "Skyblue Triple Wide FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].SetFrameID( 574,588,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].SetDropFrameID( 574 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][9].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].HName = "踌祸 堡开 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].EName = "Green Triple Wide FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].SetFrameID( 575,589,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].SetDropFrameID( 575 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][10].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].HName = "焊扼祸 堡开 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].EName = "Purple Triple Wide FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].SetFrameID( 576,590,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].SetDropFrameID( 576 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][11].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].HName = "林炔祸 堡开 3楷惯 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].EName = "Orange Triple Wide FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].SetFrameID( 518,532,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].SetDropFrameID( 518 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][12].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].HName = "胶盆 气磷";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].EName = "Storm FireCracker";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].Description = "坷弗率 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].SetFrameID( 587,601,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].SetDropFrameID( 587 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].SetGrid( 2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][13].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].HName = "畴鄂 荤帕";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].EName = "Yellow Candy";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].SetFrameID(883, 906, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].SetDropFrameID(883);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][14].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].HName = "闰 价祈";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].EName = "White Rice Cake";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].Description = "HP/MP + 200";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].SetFrameID(902, 925, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].SetDropFrameID(902);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][15].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].HName = "睛 价祈";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].EName = "Mugwort Rice Cake";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].Description = "HP/MP + 500";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].SetFrameID(901, 924, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].SetDropFrameID(901);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][16].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].HName = "曹 价祈";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].EName = "Honey Rice Cake";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].Description = "HP/MP + 1000";
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].SetFrameID(900, 923, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].SetDropFrameID(900);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ETC][17].Price = 0;

	//-----------------------------------------------------------------------
	// 乔狼 己辑 酒捞袍 抛捞喉
	//-----------------------------------------------------------------------

	InitClass(ITEM_CLASS_BLOOD_BIBLE, 12);

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].HName = "酒福皋啊";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].EName = "Armega";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].SetFrameID( 419, 433, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].SetDropFrameID(419 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].HName = "固龋饭";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].EName = "Mihole";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].SetFrameID( 420, 434, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].SetDropFrameID(420 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][1].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].HName = "虐肺";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].EName = "Kiro";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].SetFrameID( 421, 435, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].SetDropFrameID(421 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][2].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].HName = "酒捞聪";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].EName = "Ini";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].SetFrameID( 416, 430, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].SetDropFrameID(416 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][3].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].HName = "弊饭绊府";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].EName = "Gregori";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].SetFrameID( 418, 432, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].SetDropFrameID(418 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][4].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].HName = "能磨府酒";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].EName = "Concilia";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].SetFrameID( 417, 431, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].SetDropFrameID(417 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][5].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].HName = "饭瘤坷胶";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].EName = "Legios";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].SetFrameID( 410, 424, 0);	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].SetDropFrameID(410 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][6].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].HName = "鳃副";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].EName = "Hillel";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].SetFrameID( 412, 426, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].SetDropFrameID(412 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][7].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].HName = "鲤宏";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].EName = "Jave";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].SetFrameID( 411, 425, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].SetDropFrameID(411 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][8].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].HName = "匙付";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].EName = "Nema";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].SetFrameID( 414, 428, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].SetDropFrameID(414 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][9].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].HName = "酒肺荤";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].EName = "Arosa";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].SetFrameID( 415, 429, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].SetDropFrameID(415 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][10].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].HName = "瞒胶颇";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].EName = "Chaspa";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].SetFrameID( 413, 427, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].SetDropFrameID(413 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE][11].Price = 0;


	//-----------------------------------------------------------------------
	// 己 惑隆拱 酒捞袍 抛捞喉
	//-----------------------------------------------------------------------

	InitClass(ITEM_CLASS_CASTLE_SYMBOL, 6);

	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].HName = "荐龋狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].EName = "Amber of Guard";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].Description = "苛鸥何胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].SetFrameID( 426, 440, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].SetDropFrameID(426 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].HName = "历林狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].EName = "Amber of Curse";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].Description = "抛福萍快胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].SetFrameID( 428, 442, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].SetDropFrameID(428 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][1].Price = 0;

	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].HName = "汗荐狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].EName = "Amber of Avenger";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].Description = "剂萍公胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].SetFrameID( 427, 441, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].SetDropFrameID(427 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][2].Price = 0;

	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].HName = "康盔狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].EName = "Amber of Immortal";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].Description = "捻酒福捧胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].SetFrameID( 429, 443, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].SetDropFrameID(429 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][3].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].HName = "积疙狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].EName = "Amber of Life";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].Description = "奇鸥穿胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].SetFrameID( 859, 882, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].SetDropFrameID(859 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][4].Price = 0;


	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].HName = "蝴狼 骏滚";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].EName = "Amber of Light";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].Description = "琼矫府快胶 己狼 惑隆拱";
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].SetFrameID( 860, 883, 0);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].SetDropFrameID(860 );
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CASTLE_SYMBOL][5].Price = 0;


	//-----------------------------------------------------------------------
	// 目敲傅 酒捞袍 抛捞喉
	//-----------------------------------------------------------------------

	InitClass(ITEM_CLASS_COUPLE_RING, 2);

	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].HName = "巢磊侩 目敲傅";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].EName = "";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].Description = "厘馒窍脚 饶 坷弗率 努腐阑 窍矫搁 惑措祈 目敲捞 乐绰 困摹肺 捞悼 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].SetFrameID( 432, 446, 0);
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].SetDropFrameID(432 );
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].SetValue(3, 1, -1, -1, -1, 0);	
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].HName = "咯磊侩 目敲傅";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].EName = "";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].Description = "厘馒窍脚 饶 坷弗率 努腐阑 窍矫搁 惑措祈 目敲捞 乐绰 困摹肺 捞悼 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].SetFrameID( 433, 447, 0);
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].SetDropFrameID(433 );
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].SetValue(3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_COUPLE_RING][1].Price = 0;
	

	//-----------------------------------------------------------------------
	// 轨颇捞绢目敲傅 酒捞袍 抛捞喉
	//-----------------------------------------------------------------------

	InitClass(ITEM_CLASS_VAMPIRE_COUPLE_RING, 2);

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].HName = "巢磊侩 目敲傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].EName = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].Description = "厘馒窍脚 饶 坷弗率 努腐阑 窍矫搁 惑措祈 目敲捞 乐绰 困摹肺 捞悼 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].SetFrameID( 432, 446, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].SetDropFrameID(432 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].SetValue(3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].HName = "咯磊侩 目敲傅";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].EName = "";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].Description = "厘馒窍脚 饶 坷弗率 努腐阑 窍矫搁 惑措祈 目敲捞 乐绰 困摹肺 捞悼 且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].SetFrameID( 433, 447, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].SetDropFrameID(433 );
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].SetValue(3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_VAMPIRE_COUPLE_RING][1].Price = 0;

	//--------------------------------------------------------------------------
	// 捞亥飘 涅胶飘 酒捞袍 
	//--------------------------------------------------------------------------
	InitClass(ITEM_CLASS_EVENT_ITEM, 32);

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].HName = "拱狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].EName = "Soul Stone Of Water";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].SetFrameID( 504, 518,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].SetDropFrameID ( 504 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].HName = "拱狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].EName = "Soul Stone Of Water";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].SetFrameID( 505,519 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].SetDropFrameID ( 505 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][1].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].HName = "拱狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].EName = "Soul Stone Of Water";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].SetFrameID( 506, 520 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].SetDropFrameID ( 506 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][2].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].HName = "拱狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].EName = "Soul Stone Of Water";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].SetFrameID( 507, 521 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].SetDropFrameID ( 507 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][3].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].HName = "拱狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].EName = "Soul Stone Of Water";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].SetFrameID( 508, 522 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].SetDropFrameID ( 508 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][4].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].HName = "阂狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].EName = "Soul Stone Of Fire";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].SetFrameID( 509, 523 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].SetDropFrameID ( 509 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][5].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].HName = "阂狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].EName = "Soul Stone Of Fire";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].SetFrameID( 510, 524 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].SetDropFrameID ( 510 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][6].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].HName = "阂狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].EName = "Soul Stone Of Fire";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].SetFrameID( 511, 525 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].SetDropFrameID ( 511 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][7].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].HName = "阂狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].EName = "Soul Stone Of Fire";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].SetFrameID( 512, 526 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].SetDropFrameID ( 512 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][8].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].HName = "阂狼 沥飞";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].EName = "Soul Stone Of Fire";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].SetFrameID( 513, 527 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].SetDropFrameID ( 513 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][9].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].HName = "瘤档";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].EName = "Map";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].SetFrameID( 514, 528 ,0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].SetDropFrameID ( 514 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].SetValue( 3, 1, -1, -1, -1, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].SetGrid( 1, 1 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][10].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].HName = "炕肺快 瘤付飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].EName = "Yellow Zimott";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].SetFrameID( 740,754,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].SetDropFrameID( 740);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][11].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].HName = "弊赴 瘤付飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].EName = "Green Zimott";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].SetFrameID( 741,755,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].SetDropFrameID( 741 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][12].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].HName = "喉风 瘤付飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].EName = "Blue Zimott";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].SetFrameID( 742,756,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].SetDropFrameID( 742 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][13].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].HName = "饭靛 瘤付飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].EName = "Red Zimott";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].SetFrameID( 743,757,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].SetDropFrameID( 743 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][14].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].HName = "喉发 瘤付飘";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].EName = "Black Zimott";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].SetFrameID( 744,758,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].SetDropFrameID( 744 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][15].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].HName = "炕肺快 瘤福能";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].EName = "Yellow Zircon";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].SetFrameID( 745,759,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].SetDropFrameID( 745 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][16].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].HName = "弊赴 瘤福能";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].EName = "Green Zircon";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].SetFrameID( 746,760,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].SetDropFrameID( 746 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][17].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].HName = "喉风 瘤福能";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].EName = "Blue Zircon";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].SetFrameID( 747,761,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].SetDropFrameID( 747 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][18].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].HName = "饭靛 瘤福能";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].EName = "Red Zircon";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].SetFrameID( 748,762,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].SetDropFrameID( 748 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][19].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].HName = "喉发 瘤福能";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].EName = "Black Zircon";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].SetFrameID( 749,763,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].SetDropFrameID( 749 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][20].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].HName = "府乔聪框";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].EName = "Refinium";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].SetFrameID( 722,736,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].SetDropFrameID( 722 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][21].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].HName = "茎府荤令 付籍";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].EName = "Thurisaz Magic Pebble";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].SetFrameID( 723,737,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].SetDropFrameID( 723 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][22].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].HName = "霸焊 付籍";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].EName = "Gebo Magic Pebble";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].SetFrameID( 719,733,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].SetDropFrameID( 719 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][23].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].HName = "款炼 付籍";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].EName = "Wunjo Magic Pebble";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].SetFrameID( 724,738,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].SetDropFrameID( 724 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][24].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].HName = "力扼 付籍";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].EName = "Jera Magic Pebble";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].SetFrameID( 721,735,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].SetDropFrameID( 721 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][25].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].HName = "窍哎扼令 付籍";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].EName = "Hagalaz Magic Pebble";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].SetFrameID( 720,734,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].SetDropFrameID( 720 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][26].Price = 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].HName = "标惯";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].EName = "Flag";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].SetFrameID( 750,764,0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].SetDropFrameID( 750 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].SetGrid( 1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][27].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].HName = "橇府固决 1林老 背券鼻";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].EName = "Ticket to the Premium Zone";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].Description = "Ticket to the Premium Zone";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].SetFrameID( 854, 877, 0 );	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].SetDropFrameID( 854 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][28].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].HName = "竿牢狼 乔府";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].EName = "Pipe of the Blind";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].Description = "Pipe of the Blind";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].SetSoundID( SOUND_PIPE_ROUTING, SOUND_PIPE_DROP, SOUND_PIPE_DROP, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].SetFrameID( 861, 884, 0 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].SetDropFrameID( 861 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][29].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].HName = "措冠 扁盔 何利";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].EName = "Luck Charm";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].Description = "荐氰积狼 绊垫痢阑 厚绰 扁盔捞 淬变 何利.";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].SetSoundID( SOUND_PIPE_ROUTING, SOUND_PIPE_DROP, SOUND_PIPE_DROP, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].SetFrameID( 940, 976, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].SetDropFrameID(940 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][30].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].HName = "积疙狼 唱急";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].EName = "Life Spiral";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].Description = "";
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].SetSoundID( SOUND_PIPE_ROUTING, SOUND_PIPE_DROP, SOUND_PIPE_DROP, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].SetFrameID( 977, 1011, 0);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].SetDropFrameID( 977 );
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EVENT_ITEM][31].Price	= 0;
	
	// ----------------------------------------------------------------
	// 堪祸距
	// ----------------------------------------------------------------
	InitClass(ITEM_CLASS_DYE_POTION, 62 );
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].HName = "赣府堪祸距 1";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].EName = "Hair-Dye 1";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].SetFrameID( 523 , 537 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].SetDropFrameID( 523 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][0].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].HName = "赣府堪祸距 2";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].EName = "Hair-Dye 2";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].SetFrameID( 524, 538, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].SetDropFrameID( 524 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][1].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].HName = "赣府堪祸距 3";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].EName = "Hair-Dye 3";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].SetFrameID( 525, 539, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].SetDropFrameID( 525 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][2].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].HName = "赣府堪祸距 4";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].EName = "Hair-Dye 4";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].SetFrameID( 526, 540, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].SetDropFrameID( 526 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][3].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].HName = "赣府堪祸距 5";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].EName = "Hair-Dye 5";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].SetFrameID( 527, 541, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].SetDropFrameID( 527 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][4].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].HName = "赣府堪祸距 6";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].EName = "Hair-Dye 6";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].SetFrameID( 528, 542, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].SetDropFrameID( 528 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][5].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].HName = "赣府堪祸距 7";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].EName = "Hair-Dye 7";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].SetFrameID( 529 , 543, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].SetDropFrameID( 529 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][6].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].HName = "赣府堪祸距 8";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].EName = "Hair-Dye 8";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].SetFrameID( 530, 544, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].SetDropFrameID( 530 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][7].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].HName = "赣府堪祸距 9";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].EName = "Hair-Dye 9";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].SetFrameID( 531, 545, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].SetDropFrameID( 531);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][8].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].HName = "赣府堪祸距 10";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].EName = "Hair-Dye 10";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].SetFrameID( 532, 546, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].SetDropFrameID( 532 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][9].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].HName = "赣府堪祸距 11";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].EName = "Hair-Dye 11";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].SetFrameID( 533, 547, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].SetDropFrameID( 533);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].SetValue( 3, 1, -1, -1, -1, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][10].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].HName = "赣府堪祸距 12";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].EName = "Hair-Dye 12";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].SetFrameID( 534, 548, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].SetDropFrameID( 534);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][11].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].HName = "赣府堪祸距 13";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].EName = "Hair-Dye 13";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].SetFrameID( 535 , 549, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].SetDropFrameID( 535);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][12].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].HName = "赣府堪祸距 14";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].EName = "Hair-Dye 14";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].SetFrameID( 536, 550, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].SetDropFrameID( 536);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][13].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].HName = "赣府堪祸距 15";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].EName = "Hair-Dye 15";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].SetFrameID( 537, 551, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].SetDropFrameID( 537);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][14].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].HName = "赣府堪祸距 16";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].EName = "Hair-Dye 16";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].SetFrameID( 538, 552, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].SetDropFrameID( 538);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][15].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].HName = "赣府堪祸距 17";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].EName = "Hair-Dye 17";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].SetFrameID( 539 , 553, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].SetDropFrameID( 539);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][16].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].HName = "赣府堪祸距 18";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].EName = "Hair-Dye 18";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].SetFrameID( 540, 554 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].SetDropFrameID( 540 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][17].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].HName = "赣府堪祸距 19";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].EName = "Hair-Dye 19";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].SetFrameID( 541, 555 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].SetDropFrameID( 541 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][18].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].HName = "赣府堪祸距 20";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].EName = "Hair-Dye 20";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].SetFrameID( 542,  556, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].SetDropFrameID( 542 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][19].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].HName = "赣府堪祸距 21";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].EName = "Hair-Dye 21";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].SetFrameID( 543, 557 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].SetDropFrameID( 543 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][20].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].HName = "赣府堪祸距 22";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].EName = "Hair-Dye 22";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].SetFrameID( 544, 558, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].SetDropFrameID( 544 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][21].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].HName = "赣府堪祸距 23";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].EName = "Hair-Dye 23";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].SetFrameID( 545 , 559 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].SetDropFrameID( 545 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][22].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].HName = "赣府堪祸距 24";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].EName = "Hair-Dye 24";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].Description = "某腐磐狼 赣府祸阑 函版秦 凛聪促.(轨颇捞绢 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].SetFrameID( 546, 560, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].SetDropFrameID( 546 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][23].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].HName = "乔何堪祸距 1";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].EName = "Skin-Dye 1";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].SetFrameID( 547, 561, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].SetDropFrameID( 547 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][24].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].HName = "乔何堪祸距 2";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].EName = "Skin-Dye 2";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].SetFrameID( 548,  562, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].SetDropFrameID( 548 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][25].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].HName = "乔何堪祸距 3";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].EName = "Skin-Dye 3";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].SetFrameID( 549, 563, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].SetDropFrameID( 549);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][26].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].HName = "乔何堪祸距 4";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].EName = "Skin-Dye 4";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].SetFrameID( 550, 564, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].SetDropFrameID( 550 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][27].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].HName = "乔何堪祸距 5";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].EName = "Skin-Dye 5";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].SetFrameID( 551, 565, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].SetDropFrameID( 551 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][28].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].HName = "乔何堪祸距 6";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].EName = "Skin-Dye 6";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].SetFrameID( 552, 566, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].SetDropFrameID( 552 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][29].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].HName = "乔何堪祸距 7";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].EName = "Skin-Dye 7";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].SetFrameID( 553 , 567, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].SetDropFrameID( 553);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][30].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].HName = "乔何堪祸距 8";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].EName = "Skin-Dye 8";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].SetFrameID( 554, 568, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].SetDropFrameID( 554 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][31].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].HName = "乔何堪祸距 9";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].EName = "Skin-Dye 9";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].SetFrameID( 555, 569, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].SetDropFrameID( 555 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][32].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].HName = "乔何堪祸距 10";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].EName = "Skin-Dye 10";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].SetFrameID( 556, 570, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].SetDropFrameID( 556 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][33].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].HName = "乔何堪祸距 11";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].EName = "Skin-Dye 11";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].SetFrameID( 557, 571, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].SetDropFrameID( 557 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][34].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].HName = "乔何堪祸距 12";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].EName = "Skin-Dye 12";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].SetFrameID( 558, 572, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].SetDropFrameID( 558 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][35].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].HName = "乔何堪祸距 13";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].EName = "Skin-Dye 13";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].SetFrameID( 559, 573, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].SetDropFrameID( 559 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][36].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].HName = "乔何堪祸距 14";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].EName = "Skin-Dye 14";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].SetFrameID( 560, 574 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].SetDropFrameID( 560 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][37].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].HName = "乔何堪祸距 15";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].EName = "Skin-Dye 15";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].SetFrameID( 561, 575, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].SetDropFrameID( 561 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][38].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].HName = "乔何堪祸距 16";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].EName = "Skin-Dye 16";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].SetFrameID( 562, 576, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].SetDropFrameID( 562 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][39].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].HName = "乔何堪祸距 17";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].EName = "Skin-Dye 17";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].SetFrameID( 563, 577 , 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].SetDropFrameID( 563 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][40].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].HName = "乔何堪祸距 18";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].EName = "Skin-Dye 18";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].SetFrameID( 564, 578, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].SetDropFrameID( 564);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][41].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].HName = "乔何堪祸距 19";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].EName = "Skin-Dye 19";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].SetFrameID( 565, 579, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].SetDropFrameID( 565 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][42].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].HName = "乔何堪祸距 20";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].EName = "Skin-Dye 20";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].SetFrameID( 566, 580, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].SetDropFrameID( 566 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][43].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].HName = "乔何堪祸距 21";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].EName = "Skin-Dye 21";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].SetFrameID( 567, 581, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].SetDropFrameID( 567 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][44].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].HName = "乔何堪祸距 22";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].EName = "Skin-Dye 22";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].SetFrameID( 568, 582, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].SetDropFrameID( 568 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][45].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].HName = "乔何堪祸距 23";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].EName = "Skin-Dye 23";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].SetFrameID( 569, 583, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].SetDropFrameID( 569);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][46].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].HName = "乔何堪祸距 24";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].EName = "Skin-Dye 24";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].Description = "某腐磐狼 乔何祸阑 函版秦 凛聪促. (酒快胶磐令 荤侩 阂啊瓷)";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].SetFrameID( 570, 584, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].SetDropFrameID( 570 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][47].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].HName = "己傈券 酒捞袍";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].EName = "Change Sex Item";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].Description = "某腐磐狼 己喊阑 函版钦聪促.";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].SetFrameID( 711, 725, 0);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].SetDropFrameID( 711 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][48].Price =0;

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].HName = "弊赴 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].EName = "Green Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].SetFrameID( 775, 798, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].SetDropFrameID( 775 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][49].SetGrid( 1, 1 );
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].HName = "喉风 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].EName = "Blue Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].SetFrameID( 776, 799, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].SetDropFrameID( 776 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][50].SetGrid( 1, 1 );
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].HName = "饭靛 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].EName = "Red Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].SetFrameID( 777, 800, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].SetDropFrameID( 777 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][51].SetGrid( 1, 1 );

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].HName = "炕肺快 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].EName = "Yellow Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].SetFrameID( 778, 801, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].SetDropFrameID( 778 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][52].SetGrid( 1, 1 );

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].HName = "拳捞飘 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].EName = "White Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].SetFrameID( 779, 802, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].SetDropFrameID( 779 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][53].SetGrid( 1, 1 );

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].HName = "胶墨捞 喉风 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].EName = "Sky Blue Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].SetFrameID( 780, 803, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].SetDropFrameID( 780 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][54].SetGrid( 1, 1 );

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].HName = "扼老遏 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].EName = "Lilac Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].SetFrameID( 781, 804, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].SetDropFrameID( 781 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][55].SetGrid( 1, 1 );
	
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].HName = "喉发 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].EName = "Black Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].SetFrameID( 782, 805, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].SetDropFrameID( 782 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][56].SetGrid( 1, 1 );

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].HName = "匙抿凡 墨羔饭柯 器记";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].EName = "Natural Chameleon Potion";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].Description = "";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].SetFrameID( 783, 806, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].SetDropFrameID( 783 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][57].SetGrid( 1, 1 );	

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].HName = "乔绢 坷坷扼 胶沛";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].EName = "Fear Aura Stone";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].Description = "100饭骇 捞惑 某腐磐父 荤侩且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].SetFrameID( 982,1016, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].SetDropFrameID( 982 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][58].SetGrid( 1, 1 );	

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].HName = "农凡 坷坷扼 胶沛";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].EName = "Cruel Aura Stone";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].Description = "100饭骇 捞惑 某腐磐父 荤侩且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].SetFrameID( 983,1017, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].SetDropFrameID( 983 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][59].SetGrid( 1, 1 );	

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].HName = "龋橇 坷坷扼 胶沛";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].EName = "Hope Aura Stone";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].Description = "100饭骇 捞惑 某腐磐父 荤侩且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].SetFrameID( 984,1018, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].SetDropFrameID( 984 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][60].SetGrid( 1, 1 );	

	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].HName = "郴抿凡 坷坷扼 胶沛";								  
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].EName = "Natural Aura Stone";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].Description = "100饭骇 捞惑 某腐磐父 荤侩且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].SetFrameID( 981, 1015, 0 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].SetDropFrameID( 981 );
	m_pTypeInfo[ITEM_CLASS_DYE_POTION][61].SetGrid( 1, 1 );	



	// ----------------------------------------------------------------------
	// 何劝 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_RESURRECT_ITEM, 2);
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].HName = "何劝 胶农费";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].EName = "Resurrection Scroll";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].Description = "后荤 惑怕俊辑 促矫 何劝 且 锭 荤侩窍绰 酒捞袍 涝聪促";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].SetFrameID( 589, 603, 0);
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].SetDropFrameID( 589 );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][0].Price =0;

	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].HName = "郡腐辑 胶农费";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].EName = "Elixir Scroll";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].Description = "后荤惑怕俊辑 何劝 棺 老何 捞惑 惑怕甫 肯傈窍霸 雀汗且 荐 乐绰 酒捞袍 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].SetFrameID( 588, 602, 0);
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].SetDropFrameID( 588 );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].SetValue( 3,1,-1,-1,-1,0 );
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].SetGrid( 1, 1);
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_RESURRECT_ITEM][1].Price =0;
	
	// ----------------------------------------------------------------------
	// 雇教器瘤 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_MIXING_ITEM, 25 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].HName = "傀锹 雇教 器瘤 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].EName = "Weapon Mixing Forge Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].SetFrameID( 582,596,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].SetDropFrameID( 582 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].HName = "傀锹 雇教 器瘤 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].EName = "Weapon Mixing Forge Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].SetFrameID( 582,596,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].SetDropFrameID( 582 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][1].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].HName = "傀锹 雇教 器瘤 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].EName = "Weapon Mixing Forge Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].SetFrameID( 582,596,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].SetDropFrameID( 582 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][2].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].HName = "酒赣 雇教 器瘤 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].EName = "Armor Mixing Forge Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].SetFrameID( 584,598,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].SetDropFrameID(584);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][3].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].HName = "酒赣 雇教 器瘤 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].EName = "Armor Mixing Forge Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].SetFrameID( 584,598,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].SetDropFrameID(584);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][4].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].HName = "酒赣 雇教 器瘤 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].EName = "Armor Mixing Forge Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].SetFrameID( 584,598,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].SetDropFrameID(584);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][5].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].HName = "咀技辑府 雇教 器瘤 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].EName = "Accessory Mixing Forge Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].SetFrameID( 583,597,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].SetDropFrameID(583);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][6].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].HName = "咀技辑府 雇教 器瘤 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].EName = "Accessory Mixing Forge Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].SetFrameID( 583,597,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].SetDropFrameID(583);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][7].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].HName = "咀技辑府 雇教 器瘤 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].EName = "Accessory Mixing Forge Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].SetFrameID( 583,597,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].SetDropFrameID(583);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][8].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].HName = "傀锹 腔府鸥胶 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].EName = "Weapon Puritas Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].SetFrameID( 594,608,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].SetDropFrameID( 594 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][9].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].HName = "傀锹 腔府鸥胶 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].EName = "Weapon Puritas Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].SetFrameID( 594,608,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].SetDropFrameID( 594 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][10].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].HName = "傀锹 腔府鸥胶 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].EName = "Weapon Puritas Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].SetFrameID( 594,608,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].SetDropFrameID( 594 );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][11].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].HName = "酒赣 腔府鸥胶 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].EName = "Armor Puritas Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].SetFrameID( 595,609,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].SetDropFrameID(595);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][12].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].HName = "酒赣 腔府鸥胶 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].EName = "Armor Puritas Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].SetFrameID( 595,609,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].SetDropFrameID(595);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][13].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].HName = "酒赣 腔府鸥胶 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].EName = "Armor Puritas Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].SetFrameID( 595,609,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].SetDropFrameID(595);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][14].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].HName = "咀技辑府 腔府鸥胶 A屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].EName = "Accessory Puritas Type A";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].SetFrameID( 596,610,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].SetDropFrameID(596);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][15].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].HName = "咀技辑府 腔府鸥胶 B屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].EName = "Accessory Puritas Type B";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].SetFrameID( 596,610,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].SetDropFrameID(596);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][16].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].HName = "咀技辑府 腔府鸥胶 C屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].EName = "Accessory Puritas Type C";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].SetFrameID( 596,610,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].SetDropFrameID(596);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][17].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].HName = "汗炼府";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].EName = "bokjory";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].Description = "窜老 可记 酒捞袍阑 公可记 酒捞袍栏肺 函券矫难 凛聪促";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].SetFrameID( 978,1012,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].SetDropFrameID(978);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][18].Price = 0;


	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].HName = "傀锹 雇教 器瘤 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].EName = "Weapon Mixing Forge Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].SetFrameID( 582,596,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].SetDropFrameID(582);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][19].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].HName = "酒赣 雇教 器瘤 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].EName = "Armor Mixing Forge Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].SetFrameID( 584,598,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].SetDropFrameID(584);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][20].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].HName = "咀技辑府 雇教 器瘤 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].EName = "Accessory Mixing Forge Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].Description = "可记捞 促弗 鞍篮 酒捞袍阑 钦磨 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].SetFrameID( 583,597,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].SetDropFrameID(583);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].SetGrid(2,2);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][21].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].HName = "傀锹 腔府鸥胶 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].EName = "Weapon Puritas Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].SetFrameID( 594,608,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].SetDropFrameID(594);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][22].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].HName = "酒赣 腔府鸥胶 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].EName = "Armor Puritas Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].SetFrameID( 595,609,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].SetDropFrameID(595);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][23].Price = 0;

	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].HName = "咀技辑府 腔府鸥胶 D屈";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].EName = "Accessory Puritas Type D";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].Description = "饭绢酒捞袍狼 鞘夸绝绰 可记阑 力芭且 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING );
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].SetFrameID( 596,610,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].SetDropFrameID(596);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].SetValue( 3,1,-1,-1,-1,0);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].SetGrid(1,1);
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MIXING_ITEM][24].Price = 0;


	int itemType;

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 鞠胶龟靛 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_ARMSBAND, 14 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "快靛 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Wood Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(617, 631, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(617);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "酒啦海雕 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Ayurvedic Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(618, 632, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(618);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "饭牢焊快 傀捞宏 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Rainbow Wave Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(619, 633, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(619);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "拿烦 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Column Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(620, 634, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(620);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "喉扼硫 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Blossom Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(621, 635, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(621);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "技老瘤 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Selge Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(622, 636, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(622);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "橇府胶飘 庆府倾 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Priest Herihor Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(623, 637, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(623);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "磊房 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Zaret Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(624, 638, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(624);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "绊福埃 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Gurgan Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(625, 639, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(625);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "捻弊唱胶 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Kugnas Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(626, 640, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(626);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "虐纳促 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Cicada Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(817, 840, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(817);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "拱扼促胶 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Muladhara Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(818, 841, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(818);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "官牢靛 烽 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Bindrune Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(819, 842, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(819);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].HName = "靛扼捞绢靛腐 鞠胶龟靛";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].EName = "Dryadric Armsband";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetSoundID( SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND, SOUND_OUSTERS_ARMSBAND);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetFrameID(911 , 934 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_ARMSBAND][itemType].SetDropFrameID(911);
	itemType++;

	itemType = 0;


	// ----------------------------------------------------------------------
	// 酒快胶磐令 何明 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_BOOTS, 13 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "靛扼捞绢靛 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Dryad\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 692, 706, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(692);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "坷饭酒胶 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Oread\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 693, 707, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID( 693);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "胶橇扼捞飘 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Sprite\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 694, 708, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID( 694);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "乔胶飘芬 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Fisthrom\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 695, 709, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID( 695);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "饭橇府牧 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Leprekaun\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 696, 710, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID( 696);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "单瘤府浆 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Desirish\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 697, 711, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(697);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "廓 坷 歹 困胶橇 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Will O\' the Wisp\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 698, 712, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(698);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "溅捞靛 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Shade\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 699, 713, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(699);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "其捞飘府胶 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Fateris\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 811, 834, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(811);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "俊其府倔 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Epereal\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 812, 835, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(812);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "酒付抛扼胶 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Amaterasu\'s Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 813, 836, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(813);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "匙捞锹 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Naphne Boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 908 , 931,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(908);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].HName = "橇扼郡府框 何明";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].EName = "Praelium boots";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetSoundID( SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS, SOUND_OUSTERS_BOOTS);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetFrameID( 962, 998, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_BOOTS][itemType].SetDropFrameID(962 );
	itemType++;
	
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].HName = "烙其府框 何明";
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].EName = "Imperium boots";
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_GAITER][1].Price = 0;	


	itemType = 0;

	// ----------------------------------------------------------------------
	// 酒快胶磐令 铆农恩 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_CHAKRAM, 15 );

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "配快 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Tou Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(632, 646, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(632);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "欺宏 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Perv Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(633, 647, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(633);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "酒海胶 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Aves Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(634, 648, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(634);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "春焊 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Nuevo Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(635, 649, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(635);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "酒聪付 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Anima Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(636, 650, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(636);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "府飘扼 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Raetra Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(700, 714, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(700);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "其酱捞狼 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Pesuy\'s Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(701, 715, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(701);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "宏扼何胶 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Brabus Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(702, 716, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(702);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "皋掂 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Meduh Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(703, 717, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(703);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "农风坷福 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Cruor Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(704, 718, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(704);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "窍捞荤 巩 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Moon of Haisa Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(820, 843, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(820);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "酒令唱 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Ajna Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(821, 844, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(821);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "扩靛 矮 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Wind God Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(822, 845, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(822);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "风靛扼 铆农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Rudra Chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM, SOUND_OUSTERS_CHAKRAM);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID(914 , 937 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(914);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );
	itemType++;
				
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].HName = "羔府胶 瞒农恩";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].EName = "Melis chakram";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetFrameID( 964, 1000, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetDropFrameID(964 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CHAKRAM][itemType].SetAddonFrameID( 0, 0 );	
	itemType++;
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].HName = "鄂抛酒 瞒农恩";
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].EName = "Rantea chakram";
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_ORBIS][1].Price = 0;	

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 结努复 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_CIRCLET, 14 );
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "角滚 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Silver Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(607, 621, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(607);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "父岿狼 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Circlet of Full Moon";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(608, 622, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(608);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "奇鸥努 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Pentacle Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(609, 623, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(609);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "角官唱 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Sylvana Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(610, 624, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(610);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "拿烦 坷宏 况磐 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Column of Water Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(611, 625, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(611);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "肺歹 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Lothar Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(612, 626, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(612);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "辑努复 坷宏 家啦扼";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Circlet of Solyulia";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(613, 627, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(613);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "掂农狼 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Duke\'s Chrystopher Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(614, 628, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(614);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "气堪狼 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Prominence Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(615, 629, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(615);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "促捞绢待 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Diadem Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(616, 630, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(616);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "舅虐坷匙 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Alcyone Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(814, 837, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(814);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "付胶抛付 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Mastema Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(815, 838, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(815);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "鸥捞扼 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Taira Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(816, 839, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(816);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].HName = "龙府坷飘 辑努复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].EName = "Gilleot Circlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetSoundID( SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET, SOUND_OUSTERS_CIRCLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetFrameID(927 , 950 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_CIRCLET][itemType].SetDropFrameID(927);
	itemType++;

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 内飘 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_COAT, 13 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "靛扼捞绢靛 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Dryad\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(662, 676, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(662);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 1, 1 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "坷饭酒胶 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Oread\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(663, 677, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(663);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 1, 1 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "胶橇扼捞飘 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Sprite\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(664, 678, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(664);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 1, 1 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "乔胶飘芬 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Fisthrom\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(665, 679, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(665);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 2, 2 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "饭橇府牧 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Leprekaun\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(705, 719, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(705);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 2, 2 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "单瘤府浆 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Desirish\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(706, 720, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(706);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 2, 2 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "廓 坷 歹 困胶橇 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Will o\' the wisp\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(707, 721, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(707);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "嘉捞靛 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Shade\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(708, 722, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(708);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "其捞飘府胶 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Fateris\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(824, 846, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(824);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "俊其府倔 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Epereal\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(823, 847, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(823);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "酒付抛扼胶 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Amaterasu\'s Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(825, 848, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(825);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "匙捞锹 内飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Naphne Coat";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID(909 , 932 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(909);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].HName = "橇扼郡府框 海胶萍坷";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].EName = "Praelium vestio";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetSoundID( SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT, SOUND_OUSTERS_COAT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetFrameID( 963, 999, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetDropFrameID(963 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_COAT][itemType].SetAddonFrameID( 3, 3 );
	itemType++;
				
	
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].HName = "烙其府框 海胶萍坷";
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].EName = "Imperium vestio";
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_CASCA][1].Price = 0;	

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 奇带飘 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_PENDENT, 14 );
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "府滚胶 村 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Revers Dot Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(597, 611, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(597);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "坷府箕 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = 			"Orissa Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(598, 612, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(598);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "固胶平 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = 			"Mystic Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(599, 613, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(599);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "捞廉角 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = 			"Yggdrasil Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(600, 614, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(600);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "矫春捞狼 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Sinui\'s Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(601, 615, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(601);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "酒捞令 坷宏 龋矾胶 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Eyes of Horus Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(602, 616, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(602);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "浚飘 其捞胶 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Ent Face Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(603, 617, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(603);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "乔烘扼 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = 			"Fibula Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(604, 618, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(604);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "乔绢府 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = 			"Fairie Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(605, 619, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(605);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "捞笼萍救 迫能 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Egyptian Falcon Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(606, 620, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(606);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "乔弊 飘府 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Fig Tree Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(826, 849, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(826);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "坷迫风胶 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Opalus Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(827, 850, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(827);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "墨惯扼 飘府 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Kabbala Tree Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(828, 851, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(828);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].HName = "饭浆 奇带飘";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].EName = "Leshy Pendent";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetSoundID( SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT, SOUND_OUSTERS_PENDANT);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetFrameID(913 , 936 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_PENDENT][itemType].SetDropFrameID(913);
	itemType++;
	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 傅 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_RING, 14 );
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "丘橇狼 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Hemp\'s Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(637, 651, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(637);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "伎饭胶鸥 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Celesta Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(638, 652, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(638);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "技捞橇啊靛 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Safeguard Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(639, 653, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(639);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "困胶欺 坷宏 郡橇 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Whisper of Elf Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(640, 654, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(640);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "骏器胶 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Mpos Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(641, 655, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(641);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "抛老令 坷宏 箕牢 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Tails of Shine Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(687, 701, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(687);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "胶颇捞歹 酒捞令 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Spider Eyes Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(688, 702, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(688);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "贾府磐府 胶鸥 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Solitary Star Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(689, 703, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(689);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "皋农莉狼 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Macsan\'s Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(690, 704, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(690);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "捞葛福呕矾萍 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Immortality Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(691, 705, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(691);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "救崔风矫酒 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Andalusia Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(808, 831, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(808);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "付福迪 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Marduk Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(809, 832, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(809);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "富扼墨捞飘 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Malachite Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(810, 833, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(810);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].HName = "荤捞坊 傅";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].EName = "Siren Ring";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetSoundID( SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING, SOUND_OUSTERS_RING);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetFrameID(918 , 962 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_RING][itemType].SetDropFrameID(918);
	itemType++;

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 沥飞籍 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_STONE, 15 );
		
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "阂狼 沥飞籍1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Fire ElementalStone 1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(652, 666, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(652);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "阂狼 沥飞籍2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Fire ElementalStone 2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(653, 667, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(653);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "阂狼 沥飞籍3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Fire ElementalStone 3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(654, 668, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(654);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "阂狼 沥飞籍4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Fire ElementalStone 4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(655, 669, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(655);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "阂狼 沥飞籍5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Fire ElementalStone 5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(656, 670, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(656);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "拱狼 沥飞籍1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Water ElementalStone 1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(657, 671, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(657);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "拱狼 沥飞籍2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Water ElementalStone 2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(658, 672, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(658);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "拱狼 沥飞籍3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Water ElementalStone 3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(659, 673, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(659);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "拱狼 沥飞籍4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Water ElementalStone 4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(660, 674, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(660);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "拱狼 沥飞籍5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Water ElementalStone 5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(661, 675, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(661);
	itemType++;

	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "措瘤狼 沥飞籍1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Earth ElementalStone 1";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(647, 661, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(647);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "措瘤狼 沥飞籍2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Earth ElementalStone 2";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(648, 662, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(648);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "措瘤狼 沥飞籍3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Earth ElementalStone 3";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(649, 663, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(649);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "措瘤狼 沥飞籍4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Earth ElementalStone 4";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(650, 664, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(650);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].HName = "措瘤狼 沥飞籍5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].EName = "Earth ElementalStone 5";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetSoundID( SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE, SOUND_OUSTERS_ELEMENTAL_STONE);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetFrameID(651, 665, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_STONE][itemType].SetDropFrameID(651);
	itemType++;

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 府胶撇复 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_WRISTLET, 45 );
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "郴抿凡 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Natural Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(627, 641, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(627);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "官牢 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Vine Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(628, 642, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(628);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼敌 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lardun Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(629, 643, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(629);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "夸扔狼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Yohwen\'s Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(630, 644, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(630);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "葱胶敲肺令 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nixflos Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(631, 645, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(631);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼农府付 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lacrima Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(682, 696, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(682);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "祭乔穿胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sempitrnus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(683, 697, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(683);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "刚滴胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Mundus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(684, 698, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(684);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "魂农捧胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sanctus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(685, 699, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(685);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "配府靛 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Torrid Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(686, 700, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(686);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "郴抿凡 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Natural Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(627, 641, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(627);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "官牢 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Vine Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(628, 642, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(628);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼敌 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lardun Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(629, 643, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(629);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "夸扔狼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Yohwen\'s Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(630, 644, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(630);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "葱胶敲肺令 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nixflos Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(631, 645, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(631);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼农府付 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lacrima Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(682, 696, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(682);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "祭乔穿胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sempitrnus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(683, 697, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(683);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "刚滴胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Mundus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(684, 698, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(684);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "魂农捧胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sanctus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(685, 699, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(685);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "配府靛 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Torrid Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(686, 700, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(686);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "郴抿凡 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Natural Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(627, 641, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(627);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "官牢 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Vine Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(628, 642, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(628);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼敌 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lardun Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(629, 643, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(629);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "夸扔狼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Yohwen\'s Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(630, 644, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(630);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "葱胶敲肺令 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nixflos Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(631, 645, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(631);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "扼农府付 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Lacrima Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(682, 696, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(682);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "祭乔穿胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sempitrnus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(683, 697, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(683);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "刚滴胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Mundus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(684, 698, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(684);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "魂农捧胶 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Sanctus Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(685, 699, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(685);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "配府靛 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Torrid Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(686, 700, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(686);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "淬惯扼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Damballah Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(805, 828, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(805);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙农海飘 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nekhbet Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(806, 829, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(806);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "淬惯扼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Damballah Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(805, 828, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(805);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙农海飘 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nekhbet Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(806, 829, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(806);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "淬惯扼 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Damballah Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(805, 828, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(805);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙农海飘 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nekhbet Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(806, 829, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(806);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "内胶雇 俊弊 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Cosmic Egg Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(807, 830, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(807);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "内胶雇 俊弊 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Cosmic Egg Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(807, 830, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(807);
	itemType++;
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "内胶雇 俊弊 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Cosmic Egg Wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(807, 830, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(807);
	itemType++;

	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "阂狼 迫官萍 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Palvati wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(937 , 960 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(937);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "拱狼 迫官萍 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Palvati wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(937 , 960 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(937);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "措瘤狼  迫官萍 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Palvati wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(937 , 960 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(937);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙乔覆 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nepirim wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(961, 997 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(961);
	itemType++;
			
		m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙乔覆 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nepirim wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(961, 997 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(961);
	itemType++;
		m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].HName = "匙乔覆 府胶撇复";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].EName = "Nepirim wristlet";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetSoundID( SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET, SOUND_OUSTERS_WRISTLET);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetFrameID(961, 997 ,0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_WRISTLET][itemType].SetDropFrameID(961);
	itemType++;
	
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].HName = "酒福抛唱 府胶撇复";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].EName = "Altena wristlet";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][1].Price = 0;
//	
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].HName = "匙乔覆 府胶撇复";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].EName = "Nepirim wristlet";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][2].Price = 0;
//
//		m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].HName = "酒福抛唱 府胶撇复";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].EName = "Altena wristlet";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][3].Price = 0;
//	
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].HName = "匙乔覆 府胶撇复";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].EName = "Nepirim wristlet";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][4].Price = 0;	
//
//
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].HName = "酒福抛唱 府胶撇复";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].EName = "Altena wristlet";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PULSEIRA][5].Price = 0;

	
	
	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 扼官 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_LARVA, 5 );
	
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].HName = "弊赴 扼官";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].EName = "Green Larva";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetFrameID(667, 681, 0);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetDropFrameID(667);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].HName = "橇饭浆 扼官";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].EName = "Fresh Larva";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetFrameID(668, 682, 0);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetDropFrameID(668);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].HName = "局叼磒 扼官";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].EName = "Adequate Larva";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetFrameID(669, 683, 0);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetDropFrameID(669);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].HName = "沮敲 扼官";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].EName = "Ample Larva";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetFrameID(670, 684, 0);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetDropFrameID(670);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].HName = "胶溃 扼官";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].EName = "Swell Larva";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetFrameID(671, 685, 0);
	m_pTypeInfo[ITEM_CLASS_LARVA][itemType].SetDropFrameID(671);
	itemType++;

	
	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 仟颇 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_PUPA, 6 );
	
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "弊赴 仟颇";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Green Pupa";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(672, 686, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(672);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "橇饭浆 仟颇";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Fresh Pupa";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(673, 687, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(673);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "局叼磒 仟颇";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Adequate Pupa";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(674, 688, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(674);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "沮敲 仟颇";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Ample Pupa";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(675, 689, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(675);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "胶溃 仟颇";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Swell Pupa";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(676, 690, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(676);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].HName = "踌祸 荤帕";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].EName = "Green Candy";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_PUPA);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetFrameID(884, 907, 0);
	m_pTypeInfo[ITEM_CLASS_PUPA][itemType].SetDropFrameID(884);
	itemType++;
	
	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 皋捞 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_COMPOS_MEI, 5 );
	
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].HName = "弊赴 皋捞";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].EName = "Green Mei";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_MEI);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetFrameID(677, 691, 0);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetDropFrameID(677);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].HName = "橇饭浆 皋捞";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].EName = "Fresh Mei";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_MEI);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetFrameID(678, 692, 0);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetDropFrameID(678);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].HName = "局叼磒 皋捞";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].EName = "Adequate Mei";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_MEI);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetFrameID(678, 692, 0);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetDropFrameID(678);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].HName = "沮敲 皋捞";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].EName = "Ample Mei";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_MEI);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetFrameID(679, 693, 0);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetDropFrameID(679);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].HName = "胶溃 皋捞";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].EName = "Swell Mei";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetSoundID( SOUND_ITEM_MOVE_POTION, SOUND_ITEM_MOVE_POTION, SOUNDID_NULL, SOUND_OUSTERS_MEI);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetFrameID(679, 693, 0);
	m_pTypeInfo[ITEM_CLASS_COMPOS_MEI][itemType].SetDropFrameID(679);
	itemType++;

	itemType = 0;
	// ----------------------------------------------------------------------
	// 酒快胶磐令 联 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_OUSTERS_SUMMON_ITEM, 11 );
	
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "角橇 辑阁 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Sylph Summon Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(680, 694, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(680);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "角扼其 辑阁 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Sylaphe Summon Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(680, 694, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(680);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "角扼捞沸 辑阁 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Sylairon Summon Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(680, 694, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(680);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "弊赴 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Green Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(784, 807, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(784);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "坷坊瘤 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Orange Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(785, 808, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(785);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "饭靛 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Red Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(786, 809, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(786);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "炕肺快 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Yellow Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(787, 810, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(787);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "拳捞飘 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "White Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(788, 811, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(788);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "宏扼款 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Brown Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(789, 812, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(789);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "扼老遏 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Lilac Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(790, 813, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(790);
	itemType++;

	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].HName = "喉发 辑阁 烙橇 联";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].EName = "Black Summon Imp Gem";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].Description = "";
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetFrameID(791, 814, 0);
	m_pTypeInfo[ITEM_CLASS_OUSTERS_SUMMON_ITEM][itemType].SetDropFrameID(791);
	itemType++;
	
	// ----------------------------------------------------------------------
	// 酒快胶磐令 联 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_EFFECT_ITEM, 7 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].HName = "犬己扁 1";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].EName = "Megaphone 1";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].Description = "坷弗率 滚瓢 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].SetSoundID( SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].SetFrameID(710, 724, 0);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][0].SetDropFrameID(710);
	
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].HName = "犬己扁 2";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].EName = "Megaphone 2";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].Description = "坷弗率 滚瓢 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].SetSoundID( SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].SetFrameID(710, 724, 0);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][1].SetDropFrameID(710);
	 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].HName = "犬己扁 3";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].EName = "Megaphone 3"; 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].Description = "坷弗率 滚瓢 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].SetSoundID( SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL,SOUNDID_NULL);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].SetFrameID(710, 724, 0);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][2].SetDropFrameID(710);

	// 2004, 5, 18 sobeit add start
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].HName = "锅开扁";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].EName = "Translator"; 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].Description = "坷弗率 滚瓢 努腐栏肺 荤侩窍角 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].SetFrameID( 579, 593, 0 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].SetDropFrameID( 579 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][3].Price = 1;
	// 2004, 5, 18 sobeit add end

	// 2005, 1, 11 sobeit add start - 版氰摹 焊呈胶 葛贰矫拌
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].HName = "窍鞭 葛贰矫拌";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].EName = "L-grade Hourglass"; 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].Description = "版氰摹 焊呈胶 酒捞袍";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].SetFrameID( 972, 1006, 0 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].SetDropFrameID( 972 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][4].Price = 1;

	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].HName = "吝鞭 葛贰矫拌";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].EName = "M-grade Hourglass"; 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].Description = "版氰摹 焊呈胶 酒捞袍";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].SetFrameID( 971, 1005, 0 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].SetDropFrameID( 971 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][5].Price = 1;
	

	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].HName = "S-grade Hourglass";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].EName = "Translator"; 
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].Description = "版氰摹 焊呈胶 酒捞袍";
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_C4, SOUND_ITEM_MOVE_C4, SOUNDID_NULL, SOUND_ITEM_USE_C4);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].SetFrameID( 970, 1004, 0 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].SetDropFrameID( 970 );
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_EFFECT_ITEM][6].Price = 1;
	// 2004, 5, 18 sobeit add end

	// ----------------------------------------------------------------------
	// 酒快胶磐令 联 酒捞袍 努贰胶
	// ----------------------------------------------------------------------
	InitClass(ITEM_CLASS_CODE_SHEET, 2 );
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].HName = "内靛钎";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].EName = "Code Sheet";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].Description = "坷弗率 滚瓢 努腐栏肺 内靛钎甫 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].SetFrameID( 491, 505, 0 );	
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][0].SetDropFrameID( 491 );

	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].HName = "内靛钎";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].EName = "Code Sheet";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].Description = "坷弗率 滚瓢 努腐栏肺 内靛钎甫 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].SetFrameID( 491, 505, 0 );	
	m_pTypeInfo[ITEM_CLASS_CODE_SHEET][1].SetDropFrameID( 491 );

	//----------------------------------------------------------------------
	// 崔墨靛 
	//----------------------------------------------------------------------
	InitClass(ITEM_CLASS_MOON_CARD, 5 );
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].HName = "馆崔 墨靛";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].EName = "The Half Moon Card";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].Description = "腹捞 腹捞 葛酒 焊酒夸";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].SetFrameID( 770, 784, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][0].SetDropFrameID( 770 );

	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].HName = "檬铰崔 墨靛";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].EName = "The New Moon Card";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].SetFrameID( 769, 783, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][1].SetDropFrameID( 769 );

	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].HName = "Skill Card";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].EName = "Skill Card";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].Description = "使用新技能必需要的道具.";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].SetFrameID( 838, 861, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][2].SetDropFrameID( 838 );

	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].HName = "匙蕾 努肺滚";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].EName = "a four-leaf clover";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].Description = "青款 捞亥飘侩 酒捞袍. 殿荐措肺 惑前阑 靛妨夸.";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].SetFrameID( 858, 881, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][3].SetDropFrameID( 858 );
	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].HName = "颊狼 岿拌包";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].EName = "Son's Laurel Crown";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].SetFrameID( 882, 905, 0 );	
	m_pTypeInfo[ITEM_CLASS_MOON_CARD][4].SetDropFrameID( 882 );
	

	//--------------------------------------------------------------------------
	// 胶困欺
	//--------------------------------------------------------------------------
	InitClass(ITEM_CLASS_SWEEPER, 12 );
	
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].HName = "M-03-01";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].EName = "M-03-01";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].SetFrameID( 772,786,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][0].SetDropFrameID( 772 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].HName = "M-03-02";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].EName = "M-03-02";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].SetFrameID( 773,787,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][1].SetDropFrameID( 773 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].HName = "M-03-03";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].EName = "M-03-03";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].SetFrameID( 774,788,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][2].SetDropFrameID( 774 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].HName = "M-03-04";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].EName = "M-03-04";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].SetFrameID( 772,789,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][3].SetDropFrameID( 772 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].HName = "M-03-05";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].EName = "M-03-05";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].SetFrameID( 773,790,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][4].SetDropFrameID( 773 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].HName = "M-03-06";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].EName = "M-03-06";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].SetFrameID( 774,791,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][5].SetDropFrameID( 774 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].HName = "M-03-07";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].EName = "M-03-07";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].SetFrameID( 772,792, 0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][6].SetDropFrameID( 772 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].HName = "M-03-08";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].EName = "M-03-08";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].SetFrameID( 773,793,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][7].SetDropFrameID( 773 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].HName = "M-03-09";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].EName = "M-03-09";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].SetFrameID( 774,794,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][8].SetDropFrameID( 774 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].HName = "M-03-10";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].EName = "M-03-10";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].SetFrameID( 772,795,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][9].SetDropFrameID( 772 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].HName = "M-03-11";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].EName = "M-03-11";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].SetFrameID( 773,796,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][10].SetDropFrameID( 773 );

	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].HName = "M-03-12";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].EName = "M-03-12";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].SetSoundID( SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].SetFrameID( 774,797,0);
	m_pTypeInfo[ITEM_CLASS_SWEEPER][11].SetDropFrameID( 774 );
	
	//----------------------------------------------------------------------
	// 脐酒捞袍
	//----------------------------------------------------------------------
	InitClass(ITEM_CLASS_PET_ITEM, 6 );
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].HName = "啊楼 硅瘤";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].EName = "Gara Bezz";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].Description = "硅玛绰 拱矾啊扼~! 拱矾啊扼~!";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].SetFrameID( 797, 820, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][0].SetDropFrameID( 797 );

	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].HName = "匡橇刀 格临";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].EName = "Wolfdog Leash";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].Description = "匡橇刀阑 家券且 锭 静捞绰 俺格吧捞 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].SetFrameID( 797, 820, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][1].SetDropFrameID( 797 );

	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].HName = "匡滚赴 格临";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].EName = "Wolverine Leash";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].Description = "匡滚赴阑 家券且 锭 静捞绰 俺格吧捞 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].SetFrameID( 803, 826, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][2].SetDropFrameID( 803 );

	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].HName = "公急 炼辆扁";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].EName = "Radio Controller";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].Description = "季鸥快肺甫 家券且 锭 静捞绰 酒捞袍 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].SetFrameID( 839, 862, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][3].SetDropFrameID( 839 );

	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].HName = "冠零 啊磷 啊规";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].EName = "Stirge Bag";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].Description = "胶萍瘤甫 家券且 锭 静捞绰 酒捞袍 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].SetFrameID( 840, 863, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][4].SetDropFrameID( 840 );

	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].HName = "家券狼 倒";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].EName = "Summon Pixie";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].Description = "侨矫甫 家券且 锭 静捞绰 酒捞袍 涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].SetFrameID( 845, 868, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ITEM][5].SetDropFrameID( 845 );


	//----------------------------------------------------------------------
	// 脐冈捞酒捞袍
	//----------------------------------------------------------------------
	InitClass(ITEM_CLASS_PET_FOOD, 18 );
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].HName = "累篮 绊扁";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].EName = "Mini Meat";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].Description = "6矫埃 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].SetFrameID( 792, 815, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][0].SetDropFrameID( 792);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].HName = "啊涵款 绊扁";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].EName = "Light Meat";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].Description = "22矫埃 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].SetFrameID( 793, 816, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][1].SetDropFrameID( 793);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].HName = "馆率 绊扁";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].EName = "Half Meat";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].Description = "36矫埃 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].SetFrameID( 794, 817, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][2].SetDropFrameID( 794);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].HName = "奴 绊扁";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].EName = "Large Meat";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].Description = "50矫埃 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].SetFrameID( 795, 818, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][3].SetDropFrameID( 795);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].HName = "荤丰 仟措";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].EName = "Fodder";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].Description = "7老 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].SetFrameID( 804, 827, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][4].SetDropFrameID( 804);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].HName = "匡飘扼 仟措";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].EName = "Ultra Fodder";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].Description = "15老 楼府 匡橇刀/匡滚赴侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].SetFrameID( 796, 819, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][5].SetDropFrameID( 796);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].HName = "窍橇 颇况蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].EName = "Half Power Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].Description = "22矫埃 楼府 季鸥快肺侩 傈瘤涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].SetFrameID( 846, 869, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][6].SetDropFrameID( 846);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].HName = "扼瘤 颇况蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].EName = "Large Power Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].Description = "36矫埃 楼府 季鸥快肺侩 傈瘤涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].SetFrameID( 847, 870, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][7].SetDropFrameID( 847);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].HName = "窍橇 酱欺 颇况蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].EName = "Half Super Power Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].Description = "50矫埃 楼府 季鸥快肺侩 傈瘤涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].SetFrameID( 848, 871, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][8].SetDropFrameID( 848);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].HName = "扼瘤 酱欺 颇况蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].EName = "Large Super Power Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].Description = "7老 楼府 季鸥快肺侩 傈瘤涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].SetFrameID( 849, 872, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][9].SetDropFrameID( 849);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].HName = "喉矾靛 蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].EName = "Blood Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].Description = "22矫埃 楼府 胶萍瘤侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].SetFrameID( 841, 864, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][10].SetDropFrameID( 841);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].HName = "丑绵 喉矾靛 蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].EName = "Enriched Blood Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].Description = "36矫埃 楼府 胶萍瘤侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].SetFrameID( 842, 865, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][11].SetDropFrameID( 842);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].HName = "绊丑绵 喉矾靛 蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].EName = "Super Enriched Blood Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].Description = "50矫埃 楼府 胶萍瘤侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].SetFrameID( 843, 866, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][12].SetDropFrameID( 843);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].HName = "檬绊丑绵 喉矾靛 蒲";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].EName = "Ultra Enriched Blood Pack";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].Description = "7老 楼府 胶萍瘤侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].SetFrameID( 844, 867, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][13].SetDropFrameID( 844);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].HName = "固聪掂";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].EName = "Mini Dew";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].Description = "22矫埃 楼府 侨矫侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].SetFrameID( 850, 873, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][14].SetDropFrameID( 850);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].HName = "扼捞飘掂";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].EName = "Light Dew";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].Description = "36矫埃 楼府 侨矫侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].SetFrameID( 851, 874, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][15].SetDropFrameID( 851);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].HName = "窍橇掂";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].EName = "Half Dew";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].Description = "50矫埃 楼府 侨矫侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].SetFrameID( 852, 875, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][16].SetDropFrameID( 852);

	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].HName = "扼瘤掂";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].EName = "Large Dew";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].Description = "7老 楼府 侨矫侩 冈捞涝聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].SetFrameID( 853, 876, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_FOOD][17].SetDropFrameID( 853);


	//----------------------------------------------------------------------
	// 脐 牢镁飘
	//----------------------------------------------------------------------
	InitClass(ITEM_CLASS_PET_ENCHANT_ITEM, 21 );
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].HName = "饭靛 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].EName = "Red Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].SetFrameID( 798, 821, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][0].SetDropFrameID( 798 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].HName = "塞 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].EName = "STR Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][1].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].HName = "刮酶己 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].EName = "DEX Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][2].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].HName = "瘤侥 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].EName = "INT Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][3].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].HName = "雀乔啦 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].EName = "Defense Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][4].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].HName = "橇肺咆记 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].EName = "Protection Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][5].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].HName = "青款 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].EName = "Luck Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][6].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].HName = "矫具 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].EName = "Vision Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][7].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].HName = "傍拜仿 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].EName = "Damage Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][8].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].HName = "付过 傍拜仿 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].EName = "Magic Damage Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][9].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].HName = "积疙仿 犁积 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].EName = "HP Regen Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][10].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].HName = "积疙仿 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].EName = "HP Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][11].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].HName = "疙吝伏 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].EName = "ToHit Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][12].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].HName = "府官捞国 悸";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].EName = "Revival Set";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].Description = "磷篮 局肯悼拱阑 老沥 犬伏肺 混副 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].SetFrameID( 800, 823, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][13].SetDropFrameID( 800 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].HName = "橇府固决 府官捞国 悸";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].EName = "Premium Revival Set";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].Description = "磷篮 局肯悼拱阑 老沥 犬伏肺 混副 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].SetFrameID( 801, 824, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][14].SetDropFrameID( 801 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].HName = "饭靛 滚靛";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].EName = "Red Bird";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].SetFrameID( 799, 822, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][15].SetDropFrameID( 799 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].HName = "葛电 瓷仿摹 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].EName = "All Attribute Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][16].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].HName = "付过仿 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].EName = "MP Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][17].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].HName = "农府萍拿 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].EName = "Critical Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][18].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].HName = "傍拜加档 炕肺快 靛酚";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].EName = "Attack Speed Yellow Drop";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].Description = "局肯悼拱阑 绕访矫懦 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].SetFrameID( 802, 825, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][19].SetDropFrameID( 802 );

	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].HName = "孤畔飘 牢璃记";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].EName = "Mutant Injection";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].Description = "荤侩窍搁 康备利栏肺 匡滚赴栏肺 函脚窍哥 登倒副 荐 绝嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].SetSoundID( SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].SetFrameID( 857, 880, 0 );	
	m_pTypeInfo[ITEM_CLASS_PET_ENCHANT_ITEM][20].SetDropFrameID( 857 );


	InitClass(ITEM_CLASS_LUCKY_BAG, 4);

	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].HName = "檬废祸 汗林赣聪";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].EName = "The Green Lucky Bag";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].SetFrameID( 830, 853, 0 );	
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].SetDropFrameID( 830 );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].HName = "颇鄂祸 汗林赣聪";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].EName = "The Blue Lucky Bag";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].SetFrameID( 831, 854, 0 );	
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].SetDropFrameID( 831 );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].HName = "炔陛祸 汗林赣聪";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].EName = "The Gold Lucky Bag";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].SetFrameID( 832, 855, 0 );	
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].SetDropFrameID( 832 );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].HName = "橇府固决 捻迄";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].EName = "Premium Coupon";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].Description = "50厘阑 葛栏搁 货肺款 技惑捞 凯赋聪促.";
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].SetFrameID( 855, 878, 0 );	
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].SetDropFrameID( 855 );
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_LUCKY_BAG][3].Price	= 0;

	//------------------------------------------------------------------
	//	SMS Item
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_SMS_ITEM, 9);

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].HName = "U2A 公傈扁 (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].EName = "U2A Wireless Set (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].SetFrameID( 862, 885, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].SetDropFrameID( 862 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].HName = "乔狼 傈辑 (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].EName = "Blood Carrier Bat (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].SetFrameID( 863, 886, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].SetDropFrameID( 863 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].HName = "舅技捞单胶狼 唱沟蕾 (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].EName = "Alseides Leef (SMS) 12";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].SetFrameID( 864, 887, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].SetDropFrameID( 864 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].HName = "U2A 公傈扁 (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].EName = "U2A Wireless Set (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].SetFrameID( 862, 885, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].SetDropFrameID( 862 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].HName = "乔狼 傈辑 (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].EName = "Blood Carrier Bat (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].SetFrameID( 863, 886, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].SetDropFrameID( 863 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][4].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].HName = "舅技捞单胶狼 唱沟蕾 (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].EName = "Alseides Leef (SMS) 25";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].SetFrameID( 864, 887, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].SetDropFrameID( 864 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][5].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].HName = "U2A 公傈扁 (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].EName = "U2A Wireless Set (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].SetFrameID( 862, 885, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].SetDropFrameID( 862 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][6].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].HName = "乔狼 傈辑 (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].EName = "Blood Carrier Bat (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].SetFrameID( 863, 886, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].SetDropFrameID( 863 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][7].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].HName = "舅技捞单胶狼 唱沟蕾 (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].EName = "Alseides Leef (SMS) 60";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUND_ITEM_MOVE_BOMB, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].SetFrameID( 864, 887, 0 );	
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].SetDropFrameID( 864 );
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SMS_ITEM][8].Price	= 0;

	//------------------------------------------------------------------
	//	CORE ZAP Item
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_CORE_ZAP, 4);

	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].HName = "龙靛饭 内绢漓 喉发";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].EName = "Gilles de Rais Core Zap Black";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].SetFrameID( 870, 893, 0 );	
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].SetDropFrameID( 870 );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].HName = "龙靛饭 内绢漓 饭靛";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].EName = "Gilles de Rais Core Zap Red";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].SetFrameID( 872, 895, 0 );	
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].SetDropFrameID( 872 );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].HName = "龙靛饭 内绢漓 喉风";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].EName = "Gilles de Rais Core Zap Blue";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].SetFrameID( 869, 892, 0 );	
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].SetDropFrameID( 869 );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].HName = "龙靛饭 内绢漓 弊赴";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].EName = "Gilles de Rais Core Zap Green";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].SetSoundID( SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUND_ITEM_MOVE_RING, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].SetFrameID( 871, 894, 0 );	
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].SetDropFrameID( 871 );
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CORE_ZAP][3].Price	= 0;


	//------------------------------------------------------------------
	//	ITEM_CLASS_GQUEST_ITEM
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_GQUEST_ITEM, 27);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].HName = "宏府妮扼墨胶狼 祈瘤";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].EName = "Bricolakas's Letter";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].SetFrameID( 108, 70, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].SetDropFrameID( 108 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].HName = "扼福官狼 祈瘤";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].EName = "Larva's Letter";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].SetFrameID( 108, 70, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].SetDropFrameID( 108 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].HName = "焊鞭钎";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].EName = "Supply Ticket";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].SetSoundID(SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL,SOUND_ITEM_MOVE_SCROLL);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].SetFrameID(  118, 39, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].SetDropFrameID( 118 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].HName = "单捞磐 叼胶农";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].EName = "Data Disk";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].SetFrameID( 110, 66, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].SetDropFrameID( 110  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].HName = "笼青牢狼 趋籍 1窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].EName = "Bloody Stone 1";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].SetFrameID( 876, 899, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].SetDropFrameID( 876  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][4].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].HName = "笼青牢狼 趋籍 2窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].EName = "Bloody Stone 2";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].SetFrameID( 877, 900, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].SetDropFrameID( 877  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][5].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].HName = "笼青牢狼 趋籍 3窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].EName = "Bloody Stone 3";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].SetFrameID( 878, 901, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].SetDropFrameID( 878  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][6].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].HName = "背包 荐酶 1窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].EName = "Professor License 1";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].SetFrameID( 873, 896, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].SetDropFrameID( 873  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][7].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].HName = "背包 荐酶 2窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].EName = "Professor License 2";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].SetFrameID( 874, 897, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].SetDropFrameID( 874  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][8].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].HName = "背包 荐酶 3窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].EName = "Professor License 3";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].SetFrameID( 875, 898, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].SetDropFrameID( 875  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][9].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].HName = "牢档狼 朝俺 1窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].EName = "Guide Wing 1";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].SetFrameID( 879, 902, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].SetDropFrameID( 879  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][10].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].HName = "牢档狼 朝俺 2窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].EName = "Guide Wing 2";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].SetFrameID( 880, 903, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].SetDropFrameID( 880  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][11].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].HName = "牢档狼 朝俺 3窜拌";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].EName = "Guide Wing 3";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].SetFrameID( 881, 904, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].SetDropFrameID( 881  );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][12].Price	= 0;

	// 2004, 9, 13, sobeit add start - 惑磊..肚-_-
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].HName = "弧埃祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].EName = "Red Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].Description = "饭骇 5捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].SetFrameID( 833, 856, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].SetDropFrameID( 833 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][13].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].HName = "林炔祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].EName = "Orange Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].Description = "饭骇 10捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].SetFrameID( 899, 922, 0);	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].SetDropFrameID( 899 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][14].Price	= 5000;
	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].HName = "畴鄂祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].EName = "Yellow Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].Description = "饭骇 15捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].SetFrameID( 836, 859, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].SetDropFrameID( 836 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][15].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].HName = "檬废祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].EName = "Green Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].Description = "饭骇 20捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].SetFrameID( 835, 858, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].SetDropFrameID( 835 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][16].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].HName = "颇鄂祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].EName = "Blue Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].Description = "饭骇 25捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].SetFrameID( 834, 857, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].SetDropFrameID( 834 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][17].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].HName = "巢祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].EName = "Deep Blue Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].Description = "饭骇 30捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].SetFrameID( 897, 920, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].SetDropFrameID( 897 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][18].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].HName = "焊扼祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].EName = "Violet Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].Description = "饭骇 35捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].SetFrameID( 898, 921, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].SetDropFrameID( 898 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][19].Price	= 5000;

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].HName = "八篮祸 急拱惑磊";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].EName = "Black Present Box";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].Description = "饭骇 40捞惑父 凯 荐 乐嚼聪促.";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].SetSoundID( SOUND_XMAS_GIFTBOX, SOUND_XMAS_GIFTBOX, SOUNDID_NULL, SOUND_XMAS_GIFTBOX );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].SetFrameID( 896, 919, 0 );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].SetDropFrameID( 896 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][20].Price	= 5000;
	// 2004, 9, 13, sobeit add end - 惑磊..肚-_-

	// 2005, 1, 14, sobeit add start

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].HName = "浇饭捞绢狼 必厚巩辑";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].EName = "Top Secret Letter from EVE";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].SetFrameID( 976, 1010, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].SetDropFrameID( 976 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][21].SetDescriptionFrameID(952);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].HName = "轨颇捞绢狼 雀脚巩辑";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].EName = "Reply from Vampire";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].SetFrameID( 975, 1009, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].SetDropFrameID( 975 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][22].SetDescriptionFrameID(951);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].HName = "轨颇捞绢狼 必厚巩辑'";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].EName = "Top Secret Letter from Vampire";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].SetFrameID( 975, 1009, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].SetDropFrameID( 975 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][23].SetDescriptionFrameID(951);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].HName = "浇饭捞绢狼 雀脚巩辑";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].EName = "Reply from EVE";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].SetFrameID( 976, 1010, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].SetDropFrameID( 976 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][24].SetDescriptionFrameID(952);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].HName = "扼厚聪酒狼 刘钎";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].EName = "Labinia\'s Voucher";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].SetFrameID( 974, 1008, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].SetDropFrameID( 974 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][25].SetDescriptionFrameID(950);

	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].HName = "扼福官狼 傈哎";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].EName = "Larba\'s Letter";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].Description = "";
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].SetFrameID( 973, 1007, 0  );	
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].SetDropFrameID( 973 );
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].Price	= 0;
	m_pTypeInfo[ITEM_CLASS_GQUEST_ITEM][26].SetDescriptionFrameID(949);
	// 2005, 1, 14, sobeit add end


	//------------------------------------------------------------------
	//	ITEM_CLASS_TRAP_ITEM
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_TRAP_ITEM, 10);

	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].HName = "罚靛 付牢";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].EName = "Land Mine";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].SetFrameID(893, 916, 0);			
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].SetDropFrameID( 893 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].SetGrid(2, 2);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][0].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].HName = "敲饭廉 胶抛捞记";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].EName = "Pleasure Station";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].SetFrameID(894, 917, 0);			
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].SetDropFrameID( 884 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][1].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].HName = "农饭捞葛绢";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].EName = "Claymore Trap";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].SetFrameID(892, 915, 0);		
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].SetDropFrameID( 892 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].SetGrid(1, 2);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][2].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].HName = "坷配 磐房";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].EName = "Auto Turret";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].SetFrameID(891, 914, 0);	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].SetDropFrameID( 891 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].SetGrid(2, 3);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][3].Price	= 0;

	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].HName = "靛烦烹";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].EName = "Drum";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].SetFrameID(885, 908, 0);	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].SetDropFrameID( 885 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][4].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].HName = "伙阿措 A";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].EName = "Tripod A";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].SetFrameID(886, 909, 0);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].SetDropFrameID( 886 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][5].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].HName = "伙阿措 B";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].EName = "Tripod B";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].SetFrameID(887, 910, 0);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].SetDropFrameID( 887 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][6].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].HName = "伙阿措 C";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].EName = "Tripod C";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].SetFrameID(888, 911, 0);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].SetDropFrameID( 888 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][7].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].HName = "厘局拱 A";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].EName = "Obstacle A";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].SetFrameID(889, 912, 0);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].SetDropFrameID( 889 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][8].Price	= 0;
	
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].HName = "厘局拱 B";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].EName = "Obstacle B";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].Description = "";
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].SetFrameID(890, 913, 0);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].SetDropFrameID( 890 );
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_TRAP_ITEM][9].Price	= 0;
	

	//------------------------------------------------------------------
	//	ITEM_CLASS_BLOOD_BIBLE_SIGN
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_BLOOD_BIBLE_SIGN, 12);

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].HName = "酒福皋啊";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].EName = "Armega";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].SetFrameID( 419, 963, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].SetDropFrameID(419 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][0].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].HName = "固龋饭";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].EName = "Mihole";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].SetFrameID( 420, 964, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].SetDropFrameID(420 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][1].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].HName = "虐肺";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].EName = "Kiro";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].SetFrameID( 421, 965, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].SetDropFrameID(421 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][2].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].HName = "酒捞聪";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].EName = "Ini";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].SetFrameID( 416, 966, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].SetDropFrameID(416 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][3].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].HName = "弊饭绊府";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].EName = "Gregori";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].SetFrameID( 418, 967, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].SetDropFrameID(418 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][4].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].HName = "能磨府酒";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].EName = "Concilia";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].SetFrameID( 417, 968, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].SetDropFrameID(417 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][5].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].HName = "饭瘤坷胶";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].EName = "Legios";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].SetFrameID( 410, 969, 0);	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].SetDropFrameID(410 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][6].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].HName = "鳃副";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].EName = "Hillel";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].SetFrameID( 412, 970, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].SetDropFrameID(412 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][7].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].HName = "鲤宏";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].EName = "Jave";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].SetFrameID( 411, 971, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].SetDropFrameID(411 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][8].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].HName = "匙付";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].EName = "Nema";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].SetFrameID( 414, 972, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].SetDropFrameID(414 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][9].Price = 0;

	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].HName = "酒肺荤";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].EName = "Arosa";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].SetFrameID( 415, 973, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].SetDropFrameID(415 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][10].Price = 0;
	
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].HName = "瞒胶颇";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].EName = "Chaspa";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].Description = "乔狼 己辑";
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].SetFrameID( 413, 974, 0);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].SetDropFrameID(413 );
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_BLOOD_BIBLE_SIGN][11].Price = 0;


	//------------------------------------------------------------------
	//	ITEM_CLASS_WAR_ITEM
	//------------------------------------------------------------------
	InitClass(ITEM_CLASS_WAR_ITEM, 1);

	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].HName = "靛饭帮狼 传";
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].EName = "Dragon Eyes";
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].SetFrameID( 939, 975, 0);
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].SetDropFrameID(939 );
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_WAR_ITEM][0].Price = 0;
//// by csm  2瞒 傈流 侩 酒捞袍 眠啊 

//------------------------------------------------------------------
//	ITEM_CLASS_CARRYING_RECEIVER
//------------------------------------------------------------------
	InitClass(ITEM_CLASS_CARRYING_RECEIVER, 1);
				
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].HName = "牢器 胶农乏";
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].EName = "Info Scrap";
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].SetFrameID( 954, 990, 0);
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].SetDropFrameID(954 );
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][0].Price = 0;
	
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].HName = "客捞靛 牢拟捞绢矾";
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].EName = "Wide inquirer";
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_CARRYING_RECEIVER][1].Price = 0;

//------------------------------------------------------------------
//	ITEM_CLASS_SHOULDER_ARMOR
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_SHOULDER_ARMOR, 1);
				
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].HName = "配靛 归";
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].EName = "Toad Back";
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].SetFrameID( 950, 986, 0);
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].SetDropFrameID(950 );
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].Price = 0;
	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][0].SetAddonFrameID( AC_SHOULDER, AC_SHOULDER );
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].HName = "浇罚飘 啊靛";
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].EName = "Slant Guard";
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_SHOULDER_ARMOR][1].Price = 0;

//------------------------------------------------------------------
//	ITEM_CLASS_DERMIS
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_DERMIS, 1);
				
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].HName = "绢扼快令 农饭胶飘";
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].EName = "Arouse crest";
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].SetFrameID( 942, 978, 0);
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].SetDropFrameID(942 );
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_DERMIS][0].Price = 0;
	
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].HName = "捞葛呕 农饭胶飘";
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].EName = "Immortal crest";
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_DERMIS][1].Price = 0;

//------------------------------------------------------------------
//	ITEM_CLASS_PERSONA
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_PERSONA, 1);
				
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].HName = "农饭橇萍 海老";
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].EName = "Crafty veil";
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].SetFrameID( 941, 977, 0);
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].SetDropFrameID(941 );
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_PERSONA][0].Price = 0;
	
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].HName = "墨坷平 葛饿";
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].EName = "Chaotic moral";
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_PERSONA][1].Price = 0;

//------------------------------------------------------------------
//	ITEM_CLASS_FASCIA
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_FASCIA, 1);
				
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].HName = "迫付 其捞箕";
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].EName = "Palma fascia";
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].SetFrameID( 965, 1001, 0);
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].SetDropFrameID(965 );
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_FASCIA][0].Price = 0;
	
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].HName = "穿捧胶 其捞箕";
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].EName = "Nutus fascia";
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_FASCIA][1].Price = 0;	


//------------------------------------------------------------------
//	ITEM_CLASS_MITTEN
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_MITTEN, 1);
				
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].HName = "橇扼郡府框 臂肺宏";
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].EName = "Praelium gloves";
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].SetFrameID( 960, 996, 0);
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].SetDropFrameID(960 );
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_MITTEN][0].Price = 0;
	
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].HName = "烙其府框 臂肺宏";
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].EName = "Imperium gloves";
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].Description = "";
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].SetFrameID( 939, 975, 0);
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].SetDropFrameID(939 );
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].SetGrid(1, 1);
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].Weight = 1;
//	m_pTypeInfo[ITEM_CLASS_MITTEN][1].Price = 0;	


//------------------------------------------------------------------
//	ITEM_CLASS_SUB_INVENTORY
//------------------------------------------------------------------	

	InitClass(ITEM_CLASS_SUB_INVENTORY, 1);

	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].HName = "2x4 蒲";
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].EName = "2x4 Pack";
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].Description = "";
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].SetSoundID( SOUND_ITEM_MOVE_SCROLL, SOUND_ITEM_MOVE_SCROLL, SOUNDID_NULL, SOUND_ITEM_MOVE_SCROLL );
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].SetFrameID( 979,1013, 0);
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].SetDropFrameID(979 );
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].SetGrid(1, 1);
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].Weight = 1;
	m_pTypeInfo[ITEM_CLASS_SUB_INVENTORY][0].Price = 0;

	
	//
	// [ 付瘤阜 弊覆 ]
	//

	// 秦榜 措眉侩 焊籍			(271, 285) 2002.5.25
	// 岿靛呐 捞亥飘侩 绵备傍	(272, 286) 

	// 2002.6.7 己拱甸
	// 芬羔狼 绕厘				(274, 288)
	// 己狼						(275, 289)
	// 贸赤狼乔					(273, 287)
	// 开绞磊啊					(276, 290)

	// 货狼惑 酒捞袍 惑狼+窍狼 4国究*2 = 16 + 轨颇渴 6 + 秋杆 4 = 26
	// 巢磊惑狼					(280, 294)
	// 巢磊窍狼					(284, 298)
	// 咯磊惑狼					(288, 302)
	// 咯磊窍狼					(292, 306)
	// 巢磊轨颇内飘				(295, 309)
	// 咯磊轨颇内飘				(298, 312)
	// 秋杆						(302, 316)
	// 墨坷胶 弊府靛 赣府		(303, 317)
	// 轨颇捞绢 捣				(304, 318)

	// 轨颇捞绢 蓖吧捞			(314, 328)
	// 喉风 靛酚				(315, 329);

	// 轨颇捞绢 呈努			(325, 339)
	// 轨颇捞绢 农肺快			(335, 349)
	// 轨颇捞绢 酒鼓房			(345, 359)

	// 蜡聪农 酒捞袍			(372, 386)

	// 2002.9.11
	// 饭绢付胶磐 涅胶飘 酒捞袍	(376, 390)
	// 价祈6俺					(382, 396)

	// 2002.9.25
	// 阁胶磐 3辆 赣府			(385, 399)

	// 2002.12.9
	// 气磷 3俺					(388, 402)
	// 农府胶付胶飘府 13俺		(401, 415)
	// 轨颇渴 滴俺				(403, 417)

	// 2003.1.27
	// 侗惫	6辆					(409, 423)

	// 2003.1.29
	// 乔狼 己辑 12辆			(421, 435)
	// 己荐 4辆					(425, 439)

	// 2003.2.12
	// 己惑隆拱 4辆				(429, 443)

	// 轨颇 3窜 渴 2俺			(431, 445)

	// 目敲傅2 眠啊 酒捞袍 钦30	(461, 475)
	// 轨颇 巢磊 2窜渴			(462, 476)

	// 浇饭 酒捞袍 20辆			(482, 496)
	// 轨颇 酒捞袍 8辆			(490, 504)

	// 绊措巩清					(491, 505)
	// 绊措 巩清 炼阿 12俺		(503, 517)
	// 沥飞籍 拱 5俺			(508, 522)
	// 沥飞籍 阂 5俺			(513, 527)
	// 瘤档						(514, 528)
	// 捞巴历巴					(596, 610)
	
	// 酒快胶磐令 奇带飘,辑努复,鞠胶龟靛 (626, 640);
	// 酒快胶磐令 捞巴历巴 53	(679, 693)
	// 酒快胶磐令 联			(680, 694)
	// 酒快胶磐令 捣			(681, 695)
	// 吝埃俊 刚啊 儡多			(791, 814)
	// 脐 包访 酒捞袍			(801, 824)
	// 炕肺快 靛酚, 菩虐瘤俺格吧捞, 仟措7老侩 (804, 827)
	// 酒快胶磐令 11,12窜 + 蜡聪农 (828, 851)
	// 汲朝 捞亥飘 酒捞袍		(837, 860)
	// 齿付喉墨靛				(838, 861)
	// 2瞒脐 酒捞袍				(853, 876)
	// 橇府固决 背券鼻2辆		(855, 878)
	// 怕必扁1辆				(856, 879)
	// 脐 函脚 酒捞袍			(857, 880)
	// 匙蕾 农肺官				(858, 881)
	// 酒快胶磐令 傍己傈 包访 2辆 (860, 883)
	// 龙靛饭 酒捞袍(竿牢狼乔府1辆) (861, 884)
	// sms 面傈 酒捞袍 3辆		 (864, 887)
	// 匙捞怪 奇酒捞袍 2辆		 (866, 889)
	// 龙靛饭 奇畔飘殿 2辆 		 (868, 891)
	// 龙靛饭 内绢漓 4辆 		 (872, 895)
	// 涅胶飘 酒捞袍 9辆		 (881, 904);	
	// 颊狼 岿拌包				 (882, 905);	
	// 畴鄂 荤帕, 踌祸荤帕		 (884, 907)
	// 傍己傈 飘乏,厘局拱 11辆	 (895, 918) 靛烦烹, 伙阿措a, 伙阿措b, 伙阿措c, 厘局拱a, 厘局拱b, 坷配磐房, 农饭捞葛绢, 罚靛付牢,敲饭廉胶抛捞, 阁胶磐杭
	//澜 捞亥飘惑磊 4辆, 价祈3辆   (902, 925) 八沥祸急拱惑磊,巢祸急拱惑磊,焊扼祸急拱惑磊,林炔祸急拱惑磊,曹价祈,睛价祈,闰价祈
	// 130 饭骇 浇饭捞捞 酒捞袍 眠啊      
	// 130 饭骇 轨颇捞绢 酒捞袍 眠啊       
	// 130 饭骇 酒快胶磐令 酒捞袍 眠啊       
	// 喉矾靛官捞喉荤牢 12辆 眠啊 (939, 975) // 牢亥 捞固瘤父 眠啊 沁澜
	// 靛贰帮 酒捞(940,976)
	// 荐瓷 措冠 扁盔酒捞袍(941,977)
	// 2瞒 傈流 Gravis Cutter (941,978)

	// d- 配付龋农鳖瘤 969, 1003
	// 葛贰矫拌 3辆( 972,1006) 鳖瘤

	// 铰流 涅胶飘 包访 酒捞袍 5辆( 977,1011) 鳖瘤

	// 汗炼府( 978,1012) 鳖瘤
	// 付南 眠啊 酒捞袍 6辆( 钢萍蒲,  炕肺快 胶沛, 坷坷扼 胶沛 4辆(焊烹,踌祸,畴尔,焊扼))- ( 984,1018) 鳖瘤

// add by Coffee 2007-3-5  增加物品中文名称
		m_pTypeInfo[0][0].HName="魔蝎";
		m_pTypeInfo[0][1].HName="鹰眼";
		m_pTypeInfo[0][2].HName="凤凰";
		m_pTypeInfo[0][3].HName="雷鸟";
		m_pTypeInfo[0][4].HName="天使";
		m_pTypeInfo[0][5].HName="哈雷机车";
		m_pTypeInfo[0][6].HName="B2-野兽";
		m_pTypeInfo[0][7].HName="D-战斧";
		m_pTypeInfo[1][0].HName="微型血瓶";
		m_pTypeInfo[1][1].HName="小型血瓶";
		m_pTypeInfo[1][2].HName="中型血瓶";
		m_pTypeInfo[1][3].HName="巨型血瓶";
		m_pTypeInfo[1][4].HName="满血瓶";
		m_pTypeInfo[1][5].HName="微型魔法瓶";
		m_pTypeInfo[1][6].HName="小型魔法瓶";
		m_pTypeInfo[1][7].HName="中型魔法瓶";
		m_pTypeInfo[1][8].HName="巨型魔法瓶";
		m_pTypeInfo[1][9].HName="满魔法瓶";
		m_pTypeInfo[1][10].HName="神丹";
		m_pTypeInfo[1][11].HName="白色祝福";
		m_pTypeInfo[1][12].HName="恢复药剂瓶";
		m_pTypeInfo[1][13].HName="魔法药剂瓶";
		m_pTypeInfo[1][14].HName="小剂量圣水";
		m_pTypeInfo[1][15].HName="中剂量圣水";
		m_pTypeInfo[1][16].HName="大剂量圣水";
		m_pTypeInfo[1][17].HName="全剂量圣水";
		m_pTypeInfo[2][0].HName="小圣水瓶";
		m_pTypeInfo[2][1].HName="中圣水瓶";
		m_pTypeInfo[2][2].HName="大圣水瓶";
		m_pTypeInfo[2][3].HName="小剂量药瓶";
		m_pTypeInfo[2][4].HName="中剂量药瓶";
		m_pTypeInfo[2][5].HName="大剂量药瓶";
		m_pTypeInfo[2][6].HName="全剂量药瓶";
		m_pTypeInfo[3][0].HName="小瓶圣水";
		m_pTypeInfo[3][1].HName="中瓶圣水";
		m_pTypeInfo[3][2].HName="大瓶圣水";
		m_pTypeInfo[4][0].HName="SG 子弹-8";
		m_pTypeInfo[4][1].HName="SG 子弹-16";
		m_pTypeInfo[4][2].HName="AR子弹-20";
		m_pTypeInfo[4][3].HName="AR子弹-40";
		m_pTypeInfo[4][4].HName="SMG子弹-40";
		m_pTypeInfo[4][5].HName="SMG子弹-80";
		m_pTypeInfo[4][6].HName="SR子弹-10";
		m_pTypeInfo[4][7].HName="SR子弹-20";
		m_pTypeInfo[4][8].HName="银制 SG 子弹-8";
		m_pTypeInfo[4][9].HName="银制 SG 子弹-16";
		m_pTypeInfo[4][10].HName="银制 AR 子弹-20";
		m_pTypeInfo[4][11].HName="银制 AR 子弹-40";
		m_pTypeInfo[4][12].HName="SMG专用银子弹-40";
		m_pTypeInfo[4][13].HName="SMG专用银子弹-80";
		m_pTypeInfo[4][14].HName="银制 SR 子弹-10";
		m_pTypeInfo[4][15].HName="银制 SR 子弹-20";
		m_pTypeInfo[4][16].HName="SG 子弹--32";
		m_pTypeInfo[4][17].HName="AR 子弹-80";
		m_pTypeInfo[4][18].HName="SMG 专用子弹-160";
		m_pTypeInfo[4][19].HName="SR 子弹-40";
		m_pTypeInfo[4][20].HName="银制 SG 子弹--32";
		m_pTypeInfo[4][21].HName="银制 AR 子弹-80";
		m_pTypeInfo[4][22].HName="银制 SMG 专用子弹-160";
		m_pTypeInfo[4][23].HName="银制 SR 子弹-40";
		m_pTypeInfo[5][0].HName="裂片炸药";
		m_pTypeInfo[5][1].HName="散针炸药";
		m_pTypeInfo[5][2].HName="散珠炸药";
		m_pTypeInfo[5][3].HName="眩晕炸药";
		m_pTypeInfo[5][4].HName="神弩炸药";
		m_pTypeInfo[5][5].HName="损踝炸药";
		m_pTypeInfo[5][6].HName="普玛斯炸药";
		m_pTypeInfo[5][7].HName="AP-C1 炸药";
		m_pTypeInfo[5][8].HName="钻石炸药";
		m_pTypeInfo[5][9].HName="快速-EX炸药";
		m_pTypeInfo[6][0].HName="工具箱";
		m_pTypeInfo[6][1].HName="语言翻译器";
		m_pTypeInfo[7][0].HName="铜钥匙";
		m_pTypeInfo[7][1].HName="金钥匙";
		m_pTypeInfo[7][2].HName="磁体钥匙";
		m_pTypeInfo[7][3].HName="特殊钥匙";
		m_pTypeInfo[7][4].HName="绿色哈雷机车钥匙";
		m_pTypeInfo[7][5].HName="蓝色哈雷机车钥匙";
		m_pTypeInfo[7][6].HName="红色哈雷机车钥匙";
		m_pTypeInfo[7][7].HName="黄色哈雷机车钥匙";
		m_pTypeInfo[7][8].HName="橙色摩托车钥匙";
		m_pTypeInfo[7][9].HName="天蓝色哈雷机车钥匙";
		m_pTypeInfo[7][10].HName="淡紫色哈雷机车钥匙";
		m_pTypeInfo[7][11].HName="黑色哈雷机车钥匙";
		m_pTypeInfo[7][12].HName="B2-野兽钥匙";
		m_pTypeInfo[7][13].HName="D-战斧钥匙";
		m_pTypeInfo[8][0].HName="青铜戒";
		m_pTypeInfo[8][1].HName="黑光戒";
		m_pTypeInfo[8][2].HName="牧士戒";
		m_pTypeInfo[8][3].HName="金戒";
		m_pTypeInfo[8][4].HName="亚金戒";
		m_pTypeInfo[8][5].HName="幸运之戒";
		m_pTypeInfo[8][6].HName="罗撒里傲戒";
		m_pTypeInfo[8][7].HName="钻石戒";
		m_pTypeInfo[8][8].HName="白金戒";
		m_pTypeInfo[8][9].HName="圣十字戒";
		m_pTypeInfo[8][10].HName="天神戒";
		m_pTypeInfo[8][11].HName="尊从之戒";
		m_pTypeInfo[8][12].HName="赤红之星";
		m_pTypeInfo[8][13].HName="弗拉西奥";
		m_pTypeInfo[9][0].HName="铁手镯";
		m_pTypeInfo[9][1].HName="铜手镯";
		m_pTypeInfo[9][2].HName="合金手镯";
		m_pTypeInfo[9][3].HName="金刚手镯";
		m_pTypeInfo[9][4].HName="亚金手镯";
		m_pTypeInfo[9][5].HName="女神手镯";
		m_pTypeInfo[9][6].HName="神圣手镯";
		m_pTypeInfo[9][7].HName="奴隶手镯";
		m_pTypeInfo[9][8].HName="圣十字手镯";
		m_pTypeInfo[9][9].HName="圣灵手镯";
		m_pTypeInfo[9][10].HName="天鹰手镯";
		m_pTypeInfo[9][11].HName="宝石手镯";
		m_pTypeInfo[9][12].HName="达佩尔手镯";
		m_pTypeInfo[9][13].HName="科什尼克手镯";
		m_pTypeInfo[10][0].HName="十字架护身符";
		m_pTypeInfo[10][1].HName="祖母绿护身符";
		m_pTypeInfo[10][2].HName="白金护身符";
		m_pTypeInfo[10][3].HName="亚金护身符";
		m_pTypeInfo[10][4].HName="新月项链";
		m_pTypeInfo[10][5].HName="蓝光";
		m_pTypeInfo[10][6].HName="圣十字架护身符";
		m_pTypeInfo[10][7].HName="轮回之星";
		m_pTypeInfo[10][8].HName="圣血护身符";
		m_pTypeInfo[10][9].HName="圣环护身符";
		m_pTypeInfo[10][10].HName="圣恩护身符";
		m_pTypeInfo[10][11].HName="复活之链";
		m_pTypeInfo[10][12].HName="风暴护身符";
		m_pTypeInfo[10][13].HName="拉马苏项链";
		m_pTypeInfo[11][0].HName="防弹衣M";
		m_pTypeInfo[11][1].HName="防弹衣W";
		m_pTypeInfo[11][2].HName="防弹甲M";
		m_pTypeInfo[11][3].HName="防弹甲W";
		m_pTypeInfo[11][4].HName="武士服M";
		m_pTypeInfo[11][5].HName="武士服W";
		m_pTypeInfo[11][6].HName="钛金战衣M";
		m_pTypeInfo[11][7].HName="钛金战衣W";
		m_pTypeInfo[11][8].HName="武士甲M";
		m_pTypeInfo[11][9].HName="武士甲W";
		m_pTypeInfo[11][10].HName="铠甲M";
		m_pTypeInfo[11][11].HName="铠甲W";
		m_pTypeInfo[11][12].HName="神圣铠甲M";
		m_pTypeInfo[11][13].HName="神圣铠甲W";
		m_pTypeInfo[11][14].HName="战神甲W";
		m_pTypeInfo[11][15].HName="战神甲W";
		m_pTypeInfo[11][16].HName="龙神甲";
		m_pTypeInfo[11][17].HName="龙神甲";
		m_pTypeInfo[11][18].HName="装甲外套";
		m_pTypeInfo[11][19].HName="装甲外套";
		m_pTypeInfo[11][20].HName="重装甲外套";
		m_pTypeInfo[11][21].HName="重装甲外套";
		m_pTypeInfo[11][22].HName="比武盔甲";
		m_pTypeInfo[11][23].HName="比武盔甲";
		m_pTypeInfo[11][24].HName="网眼外皮";
		m_pTypeInfo[11][25].HName="网眼外皮";
		m_pTypeInfo[12][0].HName="防弹裤";
		m_pTypeInfo[12][1].HName="防弹裤";
		m_pTypeInfo[12][2].HName="防弹甲裤";
		m_pTypeInfo[12][3].HName="防弹甲裤";
		m_pTypeInfo[12][4].HName="武士裤";
		m_pTypeInfo[12][5].HName="武士裤";
		m_pTypeInfo[12][6].HName="锁子甲裤";
		m_pTypeInfo[12][7].HName="锁子甲裤";
		m_pTypeInfo[12][8].HName="武士甲裤";
		m_pTypeInfo[12][9].HName="武士甲裤";
		m_pTypeInfo[12][10].HName="铠甲裤";
		m_pTypeInfo[12][11].HName="铠甲裤";
		m_pTypeInfo[12][12].HName="神圣铠甲裤";
		m_pTypeInfo[12][13].HName="神圣铠甲裤";
		m_pTypeInfo[12][14].HName="战神甲裤";
		m_pTypeInfo[12][15].HName="战神甲裤";
		m_pTypeInfo[12][16].HName="龙神甲裤";
		m_pTypeInfo[12][17].HName="龙神甲裤";
		m_pTypeInfo[12][18].HName="装甲护腿";
		m_pTypeInfo[12][19].HName="装甲护腿";
		m_pTypeInfo[12][20].HName="重装甲护腿";
		m_pTypeInfo[12][21].HName="重装甲护腿";
		m_pTypeInfo[12][22].HName="比武装甲护腿";
		m_pTypeInfo[12][23].HName="比武装甲护腿";
		m_pTypeInfo[12][24].HName="紧身防弹裤";
		m_pTypeInfo[12][25].HName="紧身防弹裤";
		m_pTypeInfo[13][0].HName="轻靴";
		m_pTypeInfo[13][1].HName="皮靴";
		m_pTypeInfo[13][2].HName="钢靴";
		m_pTypeInfo[13][3].HName="武士靴";
		m_pTypeInfo[13][4].HName="搏击靴";
		m_pTypeInfo[13][5].HName="树脂靴";
		m_pTypeInfo[13][6].HName="战神靴";
		m_pTypeInfo[13][7].HName="墨丘利神靴";
		m_pTypeInfo[13][8].HName="野战靴";
		m_pTypeInfo[13][9].HName="萨巴通靴";
		m_pTypeInfo[13][10].HName="钢甲靴";
		m_pTypeInfo[14][0].HName="轻剑";
		m_pTypeInfo[14][1].HName="战剑";
		m_pTypeInfo[14][2].HName="阔剑";
		m_pTypeInfo[14][3].HName="烈剑";
		m_pTypeInfo[14][4].HName="锐剑";
		m_pTypeInfo[14][5].HName="哥特剑";
		m_pTypeInfo[14][6].HName="十字军剑";
		m_pTypeInfo[14][7].HName="巨剑";
		m_pTypeInfo[14][8].HName="苏格兰剑";
		m_pTypeInfo[14][9].HName="女神剑";
		m_pTypeInfo[14][10].HName="末日之刃";
		m_pTypeInfo[14][11].HName="烈蜥之剑";
		m_pTypeInfo[14][12].HName="罗马短剑";
		m_pTypeInfo[14][13].HName="韩瑞德之剑";
		m_pTypeInfo[14][14].HName="针刃";
		m_pTypeInfo[15][0].HName="弯刀";
		m_pTypeInfo[15][1].HName="通灵刀";
		m_pTypeInfo[15][2].HName="大砍刀";
		m_pTypeInfo[15][3].HName="板门刀";
		m_pTypeInfo[15][4].HName="妖月刀";
		m_pTypeInfo[15][5].HName="聚灵刀";
		m_pTypeInfo[15][6].HName="大环刀";
		m_pTypeInfo[15][7].HName="波斯弯刀";
		m_pTypeInfo[15][8].HName="战神刀";
		m_pTypeInfo[15][9].HName="武神刀";
		m_pTypeInfo[15][10].HName="百战刀";
		m_pTypeInfo[15][11].HName="科拉琴";
		m_pTypeInfo[15][12].HName="龙魄";
		m_pTypeInfo[15][13].HName="曲折圣剑";
		m_pTypeInfo[15][14].HName="双手卡博";
		m_pTypeInfo[16][0].HName="圆盾";
		m_pTypeInfo[16][1].HName="青铜盾";
		m_pTypeInfo[16][2].HName="旋风盾";
		m_pTypeInfo[16][3].HName="长蛇盾";
		m_pTypeInfo[16][4].HName="剑神盾";
		m_pTypeInfo[16][5].HName="龙神盾";
		m_pTypeInfo[16][6].HName="远征盾";
		m_pTypeInfo[16][7].HName="独角神兽盾";
		m_pTypeInfo[16][8].HName="天狮盾";
		m_pTypeInfo[16][9].HName="映日盾";
		m_pTypeInfo[16][10].HName="奥巴龙盾";
		m_pTypeInfo[16][11].HName="霍普龙盾";
		m_pTypeInfo[16][12].HName="凸面盾";
		m_pTypeInfo[16][13].HName="马刺盾";
		m_pTypeInfo[17][0].HName="银十字架";
		m_pTypeInfo[17][1].HName="拉丁十字架";
		m_pTypeInfo[17][2].HName="受难十字架";
		m_pTypeInfo[17][3].HName="格利士旦十字架";
		m_pTypeInfo[17][4].HName="主教十字架";
		m_pTypeInfo[17][5].HName="荣耀十字架";
		m_pTypeInfo[17][6].HName="高卢十字架";
		m_pTypeInfo[17][7].HName="女神十字架";
		m_pTypeInfo[17][8].HName="约旦十字架";
		m_pTypeInfo[17][9].HName="教皇十字架";
		m_pTypeInfo[17][10].HName="洛林十字架";
		m_pTypeInfo[17][11].HName="圣女凯瑟琳娜十字架";
		m_pTypeInfo[17][12].HName="信仰者十字架";
		m_pTypeInfo[18][0].HName="黑手套";
		m_pTypeInfo[18][1].HName="青铜手套";
		m_pTypeInfo[18][2].HName="牧师手套";
		m_pTypeInfo[18][3].HName="骑士手套";
		m_pTypeInfo[18][4].HName="古罗马手套";
		m_pTypeInfo[18][5].HName="钢制拳套";
		m_pTypeInfo[18][6].HName="终级拳套";
		m_pTypeInfo[18][7].HName="雷神拳套";
		m_pTypeInfo[18][8].HName="战神拳套";
		m_pTypeInfo[18][9].HName="R-精力手套";
		m_pTypeInfo[18][10].HName="重装甲手套";
		m_pTypeInfo[18][11].HName="亚蒙拳套";
		m_pTypeInfo[19][0].HName="防护帽";
		m_pTypeInfo[19][1].HName="铁盔";
		m_pTypeInfo[19][2].HName="侦察头盔";
		m_pTypeInfo[19][3].HName="导航盔";
		m_pTypeInfo[19][4].HName="武士盔";
		m_pTypeInfo[19][5].HName="冲锋盔";
		m_pTypeInfo[19][6].HName="超级冲锋盔";
		m_pTypeInfo[19][7].HName="红外扫描头盔";
		m_pTypeInfo[19][8].HName="战神盔";
		m_pTypeInfo[19][9].HName="公爵盔";
		m_pTypeInfo[19][10].HName="聚合物头盔";
		m_pTypeInfo[19][11].HName="重装甲头盔";
		m_pTypeInfo[19][12].HName="笼盔";
		m_pTypeInfo[19][13].HName="护目镜";
		m_pTypeInfo[20][0].HName="MT-10流星";
		m_pTypeInfo[20][1].HName="MT-21巨星";
		m_pTypeInfo[20][2].HName="MTB-11彗尾";
		m_pTypeInfo[20][3].HName="AM-99猛犸99";
		m_pTypeInfo[20][4].HName="TND-5铁拳";
		m_pTypeInfo[20][5].HName="SS-10诺顿";
		m_pTypeInfo[20][6].HName="RESS-1魔鬼";
		m_pTypeInfo[20][7].HName="凯斯";
		m_pTypeInfo[20][8].HName="RESS-4a美洲鳄";
		m_pTypeInfo[20][9].HName="S-200风暴";
		m_pTypeInfo[20][10].HName="ST-2A鲨鱼";
		m_pTypeInfo[20][11].HName="Spas-12暴徒";
		m_pTypeInfo[20][12].HName="S-EV11爆裂";
		m_pTypeInfo[20][13].HName="PG-13 Roc狙击步枪";
		m_pTypeInfo[21][0].HName="MD-4";
		m_pTypeInfo[21][1].HName="IS-100 ZIG";
		m_pTypeInfo[21][2].HName="M-INTER";
		m_pTypeInfo[21][3].HName="B-INTER";
		m_pTypeInfo[21][4].HName="VK-45蜻蜓";
		m_pTypeInfo[21][5].HName="MD-9天蝎";
		m_pTypeInfo[21][6].HName="沃鲁";
		m_pTypeInfo[21][7].HName="P2K 轻骑";
		m_pTypeInfo[21][8].HName="MD-Z 泽塔";
		m_pTypeInfo[21][9].HName="MD-000 海怪";
		m_pTypeInfo[21][10].HName="NS-32 水兵";
		m_pTypeInfo[21][11].HName="突击者";
		m_pTypeInfo[21][12].HName="KH-03A1狂热";
		m_pTypeInfo[21][13].HName="MG-90 Shylph";
		m_pTypeInfo[22][0].HName="MK-74 堕落";
		m_pTypeInfo[22][1].HName="P-1A 波澜";
		m_pTypeInfo[22][2].HName="MK-101 GOOSE";
		m_pTypeInfo[22][3].HName="MK-2 G2";
		m_pTypeInfo[22][4].HName="MK-2000 G2000";
		m_pTypeInfo[22][5].HName="P-61S Minerva";
		m_pTypeInfo[22][6].HName="IS-200 Fury";
		m_pTypeInfo[22][7].HName="MK-2002 瓦哈马";
		m_pTypeInfo[22][8].HName="P-38 恶谷";
		m_pTypeInfo[22][9].HName="P-40 El 城堡";
		m_pTypeInfo[22][10].HName="AR-火风";
		m_pTypeInfo[22][11].HName="AR-火虫";
		m_pTypeInfo[22][12].HName="EA-V1天使之护";
		m_pTypeInfo[22][13].HName="LM-700 Kar";
		m_pTypeInfo[22][14].HName="军营";
		m_pTypeInfo[23][0].HName="X-31M米兰";
		m_pTypeInfo[23][1].HName="X-55 V2";
		m_pTypeInfo[23][2].HName="伯伦";
		m_pTypeInfo[23][3].HName="X-45T 战斧";
		m_pTypeInfo[23][4].HName="IS-99 ??";
		m_pTypeInfo[23][5].HName="SR-1 拓斯";
		m_pTypeInfo[23][6].HName="GX90 霍特";
		m_pTypeInfo[23][7].HName="顽童";
		m_pTypeInfo[23][8].HName="哈亚";
		m_pTypeInfo[23][9].HName="IS-101魔特";
		m_pTypeInfo[23][10].HName="BFG-50";
		m_pTypeInfo[23][11].HName="BFG-50致命之针";
		m_pTypeInfo[23][12].HName="T-03梦魇";
		m_pTypeInfo[23][13].HName="V-03 Chmos";
		m_pTypeInfo[23][14].HName="战术狙击枪";
		m_pTypeInfo[24][0].HName="裂片炸弹";
		m_pTypeInfo[24][1].HName="散针炸弹";
		m_pTypeInfo[24][2].HName="散珠炸弹";
		m_pTypeInfo[24][3].HName="眩晕炸弹";
		m_pTypeInfo[24][4].HName="神弩炸弹";
		m_pTypeInfo[25][0].HName="损踝雷";
		m_pTypeInfo[25][1].HName="普玛斯雷";
		m_pTypeInfo[25][2].HName="AP-C1";
		m_pTypeInfo[25][3].HName="钻石雷";
		m_pTypeInfo[25][4].HName="快速-EX雷";
		m_pTypeInfo[26][0].HName="皮腰带";
		m_pTypeInfo[26][1].HName="铁腰带";
		m_pTypeInfo[26][2].HName="铜腰带";
		m_pTypeInfo[26][3].HName="银腰带";
		m_pTypeInfo[26][4].HName="金腰带";
		m_pTypeInfo[26][5].HName="作战腰带";
		m_pTypeInfo[26][6].HName="斜挎式腰带x8";
		m_pTypeInfo[26][7].HName="斜挎式腰带x8";
		m_pTypeInfo[27][0].HName="卷轴";
		m_pTypeInfo[27][1].HName="磁盘";
		m_pTypeInfo[27][2].HName="压缩磁盘";
		m_pTypeInfo[27][3].HName="CD";
		m_pTypeInfo[27][4].HName="竹简";
		m_pTypeInfo[27][5].HName="东洋经";
		m_pTypeInfo[27][6].HName="圣经";
		m_pTypeInfo[27][7].HName="上古卷轴";
		m_pTypeInfo[27][8].HName="文件";
		m_pTypeInfo[28][0].HName="银币";
		m_pTypeInfo[28][1].HName="银币";
		m_pTypeInfo[28][2].HName="银币";
		m_pTypeInfo[29][0].HName="尸体";
		m_pTypeInfo[30][0].HName="铁戒";
		m_pTypeInfo[30][1].HName="银戒";
		m_pTypeInfo[30][2].HName="螺旋戒";
		m_pTypeInfo[30][3].HName="嵌金戒";
		m_pTypeInfo[30][4].HName="落日戒";
		m_pTypeInfo[30][5].HName="蓝色诱惑";
		m_pTypeInfo[30][6].HName="吸血魔戒";
		m_pTypeInfo[30][7].HName="邪恶戒";
		m_pTypeInfo[30][8].HName="骷髅戒";
		m_pTypeInfo[30][9].HName="蛛眼戒";
		m_pTypeInfo[30][10].HName="灭绝之戒";
		m_pTypeInfo[30][11].HName="芬里厄之怒";
		m_pTypeInfo[30][12].HName="蝮蛇之戒";
		m_pTypeInfo[30][13].HName="紫翠玉";
		m_pTypeInfo[31][0].HName="花边手镯";
		m_pTypeInfo[31][1].HName="饰纹手镯";
		m_pTypeInfo[31][2].HName="宝石手镯";
		m_pTypeInfo[31][3].HName="大师手镯";
		m_pTypeInfo[31][4].HName="绿柱石手镯";
		m_pTypeInfo[31][5].HName="蓝宝石手镯";
		m_pTypeInfo[31][6].HName="蓝水晶手镯";
		m_pTypeInfo[31][7].HName="五毒手镯";
		m_pTypeInfo[31][8].HName="海神手镯";
		m_pTypeInfo[31][9].HName="天神手镯";
		m_pTypeInfo[31][10].HName="敏捷手镯";
		m_pTypeInfo[31][11].HName="天青石手镯";
		m_pTypeInfo[31][12].HName="魔女之实";
		m_pTypeInfo[32][0].HName="旋镖项链";
		m_pTypeInfo[32][1].HName="骷髅项链";
		m_pTypeInfo[32][2].HName="犬齿项链";
		m_pTypeInfo[32][3].HName="黑玉项链";
		m_pTypeInfo[32][4].HName="魔幻项链";
		m_pTypeInfo[32][5].HName="恶魔项链";
		m_pTypeInfo[32][6].HName="死亡之星";
		m_pTypeInfo[32][7].HName="神符";
		m_pTypeInfo[32][8].HName="黑十字架项链";
		m_pTypeInfo[32][9].HName="死神召唤";
		m_pTypeInfo[32][10].HName="死之契约";
		m_pTypeInfo[32][11].HName="恶魔之眼";
		m_pTypeInfo[32][12].HName="阿波罗诅咒";
		m_pTypeInfo[32][13].HName="乌扎特项链";
		m_pTypeInfo[33][0].HName="披风";
		m_pTypeInfo[33][1].HName="紧身服";
		m_pTypeInfo[33][2].HName="礼服";
		m_pTypeInfo[33][3].HName="暗黑紧身衣";
		m_pTypeInfo[33][4].HName="天鹅绒礼服";
		m_pTypeInfo[33][5].HName="羽毛披风";
		m_pTypeInfo[33][6].HName="隐身服";
		m_pTypeInfo[33][7].HName="血披风";
		m_pTypeInfo[33][8].HName="落日法衣";
		m_pTypeInfo[33][9].HName="落日披风";
		m_pTypeInfo[33][10].HName="大师法衣";
		m_pTypeInfo[33][11].HName="大师披风";
		m_pTypeInfo[33][12].HName="指挥官服";
		m_pTypeInfo[33][13].HName="指挥官服";
		m_pTypeInfo[33][14].HName="凝血之衣";
		m_pTypeInfo[33][15].HName="凝血之袍";
		m_pTypeInfo[33][16].HName="罗马胸甲";
		m_pTypeInfo[33][17].HName="罗马胸甲";
		m_pTypeInfo[33][18].HName="亚麻外衣";
		m_pTypeInfo[33][19].HName="亚麻外衣";
		m_pTypeInfo[33][20].HName="重披风";
		m_pTypeInfo[33][21].HName="薄皮外套";
		m_pTypeInfo[34][0].HName="骷髅";
		m_pTypeInfo[34][1].HName="肮脏兽骷髅";
		m_pTypeInfo[34][2].HName="血术士骷髅";
		m_pTypeInfo[34][3].HName="异形骷髅";
		m_pTypeInfo[34][4].HName="幽灵骷髅";
		m_pTypeInfo[34][5].HName="僵尸骷髅";
		m_pTypeInfo[34][6].HName="魔鬼骷髅";
		m_pTypeInfo[34][7].HName="队长骷髅";
		m_pTypeInfo[34][8].HName="艾克兽骷髅";
		m_pTypeInfo[34][9].HName="士兵骷髅";
		m_pTypeInfo[34][10].HName="红眼骷髅";
		m_pTypeInfo[34][11].HName="死尸骷髅";
		m_pTypeInfo[34][12].HName="金色骷髅";
		m_pTypeInfo[34][13].HName="紫色骷髅";
		m_pTypeInfo[34][14].HName="黑色骷髅";
		m_pTypeInfo[34][15].HName="水晶骷髅";
		m_pTypeInfo[34][16].HName="绿色骷髅";
		m_pTypeInfo[34][17].HName="破坏者骷髅";
		m_pTypeInfo[34][18].HName="屠杀者骷髅";
		m_pTypeInfo[34][19].HName="铁牙骷髅";
		m_pTypeInfo[34][20].HName="莫地兽骷髅";
		m_pTypeInfo[34][21].HName="食人兽骷髅";
		m_pTypeInfo[34][22].HName="女鬼骷髅";
		m_pTypeInfo[34][23].HName="刀鬼骷髅";
		m_pTypeInfo[34][24].HName="巨型兽骷髅";
		m_pTypeInfo[34][25].HName="傀儡骷髅";
		m_pTypeInfo[34][26].HName="飞影骷髅";
		m_pTypeInfo[34][27].HName="割头者骷髅";
		m_pTypeInfo[34][28].HName="混乱骑士骷髅";
		m_pTypeInfo[34][29].HName="男巫骷髅";
		m_pTypeInfo[34][30].HName="黑魔骷髅";
		m_pTypeInfo[34][31].HName="混乱使者骷髅";
		m_pTypeInfo[34][32].HName="地狱使者骷髅";
		m_pTypeInfo[34][33].HName="暗黑帝王骷髅";
		m_pTypeInfo[34][34].HName="暗黑守卫骷髅";
		m_pTypeInfo[34][35].HName="领主骷髅";
		m_pTypeInfo[34][36].HName="贪婪食者骷髅";
		m_pTypeInfo[34][37].HName="地狱魔王骷髅";
		m_pTypeInfo[34][38].HName="黑雾骷髅";
		m_pTypeInfo[34][39].HName="追讨者之头";
		m_pTypeInfo[34][40].HName="利蒙之头";
		m_pTypeInfo[34][41].HName="变异萨满之头";
		m_pTypeInfo[34][42].HName="黑暗百瑞兹的头骨";
		m_pTypeInfo[34][43].HName="亡灵巫师的骷髅";
		m_pTypeInfo[34][44].HName="岩石怪的骷髅";
		m_pTypeInfo[34][45].HName="巨骨怪的头骨";
		m_pTypeInfo[34][46].HName="石像怪的头骨";
		m_pTypeInfo[34][47].HName="梅杜莎的头骨";
		m_pTypeInfo[34][48].HName="阿撒巴洛德的头骨";
		m_pTypeInfo[34][49].HName="暗夜之狼骷髅";
		m_pTypeInfo[34][50].HName="血腥之狼";
		m_pTypeInfo[34][51].HName="血腥之狼";
		m_pTypeInfo[34][52].HName="变异艾泽头骨";
		m_pTypeInfo[34][53].HName="白骨守护神头骨";
		m_pTypeInfo[34][54].HName="朗姆护卫头骨";
		m_pTypeInfo[34][55].HName="冰冻流氓头骨";
		m_pTypeInfo[34][56].HName="弗列格尔头骨";
		m_pTypeInfo[34][57].HName="及弗雷特尔头骨";
		m_pTypeInfo[34][58].HName="特拉斯拉头骨";
		m_pTypeInfo[34][59].HName="普斯卡头骨";
		m_pTypeInfo[34][60].HName="克拉德?欧门赛克头骨";
		m_pTypeInfo[34][61].HName="诺德?科皮拉头骨";
		m_pTypeInfo[34][62].HName="剃刀头骨";
		m_pTypeInfo[34][63].HName="头骨";
		m_pTypeInfo[34][64].HName="瘸腿头骨";
		m_pTypeInfo[34][65].HName="瘸腿头骨";
		m_pTypeInfo[34][66].HName="罗伊?卡达鲁头骨";
		m_pTypeInfo[34][67].HName="夏米赛头骨";
		m_pTypeInfo[34][68].HName="兰加?特斯塔头骨";
		m_pTypeInfo[34][69].HName="奥伯斯库泽头骨";
		m_pTypeInfo[34][70].HName="司徒曼头骨";
		m_pTypeInfo[34][71].HName="和普特曼头骨";
		m_pTypeInfo[34][72].HName="奥伯斯特头骨";
		m_pTypeInfo[35][0].HName="铁杖";
		m_pTypeInfo[35][1].HName="银杖";
		m_pTypeInfo[35][2].HName="希腊权杖";
		m_pTypeInfo[35][3].HName="教主权杖";
		m_pTypeInfo[35][4].HName="教皇权杖";
		m_pTypeInfo[35][5].HName="卡轮杖";
		m_pTypeInfo[35][6].HName="震天杖";
		m_pTypeInfo[35][7].HName="圣环权杖";
		m_pTypeInfo[35][8].HName="风神杖";
		m_pTypeInfo[35][9].HName="飞雪权杖";
		m_pTypeInfo[35][10].HName="圣杖-制裁";
		m_pTypeInfo[35][11].HName="圣杖-制裁";
		m_pTypeInfo[35][12].HName="显赫阿赛";
		m_pTypeInfo[36][0].HName="血清";
		m_pTypeInfo[36][1].HName="浓缩血清";
		m_pTypeInfo[36][2].HName="高浓缩血清";
		m_pTypeInfo[36][3].HName="超浓缩血清";
		m_pTypeInfo[36][4].HName="仙丹";
		m_pTypeInfo[36][5].HName="黄色祝福";
		m_pTypeInfo[36][6].HName="活力血清";
		m_pTypeInfo[37][0].HName="狼爪";
		m_pTypeInfo[37][1].HName="蝙蝠翅膀";
		m_pTypeInfo[37][2].HName="语言翻译器";
		m_pTypeInfo[38][0].HName="VT-310";
		m_pTypeInfo[38][1].HName="VT-330";
		m_pTypeInfo[38][2].HName="VT-对讲机";
		m_pTypeInfo[39][0].HName="黑暗徽章";
		m_pTypeInfo[39][1].HName="隐匿徽章";
		m_pTypeInfo[39][2].HName="激情徽章";
		m_pTypeInfo[39][3].HName="黑暗徽章";
		m_pTypeInfo[39][4].HName="隐匿徽章";
		m_pTypeInfo[39][5].HName="激情徽章";
		m_pTypeInfo[39][6].HName="黑暗徽章";
		m_pTypeInfo[39][7].HName="隐匿徽章";
		m_pTypeInfo[39][8].HName="激情徽章";
		m_pTypeInfo[39][9].HName="黑暗徽章";
		m_pTypeInfo[39][10].HName="隐匿徽章";
		m_pTypeInfo[39][11].HName="激情徽章";
		m_pTypeInfo[39][12].HName="黑暗徽章";
		m_pTypeInfo[39][13].HName="隐匿徽章";
		m_pTypeInfo[39][14].HName="激情徽章";
		m_pTypeInfo[39][15].HName="黑暗徽章";
		m_pTypeInfo[39][16].HName="隐匿徽章";
		m_pTypeInfo[39][17].HName="激情徽章";
		m_pTypeInfo[40][0].HName="绿色礼盒";
		m_pTypeInfo[40][1].HName="红色礼盒";
		m_pTypeInfo[40][2].HName="红色礼盒";
		m_pTypeInfo[40][3].HName="蓝色礼物箱";
		m_pTypeInfo[40][4].HName="绿色礼盒";
		m_pTypeInfo[40][5].HName="黄色礼物箱";
		m_pTypeInfo[40][6].HName="黑色礼物箱";
		m_pTypeInfo[40][7].HName="黑色礼物箱";
		m_pTypeInfo[40][8].HName="黑色礼物箱";
		m_pTypeInfo[40][9].HName="黑色礼物箱";
		m_pTypeInfo[40][10].HName="黑色礼物箱";
		m_pTypeInfo[40][11].HName="黑色礼物箱";
		m_pTypeInfo[40][12].HName="黑色礼物箱";
		m_pTypeInfo[40][13].HName="黑色礼物箱";
		m_pTypeInfo[40][14].HName="黑色礼物箱";
		m_pTypeInfo[40][15].HName="黑色礼物箱";
		m_pTypeInfo[40][16].HName="红色礼盒";
		m_pTypeInfo[40][17].HName="蓝色礼盒";
		m_pTypeInfo[40][18].HName="黄色礼盒";
		m_pTypeInfo[40][19].HName="黑色礼物箱";
		m_pTypeInfo[40][20].HName="黑色礼物箱";
		m_pTypeInfo[40][21].HName="黑色礼物箱";
		m_pTypeInfo[40][22].HName="签名笔";
		m_pTypeInfo[40][23].HName="宠物签名笔";
		m_pTypeInfo[40][24].HName="额外命名笔";
		m_pTypeInfo[40][25].HName="稀有命名笔";
		m_pTypeInfo[40][26].HName="推针";
		m_pTypeInfo[41][0].HName="黑星";
		m_pTypeInfo[41][1].HName="红星";
		m_pTypeInfo[41][2].HName="蓝星";
		m_pTypeInfo[41][3].HName="绿星";
		m_pTypeInfo[41][4].HName="藏星";
		m_pTypeInfo[41][5].HName="白星";
		m_pTypeInfo[41][6].HName="蓝宝石";
		m_pTypeInfo[41][7].HName="蓝色水滴";
		m_pTypeInfo[41][8].HName="红色祝福";
		m_pTypeInfo[41][9].HName="绿色祝福";
		m_pTypeInfo[41][10].HName="蓝色祝福";
		m_pTypeInfo[41][11].HName="黑色祝福";
		m_pTypeInfo[41][12].HName="蓝鸟";
		m_pTypeInfo[41][13].HName="蓝绿祝福";
		m_pTypeInfo[41][14].HName="莴苣祝福";
		m_pTypeInfo[41][15].HName="蓝色水滴2";
		m_pTypeInfo[41][16].HName="Trans工具箱";
		m_pTypeInfo[41][17].HName="活动祝福";
		m_pTypeInfo[41][18].HName="活动用绿色祝福";
		m_pTypeInfo[41][19].HName="活动用蓝色祝福";
		m_pTypeInfo[41][20].HName="活动用黑色祝福";
		m_pTypeInfo[41][21].HName="活动用莴苣祝福";
		m_pTypeInfo[41][22].HName="黄色水滴";
		m_pTypeInfo[42][0].HName="青铜耳环";
		m_pTypeInfo[42][1].HName="银耳环";
		m_pTypeInfo[42][2].HName="金耳环";
		m_pTypeInfo[42][3].HName="死亡之光";
		m_pTypeInfo[42][4].HName="神秘之环";
		m_pTypeInfo[42][5].HName="暗黑护身符";
		m_pTypeInfo[42][6].HName="血腥盛宴";
		m_pTypeInfo[42][7].HName="永生之环";
		m_pTypeInfo[42][8].HName="猫眼";
		m_pTypeInfo[42][9].HName="末日审判";
		m_pTypeInfo[42][10].HName="灵动之环";
		m_pTypeInfo[42][11].HName="调和的宣言";
		m_pTypeInfo[42][12].HName="青玉";
		m_pTypeInfo[42][13].HName="红玉";
		m_pTypeInfo[43][0].HName="隆美尔标志";
		m_pTypeInfo[43][1].HName="处女之血";
		m_pTypeInfo[44][0].HName="破碎拳套";
		m_pTypeInfo[44][1].HName="简易拳套";
		m_pTypeInfo[44][2].HName="强力拳套";
		m_pTypeInfo[44][3].HName="合金拳套";
		m_pTypeInfo[44][4].HName="野人刺";
		m_pTypeInfo[44][5].HName="猫之拳套";
		m_pTypeInfo[44][6].HName="鹰之拳套";
		m_pTypeInfo[44][7].HName="破坏拳套";
		m_pTypeInfo[44][8].HName="强力拳套";
		m_pTypeInfo[44][9].HName="夺命拳套";
		m_pTypeInfo[44][10].HName="索魂刺";
		m_pTypeInfo[44][11].HName="显贵拳套";
		m_pTypeInfo[44][12].HName="死神拳套";
		m_pTypeInfo[44][13].HName="地狱火拳套";
		m_pTypeInfo[44][14].HName="暗黑狂魔拳套";
		m_pTypeInfo[44][15].HName="天使刺";
		m_pTypeInfo[44][16].HName="扎坎之牙";
		m_pTypeInfo[44][17].HName="巴格纳之爪";
		m_pTypeInfo[44][18].HName="蛇形弯刀";
		m_pTypeInfo[44][19].HName="棱爪";
		m_pTypeInfo[45][0].HName="红色护身符";
		m_pTypeInfo[45][1].HName="蓝色护身符";
		m_pTypeInfo[45][2].HName="紫色护身符";
		m_pTypeInfo[45][3].HName="橙色护身符";
		m_pTypeInfo[45][4].HName="粉色护身符";
		m_pTypeInfo[45][5].HName="湖蓝护身符";
		m_pTypeInfo[45][6].HName="绿色护身符";
		m_pTypeInfo[45][7].HName="暗红护身符";
		m_pTypeInfo[45][8].HName="草绿护身符";
		m_pTypeInfo[45][9].HName="桔红护身符";
		m_pTypeInfo[45][10].HName="幽黄护身符";
		m_pTypeInfo[45][11].HName="翠绿护身符";
		m_pTypeInfo[45][12].HName="灰色护身符";
		m_pTypeInfo[45][13].HName="灾殃护身符";
		m_pTypeInfo[46][0].HName="巴托里之宝石";
		m_pTypeInfo[46][1].HName="巴托里护身符";
		m_pTypeInfo[46][2].HName="布拉德之宝石";
		m_pTypeInfo[46][3].HName="特普滋护身符";
		m_pTypeInfo[46][4].HName="宝石";
		m_pTypeInfo[46][5].HName="满月卡";
		m_pTypeInfo[46][6].HName="下弦月卡";
		m_pTypeInfo[46][7].HName="红色宝袋";
		m_pTypeInfo[46][8].HName="红色宝袋";
		m_pTypeInfo[46][9].HName="Gilles de Rais护身符";
		m_pTypeInfo[47][0].HName="圣诞树碎片C";
		m_pTypeInfo[47][1].HName="圣诞树碎片A";
		m_pTypeInfo[47][2].HName="圣诞树碎片D";
		m_pTypeInfo[47][3].HName="圣诞树碎片K";
		m_pTypeInfo[47][4].HName="圣诞树碎片J";
		m_pTypeInfo[47][5].HName="圣诞树碎片H";
		m_pTypeInfo[47][6].HName="圣诞树碎片I";
		m_pTypeInfo[47][7].HName="圣诞树碎片F";
		m_pTypeInfo[47][8].HName="圣诞树碎片L";
		m_pTypeInfo[47][9].HName="圣诞树碎片E";
		m_pTypeInfo[47][10].HName="圣诞树碎片B";
		m_pTypeInfo[47][11].HName="圣诞树碎片G";
		m_pTypeInfo[47][12].HName="圣诞树";
		m_pTypeInfo[47][13].HName="古老石碑碎片Q部分";
		m_pTypeInfo[47][14].HName="古老石碑碎片A部分";
		m_pTypeInfo[47][15].HName="古老石碑碎片Z部分";
		m_pTypeInfo[47][16].HName="古老石碑碎片W部分";
		m_pTypeInfo[47][17].HName="古老石碑碎片S部分";
		m_pTypeInfo[47][18].HName="古老石碑碎片X部分";
		m_pTypeInfo[47][19].HName="古老石碑碎片E部分";
		m_pTypeInfo[47][20].HName="古老石碑碎片D部分";
		m_pTypeInfo[47][21].HName="古老石碑碎片C部分";
		m_pTypeInfo[47][22].HName="古老石碑碎片R部分";
		m_pTypeInfo[47][23].HName="古老石碑碎片F部分";
		m_pTypeInfo[47][24].HName="古老石碑碎片V部分";
		m_pTypeInfo[47][25].HName="古老石碑";
		m_pTypeInfo[47][26].HName="路标1";
		m_pTypeInfo[47][27].HName="路标2";
		m_pTypeInfo[47][28].HName="路标3";
		m_pTypeInfo[47][29].HName="粘土娃娃的碎片1";
		m_pTypeInfo[47][30].HName="粘土娃娃的碎片2";
		m_pTypeInfo[47][31].HName="粘土娃娃的碎片3";
		m_pTypeInfo[47][32].HName="粘土娃娃的碎片4";
		m_pTypeInfo[47][33].HName="粘土娃娃的碎片5";
		m_pTypeInfo[47][34].HName="粘土娃娃的碎片6";
		m_pTypeInfo[47][35].HName="粘土娃娃的碎片7";
		m_pTypeInfo[47][36].HName="粘土娃娃的碎片8";
		m_pTypeInfo[47][37].HName="粘土娃娃的碎片9";
		m_pTypeInfo[47][38].HName="粘土娃娃的碎片10";
		m_pTypeInfo[47][39].HName="粘土娃娃的碎片11";
		m_pTypeInfo[47][40].HName="粘土娃娃的碎片12";
		m_pTypeInfo[47][41].HName="粘土娃娃";
		m_pTypeInfo[48][0].HName="爆竹-天蓝色";
		m_pTypeInfo[48][1].HName="爆竹-绿色";
		m_pTypeInfo[48][2].HName="爆竹-紫色";
		m_pTypeInfo[48][3].HName="爆竹-龙";
		m_pTypeInfo[48][4].HName="爆竹-橙色";
		m_pTypeInfo[48][5].HName="爆竹-天蓝色";
		m_pTypeInfo[48][6].HName="爆竹-绿色";
		m_pTypeInfo[48][7].HName="爆竹-紫色";
		m_pTypeInfo[48][8].HName="爆竹-橙色";
		m_pTypeInfo[48][9].HName="巨型爆竹-天蓝色";
		m_pTypeInfo[48][10].HName="巨型爆竹-绿色";
		m_pTypeInfo[48][11].HName="巨型爆竹-紫色";
		m_pTypeInfo[48][12].HName="巨型爆竹-橙色";
		m_pTypeInfo[48][13].HName="风暴爆竹";
		m_pTypeInfo[48][14].HName="灵丹";
		m_pTypeInfo[48][15].HName="白色祝福";
		m_pTypeInfo[48][16].HName="莴苣祝福";
		m_pTypeInfo[48][17].HName="蜂蜜祝福";
		m_pTypeInfo[49][0].HName="亚麦加";
		m_pTypeInfo[49][1].HName="米霍尔";
		m_pTypeInfo[49][2].HName="基罗";
		m_pTypeInfo[49][3].HName="阿伊尼";
		m_pTypeInfo[49][4].HName="格利高里";
		m_pTypeInfo[49][5].HName="孔切里亚";
		m_pTypeInfo[49][6].HName="雷吉斯";
		m_pTypeInfo[49][7].HName="希而里";
		m_pTypeInfo[49][8].HName="扎夫";
		m_pTypeInfo[49][9].HName="内玛";
		m_pTypeInfo[49][10].HName="阿罗萨";
		m_pTypeInfo[49][11].HName="察斯法";
		m_pTypeInfo[50][0].HName="守护琥珀";
		m_pTypeInfo[50][1].HName="诅咒琥珀";
		m_pTypeInfo[50][2].HName="复仇琥珀";
		m_pTypeInfo[50][3].HName="永恒琥珀";
		m_pTypeInfo[50][4].HName="生命琥珀";
		m_pTypeInfo[50][5].HName="光之琥珀";
		m_pTypeInfo[51][0].HName="结婚戒指";
		m_pTypeInfo[51][1].HName="结婚戒指";
		m_pTypeInfo[52][0].HName="结婚戒指";
		m_pTypeInfo[52][1].HName="结婚戒指";
		m_pTypeInfo[53][0].HName="水灵石1";
		m_pTypeInfo[53][1].HName="水灵石2";
		m_pTypeInfo[53][2].HName="水灵石3";
		m_pTypeInfo[53][3].HName="水灵石4";
		m_pTypeInfo[53][4].HName="水灵石5";
		m_pTypeInfo[53][5].HName="火灵石1";
		m_pTypeInfo[53][6].HName="火灵石2";
		m_pTypeInfo[53][7].HName="火灵石3";
		m_pTypeInfo[53][8].HName="火灵石4";
		m_pTypeInfo[53][9].HName="火灵石5";
		m_pTypeInfo[53][10].HName="远古地图";
		m_pTypeInfo[53][11].HName="黄色矿物碎片";
		m_pTypeInfo[53][12].HName="绿色矿物碎片";
		m_pTypeInfo[53][13].HName="蓝色矿物碎片";
		m_pTypeInfo[53][14].HName="红色矿物碎片";
		m_pTypeInfo[53][15].HName="黑色矿物碎片";
		m_pTypeInfo[53][16].HName="黄色锆石";
		m_pTypeInfo[53][17].HName="绿色锆石";
		m_pTypeInfo[53][18].HName="蓝色锆石";
		m_pTypeInfo[53][19].HName="红色锆石";
		m_pTypeInfo[53][20].HName="黑色锆石";
		m_pTypeInfo[53][21].HName="瑞菲尼";
		m_pTypeInfo[53][22].HName="Thurisaz魔法圆石";
		m_pTypeInfo[53][23].HName="Gebo魔法圆石";
		m_pTypeInfo[53][24].HName="Wunjo魔法圆石";
		m_pTypeInfo[53][25].HName="Jera魔法圆石";
		m_pTypeInfo[53][26].HName="海格拉斯魔法石";
		m_pTypeInfo[53][27].HName="旗帜";
		m_pTypeInfo[53][28].HName="收费地区门票";
		m_pTypeInfo[53][29].HName="盲管";
		m_pTypeInfo[53][30].HName="福祈";
		m_pTypeInfo[53][31].HName="生命螺旋";
		m_pTypeInfo[54][0].HName="染发剂 1";
		m_pTypeInfo[54][1].HName="染发剂 2";
		m_pTypeInfo[54][2].HName="染发剂 3";
		m_pTypeInfo[54][3].HName="染发剂 4";
		m_pTypeInfo[54][4].HName="染发剂 5";
		m_pTypeInfo[54][5].HName="染发剂 6";
		m_pTypeInfo[54][6].HName="染发剂 7";
		m_pTypeInfo[54][7].HName="染发剂 8";
		m_pTypeInfo[54][8].HName="染发剂 9";
		m_pTypeInfo[54][9].HName="染发剂 10";
		m_pTypeInfo[54][10].HName="染发剂 11";
		m_pTypeInfo[54][11].HName="染发剂 12";
		m_pTypeInfo[54][12].HName="染发剂 13";
		m_pTypeInfo[54][13].HName="染发剂 14";
		m_pTypeInfo[54][14].HName="染发剂 15";
		m_pTypeInfo[54][15].HName="染发剂 16";
		m_pTypeInfo[54][16].HName="染发剂 17";
		m_pTypeInfo[54][17].HName="染发剂 18";
		m_pTypeInfo[54][18].HName="染发剂 19";
		m_pTypeInfo[54][19].HName="染发剂 20";
		m_pTypeInfo[54][20].HName="染发剂 21";
		m_pTypeInfo[54][21].HName="染发剂 22";
		m_pTypeInfo[54][22].HName="染发剂 23";
		m_pTypeInfo[54][23].HName="染发剂 24";
		m_pTypeInfo[54][24].HName="肤色转换剂1";
		m_pTypeInfo[54][25].HName="肤色转换剂2";
		m_pTypeInfo[54][26].HName="肤色转换剂3";
		m_pTypeInfo[54][27].HName="肤色转换剂4";
		m_pTypeInfo[54][28].HName="肤色转换剂5";
		m_pTypeInfo[54][29].HName="肤色转换剂6";
		m_pTypeInfo[54][30].HName="肤色转换剂7";
		m_pTypeInfo[54][31].HName="肤色转换剂8";
		m_pTypeInfo[54][32].HName="肤色转换剂9";
		m_pTypeInfo[54][33].HName="肤色转换剂10";
		m_pTypeInfo[54][34].HName="肤色转换剂11";
		m_pTypeInfo[54][35].HName="肤色转换剂12";
		m_pTypeInfo[54][36].HName="肤色转换剂13";
		m_pTypeInfo[54][37].HName="肤色转换剂14";
		m_pTypeInfo[54][38].HName="肤色转换剂15";
		m_pTypeInfo[54][39].HName="肤色转换剂16";
		m_pTypeInfo[54][40].HName="肤色转换剂17";
		m_pTypeInfo[54][41].HName="肤色转换剂18";
		m_pTypeInfo[54][42].HName="肤色转换剂19";
		m_pTypeInfo[54][43].HName="肤色转换剂20";
		m_pTypeInfo[54][44].HName="肤色转换剂21";
		m_pTypeInfo[54][45].HName="肤色转换剂22";
		m_pTypeInfo[54][46].HName="肤色转换剂23";
		m_pTypeInfo[54][47].HName="肤色转换剂24";
		m_pTypeInfo[54][48].HName="变性药水";
		m_pTypeInfo[54][49].HName="绿色变色药水";
		m_pTypeInfo[54][50].HName="蓝色变色药水";
		m_pTypeInfo[54][51].HName="红色变色药水";
		m_pTypeInfo[54][52].HName="黄色变色药水";
		m_pTypeInfo[54][53].HName="白色变色药水";
		m_pTypeInfo[54][54].HName="天青色变色药水";
		m_pTypeInfo[54][55].HName="紫色变色药水";
		m_pTypeInfo[54][56].HName="黑色变色药水";
		m_pTypeInfo[54][57].HName="自然变色药水";
		m_pTypeInfo[54][58].HName="恐惧光环石";
		m_pTypeInfo[54][59].HName="残酷光环石";
		m_pTypeInfo[54][60].HName="希望光环石";
		m_pTypeInfo[54][61].HName="自然光环石";
		m_pTypeInfo[55][0].HName="复活卷轴";
		m_pTypeInfo[55][1].HName="炼金卷轴";
		m_pTypeInfo[56][0].HName="武器合成A型";
		m_pTypeInfo[56][1].HName="武器合成B型";
		m_pTypeInfo[56][2].HName="武器合成C型";
		m_pTypeInfo[56][3].HName="防具合成A型";
		m_pTypeInfo[56][4].HName="防具合成B型";
		m_pTypeInfo[56][5].HName="防具合成C型";
		m_pTypeInfo[56][6].HName="饰品合成A型";
		m_pTypeInfo[56][7].HName="饰品合成B型";
		m_pTypeInfo[56][8].HName="饰品合成C型";
		m_pTypeInfo[56][9].HName="武器分解A型";
		m_pTypeInfo[56][10].HName="武器分解B型";
		m_pTypeInfo[56][11].HName="武器分解C型";
		m_pTypeInfo[56][12].HName="盔甲分解A型";
		m_pTypeInfo[56][13].HName="盔甲分解B型";
		m_pTypeInfo[56][14].HName="盔甲分解C型";
		m_pTypeInfo[56][15].HName="饰品分解A型";
		m_pTypeInfo[56][16].HName="饰品分解B型";
		m_pTypeInfo[56][17].HName="饰品分解C型";
		m_pTypeInfo[56][18].HName="福勺";
		m_pTypeInfo[56][19].HName="武器合成D型";
		m_pTypeInfo[56][20].HName="防具合成D型";
		m_pTypeInfo[56][21].HName="饰品合成D型";
		m_pTypeInfo[56][22].HName="武器分解D型";
		m_pTypeInfo[56][23].HName="盔甲分解D型";
		m_pTypeInfo[56][24].HName="饰品分解D型";
		m_pTypeInfo[57][0].HName="木制臂环";
		m_pTypeInfo[57][1].HName="宝石臂环";
		m_pTypeInfo[57][2].HName="水虹臂环";
		m_pTypeInfo[57][3].HName="古代臂环";
		m_pTypeInfo[57][4].HName="锦花臂环";
		m_pTypeInfo[57][5].HName="斯拉格臂环";
		m_pTypeInfo[57][6].HName="悲悯臂环";
		m_pTypeInfo[57][7].HName="苍月臂环";
		m_pTypeInfo[57][8].HName="圣翼天使臂环";
		m_pTypeInfo[57][9].HName="诸神黄昏";
		m_pTypeInfo[57][10].HName="魔翼天蝉";
		m_pTypeInfo[57][11].HName="疾风炎阳";
		m_pTypeInfo[57][12].HName="旋花臂环";
		m_pTypeInfo[57][13].HName="树妖臂环";
		m_pTypeInfo[58][0].HName="树妖之靴";
		m_pTypeInfo[58][1].HName="山妖之靴";
		m_pTypeInfo[58][2].HName="精灵之靴";
		m_pTypeInfo[58][3].HName="圣甲战靴";
		m_pTypeInfo[58][4].HName="明王之靴";
		m_pTypeInfo[58][5].HName="天狼之靴";
		m_pTypeInfo[58][6].HName="神喻战靴";
		m_pTypeInfo[58][7].HName="暗影战靴";
		m_pTypeInfo[58][8].HName="圣者战靴";
		m_pTypeInfo[58][9].HName="幻魔靴";
		m_pTypeInfo[58][10].HName="凄惶之靴";
		m_pTypeInfo[58][11].HName="精灵之靴";
		m_pTypeInfo[58][12].HName="紫焉之靴";
		m_pTypeInfo[59][0].HName="月环轮";
		m_pTypeInfo[59][1].HName="日环轮";
		m_pTypeInfo[59][2].HName="飞翼之轮";
		m_pTypeInfo[59][3].HName="因巴斯之轮";
		m_pTypeInfo[59][4].HName="生命之轮";
		m_pTypeInfo[59][5].HName="幽冥之轮";
		m_pTypeInfo[59][6].HName="破月";
		m_pTypeInfo[59][7].HName="灭日";
		m_pTypeInfo[59][8].HName="飞花流云";
		m_pTypeInfo[59][9].HName="月华无双";
		m_pTypeInfo[59][10].HName="苍风赤月";
		m_pTypeInfo[59][11].HName="玄武之澜";
		m_pTypeInfo[59][12].HName="悲鸣之风";
		m_pTypeInfo[59][13].HName="悲痛之环";
		m_pTypeInfo[59][14].HName="梅利斯之环";
		m_pTypeInfo[60][0].HName="银制发圈";
		m_pTypeInfo[60][1].HName="圆月发圈";
		m_pTypeInfo[60][2].HName="五星发圈";
		m_pTypeInfo[60][3].HName="树森之冠";
		m_pTypeInfo[60][4].HName="水苍之冠";
		m_pTypeInfo[60][5].HName="秩序之冠";
		m_pTypeInfo[60][6].HName="月神之冠";
		m_pTypeInfo[60][7].HName="首领之冠";
		m_pTypeInfo[60][8].HName="太阳神发圈";
		m_pTypeInfo[60][9].HName="王权之冠";
		m_pTypeInfo[60][10].HName="金牛之冠";
		m_pTypeInfo[60][11].HName="堕天使之怒";
		m_pTypeInfo[60][12].HName="凄惶之冠";
		m_pTypeInfo[60][13].HName="吉雷欧之冠";
		m_pTypeInfo[61][0].HName="树妖之衣";
		m_pTypeInfo[61][1].HName="山妖之衣";
		m_pTypeInfo[61][2].HName="精灵之衣";
		m_pTypeInfo[61][3].HName="圣甲战衣";
		m_pTypeInfo[61][4].HName="明王之铠";
		m_pTypeInfo[61][5].HName="天狼之铠";
		m_pTypeInfo[61][6].HName="神喻战衣";
		m_pTypeInfo[61][7].HName="暗影战衣";
		m_pTypeInfo[61][8].HName="圣者战衣";
		m_pTypeInfo[61][9].HName="幻魔甲";
		m_pTypeInfo[61][10].HName="凄惶之铠";
		m_pTypeInfo[61][11].HName="精灵外套";
		m_pTypeInfo[61][12].HName="紫焉背心";
		m_pTypeInfo[62][0].HName="小圆挂饰";
		m_pTypeInfo[62][1].HName="奥里萨挂饰";
		m_pTypeInfo[62][2].HName="神秘挂饰";
		m_pTypeInfo[62][3].HName="天地挂饰";
		m_pTypeInfo[62][4].HName="圣水挂饰";
		m_pTypeInfo[62][5].HName="何露斯之眼";
		m_pTypeInfo[62][6].HName="人面挂饰";
		m_pTypeInfo[62][7].HName="魔王之悲";
		m_pTypeInfo[62][8].HName="菲拉尔挂饰";
		m_pTypeInfo[62][9].HName="苍鹰之链";
		m_pTypeInfo[62][10].HName="残月挂饰";
		m_pTypeInfo[62][11].HName="泯灭之心";
		m_pTypeInfo[62][12].HName="精灵挂饰";
		m_pTypeInfo[62][13].HName="莱西护身符";
		m_pTypeInfo[63][0].HName="棕榈戒指";
		m_pTypeInfo[63][1].HName="钟鸣戒指";
		m_pTypeInfo[63][2].HName="保护之戒";
		m_pTypeInfo[63][3].HName="精灵密语";
		m_pTypeInfo[63][4].HName="远古戒指";
		m_pTypeInfo[63][5].HName="彗星之戒";
		m_pTypeInfo[63][6].HName="蛛眼之戒";
		m_pTypeInfo[63][7].HName="孤星之戒";
		m_pTypeInfo[63][8].HName="幻魔戒";
		m_pTypeInfo[63][9].HName="永恒之戒";
		m_pTypeInfo[63][10].HName="安达鲁西亚戒指";
		m_pTypeInfo[63][11].HName="太阳神戒指";
		m_pTypeInfo[63][12].HName="孔雀石戒指";
		m_pTypeInfo[63][13].HName="塞壬之戒";
		m_pTypeInfo[64][0].HName="火灵石1";
		m_pTypeInfo[64][1].HName="火灵石2";
		m_pTypeInfo[64][2].HName="火灵石3";
		m_pTypeInfo[64][3].HName="火灵石4";
		m_pTypeInfo[64][4].HName="火灵石5";
		m_pTypeInfo[64][5].HName="水灵石1";
		m_pTypeInfo[64][6].HName="水灵石2";
		m_pTypeInfo[64][7].HName="水灵石3";
		m_pTypeInfo[64][8].HName="水灵石4";
		m_pTypeInfo[64][9].HName="水灵石5";
		m_pTypeInfo[64][10].HName="土元素石1";
		m_pTypeInfo[64][11].HName="土元素石2";
		m_pTypeInfo[64][12].HName="土元素石3";
		m_pTypeInfo[64][13].HName="土元素石4";
		m_pTypeInfo[64][14].HName="土元素石5";
		m_pTypeInfo[65][0].HName="自然腕饰";
		m_pTypeInfo[65][1].HName="藤蔓腕饰";
		m_pTypeInfo[65][2].HName="黄铜之臂";
		m_pTypeInfo[65][3].HName="洪荒之臂";
		m_pTypeInfo[65][4].HName="水晶之臂";
		m_pTypeInfo[65][5].HName="泪之腕饰";
		m_pTypeInfo[65][6].HName="白银之臂";
		m_pTypeInfo[65][7].HName="芒杜腕饰";
		m_pTypeInfo[65][8].HName="众神腕饰";
		m_pTypeInfo[65][9].HName="炙炎腕饰";
		m_pTypeInfo[65][10].HName="自然腕饰";
		m_pTypeInfo[65][11].HName="藤蔓腕饰";
		m_pTypeInfo[65][12].HName="黄铜之臂";
		m_pTypeInfo[65][13].HName="洪荒之臂";
		m_pTypeInfo[65][14].HName="水晶之臂";
		m_pTypeInfo[65][15].HName="泪之腕饰";
		m_pTypeInfo[65][16].HName="白银之臂";
		m_pTypeInfo[65][17].HName="芒杜腕饰";
		m_pTypeInfo[65][18].HName="众神腕饰";
		m_pTypeInfo[65][19].HName="炙炎腕饰";
		m_pTypeInfo[65][20].HName="自然腕饰";
		m_pTypeInfo[65][21].HName="藤蔓腕饰";
		m_pTypeInfo[65][22].HName="黄铜之臂";
		m_pTypeInfo[65][23].HName="洪荒之臂";
		m_pTypeInfo[65][24].HName="水晶之臂";
		m_pTypeInfo[65][25].HName="泪之腕饰";
		m_pTypeInfo[65][26].HName="白银之臂";
		m_pTypeInfo[65][27].HName="芒杜腕饰";
		m_pTypeInfo[65][28].HName="众神腕饰";
		m_pTypeInfo[65][29].HName="炙炎腕饰";
		m_pTypeInfo[65][30].HName="狂嗜";
		m_pTypeInfo[65][31].HName="毁灭";
		m_pTypeInfo[65][32].HName="狂嗜";
		m_pTypeInfo[65][33].HName="毁灭";
		m_pTypeInfo[65][34].HName="狂嗜";
		m_pTypeInfo[65][35].HName="毁灭";
		m_pTypeInfo[65][36].HName="裁决之炎";
		m_pTypeInfo[65][37].HName="裁决之炎";
		m_pTypeInfo[65][38].HName="裁决之炎";
		m_pTypeInfo[65][39].HName="帕尔瓦蒂之臂";
		m_pTypeInfo[65][40].HName="帕尔瓦蒂之臂";
		m_pTypeInfo[65][41].HName="帕尔瓦蒂之臂";
		m_pTypeInfo[65][42].HName="堕落精灵之臂";
		m_pTypeInfo[65][43].HName="堕落精灵之臂";
		m_pTypeInfo[65][44].HName="堕落精灵之臂";
		m_pTypeInfo[66][0].HName="绿色幼虫";
		m_pTypeInfo[66][1].HName="新鲜幼虫";
		m_pTypeInfo[66][2].HName="中等幼虫";
		m_pTypeInfo[66][3].HName="饱满幼虫";
		m_pTypeInfo[66][4].HName="肥大幼虫";
		m_pTypeInfo[67][0].HName="绿色的蛹";
		m_pTypeInfo[67][1].HName="新鲜的蛹";
		m_pTypeInfo[67][2].HName="中等的蛹";
		m_pTypeInfo[67][3].HName="饱满的蛹";
		m_pTypeInfo[67][4].HName="肥大的蛹";
		m_pTypeInfo[67][5].HName="秘丹";
		m_pTypeInfo[68][0].HName="绿色果实";
		m_pTypeInfo[68][1].HName="新鲜果实";
		m_pTypeInfo[68][2].HName="中等果实";
		m_pTypeInfo[68][3].HName="饱满果实";
		m_pTypeInfo[68][4].HName="硕大果实";
		m_pTypeInfo[69][0].HName="初级风精灵召唤石";
		m_pTypeInfo[69][1].HName="中级风精灵召唤石";
		m_pTypeInfo[69][2].HName="高级风精灵召唤石";
		m_pTypeInfo[69][3].HName="绿色精灵宝石";
		m_pTypeInfo[69][4].HName="橙色精灵石";
		m_pTypeInfo[69][5].HName="红色精灵石";
		m_pTypeInfo[69][6].HName="黄色精灵石";
		m_pTypeInfo[69][7].HName="白色精灵石";
		m_pTypeInfo[69][8].HName="褐色精灵石";
		m_pTypeInfo[69][9].HName="紫色精灵石";
		m_pTypeInfo[69][10].HName="黑色精灵石";
		m_pTypeInfo[70][0].HName="对讲机1";
		m_pTypeInfo[70][1].HName="对讲机2";
		m_pTypeInfo[70][2].HName="对讲机3";
		m_pTypeInfo[70][3].HName="语言翻译器";
		m_pTypeInfo[70][4].HName="沙漏";
		m_pTypeInfo[70][5].HName="沙漏";
		m_pTypeInfo[70][6].HName="沙漏";
		m_pTypeInfo[71][0].HName="密码表";
		m_pTypeInfo[71][1].HName="密码表";
		m_pTypeInfo[72][0].HName="半月卡";
		m_pTypeInfo[72][1].HName="月牙卡";
		m_pTypeInfo[72][2].HName="Skill Card";
		m_pTypeInfo[72][3].HName=" 门票 ";
		m_pTypeInfo[72][4].HName="劳雷尔的皇冠";
		m_pTypeInfo[73][0].HName="M-03-01";
		m_pTypeInfo[73][1].HName="M-03-02";
		m_pTypeInfo[73][2].HName="M-03-03";
		m_pTypeInfo[73][3].HName="M-03-04";
		m_pTypeInfo[73][4].HName="M-03-05";
		m_pTypeInfo[73][5].HName="M-03-06";
		m_pTypeInfo[73][6].HName="M-03-07";
		m_pTypeInfo[73][7].HName="M-03-08";
		m_pTypeInfo[73][8].HName="M-03-09";
		m_pTypeInfo[73][9].HName="M-03-10";
		m_pTypeInfo[73][10].HName="M-03-11";
		m_pTypeInfo[73][11].HName="M-03-12";
		m_pTypeInfo[74][0].HName="假的";
		m_pTypeInfo[74][1].HName="小狗项圈";
		m_pTypeInfo[74][2].HName="狼狗项圈";
		m_pTypeInfo[74][3].HName="无线调整器";
		m_pTypeInfo[74][4].HName="蝙蝠皮包";
		m_pTypeInfo[74][5].HName="召唤石";
		m_pTypeInfo[75][0].HName="碎肉";
		m_pTypeInfo[75][1].HName="小块的肉";
		m_pTypeInfo[75][2].HName="中等大小的肉";
		m_pTypeInfo[75][3].HName="大块的肉";
		m_pTypeInfo[75][4].HName="加强饲料";
		m_pTypeInfo[75][5].HName="加强饲料";
		m_pTypeInfo[75][6].HName="中型力量包";
		m_pTypeInfo[75][7].HName="巨型力量包";
		m_pTypeInfo[75][8].HName="中型超级力量包";
		m_pTypeInfo[75][9].HName="巨型超级力量包";
		m_pTypeInfo[75][10].HName="血包";
		m_pTypeInfo[75][11].HName="浓缩血包";
		m_pTypeInfo[75][12].HName="高浓缩血包";
		m_pTypeInfo[75][13].HName="超浓缩血包";
		m_pTypeInfo[75][14].HName="微型露水";
		m_pTypeInfo[75][15].HName="小露水";
		m_pTypeInfo[75][16].HName="中型露水";
		m_pTypeInfo[75][17].HName="大露水";
		m_pTypeInfo[76][0].HName="红色水滴";
		m_pTypeInfo[76][1].HName="黄色力量水滴";
		m_pTypeInfo[76][2].HName="黄色敏捷水滴";
		m_pTypeInfo[76][3].HName="黄色智力水滴";
		m_pTypeInfo[76][4].HName="黄色回避水滴";
		m_pTypeInfo[76][5].HName="黄色防御水滴";
		m_pTypeInfo[76][6].HName="黄色幸运水滴";
		m_pTypeInfo[76][7].HName="黄色视野水滴";
		m_pTypeInfo[76][8].HName="黄色损害水滴";
		m_pTypeInfo[76][9].HName="黄色魔法攻击力水滴";
		m_pTypeInfo[76][10].HName="黄色生命力再生水滴";
		m_pTypeInfo[76][11].HName="黄色生命力水滴";
		m_pTypeInfo[76][12].HName="黄色命中率水滴";
		m_pTypeInfo[76][13].HName="恢复设置";
		m_pTypeInfo[76][14].HName="调整复活费用";
		m_pTypeInfo[76][15].HName="红鸟";
		m_pTypeInfo[76][16].HName="黄色全部能力值水滴";
		m_pTypeInfo[76][17].HName="黄色魔力水滴";
		m_pTypeInfo[76][18].HName="黄色有效打击水滴";
		m_pTypeInfo[76][19].HName="黄色攻速水滴";
		m_pTypeInfo[76][20].HName="变异注射液";
		m_pTypeInfo[77][0].HName="绿色宝袋";
		m_pTypeInfo[77][1].HName="蓝色宝袋";
		m_pTypeInfo[77][2].HName="金色宝袋";
		m_pTypeInfo[77][3].HName="优惠费用";
		m_pTypeInfo[78][0].HName="U2A 无线电 (SMS) 12";
		m_pTypeInfo[78][1].HName="带血蝙蝠 (SMS) 12";
		m_pTypeInfo[78][2].HName="阿尔塞德斯树叶(SMS) 12";
		m_pTypeInfo[78][3].HName="U2A 无线电 (SMS) 25";
		m_pTypeInfo[78][4].HName="带血蝙蝠 (SMS) 25";
		m_pTypeInfo[78][5].HName="阿尔塞德斯树叶(SMS) 25";
		m_pTypeInfo[78][6].HName="U2A 无线电 (SMS) 60";
		m_pTypeInfo[78][7].HName="带血蝙蝠 (SMS) 60";
		m_pTypeInfo[78][8].HName="阿尔塞德斯树叶(SMS) 60";
		m_pTypeInfo[79][0].HName="Gilles de Rais Core Zap黑";
		m_pTypeInfo[79][1].HName="Gilles de Rais Core Zap红";
		m_pTypeInfo[79][2].HName="Gilles de Rais Core Zap蓝";
		m_pTypeInfo[79][3].HName="Gilles de Rais Core Zap绿";
		m_pTypeInfo[80][0].HName="布里科拉卡斯的信";
		m_pTypeInfo[80][1].HName="拉里巴的信";
		m_pTypeInfo[80][2].HName="补给券";
		m_pTypeInfo[80][3].HName="数据磁盘";
		m_pTypeInfo[80][4].HName="鲜血之石1";
		m_pTypeInfo[80][5].HName="鲜血之石2";
		m_pTypeInfo[80][6].HName="鲜血之石3";
		m_pTypeInfo[80][7].HName="教授证1";
		m_pTypeInfo[80][8].HName="教授证2";
		m_pTypeInfo[80][9].HName="教授证3";
		m_pTypeInfo[80][10].HName="向导之翼1";
		m_pTypeInfo[80][11].HName="向导之翼2";
		m_pTypeInfo[80][12].HName="向导之翼3";
		m_pTypeInfo[80][13].HName="红色礼品盒";
		m_pTypeInfo[80][14].HName="橙色礼品盒";
		m_pTypeInfo[80][15].HName="黄色礼品盒";
		m_pTypeInfo[80][16].HName="绿色礼品盒";
		m_pTypeInfo[80][17].HName="蓝色礼品盒";
		m_pTypeInfo[80][18].HName="深蓝色礼品盒";
		m_pTypeInfo[80][19].HName="紫色礼品盒";
		m_pTypeInfo[80][20].HName="黑色礼品盒";
		m_pTypeInfo[80][21].HName="来自EVE的最高机密信件";
		m_pTypeInfo[80][22].HName="吸血鬼的回复";
		m_pTypeInfo[80][23].HName="来自吸血鬼的最高机密信件";
		m_pTypeInfo[80][24].HName="EVE的回复";
		m_pTypeInfo[80][25].HName="拉维亚的凭证";
		m_pTypeInfo[80][26].HName="拉里巴的信";
		m_pTypeInfo[81][0].HName="地雷";
		m_pTypeInfo[81][1].HName="迷惑炮弹";
		m_pTypeInfo[81][2].HName="破片陷阱";
		m_pTypeInfo[81][3].HName="自动炮台";
		m_pTypeInfo[81][4].HName="鼓";
		m_pTypeInfo[81][5].HName="三脚架A";
		m_pTypeInfo[81][6].HName="三脚架B";
		m_pTypeInfo[81][7].HName="三脚架C";
		m_pTypeInfo[81][8].HName="路障A";
		m_pTypeInfo[81][9].HName="路障B";
		m_pTypeInfo[82][0].HName="亚麦加";
		m_pTypeInfo[82][1].HName="米霍尔";
		m_pTypeInfo[82][2].HName="基罗";
		m_pTypeInfo[82][3].HName="阿伊尼";
		m_pTypeInfo[82][4].HName="格利高里";
		m_pTypeInfo[82][5].HName="孔切里亚";
		m_pTypeInfo[82][6].HName="雷吉斯";
		m_pTypeInfo[82][7].HName="希而里";
		m_pTypeInfo[82][8].HName="扎夫";
		m_pTypeInfo[82][9].HName="内玛";
		m_pTypeInfo[82][10].HName="阿罗萨";
		m_pTypeInfo[82][11].HName="察斯法";
		m_pTypeInfo[83][0].HName="龙之眼";
		m_pTypeInfo[84][0].HName="信息碎片";
		m_pTypeInfo[85][0].HName="蛤蟆背";
		m_pTypeInfo[86][0].HName="觉醒之冠";
		m_pTypeInfo[87][0].HName="怨灵面具";
		m_pTypeInfo[88][0].HName="帕尔玛腰带";
		m_pTypeInfo[89][0].HName="紫焉手套";
		m_pTypeInfo[90][0].HName="2x4背包";
//  end 
#endif
}

ITEMCLASS_TABLE::~ITEMCLASS_TABLE()
{
}

//---------------------------------------------------------------------
// c class甫 size俺父怒 檬扁拳茄促.
//---------------------------------------------------------------------
void
ITEMCLASS_TABLE::InitClass( int c, int size )
{
	// class俊 size俺父怒 type阑 积己	
	m_pTypeInfo[c].Init( size );
}
