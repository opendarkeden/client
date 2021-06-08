// MEFFECTSPRITETYPETABLE_INFO.h: interface for the MEFFECTSPRITETYPETABLE_INFO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEFFECTSPRITETYPETABLE_INFO_H__26830D52_AD83_4A21_BEDE_FC0B06755DC0__INCLUDED_)
#define AFX_MEFFECTSPRITETYPETABLE_INFO_H__26830D52_AD83_4A21_BEDE_FC0B06755DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum BLT_TYPE 
{
	BLT_NORMAL = 0,
	BLT_EFFECT,
	BLT_SHADOW,
	BLT_SCREEN
};
typedef unsigned short TYPE_FRAMEID;
typedef unsigned short TYPE_EFFECTSPRITETYPE;

class CEffectSpriteType  
{
public:
	CEffectSpriteType();
	virtual ~CEffectSpriteType();
public:
	int LoadFromFile(CFile* file);
public:
		BLT_TYPE					BltType;				// 免仿屈侥
		unsigned short				FrameID;				// Frame ID	
		bool						RepeatFrame;			// 馆汗登绰 frame牢啊?
		unsigned short				ActionEffectFrameID;	// action 付促 促弗 frame阑 焊咯拎具窍绰 effect牢啊?
		//TYPE_FRAMEID				PairFrameID;			// 悼矫俊 免仿登绢具 窍绰 娄~捞促.
		WORD*						PairFrameIDList;		// 悼矫俊 免仿登绢具 窍绰 捞棋飘甸~捞促.
		// add by coffee
		int							PairFrameIDListNum;     //PairFrameIDlist数量
		// end 
		bool						bPairFrameBack;			// 悼矫俊 免仿登绰 捞棋飘啊 第俊 免仿登绰啊
		unsigned short				FemaleEffectSpriteType;	// 咯磊侩..
};


class CEffectSpriteTypeManager
{
public:
	CEffectSpriteTypeManager();
	virtual ~CEffectSpriteTypeManager();
public:
	//获取数据
	CEffectSpriteType* GetEffectTable(int id)	{ if (id>m_size) { return NULL; } return &m_pEffectTab[id];	}
	//获取表格大小
	int GetSize(){ return m_size;}
	//读取文件
	int LoadFromFile(char* FileName);
protected:
private:
	//保存表格数据
	CEffectSpriteType* m_pEffectTab;
	//所有数据
	int m_size;

};
#endif // !defined(AFX_MEFFECTSPRITETYPETABLE_INFO_H__26830D52_AD83_4A21_BEDE_FC0B06755DC0__INCLUDED_)
