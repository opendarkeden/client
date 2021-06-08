#pragma once
typedef struct sSkillnode 
{	
	CString Name;
	int Namelen;
	CString HName;
	int HNamelen;
	int Level;
	int x;
	int y;
	short SpriteID;
	int MP;
	unsigned char Passive;
	unsigned char Active;
	unsigned char skillStep;
	int idNum;
	unsigned short * ACTIONINFO_idlist;

	sSkillnode()
	{
		Namelen = 0;
		HNamelen = 0;
		Level = 0 ;
		x=0;
		y=0;
		SpriteID=0;
		MP=0;
		Passive=0;
		Active=0;
		idNum=0;
		ACTIONINFO_idlist = NULL;
	}

	~sSkillnode()
	{

		if (ACTIONINFO_idlist)
		{	
			delete[] ACTIONINFO_idlist;
		}
	}


}sSkillnode,*lpsSkillnode;

class CSkillFile
{
public:
	CSkillFile(void);
	~CSkillFile(void);
	bool readFromFile(CString FileName);
	lpsSkillnode getSkillnode(int num){return &m_lpsSkillnode[num];}
	int numberofskill;

private:
	CString m_filename;
	lpsSkillnode m_lpsSkillnode;
public:
	// ´æ´¢µ½ÎÄ¼þ
	bool SaveToFile(void);
};

