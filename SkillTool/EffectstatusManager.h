// EffectstatusManager.h: interface for the CEffectstatusManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_EFFECTSTATUSMANAGER_H__E2E376DF_501B_41FD_B040_773B880C3C6F__INCLUDED_)
#define AFX_EFFECTSTATUSMANAGER_H__E2E376DF_501B_41FD_B040_773B880C3C6F__INCLUDED_

#include "../Client/MTypeDef.h"
#include "../Client/EffectSpriteTypeDef.h"
#include "../SkillDef.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEffectstatus
{
public:
	int LoadFromFile(CFile *file);
	CEffectstatus();
	virtual ~CEffectstatus();
public:
	bool						bUseEffectSprite;		// 是否使用EffectSprite效果
	bool						bAttachGround;			// 一般为默认值
	TYPE_EFFECTSPRITETYPE		EffectSpriteType;		// ENUM_EFFECTSPRITETYPE 类型文件 EffectSpriteType.h
	WORD						EffectColor;			// 效果颜色
	ADDON						EffectColorPart;		// 效果部位
	TYPE_ACTIONINFO				ActionInfo;				// ACTIONINFO 动作类型   类型文件 SkillDef.h  
	TYPE_ACTIONINFO				OriginalActionInfo;		// ACTIONINFO 动作类型   类型文件 SkillDef.h 
	int							SoundID;				// SoundID
private:
};


class CEffectstatusManager  
{
public:

	//构造函数
	CEffectstatusManager();
	//析解函数
	virtual ~CEffectstatusManager();
public:
	//读取文件
	int LoadFromFile(char* FileName);
	//获取数据
	CEffectstatus* GetTable(int n){	if (n<m_size){return &m_pTable[n];}return NULL;}
	//获取大小
	int GetSize(){return m_size;}

private:
	int m_size;
	CEffectstatus* m_pTable;

};

#endif // !defined(AFX_EFFECTSTATUSMANAGER_H__E2E376DF_501B_41FD_B040_773B880C3C6F__INCLUDED_)
