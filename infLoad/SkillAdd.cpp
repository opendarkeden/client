// SkillAdd.cpp: implementation of the SkillAdd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "infLoad.h"
#include "SkillAdd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SkillAdd::SkillAdd()
{
	m_pSkillInfoTable=NULL;
}

SkillAdd::~SkillAdd()
{
	if (m_pSkillInfoTable != NULL)
	{
		delete m_pSkillInfoTable;
		m_pSkillInfoTable = NULL;
	}
}

void SkillAdd::LoadFile(const char *filename)
{
	if (m_pSkillInfoTable != NULL)
	{
		delete m_pSkillInfoTable;
		m_pSkillInfoTable = NULL;
	}

	m_pSkillInfoTable = new MSkillInfoTable;
	ofstream  out("Skill_dk2.inf",ios::binary);
	m_pSkillInfoTable->SaveToFile(out);
	/*
	//LAOD FILE
	class ifstream skillTable2;
	if (!FileOpenBinary(filename, skillTable2))
		return;
	m_pSkillInfoTable->LoadFromFile(skillTable2);
	skillTable2.close();*/
	//////////////////////////////////////////////////////////////////////////
	MSkillInfoTable *m_pSkillInfoTable2=NULL;
	if (m_pSkillInfoTable2 != NULL)
	{
		delete m_pSkillInfoTable2;
		m_pSkillInfoTable2 = NULL;
	}

	m_pSkillInfoTable2 = new MSkillInfoTable;
	ofstream  out2("SkillInfo_dk2.inf",ios::binary);
	m_pSkillInfoTable2->SaveToFile(out2);
	/*
	//LAOD FILE
	class ifstream skillTable3;
	if (!FileOpenBinary(filename, skillTable3))
		return;
	m_pSkillInfoTable2->LoadFromFile(skillTable3);
	skillTable2.close();*/
}
