// SkillFileManager.h: interface for the CSkillFileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLFILEMANAGER_H__1D994268_F217_4124_B9D6_8410F2DF1795__INCLUDED_)
#define AFX_SKILLFILEMANAGER_H__1D994268_F217_4124_B9D6_8410F2DF1795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSkillFile
{
public:
	CSkillFile();
	virtual ~CSkillFile();
	int LoadFromFile(CFile *file);
protected:
private:
};


class CSkillFileManager  
{
public:
	CSkillFileManager();
	virtual ~CSkillFileManager();

public:
	//读取文件
	int LoadFromFile(char* FileName);
	//获取数据
	CSkillFile* GetTable(int n){	if (n<m_size){return &m_pTable[n];}return NULL;}
	//获取大小
	int GetSize(){return m_size;}
public:
	//技能类型
	CSkillFile* m_pTable;
	//数量
	int			m_size;
};

#endif // !defined(AFX_SKILLFILEMANAGER_H__1D994268_F217_4124_B9D6_8410F2DF1795__INCLUDED_)
