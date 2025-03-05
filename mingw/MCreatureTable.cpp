//----------------------------------------------------------------------
// MCreatureTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MCreatureTable.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
CREATURE_TABLE*		g_pCreatureTable = NULL;

CreatureSpriteTypeMapper* g_pCreatureSpriteTypeMapper = NULL;


//----------------------------------------------------------------------
//
// ITEM_WEARINFO
//
//----------------------------------------------------------------------
ITEM_WEARINFO::ITEM_WEARINFO()
{
	skinColor = 0;
	hairColor = 0;		
	jacketColor = 0;
	pantsColor = 0;
	helmetColor = 0;
	weaponColor = 0;
	shieldColor = 0;
	motorcycleColor = 0;
		
	hair = 0;		
	jacket = 0;
	pants = 0;
	helmet = 0;
	weapon = 0;
	shield = 0;
	motorcycle = 0;	
}

//----------------------------------------------------------------------
// operator =
//----------------------------------------------------------------------
void		
ITEM_WEARINFO::operator = (const ITEM_WEARINFO& info)
{
	skinColor = info.skinColor;
	hairColor = info.hairColor;		
	jacketColor = info.jacketColor;
	pantsColor = info.pantsColor;
	helmetColor = info.helmetColor;
	weaponColor = info.weaponColor;
	shieldColor = info.shieldColor;
	motorcycleColor = info.motorcycleColor;
		
	hair = info.hair;		
	jacket = info.jacket;
	pants = info.pants;
	helmet = info.helmet;
	weapon = info.weapon;
	shield = info.shield;
	motorcycle = info.motorcycle;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void		
ITEM_WEARINFO::SaveToFile(ofstream& file)
{
	file.write((const char*)&skinColor, 2);
	file.write((const char*)&hairColor, 2);
	file.write((const char*)&jacketColor, 2);
	file.write((const char*)&pantsColor, 2);
	file.write((const char*)&helmetColor, 2);
	file.write((const char*)&weaponColor, 2);
	file.write((const char*)&shieldColor, 2);
	file.write((const char*)&motorcycleColor, 2);
	
	file.write((const char*)&hair, 1);
	file.write((const char*)&jacket, 1);
	file.write((const char*)&pants, 1);
	file.write((const char*)&helmet, 1);
	file.write((const char*)&weapon, 1);
	file.write((const char*)&shield, 1);
	file.write((const char*)&motorcycle, 1);	
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void		
ITEM_WEARINFO::LoadFromFile(ifstream& file)
{
	file.read((char*)&skinColor, 2);
	file.read((char*)&hairColor, 2);
	file.read((char*)&jacketColor, 2);
	file.read((char*)&pantsColor, 2);
	file.read((char*)&helmetColor, 2);
	file.read((char*)&weaponColor, 2);
	file.read((char*)&shieldColor, 2);
	file.read((char*)&motorcycleColor, 2);
	
	file.read((char*)&hair, 1);
	file.read((char*)&jacket, 1);
	file.read((char*)&pants, 1);
	file.read((char*)&helmet, 1);
	file.read((char*)&weapon, 1);
	file.read((char*)&shield, 1);
	file.read((char*)&motorcycle, 1);
}


//----------------------------------------------------------------------
//
// CREATURETABLE_INFO
//
//----------------------------------------------------------------------
CREATURETABLE_INFO::CREATURETABLE_INFO()
{ 	
	MoveTimesMotor = 1;
	bMale			= true;
	ColorSet		= 0;
	m_pActionSound	= NULL; 
	m_pActionCount	= NULL;	

	bFlyingCreature = false;
	FlyingHeight = 0;
	bHeadCut = false;
	HPBarWidth = 120;
	
	ChangeColorSet = 0xFFFF;	// default는 사용하지 않는다.

	pItemWearInfo = NULL;

	ShadowCount = 0;
}

CREATURETABLE_INFO::~CREATURETABLE_INFO()
{
	
	if (m_pActionSound != NULL)
	{
		delete [] m_pActionSound;
		m_pActionSound = NULL;
	}
		

	if (m_pActionCount != NULL)
	{
		delete [] m_pActionCount;
		m_pActionCount = NULL;
	}

	if (pItemWearInfo!=NULL)
	{
		delete pItemWearInfo;
	}
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Creature Type
//----------------------------------------------------------------------
void				
CREATURETABLE_INFO::SetCreatureTribe(enum CREATURETRIBE ct, int nMaxAction)
{ 
	m_CreatureTribe = ct; 

	InitActionType(nMaxAction); 	
}

//----------------------------------------------------------------------
// Init ActionSound Type ( m_CreatureTribe )
//----------------------------------------------------------------------
// m_CreatureType에 맞게 SoundID의 개수를 적절히 설정한다.
//----------------------------------------------------------------------
void			
CREATURETABLE_INFO::InitActionType(int nMaxAction)
{
	if (m_pActionSound!=NULL)
	{
		delete [] m_pActionSound;
		m_pActionSound = NULL;
	}

	if (m_pActionCount!=NULL)
	{
		delete [] m_pActionCount;
		m_pActionCount = NULL;
	}

	int max;
	if(0 == nMaxAction)
		max = GetActionMax();
	else 
		max = nMaxAction;

	m_pActionSound = new TYPE_SOUNDID [max];
	m_pActionCount = new int [max];

	for (int i=0; i<max; i++)
	{
		m_pActionSound[i] = SOUNDID_NULL;
		m_pActionCount[i] = 0;
	}
}

//----------------------------------------------------------------------
// Get ActionSound Max
//----------------------------------------------------------------------
int
CREATURETABLE_INFO::GetActionMax() const
{

	switch (m_CreatureTribe)
	{
		case CREATURETRIBE_SLAYER :			// 슬레이어
		case CREATURETRIBE_SLAYER_NPC :		// 슬레이어 NPC
			return ACTION_MAX_SLAYER;
		break;

		case CREATURETRIBE_VAMPIRE :			// 뱀파이어
			if(204 == SpriteTypes[0]) // 질드레
				return ACTION_MAX_OUSTERS;
			return ACTION_MAX_VAMPIRE;
		break;

		case CREATURETRIBE_OUSTERS :			// 아우스터즈
		case CREATURETRIBE_OUSTERS_NPC:
			return ACTION_MAX_OUSTERS;
			break;
			
		case CREATURETRIBE_NPC :				// NPC
			//return ACTION_MAX_NPC;
			return ACTION_MAX_VAMPIRE;
		break;		
	}

	return 0;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void			
CREATURETABLE_INFO::SaveToFile(ofstream& file)
{
	Name.SaveToFile( file );

	int stcnt = SpriteTypes.GetSize();
	file.write((const char*)&stcnt, 4);
	for(int st = 0; st < stcnt; st++)
	{
//		file.write((const char*)&SpriteTypes[stcnt-(st+1)], 4);
		file.write((const char*)&SpriteTypes[st], 4);
	}
	
	file.write((const char*)&bMale, 1);
	BYTE ct = m_CreatureTribe;
	file.write((const char*)&ct, 1);
	file.write((const char*)&MoveTimes, 1);
	file.write((const char*)&MoveRatio, 1);
	file.write((const char*)&MoveTimesMotor, 1);
	file.write((const char*)&Height, 4);
	file.write((const char*)&DeadHeight, 4);
	file.write((const char*)&DeadActionInfo, SIZE_ACTIONINFO);
	file.write((const char*)&ColorSet, 4);
	file.write((const char*)&bFlyingCreature, 1);
	file.write((const char*)&FlyingHeight, 4);
	file.write((const char*)&bHeadCut, 4);
	file.write((const char*)&HPBarWidth, 4);
	file.write((const char*)&ChangeColorSet, 2);
	file.write((const char*)&ShadowCount, 2);
	
	
	// 각각의 soundID를 저장한다.
	int max = GetActionMax();
	for (int i=0; i<max; i++)
	{
		file.write((const char*)&m_pActionSound[i], SIZE_SOUNDID);
	}

	// 각각의 CountID를 저장한다.
	for (i=0; i<max; i++)
	{
		file.write((const char*)&m_pActionCount[i], 4);
	}

	//---------------------------------------------------------------
	// 슬레이어 NPC 복장 정보
	//---------------------------------------------------------------
	bool bExistItemWearInfo = (pItemWearInfo!=NULL);
	
	file.write((const char*)&bExistItemWearInfo, 1);

	if (bExistItemWearInfo)
	{
		pItemWearInfo->SaveToFile( file );
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void			
CREATURETABLE_INFO::LoadFromFile(ifstream& file)
{
	Name.LoadFromFile( file );

	int stcount = 0;
	file.read((char*)&stcount, 4);
#ifndef __INIT_INFO_
	SpriteTypes.Init(stcount);
#endif
	//add by viva
	for(int st = 0; st < stcount; st++) 
	{	
		file.read((char*)&SpriteTypes[st], 4);
/*		if(Name == "NewVampireMale180")
			SpriteTypes[st] = 0xFC;
		else if(Name == "NewVampireFeMale180")
			SpriteTypes[st] = 0xFD;
		else if(Name == "NewVampireMale195")
			SpriteTypes[st] = 0xFC;
		else if(Name == "NewVampireFeMale195")
			SpriteTypes[st] = 0xFD;
		else if(Name == "NewVampireMale215")
			SpriteTypes[st] = 0xFC;
		else if(Name == "NewVampireFeMale215")
			SpriteTypes[st] = 0xFD;			*/
	}
	

	file.read((char*)&bMale, 1);

	BYTE ct;	
	file.read((char*)&ct, 1);
	SetCreatureTribe((enum CREATURETRIBE)ct);		// soundID array초기화

	file.read((char*)&MoveTimes, 1);
	file.read((char*)&MoveRatio, 1);
	file.read((char*)&MoveTimesMotor, 1);
	file.read((char*)&Height, 4);
	// add by Coffee 2006.11.3  劤경굶灌列鑒앴
	//DWORD dwLaJi=0;
	//file.read((char*)&dwLaJi,4);
	// end 
	file.read((char*)&DeadHeight, 4);
	file.read((char*)&DeadActionInfo, SIZE_ACTIONINFO);
	file.read((char*)&ColorSet, 4);
	file.read((char*)&bFlyingCreature, 1);
	file.read((char*)&FlyingHeight, 4);
	file.read((char*)&bHeadCut, 4);
	file.read((char*)&HPBarWidth, 4);
	file.read((char*)&ChangeColorSet, 2);
	file.read((char*)&ShadowCount, 2);

	// 각각의 SoundID를 load한다.
	int max = GetActionMax();
	for (int i=0; i<max; i++)
	{
		file.read((char*)&m_pActionSound[i], SIZE_SOUNDID);
	}

	// 각각의 SoundID를 load한다.
	for (i=0; i<max; i++)
	{
		file.read((char*)&m_pActionCount[i], 4);
	}
	//add by viva
	bool isread=true;
	if(m_CreatureTribe==4||m_CreatureTribe==5)
	{
		char temp[24];
//		file.read(temp,24);
		isread = false;
	}
	//---------------------------------------------------------------
	// 슬레이어 NPC 복장 정보
	//---------------------------------------------------------------
	if (pItemWearInfo!=NULL)
	{
		delete pItemWearInfo;
		pItemWearInfo = NULL;
	}

	bool bExistItemWearInfo;
	
	file.read((char*)&bExistItemWearInfo, 1);

	if (bExistItemWearInfo&&isread/*add by viva*/)
	{
		pItemWearInfo = new ITEM_WEARINFO;
		pItemWearInfo->LoadFromFile( file );
	}
}

//----------------------------------------------------------------------
// assign operator = 
//----------------------------------------------------------------------
void				
CREATURETABLE_INFO::operator = (const CREATURETABLE_INFO& creatureInfo)
{
	Name		= creatureInfo.Name;
	SpriteTypes.Init(creatureInfo.SpriteTypes.GetSize());

	for(int st = 0; st< creatureInfo.SpriteTypes.GetSize(); st++)
		SpriteTypes[st] = creatureInfo.SpriteTypes[st];

	bMale		= creatureInfo.bMale;
	SetCreatureTribe( creatureInfo.m_CreatureTribe );

	MoveTimes		= creatureInfo.MoveTimes;
	MoveRatio		= creatureInfo.MoveRatio;
	MoveTimesMotor	= creatureInfo.MoveTimesMotor;
	Height			= creatureInfo.Height;
	DeadHeight		= creatureInfo.DeadHeight;
	DeadActionInfo	= creatureInfo.DeadActionInfo;
	ColorSet		= creatureInfo.ColorSet;
	bFlyingCreature = creatureInfo.bFlyingCreature;
	FlyingHeight	= creatureInfo.FlyingHeight;
	bHeadCut		= creatureInfo.bHeadCut;
	HPBarWidth		= creatureInfo.HPBarWidth;
	ChangeColorSet = creatureInfo.ChangeColorSet;
	ShadowCount		= creatureInfo.ShadowCount;

	// 각각의 SoundID를 load한다.
	int max = GetActionMax();
	for (int i=0; i<max; i++)
	{
		m_pActionSound[i] = creatureInfo.m_pActionSound[i];
	}

	// 각각의 SoundID를 load한다.
	for (i=0; i<max; i++)
	{
		m_pActionCount[i] = creatureInfo.m_pActionCount[i];
	}

	if (pItemWearInfo!=NULL)
	{
		delete pItemWearInfo;
		pItemWearInfo = NULL;
	}

	if (creatureInfo.pItemWearInfo!=NULL)
	{
		pItemWearInfo = new ITEM_WEARINFO;
		
		*pItemWearInfo = *(creatureInfo.pItemWearInfo);
	}	
}


//----------------------------------------------------------------------
//
// constructor / destructor
// 
//----------------------------------------------------------------------
CreatureSpriteTypeMapper::CreatureSpriteTypeMapper()
{
}

CreatureSpriteTypeMapper::~CreatureSpriteTypeMapper()
{
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void	
CreatureSpriteTypeMapper::Init(int numSpriteTypes)
{
	Release();

	m_CreatureSpriteTypes.reserve( numSpriteTypes );

	for (int i=0; i<numSpriteTypes; i++)
	{
		m_CreatureSpriteTypes[i] = NULL;
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void	
CreatureSpriteTypeMapper::Release()
{
	int numSpriteTypes = m_CreatureSpriteTypes.capacity();

	for (int i=0; i<numSpriteTypes; i++)
	{
		if (m_CreatureSpriteTypes[i] != NULL)
		{
			delete m_CreatureSpriteTypes[i];
			m_CreatureSpriteTypes[i] = NULL;
		}
	}
}


//----------------------------------------------------------------------
// Add CreatureType
//----------------------------------------------------------------------
void	
CreatureSpriteTypeMapper::AddCreatureType(TYPE_SPRITEID spriteID, WORD creatureType)
{
	if (m_CreatureSpriteTypes[spriteID]==NULL)
	{
		CREATURE_TYPES* pCreatureTypes = new CREATURE_TYPES;
		pCreatureTypes->push_back( creatureType );

		m_CreatureSpriteTypes[spriteID] = pCreatureTypes;
	}
	else
	{
		m_CreatureSpriteTypes[spriteID]->push_back( creatureType );
	}
}

//----------------------------------------------------------------------
// Get Random CreatureType
//----------------------------------------------------------------------
int		
CreatureSpriteTypeMapper::GetRandomCreatureType(TYPE_SPRITEID spriteID) const
{
	const int defaultCreatureType = 28;

	if (spriteID > m_CreatureSpriteTypes.capacity())
	{
		// 블러드 워록 default - -;
		return defaultCreatureType;
	}

	const CREATURE_TYPES* pCreatureTypes = m_CreatureSpriteTypes[spriteID];

	int numCreatures = pCreatureTypes->capacity();

	if (numCreatures==0)
	{
		return defaultCreatureType;
	}

	int selectNum = rand()%numCreatures;

	return (*pCreatureTypes)[selectNum];
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void				
CreatureSpriteTypeMapper::SaveToFile(ofstream& file)
{
	int numSpriteTypes = m_CreatureSpriteTypes.capacity();

	file.write((const char*)&numSpriteTypes, 4);

	for (int i=0; i<numSpriteTypes; i++)
	{
		CREATURE_TYPES* pCreatureTypes = m_CreatureSpriteTypes[i];

		if (pCreatureTypes==NULL)
		{
			int numCreatureTypes = 0;
			file.write((const char*)&numCreatureTypes, 4);
		}
		else
		{
			int numCreatureTypes = pCreatureTypes->size();

			file.write((const char*)&numCreatureTypes, 4);

			for (int j=0; j<numCreatureTypes; j++)
			{
				WORD num = (*pCreatureTypes)[j];
				file.write((const char*)&num, 2);
			}
		}
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void				
CreatureSpriteTypeMapper::LoadFromFile(ifstream& file)
{
	int numSpriteTypes;
	
	file.read((char*)&numSpriteTypes, 4);
	
	Init( numSpriteTypes );

	for (int i=0; i<numSpriteTypes; i++)
	{
		CREATURE_TYPES* pCreatureTypes = new CREATURE_TYPES;
		
		m_CreatureSpriteTypes[i] = pCreatureTypes;

		int numCreatureTypes;
		
		file.read((char*)&numCreatureTypes, 4);

		pCreatureTypes->reserve( numCreatureTypes );

		for (int j=0; j<numCreatureTypes; j++)
		{
			WORD creatureType;
			file.read((char*)&creatureType, 2);

			(*pCreatureTypes)[j] = creatureType;
		}
	}
}