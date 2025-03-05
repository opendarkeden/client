//-----------------------------------------------------------------------------
// MInternetConnection.cpp
//-----------------------------------------------------------------------------

#include "MInternetConnection.h"
#include "Shlwapi.h"

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define BUFFER_SIZE			4096

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
char		MInternetConnection::m_LastInfo[INFO_BUFFER_SIZE];

//-----------------------------------------------------------------------------
//
//								MInternetConnection
//
//-----------------------------------------------------------------------------
MInternetConnection::MInternetConnection(const char* pAppName, DWORD dwFlag)
{
	m_hInternet = NULL;

	if (pAppName!=NULL)
	{
		InitConnection(pAppName, dwFlag);
	}
}

MInternetConnection::~MInternetConnection()
{
	ReleaseConnection();
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
MInternetConnection::ReleaseConnection()
{
	if (m_hInternet!=NULL)
	{
		InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}
}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
BOOL
MInternetConnection::InitConnection(const char* pAppName, DWORD dwFlag)
{
	ReleaseConnection();

	m_hInternet = InternetOpen(TEXT("pAppName"),
								INTERNET_OPEN_TYPE_PRECONFIG,
								NULL, 
								NULL, 
								dwFlag);

	if (m_hInternet==NULL)
	{
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Set Option
//-----------------------------------------------------------------------------
BOOL
MInternetConnection::SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
{
	if (InternetSetOption(m_hInternet,
							dwOption,
							lpBuffer,
							dwBufferLength))
	{
		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------
// Query Option
//-----------------------------------------------------------------------------
BOOL
MInternetConnection::QueryOption(DWORD dwOption, LPVOID lpBuffer, DWORD& dwBufferLength)
{
	if (InternetQueryOption(m_hInternet,
							dwOption,
							lpBuffer,
							&dwBufferLength))
	{
		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// GetLastResponseInfo
//-----------------------------------------------------------------------------
const char*
MInternetConnection::GetLastResponseInfo()
{
	DWORD dwError = 0;
	DWORD dwBufferLength = INFO_BUFFER_SIZE;
	m_LastInfo[0] = '\0';

	if (InternetGetLastResponseInfo(&dwError,
									m_LastInfo,
									&dwBufferLength))
	{		
		// -_-;
	}

	return m_LastInfo;
}


//-----------------------------------------------------------------------------
//
//							MInternetFile
//
//-----------------------------------------------------------------------------
MInternetFile::MInternetFile(const char* pURL, const char* pLocalName)
{
	m_hFile = NULL;
	m_pLocalName = NULL;
	m_nReceived = 0;

	if (pURL!=NULL && pLocalName!=NULL)
	{
		InitConnection();
		Init( pURL, pLocalName );
	}
}
		
MInternetFile::~MInternetFile()
{	
	Release();
	ReleaseConnection();
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void		
MInternetFile::Release()
{
	Flush();

	if (m_pLocalName!=NULL)
	{
		delete [] m_pLocalName;
		m_pLocalName = NULL;
	}
	
	m_nReceived = 0;
}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
BOOL		
MInternetFile::Init(const char* pURL, const char* pLocalName)
{
	Release();

	if (pURL!=NULL
		&& PathIsURL(pURL)
		&& pLocalName!=NULL
		&& (IsInitConnection() || MInternetConnection::InitConnection()))
	{	
		m_hFile = InternetOpenUrl(m_hInternet,
									pURL,
									NULL, 
									0,
									INTERNET_FLAG_RESYNCHRONIZE, //INTERNET_FLAG_NEED_FILE,
									0);
	
		//---------------------------------------------------------------------
		// URL에 file이 제대로 있는지 확인..
		//---------------------------------------------------------------------
		// local dir에 생성되는지를 보면 된다. --;;
		char lpBuffer[1024];
		DWORD dwBufferLength = 1024;

		if (!InternetQueryOption(m_hFile, 
								INTERNET_OPTION_DATAFILE_NAME, 
								(void*)lpBuffer, 
								&dwBufferLength))
		{
			if (m_hFile!=NULL)
			{
				InternetCloseHandle( m_hFile );
				m_hFile = NULL;
			}

			return FALSE;
		}

		if (m_hFile!=NULL)
		{
			m_pLocalName = new char [strlen(pLocalName) + 1];
			strcpy(m_pLocalName, pLocalName);

			m_LocalFile.open( m_pLocalName, std::ios::binary | std::ios::trunc );			

			return TRUE;
		}

	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------	
// 읽은게 없다면 false를 return한다.
//
// 화일 size를 몰라서 좀 애매하네.. - -;
//-----------------------------------------------------------------------------	
BOOL
MInternetFile::Update()
{
	if (m_hFile)
	{
		//const char* url = "http://211.219.153.53/PatchExec.exe";
		//const char* filename = "PatchExec.exe";	

		char pBuffer[BUFFER_SIZE];
		DWORD nRead;

		InternetReadFile( m_hFile, pBuffer, BUFFER_SIZE, &nRead );
		m_nReceived += nRead;

		const char*	str = GetLastResponseInfo();

		if (nRead==0)
		{
			return FALSE;
		}
		
		m_LocalFile.write((const char*)&pBuffer, nRead);

		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------	
// Flush
//-----------------------------------------------------------------------------	
void
MInternetFile::Flush()
{
	if (m_LocalFile.is_open())
	{
		m_LocalFile.close();		
	}
	
	if (m_hFile!=NULL)
	{
		InternetCloseHandle( m_hFile );
		m_hFile = NULL;
	}
}
