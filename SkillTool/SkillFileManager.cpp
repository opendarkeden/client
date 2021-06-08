// SkillFileManager.cpp: implementation of the CSkillFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillTool.h"
#include "SkillFileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillFileManager::CSkillFileManager()
{
	//初始化变量
	m_pTable=NULL;
	m_size=0;
}

CSkillFileManager::~CSkillFileManager()
{
	//销毁内存
	if (m_pTable!=NULL)
	{
		delete[] m_pTable;
	}
}
int CSkillFileManager::LoadFromFile(char *FileName)
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
	m_pTable = new CSkillFile[m_size];
	for (int i=0;i<m_size;i++)
	{
	#ifdef _DEBUG
		if (i==m_size-1)
		{
			i=i;
		}
	#endif
	}
	file.Close();
	return 1;
}


//   CSkillFile Class

CSkillFile::CSkillFile()
{

}

CSkillFile::~CSkillFile()
{

}

int CSkillFile::LoadFromFile(CFile *file)
{
	return 1;
}