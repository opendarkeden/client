//--------------------------------------------------------------------------
// PackFileManager.cpp
//--------------------------------------------------------------------------
#include "Client_PCH.h"
#include "PackFileManager.h"


//--------------------------------------------------------------------------
//
//							PackFileInfo
//
//--------------------------------------------------------------------------
PackFileInfo::PackFileInfo(DWORD id, const char* pFilename)
{
	m_ID		= id;

	if (pFilename!=NULL)
	{
		m_Filename	= pFilename;
	}

	m_FP		= 0;
}

PackFileInfo::~PackFileInfo()
{
}

//--------------------------------------------------------------------------
// Save To File
//--------------------------------------------------------------------------
void				
PackFileInfo::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&m_ID, 4);
	
	BYTE len = m_Filename.size();
	file.write((const char*)&len, 1);
	if (len>0)
	{
		file.write((const char*)m_Filename.c_str(), len);
	}

	file.write((const char*)&m_FP, 4);
}

//--------------------------------------------------------------------------
// Load From File
//--------------------------------------------------------------------------
void				
PackFileInfo::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&m_ID, 4);
	
	BYTE len;
	file.read((char*)&len, 1);

	if (len>0)
	{
		char buf[256];
		file.read((char*)buf, len);
		m_Filename = buf;
	}

	file.read((char*)&m_FP, 4);
}

//--------------------------------------------------------------------------
// Save To File Data
//--------------------------------------------------------------------------
bool		
PackFileInfo::SaveToFileData(std::ofstream& file)
{
	return true;
}