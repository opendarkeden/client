
#include "Client_PCH.h"
#include "ShrineInfoManager.h"
#include "RarFile.h"
#ifdef __GAME_CLIENT__
#include "Properties.h"
#endif

RegenTowerInfoManager *g_pRegenTowerInfoManager = NULL;

void RegenTowerInfo::LoadFromLine(char *szLine)
{
	if( szLine == NULL )
		return;

	sscanf( szLine,"%d %d %d %d",&num, &zoneID,&x,&y);
	owner = -1;
}

RegenTowerInfoManager::RegenTowerInfoManager()
{
}

bool RegenTowerInfoManager::LoadRegenTowerInfo()
{
	CRarFile rarfile;
	
#ifdef __GAME_CLIENT__
	rarfile.SetRAR( g_pFileDef->getProperty("FILE_INFO_DATA").c_str(), "darkeden" );
	rarfile.Open( g_pFileDef->getProperty("FILE_REGEN_TOWER_INFO").c_str() );
#else
	rarfile.SetRAR( "data\\info\\infodata.rpk", "darkeden" );
	rarfile.Open( "RegenTowerPosition.inf" );
#endif
	
	if( !rarfile.IsSet() )
		return false;

	char szLine[512];
	bool bInit = false;
	
	while( rarfile.GetString( szLine, 512 ) )
	{
		if( szLine[0] == ';' )
			continue;

		if( szLine[0] == '*' && bInit == false )
		{
			int n;
			sscanf(szLine+1,"%d",&n);

			Init( n );
			bInit = true;
			continue;
		}
		
		if( strlen(szLine) <= 0 )
			continue;
		
		int num;
		sscanf(szLine,"%d",&num);

		if( num >= 0 && num < GetSize() )
			m_pTypeInfo[num].LoadFromLine( szLine );
	}
	rarfile.Release();
	return true;
}
