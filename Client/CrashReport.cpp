/********************************************************************
	created:	2003/12/01
	created:	1:12:2003   12:15
	filename: 	D:\study\smodulelib\CrashReport.cpp
	file path:	D:\study\smodulelib
	file base:	CrashReport
	file ext:	cpp
	author:		쑥갓
	
	purpose:	치명적인 오류가 발생했을때 로그를 남긴다.
*********************************************************************/
#include "Client_PCH.h"
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <imagehlp.h>
#else
#include "../../basic/Platform.h"
#endif
#include "CrashReport.h"
#include "Properties.h"
#include "MCrashReportManager.h"
#include "ServerInfo.h"

///////////////////////////////////////////////////////////////////////////////
// GetExceptionDescription
//
// Translate the exception code into something human readable
static const TCHAR *GetExceptionDescription(DWORD ExceptionCode)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		TCHAR *	ExceptionName;
	};

#if 0  // from winnt.h
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)    
#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)    
#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)    
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)    
#define STATUS_PENDING                   ((DWORD   )0x00000103L)    
#define STATUS_SEGMENT_NOTIFICATION      ((DWORD   )0x40000005L)    
#define STATUS_GUARD_PAGE_VIOLATION      ((DWORD   )0x80000001L)    
#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)    
#define STATUS_BREAKPOINT                ((DWORD   )0x80000003L)    
#define STATUS_SINGLE_STEP               ((DWORD   )0x80000004L)    
#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)    
#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)    
#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)    
#define STATUS_NO_MEMORY                 ((DWORD   )0xC0000017L)    
#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)    
#define STATUS_NONCONTINUABLE_EXCEPTION  ((DWORD   )0xC0000025L)    
#define STATUS_INVALID_DISPOSITION       ((DWORD   )0xC0000026L)    
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)    
#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)    
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)    
#define STATUS_FLOAT_INEXACT_RESULT      ((DWORD   )0xC000008FL)    
#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)    
#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)    
#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)    
#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)    
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)    
#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)    
#define STATUS_PRIVILEGED_INSTRUCTION    ((DWORD   )0xC0000096L)    
#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)    
#define STATUS_CONTROL_C_EXIT            ((DWORD   )0xC000013AL)    
#define STATUS_FLOAT_MULTIPLE_FAULTS     ((DWORD   )0xC00002B4L)    
#define STATUS_FLOAT_MULTIPLE_TRAPS      ((DWORD   )0xC00002B5L)    
#define STATUS_ILLEGAL_VLM_REFERENCE     ((DWORD   )0xC00002C0L)     
#endif

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, "a Control-C"},
		{0x40010008, "a Control-Break"},
		{0x80000002, "a Datatype Misalignment"},
		{0x80000003, "a Breakpoint"},
		{0xc0000005, "an Access Violation"},
		{0xc0000006, "an In Page Error"},
		{0xc0000017, "a No Memory"},
		{0xc000001d, "an Illegal Instruction"},
		{0xc0000025, "a Noncontinuable Exception"},
		{0xc0000026, "an Invalid Disposition"},
		{0xc000008c, "a Array Bounds Exceeded"},
		{0xc000008d, "a Float Denormal Operand"},
		{0xc000008e, "a Float Divide by Zero"},
		{0xc000008f, "a Float Inexact Result"},
		{0xc0000090, "a Float Invalid Operation"},
		{0xc0000091, "a Float Overflow"},
		{0xc0000092, "a Float Stack Check"},
		{0xc0000093, "a Float Underflow"},
		{0xc0000094, "an Integer Divide by Zero"},
		{0xc0000095, "an Integer Overflow"},
		{0xc0000096, "a Privileged Instruction"},
		{0xc00000fD, "a Stack Overflow"},
		{0xc0000142, "a DLL Initialization Failed"},
		{0xe06d7363, "a Microsoft C++ Exception"},
	};

	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return ExceptionMap[i].ExceptionName;

	return "an Unknown exception type";
}

typedef struct _tagADDRESS64 {
    DWORD64       Offset;
    WORD          Segment;
    ADDRESS_MODE  Mode;
} ADDRESS64, *LPADDRESS64;

