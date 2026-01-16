#ifndef __FAME_INFO_H__
#define __FAME_INFO_H__

#include "CTypeTable.h"

#include <fstream>
using namespace std;


typedef unsigned char	BYTE;

class FameInfo
{
public :
	FameInfo();
	~FameInfo();
	
	void	LoadFromFile(std::ifstream& file);
	void	SaveToFile(std::ofstream& file);
	DWORD	GetFame( SKILLDOMAIN type );

private :	
	BYTE m_level;
	BYTE m_domaintype;
	DWORD m_fame;
};

class FameInfoTable : public CTypeTable<FameInfo>
{
public :
	
	FameInfoTable();

	int		GetFameForLevel(SKILLDOMAIN type, int level);
	
};

extern FameInfoTable *g_pFameInfoTable;

#endif
