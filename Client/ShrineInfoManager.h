#pragma once

#pragma warning(disable:4786)

#include "CTypeTable.h"
#include <vector>

class RegenTowerInfo
{
public :
	RegenTowerInfo() { zoneID = -1; x=0,y=0; owner = -1; }
	~RegenTowerInfo() {}
	void	LoadFromLine(char *szLine);

	void	LoadFromFile(class ifstream& file) { }
	void	SaveToFile(class ofstream& file) { }
	
	int		num;
	int		zoneID;	
	int		x,y;
	int		owner;
};

class RegenTowerInfoManager : public CTypeTable< RegenTowerInfo >
{
public :	
	RegenTowerInfoManager();

	bool			LoadRegenTowerInfo();
	
private :
};

// Singleton pattern 인터페이스는 어디있는거야~~
extern RegenTowerInfoManager *g_pRegenTowerInfoManager;