typedef struct _KDHELP64 {

    //
    // address of kernel thread object, as provided in the
    // WAIT_STATE_CHANGE packet.
    //
    DWORD64   Thread;

    //
    // offset in thread object to pointer to the current callback frame
    // in kernel stack.
    //
    DWORD   ThCallbackStack;

    //
    // offset in thread object to pointer to the current callback backing
    // store frame in kernel stack.
    //
    DWORD   ThCallbackBStore;

    //
    // offsets to values in frame:
    //
    // address of next callback frame
    DWORD   NextCallback;

    // address of saved frame pointer (if applicable)
    DWORD   FramePointer;


    //
    // Address of the kernel function that calls out to user mode
    //
    DWORD64   KiCallUserMode;

    //
    // Address of the user mode dispatcher function
    //
    DWORD64   KeUserCallbackDispatcher;

    //
    // Lowest kernel mode address
    //
    DWORD64   SystemRangeStart;

    DWORD64  Reserved[8];

} KDHELP64, *PKDHELP64;

typedef struct _tagSTACKFRAME64 {
    ADDRESS64   AddrPC;               // program counter
    ADDRESS64   AddrReturn;           // return address
    ADDRESS64   AddrFrame;            // frame pointer
    ADDRESS64   AddrStack;            // stack pointer
    ADDRESS64   AddrBStore;           // backing store pointer
    PVOID       FuncTableEntry;       // pointer to pdata/fpo or NULL
    DWORD64     Params[4];            // possible arguments to the function
    BOOL        Far;                  // WOW far call
    BOOL        Virtual;              // is this a virtual frame?
    DWORD64     Reserved[3];
    KDHELP64    KdHelp;
} STACKFRAME64, *LPSTACKFRAME64;

typedef
BOOL
(__stdcall *PREAD_PROCESS_MEMORY_ROUTINE64)(
    HANDLE      hProcess,
    DWORD64     qwBaseAddress,
    PVOID       lpBuffer,
    DWORD       nSize,
    LPDWORD     lpNumberOfBytesRead
    );

typedef
PVOID
(__stdcall *PFUNCTION_TABLE_ACCESS_ROUTINE64)(
    HANDLE  hProcess,
    DWORD64 AddrBase
    );

typedef
DWORD64
(__stdcall *PGET_MODULE_BASE_ROUTINE64)(
    HANDLE  hProcess,
    DWORD64 Address
    );

typedef
DWORD64
(__stdcall *PTRANSLATE_ADDRESS_ROUTINE64)(
    HANDLE    hProcess,
    HANDLE    hThread,
    LPADDRESS64 lpaddr
    );

