// MySkillInfo.h: interface for the CMySkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSKILLINFO_H__63533D1B_A545_460C_9190_474837F2C3DB__INCLUDED_)
#define AFX_MYSKILLINFO_H__63533D1B_A545_460C_9190_474837F2C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CLIENT\MSkillManager.h"

class CMySkillInfo : public MSkillInfoTable  
{
public:
	CMySkillInfo();
	virtual ~CMySkillInfo();
public:
	void SaveAllToFile(class ofstream &file);
	void SaveOneToFile(SKILLINFO_NODE *TypeInfo,class ofstream &file);
	void LoadSKILLInfoFile(class ifstream& file);
	int dwSize;
private:
	int *Skilltype;
};

#endif // !defined(AFX_MYSKILLINFO_H__63533D1B_A545_460C_9190_474837F2C3DB__INCLUDED_)
