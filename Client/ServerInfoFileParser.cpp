#include "Client_PCH.h"
#include "ServerInfoFileParser.h"
#include "Mintr.h"

using namespace std;
ServerInfoFileParser * g_pConfigForeign = NULL;


ServerInfoFileParser::ServerInfoFileParser(std::string infofilename)
{
	m_FileName = infofilename;
}

ServerInfoFileParser::~ServerInfoFileParser()
{
}

std::string			ServerInfoFileParser::getProperty(int dimension, std::string key)
{
	std::ifstream file( m_FileName.c_str(), ios::in );

	bool bStart = false;
	int dim=0;
	
	std::string ukey = key;
	char sztemp[2048];
	ukey += ":";
		
	while( !file.eof() )
	{
		std::string line;
		memset(sztemp,0,2048);
		file.getline( sztemp, 2048 );
			
		line = sztemp;
		while(1)
		{
			int pos = line.find(" ");
			if(pos != -1)
			{
				line.erase( pos, 1 );
			}
			else
				break;
		}

		if( line.empty() || line.c_str()[0] == '#')
			continue;

		if( line.c_str()[0] == '@' )
		{
			bStart = !bStart;
			if( !bStart )
				dim++;				
		}
		
		if( dim == dimension && bStart )
		{
			int pos = line.find( ukey );
			if( pos == 0 )
			{
				pos = line.find(":");
				if( pos == -1 )
					continue;
				
				string ret = line.c_str()+pos+1;
				file.close();
				return  ret;
			}
		}
	}
	
	file.close();
	//MessageBox( NULL, key.c_str(), "Cannot find key",MB_OK);
	return "";
}

int					ServerInfoFileParser::getPropertyInt( int dimension, std::string key)
{
	string re = getProperty( dimension, key );
	if (re.empty() ) return -1;
	
	return atoi( re.c_str() );
}