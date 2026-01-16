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