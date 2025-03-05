//-----------------------------------------------------------------------------
// DebugInfo.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include <New.h>
#include "DebugInfo.h"
#include <stdarg.h>
//#include <io.h>
//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h> 

//extern int g_MemLogFile;

#ifdef	OUTPUT_DEBUG

	char	g_StringBuffer[256];	

//	void* PASCAL operator new ( unsigned int cb, const char* lpszFileName, int nLine)
//	{
//		void* res = ::operator new (cb);
//
////		sprintf(g_StringBuffer, "[new] [%s : %d] %d bytes in %x\n", lpszFileName, nLine, cb, res);
////		OutputDebugString(g_StringBuffer);	
//
//		//_write( g_MemLogFile, g_StringBuffer, strlen(g_StringBuffer) );
//		
//		//inline void *__cdecl operator new(size_t, void *_P)
//		//{return (_P); }
//		
//		return res;
//	}
//
//	void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine)
//	{
////		sprintf(g_StringBuffer, "[delete] [%s : %d] in %x\n", lpszFileName, nLine, p);
////		OutputDebugString(g_StringBuffer);
//
//		//_write( g_MemLogFile, g_StringBuffer, strlen(g_StringBuffer) );
//		
//		::operator delete (p);
//	}

	void	DEBUG_ADD(const char* message)
	{
		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
			_MinTrace( message );
		if (g_pDebugMessage!=NULL)
			g_pDebugMessage->Add( message );
	}

	void	DEBUG_ADD_ERR(const char* message)
	{
		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
			_MinTraceErr( message );
		if (g_pDebugMessage!=NULL)
			g_pDebugMessage->Add( message );
	}

	void	DEBUG_ADD_WAR(const char* message)
	{
		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
			_MinTraceWar( message );
		if (g_pDebugMessage!=NULL)
			g_pDebugMessage->Add( message );
	}

	void	DEBUG_CMD(int cmd, const char* message)
	{
		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
			_CmdMinTraceA( cmd, message );
		if (g_pDebugMessage!=NULL)
			g_pDebugMessage->Add( message );
	}
	
	void	DEBUG_ADD_FORMAT(const char* first, ...)
	{
		
//		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
//		if (hWnd)
//		{
//			va_list		vl;
//			va_start(vl, first);
//			vsprintf(g_StringBuffer, first, vl);
//			va_end(vl);
//			_MinTraceA(g_StringBuffer);
//		}
//		if (g_pDebugMessage!=NULL)
//		{
//			va_list		vl;
//			
//			va_start(vl, first);
//			g_pDebugMessage->AddFormatVL( first, vl );
//			va_end(vl);
//		}
	}

	void	DEBUG_ADD_FORMAT_ERR(const char* first, ...)
	{
 		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
		{
			va_list		vl;
			va_start(vl, first);
			vsprintf(g_StringBuffer, first, vl);
			va_end(vl);
			_MinTraceErrA(g_StringBuffer);
		}
		if (g_pDebugMessage!=NULL)
		{
			va_list		vl;
			
			va_start(vl, first);
			g_pDebugMessage->AddFormatVL( first, vl );
			va_end(vl);
		}
	}
	
	void	DEBUG_ADD_FORMAT_WAR(const char* first, ...)
	{
		HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
		if (hWnd)
		{
			va_list		vl;
			va_start(vl, first);
			vsprintf(g_StringBuffer, first, vl);
			va_end(vl);
			_MinTraceWarA(g_StringBuffer);
		}
		if (g_pDebugMessage!=NULL)
		{
			va_list		vl;
			
			va_start(vl, first);
			g_pDebugMessage->AddFormatVL( first, vl );
			va_end(vl);
		}
	}
	

	/*
	void PASCAL operator delete [] (void *p, LPCSTR lpszFileName, int nLine)
	{
		sprintf(g_StringBuffer, "[delete[]] [%s : %d] %d bytes in %x\n", lpszFileName, nLine, p);
		OutputDebugString(g_StringBuffer);

		::operator delete [] (p);
	}
	*/

#endif

