#include "StdAfx.h"
#include ".\skillfile.h"

CSkillFile::CSkillFile(void)
{

	numberofskill = 0;
	m_lpsSkillnode=NULL;
}

CSkillFile::~CSkillFile(void)
{
	if (m_lpsSkillnode)
	{
		delete [] m_lpsSkillnode;
	}
}

bool CSkillFile::readFromFile(CString FileName)
{
	CFile m_file;
	
	m_file.Open(FileName,CFile::modeRead);
	m_filename = FileName;
	m_file.Read(&numberofskill,4);
	
	m_lpsSkillnode = new sSkillnode[numberofskill];
	char *temp;
	for (int i = 0; i<numberofskill;i++)
	{
		m_file.Read(&m_lpsSkillnode[i].Namelen,4);
		temp =new char[m_lpsSkillnode[i].Namelen];
		m_file.Read(temp,m_lpsSkillnode[i].Namelen);
		m_lpsSkillnode[i].Name.Format("%s",temp);
		delete [] temp;
		m_file.Read(&m_lpsSkillnode[i].HNamelen,4);
		temp =new char[m_lpsSkillnode[i].HNamelen];
		m_file.Read(temp,m_lpsSkillnode[i].HNamelen);
		m_lpsSkillnode[i].HName.Format("%s",temp);
		delete [] temp;
		m_file.Read(&m_lpsSkillnode[i].Level,4);
		m_file.Read(&m_lpsSkillnode[i].x,4);
		m_file.Read(&m_lpsSkillnode[i].y,4);
		m_file.Read(&m_lpsSkillnode[i].SpriteID,2);
		m_file.Read(&m_lpsSkillnode[i].MP,4);
		m_file.Read(&m_lpsSkillnode[i].Passive,1);
		m_file.Read(&m_lpsSkillnode[i].Active,1);
		m_file.Read(&m_lpsSkillnode[i].skillStep,1);
		m_file.Read(&m_lpsSkillnode[i].idNum,4);
		if (m_lpsSkillnode[i].idNum !=0)
		{
			m_lpsSkillnode[i].ACTIONINFO_idlist = new unsigned short[m_lpsSkillnode[i].idNum];
			m_file.Read(m_lpsSkillnode[i].ACTIONINFO_idlist,2*m_lpsSkillnode[i].idNum);
		}
	}
	
	m_file.Close();
	return true;
}
// ´æ´¢µ½ÎÄ¼þ
bool CSkillFile::SaveToFile(void)
{

	CFile m_file(m_filename,CFile::modeWrite);
	m_file.Write(&numberofskill,4);

//	m_lpsSkillnode = new sSkillnode[numberofskill];
	//char *temp;
	for (int i = 0; i<numberofskill;i++)
	{
		m_file.Write(&m_lpsSkillnode[i].Namelen,4);
	//	temp =new char[m_lpsSkillnode[i].Namelen];
		m_file.Write(m_lpsSkillnode[i].Name,m_lpsSkillnode[i].Namelen);
	//	m_lpsSkillnode[i].Name.SetString(temp,m_lpsSkillnode[i].Namelen);
	//	delete [] temp;
		m_file.Write(&m_lpsSkillnode[i].HNamelen,4);
//		temp =new char[m_lpsSkillnode[i].HNamelen];
		m_file.Write(m_lpsSkillnode[i].HName,m_lpsSkillnode[i].HNamelen);
//		m_lpsSkillnode[i].HName.SetString(temp,m_lpsSkillnode[i].HNamelen);
//		delete [] temp;
		m_file.Write(&m_lpsSkillnode[i].Level,4);
		m_file.Write(&m_lpsSkillnode[i].x,4);
		m_file.Write(&m_lpsSkillnode[i].y,4);
		m_file.Write(&m_lpsSkillnode[i].SpriteID,2);
		m_file.Write(&m_lpsSkillnode[i].MP,4);
		m_file.Write(&m_lpsSkillnode[i].Passive,1);
		m_file.Write(&m_lpsSkillnode[i].Active,1);
		m_file.Write(&m_lpsSkillnode[i].skillStep,1);
		m_file.Write(&m_lpsSkillnode[i].idNum,4);
		if (m_lpsSkillnode[i].idNum !=0)
		{
//			m_lpsSkillnode[i].ACTIONINFO_idlist = new unsigned short[m_lpsSkillnode[i].idNum];
			m_file.Write(m_lpsSkillnode[i].ACTIONINFO_idlist,2*m_lpsSkillnode[i].idNum);
		}
	}

	return true;
}