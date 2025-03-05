#pragma once



class ServerInfoFileParser
{
public :
	ServerInfoFileParser(std::string infofilename);
	~ServerInfoFileParser();

	std::string		getProperty(int dimension, std::string key);
	int				getPropertyInt( int dimension, std::string key);

private :
	std::string						m_FileName;
//	std::vector< std::string >		m_LineList;
};

extern ServerInfoFileParser * g_pConfigForeign;