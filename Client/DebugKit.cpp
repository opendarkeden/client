// DebugKit.cpp: implementation of the CDebugKit class.
//
//////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "stdafx.h"
#include "DebugKit.h"

#include <wtypes.h>
#include "fstream"

extern BOOL g_bMsgOutPutFlag;
extern BOOL g_bMsgDetailFlag;
extern BOOL g_bMsgContentFlag;
extern CMessageStringTable g_MessageStringTable;
extern BYTE g_nKeyMapSelect;
extern int g_nGameVersion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugKit::CDebugKit()
{
	m_DebugInfo = new Properties;
}

CDebugKit::~CDebugKit()
{
	if(m_DebugInfo!=NULL)
		delete m_DebugInfo;
}

BOOL CDebugKit::load()
{
	try{
		m_DebugInfo->load("DebugInfo.ini");
	}
	catch (Error) {
		return FALSE;
	}
	catch (IOException) {
		return FALSE;
	}
	return TRUE;
}

std::string CDebugKit::GetMsgFileName()
{
	try{
		return m_DebugInfo->getProperty("MsgFileName");
	}
	catch (NoSuchElementException) {
		return NULL;
	}
}

BOOL CDebugKit::GetMsgOutPutFlag()
{
	try{
		return ((m_DebugInfo->getPropertyInt("MsgOutPutFlag")>0)?TRUE:FALSE);
	}
	catch (NoSuchElementException) {
		return FALSE;
	}
}

BOOL CDebugKit::GetMsgDetailFlag()
{
	try{
		return ((m_DebugInfo->getPropertyInt("MsgDetailFlag")>0)?TRUE:FALSE);
	}
	catch (NoSuchElementException) {
		return FALSE;
	}
}

BOOL CDebugKit::GetMsgContentFlag()
{
	try{
		return ((m_DebugInfo->getPropertyInt("MsgContentFlag")>0)?TRUE:FALSE);
	}
	catch (NoSuchElementException) {
		return FALSE;
	}
}

BYTE CDebugKit::GetAuthKeyMap()
{
	try{
		return m_DebugInfo->getPropertyInt("AuthKeyMap");
	}
	catch (NoSuchElementException) {
		return FALSE;
	}
}
int CDebugKit::GetGameVersion()
{
	try{
		return m_DebugInfo->getPropertyInt("GameVersion");
	}
	catch (NoSuchElementException) {
		return 0;
	}
}

const char *CMessageStringTable::GetMessageName(WORD MessageID)
{
	if(IsExistMessageName(MessageID))
		return m_MessageName[MessageID].c_str();
	else
		return "UNKNOWN_MESSAGE";
}

CMessageStringTable::CMessageStringTable()
{
}
CMessageStringTable::~CMessageStringTable()
{
}
void CMessageStringTable::LoadFromFile(std::string strFileName)
{
	int i=0;
	std::ifstream ifile( strFileName.c_str() , std::ios::in );
//	std::ifstream ifile( "MessageDefine.ini" , ios::in );
	if ( ! ifile )
		return;	
	while ( true ) {
		
		std::string line;
		std::getline( ifile , line );
		
		if ( ifile.eof() )
			break;
		uint key_begin = line.find_first_not_of( " \t" );
		if ( key_begin == std::string::npos )
			continue;
		uint key_end = line.find( "," , key_begin );
		std::string key = line.substr( key_begin , key_end - key_begin );
		SetMessageName(i++,key);
	}		
	ifile.close();
}
void CMessageStringTable::SaveToFile(std::string strFileName)
{

}

#ifdef DEBUG_INFO
BOOL InitDebugInfo()
{
	CDebugKit *p_dKit = new CDebugKit();
	if(p_dKit->load() == FALSE)
		return FALSE;

	g_bMsgOutPutFlag = p_dKit->GetMsgOutPutFlag();
	g_bMsgContentFlag = p_dKit->GetMsgContentFlag();
	g_bMsgDetailFlag = p_dKit->GetMsgDetailFlag();

	g_nKeyMapSelect = p_dKit->GetAuthKeyMap();
	g_nGameVersion = p_dKit->GetGameVersion();
	std::string strFileName;
	strFileName = p_dKit->GetMsgFileName();
	
	g_MessageStringTable.LoadFromFile(strFileName);

	const char *p_cName = g_MessageStringTable.GetMessageName(100);
	delete p_dKit;
	return TRUE;
}

void ClearDebugInfo()
{
	g_MessageStringTable.ClearMessageName();
}

void PrintMessageDetail(ofstream file, char *strMsg, int length)
{
	char temp[1024];
	int i,j=0;
	memset(temp,0,sizeof(temp));
	j += sprintf(temp+j,"\t");
	for(i=0; i<length; i++)
	{
		j += sprintf(temp+j,"%02x ",(BYTE)strMsg[i]);
		if((i+1)%16 == 0 && i+1 != length)
			j += sprintf(temp+j,"\n\t");
	}
	j += sprintf(temp+j,"\0");
	file << temp << endl;
}
#endif