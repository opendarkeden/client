//----------------------------------------------------------------------
// MEffectSpriteTypeTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffectSpriteTypeTable.h"


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
EFFECTSPRITETYPE_TABLE*			g_pEffectSpriteTypeTable = NULL;
MActionEffectSpriteTypeTable*	g_pActionEffectSpriteTypeTable = NULL;

// Memory protection: Store a copy of the pointer to detect corruption
// If the pointer is corrupted, we can detect it by comparing with the shadow
#ifdef __SANITIZE_ADDRESS__
EFFECTSPRITETYPE_TABLE* g_pEffectSpriteTypeTable_shadow = NULL;
EFFECTSPRITETYPE_TABLE::TYPE* g_pEffectSpriteTypeTable_m_pTypeInfo_shadow = NULL;
static uint64_t g_pEffectSpriteTypeTable_canary = 0xDEADBEEFCAFEBABEULL;

void validate_effect_sprite_table_pointer(const char* location) {
	if (g_pEffectSpriteTypeTable != g_pEffectSpriteTypeTable_shadow) {
		fprintf(stderr, "[CORRUPTION] g_pEffectSpriteTypeTable corrupted at %s!\n", location);
		fprintf(stderr, "[CORRUPTION] Expected: %p, Got: %p\n",
		        g_pEffectSpriteTypeTable_shadow, g_pEffectSpriteTypeTable);
		// Don't abort - let ASAN handle the crash with better diagnostics
	}

	// Check if m_pTypeInfo internal pointer is corrupted
	if (g_pEffectSpriteTypeTable != NULL && g_pEffectSpriteTypeTable_shadow != NULL) {
		EFFECTSPRITETYPE_TABLE::TYPE* current_m_pTypeInfo = g_pEffectSpriteTypeTable->GetInternalPointer();
		if (current_m_pTypeInfo != g_pEffectSpriteTypeTable_m_pTypeInfo_shadow) {
			fprintf(stderr, "[CORRUPTION] g_pEffectSpriteTypeTable->m_pTypeInfo corrupted at %s!\n", location);
			fprintf(stderr, "[CORRUPTION] Expected m_pTypeInfo: %p, Got: %p\n",
			        g_pEffectSpriteTypeTable_m_pTypeInfo_shadow, current_m_pTypeInfo);
		}

		// Check if m_pTypeInfo points to freed SDL surface memory region
		// SDL surfaces are typically allocated in specific memory ranges
		uintptr_t ptr_addr = (uintptr_t)current_m_pTypeInfo;
		// Check if it looks like a heap pointer that might be in a freed region
		if (ptr_addr > 0x1000 && ptr_addr < 0x100000000ULL) {
			// Use ASAN to check if the memory is poisoned
			if (__asan_address_is_poisoned(current_m_pTypeInfo, sizeof(EFFECTSPRITETYPE_TABLE::TYPE))) {
				fprintf(stderr, "[CORRUPTION] m_pTypeInfo points to poisoned/freed memory at %s!\n", location);
				fprintf(stderr, "[CORRUPTION] m_pTypeInfo=%p\n", current_m_pTypeInfo);
			}
		}
	}

	if (g_pEffectSpriteTypeTable_canary != 0xDEADBEEFCAFEBABEULL) {
		fprintf(stderr, "[CORRUPTION] Canary corrupted at %s!\n", location);
		fprintf(stderr, "[CORRUPTION] Canary value: 0x%llx\n", g_pEffectSpriteTypeTable_canary);
	}
}

#define VALIDATE_TABLE() validate_effect_sprite_table_pointer(__FUNCTION__)
#else
#define VALIDATE_TABLE() do {} while(0)
#endif

//----------------------------------------------------------------------
//
//		FRAME_TYPE
//
//----------------------------------------------------------------------
void		
FRAME_TYPE::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&FrameID, SIZE_FRAMEID);
}

void		
FRAME_TYPE::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&FrameID, SIZE_FRAMEID);
}

//----------------------------------------------------------------------
//
//		ACTION_FRAMEID_ARRAY
//
//----------------------------------------------------------------------
ACTION_FRAMEID_ARRAY::ACTION_FRAMEID_ARRAY()
{
	Init( ACTION_MAX_SLAYER );
}

ACTION_FRAMEID_ARRAY::~ACTION_FRAMEID_ARRAY()
{
}

//----------------------------------------------------------------------
//
//		MActionEffectSpriteTypeTable
//
//----------------------------------------------------------------------
MActionEffectSpriteTypeTable::MActionEffectSpriteTypeTable()
{
}

MActionEffectSpriteTypeTable::~MActionEffectSpriteTypeTable()
{
}

//----------------------------------------------------------------------
//
//		EFFECTSPRITETYPETABLE_INFO
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
EFFECTSPRITETYPETABLE_INFO::EFFECTSPRITETYPETABLE_INFO()
{
	bPairFrameBack = false;
}

void			
EFFECTSPRITETYPETABLE_INFO::SaveToFile(std::ofstream& file)
{
	BYTE bt = (BYTE)BltType;
	file.write((const char*)&bt, 1);
	file.write((const char*)&FrameID, SIZE_FRAMEID);			

	BYTE flag=0;
	
	flag = ( (RepeatFrame==true)<<1 ) | ( bPairFrameBack == true );

	file.write((const char*)&flag, 1);

	file.write((const char*)&ActionEffectFrameID, SIZE_FRAMEID);
	//file.write((const char*)&PairFrameID, SIZE_FRAMEID);	
	file.write((const char*)&FemaleEffectSpriteType, SIZE_EFFECTSPRITETYPE);		

	//----------------------------------------------------------
	// Pair FrameID List
	//----------------------------------------------------------
	BYTE numPair = PairFrameIDList.size();
	file.write((const char*)&numPair, 1);

	FRAMEID_LIST::const_iterator iFrame = PairFrameIDList.begin();

	while (iFrame != PairFrameIDList.end())
	{
		TYPE_FRAMEID frameID = *iFrame;

		file.write((const char*)&frameID, SIZE_FRAMEID);

		iFrame++;
	}
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
EFFECTSPRITETYPETABLE_INFO::LoadFromFile(std::ifstream& file)
{
	BYTE bt;
	file.read((char*)&bt, 1);
	BltType = (BLT_TYPE)bt;
	file.read((char*)&FrameID, SIZE_FRAMEID);	
	BYTE flag;
	
	file.read((char*)&flag, 1);
	
	RepeatFrame = (flag>>1)&0x1;
	bPairFrameBack = (flag)&0x1;

	file.read((char*)&ActionEffectFrameID, SIZE_FRAMEID);
	//file.read((char*)&PairFrameID, SIZE_FRAMEID);	
	file.read((char*)&FemaleEffectSpriteType, SIZE_EFFECTSPRITETYPE);	

	//----------------------------------------------------------
	// Pair FrameID List
	//----------------------------------------------------------
	PairFrameIDList.clear();

	BYTE numPair;	
	file.read((char*)&numPair, 1);

	TYPE_FRAMEID frameID;
	int intNumPair = numPair;
	for (int i=0; i<intNumPair; i++)
	{	
		file.read((char*)&frameID, SIZE_FRAMEID);

		PairFrameIDList.push_back( frameID );
	}
}