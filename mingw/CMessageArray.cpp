//----------------------------------------------------------------------
// CMessageArray.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CMessageArray.h"
#include "DebugInfo.h"

#ifdef OUTPUT_DEBUG
	//#define OUTPUT_FILE_LOG
#endif

#define __LOGGING__

#if defined(OUTPUT_DEBUG) && defined(__GAME_CLIENT__)
	CRITICAL_SECTION			g_Lock;

	#define __BEGIN_LOCK	EnterCriticalSection(&g_Lock);
	#define __END_LOCK		LeaveCriticalSection(&g_Lock);
#else
	#define __BEGIN_LOCK	((void)0);
	#define __END_LOCK		((void)0);
#endif


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
CMessageArray::CMessageArray()
{
	m_Max		= 0;
	m_Length	= 0;
	m_Current	= 0;
	m_ppMessage = NULL;

	m_bLog		= false;
	m_LogFile	= 0;
	m_Filename	= NULL;
}

CMessageArray::~CMessageArray()
{
	Release();
}


//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
CMessageArray::Init(int max, int length, const char* filename)
{
	// 일단 메모리 제거..
	Release();

	m_Max		= max;
	m_Length	= length;
	m_Current	= 0;

	// new
	m_ppMessage = new char* [ m_Max ];

	for (int i=0; i<m_Max; i++)
	{
		m_ppMessage[i] = new char [ m_Length+1 ];
		m_ppMessage[i][0] = NULL;
	}

	// file Log
	if (filename!=NULL)
	{
		// filename을 기억해둔다.
		m_Filename = new char [strlen(filename)+1];
		strcpy(m_Filename, filename);

		m_LogFile = _open(filename, _O_WRONLY | _O_TEXT | _O_CREAT | _O_TRUNC);

		if (m_LogFile!=-1)
		{
			m_bLog = true;
		}
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CMessageArray::Release()
{
	// 자꾸 여기서 에러나서리..
	// 음냐.. 도대체 어디서 문제가 생기는걸까.
	// 못찾겠다.. 아이고..
	//#ifndef _DEBUG
		if (m_ppMessage!=NULL)
		{
			for (int i=0; i<m_Max; i++)
			{
				if (m_ppMessage[i]!=NULL)
				{			
					delete [] m_ppMessage[i];			
				}
			}

			delete [] m_ppMessage;

			m_Max		= 0;
			m_Length	= 0;
			m_Current	= 0;
			m_ppMessage = NULL;
		}
	//#endif

	// file log
	if (m_bLog)
	{
		_close(m_LogFile);
		m_bLog = false;

		if (m_Filename != NULL)
		{
			delete [] m_Filename;
		}
	}
}

//----------------------------------------------------------------------
// Add 
//----------------------------------------------------------------------
// String을 추가한다. 끝에~..
//----------------------------------------------------------------------
void		
CMessageArray::Add(const char *str)
{	
	#ifndef __LOGGING__
		return;
	#endif

	__BEGIN_LOCK	

	int len = strlen(str);
	
	// file log
	if (m_bLog)
	{ 
		// [ TEST CODE ] 시간 출력
		//sprintf(g_MessageBuffer, "[%4d] ", timeGetTime() % 10000);
		//_write( m_LogFile, g_MessageBuffer, strlen(g_MessageBuffer) );

		//m_LogFile << str << endl;
		_write( m_LogFile, str, len );
		_write( m_LogFile, "\n", 1 );

		// [ TEST CODE ] 화일 닫고 다시 열기
		#ifdef OUTPUT_FILE_LOG
			_close( m_LogFile );
			m_LogFile = _open(m_Filename, _O_WRONLY | _O_TEXT | _O_APPEND | _O_CREAT);
		#endif
	}	

	if (len >= m_Length)
	{		
		for (int i=0; i<m_Length; i++)
		{
			m_ppMessage[m_Current][i] = str[i];
		}	
		m_ppMessage[m_Current][m_Length] = NULL;		
		//strcpy(m_ppMessage[m_Current], "[ERROR] CMessageArray::Add() - String too long");		
	}
	else
	{
		// 저장
		strcpy(m_ppMessage[m_Current], str);
	}

	m_Current++;
	if (m_Current==m_Max) m_Current=0;

	__END_LOCK
}

//----------------------------------------------------------------------
// Add To File
//----------------------------------------------------------------------
// File에만 추가한다. 끝에~..
//----------------------------------------------------------------------
void		
CMessageArray::AddToFile(const char *str)
{
	#ifndef __LOGGING__
		return;
	#endif

	__BEGIN_LOCK
	
	// file log
	if (m_bLog)
	{
		// [ TEST CODE ] 시간 출력
		//sprintf(g_MessageBuffer, "[%4d] ", timeGetTime() % 10000);
		//_write( m_LogFile, g_MessageBuffer, strlen(g_MessageBuffer) );

		//m_LogFile << str << endl;
		_write( m_LogFile, str, strlen( str ) );
		_write( m_LogFile, "\n", 1 );

		// [ TEST CODE ] 화일 닫고 다시 열기
		#ifdef OUTPUT_FILE_LOG
			_close( m_LogFile );
			m_LogFile = _open(m_Filename, _O_WRONLY | _O_TEXT | _O_APPEND | _O_CREAT);
		#endif
	}	

	__END_LOCK
}

//--------------------------------------------------------------------------
// Add Format VL
//--------------------------------------------------------------------------
void
CMessageArray::AddFormatVL(const char* format, va_list& vl)
{
	__BEGIN_LOCK

	//AddFormat( format, vl );
//	va_list		vl;
	static char Buffer[4096];

//    va_start(vl, format);
	vsprintf(Buffer, format, vl);    
 //   va_end(vl);

	int len = strlen(Buffer);
 
	// file log
	if (m_bLog)
	{
		// [ TEST CODE ] 시간 출력
		//sprintf(g_MessageBuffer, "[%4d] ", timeGetTime() % 10000);
		//_write( m_LogFile, g_MessageBuffer, strlen(g_MessageBuffer) );

		//m_LogFile << str << endl;
		_write( m_LogFile, Buffer, len );
		_write( m_LogFile, "\n", 1 );

		// [ TEST CODE ] 화일 닫고 다시 열기
		#ifdef OUTPUT_FILE_LOG
			_close( m_LogFile );
			m_LogFile = _open(m_Filename, _O_WRONLY | _O_TEXT | _O_APPEND | _O_CREAT);
		#endif
	}	

	// 혹시 넘어갈까봐.. (이거 심각한데. - -;;)
	if (len >= m_Length)
	{		
		for (int i=0; i<m_Length; i++)
		{
			m_ppMessage[m_Current][i] = Buffer[i];
		}	

		m_ppMessage[m_Current][m_Length] = NULL;	
	}
	else
	{
		// 저장
		strcpy(m_ppMessage[m_Current], Buffer);
	}
	

	m_Current++;
	if (m_Current==m_Max) m_Current=0;

	__END_LOCK
}

//--------------------------------------------------------------------------
// Add Format
//--------------------------------------------------------------------------
// 적절한 형식으로 string을 만든다.
//--------------------------------------------------------------------------
void
CMessageArray::AddFormat(const char* format, ...)
{
	#ifndef __LOGGING__
		return;
	#endif

	__BEGIN_LOCK
	

	va_list		vl;
	static char Buffer[4096];

    va_start(vl, format);
	vsprintf(Buffer, format, vl);    
    va_end(vl);

	int len = strlen(Buffer);
 
	// file log
	if (m_bLog)
	{
		// [ TEST CODE ] 시간 출력
		//sprintf(g_MessageBuffer, "[%4d] ", timeGetTime() % 10000);
		//_write( m_LogFile, g_MessageBuffer, strlen(g_MessageBuffer) );

		//m_LogFile << str << endl;
		_write( m_LogFile, Buffer, len );
		_write( m_LogFile, "\n", 1 );

		// [ TEST CODE ] 화일 닫고 다시 열기
		#ifdef OUTPUT_FILE_LOG
			_close( m_LogFile );
			m_LogFile = _open(m_Filename, _O_WRONLY | _O_TEXT | _O_APPEND | _O_CREAT);
		#endif
	}	

	// 혹시 넘어갈까봐.. (이거 심각한데. - -;;)
	if (len >= m_Length)
	{		
		for (int i=0; i<m_Length; i++)
		{
			m_ppMessage[m_Current][i] = Buffer[i];
		}	

		m_ppMessage[m_Current][m_Length] = NULL;	
	}
	else
	{
		// 저장
		strcpy(m_ppMessage[m_Current], Buffer);
	}
	

	m_Current++;
	if (m_Current==m_Max) m_Current=0;

	__END_LOCK
}

//----------------------------------------------------------------------
// Next
//----------------------------------------------------------------------
// Current를 next로 바꾼다..
//----------------------------------------------------------------------
void
CMessageArray::Next()
{	
	#ifndef __LOGGING__
		return;
	#endif

	__BEGIN_LOCK

	// file log
	if (m_bLog)
	{
		//m_LogFile << str << endl;
		_write( m_LogFile, m_ppMessage[m_Current], strlen( m_ppMessage[m_Current] ) );
		_write( m_LogFile, "\n", 1 );
	}	

	m_Current++; 
	if (m_Current==m_Max) m_Current=0;

	__END_LOCK
}

//----------------------------------------------------------------------
// operator []
//----------------------------------------------------------------------
// 0 ~ MAX-1
// 0이 가장 오래된 String이고 MAX-1이 가장 최근에 것으로
// return해야 한다.
//----------------------------------------------------------------------
const char*	
CMessageArray::operator [] (int i)
{ 
	//                i   = 실제로 return되어야 하는 값
	//m_Current - (3-[0]) = m_Current;
	//m_Current - (3-[1]) = m_Current - 2;
	//m_Current - (3-[2]) = m_Current - 1;

	__BEGIN_LOCK

	BYTE gap = m_Max - i;

	if (gap > m_Current)
	{

		const char* pMessage = m_ppMessage[m_Current + m_Max - gap];
		__END_LOCK

		return pMessage;
	}
	
	const char* pMessage = m_ppMessage[m_Current - gap];	

	__END_LOCK

	return pMessage;
}

//----------------------------------------------------------------------
// Clear
//----------------------------------------------------------------------
void
CMessageArray::Clear()
{
	for (int i=0; i<m_Max; i++)
	{
		m_ppMessage[i][0] = NULL;
	}
}