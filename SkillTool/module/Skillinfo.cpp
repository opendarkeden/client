#include "StdAfx.h"
#include ".\skillinfo.h"
#include "..\\Client\\SkillDef.h"

CSkillinfo::CSkillinfo(void)
{
	m_lpsSkillnode=NULL;
	m_curlpsSkillnode = NULL;
	NumofSkill = 0;
}

CSkillinfo::~CSkillinfo(void)
{	
	if(m_lpsSkillnode)
	{
		delete [] m_lpsSkillnode;
	}
}


bool CSkillinfo::OpenFromFile(CString FileName)
{
	CFile m_file;
	m_FileName = FileName;
	m_file.Open(FileName,CFile::modeRead);

	m_file.Read(&NumofSkill,4);
	
	if(NumofSkill!=0)
	{
		m_lpsSkillnode = new sSkillnode[NumofSkill];
	}
	
	for (int i = 0;i < NumofSkill;i++)
	{	
		m_file.Read(&m_lpsSkillnode[i].skillID,4);
		m_file.Read(&m_lpsSkillnode[i].LearnLevel,4);
		m_file.Read(&m_lpsSkillnode[i].DomainType,4);
		m_file.Read(&m_lpsSkillnode[i].dwNameLen,4);
		if (m_lpsSkillnode[i].dwNameLen!=0)
		{
			m_lpsSkillnode[i].name = new char[m_lpsSkillnode[i].dwNameLen+1];
			m_file.Read(m_lpsSkillnode[i].name,m_lpsSkillnode[i].dwNameLen);
			m_lpsSkillnode[i].name[m_lpsSkillnode[i].dwNameLen]=NULL;
		}
		m_file.Read(&m_lpsSkillnode[i].dwHnameLen,4);
		if (m_lpsSkillnode[i].dwHnameLen!=0)
		{
			m_lpsSkillnode[i].Hname = new char[m_lpsSkillnode[i].dwHnameLen+1];
			m_file.Read(m_lpsSkillnode[i].Hname,m_lpsSkillnode[i].dwHnameLen);
			m_lpsSkillnode[i].Hname[m_lpsSkillnode[i].dwHnameLen]=NULL;
		}
		m_file.Read(&m_lpsSkillnode[i].minDamage,4);
		m_file.Read(&m_lpsSkillnode[i].maxDamage,4);
		m_file.Read(&m_lpsSkillnode[i].minDelay,4);
		m_file.Read(&m_lpsSkillnode[i].maxDelay,4);
		m_file.Read(&m_lpsSkillnode[i].minDuration,4);
		m_file.Read(&m_lpsSkillnode[i].maxDuration,4);
		m_file.Read(&m_lpsSkillnode[i].mp,4);
		m_file.Read(&m_lpsSkillnode[i].minRange,4);
		m_file.Read(&m_lpsSkillnode[i].maxRange,4);
		m_file.Read(&m_lpsSkillnode[i].maxExp,4);
		SKILLDOMAIN SKillType;
		SKillType = (SKILLDOMAIN)m_lpsSkillnode[i].DomainType;
		
		if(m_lpsSkillnode[i].DomainType == 7)
		{
			m_lpsSkillnode[i].m_sSkillOUSTERS = new sSkillOUSTERS;

			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->SkillPoint,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->LevelUpPoint,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill,4);
			if(m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill != 0)
			{
				m_lpsSkillnode[i].m_sSkillOUSTERS->Skilllist= new int[m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill];
				m_file.Read(m_lpsSkillnode[i].m_sSkillOUSTERS->Skilllist,m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill*4);
			}
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Fire,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Water,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Earth,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Wind,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Sum,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Wristlet,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone1,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone2,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone3,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone4,4);
			m_file.Read(&m_lpsSkillnode[i].ElementalDomain,4);
			m_file.Read(&m_lpsSkillnode[i].m_sSkillOUSTERS->CanDelete,1);
		}
	/*	else
		{
			m_lpsSkillnode[i].ElementalDomain= -1;
		}*/
	}
	m_file.Close();
	return  true;
}

lpsSkillnode CSkillinfo::Getnode(int num)
{
	
	if(m_lpsSkillnode&&num < NumofSkill)
	{
			return &m_lpsSkillnode[num];
	}
	else 
		return NULL;
}
// 保存到文件
bool CSkillinfo::SavetoFile(void)
{	
	CFile m_file;
	m_file.Open(m_FileName,CFile::modeWrite|CFile::modeCreate);

	m_file.Write(&NumofSkill,4);


	for (int i = 0;i < NumofSkill;i++)
	{	
		m_file.Write(&m_lpsSkillnode[i].skillID,4);
		m_file.Write(&m_lpsSkillnode[i].LearnLevel,4);
		m_file.Write(&m_lpsSkillnode[i].DomainType,4);
		m_file.Write(&m_lpsSkillnode[i].dwNameLen,4);

			
		m_file.Write(m_lpsSkillnode[i].name,m_lpsSkillnode[i].dwNameLen);
		
		m_file.Write(&m_lpsSkillnode[i].dwHnameLen,4);
		
		m_file.Write(m_lpsSkillnode[i].Hname,m_lpsSkillnode[i].dwHnameLen);
	
		m_file.Write(&m_lpsSkillnode[i].minDamage,4);
		m_file.Write(&m_lpsSkillnode[i].maxDamage,4);
		m_file.Write(&m_lpsSkillnode[i].minDelay,4);
		m_file.Write(&m_lpsSkillnode[i].maxDelay,4);
		m_file.Write(&m_lpsSkillnode[i].minDuration,4);
		m_file.Write(&m_lpsSkillnode[i].maxDuration,4);
		m_file.Write(&m_lpsSkillnode[i].mp,4);
		m_file.Write(&m_lpsSkillnode[i].minRange,4);
		m_file.Write(&m_lpsSkillnode[i].maxRange,4);
		m_file.Write(&m_lpsSkillnode[i].maxExp,4);

		if(m_lpsSkillnode[i].DomainType == 7)
		{
			//m_lpsSkillnode[i].m_sSkillOUSTERS = new sSkillOUSTERS;

			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->SkillPoint,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->LevelUpPoint,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill,4);
			if(m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill != 0)
			{
				//m_lpsSkillnode[i].m_sSkillOUSTERS->Skilllist= new int[m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill];
				m_file.Write(m_lpsSkillnode[i].m_sSkillOUSTERS->Skilllist,m_lpsSkillnode[i].m_sSkillOUSTERS->szSkill*4);
			}
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Fire,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Water,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Earth,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Wind,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Sum,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Wristlet,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone1,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone2,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone3,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->Stone4,4);
			m_file.Write(&m_lpsSkillnode[i].ElementalDomain,4);
			m_file.Write(&m_lpsSkillnode[i].m_sSkillOUSTERS->CanDelete,1);
		}
	}
	m_file.Close();
/*	if(m_lpsSkillnode)
	{
		delete [] m_lpsSkillnode;
	}
	OpenFromFile(m_FileName);*/
	return true;
}
