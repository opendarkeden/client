#pragma once
typedef struct sSkillOUSTERS
{
	int SkillPoint;     //技能点
	int LevelUpPoint;   //升级技能点
	int szSkill;        //技能长度
	int * Skilllist;    //技能列表
	int Fire;           //火
	int Water;			//水
	int Earth;			//土
	int Wind;			//风
	int Sum;			//合计
	int Wristlet;		//
	int Stone1;
	int Stone2;
	int Stone3;
	int Stone4;
	byte CanDelete;


	sSkillOUSTERS()
	{
		SkillPoint=0;
		LevelUpPoint=0;
		szSkill=0;
		Skilllist=NULL;
		Fire=0;
		Water=0;
		Earth=0;
		Wind=0;
		Sum=0;
		Wristlet=0;
		Stone1=0;
		Stone2=0;
		Stone3=0;
		Stone4=0;
		CanDelete=0;
	}
	~sSkillOUSTERS()
	{
		if (Skilllist !=NULL)
		{
			delete[] Skilllist;
		}
	}

}sSkillOUSTERS,*lpsSkillOUSTERS;
typedef struct sSkillnode{
	int skillID;      //技能ID
	int LearnLevel;   //学习等级
	int DomainType;   //技能类型
	DWORD dwNameLen;  //技能名称长度
	char* name;       //技能名称
	DWORD dwHnameLen; //技能中文名称长度
	char* Hname;      //技能中文名称
	int minDamage;    //技能最小伤害
	int maxDamage;    //技能最大伤害
	int minDelay;     //技能最小延时
	int maxDelay;     //技能最大延时
	int minDuration;  //技能最小持续时间
	int maxDuration;  //技能最大持续时间
	int mp;           //技能消耗MP
	int minRange;     //技能最小使用距离
	int maxRange;     //技能最大使用距离
	int maxExp;       //技能最大经验
	int ElementalDomain;//土，水，气，火类型
	lpsSkillOUSTERS m_sSkillOUSTERS; //魔灵
	
	
	sSkillnode()
	{	
		skillID=0;
		LearnLevel=0;
		DomainType=0;
		dwNameLen=0;
		name=NULL;
		dwHnameLen=0;
		Hname=NULL;
		minDamage=0;
		maxDamage=0;
		minDelay=0;
		maxDelay=0;
		minDuration=0;
		maxDuration=0;
		mp=0;
		minRange=0;
		maxRange=0;
		maxExp=0;
		ElementalDomain=-1;
		m_sSkillOUSTERS = NULL;

	}
	~sSkillnode()
	{
		if(name!=NULL)
		{
			delete[] name;
		}
		if (Hname!=NULL)
		{
			delete[] Hname;
		}
	}

}sSkillnode,*lpsSkillnode;
typedef struct sSkillnodeString
{
	int skillID;
	int LearnLevel;
	int DomainType;
	DWORD dwNameLen;
	CString name;
	DWORD dwHnameLen;
	CString Hname;
	int minDamage;
	int maxDamage;
	int minDelay;
	int maxDelay;
	int minDuration;
	int maxDuration;
	int mp;
	int minRange;
	int maxRange;
	int maxExp;
	int ElementalDomain;

	lpsSkillOUSTERS m_sSkillOUSTERS;

	sSkillnodeString()
	{	
		skillID=0;
		LearnLevel=0;
		DomainType=0;
		dwNameLen=0;
		dwHnameLen=0;
		minDamage=0;
		maxDamage=0;
		minDelay=0;
		maxDelay=0;
		minDuration=0;
		maxDuration=0;
		mp=0;
		minRange=0;
		maxRange=0;
		maxExp=0;
		ElementalDomain=-1;
		m_sSkillOUSTERS = NULL;
	}
	~sSkillnodeString()
	{
		if (m_sSkillOUSTERS)
		{	
			delete m_sSkillOUSTERS;
		}
	}
}sSkillnodeString,*lpsSkillnodeString;
class CSkillinfo
{
public:
	CSkillinfo(void);
	~CSkillinfo(void);
	
	bool OpenFromFile(CString FileName);
	lpsSkillnode Getnode(int num);
	int NumofSkill;
private:
	lpsSkillnode m_lpsSkillnode;
	lpsSkillnode m_curlpsSkillnode;
	CString m_FileName;
public:
	// 保存到文件
	bool SavetoFile(void);
};
