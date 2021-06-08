// SkillAdd.h: interface for the SkillAdd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLADD_H__CE7A35A3_15E9_4A3E_9FF7_DFA8A066571F__INCLUDED_)
#define AFX_SKILLADD_H__CE7A35A3_15E9_4A3E_9FF7_DFA8A066571F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SkillAdd  
{
public:
	MSkillInfoTable* m_pSkillInfoTable;
	void LoadFile(const char* filename);
	SkillAdd();
	virtual ~SkillAdd();

};

#endif // !defined(AFX_SKILLADD_H__CE7A35A3_15E9_4A3E_9FF7_DFA8A066571F__INCLUDED_)
