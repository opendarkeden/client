// MySkillInfo.cpp: implementation of the CMySkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "infLoad.h"
#include "MySkillInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySkillInfo::CMySkillInfo()
{
	Skilltype = NULL;
}

CMySkillInfo::~CMySkillInfo()
{
	if (Skilltype != NULL)
	delete[] Skilltype;
}

void CMySkillInfo::SaveOneToFile(SKILLINFO_NODE *TypeInfo,class ofstream &file)
{
	MString hname,name;

	hname.Format("%s",TypeInfo->GetHName());
	name.Format("%s",TypeInfo->GetName());
	DWORD DWTemp = TypeInfo->GetLearnLevel();

	if (hname == "Not Defined")
	{	
		DWTemp = 0x64;
		hname.Format("%s","EmptySkill");
		name.Format("%s","Empty Skill");
	}

	file.write((const char *)&DWTemp,4);
	file.write((const char *)&TypeInfo->DomainType,4);

	name.SaveToFile(file);
	hname.SaveToFile(file);

	file.write((const char *)&TypeInfo->minDamage,4);
	file.write((const char *)&TypeInfo->maxDamage,4);
	file.write((const char *)&TypeInfo->minDelay,4);
	file.write((const char *)&TypeInfo->maxDelay,4);

	file.write((const char *)&TypeInfo->minDuration,4);
	file.write((const char *)&TypeInfo->maxDuration,4);
	
	DWTemp = TypeInfo->GetMP();
	file.write((const char *)&DWTemp,4);
	file.write((const char *)&TypeInfo->minRange,4);
	file.write((const char *)&TypeInfo->maxRange,4);

	file.write((const char *)&TypeInfo->maxExp,4);

	if (TypeInfo->DomainType == SKILLDOMAIN_OUSTERS)
	{
		file.write((const char *)&TypeInfo->SkillPoint,sizeof(int));
		file.write((const char *)&TypeInfo->LevelUpPoint,sizeof(int));
		DWTemp = TypeInfo->SkillTypeList.size();
		file.write((const char *)&DWTemp,sizeof(int));

		SKILLINFO_NODE::SKILLTYPE_LIST::const_iterator skilid = TypeInfo->SkillTypeList.begin();
	
		while (skilid != TypeInfo->SkillTypeList.end())
		{
			int idinfo = *skilid;

			file.write((const char *)&idinfo,4);

			skilid++;
		}

		file.write((const char *)&TypeInfo->Fire,sizeof(int));
		file.write((const char *)&TypeInfo->Water,sizeof(int));
		file.write((char*)&TypeInfo->Earth,			sizeof(int));
		file.write((char*)&TypeInfo->Wind,				sizeof(int));
		file.write((char*)&TypeInfo->Sum,				sizeof(int));
		file.write((char*)&TypeInfo->Wristlet,			sizeof(int));
		file.write((char*)&TypeInfo->Stone1,			sizeof(int));
		file.write((char*)&TypeInfo->Stone2,			sizeof(int));
		file.write((char*)&TypeInfo->Stone3,			sizeof(int));
		file.write((char*)&TypeInfo->Stone4,			sizeof(int));
		file.write((char*)&TypeInfo->ElementalDomain,	sizeof(int));
		file.write((char*)&TypeInfo->CanDelete,		sizeof(BYTE));
	}
}

void CMySkillInfo::SaveAllToFile(class ofstream &file)
{
	file.write((const char *)&dwSize,4);

	for (int i = 0;i < dwSize;i++)
	{
		file.write((char*)&Skilltype[i],4);

		SaveOneToFile(&m_pTypeInfo[Skilltype[i]],file); 
	}
}

void CMySkillInfo::LoadSKILLInfoFile(class ifstream& file)
{
	int num;
	
	file.read((char*)&num, 4);

	Skilltype = new int[num];

	dwSize = num;
	ZeroMemory(Skilltype,num * 4);

	for (int i=0; i<num; i++)
	{
		file.read((char*)&Skilltype[i], 4);

		// 적절한 skillType에 loading한다.
		m_pTypeInfo[Skilltype[i]].LoadFromFileServerSkillInfo( file );
	}

	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].minRange = 8;		// 최소 사정거리
	m_pTypeInfo[MAGIC_THROW_HOLY_WATER].maxRange = 8;		// 최소 사정거리

	m_pTypeInfo[BOMB_SPLINTER].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_SPLINTER].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[BOMB_ACER].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_ACER].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[BOMB_BULLS].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_BULLS].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[BOMB_STUN].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_STUN].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[BOMB_CROSSBOW].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_CROSSBOW].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[BOMB_TWISTER].minRange = 6;		// 최소 사정거리
	m_pTypeInfo[BOMB_TWISTER].maxRange = 6;		// 최소 사정거리

	m_pTypeInfo[MAGIC_RAPID_GLIDING].minRange = 2;
	m_pTypeInfo[MAGIC_RAPID_GLIDING].maxRange = 6;
	
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].minRange = 1;
	m_pTypeInfo[SKILL_ULTIMATE_BLOW].maxRange = 3;
}