// MEFFECTSPRITETYPETABLE_INFO.cpp: implementation of the MEFFECTSPRITETYPETABLE_INFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillTool.h"
#include "CEffectSpriteTypeManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEffectSpriteTypeManager::CEffectSpriteTypeManager()
{
	m_size =0;
	m_pEffectTab=NULL;
}
CEffectSpriteTypeManager::~CEffectSpriteTypeManager()
{
	if (m_pEffectTab!=NULL)
	{
		delete[] m_pEffectTab;
	}
}
int CEffectSpriteTypeManager::LoadFromFile(char* FileName)
{
	CFile file;
	if (!file.Open(FileName,CFile::modeRead))
	{
		return 0;
	}
	file.Read(&m_size,4);
 	if (m_pEffectTab!=NULL)
	{
		delete[] m_pEffectTab;
	}
	m_pEffectTab = new CEffectSpriteType[m_size];
	int a=0,b=0,c=0,d=0;
	for (int i=0;i<m_size;i++)
	{
		m_pEffectTab[i].LoadFromFile(&file);
		switch(m_pEffectTab[i].BltType)
		{
			case 0:
				a++;
				break;
			case 1:
				b++;
				break;
			case 2:
				c++;
				break;
			case 3:
				d++;
				break;
		}
	}
	/* dk2                    kr
	BLT_NORMAL =67			BLT_NORMAL =67
	BLT_EFFECT =1229		BLT_EFFECT =1348   
	BLT_SHADOW =1			BLT_SHADOW =1
	BLT_SCREEN =570			BLT_SCREEN =578
	*/
	file.Close();
	return 1;
}



CEffectSpriteType::CEffectSpriteType()
{
	PairFrameIDList=NULL;
	PairFrameIDListNum=0;
}

CEffectSpriteType::~CEffectSpriteType()
{
	if (PairFrameIDList!=NULL)
	{
		delete[] PairFrameIDList;
	}
}
int CEffectSpriteType::LoadFromFile(CFile* file)
{
	BYTE bt;
	file->Read((char*)&bt, 1);
	BltType = (BLT_TYPE)bt;
	file->Read((char*)&FrameID, 2);	
	BYTE flag;
	
	file->Read((char*)&flag, 1);
	
	RepeatFrame = (flag>>1)&0x1;
	bPairFrameBack = (flag)&0x1;

	file->Read((char*)&ActionEffectFrameID, 2);	
	file->Read((char*)&FemaleEffectSpriteType, 2);	

	//----------------------------------------------------------
	// Pair FrameID List
	//----------------------------------------------------------


	BYTE numPair;	
	file->Read((char*)&numPair, 1);
	if (PairFrameIDList!=NULL)
	{
		delete[] PairFrameIDList;
	}
	PairFrameIDList=new WORD[numPair];

	WORD frameID;
	PairFrameIDListNum = numPair;
	for (int i=0; i<PairFrameIDListNum; i++)
	{	
		file->Read((char*)&frameID, 2);

		PairFrameIDList[frameID];
	}
	return 1;
}