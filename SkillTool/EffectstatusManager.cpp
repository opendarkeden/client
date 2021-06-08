// EffectstatusManager.cpp: implementation of the CEffectstatusManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillTool.h"
#include "EffectstatusManager.h"
#include "../Client/MTypeDef.h"
#include "../Client/EffectSpriteTypeDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//#define EFFECTSPRITETYPE_NULL 0xFFFF;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectstatusManager::CEffectstatusManager()
{
	m_size=0;
	m_pTable=NULL;
}

CEffectstatusManager::~CEffectstatusManager()
{
	delete[] m_pTable;
}

int CEffectstatusManager::LoadFromFile(char *FileName)
{
	CFile file;
	if (!file.Open(FileName,CFile::modeRead))
	{
		return 0;
	}
	file.Read(&m_size,4);
 	if (m_pTable!=NULL)
	{
		delete[] m_pTable;
	}
	m_pTable = new CEffectstatus[m_size];
	
	for (int i=0;i<m_size;i++)
	{
	#ifdef _DEBUG
		if (i==m_size-1)
		{
			i=i;
		}
	#endif
		m_pTable[i].LoadFromFile(&file);
		WORD n=m_pTable[i].EffectSpriteType;
	}
	file.Close();
	return 1;
}


CEffectstatus::CEffectstatus()
{
	//各变量初始值
	bUseEffectSprite	= true;
	bAttachGround		= false;

	EffectSpriteType	= EFFECTSPRITETYPE_NULL;
	EffectColor			= 0xFFFF;
	EffectColorPart		= ADDON_NULL;
	
	ActionInfo			= ACTIONINFO_NULL;
	OriginalActionInfo	= ACTIONINFO_NULL;
	SoundID				= SOUNDID_NULL;	

}
CEffectstatus::~CEffectstatus()
{
	
}
int CEffectstatus::LoadFromFile(CFile *file)
{
	file->Read((char*)&bUseEffectSprite, 1);
	file->Read((char*)&bAttachGround, 1);

	file->Read((char*)&EffectSpriteType,2);// SIZE_EFFECTSPRITETYPE);	
	file->Read((char*)&EffectColor, 2);

	BYTE part;
	file->Read((char*)&part, 1);
	EffectColorPart = (ADDON)part;

	file->Read((char*)&ActionInfo, 2);//SIZE_ACTIONINFO);
	file->Read((char*)&OriginalActionInfo,2); //SIZE_ACTIONINFO);
	file->Read((char*)&SoundID, sizeof(int) );	
	return 1;
}
