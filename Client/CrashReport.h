/********************************************************************
	created:	2003/12/01
	created:	1:12:2003   12:16
	filename: 	D:\study\smodulelib\CrashReport.h
	file path:	D:\study\smodulelib
	file base:	CrashReport
	file ext:	h
	author:		쑥갓
	
	purpose:	치명적인 오류가 났을때 로그를 남긴다.
*********************************************************************/

#pragma once

#ifdef PLATFORM_WINDOWS
extern LONG __stdcall RecordExceptionInfo( _EXCEPTION_POINTERS* pExp );
#else
// Declaration for non-Windows platforms (implementation in CrashReport.cpp)
LONG __stdcall RecordExceptionInfo( void* pExp );
#endif

// 로그 파일명을 바꾸고 싶으면 여길 바꾸셈
#define OLD_CRASH_LOG_FILENAME	"CrashReport.log"
#define CRASH_LOG_FILENAME		"CrashReport2.log"

#define InitCrashReport()		SetUnhandledExceptionFilter( RecordExceptionInfo )