// StackWalk64()
typedef BOOL (__stdcall *tSW)( 
  DWORD MachineType, 
  HANDLE hProcess,
  HANDLE hThread, 
  LPSTACKFRAME64 StackFrame, 
  PVOID ContextRecord,
  PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
  PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
  PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
  PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
tSW pSW = NULL;

// SymFunctionTableAccess64()
typedef PVOID (__stdcall *tSFTA)( HANDLE hProcess, DWORD64 AddrBase );
tSFTA pSFTA = NULL;

// SymGetModuleBase64()
typedef DWORD64 (__stdcall *tSGMB)( IN HANDLE hProcess, IN DWORD64 dwAddr );
tSGMB pSGMB = NULL;

extern BOOL GetWinVersion(char *szVersion);

#ifdef PLATFORM_WINDOWS
LONG __stdcall RecordExceptionInfo( _EXCEPTION_POINTERS* pExp )
{
	if( g_pSocket != NULL )
	{
		g_pSocket->disconnect();
	}

	PEXCEPTION_RECORD Exception = pExp->ExceptionRecord;
	PCONTEXT          Context   = pExp->ContextRecord;

//	ofstream logFile(CRASH_LOG_FILENAME, ios::app);

	MCrashReportManager crm;
	crm.Init(1);
	MCrashReport &cr = crm[0];

	char szTemp[1024];
	GetModuleFileName(NULL, szTemp, 1024);
	SYSTEMTIME st;

	HANDLE ModuleFile = CreateFile(szTemp, GENERIC_READ,
				FILE_SHARE_READ, 0, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, 0);

	if (ModuleFile != INVALID_HANDLE_VALUE)
	{
		FILETIME LastWriteTime;
		if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
		{
			FileTimeToLocalFileTime(&LastWriteTime, &LastWriteTime);
			FileTimeToSystemTime(&LastWriteTime, &st);

			wsprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			cr.SetExcutableTime(szTemp);
		}
		CloseHandle(ModuleFile);
	}

	int version = 0;
	class ifstream versionFile(g_pFileDef->getProperty("FILE_INFO_VERSION").c_str(), ios::binary | ios::nocreate);

	if (versionFile.is_open())
	{
		versionFile.read((char*)&version, 4);
		versionFile.close();
	}
	cr.SetVersion(version);

	// print out operating system
	strcpy(szTemp, "");
	GetWinVersion(szTemp);
	cr.SetOS(szTemp);

	wsprintf(szTemp, "0x%08x", Context->Eip);
	cr.SetAddress(szTemp);

	wsprintf(szTemp, "%s(0x%08x)", GetExceptionDescription(Exception->ExceptionCode), Exception->ExceptionCode);
	cr.SetMessage(szTemp);

	std::string callStack;

	int frameNum;
	DWORD imageType = IMAGE_FILE_MACHINE_I386;

	HANDLE hThread;
	HANDLE hSWProcess = GetCurrentProcess();
	if(DuplicateHandle( hSWProcess, GetCurrentThread(), hSWProcess, &hThread, 0, false, DUPLICATE_SAME_ACCESS ) != 0)
	{

		STACKFRAME64 s; // in/out stackframe
		memset( &s, '\0', sizeof s );

		HINSTANCE hImagehlpDll = LoadLibrary( "dbghelp.dll" );
		if ( hImagehlpDll != NULL )
		{
			pSFTA = (tSFTA) GetProcAddress( hImagehlpDll, "SymFunctionTableAccess64" );
			pSGMB = (tSGMB) GetProcAddress( hImagehlpDll, "SymGetModuleBase64" );
			pSW = (tSW) GetProcAddress( hImagehlpDll, "StackWalk64" );

			if(pSFTA != NULL && pSGMB != NULL && pSW != NULL)
			{
				s.AddrPC.Offset = Context->Eip;
				s.AddrPC.Mode = AddrModeFlat;
				s.AddrFrame.Offset = Context->Ebp;
				s.AddrFrame.Mode = AddrModeFlat;

				for ( frameNum = 0; ; ++ frameNum )
				{
					if ( ! pSW( imageType, hSWProcess, hThread, &s, NULL, NULL, pSFTA, pSGMB, NULL ) )
						break;

					wsprintf(szTemp, "0x%08x", s.AddrPC.Offset);
					callStack += ' ';
					callStack += szTemp;
				}
			}
			FreeLibrary( hImagehlpDll );
		}

		CloseHandle( hThread );
	}
	cr.SetCallStack(callStack.c_str());

	crm.SaveToFile(CRASH_LOG_FILENAME);

	return EXCEPTION_EXECUTE_HANDLER;
}
#else
// Stub implementation for non-Windows platforms
LONG __stdcall RecordExceptionInfo( struct _EXCEPTION_POINTERS* pExp )
{
    // Crash reporting not implemented on non-Windows platforms
    // Just return to continue execution
    (void)pExp;  // Suppress unused parameter warning
    return 0;
}
#endif // PLATFORM_WINDOWS

//void RecordExceptionInfo( unsigned int u, _EXCEPTION_POINTERS* pExp )
//{
//	PEXCEPTION_RECORD Exception = pExp->ExceptionRecord;
//	PCONTEXT          Context   = pExp->ContextRecord;
//
//	ofstream logFile(CRASH_LOG_FILENAME, ios::app);
//
//	if(logFile.is_open())
//	{
//		char szTemp[1024];
//		WIN32_FILE_ATTRIBUTE_DATA fad;
//	 	GetModuleFileName(NULL, szTemp, 1024);
//		GetFileAttributesEx(szTemp, GetFileExInfoStandard, &fad);
//		SYSTEMTIME st;
//		FileTimeToSystemTime(&fad.ftLastWriteTime, &st);
//
//		int version = 0;
//		class ifstream versionFile(g_pFileDef->getProperty("FILE_INFO_VERSION").c_str(), ios::binary | ios::nocreate);
//
//		if (versionFile.is_open())
//		{
//			versionFile.read((char*)&version, 4);
//			versionFile.close();
//		}
//
//		// print out operating system
//		TCHAR szWinVer[512];
//		strcpy(szWinVer, "");
//		GetWinVersion(szWinVer);
//
//		// *CrashLog 2003-12-01 10:50:00 98 0xffffffff 0xffffffff에서 메모리 침범 에러
//		wsprintf(szTemp, "%04d-%02d-%02d %02d:%02d:%02d %d 0x%08x %s (0x%08x), %s", 
//			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
//			version,
//			Context->Eip,
//			GetExceptionDescription(Exception->ExceptionCode), Exception->ExceptionCode,
//			szWinVer);
////		logFile << GetExceptionDescription(Exception->ExceptionCode) << Exception->ExceptionCode << "at" << Context->Eip << endl;
//		logFile << szTemp << endl;
//
//		logFile.close();
//	}
//
//    throw SE_Exception( u );
//}
