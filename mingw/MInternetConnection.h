//-----------------------------------------------------------------------------
// MInternetConnection.h
//-----------------------------------------------------------------------------
//
// < 사용하는 lib들 >
//
//   WinINet.lib
//   Shlwapi.lib - PathIsURL() 때문에 T_T;
//
//
// < MInternetConnection >
//							: internet에 연결한다.
//
//
// < MInternetFile >
//							: internet에서 file을 가져온다.
//
//-----------------------------------------------------------------------------

#ifndef __MINTERNET_CONNECTION_H__
#define __MINTERNET_CONNECTION_H__

#include <Windows.h>
#include <WinINet.h>
#include <fstream>


#define INFO_BUFFER_SIZE		1024


//-----------------------------------------------------------------------------
// MInternetConnection
//-----------------------------------------------------------------------------
class MInternetConnection {
	public :
		MInternetConnection(const char* pAppName=NULL, DWORD dwFlag=0);
		virtual ~MInternetConnection();

		BOOL				InitConnection(const char* pAppName="MInternetConnectionApp", DWORD dwFlag=0);
		void				ReleaseConnection();

		BOOL				IsInitConnection() const		{ return m_hInternet!=NULL; }

		virtual BOOL		Update() = 0;

		// option
		BOOL				SetOption(DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);
		BOOL				QueryOption(DWORD dwOption, LPVOID lpBuffer, DWORD& dwBufferLength);

		static const char*	GetLastResponseInfo();

	protected :
		HINTERNET			m_hInternet;
		static char			m_LastInfo[INFO_BUFFER_SIZE];
};

//-----------------------------------------------------------------------------
// MInternetFile
//-----------------------------------------------------------------------------
class MInternetFile : public MInternetConnection {
	public :
		MInternetFile(const char* pURL=NULL, const char* pLocalName=NULL);
		virtual ~MInternetFile();

		virtual BOOL		Init(const char* pURL, const char* pLocalName);
		virtual void		Release();
		
		virtual BOOL		Update();

		// Get
		const char*			GetFilename() const			{ return m_pLocalName; }
		DWORD				GetReceivedSize() const		{ return m_nReceived; }

		// Flush
		virtual void		Flush();

	protected :
		HINTERNET			m_hFile;
		char*				m_pLocalName;
		std::ofstream		m_LocalFile;
		DWORD				m_nReceived;
};

#endif
